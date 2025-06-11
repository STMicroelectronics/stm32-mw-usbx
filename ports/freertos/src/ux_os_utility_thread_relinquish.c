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
/*    _ux_os_utility_thread_relinquish                    PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function helps the thread relinquish its control.              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    taskYIELD                            FreeRTOS relinquish thread     */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
VOID _ux_os_utility_thread_relinquish(VOID)
{
  if (xPortIsInsideInterrupt())
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_THREAD_ERROR);
    return;
  }
  else
  {
    /* Call Freertos to relinquish a USBX thread.  */
    taskYIELD();
  }
}
