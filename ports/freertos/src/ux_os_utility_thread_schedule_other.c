/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2024 STMicroelectronics
 *
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** USBX Component                                                        */
/**                                                                       */
/**   Utility                                                             */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_os_utility_thread_schedule_other                PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function force the scheduling of all other threads.            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    caller_priority                        Priority to restore.         */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    uxTaskPriorityGet                     FreeRTOS priority identify    */
/*    vTaskPrioritySet                      FreeRTOS priority change      */
/*    _ux_utility_thread_relinquish         FreeRTOS relinquish           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_thread_schedule_other(UINT caller_priority)
{
  UINT old_priority;
  UX_THREAD thread_id;

  thread_id.task_handle = xTaskGetCurrentTaskHandle();

  if ((thread_id.task_handle == NULL) || (xPortIsInsideInterrupt()))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_THREAD_ERROR);
    return UX_THREAD_ERROR;
  }

  old_priority = uxTaskPriorityGet(thread_id.task_handle);
  vTaskPrioritySet(thread_id.task_handle, (UBaseType_t)_ux_system -> ux_system_thread_lowest_priority);

  /* Wait until all other threads passed into the scheduler. */
  _ux_utility_thread_relinquish();

  /* And now return the priority of the thread to normal.  */
  vTaskPrioritySet(thread_id.task_handle, old_priority);

  /* Return the status. */
  return (UX_SUCCESS);
}
