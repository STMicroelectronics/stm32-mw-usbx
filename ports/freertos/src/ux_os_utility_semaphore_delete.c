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
/*    _ux_os_utility_semaphore_delete                     PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes the specified semaphore.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    semaphore                             Semaphore to delete           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    vSemaphoreDelete                      FreeRTOS semaphore delete     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_semaphore_delete(UX_SEMAPHORE *semaphore)
{
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

  if (xPortIsInsideInterrupt())
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_SEMAPHORE_ERROR);
    return UX_SEMAPHORE_ERROR;
  }

#if (configQUEUE_REGISTRY_SIZE > 0)
  vQueueUnregisterQueue(semaphore->semaphore_handle);
#endif

  /* Delete the semaphore */
  vSemaphoreDelete(semaphore->semaphore_handle);

  /* Reset the semaphore handle and suspended count */
  taskENTER_CRITICAL();
  semaphore->semaphore_handle = 0U;
  semaphore->ux_semaphore_suspended_count = 0U;
  taskEXIT_CRITICAL();

  /* Return completion status.  */
  return(UX_SUCCESS);
}


