export PATH := bin:$(PATH)
SHELL = /bin/sh
CC=gcc
CFLAGS=-std=c99 -pedantic -Os -Wall -Wextra -Werror -fwrapv
CLIBS=-lm -lrt -pthread

PROJ=.
SRC=$(PROJ)/src
INCLUDE=$(PROJ)/include
LIB=$(PROJ)/epicycle
BUILD=$(PROJ)/build
MAIN=$(PROJ)/main.c

MACROS=__MEDIUM__ POLY_DEG=5 GMAT_NDIM=13
CPPFLAGS=-I$(INCLUDE) $(MACROS:%=-D%)
LDFLAGS=-L$(LIB) -Wl,--enable-new-dtags,-R$(LIB)

BASE=log.c
MATH=vec.c quat.c mat.c dmat.c gmat.c poly.c interp.c ode.c
CORE=force_model.c
GEE=gee.c geopot.c geomag.c stdatm.c
ALL=base math core gee

epicycle.x86: $(ALL:%=$(LIB)/libepi%.so)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(MAIN) $(ALL:%=-lepi%) $(CLIBS) -o $@

$(LIB)/libepigee.so: $(GEE:%.c=$(BUILD)/%.o)
	mkdir -p $(@D)
	$(CC) -shared $(CFLAGS) $(CPPFLAGS) -fPIC $(LDFLAGS) $(CLIBS) $^ -lepibase -lepimath -o $@

$(LIB)/libepicore.so: $(CORE:%.c=$(BUILD)/%.o)
	mkdir -p $(@D)
	$(CC) -shared $(CFLAGS) $(CPPFLAGS) -fPIC $(LDFLAGS) $(CLIBS) $^ -lepibase -lepimath -o $@

$(LIB)/libepimath.so: $(MATH:%.c=$(BUILD)/%.o)
	mkdir -p $(@D)
	$(CC) -shared $(CFLAGS) $(CPPFLAGS) -fPIC $(LDFLAGS) $(CLIBS) $^ -lepibase -o $@

$(LIB)/libepibase.so: $(BASE:%.c=$(BUILD)/%.o) dummy.o
	mkdir -p $(@D)
	$(CC) -shared $(CFLAGS) $(CPPFLAGS) -fPIC $(LDFLAGS) $(CLIBS) $^ -o $@

dummy.o: dummy.c $(INCLUDE)/*.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -fPIC $(CPPFLAGS) $(CLIBS) -c $< -o $@

$(BUILD)/%.o: $(SRC)/%.c $(INCLUDE)/*.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -fPIC $(CPPFLAGS) $(CLIBS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) ./epicycle.x86 $(LIB)/*.so $(BUILD)/*.o ./*.o
