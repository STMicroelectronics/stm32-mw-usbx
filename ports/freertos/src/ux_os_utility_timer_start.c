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
/*    _ux_os_utility_timer_start                            PORTABLE C    */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function starts a timer.                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    timer                                 Pointer to timer              */
/*    initial_ticks                         New Period of timer           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xTimerChangePeriod                    FreeRTOS timer update         */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_timer_start(UX_TIMER *timer, ULONG initial_ticks)
{
  UX_ASSERT(timer != UX_NULL);
  UX_ASSERT(timer->timer_handle != UX_NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((timer == NULL) || (timer->timer_handle == NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_ERROR);
    return UX_ERROR;
  }
#endif

  if (xPortIsInsideInterrupt())
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_ERROR);
    return UX_ERROR;
  }

  if (xTimerChangePeriod(timer->timer_handle, initial_ticks, 0) != pdPASS)
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_ERROR);
    return UX_ERROR;
  }

  /* Return completion status.  */
  return(UX_SUCCESS);
}
