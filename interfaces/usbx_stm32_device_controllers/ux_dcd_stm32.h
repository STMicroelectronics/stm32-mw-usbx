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

/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    ux_dcd_stm32.h                                      PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the USB OTG device equivalences for the STM32     */
/*    controller.                                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s), used ST  */
/*                                            HAL library to drive the    */
/*                                            controller,                 */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Chaoqiong Xiao           Modified comment(s),          */
/*                                            added standalone support,   */
/*                                            added bi-dir EP support,    */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/

#ifndef UX_DCD_STM32_H
#define UX_DCD_STM32_H

#include "stm32_hal.h"

/* Define STM32 generic equivalences.  */

#define UX_DCD_STM32_SLAVE_CONTROLLER                           0x80
#define UX_DCD_STM32_IN_FIFO                                    3


#define UX_DCD_STM32_FLUSH_RX_FIFO                              0x00000010
#define UX_DCD_STM32_FLUSH_TX_FIFO                              0x00000020
#define UX_DCD_STM32_FLUSH_FIFO_ALL                             0x00000010
#define UX_DCD_STM32_ENDPOINT_SPACE_SIZE                        0x00000020
#define UX_DCD_STM32_ENDPOINT_CHANNEL_SIZE                      0x00000020


/* Define USB STM32 physical endpoint status definition.  */

#define UX_DCD_STM32_ED_STATUS_UNUSED                            0u
#define UX_DCD_STM32_ED_STATUS_USED                              1u
#define UX_DCD_STM32_ED_STATUS_TRANSFER                          2u
#define UX_DCD_STM32_ED_STATUS_STALLED                           4u
#define UX_DCD_STM32_ED_STATUS_DONE                              8u
#define UX_DCD_STM32_ED_STATUS_SETUP_IN                          (1u<<8)
#define UX_DCD_STM32_ED_STATUS_SETUP_STATUS                      (2u<<8)
#define UX_DCD_STM32_ED_STATUS_SETUP_OUT                         (3u<<8)
#define UX_DCD_STM32_ED_STATUS_SETUP                             (3u<<8)
#define UX_DCD_STM32_ED_STATUS_TASK_PENDING                      (1u<<10)

/* Define USB STM32 physical endpoint state machine definition.  */

#define UX_DCD_STM32_ED_STATE_IDLE                               0
#define UX_DCD_STM32_ED_STATE_DATA_TX                            1
#define UX_DCD_STM32_ED_STATE_DATA_RX                            2
#define UX_DCD_STM32_ED_STATE_STATUS_TX                          3
#define UX_DCD_STM32_ED_STATE_STATUS_RX                          4

/* Define USB STM32 device callback notification state definition.  */

#define UX_DCD_STM32_SOF_RECEIVED                                0xF0U
#define UX_DCD_STM32_DEVICE_CONNECTED                            0xF1U
#define UX_DCD_STM32_DEVICE_DISCONNECTED                         0xF2U
#define UX_DCD_STM32_DEVICE_RESUMED                              0xF3U
#define UX_DCD_STM32_DEVICE_SUSPENDED                            0xF4U
#define UX_DCD_STM32_DEVICE_BCD                                  0xF5U
#define UX_DCD_STM32_DEVICE_LPM                                  0xF6U

/* Define USB STM32 endpoint transfer status definition.  */

#define UX_DCD_STM32_ED_TRANSFER_STATUS_IDLE                     0
#define UX_DCD_STM32_ED_TRANSFER_STATUS_SETUP                    1
#define UX_DCD_STM32_ED_TRANSFER_STATUS_IN_COMPLETION            2
#define UX_DCD_STM32_ED_TRANSFER_STATUS_OUT_COMPLETION           3

/* Define USB STM32 physical endpoint structure.  */

typedef struct UX_DCD_STM32_ED_STRUCT
{
    struct UX_SLAVE_ENDPOINT_STRUCT
                    *ux_dcd_stm32_ed_endpoint;
    ULONG           ux_dcd_stm32_ed_status;
    UCHAR           ux_dcd_stm32_ed_state;
    UCHAR           ux_dcd_stm32_ed_index;
    UCHAR           ux_dcd_stm32_ed_direction;
    UCHAR           reserved;
} UX_DCD_STM32_ED;


/* Define USB STM32 DCD structure definition.  */

typedef struct UX_DCD_STM32_STRUCT
{

    struct UX_SLAVE_DCD_STRUCT
                        *ux_dcd_stm32_dcd_owner;
    struct UX_DCD_STM32_ED_STRUCT
                        *ux_dcd_stm32_ed;
#if defined(UX_DEVICE_BIDIRECTIONAL_ENDPOINT_SUPPORT)
    struct UX_DCD_STM32_ED_STRUCT
                        *ux_dcd_stm32_ed_in;
#endif /* defined(UX_DEVICE_BIDIRECTIONAL_ENDPOINT_SUPPORT) */
    hal_pcd_handle_t   *pcd_handle;
} UX_DCD_STM32;

