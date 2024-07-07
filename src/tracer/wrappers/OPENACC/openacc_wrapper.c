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

#ifdef HAVE_DLFCN_H
# define __USE_GNU
# include <dlfcn.h>
# undef  __USE_GNU
#endif
#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "wrapper.h"
#include "openacc_common.h"
#include "openacc_probe.h"
#include "buffers.h"
#include "threadinfo.h"
#include <time.h>
#include <clock.h>
#include <acc_prof.h>

static int DEBUG =0;


void prof_launch(acc_prof_info* profinfo, acc_event_info* eventinfo,acc_api_info* apiinfo) {
    acc_launch_event_info* launchinfo;
    launchinfo = (acc_launch_event_info*) eventinfo;
    switch(launchinfo->event_type){
        case acc_ev_enqueue_launch_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_enqueue_launch_start\n", TIME);
        Extrae_openacc_enqueue_launch_Enter (launchinfo->kernel_name);
        break;
        case acc_ev_enqueue_launch_end:
        if (DEBUG)
            printf("%" PRId64 " acc_ev_enqueue_launch_end\n", TIME);
        Extrae_openacc_enqueue_launch_Exit ();
        break;
        default:
        break;
    }
}



void prof_data(acc_prof_info* profinfo, acc_event_info* eventinfo, acc_api_info* apiinfo){
    acc_data_event_info* datainfo;
    datainfo = (acc_data_event_info*)eventinfo;
    switch(datainfo->event_type ){
        case acc_ev_create:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_create\n", TIME);
        Extrae_openacc_create_Enter ();
        break;
        case acc_ev_delete:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_delete\n", TIME);
        Extrae_openacc_delete_Enter ();
        break;
        case acc_ev_alloc:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_alloc\n", TIME);
        Extrae_openacc_alloc_Enter ();
        break;
        case acc_ev_free:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_free\n", TIME);
        Extrae_openacc_free_Enter ();
        break;
        case acc_ev_enqueue_upload_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_enqueue_upload_start\n", TIME);
        Extrae_openacc_enqueue_upload_start_Enter ();
        case acc_ev_enqueue_upload_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_enqueue_upload_end\n", TIME);
        Extrae_openacc_enqueue_upload_end_Exit();
        break;
        case acc_ev_enqueue_download_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_enqueue_download_start\n", TIME);
        Extrae_openacc_enqueue_download_start_Enter ();
        case acc_ev_enqueue_download_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_enqueue_download_end\n", TIME);
        Extrae_openacc_enqueue_download_end_Exit();
        break;
        default:
        break;
    }
}



void prof_other(acc_prof_info* profinfo, acc_event_info* eventinfo,acc_api_info* apiinfo) {
    acc_other_event_info* otherinfo;
    otherinfo = (acc_other_event_info*) eventinfo;
    switch( otherinfo->event_type ){
        case acc_ev_runtime_shutdown:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_runtime_shutdown\n", TIME);
        Extrae_openacc_runtime_shutdown_Enter();
        break;
        case acc_ev_device_init_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_device_init_start\n", TIME);
        Extrae_openacc_device_init_start_Enter();
        break;
        case acc_ev_device_init_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_device_init_end\n", TIME);
        Extrae_openacc_device_init_end_Exit();
        break;
        case acc_ev_device_shutdown_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_device_shutdown_start\n", TIME);
        Extrae_openacc_device_shutdown_start_Enter();
        break;
        case acc_ev_device_shutdown_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_device_shutdown_end\n", TIME);
        Extrae_openacc_device_shutdown_end_Exit();
        break;
        case acc_ev_compute_construct_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_compute_construct_start\n", TIME);
        Extrae_openacc_compute_contruct_Enter ();
        break;
        case acc_ev_compute_construct_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_compute_construct_end\n", TIME);
        Extrae_openacc_compute_contruct_Exit();
        break;
        case acc_ev_enter_data_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_enter_data_start\n", TIME);
        Extrae_openacc_enter_data_Enter ();
        break;
        case acc_ev_enter_data_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_enter_data_end\n", TIME);
        Extrae_openacc_enter_data_Exit ();
        break;
        case acc_ev_exit_data_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_exit_data_start\n", TIME);
        Extrae_openacc_exit_data_Enter ();
        case acc_ev_exit_data_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_exit_data_end\n", TIME);
        Extrae_openacc_exit_data_Exit ();
        case acc_ev_update_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_update_start\n", TIME);
        Extrae_openacc_update_start_Enter ();
        break;
        case acc_ev_update_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_update_end\n", TIME);
        Extrae_openacc_update_end_Exit ();
        break;
        case acc_ev_wait_start:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_wait_start\n", TIME);
        Extrae_openacc_wait_start_Enter();
        break;
        case acc_ev_wait_end:
        if (DEBUG)
          printf("%" PRId64 " acc_ev_wait_end\n", TIME);
        Extrae_openacc_wait_end_Exit();
        default:
        break;
    }
}

void acc_register_library(acc_prof_reg acc_prof_register, acc_prof_reg acc_prof_unregister, acc_prof_lookup lookup) {
    //launch
    acc_prof_register(acc_ev_enqueue_launch_start,prof_launch,0);
    acc_prof_register(acc_ev_enqueue_launch_end,prof_launch,0);
    //data 
    acc_prof_register(acc_ev_create,prof_data,0);
    acc_prof_register(acc_ev_delete,prof_data,0);
    acc_prof_register(acc_ev_alloc,prof_data,0);
    acc_prof_register(acc_ev_free,prof_data,0);
    acc_prof_register(acc_ev_enqueue_upload_start,prof_data,0);
    acc_prof_register(acc_ev_enqueue_upload_end,prof_data,0);
    acc_prof_register(acc_ev_enqueue_download_start,prof_data,0);
    acc_prof_register(acc_ev_enqueue_download_end,prof_data,0);
    //other
    acc_prof_register(acc_ev_runtime_shutdown,prof_other,0);
    acc_prof_register(acc_ev_device_init_start,prof_other,0);
    acc_prof_register(acc_ev_device_init_end,prof_other,0);
    acc_prof_register(acc_ev_device_shutdown_start,prof_other,0);
    acc_prof_register(acc_ev_device_shutdown_end,prof_other,0);
    acc_prof_register(acc_ev_compute_construct_start,prof_other,0);
    acc_prof_register(acc_ev_compute_construct_end,prof_other,0);
    acc_prof_register(acc_ev_enter_data_start,prof_other,0);
    acc_prof_register(acc_ev_enter_data_end,prof_other,0);
    acc_prof_register(acc_ev_exit_data_start,prof_other,0);
    acc_prof_register(acc_ev_exit_data_end,prof_other,0);
    acc_prof_register(acc_ev_update_start,prof_other,0);
    acc_prof_register(acc_ev_update_end,prof_other,0);
    acc_prof_register(acc_ev_wait_start,prof_other,0);
    acc_prof_register(acc_ev_wait_end,prof_other,0);
}