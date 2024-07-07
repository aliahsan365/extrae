/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                                   Extrae                                  *
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

#include "common.h"

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif

#include "file_set.h"
#include "object_tree.h"
#include "trace_to_prv.h"
#include "semantics.h"
#include "paraver_state.h"
#include "paraver_generator.h"
#include "addresses.h"
#include "options.h"

#include "record.h"
#include "events.h"



static int OpenACC_Call (event_t *event, unsigned long long current_time,
	unsigned int cpu, unsigned int ptask, unsigned int task,
	unsigned int thread, FileSet_t *fset)
{
	
	unsigned EvType, EvValue, state;
	UNREFERENCED_PARAMETER(fset);

	EvType  = Get_EvEvent (event);
	EvValue = Get_EvValue (event);
	
	trace_paraver_event (cpu, ptask, task, thread, current_time, EvType,EvValue);
	
	return 0;

}


SingleEv_Handler_t PRV_OpenACC_Event_Handlers[] = {
    /* Calls */
	{ OPENACC_LAUNCH_EV, OpenACC_Call},
	{ OPENACC_DATA_EV, OpenACC_Call},
	{ OPENACC_OTHERS_EV, OpenACC_Call},
	{ NULL_EV, NULL }
};

