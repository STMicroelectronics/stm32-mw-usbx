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
/*    _ux_os_utility_mutex_on                             PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function gets system protection.                               */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    Mutex                                                               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xSemaphoreTake                        FreeRTOS mutex get            */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
VOID _ux_os_utility_mutex_on(UX_MUTEX *mutex)
{
  UX_ASSERT(mutex != NULL);
  UX_ASSERT(mutex->mutex_handle != NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((mutex == NULL) || (mutex->mutex_handle == NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_MUTEX_ERROR);
    return;
  }
#endif

  if (xPortIsInsideInterrupt())
  {
    /* Error Return */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_MUTEX_ERROR);
    return;
  }

  /* Call FreeRTOS to get system mutex. */
  if (xSemaphoreTakeRecursive(mutex->mutex_handle, portMAX_DELAY) != pdTRUE)
  {
    /* Error Return */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_MUTEX_ERROR);
  }

  return;
}
