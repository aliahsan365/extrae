/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                  MPItrace                                 *
 *              Instrumentation package for parallel applications            *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#include "common.h"

static char UNUSED rcsid[] = "$Id$";

#ifdef HAVE_SYS_TIME_H
# include <sys/time.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#include "utils.h"
#include "events.h"
#include "clock.h"
#include "threadid.h"
#include "record.h"
#include "trace_macros.h"
#include "wrapper.h"
#include "stdio.h"
#include "xml-parse.h"
#include "common_hwc.h"
#include "papi_hwc.h"
#include "hwc_version.h"

#if defined(IS_BGL_MACHINE)
# define COUNTERS_INFO
# ifdef HAVE_BGL_PERFCTR_H
#  include <bgl_perfctr.h>
# endif
# ifdef HAVE_BGL_PERFCTR_EVENTS_H
#  include <bgl_perfctr_events.h>
# endif
#endif

/*------------------------------------------------ Static Variables ---------*/

#if !defined(PAPIv3)
# error "-DNEW_HWC_SYSTEM requires PAPI v3 support"
#endif

int HWCBE_PAPI_Allocate_eventsets_per_thread (int num_set, int old_thread_num, int new_thread_num)
{
	int i;

	HWC_sets[num_set].eventsets = (int *) realloc (HWC_sets[num_set].eventsets, sizeof(int)*new_thread_num);
	if (HWC_sets[num_set].eventsets == NULL)
	{
		fprintf (stderr, PACKAGE_NAME": Cannot allocate memory for HWC_set\n");
		return FALSE;
	}

	for (i = old_thread_num; i < new_thread_num; i++)
		HWC_sets[num_set].eventsets[i] = PAPI_NULL;

	return TRUE;
}

#if defined(SAMPLING_SUPPORT)
int Add_Overflows_To_Set (int rank, int num_set, int pretended_set, 
	int num_overflows, char **counter_to_ovfs, unsigned long long *ovf_values)
{
	int cnt, i, found;
	char *strtoul_check;

	HWC_sets[num_set].OverflowCounter = (int*) malloc (sizeof(int) * num_overflows);
	if (HWC_sets[num_set].OverflowCounter == NULL)
	{
		fprintf (stderr, PACKAGE_NAME": ERROR cannot allocate memory for OverflowCounter structure at %s:%d\n",__FILE__,__LINE__);
		return FALSE;
	}
	HWC_sets[num_set].OverflowValue = (long long*) malloc (sizeof(long long) * num_overflows);
	if (HWC_sets[num_set].OverflowValue == NULL)
	{
		fprintf (stderr, PACKAGE_NAME": ERROR cannot allocate memory for OverflowValue structure at %s:%d\n",__FILE__,__LINE__);
		return FALSE;
	}
	HWC_sets[num_set].NumOverflows = num_overflows;
	for (cnt = 0; cnt < num_overflows; cnt++)
	{
		char *counter_last_position = &(counter_to_ovfs[cnt][strlen(counter_to_ovfs[cnt])]);
	
		/* Convert this counter into a code */
		HWC_sets[num_set].OverflowCounter[cnt] = strtoul (counter_to_ovfs[cnt], &strtoul_check, 16);
		if (strtoul_check != counter_last_position)
		{
			int EventCode;
			if (PAPI_event_name_to_code(counter_to_ovfs[cnt], &EventCode) != PAPI_OK)
			{
				if (rank == 0)
					fprintf (stderr, PACKAGE_NAME": Cannot parse HWC %s in set %d for sampling, skipping\n", counter_to_ovfs[cnt], pretended_set);
				HWC_sets[num_set].OverflowCounter[cnt] = NO_COUNTER;
			}
			else
				HWC_sets[num_set].OverflowCounter[cnt] = EventCode;
		}

		/* Check if this counter code is in the HWC_set */
		if (HWC_sets[num_set].OverflowCounter[cnt] != NO_COUNTER)
		{
			for (found = FALSE, i = 0; i < HWC_sets[num_set].num_counters; i++)
				found |= (HWC_sets[num_set].counters[i] == HWC_sets[num_set].OverflowCounter[cnt]);

			if (!found)
			{
				HWC_sets[num_set].OverflowCounter[cnt] = NO_COUNTER;
				if (rank == 0)
					fprintf (stderr, PACKAGE_NAME": Sampling counter %s is not in available in set\n", counter_to_ovfs[cnt]);
				/* return FALSE; */
			}
		}

		HWC_sets[num_set].OverflowValue[cnt] = ovf_values[cnt];
	
		if (rank == 0)
			fprintf (stdout, PACKAGE_NAME": HWC set %d sampling counter %s (0x%08x) every %lld events.\n", pretended_set, counter_to_ovfs[cnt], HWC_sets[num_set].OverflowCounter[cnt], ovf_values[cnt]);
	}


	return TRUE;
}
#endif

