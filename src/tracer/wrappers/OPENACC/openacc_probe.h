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

#ifndef OPENACC_PROBE_H_INCLUDED
#define OPENACC_PROBE_H_INCLUDED

//launch
void Probe_openacc_enqueue_launch_Enter();
void Probe_openacc_enqueue_launch_Exit();
//data
void Probe_openacc_create_Enter();
void Probe_openacc_delete_Enter();
void Probe_openacc_alloc_Enter();
void Probe_openacc_free_Enter();
void Probe_openacc_enqueue_upload_start_Enter();
void Probe_openacc_enqueue_upload_end_Exit(); 
void Probe_openacc_enqueue_download_start_Enter();
void Probe_openacc_enqueue_download_end_Exit();
//others
void Probe_openacc_runtime_shutdown_Enter();
void Probe_openacc_device_init_start_Enter();
void Probe_openacc_device_init_end_Exit();
void Probe_openacc_device_shutdown_start_Enter();
void Probe_openacc_device_shutdown_end_Exit();
void Probe_openacc_compute_contruct_Enter();
void Probe_openacc_compute_contruct_Exit();
void Probe_openacc_enter_data_Enter();
void Probe_openacc_enter_data_Exit();
void Probe_openacc_exit_data_Enter();
void Probe_openacc_exit_data_Exit();
void Probe_openacc_update_start_Enter ();
void Probe_openacc_update_end_Exit ();
void Probe_openacc_wait_start_Enter();
void Probe_openacc_wait_end_Exit();

void Extrae_set_trace_OpenACC (int b);
int Extrae_get_trace_OpenACC (void);

#endif /* OPENACC_PROBE_H_INCLUDED */