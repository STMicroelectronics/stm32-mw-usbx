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
/*    _ux_os_utility_thread_delete                        PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes a thread for USBX.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    thread_ptr                            Thread control block pointer  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    eTaskGetState                 FreeRTOS get state of thread          */
/*    vTaskDelete                   FreeRTOS terminate and delete thread  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_thread_delete(UX_THREAD *thread_ptr)
{
  UX_ASSERT(thread_ptr != UX_NULL);
  UX_ASSERT(thread_ptr->task_handle != UX_NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((thread_ptr == NULL) || (thread_ptr->task_handle == NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_THREAD_ERROR);
    return UX_THREAD_ERROR;
  }
#endif

  if ((xPortIsInsideInterrupt() == pdTRUE) || (eTaskGetState(thread_ptr->task_handle) == eDeleted))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_THREAD_ERROR);
    return UX_THREAD_ERROR;
  }

  /* Terminate and delete the USBX thread. */
  vTaskDelete(thread_ptr->task_handle);

  /* Reset the thread handle */
  thread_ptr->task_handle = 0U;

  /* Return completion status.  */
  return(UX_SUCCESS);
}
