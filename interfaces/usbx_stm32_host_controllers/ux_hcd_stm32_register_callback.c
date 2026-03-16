/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
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
/**   STM32 Controller Driver                                             */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define UX_SOURCE_CODE
#define UX_HCD_STM32_SOURCE_CODE


/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_hcd_stm32.h"
#include "ux_host_stack.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_hcd_stm32_register_callback                     PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function will register USB HCD callbacks.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    hcd_stm32                             Pointer to device controller  */
/*    parameter                                                           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    STM32 Controller Driver                                             */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Chaoqiong Xiao           Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
UINT _ux_hcd_stm32_register_callback(UX_HCD_STM32 *hcd_stm32, VOID *parameter)
{
#if defined (USE_HAL_HCD_REGISTER_CALLBACKS) && (USE_HAL_HCD_REGISTER_CALLBACKS == 1U)
  /* Register USB HCD SOF Callback. */
  HAL_HCD_RegisterSofCallback(hcd_stm32 -> hcd_handle, HAL_HCD_SofCallback);

  /* Register USB HCD Connect Callback. */
  HAL_HCD_RegisterPortConnectCallback(hcd_stm32 -> hcd_handle, HAL_HCD_PortConnectCallback);

  /* Register USB HCD Disconnect Callback. */
  HAL_HCD_RegisterPortDisconnectCallback(hcd_stm32 -> hcd_handle, HAL_HCD_PortDisconnectCallback);

  /* Register USB HCD Port Enabled Callback. */
  HAL_HCD_RegisterPortEnabledCallback(hcd_stm32 -> hcd_handle, HAL_HCD_PortEnabledCallback);

  /* Register USB HCD Port Disabled Callback. */
  HAL_HCD_RegisterPortDisabledCallback(hcd_stm32 -> hcd_handle, HAL_HCD_PortDisabledCallback);

  /* Register USB HCD Suspend Callback. */
  HAL_HCD_RegisterPortSuspendCallback(hcd_stm32 -> hcd_handle, HAL_HCD_PortSuspendCallback);

  /* Register USB HCD Resume Callback. */
  HAL_HCD_RegisterPortResumeCallback(hcd_stm32 -> hcd_handle, HAL_HCD_PortResumeCallback);

  /* Register USB HCD Host Channel Notify URB Change Callback. */
  HAL_HCD_RegisterChannelNotifyURBChangeCallback(hcd_stm32 -> hcd_handle, HAL_HCD_ChannelNotifyURBChangeCallback);

#endif /* (USE_HAL_HCD_REGISTER_CALLBACKS) */

  /* This function never fails.  */
  return(UX_SUCCESS);
}