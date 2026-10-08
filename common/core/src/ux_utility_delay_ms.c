/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026-present Eclipse ThreadX contributors
 * Copyright (c) 2025-2026 STMicroelectronics.
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
/**   Host Stack                                                          */
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
/*    _ux_utility_delay_ms                                PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function causes the calling thread to sleep for the            */
/*    specified number of milliseconds                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    ms_wait                               Number of milliseconds to     */
/*                                            wait for                    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_utility_sleep_ms                     RTOS sleep function        */
/*    _ux_utility_time_get                     Get current time           */
/*    _ux_utility_time_elapsed                 STANDALONE sleep function  */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
VOID _ux_utility_delay_ms(ULONG ms_wait)
{

#if defined (UX_STANDALONE)
  ULONG   ticks;

  /* Get current time.  */
  ticks = _ux_utility_time_get();

  /* Wait until timeout.  */
  while(_ux_utility_time_elapsed(ticks, _ux_utility_time_get()) <
        UX_MS_TO_TICK_NON_ZERO(ms_wait));
  /* Return completion status.  */
  return;
#else
  _ux_utility_sleep_ms(ms_wait);
  /* Return completion status.  */
  return;
#endif

}
