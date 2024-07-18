#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "config.h"
#include "util.h"
#include "log.h"
#include "interp.h"
#include "ode.h"
#include "shared_data.h"
#include "data_model.h"
#include "force_model.h"
#include "gee.h"
#include "geopot.h"
#include "geomag.h"
#include "stdatm.h"

enum log_e noise_level;
char* file_name;

ode_meth_t ode_meth = NULL;
struct force_model_s force_model = {
    .size=0,
    .accum_fun=apply_force_model,
    .abstol={
        .r_bar={1.0e-3, 1.0e-3, 1.0e-3},
        .q={M_ARCMIN / 2, M_ARCMIN / 2, M_ARCMIN / 2, M_ARCMIN / 2},
        .v_bar={1.0e-3, 1.0e-3, 1.0e-3},
        .om_bar={M_ARCMIN, M_ARCMIN, M_ARCMIN}
    },
    .reltol={
        .r_bar={1.0e-9, 1.0e-9, 1.0e-9},
        .q={M_ARCSEC / 2, M_ARCSEC / 2, M_ARCSEC / 2, M_ARCSEC / 2},
        .v_bar={1.0e-6, 1.0e-6, 1.0e-6},
        .om_bar={M_ARCSEC, M_ARCSEC, M_ARCSEC}
    }
};

/* handle keyboard interrupt
 * :param int signal: received signal
 */
volatile sig_atomic_t last_signal = 0;
void handle_signal(int signal) {
    last_signal = signal;
}

int setup(int argc, char ** argv) {
    char* temp = getenv("NOISE_LEVEL");
    if (temp == NULL) noise_level = NOISE_LEVEL;
    else              noise_level = atoi(temp);
    
    static struct option longopts[] = {
        {"verbose", no_argument,       NULL, 'v'},
        {"quiet",   no_argument,       NULL, 'q'},
        {"method",  required_argument, NULL, 'm'},
        {"gee",     no_argument,       NULL, 'g'},
        {"geopot",  no_argument,       NULL,  0 },
        {"geomag",  no_argument,       NULL,  0 },
        {"geoall",  no_argument,       NULL,  0 },
        {"stdatm",  no_argument,       NULL,  0 },
        {"adapt",   no_argument,       NULL, 'a'},
        {0,         0,                 0,     0 }
    };
    int longindex = 0;

    int c;
    do {
        c = getopt_long(argc, argv, "vqm:ga", longopts, &longindex);
        switch (c) {
        case 0:
            if (!strcmp(longopts[longindex].name, "gee")) {
                LOG_WARNING("gee: `true`");
                force_model.size = MAX(1, force_model.size);
                force_model.fun_lst[0] = gee_fast;
            } else if (!strcmp(longopts[longindex].name, "stdatm")) {
                LOG_WARNING("stdatm: `true`");
                force_model.size = MAX(2, force_model.size);
                force_model.fun_lst[1] = stdatm;
            } else if (!strcmp(longopts[longindex].name, "geopot")) {
                LOG_WARNING("geopot: `true`");
                force_model.size = MAX(3, force_model.size);
                force_model.fun_lst[2] = geopot;
            } else if (!strcmp(longopts[longindex].name, "geomag")) {
                LOG_WARNING("geomag: `true`");
                force_model.size = MAX(5, force_model.size);
                force_model.fun_lst[3] = geomag;
                force_model.fun_lst[4] = em;
            } else if (!strcmp(longopts[longindex].name, "geoall")) {
                LOG_WARNING("geoall: `true`");
                force_model.size = MAX(4, force_model.size);
                force_model.fun_lst[2] = geoall;
                force_model.fun_lst[3] = em;
                force_model.fun_lst[4] = NULL;
            } else if (!strcmp(longopts[longindex].name, "adapt"))
                force_model.step_fun = adjust_time_step;
            break;
        case 'v':
            if (noise_level > E_DEBUG) 
                noise_level -= LOG_LEVEL_UP;
            break;
        case 'q':
            if (noise_level < E_ERROR) 
                noise_level += LOG_LEVEL_UP;
            break;
        case 'm':
            if (!strcmp(optarg, "verlet"))
                ode_meth = ODE_METHOD_NAME(verlet);
            else if (!strcmp(optarg, "rk4"))
                ode_meth = ODE_METHOD_NAME(rk4);
            else if (!strcmp(optarg, "dopri"))
                ode_meth = ODE_METHOD_NAME(dopri);
            else if (!strcmp(optarg, "vgl4"))
                ode_meth = ODE_METHOD_NAME(vgl4);
            else if (!strcmp(optarg, "vgl6"))
                ode_meth = ODE_METHOD_NAME(vgl6);
            break;
        case 'a':
            force_model.step_fun = adjust_time_step;
            break;
        case 'g':
            LOG_WARNING("gee: `true`");
            force_model.size = MAX(1, force_model.size);
            force_model.fun_lst[0] = gee_fast;
            break;
        case -1:
            break;
        default:
            return -1;
        }
    } while (c != -1);
    if (optind < argc) file_name = argv[optind];
    else               file_name = FILE_NAME;
    return 0;
}

