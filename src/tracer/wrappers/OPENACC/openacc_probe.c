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

#include "threadid.h"
#include "wrapper.h"
#include "trace_macros.h"
#include "openacc_probe.h"

static int trace_openacc = TRUE;

void Extrae_set_trace_OpenACC (int b)
{ trace_openacc = b; }

int Extrae_get_trace_OpenACC (void)
{ return trace_openacc; }

#if 0
# define DEBUG fprintf (stdout, "THREAD %d: %s\n", THREADID, __FUNCTION__);
#else
# define DEBUG
#endif

//LAUNCH
void Probe_openacc_enqueue_launch_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_LAUNCH_EV, OPENACC_ENQUEUE_LAUNCH_VAL, EMPTY); 
}

void Probe_openacc_enqueue_launch_Exit() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_LAUNCH_EV, OPENACC_END_VAL, EMPTY); 
}

//DATA
void  Probe_openacc_create_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_DATA_EV,OPENACC_CREATE_VAL, EMPTY); 
}

void  Probe_openacc_delete_Enter(){
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_DATA_EV,OPENACC_DELETE_VAL,EMPTY); 
}

void Probe_openacc_alloc_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_DATA_EV,OPENACC_ALLOC_VAL, EMPTY); 
}

void Probe_openacc_free_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_DATA_EV, OPENACC_FREE_VAL, EMPTY); 
}

void Probe_openacc_enqueue_upload_start_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_DATA_EV, OPENACC_ENQUEUE_UPLOAD_VAL, EMPTY); 
}

void  Probe_openacc_enqueue_upload_end_Exit() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_DATA_EV, OPENACC_END_VAL, EMPTY); 
}

void Probe_openacc_enqueue_download_start_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_DATA_EV, OPENACC_ENQUEUE_DOWNLOAD_VAL, EMPTY); 
}

void Probe_openacc_enqueue_download_end_Exit() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME,  OPENACC_DATA_EV, OPENACC_END_VAL, EMPTY); 
} 

//OTHERS

void Probe_openacc_runtime_shutdown_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV, OPENACC_RUNTIME_SHUTDOWN_VAL, EMPTY); 
} 

void Probe_openacc_device_init_start_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV , OPENACC_DEVICE_INIT_VAL, EMPTY); 
}

void Probe_openacc_device_init_end_Exit() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV, OPENACC_END_VAL, EMPTY); 
}

void Probe_openacc_device_shutdown_start_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV , OPENACC_DEVICE_SHUTDOWN_VAL, EMPTY);
}

void Probe_openacc_device_shutdown_end_Exit() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV, OPENACC_END_VAL, EMPTY);
}

void Probe_openacc_compute_contruct_Enter()
{
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME,OPENACC_OTHERS_EV , OPENACC_COMPUTE_CONSTRUCT_VAL, EMPTY); 
}


void Probe_openacc_compute_contruct_Exit()
{
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV, OPENACC_END_VAL, EMPTY); 
}

void Probe_openacc_enter_data_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME,OPENACC_OTHERS_EV, OPENACC_ENTER_DATA_VAL, EMPTY); 
}

void Probe_openacc_enter_data_Exit() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME,OPENACC_OTHERS_EV , OPENACC_END_VAL, EMPTY); 
}

void Probe_openacc_exit_data_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME,OPENACC_OTHERS_EV, OPENACC_EXIT_DATA_VAL, EMPTY); 
}

void Probe_openacc_exit_data_Exit() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME,OPENACC_OTHERS_EV , OPENACC_END_VAL, EMPTY); 
}

void Probe_openacc_update_start_Enter () {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV, OPENACC_UPDATE_VAL, EMPTY); 
}

void Probe_openacc_update_end_Exit () {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV, OPENACC_END_VAL, EMPTY); 
}

void Probe_openacc_wait_start_Enter() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV, OPENACC_WAIT_VAL, EMPTY); 
}

void Probe_openacc_wait_end_Exit() {
    DEBUG
    if (mpitrace_on && Extrae_get_trace_OpenACC())
        TRACE_MISCEVENTANDCOUNTERS(TIME, OPENACC_OTHERS_EV, OPENACC_END_VAL, EMPTY); 
}