int HWCBE_PAPI_Add_Set (int pretended_set, int rank, int ncounters, char **counters,
	char *domain, char *change_at_globalops, char *change_at_time, 
	int num_overflows, char **overflow_counters, unsigned long long *overflow_values)
{
	int i, rc, num_set = HWC_num_sets;
	PAPI_event_info_t info;

#if !defined(SAMPLING_SUPPORT)
	UNREFERENCED_PARAMETER(num_overflows);
	UNREFERENCED_PARAMETER(overflow_counters);
	UNREFERENCED_PARAMETER(overflow_values);
#endif
	
	if (ncounters == 0 || counters == NULL)
		return 0;
	
	if (ncounters > MAX_HWC)
	{
		fprintf (stderr, PACKAGE_NAME": You cannot provide more HWC counters than %d (see set %d)\n", MAX_HWC, pretended_set);
		ncounters = MAX_HWC;
	}
	
	HWC_sets = (struct HWC_Set_t *) realloc (HWC_sets, sizeof(struct HWC_Set_t)* (HWC_num_sets+1));
	if (HWC_sets == NULL)
	{
		fprintf (stderr, PACKAGE_NAME": Cannot allocate memory for HWC_set (rank %d)\n", rank);
		return 0;
	}

	/* Initialize this set */
	HWC_sets[num_set].num_counters = 0;
	HWC_sets[num_set].eventsets = NULL;
#if defined(SAMPLING_SUPPORT)
	HWC_sets[num_set].OverflowCounter = NULL;
	HWC_sets[num_set].OverflowValue = NULL;
	HWC_sets[num_set].NumOverflows = 0;
#endif

	for (i = 0; i < ncounters; i++)
	{
		/* counter_last_position will hold the address of the end of the 
		   counter[i] string 
		   This shall be compared with strtoul_check to know if the hex
		   is correct or not
		*/
		char *counter_last_position = &(counters[i][strlen(counters[i])]);
		char *strtoul_check;

		HWC_sets[num_set].counters[HWC_sets[num_set].num_counters] = 
			strtoul (counters[i], &strtoul_check, 16);

		if (strtoul_check != counter_last_position)
		{
			int EventCode;
			if (PAPI_event_name_to_code(counters[i], &EventCode) != PAPI_OK)
			{
				if (rank == 0)
					fprintf (stderr, PACKAGE_NAME": Cannot parse HWC %s in set %d, skipping\n", counters[i], pretended_set);
			}
			else
			{
				HWC_sets[num_set].counters[HWC_sets[num_set].num_counters] = EventCode;
			}
		}

		rc = PAPI_get_event_info (HWC_sets[num_set].counters[HWC_sets[num_set].num_counters], &info);
		if (rc != PAPI_OK)
		{
			if (rank == 0)
				fprintf (stderr, PACKAGE_NAME": Error! Cannot query information for hardware counter %s (0x%08x). Check set %d.\n", counters[i], HWC_sets[num_set].counters[HWC_sets[num_set].num_counters], pretended_set);

			HWC_sets[num_set].counters[HWC_sets[num_set].num_counters] = NO_COUNTER;
		}
		/* Native events seem that could have info.count = 0! */
		else if (rc == PAPI_OK && info.count == 0 && (HWC_sets[num_set].counters[HWC_sets[num_set].num_counters] & PAPI_NATIVE_MASK) == 0)
		{
			if (rank == 0)
				fprintf (stderr, PACKAGE_NAME": Error! Hardware counter %s (0x%08x) is not available. Check set %d.\n", counters[i], HWC_sets[num_set].counters[HWC_sets[num_set].num_counters], pretended_set);

			HWC_sets[num_set].counters[HWC_sets[num_set].num_counters] = NO_COUNTER;
		}
		else 
			HWC_sets[num_set].num_counters++;
	}
	
	if (HWC_sets[num_set].num_counters == 0)
	{
		if (rank == 0)
			fprintf (stderr, PACKAGE_NAME": Set %d of counters seems to be empty/invalid, skipping\n", pretended_set);
		return 0;
	}

	/* Just check if the user wants us to change the counters in some manner */
	if (change_at_time != NULL)
	{
		HWC_sets[num_set].change_at = getTimeFromStr (change_at_time, 
			"change-at-time", rank);
		HWC_sets[num_set].change_type = 
				(HWC_sets[num_set].change_at == 0)?CHANGE_NEVER:CHANGE_TIME;
	}
	else if (change_at_globalops != NULL)
	{
		HWC_sets[num_set].change_at = strtoul (change_at_globalops, (char **) NULL, 10);
		HWC_sets[num_set].change_type = 
			(HWC_sets[num_set].change_at == 0)?CHANGE_NEVER:CHANGE_GLOPS;
	}
	else
		HWC_sets[num_set].change_type = CHANGE_NEVER;
	
	if (domain != NULL)
	{
		if (!strcasecmp(domain, "all"))
		{
			if (rank == 0)
				fprintf (stdout, PACKAGE_NAME": PAPI domain set to ALL for HWC set %d\n",
					pretended_set);
			HWC_sets[num_set].domain = PAPI_DOM_ALL;
		}	
		else if (!strcasecmp(domain, "kernel"))
		{
			if (rank == 0)
				fprintf (stdout, PACKAGE_NAME": PAPI domain set to KERNEL for HWC set %d\n",
					pretended_set);
			HWC_sets[num_set].domain = PAPI_DOM_KERNEL;
		}	
		else if (!strcasecmp(domain, "user"))
		{
			if (rank == 0)
				fprintf (stdout, PACKAGE_NAME": PAPI domain set to USER for HWC set %d\n",
					pretended_set);
			HWC_sets[num_set].domain = PAPI_DOM_USER;
		}	
		else if (!strcasecmp(domain, "other"))
		{
			if (rank == 0)
				fprintf (stdout, PACKAGE_NAME": PAPI domain set to OTHER for HWC set %d\n",
					pretended_set);
			HWC_sets[num_set].domain = PAPI_DOM_OTHER;
		}	
		else
		{
			if (rank == 0)
				fprintf (stdout, PACKAGE_NAME": PAPI domain set to USER for HWC set %d\n",
					pretended_set);
			HWC_sets[num_set].domain = PAPI_DOM_USER;
		}
	} /* domain != NULL */
	else
	{
		if (rank == 0)
			fprintf (stdout, PACKAGE_NAME": PAPI domain set to USER for HWC set %d\n",
				pretended_set);
		HWC_sets[num_set].domain = PAPI_DOM_USER;
	}

	HWCBE_PAPI_Allocate_eventsets_per_thread (num_set, 0, Backend_getNumberOfThreads());

	/* We validate this set */
	HWC_num_sets++;

	if (rank == 0)
	{
		fprintf (stdout, PACKAGE_NAME": HWC set %d contains following counters < ", pretended_set);
		for (i = 0; i < HWC_sets[num_set].num_counters; i++)
		{
			if (HWC_sets[num_set].counters[i] != NO_COUNTER)
			{
				char counter_name[PAPI_MAX_STR_LEN];

				PAPI_event_code_to_name (HWC_sets[num_set].counters[i], counter_name);
				fprintf (stdout, "%s (0x%08x) ", counter_name, HWC_sets[num_set].counters[i]);
			}
		}
		fprintf (stdout, ">");

		if (HWC_sets[num_set].change_type == CHANGE_TIME)
			fprintf (stdout, " - changing every %lld nanoseconds\n", HWC_sets[num_set].change_at);
		else if (HWC_sets[num_set].change_type == CHANGE_GLOPS)
			fprintf (stdout, " - changing every %lld global operations\n", HWC_sets[num_set].change_at);
		else
			fprintf (stdout, " - never changes\n");

		fflush (stdout);
	}

#if defined(SAMPLING_SUPPORT)
	if (num_overflows > 0)
		Add_Overflows_To_Set (rank, num_set, pretended_set, num_overflows,
			overflow_counters, overflow_values);
#endif

	return HWC_sets[num_set].num_counters;
}

