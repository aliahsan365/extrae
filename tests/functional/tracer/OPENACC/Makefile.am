include $(top_srcdir)/PATHS

EXTRA_DIST = \
 openacc-check.pcf.reference \
 openacc-check.sh \
 extrae.xml

check_PROGRAMS = \
 openacc-check

TESTS = \
 openacc-check.sh

if HAVE_PAPI
if !HAVE_PAPI_EMBEDDED
  PAPI_LINKER_FLAGS = -L@PAPI_SHAREDLIBSDIR@ -R @PAPI_SHAREDLIBSDIR@ @PAPI_LIBS@
endif
endif
if HAVE_PMAPI
  PMAPI_LINKER_FLAGS = -lpmapi
endif
HWC_LINKER_FLAGS = $(PAPI_LINKER_FLAGS) $(PMAPI_LINKER_FLAGS)

openacc_check_SOURCES = openacc-check.c
openacc_check_CFLAGS = -I$(INCLUDE_DIR) -I$(SRC_DIR)/common @OPENACC_CFLAGS@
openacc_check_LDFLAGS = -L$(TRACER_LIB) -loacctrace $(HWC_LINKER_FLAGS) -L@OPENACC_SHAREDLIBSDIR@ -R @OPENACC_SHAREDLIBSDIR@ -lOpenACC
