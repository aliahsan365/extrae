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
#include "debug.h"

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#include "openacc_common.h"

#include "taskid.h"
#include "threadinfo.h"
#include "wrapper.h"
#include "trace_macros.h"
#include "openacc_probe.h"



int static DEBUG  = 0;

static struct OPENACCdevices_t *devices = NULL;
static int OPENACCdevices = 0;


void get_device_devid(int* devid) {
    *devid = acc_get_device_num(acc_get_device());
}

//de momento solo para devices nvidia
void Extrae_OPENACC_Initialize(int devid) {
   // If devices table is not initialized, create it first 
   if (devices == NULL) 
    {
        //test de configure para mas devices
        int OPENACCdevices = acc_get_num_devices(acc_device_nvidia);
        //printf("nvidia devices in the system are: %d\n", OPENACCdevices );
        devices = (struct OPENACCdevices_t*) malloc (sizeof(struct OPENACCdevices_t)*OPENACCdevices);
        if (devices == NULL)
        {
            fprintf (stderr, PACKAGE_NAME": Error! Cannot allocate information for OPENACC nvidia-devices!\n");
            exit (-1);
        }

        for (int i = 0; i < OPENACCdevices; i++)
            devices[i].initialized = FALSE;
    }

    /* If the device we're using is not initialized, create its structures */
    if (!devices[devid].initialized)
    {
        char _threadname[THREAD_INFO_NAME_LEN];
        char _hostname[HOST_NAME_MAX];
        unsigned prev_threadid;
        int found;
        /* Was the thread created before (i.e. did we executed a openaccdevicereset?) */
        if (gethostname(_hostname, HOST_NAME_MAX) == 0)
            sprintf (_threadname, "OPENACC-D%d", devid+1);
        else
            sprintf (_threadname, "OPENACC-D%d", devid+1);
        prev_threadid = Extrae_search_thread_name (_threadname, &found);

        if (found)
        {
            /* If thread name existed, reuse its thread id */
            devices[devid].id = prev_threadid;
        }
        else
        {
            /* For timing purposes we change num of threads here instead of doing Backend_getNumberOfThreads() + CUDAdevices*/
            /*
             * XXX Should this be Backend_getMaximumOfThreads()? If we
             * previously increased the number of threads in another runtime,
             * and then decreased them, we will end up with a line with mixed
             * semantics (thread&stream).
             */
            Backend_ChangeNumberOfThreads(Backend_getNumberOfThreads() + 1);
            devices[devid].id = Backend_getNumberOfThreads()-1;

            /* Set thread name */
            Extrae_set_thread_name(devices[devid].id, _threadname);
        }
        //devices[devid].buffer = new_Buffer()
        devices[devid].initialized = TRUE;
    }
}

/*
static cudaEvent_t t1, t2;


static unsigned long long host_reference_time;
static cudaEvent_t device_reference_time;
//host y deviece miran el reloj a la vez
void synchronize_clocks() {
    cudaEventCreate(&device_reference_time);
    cudaEventRecord(device_reference_time,0);
    cudaEventSynchronize(device_reference_time);
    host_reference_time=TIME;
}


void Extrae_openacc_enqueue_launch_Enter (const char * kernel_name) {
    Extrae_OPENACC_Initialize(0);
    static int count =0;
    if (count ==0) {
        synchronize_clocks();
        ++count;
    }
    Probe_openacc_enqueue_launch_Enter();
    cudaEventCreate(&t1);
    cudaEventRecord(t1,0);
    cudaEventSynchronize(t1);
    float begin_time;
    cudaEventElapsedTime (&begin_time, device_reference_time, t1);
    //printf("extrae gpu begin time : %f us \n", begin_time*1000);
    cudaEventDestroy(t1);
    OPENACC_TRACE(0,host_reference_time + ((UINT64) (begin_time*1000000)), OPENACC_LAUNCH_EV, OPENACC_DEVICE_EXECUTE_VAL, EMPTY);
}

void Extrae_openacc_enqueue_launch_Exit() {

    Probe_openacc_enqueue_launch_Exit();
}



void Extrae_openacc_wait_end_Exit() {
    cudaEventCreate(&t2);
    cudaEventRecord(t2,0);
    cudaEventSynchronize(t2);
    //mide el tiempo des de la lectura de syncotronize hasta la lectura del t2
    float end_time;
    cudaEventElapsedTime (&end_time, device_reference_time, t2);
    //printf("extrae gpu end time : %f us \n", end_time*1000);
    cudaEventDestroy(t2);
    OPENACC_TRACE(0, host_reference_time + ((UINT64) (end_time*1000000)), OPENACC_LAUNCH_EV, OPENACC_END_VAL, EMPTY);
    Probe_openacc_wait_end_Exit();
}

*/



void Extrae_openacc_enqueue_launch_Enter (const char * kernel_name) {
    Probe_openacc_enqueue_launch_Enter();
}


void Extrae_openacc_enqueue_launch_Exit() {
    Probe_openacc_enqueue_launch_Exit();
}

void Extrae_openacc_create_Enter () {
    Probe_openacc_create_Enter();
}

void Extrae_openacc_delete_Enter () {
    Probe_openacc_delete_Enter();
}

void Extrae_openacc_alloc_Enter () {
    Probe_openacc_alloc_Enter();
}

void Extrae_openacc_free_Enter () {
    Probe_openacc_free_Enter();
}

void Extrae_openacc_enqueue_upload_start_Enter () {
   Probe_openacc_enqueue_upload_start_Enter();
}

void Extrae_openacc_enqueue_upload_end_Exit () {
    Probe_openacc_enqueue_upload_end_Exit(); 
}

void Extrae_openacc_enqueue_download_start_Enter () {
    Probe_openacc_enqueue_download_start_Enter();
}

void Extrae_openacc_enqueue_download_end_Exit () {
    Probe_openacc_enqueue_download_end_Exit ();
}

//others
void Extrae_openacc_runtime_shutdown_Enter() {
    Probe_openacc_runtime_shutdown_Enter(); 
}

void Extrae_openacc_device_init_start_Enter() {
    Probe_openacc_device_init_start_Enter();
}

void Extrae_openacc_device_init_end_Exit() {
    Probe_openacc_device_init_end_Exit();
}

void Extrae_openacc_device_shutdown_start_Enter() {
    Probe_openacc_device_shutdown_start_Enter();
}

void Extrae_openacc_device_shutdown_end_Exit() {
    Probe_openacc_device_shutdown_end_Exit();
}

void Extrae_openacc_compute_contruct_Enter () {
    Probe_openacc_compute_contruct_Enter(); 
}

void Extrae_openacc_compute_contruct_Exit () {
    Probe_openacc_compute_contruct_Exit();
}

void Extrae_openacc_enter_data_Enter () {
    Probe_openacc_enter_data_Enter();
}

void Extrae_openacc_enter_data_Exit () {
    Probe_openacc_enter_data_Exit();
}

void Extrae_openacc_exit_data_Enter () {
    Probe_openacc_exit_data_Enter();
}

void Extrae_openacc_exit_data_Exit () {
    Probe_openacc_exit_data_Exit();
}

void Extrae_openacc_update_start_Enter () {
    Probe_openacc_update_start_Enter ();
}

void Extrae_openacc_update_end_Exit () {
    Probe_openacc_update_end_Exit ();
}

void Extrae_openacc_wait_start_Enter() {
    Probe_openacc_wait_start_Enter();
}

void Extrae_openacc_wait_end_Exit() {
    Probe_openacc_wait_end_Exit();
}

