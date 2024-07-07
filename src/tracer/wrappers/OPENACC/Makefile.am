include $(top_srcdir)/PATHS

# Wrappers for openacc instrumentation
WRAPPERS_OPENACC = \
 openacc_wrapper.c openacc_wrapper.h \
 openacc_common.c openacc_common.h \
 openacc_probe.c openacc_probe.h

noinst_LTLIBRARIES  = libwrap_openacc.la

libwrap_openacc_la_CFLAGS = \
 -I$(COMMON_MPI_DIR) \
 -I$(TRACER_INC) \
 -I$(HWC_INC) \
 -I$(CLOCKS_INC) \
 -I$(INTERFACES_INC)/API \
 -I$(WRAPPERS_INC)/API \
 -I$(top_srcdir) \
 -I$(top_srcdir)/include \
 -I$(COMMON_INC) \
 $(UNWIND_CFLAGS) \
-I$(OPENACC_HOME)/include

libwrap_openacc_la_SOURCES = $(WRAPPERS_OPENACC)