static inline struct UX_DCD_STM32_ED_STRUCT *_stm32_ed_get(UX_DCD_STM32 *dcd_stm32, ULONG ep_addr)
{
#if defined(UX_DEVICE_BIDIRECTIONAL_ENDPOINT_SUPPORT)
ULONG ep_dir = ep_addr & 0x80u;
#endif /* defined(UX_DEVICE_BIDIRECTIONAL_ENDPOINT_SUPPORT) */
ULONG ep_num = ep_addr & 0x7Fu;

if (ep_num >= dcd_stm32->pcd_handle->endpoints_nbr)
        return(UX_NULL);

#if defined(UX_DEVICE_BIDIRECTIONAL_ENDPOINT_SUPPORT)
    if (ep_dir)
        return(dcd_stm32->ux_dcd_stm32_ed_in + ep_num);
#endif /* defined(UX_DEVICE_BIDIRECTIONAL_ENDPOINT_SUPPORT) */

    return(dcd_stm32->ux_dcd_stm32_ed + ep_num);
}

/* Define USB STM32 HAL PCD Callback prototypes.  */

VOID    HAL_PCD_SetupStageCallback(hal_pcd_handle_t *hpcd);
VOID    HAL_PCD_DataInStageCallback(hal_pcd_handle_t *hpcd, uint8_t ep_num);
VOID    HAL_PCD_DataOutStageCallback(hal_pcd_handle_t *hpcd, uint8_t ep_num);
VOID    HAL_PCD_ResetCallback(hal_pcd_handle_t *hpcd);
VOID    HAL_PCD_ConnectCallback(hal_pcd_handle_t *hpcd);
VOID    HAL_PCD_DisconnectCallback(hal_pcd_handle_t *hpcd);
VOID    HAL_PCD_SuspendCallback(hal_pcd_handle_t *hpcd);
VOID    HAL_PCD_ResumeCallback(hal_pcd_handle_t *hpcd);
VOID    HAL_PCD_SofCallback(hal_pcd_handle_t *hpcd);
VOID    HAL_PCD_IsoINIncompleteCallback(hal_pcd_handle_t *hpcd, uint8_t ep_num);
VOID    HAL_PCD_IsoOUTIncompleteCallback(hal_pcd_handle_t *hpcd, uint8_t ep_num);
VOID    HAL_PCD_LpmCallback(hal_pcd_handle_t *hpcd, hal_pcd_lpm_active_status_t lpm_status);
VOID    HAL_PCD_BcdCallback(hal_pcd_handle_t *hpcd, hal_pcd_bcd_port_type_t port_type);

/* Define USB STM32 DCD prototypes.  */

UINT    _ux_dcd_stm32_endpoint_create(UX_DCD_STM32 *dcd_stm32, UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_dcd_stm32_endpoint_destroy(UX_DCD_STM32 *dcd_stm32, UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_dcd_stm32_endpoint_reset(UX_DCD_STM32 *dcd_stm32, UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_dcd_stm32_endpoint_stall(UX_DCD_STM32 *dcd_stm32, UX_SLAVE_ENDPOINT *endpoint);
UINT    _ux_dcd_stm32_endpoint_status(UX_DCD_STM32 *dcd_stm32, ULONG endpoint_index);
UINT    _ux_dcd_stm32_frame_number_get(UX_DCD_STM32 *dcd_stm32, ULONG *frame_number);
UINT    _ux_dcd_stm32_function(UX_SLAVE_DCD *dcd, UINT function, VOID *parameter);
UINT    _ux_dcd_stm32_initialize_complete(VOID);
UINT    _ux_dcd_stm32_transfer_abort(UX_DCD_STM32 *dcd_stm32, UX_SLAVE_TRANSFER *transfer_request);
UINT    _ux_dcd_stm32_register_callback(UX_DCD_STM32 *dcd_stm32, VOID *parameter);
#if !defined(UX_DEVICE_STANDALONE)
UINT    _ux_dcd_stm32_transfer_request(UX_DCD_STM32 *dcd_stm32, UX_SLAVE_TRANSFER *transfer_request);
#else
UINT    _ux_dcd_stm32_transfer_run(UX_DCD_STM32 *dcd_stm32, UX_SLAVE_TRANSFER *transfer_request);
VOID    _ux_dcd_stm32_setup_isr_pending(UX_DCD_STM32 *dcd_stm32);
#endif /* !defined(UX_DEVICE_STANDALONE) */

UINT    _ux_dcd_stm32_initialize(ULONG dcd_io, ULONG parameter);
UINT    _ux_dcd_stm32_uninitialize(ULONG dcd_io, ULONG parameter);


#define ux_dcd_stm32_initialize                      _ux_dcd_stm32_initialize
#define ux_dcd_stm32_uninitialize                    _ux_dcd_stm32_uninitialize

#endif /* UX_DCD_STM32_H */

