/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation
 * Copyright (c) 2026-present Eclipse ThreadX contributors
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
/**   Device Storage Class                                                */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define UX_SOURCE_CODE


/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_device_class_storage.h"
#include "ux_device_stack.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_device_class_storage_start_stop                 PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function starts or stops the media. The device load or eject   */
/*    the medium.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    storage                               Pointer to storage class      */
/*    lun                                   Logical unit number           */
/*    endpoint_in                           Pointer to IN endpoint        */
/*    endpoint_out                          Pointer to OUT endpoint       */
/*    cbwcb                                 Pointer to CBWCB              */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Device Storage Class                                                */
/*                                                                        */
/**************************************************************************/
UINT  _ux_device_class_storage_start_stop(UX_SLAVE_CLASS_STORAGE *storage, ULONG lun,
                                            UX_SLAVE_ENDPOINT *endpoint_in,
                                            UX_SLAVE_ENDPOINT *endpoint_out, UCHAR * cbwcb)
{

ULONG   power_condition;
ULONG   start;
ULONG   load_eject;
UINT    cb_status = UX_SUCCESS;

    UX_PARAMETER_NOT_USED(endpoint_in);
    UX_PARAMETER_NOT_USED(endpoint_out);

    /* If trace is enabled, insert this event into the trace buffer.  */
    UX_TRACE_IN_LINE_INSERT(UX_TRACE_DEVICE_CLASS_STORAGE_START_STOP, storage, lun, 0, 0, UX_TRACE_DEVICE_CLASS_EVENTS, 0, 0)

    /* Parse START/STOP UNIT (SBC 0x1B): byte 4 has POWER CONDITION (bits 7..4),
       LOEJ (bit 1) and START (bit 0).  */
    power_condition = (ULONG)((cbwcb[4] & 0xF0) >> 4);
    start           = (ULONG)(cbwcb[4] & 0x01);
    load_eject      = (ULONG)((cbwcb[4] & 0x02) >> 1);

    /* Reject EJECT (LOEJ=1, START=0) on a removable medium that is currently
       prevented from removal: terminate with CHECK CONDITION /
       ILLEGAL REQUEST / INVALID FIELD IN CDB-style error.  */
    if ((load_eject == 1) && (start == 0) &&
        (storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_prevent_medium_removal ==
             UX_SLAVE_CLASS_STORAGE_MEDIUM_REMOVAL_SHALL_BE_PREVENTED) &&
        (storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_media_removable_flag !=
             UX_SLAVE_CLASS_STORAGE_MEDIA_IS_NOT_REMOVABLE))
    {
        /* Update the REQUEST SENSE codes.  */
        storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_request_sense_status =
            UX_DEVICE_CLASS_STORAGE_SENSE_STATUS(UX_SLAVE_CLASS_STORAGE_SENSE_KEY_ILLEGAL_REQUEST,
                                                 UX_SLAVE_CLASS_STORAGE_ASC_KEY_INVALID_COMMAND, 0x00);

        /* Set the CSW with failure and return CSW back to host.  */
        storage -> ux_slave_class_storage_csw_status = UX_SLAVE_CLASS_STORAGE_CSW_FAILED;
        return(UX_SUCCESS);
    }

    /* Call the media start/stop function (if registered) BEFORE mutating the
       loaded state, so that the application can refuse the operation.  */
    if (storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_media_start_stop != UX_NULL)
    {
        cb_status = storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_media_start_stop(
                            storage, lun, power_condition, start, load_eject);
    }

    if (cb_status != UX_SUCCESS)
    {
        /* Application refused the request: set NOT READY for eject, HARDWARE ERROR for load.  */
        if ((load_eject == 1) && (start == 0))
        {
            storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_request_sense_status =
                UX_DEVICE_CLASS_STORAGE_SENSE_STATUS(UX_SLAVE_CLASS_STORAGE_SENSE_KEY_NOT_READY,
                                                     UX_SLAVE_CLASS_STORAGE_SENSE_CODE_NOT_PRESENT, 0x00);
        }
        else
        {
            storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_request_sense_status =
                UX_DEVICE_CLASS_STORAGE_SENSE_STATUS(UX_SLAVE_CLASS_STORAGE_SENSE_KEY_HARDWARE_ERROR,
                                                     UX_SLAVE_CLASS_STORAGE_SENSE_CODE_NOT_READY, 0x00);
        }
        storage -> ux_slave_class_storage_csw_status = UX_SLAVE_CLASS_STORAGE_CSW_FAILED;
        return(UX_SUCCESS);
    }

    /*  power_condition = 0, load_eject = 0 : no action regarding loading or ejecting the medium.
        power_condition = 0, load_eject = 1, start = 0 : unload the medium.
        power_condition = 0, load_eject = 1, start = 1 : load the medium.
        */
    if (power_condition == UX_SLAVE_CLASS_STORAGE_POWER_CONDITION_START_VALID)
    {
        if (load_eject != 0)
        {
            if (start == 0)
            {
                /* Eject media: mark as empty. */
                storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_medium_loaded_status =
                                                                    UX_SLAVE_CLASS_STORAGE_MEDIUM_EJECTED;
            }
            else
            {
                /* Load media: mark as present.  */
                storage -> ux_slave_class_storage_lun[lun].ux_slave_class_storage_medium_loaded_status =
                                                                    UX_SLAVE_CLASS_STORAGE_MEDIUM_LOADED;
            }
        }
    }

    /* We set the CSW with success.  */
    storage -> ux_slave_class_storage_csw_status = UX_SLAVE_CLASS_STORAGE_CSW_PASSED;

    /* Return successful completion.  */
    return(UX_SUCCESS);
}
