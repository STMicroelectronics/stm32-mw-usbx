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
/*    _ux_os_utility_event_flags_set                      PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function set event flags from event flag group                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    group_ptr                             Event flag control group      */
/*    flags_to_set                          32 bits variable event flags  */
/*    set_option                            set option                    */
/*                                                                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xEventGroupClearBits                  FreeRTOS clear event flag     */
/*    xEventGroupSetBits                    FreeRTOS set event flag       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_event_flags_set(UX_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set, UINT set_option)
{
  LONG yield;
  UX_ASSERT(group_ptr != NULL);
  UX_ASSERT(group_ptr->event_group_handle != NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((group_ptr == NULL) || (group_ptr->event_group_handle == NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_EVENT_ERROR);
    return UX_EVENT_ERROR;
  }
#endif

  if (set_option != UX_OR)
  {
    if (xPortIsInsideInterrupt())
    {
      xEventGroupGetBitsFromISR(group_ptr->event_group_handle);

      if (xEventGroupClearBitsFromISR(group_ptr->event_group_handle, (EventBits_t) ~flags_to_set) == pdFAIL)
      {
        _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_EVENT_ERROR);
        return UX_EVENT_ERROR;
      }
    }
    else
    {
      xEventGroupClearBits(group_ptr->event_group_handle, (EventBits_t) ~flags_to_set);
    }
  }

  if (xPortIsInsideInterrupt())
  {
    yield = pdFALSE;

    if (xEventGroupSetBitsFromISR(group_ptr->event_group_handle, (EventBits_t) flags_to_set, &yield) == pdFAIL)
    {
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_EVENT_ERROR);
      return UX_EVENT_ERROR;
    }
    else
    {
      portYIELD_FROM_ISR(yield);
    }
  }
  else
  {
    xEventGroupSetBits(group_ptr->event_group_handle, (EventBits_t) flags_to_set);
  }

  /* Return completion status.  */
  return(UX_SUCCESS);
}