/* application main function
 * :returns int: error code
 */
int main(int argc, char ** argv) {
    signal(SIGINT, handle_signal);
    
    if (setup(argc, argv) != 0) return EXIT_FAILURE;
    LOG_INFO("noise level: `%d`", noise_level);
    LOG_INFO("file name: `%s`", file_name);
    
    int flag = O_RDWR | O_CREAT | O_TRUNC,
        prot = PROT_READ | PROT_WRITE,
        len  = sizeof(struct shared_data_s) + sizeof(struct data_model_s);
    
    int fd = shm_open(file_name, flag, S_IRUSR | S_IWUSR);
    if (fd < 0)
        goto shm_open_failed;
    else if (ftruncate(fd, len) < 0)
        goto ftruncate_or_mmap_failed;
         
    struct shared_data_s* shared_data = mmap(NULL, len, prot, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
        goto ftruncate_or_mmap_failed;
    
    if ((sem_init(&shared_data->sem1, 1, 0) < 0) ||
        (sem_init(&shared_data->sem2, 1, 1) < 0))
        goto sem_init_failed;

    shared_data->size = sizeof(struct data_model_s);
    struct data_model_s* data_model = (struct data_model_s*) &shared_data->data;
    memset(data_model, 0, sizeof(struct data_model_s));
    size_t* size = &data_model->size;
    struct cfg_s* cfg = &data_model->cfg;
    struct st_s swap[3];
    struct st_s *st = &data_model->st, *prev = &swap[0], *next = &swap[1], *curr = &swap[2];
    struct ch_s* ch = &data_model->ch;
    struct in_s* in = &data_model->in;
    struct out_s* out = &data_model->out;
    struct em_s* em = &data_model->em;
    
    interp_init();
    stdatm_init();

    bool first = true;
    while (last_signal != SIGINT) {  // TODO exit condition
        if (sem_wait(&shared_data->sem1) != 0)
            continue;
        RESET_STATS();
        START_CLOCK();
        if (first) {
            memcpy(next, st, sizeof(struct st_s));
            first = false;
        }
        LOG_INFO("delta_t: %f", cfg->clk.delta_t);
        while (ch->clk.t > next->clk.t) {
            SWAP(&prev, &next);
            solve_em(*size, cfg, em);
            do solve_st_dot(*size, cfg, prev, next, in);
            while (
                !solve_ivp(
                    prev->clk.t, (double*) &prev->sys,
                    next->clk.t, (double*) &next->sys,
                    ode_meth,  // default `ode_meth_t`
                    force_model.accum_fun,
                    force_model.step_fun,
                    force_model.abstol.st,
                    force_model.reltol.st, 
                    9, *size, cfg, prev, next, curr, in, out, em, &force_model
                )
            );
            quat_unit(next->sys.q, next->sys.q); // XXX hack
            next->clk.n = prev->clk.n + 1;
        }
        curr->clk.n = MAX(st->clk.n + 1, next->clk.n);
        curr->clk.t = ch->clk.t;
        interp_st(*size, prev, next, curr);
        memcpy(st, curr, sizeof(struct st_s));
        if (solve_ch(*size, ch, st, curr, in, em)) {
            solve_st_delta(*size, cfg, curr, st, out);
            memcpy(next, st, sizeof(struct st_s));
        } else
            solve_out(*size, cfg, curr, out);
        STOP_CLOCK();
        SHOW_STATS();
        sem_post(&shared_data->sem2);
    }

    sem_destroy(&shared_data->sem1);
    sem_destroy(&shared_data->sem2);
sem_init_failed:
    munmap(shared_data, sizeof(struct shared_data_s) + sizeof(struct data_model_s));
ftruncate_or_mmap_failed:
    shm_unlink(file_name);
shm_open_failed:
    if (errno) LOG_WARNING("[%d] %s", errno, strerror(errno));
    signal(SIGINT, SIG_DFL);
    return EXIT_SUCCESS;
}

