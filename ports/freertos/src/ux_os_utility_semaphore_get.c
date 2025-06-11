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
/*    _ux_os_utility_semaphore_get                        PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets a semaphore signal.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    semaphore                             Semaphore to get signal from  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xTaskGetCurrentTaskHandle             FreeRTOS identify thread      */
/*    uxTaskPriorityGet                     FreeRTOS get thread info      */
/*    xSemaphoreTake                        FreeRTOS semaphore get        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_semaphore_get(UX_SEMAPHORE *semaphore, ULONG semaphore_signal)
{
  UINT priority;
  UX_THREAD thread_id;
  BaseType_t yield;

  UX_ASSERT(semaphore != NULL);
  UX_ASSERT(semaphore->semaphore_handle != NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((semaphore == NULL) || (semaphore->semaphore_handle == NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
    return UX_SEMAPHORE_ERROR;
  }
#endif

  thread_id.task_handle = xTaskGetCurrentTaskHandle();

  UX_ASSERT(thread_id.task_handle != NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if (thread_id.task_handle == NULL)
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_THREAD_ERROR);
    return UX_THREAD_ERROR;
  }
#endif

  /* Call FreeRTOS to get the semaphore.*/
  if (xPortIsInsideInterrupt())
  {
    priority = uxTaskPriorityGetFromISR(thread_id.task_handle);
    yield = pdFALSE;

    /* Increment the suspended count before waiting */
    taskENTER_CRITICAL();
    semaphore->ux_semaphore_suspended_count++;
    taskEXIT_CRITICAL();

    if (xSemaphoreTakeFromISR(semaphore->semaphore_handle, &yield) != pdPASS)
    {
      /* Decrement the suspended count */
      taskENTER_CRITICAL();
      semaphore->ux_semaphore_suspended_count--;
      taskEXIT_CRITICAL();

      /* Error trap. */
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
      return UX_SEMAPHORE_ERROR;
    }
    else
    {
      /* Decrement the suspended count */
      taskENTER_CRITICAL();
      semaphore->ux_semaphore_suspended_count--;
      taskEXIT_CRITICAL();

      portYIELD_FROM_ISR(yield);
    }
  }
  else
  {
    priority = uxTaskPriorityGet(thread_id.task_handle);

    /* Increment the suspended count before waiting */
    taskENTER_CRITICAL();
    semaphore->ux_semaphore_suspended_count++;
    taskEXIT_CRITICAL();

    if (xSemaphoreTake(semaphore->semaphore_handle, (TickType_t)semaphore_signal) != pdPASS)
    {
      /* Decrement the suspended count */
      taskENTER_CRITICAL();
      semaphore->ux_semaphore_suspended_count--;
      taskEXIT_CRITICAL();

      /* Error trap. */
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
      return UX_SEMAPHORE_ERROR;
    }
    else
    {
      /* Decrement the suspended count */
      taskENTER_CRITICAL();
      semaphore->ux_semaphore_suspended_count--;
      taskEXIT_CRITICAL();
    }
  }

  /* Is this the lowest priority thread in the system  */
  if (priority < _ux_system -> ux_system_thread_lowest_priority)
  {
    /* We need to remember this thread priority.  */
    _ux_system -> ux_system_thread_lowest_priority = priority;
  }

  /* Return completion status.  */
  return(UX_SUCCESS);
}
