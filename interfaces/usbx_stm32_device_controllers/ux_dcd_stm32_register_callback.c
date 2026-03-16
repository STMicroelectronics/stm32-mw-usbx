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
#define UX_DCD_STM32_SOURCE_CODE


/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_dcd_stm32.h"
#include "ux_device_stack.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_dcd_stm32_register_callback                     PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function will register USB PCD callbacks.                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    dcd_stm32                             Pointer to device controller  */
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
UINT _ux_dcd_stm32_register_callback(UX_DCD_STM32 *dcd_stm32, VOID *parameter)
{

#if defined (USE_HAL_PCD_REGISTER_CALLBACKS) && (USE_HAL_PCD_REGISTER_CALLBACKS == 1U)
  /* Register USB PCD Data OUT Stage Callback. */
  HAL_PCD_RegisterDataOutStageCallback(dcd_stm32 -> pcd_handle,
                                       HAL_PCD_DataOutStageCallback);

  /* Register USB PCD Data IN Stage Callback. */
  HAL_PCD_RegisterDataInStageCallback(dcd_stm32 -> pcd_handle,
                                      HAL_PCD_DataInStageCallback);

  /* Register USB PCD Setup Callback. */
  HAL_PCD_RegisterSetupCallback(dcd_stm32 -> pcd_handle,
                                HAL_PCD_SetupStageCallback);

  /* Register USB PCD Reset Callback. */
  HAL_PCD_RegisterResetCallback(dcd_stm32 -> pcd_handle,
                                HAL_PCD_ResetCallback);

  /* Register USB PCD SOF Callback. */
  HAL_PCD_RegisterSofCallback(dcd_stm32 -> pcd_handle,
                              HAL_PCD_SofCallback);

  /* Register USB PCD Suspend Callback .*/
  HAL_PCD_RegisterSuspendCallback(dcd_stm32 -> pcd_handle,
                                  HAL_PCD_SuspendCallback);

  /* Register USB PCD Resume Callback */
  HAL_PCD_RegisterResumeCallback(dcd_stm32 -> pcd_handle,
                                 HAL_PCD_ResumeCallback);

  /* Register USB PCD Connect Callback. */
  HAL_PCD_RegisterConnectCallback(dcd_stm32 -> pcd_handle,
                                  HAL_PCD_ConnectCallback);

  /* Register USB PCD Disconnect Callback. */
  HAL_PCD_RegisterDisconnectCallback(dcd_stm32 -> pcd_handle,
                                     HAL_PCD_DisconnectCallback);

  /* Register USB PCD Iso OUT incomplete Callback. */
  HAL_PCD_RegisterIsoOutIncpltCallback(dcd_stm32 -> pcd_handle,
                                       HAL_PCD_IsoOUTIncompleteCallback);

  /* Register USB PCD Iso IN incomplete Callback. */
  HAL_PCD_RegisterIsoInIncpltCallback(dcd_stm32 -> pcd_handle,
                                      HAL_PCD_IsoINIncompleteCallback);

  /* Register USB PCD LPM incomplete Callback. */
  HAL_PCD_RegisterLpmCallback(dcd_stm32 -> pcd_handle,
                              HAL_PCD_LpmCallback);

  /* Register USB PCD BCD incomplete Callback. */
  HAL_PCD_RegisterBcdCallback(dcd_stm32 -> pcd_handle,
                              HAL_PCD_BcdCallback);
#endif /* (USE_HAL_PCD_REGISTER_CALLBACKS) */

  /* This function never fails.  */
  return(UX_SUCCESS);
}