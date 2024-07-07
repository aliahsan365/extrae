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

#include <config.h>
#include <openacc.h>
#include <cuda_runtime_api.h>
//# include <cupti.h>
//# include <cupti_events.h>

#define MAX_OPENACC_EVENTS 32768


#define CUDA_SUCCESS 0

#define CHECK_CU_ERROR(err, cufunc)                             \
  if (err != CUDA_SUCCESS)                                      \
    {                                                           \
      printf ("Error %d for CUDA Driver API function '%s'.\n",  \
              err,  #cufunc);                                   \
      exit(-1);                                                 \
    }


cudaError_t cudaGetDevice (int *);
cudaError_t cudaEventCreateWithFlags (cudaEvent_t *, unsigned);
cudaError_t cudaEventRecord(cudaEvent_t , cudaStream_t);
cudaError_t cudaEventSynchronize(cudaEvent_t);
cudaError_t cudaGetDeviceCount(int *);
cudaError_t cudaEventElapsedTime(float *, cudaEvent_t, cudaEvent_t);



typedef enum {
	EXTRAE_OPENACC_NEW_TIME,
	EXTRAE_OPENACC_PREVIOUS_TIME
} Extrae_OPENACC_Time_Type;

/* Information per stream required during tracing */
struct RegisteredStreams_t
{
	UINT64 host_reference_time;
	cudaEvent_t device_reference_time; /* accessed through cudaEvent_t */
	unsigned threadid; /* In Paraver sense */
	cudaStream_t stream;

	unsigned nevents;
	cudaEvent_t ts_events[MAX_OPENACC_EVENTS];
	unsigned events[MAX_OPENACC_EVENTS];
	unsigned long long values[MAX_OPENACC_EVENTS];

	Extrae_OPENACC_Time_Type timetype[MAX_OPENACC_EVENTS];
};


struct OPENACCdevices_t 
{
	struct RegisteredStreams_t *Stream;
    int id;
    int initialized;
    int nstreams;
};

//launch
void Extrae_openacc_enqueue_launch_Enter (const char*);
void Extrae_openacc_enqueue_launch_Exit ();
//data
void Extrae_openacc_create_Enter ();
void Extrae_openacc_delete_Enter ();
void Extrae_openacc_alloc_Enter ();
void Extrae_openacc_free_Enter ();
void Extrae_openacc_enqueue_upload_start_Enter ();
void Extrae_openacc_enqueue_upload_end_Exit ();
void Extrae_openacc_enqueue_download_start_Enter ();
void Extrae_openacc_enqueue_download_end_Exit ();
//others
void Extrae_openacc_runtime_shutdown_Enter();
void Extrae_openacc_device_init_start_Enter();
void Extrae_openacc_device_init_end_Exit();
void Extrae_openacc_device_shutdown_start_Enter();
void Extrae_openacc_device_shutdown_end_Exit();
void Extrae_openacc_compute_contruct_Enter ();
void Extrae_openacc_compute_contruct_Exit ();
void Extrae_openacc_enter_data_Enter ();
void Extrae_openacc_enter_data_Exit ();
void Extrae_openacc_exit_data_Enter ();
void Extrae_openacc_exit_data_Exit ();
void Extrae_openacc_update_start_Enter ();
void Extrae_openacc_update_end_Exit ();
void Extrae_openacc_wait_start_Enter();
void Extrae_openacc_wait_end_Exit();