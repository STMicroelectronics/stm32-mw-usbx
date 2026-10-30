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

/* Declaration of a wrapper function for the timer callback */
VOID _utility_timer_callback_wrapper(UX_TIMER *timer);

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_os_utility_timer_create                         PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function creates a timer.                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    timer                                 Pointer to timer              */
/*    timer_name                            Name of timer                 */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_timer_create(UX_TIMER *timer, CHAR *timer_name, VOID (*expiration_function) (ULONG),
                                 ULONG expiration_input, ULONG initial_ticks, ULONG reschedule_ticks,
                                 UINT activation_flag)
{
#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((timer == NULL) || (timer->timer_handle != NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_ERROR);
    return UX_ERROR;
  }
#endif

  /* Return completion status. */
  return(UX_SUCCESS);
}

/* Definition of the wrapper function for the timer callback */
VOID _utility_timer_callback_wrapper(UX_TIMER *timer)
{
  /* Return completion status.  */
  return;
}