#if defined(SAMPLING_SUPPORT)
void sampling_handler (int EventSet, void *address, long_long overflow_vector, void *context)
{
	UNREFERENCED_PARAMETER(overflow_vector);
	UNREFERENCED_PARAMETER(context);
	UNREFERENCED_PARAMETER(EventSet);

	if (isSamplingEnabled())
	{
		iotimer_t temps = TIME;
		SAMPLE_EVENT_HWC (temps, SAMPLING_EV, (unsigned long long) address);
		trace_callers (temps, 7, CALLER_SAMPLING);
	}
}
#endif

int HWCBE_PAPI_Start_Set (UINT64 time, int numset, int threadid)
{
#if defined(SAMPLING_SUPPORT)
	int i;
#endif
	int rc;

	/* The given set is a valid one? */
	if (numset < 0 || numset >= HWC_num_sets)
		return FALSE;

	HWC_current_changeat = HWC_sets[numset].change_at;
	HWC_current_changetype = HWC_sets[numset].change_type;
	HWC_current_timebegin[threadid] = time;

	/* Mark this counter set as the current set */
	HWCEVTSET(threadid) = HWC_sets[numset].eventsets[threadid];

#if defined(SAMPLING_SUPPORT)
	for (i = 0; i < HWC_sets[numset].NumOverflows; i++)
	{
		if (HWC_sets[numset].OverflowCounter[i] != NO_COUNTER)
		{
			rc = PAPI_overflow (HWCEVTSET(threadid), HWC_sets[numset].OverflowCounter[i], HWC_sets[numset].OverflowValue[i], 0, sampling_handler);
			if (rc < 0)
			{
				setSamplingEnabled (FALSE);
				fprintf (stderr, PACKAGE_NAME": PAPI_overflow failed for thread %d - counter %x!\n", threadid, HWC_sets[numset].OverflowCounter[i]);
			}
			else
				setSamplingEnabled (TRUE);
		}
	}
#endif

	rc = PAPI_start (HWCEVTSET(threadid));
 	if (rc == PAPI_OK)
	{
		TRACE_EVENT (time, HWC_CHANGE_EV, numset);

#if defined(SAMPLING_SUPPORT)
		if (HWC_sets[numset].NumOverflows > 0)
		{
			long long overflow_values[MAX_HWC];

			HARDWARE_COUNTERS_OVERFLOW(HWC_sets[numset].num_counters, 
			                           HWC_sets[numset].counters, 
			                           HWC_sets[numset].NumOverflows, 
			                           HWC_sets[numset].OverflowCounter,
			                           overflow_values);

			TRACE_EVENT_AND_GIVEN_COUNTERS (time, HWC_SET_OVERFLOW_EV, 0, MAX_HWC, overflow_values);
		}
#endif
	}
	else
	{
		fprintf (stderr, PACKAGE_NAME": PAPI_start failed to start eventset %d on thread %d! (error = %d)\n", numset+1, threadid, rc);
		if (rc == PAPI_ESYS)
		{
			perror ("PAPI_start");
			fprintf (stderr, PACKAGE_NAME": errno = %d\n", errno);
		}
	}

	return rc == PAPI_OK;
}

