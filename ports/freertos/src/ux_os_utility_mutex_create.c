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
/*    _ux_os_utility_mutex_create                         PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a protection mutex.                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    mutex                                 Pointer to mutex              */
/*    mutex_name                            Name of mutex                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xSemaphoreCreateMutex                 FreeRTOS mutex create         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_mutex_create(UX_MUTEX *mutex, CHAR *mutex_name)
{
  UX_ASSERT(mutex != UX_NULL);
  UX_ASSERT(mutex_name != UX_NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  /* Check if the mutex is already created */
  if ((mutex == NULL) || (mutex->mutex_handle != NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_MUTEX_ERROR);
    return UX_MUTEX_ERROR;
  }
#endif

  if (xPortIsInsideInterrupt())
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_MUTEX_ERROR);
    return UX_MUTEX_ERROR;
  }

  /* Call FreeRTOS to create the mutex object. */
  mutex->mutex_handle = xSemaphoreCreateRecursiveMutex();

  /* Check for errors. */
  if (mutex->mutex_handle == NULL)
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_MUTEX_ERROR);
    return UX_MUTEX_ERROR;
  }

#if (configQUEUE_REGISTRY_SIZE > 0)
  vQueueAddToRegistry(mutex->mutex_handle, mutex_name);
#endif

  /* Return completion status. */
  return UX_SUCCESS;
}
