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
#ifdef HAVE_BFD
# include "addr2info.h"
#endif

#include <unistd.h>
#include <fcntl.h>

#include "events.h"
#include "mpi2out.h"
#include "options.h"




#define OPENACC_LAUNCH_INDEX                           0
#define OPENACC_DATA_INDEX                             1
#define OPENACC_OTHERS_INDEX                           2

#define MAX_OPENACC_INDEX                              3

static int inuse[MAX_OPENACC_INDEX] = { FALSE };


void Enable_OpenACC_Operation(int type) {
	if (type == OPENACC_LAUNCH_EV)
		inuse[OPENACC_LAUNCH_INDEX] = TRUE;
	else if (type == OPENACC_DATA_EV)
		inuse[OPENACC_DATA_INDEX] = TRUE;
	else if (type == OPENACC_OTHERS_EV)
		inuse[OPENACC_OTHERS_INDEX] = TRUE;
	
	
}

#if defined(PARALLEL_MERGE)

#include <mpi.h>
#include "mpi-aux.h"

void Share_OpenACC_Operations(void ) {
    
	int res, i, tmp[MAX_OPENACC_INDEX];

	res = MPI_Reduce (inuse, tmp, MAX_OPENACC_INDEX, MPI_INT, MPI_BOR, 0, MPI_COMM_WORLD);

	MPI_CHECK(res, MPI_Reduce, "While sharing OpenACC enabled operations");

    for (i = 0; i < MAX_OPENACC_INDEX; i++)
		inuse[i] = tmp[i];
    
}
#endif

void OPENACCEvent_WriteEnabledOperations (FILE * fd){

	int anyused = FALSE;
	int i;

	for (i = 0; i < MAX_OPENACC_INDEX; i++)
		anyused = anyused || inuse[i];
	
	if (anyused)
	{
		if (inuse[OPENACC_LAUNCH_INDEX]) {
			fprintf (fd, "EVENT_TYPE\n" "%d   %d    OPENACC LAUNCH\n", 0, OPENACC_LAUNCH_EV);
			fprintf (fd, "VALUES\n" "0 End\n");
			fprintf (fd, "%d OPENACC_ENQUEUE_LAUNCH\n", OPENACC_ENQUEUE_LAUNCH_VAL);
			fprintf (fd, "%d OPENACC_DEVICE_EXECUTE\n", OPENACC_DEVICE_EXECUTE_VAL);
		}

		if (inuse[OPENACC_DATA_INDEX]) {
			fprintf (fd, "EVENT_TYPE\n"
		             "%d   %d    OPENACC DATA\n", 1, OPENACC_DATA_EV);
			fprintf (fd, "VALUES\n"
		             "0 End\n");
			fprintf (fd, "%d OPENACC_CREATE\n", OPENACC_CREATE_VAL);
			fprintf (fd, "%d OPENACC_DELETE\n", OPENACC_DELETE_VAL);
			fprintf (fd, "%d OPENACC_ALLOC\n", OPENACC_ALLOC_VAL);
			fprintf (fd, "%d OPENACC_FREE\n",OPENACC_FREE_VAL);
			fprintf (fd, "%d OPENACC_ENQUEUE_UPLOAD\n", OPENACC_ENQUEUE_UPLOAD_VAL);
			fprintf (fd, "%d OPENACC_ENQUEUE_DOWNLOAD\n",OPENACC_ENQUEUE_DOWNLOAD_VAL );
		}
		if (inuse[OPENACC_OTHERS_INDEX]) {
			fprintf (fd, "EVENT_TYPE\n"
		             "%d   %d    OPENACC OTHERS\n", 2, OPENACC_OTHERS_EV);
			fprintf (fd, "VALUES\n"
		             "0 End\n");
			fprintf (fd, "%d OPENACC_DEVICE_INIT\n", OPENACC_DEVICE_INIT_VAL);
			fprintf (fd, "%d OPENACC_DEVICE_SHUTDOWN\n", OPENACC_DEVICE_SHUTDOWN_VAL);
			fprintf (fd, "%d OPENACC_RUNTIME_SHUTDOWN\n", OPENACC_RUNTIME_SHUTDOWN_VAL);
			fprintf (fd, "%d OPENACC_COMPUTE_CONSTRUCT\n",OPENACC_COMPUTE_CONSTRUCT_VAL);
			fprintf (fd, "%d OPENACC_ENTER_DATA\n", OPENACC_ENTER_DATA_VAL);
			fprintf (fd, "%d OPENACC_EXIT_DATA\n",OPENACC_EXIT_DATA_VAL);
			fprintf (fd, "%d OPENACC_UPDATE\n", OPENACC_UPDATE_VAL);
			fprintf (fd, "%d OPENACC_WAIT\n",OPENACC_WAIT_VAL);
		}

	}



}
