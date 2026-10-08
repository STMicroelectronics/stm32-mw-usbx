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
/**   EHCI Controller Driver                                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/* Include necessary system files.  */

#define UX_SOURCE_CODE

#include "ux_api.h"
#include "ux_hcd_ehci.h"
#include "ux_host_stack.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _ux_hcd_ehci_request_transfer                       PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*     This function is the handler for all the transactions on the USB.  */
/*     The transfer request passed as parameter contains the endpoint and */
/*     the device descriptors in addition to the type of transaction de   */
/*     be executed.                                                       */
/*                                                                        */
/*     This function routes the transfer_request to according to the type */
/*     of transfer to be executed.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    hcd_ehci                              Pointer to EHCI controller    */
/*    transfer_request                      Pointer to transfer request   */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Completion Status                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_hcd_ehci_request_control_transfer     Start control transfer    */
/*    _ux_hcd_ehci_request_bulk_transfer        Start bulk transfer       */
/*    _ux_hcd_ehci_request_interrupt_transfer   Start interrupt transfer  */
/*    _ux_hcd_ehci_request_isochronous_transfer Start iso transfer        */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    EHCI Controller Driver                                              */
/*                                                                        */
/**************************************************************************/
UINT  _ux_hcd_ehci_request_transfer(UX_HCD_EHCI *hcd_ehci, UX_TRANSFER *transfer_request)
{

UX_ENDPOINT     *endpoint;
ULONG           endpoint_type;
UINT            status;


    if (transfer_request == UX_NULL)
    {
#if defined(UX_HOST_STANDALONE)
        return(UX_STATE_ERROR);
#else
        return(UX_ERROR);
#endif /* UX_HOST_STANDALONE */
    }

    /* Get the pointer to the Endpoint.  */
    endpoint =  (UX_ENDPOINT *) transfer_request -> ux_transfer_request_endpoint;

    if (endpoint == UX_NULL)
    {
#if defined(UX_HOST_STANDALONE)
        return(UX_STATE_ERROR);
#else
        return(UX_ERROR);
#endif /* UX_HOST_STANDALONE */
    }

    /* Reuse the endpoint type in the standalone gate and the common dispatch.  */
    endpoint_type = endpoint -> ux_endpoint_descriptor.bmAttributes & UX_MASK_ENDPOINT_TYPE;

#if defined(UX_HOST_STANDALONE)
    if (endpoint_type != UX_CONTROL_ENDPOINT)
    {
        UX_EHCI_ED *ed;

        ed = (UX_EHCI_ED *) endpoint -> ux_endpoint_ed;
        if (ed == UX_NULL)
        {
            return(UX_STATE_ERROR);
        }

        if (transfer_request -> ux_transfer_request_completion_code != UX_TRANSFER_STATUS_PENDING)
        {
            return((transfer_request -> ux_transfer_request_completion_code == UX_SUCCESS) ?
                   UX_STATE_NEXT : UX_STATE_ERROR);
        }

        if (ed -> ux_ehci_ed_first_td != UX_NULL)
        {
            return(UX_STATE_WAIT);
        }
    }
#endif /* UX_HOST_STANDALONE */

    /* We reset the actual length field of the transfer request as a safety measure.  */
    transfer_request -> ux_transfer_request_actual_length =  0;

    /* Isolate the endpoint type and route the transfer request.  */
    switch (endpoint_type)
    {

    case UX_CONTROL_ENDPOINT:

        status =  _ux_hcd_ehci_request_control_transfer(hcd_ehci, transfer_request);
        break;


    case UX_BULK_ENDPOINT:

        status =  _ux_hcd_ehci_request_bulk_transfer(hcd_ehci, transfer_request);
        break;


    case UX_INTERRUPT_ENDPOINT:

        status =  _ux_hcd_ehci_request_interrupt_transfer(hcd_ehci, transfer_request);
        break;


    case UX_ISOCHRONOUS_ENDPOINT:

        status =  _ux_hcd_ehci_request_isochronous_transfer(hcd_ehci, transfer_request);
        break;

    default:
#if defined(UX_HOST_STANDALONE)
        return(UX_STATE_ERROR);
#else
        status = UX_ERROR;
        break;
#endif /* UX_HOST_STANDALONE */

    }

#if defined(UX_HOST_STANDALONE)
    if (endpoint_type == UX_CONTROL_ENDPOINT)
    {
        return((status == UX_SUCCESS) ? UX_STATE_NEXT : UX_STATE_ERROR);
    }

    return((status == UX_SUCCESS) ? UX_STATE_WAIT : UX_STATE_ERROR);
#else
    /* Note that it is physically impossible to have a wrong endpoint type here
       so no error checking.  */
    return(status);         
#endif /* UX_HOST_STANDALONE */
}

