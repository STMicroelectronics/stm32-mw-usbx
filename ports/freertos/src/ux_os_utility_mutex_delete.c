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
/*    _ux_os_utility_mutex_delete                         PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function deletes a protection mutex.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    mutex                                 Pointer to mutex              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    vSemaphoreDelete                      FreeRTOS mutex delete         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_mutex_delete(UX_MUTEX *mutex)
{
  UX_ASSERT(mutex != NULL);
  UX_ASSERT(mutex->mutex_handle != NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((mutex == NULL) || (mutex->mutex_handle == NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_MUTEX_ERROR);
    return UX_MUTEX_ERROR;
  }
#endif

  if (xPortIsInsideInterrupt())
  {
    return UX_MUTEX_ERROR;
  }

  /* Call FreeRTOS to delete the Mutex object. */
#if (configQUEUE_REGISTRY_SIZE > 0)
  vQueueUnregisterQueue(mutex->mutex_handle);
#endif

  vSemaphoreDelete(mutex->mutex_handle);

  /* Reset the mutex handle */
  mutex->mutex_handle = 0U;

  /* Return completion status. */
  return (UX_SUCCESS);
}
