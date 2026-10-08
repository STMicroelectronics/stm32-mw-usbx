/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026 STMicroelectronics.
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
/*    _ux_os_utility_semaphore_put                        PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function sets a semaphore signal.                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    semaphore                             Semaphore to signal           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xSemaphoreGive                       FreeRTOS semaphore put         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_semaphore_put(UX_SEMAPHORE *semaphore)
{
  BaseType_t semaphore_status;
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

  /* Check if we are inside an interrupt. */
  if (xPortIsInsideInterrupt())
  {
    yield = pdFALSE;

    /* A failed put on a binary semaphore means it is already signaled. */
    semaphore_status = xSemaphoreGiveFromISR(semaphore->semaphore_handle, &yield);
    if ((semaphore_status != pdTRUE) && (semaphore->ux_semaphore_initial_count > 1U))
    {
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
      return UX_SEMAPHORE_ERROR;
    }

    portYIELD_FROM_ISR(yield);
  }
  else
  {
    /* A failed put on a binary semaphore means it is already signaled. */
    semaphore_status = xSemaphoreGive(semaphore->semaphore_handle);
    if ((semaphore_status != pdPASS) && (semaphore->ux_semaphore_initial_count > 1U))
    {
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
      return UX_SEMAPHORE_ERROR;
    }
  }

  /* Return completion status. */
  return(UX_SUCCESS);
}