int HWCBE_PAPI_Stop_Set (UINT64 time, int numset, int threadid)
{
	long long values[MAX_HWC];
	int rc;

	UNREFERENCED_PARAMETER(time);

	if (numset < 0 || numset >= HWC_num_sets)
		return FALSE;

	rc = PAPI_stop (HWC_sets[numset].eventsets[threadid], values);
	if (rc != PAPI_OK)
	{
		fprintf (stderr, PACKAGE_NAME": PAPI_stop failed for thread %d! (error = %d)\n", threadid, rc);
	}
	else if (rc == PAPI_ESYS)
	{
		perror ("PAPI_stop");
		fprintf (stderr, PACKAGE_NAME": errno = %d\n", errno);
	}

	return rc == PAPI_OK;
}

/******************************************************************************
 **      Function name : PAPI_Initialize
 **
 **      Description :
 ******************************************************************************/

void HWCBE_PAPI_Initialize (int TRCOptions)
{
	UNREFERENCED_PARAMETER(TRCOptions);

	int rc;
	void *thread_identifier_function;

	/* PAPI initialization */
	rc = PAPI_library_init (PAPI_VER_CURRENT);
	if (rc != PAPI_VER_CURRENT)
	{
		if (rc > 0)
		{
			fprintf (stderr,
				PACKAGE_NAME": PAPI library version mismatch!\n"
				"          MPItrace is compiled against PAPI v%d.%d , and \n"
				"          PAPI_library_init reported v%d.%d ,\n"
				"          Check that LD_LIBRARY_PATH points to the correct PAPI library.\n",
				PAPI_VERSION_MAJOR(PAPI_VER_CURRENT),
				PAPI_VERSION_MINOR(PAPI_VER_CURRENT),
				PAPI_VERSION_MAJOR(rc),
				PAPI_VERSION_MINOR(rc));
		}
		fprintf (stderr, PACKAGE_NAME": Can't use hardware counters!\n");
		fprintf (stderr, PACKAGE_NAME": PAPI library error: %s\n", PAPI_strerror (rc));

		if (rc == PAPI_ESYS)
			perror (" mpitrace: PAPI system error is ");

		return;
	}

#if defined(SAMPLING_SUPPORT)
	/* Use any kind of sampling -- software or hardware */
	SamplingSupport = TRUE;
#endif

	thread_identifier_function = get_trace_thread_number_function();

	if (thread_identifier_function != NULL)
	{
		if ((rc = PAPI_thread_init ((unsigned long (*)(void)) thread_identifier_function)) != PAPI_OK)
		{
			fprintf (stderr, PACKAGE_NAME": PAPI_thread_init failed! Reason: %s\n", PAPI_strerror(rc));
			return;
		}
	}
}

