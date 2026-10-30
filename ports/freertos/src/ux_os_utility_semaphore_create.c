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
/*    _ux_os_utility_semaphore_create                     PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a semaphore.                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    semaphore                             Semaphore to create           */
/*    semaphore_name                        Semaphore name                */
/*    initial_count                         Initial semaphore count       */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xSemaphoreCreateBinary                FreeRTOS semaphore create     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_semaphore_create(UX_SEMAPHORE *semaphore, CHAR *semaphore_name, UINT initial_count)
{
  UX_ASSERT(semaphore != UX_NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((semaphore == NULL) || (semaphore->semaphore_handle != NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
    return UX_SEMAPHORE_ERROR;
  }
#endif

  if (xPortIsInsideInterrupt())
  {
    /* Semaphore creation is not supported from ISR context. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
    return UX_SEMAPHORE_ERROR;
  }

  /* Initialize the suspended count to 0. */
  semaphore->ux_semaphore_suspended_count = 0U;
  semaphore->ux_semaphore_initial_count = initial_count;

  if (initial_count <= 1U)
  {
    /* Binary semaphore for initial_count 0 or 1. */
    semaphore->semaphore_handle = xSemaphoreCreateBinary();

    if (semaphore->semaphore_handle == NULL)
    {
      semaphore->ux_semaphore_initial_count = 0U;

      /* Error trap. */
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
      return UX_SEMAPHORE_ERROR;
    }

    if (initial_count != 0U)
    {
      if (xSemaphoreGive(semaphore->semaphore_handle) != pdPASS)
      {
        vSemaphoreDelete(semaphore->semaphore_handle);
        semaphore->semaphore_handle = NULL;
        semaphore->ux_semaphore_initial_count = 0U;

        /* Error trap. */
        _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
        return UX_SEMAPHORE_ERROR;
      }
    }
  }
  else
  {
    /* Counting semaphore for initial_count > 1. */
    semaphore->semaphore_handle = xSemaphoreCreateCounting((UBaseType_t)initial_count,
                                                           (UBaseType_t)initial_count);

    if (semaphore->semaphore_handle == NULL)
    {
      semaphore->ux_semaphore_initial_count = 0U;

      /* Error trap. */
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
      return UX_SEMAPHORE_ERROR;
    }
  }

#if (configQUEUE_REGISTRY_SIZE > 0)
  vQueueAddToRegistry(semaphore->semaphore_handle, semaphore_name);
#endif

  /* Return completion status.  */
  return(UX_SUCCESS);
}
