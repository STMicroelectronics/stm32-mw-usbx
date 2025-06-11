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
/*    _ux_os_utility_event_flags_get                      PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function get event flags from event flag group                 */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    group_ptr                             Event flag control group      */
/*    requested_flags                       32 bits variable event flags  */
/*    get_option                            AND/OR/CLEAR ... options      */
/*    actual_flag_ptr                       where the flags are placed    */
/*    wait_option                           waiting option                */
/*                                                                        */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    xEventGroupWaitBits                   FreeRTOS get event flag       */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    USBX Components                                                     */
/*                                                                        */
/**************************************************************************/
UINT _ux_os_utility_event_flags_get(UX_EVENT_FLAGS_GROUP *group_ptr, ULONG requested_flags,
                                    UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option)
{

  UX_ASSERT(group_ptr != UX_NULL);
  UX_ASSERT(group_ptr->event_group_handle != NULL);
  UX_ASSERT(actual_flags_ptr != UX_NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((group_ptr == NULL) || (group_ptr->event_group_handle == NULL) || (actual_flags_ptr == NULL))
  {
    /* Error trap. */
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_EVENT_ERROR);
    return UX_EVENT_ERROR;
  }
#endif

  EventGroupHandle_t hEventGroup = group_ptr->event_group_handle;
  BaseType_t wait_for_all = 0U;
  BaseType_t clear_on_exit = 1U;

  if ((hEventGroup == NULL) || ((requested_flags & 0xFF000000) != 0U) || (xPortIsInsideInterrupt()))
  {
    *actual_flags_ptr = 0xFFFFFFFF;
    _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_EVENT_ERROR);
    return(UX_EVENT_ERROR);
  }

  if ((get_option == UX_AND) || (get_option == UX_OR))
  {
    clear_on_exit = pdFAIL;
  }
  else if (get_option == UX_AND_CLEAR)
  {
    wait_for_all = pdTRUE;
  }

  *actual_flags_ptr = xEventGroupWaitBits(hEventGroup, (EventBits_t)requested_flags, clear_on_exit, wait_for_all, (TickType_t)wait_option);

  if (get_option == UX_AND_CLEAR)
  {
    if (((requested_flags & *actual_flags_ptr) != requested_flags) || ((requested_flags & *actual_flags_ptr) == 0U))
    {
      *actual_flags_ptr = 0xFFFFFFFF;
      _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_UTILITY, UX_EVENT_ERROR);
      return(UX_EVENT_ERROR);
    }
  }
  /* Return completion status. */
  return(UX_SUCCESS);
}
