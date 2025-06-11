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
/*    _ux_os_utility_thread_create                        PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a thread for USBX.                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    thread_ptr                            Thread control block pointer  */
/*    name                                  Pointer to thread name string */
/*    entry_function                        Entry function of the thread  */
/*    entry_input                           32-bit input value to thread  */
/*    stack_start                           Pointer to start of stack     */
/*    stack_size                            Stack size in bytes           */
/*    priority                              Priority of thread (0-31)     */
/*    preempt_threshold                     Preemption threshold          */
/*    time_slice                            Thread time-slice value       */
/*    auto_start                            Automatic start selection     */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xTaskCreate                           FreeRTOS create thread        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_thread_create(UX_THREAD *thread_ptr, CHAR *name,
                                  VOID (*entry_function)(ULONG), ULONG entry_input,
                                  VOID *stack_start, ULONG stack_size,
                                  UINT priority, UINT preempt_threshold,
                                  ULONG time_slice, UINT auto_start)
{
  UX_ASSERT(thread_ptr != UX_NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((thread_ptr == NULL) || (*entry_function == NULL) || (thread_ptr->task_handle != NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_THREAD_ERROR);
    return UX_THREAD_ERROR;
  }
#endif

  if (xPortIsInsideInterrupt())
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_THREAD_ERROR);
    return UX_THREAD_ERROR;
  }

  /* Call FreeRTOS to create the thread. */
  if (xTaskCreate((TaskFunction_t)entry_function, name, (configSTACK_DEPTH_TYPE)stack_size,
                  (void *)entry_input, (UBaseType_t)priority, (TaskHandle_t *)&thread_ptr->task_handle) != pdPASS)
  {
    /* Reset the thread handle */
    thread_ptr->task_handle = 0U;
    return UX_THREAD_ERROR;
  }

  /* Store the address of thread_ptr in the thread local storage */
  vTaskSetThreadLocalStoragePointer(thread_ptr->task_handle, UX_THREAD_POINTER_INDEX, thread_ptr);

  if (auto_start == UX_DONT_START)
  {
    _ux_utility_thread_suspend(thread_ptr);
  }

  /* Return completion status. */
  return(UX_SUCCESS);
}

