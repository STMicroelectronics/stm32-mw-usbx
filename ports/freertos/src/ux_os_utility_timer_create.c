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

/* Declaration of a wrapper function for the timer callback */
VOID _utility_timer_callback_wrapper(TimerHandle_t xTimer);

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
/*  CALLS                                                                 */
/*                                                                        */
/*    xTimerCreate                          FreeRTOS timer create         */
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
  UX_ASSERT(timer != UX_NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((timer == NULL) || (timer->timer_handle != NULL))
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

  timer->timer_handle = xTimerCreate(timer_name, (TickType_t)initial_ticks, pdTRUE, (void *)timer,
                                     (TimerCallbackFunction_t)_utility_timer_callback_wrapper);

  if (timer->timer_handle == NULL)
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_ERROR);
    return UX_ERROR;
  }

  /* Assign the provided expiration function to the callback timer function pointer */
  timer->timer_callback_function = expiration_function;
  timer->timer_callback_input = expiration_input;

  if (activation_flag == UX_AUTO_ACTIVATE)
  {
    if (_ux_utility_timer_start(timer, initial_ticks) != UX_SUCCESS)
    {
      /* Error trap. */
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_ERROR);
      return UX_ERROR;
    }
  }

  /* Return completion status. */
  return(UX_SUCCESS);
}

/* Definition of the wrapper function for the timer callback
 * This function is called by the FreeRTOS timer system when the timer expires
 */
VOID _utility_timer_callback_wrapper(TimerHandle_t xTimer)
{
  UX_TIMER *timer;

  /* Obtain the timer ID */
  timer = (UX_TIMER *)pvTimerGetTimerID(xTimer);

  /* Call the application callback timer function */
  if ((timer != NULL) && (timer->timer_callback_function != NULL))
  {
    timer->timer_callback_function(timer->timer_callback_input);
  }
}

