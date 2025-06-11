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
#include "tx_timer.h"

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
/*    initial_ticks                         New period of timer           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _tx_timer_system_activate             ThreadX timer activate        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_timer_start(UX_TIMER *timer, ULONG initial_ticks)
{

  UX_ASSERT(timer != UX_NULL);

#ifdef TX_TIMER_ENABLE_PERFORMANCE_INFO

  /* Increment the total activations counter.  */
  _tx_timer_performance_activate_count++;

  /* Increment the number of activations on this timer.  */
  timer -> tx_timer_performance_activate_count++;
#endif

  /* Call actual activation function.  */
  _tx_timer_system_activate(&(timer -> tx_timer_internal));

  /* Return completion status.  */
  return(UX_SUCCESS);
}
