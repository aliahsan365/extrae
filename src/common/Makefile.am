if HAVE_MPI
SUBDIRS = MPI
endif

include $(top_srcdir)/PATHS

common_SRCS = \
 utils.c utils.h \
 events.c events.h \
 openshmem_events.c openshmem_events.h \
 new_openmp_events.h \
 timesync.c timesync.h \
 new-queue.c new-queue.h \
 extrae_vector.c extrae_vector.h \
 MurmurHash2.c MurmurHash2.h \
 intel-pebs-types.h \
 debug.h \
 common.h \
 num_hwc.h \
 types.h \
 hwc_version.h \
 xalloc.h \
 symptr.h

common_INCLUDES = -I$(COMMON_INC)

noinst_LTLIBRARIES = libcommon.la
libcommon_la_SOURCES = $(common_SRCS)
libcommon_la_CFLAGS  = $(common_INCLUDES)
libcommon_la_LDFLAGS = -lrt 

#
#noinst_PROGRAMS = common
#common_SOURCES  = $(common_SRCS)
#common_CFLAGS   = $(common_INCLUDES)
#common_LINK     = touch
#
#if DPCL_TRACING
#else
#noinst_LTLIBRARIES = libcommon.la
#libcommon_la_SOURCES = $(common_SRCS)
#libcommon_la_CFLAGS  = $(common_INCLUDES)
#endif
#