int HWCBE_PAPI_Init_Thread (UINT64 time, int threadid)
{
	int i, j, rc;
	PAPI_option_t options;

	/* If there aren't configured sets, or the thread is already init'ed, leave */
	if (HWC_num_sets <= 0 || HWC_Thread_Initialized[threadid])
		return FALSE;

	memset (&options, 0, sizeof(options));

	for (i = 0; i < HWC_num_sets; i++)
	{
		/* Create the eventset. Each thread will create its own eventset */
		rc = PAPI_create_eventset (&(HWC_sets[i].eventsets[threadid]));
		if (PAPI_OK != rc)
		{
			fprintf (stderr, PACKAGE_NAME": Error! Unable to create eventset (%d of %d) in thread %d\n", i+1, HWC_num_sets, threadid);
			continue;
		}

		/* Set the domain for these eventsets */
		options.domain.eventset = HWC_sets[i].eventsets[threadid];
		options.domain.domain = HWC_sets[i].domain;
		PAPI_set_opt (PAPI_DOMAIN, &options);
		
		/* Add the selected counters */
		for (j = 0; j < HWC_sets[i].num_counters; j++)
		{
			if (HWC_sets[i].counters[j] != NO_COUNTER)
			{
				rc = PAPI_add_event (HWC_sets[i].eventsets[threadid], HWC_sets[i].counters[j]);
				if (rc != PAPI_OK)
				{
					char EventName[PAPI_MAX_STR_LEN];

					PAPI_event_code_to_name (HWC_sets[i].counters[j], EventName);
					fprintf (stderr, PACKAGE_NAME": Error! Hardware counter %s (0x%08x) cannot be added in set %d (thread %d)\n", EventName, HWC_sets[i].counters[j], i+1, threadid);
					HWC_sets[i].counters[j] = NO_COUNTER;
					/* break; */
				}
			}
		}
	}

	HWC_Thread_Initialized[threadid] = HWCBE_PAPI_Start_Set (time, HWC_current_set[threadid], threadid);

	return HWC_Thread_Initialized[threadid];
}

int HWCBE_PAPI_Read (unsigned int tid, long long *store_buffer)
{
	int EventSet = HWCEVTSET(tid);

	if (PAPI_read(EventSet, store_buffer) != PAPI_OK)
	{
		fprintf (stderr, PACKAGE_NAME": PAPI_read failed for thread %d evtset %d (%s:%d)\n",
			tid, EventSet, __FILE__, __LINE__);
		return 0;
	}
	return 1;
}

int HWCBE_PAPI_Reset (unsigned int tid)
{
	if (PAPI_reset(HWCEVTSET(tid)) != PAPI_OK)
	{
		fprintf (stderr, PACKAGE_NAME": PAPI_reset failed for thread %d evtset %d (%s:%d)\n", \
			tid, HWCEVTSET(tid), __FILE__, __LINE__);
		return 0;
	}
	return 1;
}

int HWCBE_PAPI_Accum (unsigned int tid, long long *store_buffer)
{
	if (PAPI_accum(HWCEVTSET(tid), store_buffer) != PAPI_OK)
	{
		fprintf (stderr, PACKAGE_NAME": PAPI_accum failed for thread %d evtset %d (%s:%d)\n", \
			tid, HWCEVTSET(tid), __FILE__, __LINE__);
		return 0;		
	}
	return 1;
}
