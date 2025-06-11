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
/*    _ux_os_utility_sleep_ms                             PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function causes the calling thread to sleep for the            */
/*    specified number of milliseconds                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ms_wait                               Number of milliseconds to     */
/*                                          wait for                      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    vTaskDelay                            FreeRTOS sleep function       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
VOID _ux_os_utility_sleep_ms(ULONG ms_wait)
{
  TickType_t ticks = pdMS_TO_TICKS(ms_wait);

  if (xPortIsInsideInterrupt())
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_ERROR);
    return;
  }

  /* For safety add 1 to ticks.  */
  ticks++;

  /* Call FreeRTOS vTaskDelay function. */
  vTaskDelay(ticks);

  /* Return completion status.  */
  return;
}
