include $(top_srcdir)/PATHS

check_PROGRAMS = dump-events

TESTS = dump-events.sh 

TESTS_ENVIRONMENT = \
  EXTRAE_HOME=$(top_builddir)

EXTRA_DIST = \
 dump-events.reference \
 dump-events.sh

if HAVE_PAPI
if !HAVE_PAPI_EMBEDDED
  PAPI_LINKER_FLAGS = @PAPI_LDFLAGS@ @PAPI_RPATH@ @PAPI_LIBS@
endif
endif
if HAVE_PMAPI
  PMAPI_LINKER_FLAGS = -lpmapi
endif
HWC_LINKER_FLAGS = $(PAPI_LINKER_FLAGS) $(PMAPI_LINKER_FLAGS)

dump_events_SOURCES = dump-events.c
dump_events_CFLAGS = -I$(INCLUDE_DIR) -I$(COMMON_INC)
dump_events_LDFLAGS = -L$(TRACER_LIB) -lseqtrace $(HWC_LINKER_FLAGS)

