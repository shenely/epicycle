#define PY_SSIZE_T_CLEAN
#include <stddef.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <Python.h>
#include <structmember.h>
#include "log.h"
#include "shared_data.h"
#include "data_model.h"

enum log_e noise_level = E_DEBUG;

typedef struct {
    PyObject_HEAD
    /* public */
    char* filename;
    char mode;
    /* private */
    int fd;
    struct shared_data_s* shared_data;
} EpicycleConsoleObject;

static int
EpicycleConsole_init(
    EpicycleConsoleObject* self,
    PyObject* args,
    PyObject* kwargs
) {
    static char* kwlist[] = {"filename", "mode", NULL};
    self->mode = 'w';
    if (!PyArg_ParseTupleAndKeywords(
        args, kwargs, "es|C", kwlist, NULL,
        &self->filename, &self->mode
    ))  return -1;
    return 0;
}

static void
EpicycleConsole_dealloc(
    EpicycleConsoleObject* self
){
    PyMem_Free(self->filename);
    Py_TYPE(self)->tp_free((PyObject*) self);
}

static int
EpicycleConsole_getbuffer(
    EpicycleConsoleObject* self,
    Py_buffer* view,
    int flags
) {
    static ssize_t shape = sizeof(struct data_model_s),
                   strides = 1;
    view->buf        = self->shared_data->data;
    view->len        = sizeof(struct data_model_s);
    view->readonly   = self->mode != 'w';
    view->itemsize   = 1;
    view->format     = NULL;
    view->ndim       = 1;
    view->shape      = &shape;
    view->strides    = &strides;
    view->suboffsets = NULL;
    view->obj        = (PyObject*) self;
    Py_INCREF(self);
    return 0;
}

static PyBufferProcs
EpicycleConsole_as_buffer = {
    .bf_getbuffer = (getbufferproc) EpicycleConsole_getbuffer
};

static PyMemberDef
EpicycleConsole_members[] = {
    {"filename", T_STRING, offsetof(EpicycleConsoleObject, filename), READONLY},
    {"mode",     T_CHAR,   offsetof(EpicycleConsoleObject, mode),     READONLY},
    {NULL}  /* Sentinel */
};

static EpicycleConsoleObject*
EpicycleConsole_enter(
    EpicycleConsoleObject* self,
    PyObject* Py_UNUSED(args)
) {
    if (sem_wait(&self->shared_data->sem2) < 0) {
        PyErr_SetFromErrno(PyExc_Exception);
        return NULL;
    }
    Py_INCREF(self);
    return self;
}

static PyObject*
EpicycleConsole_exit(
    EpicycleConsoleObject* self,
    PyObject* Py_UNUSED(args)
) {
    if (sem_post(&self->shared_data->sem1) < 0) {
        PyErr_SetFromErrno(PyExc_Exception);
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyObject*
EpicycleConsole_open(
    EpicycleConsoleObject* self
) {
    int flag =              (self->mode == 'w') ? O_RDWR     : O_RDONLY,
        prot = PROT_READ | ((self->mode == 'w') ? PROT_WRITE : PROT_NONE),
        size = sizeof(struct shared_data_s) + sizeof(struct data_model_s);

    self->fd = shm_open(self->filename, flag, 0);
    if (self->fd < 0)
        goto shm_open_failed;

    self->shared_data = mmap(NULL, size, prot, MAP_SHARED, self->fd, 0);
    if (self->shared_data == MAP_FAILED)
        goto mmap_failed;

    Py_INCREF(self);
    return (PyObject*) self;

mmap_failed:
    close(self->fd);
shm_open_failed:
    PyErr_SetFromErrno(PyExc_Exception);
    Py_RETURN_NONE;
}

static PyObject*
EpicycleConsole_close(
    EpicycleConsoleObject* self
) {
    if ((intptr_t) self->shared_data > 0)
        munmap(self->shared_data, sizeof(struct shared_data_s));
    if (self->fd > 0)
        close(self->fd);
    Py_RETURN_NONE;
}

static PyMethodDef
EpicycleConsole_methods[] = {
    {"__enter__", (PyCFunction) EpicycleConsole_enter, METH_NOARGS},
    {"__exit__",  (PyCFunction) EpicycleConsole_exit,  METH_VARARGS},
    {"open",      (PyCFunction) EpicycleConsole_open,  METH_NOARGS},
    {"close",     (PyCFunction) EpicycleConsole_close, METH_NOARGS},
    {NULL}  /* Sentinel */
};

static PyTypeObject
EpicycleConsoleType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name      = "_epicycle.EpicycleConsole",
    .tp_basicsize = sizeof(EpicycleConsoleObject),
    .tp_itemsize  = 0,
    .tp_flags     = Py_TPFLAGS_DEFAULT,
    .tp_new       = PyType_GenericNew,
    .tp_init      = (initproc) EpicycleConsole_init,
    .tp_dealloc   = (destructor)   EpicycleConsole_dealloc,
    .tp_as_buffer =               &EpicycleConsole_as_buffer,
    .tp_members   =                EpicycleConsole_members,
    .tp_methods   =                EpicycleConsole_methods
};

static PyModuleDef EpicycleModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_epicycle",
    .m_doc = "Circles within circles...",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit__epicycle(void) {
    if (PyType_Ready(&EpicycleConsoleType) < 0)
        return NULL;

    PyObject* m = PyModule_Create(&EpicycleModule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&EpicycleConsoleType);
    if (PyModule_AddObject(
        m, "EpicycleConsole", (PyObject*) &EpicycleConsoleType) < 0
    ) {
        Py_DECREF(&EpicycleConsoleType);
        return NULL;
    }
    
    return m;
}

