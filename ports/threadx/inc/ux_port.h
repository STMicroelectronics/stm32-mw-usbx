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
/**   Port Specific                                                       */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  PORT SPECIFIC C INFORMATION                            RELEASE        */
/*                                                                        */
/*    ux_port.h                                            Generic        */
/*                                                           6.4.1        */
/*                                                                        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains data type definitions that make USBX function    */
/*    identically on a variety of different processor architectures.      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  12-31-2020     Chaoqiong Xiao           Initial Version 6.1.3         */
/*  01-31-2022     Chaoqiong Xiao           Modified comment(s),          */
/*                                            moved tx_api.h include and  */
/*                                            typedefs from ux_api.h,     */
/*                                            resulting in version 6.1.10 */
/*  21-05-2024     STMicroelectronics       Modified the code             */
/*                                            to be RTOS agnostic support */
/*                                            resulting in version 6.4.1  */
/*                                                                        */
/**************************************************************************/

#ifndef UX_PORT_H
#define UX_PORT_H


/* Determine if the optional USBX user define file should be used.  */

#ifdef UX_INCLUDE_USER_DEFINE_FILE

/* Yes, include the user defines in ux_user.h. The defines in this file may
   alternately be defined on the command line.  */

#include "ux_user.h"
#endif


/* Include library header files.  */

#include <stdio.h>
#include <string.h>
#include "tx_api.h"


/* CPU definition for X86 systems without preemptive timer function.
   This will make USBX uses the controller for the timer. */

#undef THREADX_X86_NO_PTIMER


/* For X86 systems, the define #define UX_USE_IO_INSTRUCTIONS should be used.  */


/* Define additional generic USBX types.  */

typedef long                        SLONG;


/*  Generic USBX Project constants follow.  */

#ifndef UX_PERIODIC_RATE
#define UX_PERIODIC_RATE                                    100
#endif

#ifndef UX_MAX_CLASS_DRIVER
#define UX_MAX_CLASS_DRIVER                                 2
#endif

#ifndef UX_MAX_SLAVE_CLASS_DRIVER
#define UX_MAX_SLAVE_CLASS_DRIVER                           2
#endif

#ifndef UX_MAX_HCD
#define UX_MAX_HCD                                          1
#endif

#ifndef UX_MAX_DEVICES
#define UX_MAX_DEVICES                                      2
#endif

#ifndef UX_MAX_ED
#define UX_MAX_ED                                           80
#endif

#ifndef UX_MAX_TD
#define UX_MAX_TD                                           32
#endif

#ifndef UX_MAX_ISO_TD
#define UX_MAX_ISO_TD                                       2
#endif

#ifndef UX_HOST_ENUM_THREAD_STACK_SIZE
#define UX_HOST_ENUM_THREAD_STACK_SIZE                      (2*1024)
#endif

#ifndef UX_THREAD_STACK_SIZE
#define UX_THREAD_STACK_SIZE                                (1*1024)
#endif

#ifndef UX_THREAD_PRIORITY_ENUM
#define UX_THREAD_PRIORITY_ENUM                             20
#endif

#ifndef UX_THREAD_PRIORITY_CLASS
#define UX_THREAD_PRIORITY_CLASS                            20
#endif

#ifndef UX_THREAD_PRIORITY_KEYBOARD
#define UX_THREAD_PRIORITY_KEYBOARD                         20
#endif

#ifndef UX_HOST_OS_CLASS_STORAGE_THREAD_PRIORITY_CLASS
#define UX_HOST_OS_CLASS_STORAGE_THREAD_PRIORITY_CLASS      20
#endif

#ifndef UX_PICTBRIDGE_OS_THREAD_PRIORITY_CLASS
#define UX_PICTBRIDGE_OS_THREAD_PRIORITY_CLASS              20
#endif

#ifndef UX_THREAD_PRIORITY_HCD
#define UX_THREAD_PRIORITY_HCD                              2
#endif

#ifndef UX_THREAD_PRIORITY_DCD
#define UX_THREAD_PRIORITY_DCD                              2
#endif

#ifndef UX_NO_TIME_SLICE
#define UX_NO_TIME_SLICE                                    0
#endif

#ifndef UX_MAX_SLAVE_LUN
#define UX_MAX_SLAVE_LUN                                    1
#endif

#ifndef UX_MAX_HOST_LUN
#define UX_MAX_HOST_LUN                                     1
#endif

#ifndef UX_HOST_CLASS_STORAGE_MAX_MEDIA
#define UX_HOST_CLASS_STORAGE_MAX_MEDIA                     1
#endif

#ifndef UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH
#define UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH                 256
#endif


#ifndef UX_SLAVE_REQUEST_DATA_MAX_LENGTH
#define UX_SLAVE_REQUEST_DATA_MAX_LENGTH                    2048
#endif

#ifndef UX_USE_IO_INSTRUCTIONS

/* Don't use IO instructions if this define is not set.  Default to memory mapped.  */

#define inpb(a)                                            *((UCHAR *)  (a))
#define inpw(a)                                            *((USHORT *) (a))
#define inpl(a)                                            *((ULONG *)  (a))
#define outpb(a, b)                                        *((UCHAR *)  (a)) =  ((UCHAR)  (b))
#define outpw(a, b)                                        *((USHORT *) (a)) =  ((USHORT) (b))
#define outpl(a, b)                                        *((ULONG *)  (a)) =  ((ULONG)  (b))
#else


/* Define simple prototypes for non-memory mapped hardware access.  */

UCHAR   inpb(ULONG);
USHORT  inpw(ULONG);
ULONG   inpl(ULONG);

VOID    outpb(ULONG,UCHAR);
VOID    outpw(ULONG,USHORT);
VOID    outpl(ULONG,ULONG);

#endif
/* Define local delay function for board specific bsps.  */
#ifdef TI_AM335
    #define UX_BSP_SPECIFIC_DELAY_FUNCTION
#endif


/* Define interrupt lockout constructs to protect the memory allocation/release which could happen
   under ISR in the device stack.  */

#define UX_INT_SAVE_AREA        unsigned int  old_interrupt_posture;
#define UX_DISABLE_INTS         old_interrupt_posture =  tx_interrupt_control(TX_INT_DISABLE);
#define UX_RESTORE_INTS         tx_interrupt_control(old_interrupt_posture);

/* Determine if tracing is enabled.  */
#if defined(TX_ENABLE_EVENT_TRACE) && !defined(UX_STANDALONE)
#define UX_ENABLE_EVENT_TRACE
#endif

#ifdef UX_ENABLE_EVENT_TRACE
/* Trace is enabled. Remap calls so that interrupts can be disabled around the actual event logging.  */
#include "tx_trace.h"

/* Map the trace macros to internal USBX versions so we can get interrupt protection.  */
#define UX_TRACE_OBJECT_REGISTER(t,p,n,a,b)                 _ux_os_trace_object_register(t, (VOID *) p, (CHAR *) n, (ULONG) a, (ULONG) b);
#define UX_TRACE_OBJECT_UNREGISTER(o)                       _ux_os_trace_object_unregister((VOID *) o);
#define UX_TRACE_IN_LINE_INSERT(i,a,b,c,d,f,g,h)            _ux_os_trace_event_insert((ULONG) i, (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) d, (ULONG) f, g, h);
#define UX_TRACE_EVENT_UPDATE(e,t,i,a,b,c,d)                _ux_os_trace_event_update((TX_TRACE_BUFFER_ENTRY *) e, (ULONG) t, (ULONG) i, (ULONG) a, (ULONG) b, (ULONG) c, (ULONG) d);

/* Define USBX trace prototypes.  */
VOID    _ux_os_trace_object_register(UCHAR object_type, VOID *object_ptr, CHAR *object_name, ULONG parameter_1, ULONG parameter_2);
VOID    _ux_os_trace_object_unregister(VOID *object_ptr);
VOID    _ux_os_trace_event_insert(ULONG event_id, ULONG info_field_1, ULONG info_field_2, ULONG info_field_3, ULONG info_field_4, ULONG filter, TX_TRACE_BUFFER_ENTRY **current_event, ULONG *current_timestamp);
VOID    _ux_os_trace_event_update(TX_TRACE_BUFFER_ENTRY *event, ULONG timestamp, ULONG event_id, ULONG info_field_1, ULONG info_field_2, ULONG info_field_3, ULONG info_field_4);

/* Define USBX event trace constants.  */
#define UX_TRACE_OBJECT_TYPE_BASE                                       20
#define UX_TRACE_HOST_OBJECT_TYPE_DEVICE                                (UX_TRACE_OBJECT_TYPE_BASE + 1)
#define UX_TRACE_HOST_OBJECT_TYPE_INTERFACE                             (UX_TRACE_OBJECT_TYPE_BASE + 2)
#define UX_TRACE_HOST_OBJECT_TYPE_ENDPOINT                              (UX_TRACE_OBJECT_TYPE_BASE + 3)
#define UX_TRACE_HOST_OBJECT_TYPE_CLASS_INSTANCE                        (UX_TRACE_OBJECT_TYPE_BASE + 4)

#define UX_TRACE_DEVICE_OBJECT_TYPE_DEVICE                              (UX_TRACE_OBJECT_TYPE_BASE + 5)
#define UX_TRACE_DEVICE_OBJECT_TYPE_INTERFACE                           (UX_TRACE_OBJECT_TYPE_BASE + 6)
#define UX_TRACE_DEVICE_OBJECT_TYPE_ENDPOINT                            (UX_TRACE_OBJECT_TYPE_BASE + 7)
#define UX_TRACE_DEVICE_OBJECT_TYPE_CLASS_INSTANCE                      (UX_TRACE_OBJECT_TYPE_BASE + 8)

/* Define event filters that can be used to selectively disable certain events or groups of events.  */
#define UX_TRACE_ALL_EVENTS                                             0x7F000000  /* All USBX events                          */
#define UX_TRACE_ERRORS                                                 0x01000000  /* USBX Errors events                       */
#define UX_TRACE_HOST_STACK_EVENTS                                      0x02000000  /* USBX Host Class Events                   */
#define UX_TRACE_DEVICE_STACK_EVENTS                                    0x04000000  /* USBX Device Class Events                 */
#define UX_TRACE_HOST_CONTROLLER_EVENTS                                 0x08000000  /* USBX Host Controller Events              */
#define UX_TRACE_DEVICE_CONTROLLER_EVENTS                               0x10000000  /* USBX Device Controllers Events           */
#define UX_TRACE_HOST_CLASS_EVENTS                                      0x20000000  /* USBX Host Class Events                   */
#define UX_TRACE_DEVICE_CLASS_EVENTS                                    0x40000000  /* USBX Device Class Events                 */


/* Define the trace events in USBX, if not defined.  */

/* Define the USBX host stack events.  */
#define UX_TRACE_HOST_STACK_EVENTS_BASE                                 600
#define UX_TRACE_HOST_STACK_CLASS_INSTANCE_CREATE                       (UX_TRACE_HOST_STACK_EVENTS_BASE + 1)               /* I1 = class           , I2 = class instance                                                       */
#define UX_TRACE_HOST_STACK_CLASS_INSTANCE_DESTROY                      (UX_TRACE_HOST_STACK_EVENTS_BASE + 2)               /* I1 = class           , I2 = class instance                                                       */
#define UX_TRACE_HOST_STACK_CONFIGURATION_DELETE                        (UX_TRACE_HOST_STACK_EVENTS_BASE + 3)               /* I1 = configuration                                                                               */
#define UX_TRACE_HOST_STACK_CONFIGURATION_ENUMERATE                     (UX_TRACE_HOST_STACK_EVENTS_BASE + 4)               /* I1 = device                                                                                      */
#define UX_TRACE_HOST_STACK_CONFIGURATION_INSTANCE_CREATE               (UX_TRACE_HOST_STACK_EVENTS_BASE + 5)               /* I1 = configuration                                                                               */
#define UX_TRACE_HOST_STACK_CONFIGURATION_INSTANCE_DELETE               (UX_TRACE_HOST_STACK_EVENTS_BASE + 6)               /* I1 = configuration                                                                               */
#define UX_TRACE_HOST_STACK_CONFIGURATION_SET                           (UX_TRACE_HOST_STACK_EVENTS_BASE + 7)               /* I1 = configuration                                                                               */
#define UX_TRACE_HOST_STACK_DEVICE_ADDRESS_SET                          (UX_TRACE_HOST_STACK_EVENTS_BASE + 8)               /* I1 = device          , I2 = device address                                                       */
#define UX_TRACE_HOST_STACK_DEVICE_CONFIGURATION_GET                    (UX_TRACE_HOST_STACK_EVENTS_BASE + 9)               /* I1 = device          , I2 = configuration                                                        */
#define UX_TRACE_HOST_STACK_DEVICE_CONFIGURATION_SELECT                 (UX_TRACE_HOST_STACK_EVENTS_BASE + 10)              /* I1 = device          , I2 = configuration                                                        */
#define UX_TRACE_HOST_STACK_DEVICE_DESCRIPTOR_READ                      (UX_TRACE_HOST_STACK_EVENTS_BASE + 11)              /* I1 = device                                                                                      */
#define UX_TRACE_HOST_STACK_DEVICE_GET                                  (UX_TRACE_HOST_STACK_EVENTS_BASE + 12)              /* I1 = device index                                                                                */
#define UX_TRACE_HOST_STACK_DEVICE_REMOVE                               (UX_TRACE_HOST_STACK_EVENTS_BASE + 13)              /* I1 = hcd             , I2 = parent          , I3 = port index        , I4 = device               */
#define UX_TRACE_HOST_STACK_DEVICE_RESOURCE_FREE                        (UX_TRACE_HOST_STACK_EVENTS_BASE + 14)              /* I1 = device                                                                                      */
#define UX_TRACE_HOST_STACK_ENDPOINT_INSTANCE_CREATE                    (UX_TRACE_HOST_STACK_EVENTS_BASE + 15)              /* I1 = device          , I2 = endpoint                                                             */
#define UX_TRACE_HOST_STACK_ENDPOINT_INSTANCE_DELETE                    (UX_TRACE_HOST_STACK_EVENTS_BASE + 16)              /* I1 = device          , I2 = endpoint                                                             */
#define UX_TRACE_HOST_STACK_ENDPOINT_RESET                              (UX_TRACE_HOST_STACK_EVENTS_BASE + 17)              /* I1 = device          , I2 = endpoint                                                             */
#define UX_TRACE_HOST_STACK_ENDPOINT_TRANSFER_ABORT                     (UX_TRACE_HOST_STACK_EVENTS_BASE + 18)              /* I1 = endpoint                                                                                    */
#define UX_TRACE_HOST_STACK_HCD_REGISTER                                (UX_TRACE_HOST_STACK_EVENTS_BASE + 19)              /* I1 = hcd name        , I2 = parameter 1     , I3 = parameter 2                                   */
#define UX_TRACE_HOST_STACK_INITIALIZE                                  (UX_TRACE_HOST_STACK_EVENTS_BASE + 20)              /*                                                                                                  */
#define UX_TRACE_HOST_STACK_INTERFACE_ENDPOINT_GET                      (UX_TRACE_HOST_STACK_EVENTS_BASE + 21)              /* I1 = interface       , I2 = endpoint index                                                       */
#define UX_TRACE_HOST_STACK_INTERFACE_INSTANCE_CREATE                   (UX_TRACE_HOST_STACK_EVENTS_BASE + 22)              /* I1 = interface                                                                                   */
#define UX_TRACE_HOST_STACK_INTERFACE_INSTANCE_DELETE                   (UX_TRACE_HOST_STACK_EVENTS_BASE + 23)              /* I1 = interface                                                                                   */
#define UX_TRACE_HOST_STACK_INTERFACE_SET                               (UX_TRACE_HOST_STACK_EVENTS_BASE + 24)              /* I1 = interface                                                                                   */
#define UX_TRACE_HOST_STACK_INTERFACE_SETTING_SELECT                    (UX_TRACE_HOST_STACK_EVENTS_BASE + 25)              /* I1 = interface                                                                                   */
#define UX_TRACE_HOST_STACK_NEW_CONFIGURATION_CREATE                    (UX_TRACE_HOST_STACK_EVENTS_BASE + 26)              /* I1 = device          , I2 = configuration                                                        */
#define UX_TRACE_HOST_STACK_NEW_DEVICE_CREATE                           (UX_TRACE_HOST_STACK_EVENTS_BASE + 27)              /* I1 = hcd             , I2 = device owner    , I3 = port index        , I4 = device               */
#define UX_TRACE_HOST_STACK_NEW_ENDPOINT_CREATE                         (UX_TRACE_HOST_STACK_EVENTS_BASE + 28)              /* I1 = interface       , I2 = endpoint                                                             */
#define UX_TRACE_HOST_STACK_RH_CHANGE_PROCESS                           (UX_TRACE_HOST_STACK_EVENTS_BASE + 29)              /* I1 = port index                                                                                  */
#define UX_TRACE_HOST_STACK_RH_DEVICE_EXTRACTION                        (UX_TRACE_HOST_STACK_EVENTS_BASE + 30)              /* I1 = hcd             , I2 = port index                                                           */
#define UX_TRACE_HOST_STACK_RH_DEVICE_INSERTION                         (UX_TRACE_HOST_STACK_EVENTS_BASE + 31)              /* I1 = hcd             , I2 = port index                                                           */
#define UX_TRACE_HOST_STACK_TRANSFER_REQUEST                            (UX_TRACE_HOST_STACK_EVENTS_BASE + 32)              /* I1 = device          , I2 = endpoint        , I3 = transfer request                              */
#define UX_TRACE_HOST_STACK_TRANSFER_REQUEST_ABORT                      (UX_TRACE_HOST_STACK_EVENTS_BASE + 33)              /* I1 = device          , I2 = endpoint        , I3 = transfer request                              */
#define UX_TRACE_HOST_STACK_UNINITIALIZE                                (UX_TRACE_HOST_STACK_EVENTS_BASE + 34)              /*                                                                                                  */
#define UX_TRACE_HOST_STACK_HCD_UNREGISTER                              (UX_TRACE_HOST_STACK_EVENTS_BASE + 35)              /* I1 = hcd name        , I2 = parameter 1     , I3 = parameter 2                                   */
#define UX_TRACE_HOST_STACK_CLASS_REGISTER                              (UX_TRACE_HOST_STACK_EVENTS_BASE + 36)              /* I1 = class name      , I2 = entry function                                                       */
#define UX_TRACE_HOST_STACK_CLASS_UNREGISTER                            (UX_TRACE_HOST_STACK_EVENTS_BASE + 37)              /* I1 = class entry                                                                                 */
#define UX_TRACE_HOST_STACK_DEVICE_STRING_GET                           (UX_TRACE_HOST_STACK_EVENTS_BASE + 38)              /* I1 = device          , I2 = buffer          , I3 = length            , I4 = (langID<<16) | index */
#define UX_TRACE_HOST_STACK_DEVICE_CONFIGURATION_ACTIVATE               (UX_TRACE_HOST_STACK_EVENTS_BASE + 39)              /* I1 = device          , I2 = configuration                                                        */
#define UX_TRACE_HOST_STACK_DEVICE_CONFIGURATION_DEACTIVATE             (UX_TRACE_HOST_STACK_EVENTS_BASE + 40)              /* I1 = device          , I2 = configuration                                                        */

/* Define the USBX host class events.  */

#define UX_TRACE_HOST_CLASS_EVENTS_BASE                                 650
#define UX_TRACE_HOST_CLASS_ASIX_ACTIVATE                               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 1)               /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_ASIX_DEACTIVATE                             (UX_TRACE_HOST_CLASS_EVENTS_BASE + 2)               /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_ASIX_INTERRUPT_NOTIFICATION                 (UX_TRACE_HOST_CLASS_EVENTS_BASE + 3)               /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_ASIX_READ                                   (UX_TRACE_HOST_CLASS_EVENTS_BASE + 4)               /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_ASIX_WRITE                                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 5)               /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */

#define UX_TRACE_HOST_CLASS_AUDIO_ACTIVATE                              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 10)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_AUDIO_CONTROL_VALUE_GET                     (UX_TRACE_HOST_CLASS_EVENTS_BASE + 11)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_AUDIO_CONTROL_VALUE_SET                     (UX_TRACE_HOST_CLASS_EVENTS_BASE + 12)              /* I1 = class instance  , I2 = audio control                                                        */
#define UX_TRACE_HOST_CLASS_AUDIO_DEACTIVATE                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 13)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_AUDIO_READ                                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 14)              /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_AUDIO_STREAMING_SAMPLING_GET                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 15)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_AUDIO_STREAMING_SAMPLING_SET                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 16)              /* I1 = class instance  , I2 = audio sampling                                                       */
#define UX_TRACE_HOST_CLASS_AUDIO_WRITE                                 (UX_TRACE_HOST_CLASS_EVENTS_BASE + 17)              /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */

#define UX_TRACE_HOST_CLASS_CDC_ACM_ACTIVATE                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 20)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_CDC_ACM_DEACTIVATE                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 21)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_CDC_ACM_IOCTL_SET_LINE_CODING               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 22)              /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_CDC_ACM_IOCTL_GET_LINE_CODING               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 23)              /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_CDC_ACM_IOCTL_SET_LINE_STATE                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 24)              /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_CDC_ACM_IOCTL_SEND_BREAK                    (UX_TRACE_HOST_CLASS_EVENTS_BASE + 25)              /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_CDC_ACM_IOCTL_ABORT_IN_PIPE                 (UX_TRACE_HOST_CLASS_EVENTS_BASE + 26)              /* I1 = class instance  , I2 = endpoint                                                             */
#define UX_TRACE_HOST_CLASS_CDC_ACM_IOCTL_ABORT_OUT_PIPE                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 27)              /* I1 = class instance  , I2 = endpointr                                                            */
#define UX_TRACE_HOST_CLASS_CDC_ACM_IOCTL_NOTIFICATION_CALLBACK         (UX_TRACE_HOST_CLASS_EVENTS_BASE + 28)              /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_CDC_ACM_IOCTL_GET_DEVICE_STATUS             (UX_TRACE_HOST_CLASS_EVENTS_BASE + 29)              /* I1 = class instance  , I2 = device status                                                        */
#define UX_TRACE_HOST_CLASS_CDC_ACM_READ                                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 30)              /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_CDC_ACM_RECEPTION_START                     (UX_TRACE_HOST_CLASS_EVENTS_BASE + 31)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_CDC_ACM_RECEPTION_STOP                      (UX_TRACE_HOST_CLASS_EVENTS_BASE + 32)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_CDC_ACM_WRITE                               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 33)              /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */

#define UX_TRACE_HOST_CLASS_CDC_ECM_ACTIVATE                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 35)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_CDC_ECM_DEACTIVATE                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 36)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_CDC_ECM_READ                                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 37)              /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_CDC_ECM_WRITE                               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 38)              /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_CDC_ECM_INTERRUPT_NOTIFICATION              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 39)              /* I1 = class instance                                                                              */

#define UX_TRACE_HOST_CLASS_HID_ACTIVATE                                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 40)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_HID_CLIENT_REGISTER                         (UX_TRACE_HOST_CLASS_EVENTS_BASE + 41)              /* I1 = hid client name                                                                             */
#define UX_TRACE_HOST_CLASS_HID_DEACTIVATE                              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 42)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_HID_IDLE_GET                                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 43)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_HID_IDLE_SET                                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 44)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_HID_KEYBOARD_ACTIVATE                       (UX_TRACE_HOST_CLASS_EVENTS_BASE + 45)              /* I1 = class instance  , I2 = hid client instance                                                  */
#define UX_TRACE_HOST_CLASS_HID_KEYBOARD_DEACTIVATE                     (UX_TRACE_HOST_CLASS_EVENTS_BASE + 46)              /* I1 = class instance  , I2 = hid client instance                                                  */
#define UX_TRACE_HOST_CLASS_HID_MOUSE_ACTIVATE                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 47)              /* I1 = class instance  , I2 = hid client instance                                                  */
#define UX_TRACE_HOST_CLASS_HID_MOUSE_DEACTIVATE                        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 48)              /* I1 = class instance  , I2 = hid client instance                                                  */
#define UX_TRACE_HOST_CLASS_HID_REMOTE_CONTROL_ACTIVATE                 (UX_TRACE_HOST_CLASS_EVENTS_BASE + 49)              /* I1 = class instance  , I2 = hid client instance                                                  */
#define UX_TRACE_HOST_CLASS_HID_REMOTE_CONTROL_DEACTIVATE               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 50)              /* I1 = class instance  , I2 = hid client instance                                                  */
#define UX_TRACE_HOST_CLASS_HID_REPORT_GET                              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 51)              /* I1 = class instance  , I2 = client report                                                        */
#define UX_TRACE_HOST_CLASS_HID_REPORT_SET                              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 52)              /* I1 = class instance  , I2 = client report                                                        */
#define UX_TRACE_HOST_CLASS_HID_REMOTE_CONTROL_CALLBACK                 (UX_TRACE_HOST_CLASS_EVENTS_BASE + 53)              /* I1 = client instance , I2 = remote control instance                                              */

#define UX_TRACE_HOST_CLASS_HUB_ACTIVATE                                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 60)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_HUB_CHANGE_DETECT                           (UX_TRACE_HOST_CLASS_EVENTS_BASE + 62)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_HUB_PORT_CHANGE_CONNECTION_PROCESS          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 63)              /* I1 = class instance  , I2 = port            , I3 = port status                                   */
#define UX_TRACE_HOST_CLASS_HUB_PORT_CHANGE_ENABLE_PROCESS              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 64)              /* I1 = class instance  , I2 = port            , I3 = port status                                   */
#define UX_TRACE_HOST_CLASS_HUB_PORT_CHANGE_OVER_CURRENT_PROCESS        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 65)              /* I1 = class instance  , I2 = port            , I3 = port status                                   */
#define UX_TRACE_HOST_CLASS_HUB_PORT_CHANGE_RESET_PROCESS               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 66)              /* I1 = class instance  , I2 = port            , I3 = port status                                   */
#define UX_TRACE_HOST_CLASS_HUB_PORT_CHANGE_SUSPEND_PROCESS             (UX_TRACE_HOST_CLASS_EVENTS_BASE + 67)              /* I1 = class instance  , I2 = port            , I3 = port status                                   */
#define UX_TRACE_HOST_CLASS_HUB_DEACTIVATE                              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 68)              /* I1 = class instance                                                                              */

#define UX_TRACE_HOST_CLASS_PIMA_ACTIVATE                               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 70)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PIMA_DEACTIVATE                             (UX_TRACE_HOST_CLASS_EVENTS_BASE + 71)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PIMA_DEVICE_INFO_GET                        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 72)              /* I1 = class instance  , I2 = pima device                                                          */
#define UX_TRACE_HOST_CLASS_PIMA_DEVICE_RESET                           (UX_TRACE_HOST_CLASS_EVENTS_BASE + 73)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PIMA_NOTIFICATION                           (UX_TRACE_HOST_CLASS_EVENTS_BASE + 74)              /* I1 = class instance  , I2 = event code      , I3 = transaction ID    , I4 = parameter1           */
#define UX_TRACE_HOST_CLASS_PIMA_NUM_OBJECTS_GET                        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 75)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_CLOSE                           (UX_TRACE_HOST_CLASS_EVENTS_BASE + 76)              /* I1 = class instance  , I2 = object                                                               */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_COPY                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 77)              /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_DELETE                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 78)              /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_GET                             (UX_TRACE_HOST_CLASS_EVENTS_BASE + 79)              /* I1 = class instance  , I2 = object handle   , I3 = object                                        */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_INFO_GET                        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 80)              /* I1 = class instance  , I2 = object handle   , I3 = object                                        */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_INFO_SEND                       (UX_TRACE_HOST_CLASS_EVENTS_BASE + 81)              /* I1 = class instance  , I2 = object                                                               */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_MOVE                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 82)              /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_SEND                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 83)              /* I1 = class instance  , I2 = object          , I3 = object_buffer     , I4 = object length        */
#define UX_TRACE_HOST_CLASS_PIMA_OBJECT_TRANSFER_ABORT                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 84)              /* I1 = class instance  , I2 = object handle   , I3 = object                                        */
#define UX_TRACE_HOST_CLASS_PIMA_READ                                   (UX_TRACE_HOST_CLASS_EVENTS_BASE + 85)              /* I1 = class instance  , I2 = data pointer    , I3 = data length                                   */
#define UX_TRACE_HOST_CLASS_PIMA_REQUEST_CANCEL                         (UX_TRACE_HOST_CLASS_EVENTS_BASE + 86)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PIMA_SESSION_CLOSE                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 87)              /* I1 = class instance  , I2 = pima session                                                         */
#define UX_TRACE_HOST_CLASS_PIMA_SESSION_OPEN                           (UX_TRACE_HOST_CLASS_EVENTS_BASE + 88)              /* I1 = class instance  , I2 = pima session                                                         */
#define UX_TRACE_HOST_CLASS_PIMA_STORAGE_IDS_GET                        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 89)              /* I1 = class instance  , I2 = storage ID array, I3 = storage ID length                             */
#define UX_TRACE_HOST_CLASS_PIMA_STORAGE_INFO_GET                       (UX_TRACE_HOST_CLASS_EVENTS_BASE + 90)              /* I1 = class instance  , I2 = storage ID      , I3 = storage                                       */
#define UX_TRACE_HOST_CLASS_PIMA_THUMB_GET                              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 91)              /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_HOST_CLASS_PIMA_WRITE                                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 92)              /* I1 = class instance  , I2 = data pointer    , I3 = data length                                   */

#define UX_TRACE_HOST_CLASS_PRINTER_ACTIVATE                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 100)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PRINTER_DEACTIVATE                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 101)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PRINTER_NAME_GET                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 102)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PRINTER_READ                                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 103)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_PRINTER_WRITE                               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 104)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_PRINTER_SOFT_RESET                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 105)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PRINTER_STATUS_GET                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 106)             /* I1 = class instance  , I2 = printer status                                                       */
#define UX_TRACE_HOST_CLASS_PRINTER_DEVICE_ID_GET                       (UX_TRACE_HOST_CLASS_EVENTS_BASE + 107)             /* I1 = class instance  , I2 = printer         , I3 = data pointer      , I4 = buffer length        */

#define UX_TRACE_HOST_CLASS_PROLIFIC_ACTIVATE                           (UX_TRACE_HOST_CLASS_EVENTS_BASE + 110)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PROLIFIC_DEACTIVATE                         (UX_TRACE_HOST_CLASS_EVENTS_BASE + 111)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_SET_LINE_CODING              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 112)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_GET_LINE_CODING              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 113)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_SET_LINE_STATE               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 114)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_PURGE                        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 115)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_SEND_BREAK                   (UX_TRACE_HOST_CLASS_EVENTS_BASE + 116)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_ABORT_IN_PIPE                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 117)             /* I1 = class instance  , I2 = endpoint                                                             */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_ABORT_OUT_PIPE               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 118)             /* I1 = class instance  , I2 = endpointr                                                            */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_REPORT_DEVICE_STATUS_CHANGE  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 119)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_PROLIFIC_IOCTL_GET_DEVICE_STATUS            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 120)             /* I1 = class instance  , I2 = device status                                                        */
#define UX_TRACE_HOST_CLASS_PROLIFIC_READ                               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 121)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_PROLIFIC_RECEPTION_START                    (UX_TRACE_HOST_CLASS_EVENTS_BASE + 122)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PROLIFIC_RECEPTION_STOP                     (UX_TRACE_HOST_CLASS_EVENTS_BASE + 123)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_PROLIFIC_WRITE                              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 124)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */

#define UX_TRACE_HOST_CLASS_STORAGE_ACTIVATE                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 130)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_STORAGE_DEACTIVATE                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 131)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_STORAGE_MEDIA_CAPACITY_GET                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 132)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_STORAGE_MEDIA_FORMAT_CAPACITY_GET           (UX_TRACE_HOST_CLASS_EVENTS_BASE + 133)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_STORAGE_MEDIA_MOUNT                         (UX_TRACE_HOST_CLASS_EVENTS_BASE + 134)             /* I1 = class instance  , I2 = sector                                                               */
#define UX_TRACE_HOST_CLASS_STORAGE_MEDIA_OPEN                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 135)             /* I1 = class instance  , I2 = media                                                                */
#define UX_TRACE_HOST_CLASS_STORAGE_MEDIA_READ                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 136)             /* I1 = class instance  , I2 = sector start    , I3 = sector count      , I4 = data pointer         */
#define UX_TRACE_HOST_CLASS_STORAGE_MEDIA_WRITE                         (UX_TRACE_HOST_CLASS_EVENTS_BASE + 137)             /* I1 = class instance  , I2 = sector start    , I3 = sector count      , I4 = data pointer         */
#define UX_TRACE_HOST_CLASS_STORAGE_REQUEST_SENSE                       (UX_TRACE_HOST_CLASS_EVENTS_BASE + 138)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_STORAGE_START_STOP                          (UX_TRACE_HOST_CLASS_EVENTS_BASE + 139)             /* I1 = class instance  , I2 = start stop signal                                                    */
#define UX_TRACE_HOST_CLASS_STORAGE_UNIT_READY_TEST                     (UX_TRACE_HOST_CLASS_EVENTS_BASE + 140)             /* I1 = class instance                                                                              */

#define UX_TRACE_HOST_CLASS_DPUMP_ACTIVATE                              (UX_TRACE_HOST_CLASS_EVENTS_BASE + 150)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_DPUMP_DEACTIVATE                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 151)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_DPUMP_READ                                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 152)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_DPUMP_WRITE                                 (UX_TRACE_HOST_CLASS_EVENTS_BASE + 153)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */

#define UX_TRACE_HOST_CLASS_SWAR_ACTIVATE                               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 160)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_SWAR_DEACTIVATE                             (UX_TRACE_HOST_CLASS_EVENTS_BASE + 161)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_SWAR_IOCTL_ABORT_IN_PIPE                    (UX_TRACE_HOST_CLASS_EVENTS_BASE + 162)              /* I1 = class instance  , I2 = endpoint                                                             */
#define UX_TRACE_HOST_CLASS_SWAR_IOCTL_ABORT_OUT_PIPE                   (UX_TRACE_HOST_CLASS_EVENTS_BASE + 163)              /* I1 = class instance  , I2 = endpointr                                                            */
#define UX_TRACE_HOST_CLASS_SWAR_READ                                   (UX_TRACE_HOST_CLASS_EVENTS_BASE + 164)              /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_SWAR_RECEPTION_START                        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 165)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_SWAR_RECEPTION_STOP                         (UX_TRACE_HOST_CLASS_EVENTS_BASE + 166)              /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_SWAR_WRITE                                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 167)              /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */

#define UX_TRACE_HOST_CLASS_GSER_ACTIVATE                               (UX_TRACE_HOST_CLASS_EVENTS_BASE + 170)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_GSER_DEACTIVATE                             (UX_TRACE_HOST_CLASS_EVENTS_BASE + 171)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_SET_LINE_CODING                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 172)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_GET_LINE_CODING                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 173)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_SET_LINE_STATE                   (UX_TRACE_HOST_CLASS_EVENTS_BASE + 174)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_PURGE                            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 175)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_SEND_BREAK                       (UX_TRACE_HOST_CLASS_EVENTS_BASE + 176)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_ABORT_IN_PIPE                    (UX_TRACE_HOST_CLASS_EVENTS_BASE + 177)             /* I1 = class instance  , I2 = endpoint                                                             */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_ABORT_OUT_PIPE                   (UX_TRACE_HOST_CLASS_EVENTS_BASE + 178)             /* I1 = class instance  , I2 = endpointr                                                            */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_REPORT_DEVICE_STATUS_CHANGE      (UX_TRACE_HOST_CLASS_EVENTS_BASE + 179)             /* I1 = class instance  , I2 = parameter                                                            */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_GET_DEVICE_STATUS                (UX_TRACE_HOST_CLASS_EVENTS_BASE + 180)             /* I1 = class instance  , I2 = device status                                                        */
#define UX_TRACE_HOST_CLASS_GSER_IOCTL_NOTIFICATION_CALLBACK            (UX_TRACE_HOST_CLASS_EVENTS_BASE + 181)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_GSER_READ                                   (UX_TRACE_HOST_CLASS_EVENTS_BASE + 182)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */
#define UX_TRACE_HOST_CLASS_GSER_RECEPTION_START                        (UX_TRACE_HOST_CLASS_EVENTS_BASE + 183)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_GSER_RECEPTION_STOP                         (UX_TRACE_HOST_CLASS_EVENTS_BASE + 184)             /* I1 = class instance                                                                              */
#define UX_TRACE_HOST_CLASS_GSER_WRITE                                  (UX_TRACE_HOST_CLASS_EVENTS_BASE + 185)             /* I1 = class instance  , I2 = data pointer    , I3 = requested length                              */

/* Define the USBX device stack events.  */

#define UX_TRACE_DEVICE_STACK_EVENTS_BASE                               850
#define UX_TRACE_DEVICE_STACK_ALTERNATE_SETTING_GET                     (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 1)             /* I1 = interface value                                                                             */
#define UX_TRACE_DEVICE_STACK_ALTERNATE_SETTING_SET                     (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 2)             /* I1 = interface value , I2 = alternate setting value                                              */
#define UX_TRACE_DEVICE_STACK_CLASS_REGISTER                            (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 3)             /* I1 = class name      , I2 = interface number, I3 = parameter                                     */
#define UX_TRACE_DEVICE_STACK_CLEAR_FEATURE                             (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 4)             /* I1 = request type    , I2 = request value   , I3 = request index                                 */
#define UX_TRACE_DEVICE_STACK_CONFIGURATION_GET                         (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 5)             /* I1 = configuration value                                                                         */
#define UX_TRACE_DEVICE_STACK_CONFIGURATION_SET                         (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 6)             /* I1 = configuration value                                                                         */
#define UX_TRACE_DEVICE_STACK_CONNECT                                   (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 7)             /*                                                                                                  */
#define UX_TRACE_DEVICE_STACK_DESCRIPTOR_SEND                           (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 8)             /* I1 = descriptor type , I2 = request index                                                        */
#define UX_TRACE_DEVICE_STACK_DISCONNECT                                (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 9)             /* I1 = device                                                                                      */
#define UX_TRACE_DEVICE_STACK_ENDPOINT_STALL                            (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 10)            /* I1 = endpoint                                                                                    */
#define UX_TRACE_DEVICE_STACK_GET_STATUS                                (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 11)            /* I1 = request type    , I2 = request value   , I3 = request index                                 */
#define UX_TRACE_DEVICE_STACK_HOST_WAKEUP                               (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 12)            /*                                                                                                  */
#define UX_TRACE_DEVICE_STACK_INITIALIZE                                (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 13)            /*                                                                                                  */
#define UX_TRACE_DEVICE_STACK_INTERFACE_DELETE                          (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 14)            /* I1 = interface                                                                                   */
#define UX_TRACE_DEVICE_STACK_INTERFACE_GET                             (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 15)            /* I1 = interface value                                                                             */
#define UX_TRACE_DEVICE_STACK_INTERFACE_SET                             (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 16)            /* I1 = alternate setting value                                                                     */
#define UX_TRACE_DEVICE_STACK_SET_FEATURE                               (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 17)            /* I1 = request value   , I2 = request index                                                        */
#define UX_TRACE_DEVICE_STACK_TRANSFER_ABORT                            (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 18)            /* I1 = transfer request, I2 = completion code                                                      */
#define UX_TRACE_DEVICE_STACK_TRANSFER_ALL_REQUEST_ABORT                (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 19)            /* I1 = endpoint        , I2 = completion code                                                      */
#define UX_TRACE_DEVICE_STACK_TRANSFER_REQUEST                          (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 20)            /* I1 = transfer request                                                                            */
#define UX_TRACE_DEVICE_STACK_MICROSOFT_EXTENSION_REGISTER              (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 21)            /* I1 = transfer request                                                                            */
#define UX_TRACE_DEVICE_STACK_CLASS_UNREGISTER                          (UX_TRACE_DEVICE_STACK_EVENTS_BASE + 22)            /* I1 = class name                                                                                  */

/* Define the USBX device stack events first.  */

#define UX_TRACE_DEVICE_CLASS_EVENTS_BASE                               900
#define UX_TRACE_DEVICE_CLASS_DPUMP_ACTIVATE                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 1)             /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_DPUMP_DEACTIVATE                          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 2)             /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_DPUMP_READ                                (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 3)             /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */
#define UX_TRACE_DEVICE_CLASS_DPUMP_WRITE                               (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 4)             /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */
#define UX_TRACE_DEVICE_CLASS_DPUMP_CHANGE                              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 5)             /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */

#define UX_TRACE_DEVICE_CLASS_CDC_ACM_ACTIVATE                          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 10)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ACM_DEACTIVATE                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 11)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ACM_READ                              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 12)            /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ACM_WRITE                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 13)            /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */

#define UX_TRACE_DEVICE_CLASS_HID_ACTIVATE                              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 20)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_HID_DEACTIVATE                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 21)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_HID_EVENT_GET                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 22)            /* I1 = class instance  , I2 = hid event                                                            */
#define UX_TRACE_DEVICE_CLASS_HID_EVENT_SET                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 23)            /* I1 = class instance  , I2 = hid event                                                            */
#define UX_TRACE_DEVICE_CLASS_HID_REPORT_GET                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 24)            /* I1 = class instance  , I2 = descriptor type , I3 = request index                                 */
#define UX_TRACE_DEVICE_CLASS_HID_REPORT_SET                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 25)            /* I1 = class instance  , I2 = descriptor type , I3 = request index                                 */
#define UX_TRACE_DEVICE_CLASS_HID_DESCRIPTOR_SEND                       (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 26)            /* I1 = class instance  , I2 = descriptor type , I3 = request index                                 */
#define UX_TRACE_DEVICE_CLASS_HID_READ                                  (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 27)            /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */
#define UX_TRACE_DEVICE_CLASS_HID_RECEIVER_EVENT_GET                    (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 28)            /* I1 = class instance  , I2 = receiver event  , I3 = wait_option                                   */
#define UX_TRACE_DEVICE_CLASS_HID_RECEIVER_EVENT_FREE                   (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 29)            /* I1 = class instance  , I2 = receiver event                                                       */

#define UX_TRACE_DEVICE_CLASS_PIMA_ACTIVATE                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 30)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_DEACTIVATE                           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 31)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_DEVICE_INFO_SEND                     (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 32)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_EVENT_GET                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 33)            /* I1 = class instance  , I2 = pima event                                                           */
#define UX_TRACE_DEVICE_CLASS_PIMA_EVENT_SET                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 34)            /* I1 = class instance  , I2 = pima event                                                           */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECT_ADD                           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 35)            /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECT_DATA_GET                      (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 36)            /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECT_DATA_SEND                     (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 37)            /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECT_DELETE                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 38)            /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECT_HANDLES_SEND                  (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 39)            /* I1 = class instance  , I2 = storage id      , I3 = object format code, I4 = object association   */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECT_INFO_GET                      (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 40)            /* I1 = class instance  , I2 = object handle                                                        */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECT_INFO_SEND                     (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 41)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECTS_NUMBER_SEND                  (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 42)            /* I1 = class instance  , I2 = storage id      , I3 = object format code, I4 = object association   */
#define UX_TRACE_DEVICE_CLASS_PIMA_PARTIAL_OBJECT_DATA_GET              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 43)            /* I1 = class instance  , I2 = object handle   , I3 = offset requested  , I4 = length requested     */
#define UX_TRACE_DEVICE_CLASS_PIMA_RESPONSE_SEND                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 44)            /* I1 = class instance  , I2 = response code   , I3 = number parameter  , I4 = pima parameter 1     */
#define UX_TRACE_DEVICE_CLASS_PIMA_STORAGE_ID_SEND                      (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 45)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_STORAGE_INFO_SEND                    (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 46)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_GET_DEVICE_PROP_DESC_GET             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 47)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_GET_DEVICE_PROP_VALUE                (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 48)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_GET_DEVICE_PROP_VALUE_SET            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 49)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_GET_OBJECT_PROP_DESC                 (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 50)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_GET_OBJECT_PROP_VALUE                (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 51)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECTS_PROPS_SUPPORTED_GET          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 52)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_GET_OBJECT_REFERENCES                (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 53)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_OBJECTS_PROPS_SUPPORTED_SET          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 54)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_SET_OBJECT_REFERENCES                (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 55)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_STORAGE_FORMAT                       (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 56)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_DEVICE_RESET                         (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 57)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PIMA_SET_OBJECT_PROP_VALUE                (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 58)            /* I1 = class instance                                                                              */

#define UX_TRACE_DEVICE_CLASS_RNDIS_ACTIVATE                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 60)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_RNDIS_DEACTIVATE                          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 61)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_RNDIS_PACKET_RECEIVE                      (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 62)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_RNDIS_PACKET_TRANSMIT                     (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 63)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_RNDIS_MSG_QUERY                           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 64)            /* I1 = class instance  , I2 = rndis OID                                                            */
#define UX_TRACE_DEVICE_CLASS_RNDIS_MSG_KEEP_ALIVE                      (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 65)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_RNDIS_MSG_RESET                           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 66)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_RNDIS_MSG_SET                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 67)            /* I1 = class instance  , I2 = rndis OID                                                            */

#define UX_TRACE_DEVICE_CLASS_STORAGE_ACTIVATE                          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 70)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_STORAGE_DEACTIVATE                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 71)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_STORAGE_FORMAT                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 72)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_INQUIRY                           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 73)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_MODE_SELECT                       (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 74)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_MODE_SENSE                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 75)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_PREVENT_ALLOW_MEDIA_REMOVAL       (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 76)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_READ                              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 77)            /* I1 = class instance  , I2 = lun             , I3 = sector              , I4 = number sectors     */
#define UX_TRACE_DEVICE_CLASS_STORAGE_READ_CAPACITY                     (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 78)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_READ_FORMAT_CAPACITY              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 79)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_READ_TOC                          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 80)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_REQUEST_SENSE                     (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 81)            /* I1 = class instance  , I2 = lun             , I3 = sense key           , I4 = code               */
#define UX_TRACE_DEVICE_CLASS_STORAGE_TEST_READY                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 82)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_START_STOP                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 83)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_VERIFY                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 84)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_WRITE                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 85)            /* I1 = class instance  , I2 = lun             , I3 = sector              , I4 = number sectors     */
#define UX_TRACE_DEVICE_CLASS_STORAGE_GET_CONFIGURATION                 (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 86)            /* I1 = class instance  , I2 = lun                                                                  */
#define UX_TRACE_DEVICE_CLASS_STORAGE_SYNCHRONIZE_CACHE                 (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 87)            /* I1 = class instance  , I2 = lun             , I3 = sector              , I4 = number sectors     */
#define UX_TRACE_DEVICE_CLASS_STORAGE_OTHER                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 88)            /* I1 = class instance  , I2 = lun                                                                  */

#define UX_TRACE_DEVICE_CLASS_CDC_ECM_ACTIVATE                          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 90)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ECM_DEACTIVATE                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 91)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ECM_CHANGE                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 92)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ECM_READ                              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 93)            /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ECM_WRITE                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 94)            /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ECM_PACKET_TRANSMIT                   (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 95)            /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */
#define UX_TRACE_DEVICE_CLASS_CDC_ECM_PACKET_RECEIVE                    (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 96)            /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */

#define UX_TRACE_DEVICE_CLASS_DFU_ACTIVATE                              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 97)            /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_DFU_DEACTIVATE                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 98)            /* I1 = class instance                                                                              */

#define UX_TRACE_DEVICE_CLASS_PRINTER_ACTIVATE                          (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 100)           /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PRINTER_DEACTIVATE                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 101)           /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_PRINTER_READ                              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 102)           /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */
#define UX_TRACE_DEVICE_CLASS_PRINTER_WRITE                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 103)           /* I1 = class instance  , I2 = buffer          , I3 = requested_length                              */

#define UX_TRACE_DEVICE_CLASS_CCID_ACTIVATE                             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 110)           /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_CCID_DEACTIVATE                           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 111)           /* I1 = class instance                                                                              */
#define UX_TRACE_DEVICE_CLASS_CCID_REQ_ABORT                            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 112)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_ICC_POWER_ON               (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 113)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_ICC_POWER_OFF              (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 114)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_GET_SLOT_STATUS            (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 115)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_XFR_BLOCK                  (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 116)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_GET_PARAMETERS             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 117)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_RESET_PARAMETERS           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 118)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_SET_PARAMETERS             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 119)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_ESCAPE                     (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 120)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_ICC_CLOCK                  (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 121)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_T0_APDU                    (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 122)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_SECURE                     (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 123)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_MECHANICAL                 (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 124)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_ABORT                      (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 125)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_PC_TO_RDR_SET_RATE_CLOCK             (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 126)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_RDR_TO_PC_RESPONSE                   (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 127)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_ICC_INSERT                           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 128)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_ICC_REMOVE                           (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 129)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_AUTO_SEQ_START                       (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 130)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_AUTO_SEQ_STOP                        (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 131)           /* I1 = class instance  , I2 = slot                                                                 */
#define UX_TRACE_DEVICE_CLASS_CCID_TIME_EXTENSION                       (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 132)           /* I1 = class instance  , I2 = slot            , I3 = time                                          */
#define UX_TRACE_DEVICE_CLASS_CCID_HARDWARE_ERROR                       (UX_TRACE_DEVICE_CLASS_EVENTS_BASE + 133)           /* I1 = class instance  , I2 = slot                                                                 */
/* Define the USBX Error Event.  */
#define UX_TRACE_ERROR                                                  999


#else
#define UX_TRACE_OBJECT_REGISTER(t,p,n,a,b)
#define UX_TRACE_OBJECT_UNREGISTER(o)
#define UX_TRACE_IN_LINE_INSERT(i,a,b,c,d,f,g,h)
#define UX_TRACE_EVENT_UPDATE(e,t,i,a,b,c,d)
#endif




/* Defines the number of ThreadX timer ticks per seconds. By default, the ThreadX timer tick is 10ms,
   so the default value for this constant is 100.  If TX_TIMER_TICKS_PER_SECOND is defined,
   this value is derived from TX_TIMER_TICKS_PER_SECOND.  */

#ifndef UX_PERIODIC_RATE
#ifdef TX_TIMER_TICKS_PER_SECOND
#define UX_PERIODIC_RATE                                                (TX_TIMER_TICKS_PER_SECOND)
#else
#define UX_PERIODIC_RATE                                                100
#endif
#endif


/* Define interrupts lockout constructs to protect the memory allocation/release which could happen
   under ISR in the device stack.  */

#ifndef UX_INTERRUPT_SAVE_AREA
#ifdef  TX_INTERRUPT_SAVE_AREA
#define UX_INTERRUPT_SAVE_AREA                                          TX_INTERRUPT_SAVE_AREA
#else
#define UX_INTERRUPT_SAVE_AREA                                          ALIGN_TYPE saved_flags;
#endif
#endif

#ifndef UX_DISABLE
#ifdef  TX_DISABLE
#define UX_DISABLE                                                      TX_DISABLE
#else
#define UX_DISABLE                                                      saved_flags = _ux_utility_interrupt_disable();
#endif
#endif

#ifndef UX_RESTORE
#ifdef  TX_RESTORE
#define UX_RESTORE                                                      TX_RESTORE
#else
#define UX_RESTORE                                                      _ux_utility_interrupt_restore(saved_flags);
#endif
#endif

#ifndef UX_INT_SAVE_AREA    /* Backward compatible.  */
#define UX_INT_SAVE_AREA                                                UX_INTERRUPT_SAVE_AREA
#endif
#ifndef UX_DISABLE_INTS     /* Backward compatible.  */
#define UX_DISABLE_INTS                                                 UX_DISABLE
#endif
#ifndef UX_RESTORE_INTS     /* Backward compatible.  */
#define UX_RESTORE_INTS                                                 UX_RESTORE
#endif


/* Define thread/timer related things.  */

#ifndef UX_EMPTY
#ifdef  TX_EMPTY
#define UX_EMPTY                                                        TX_EMPTY
#else
#define UX_EMPTY                                                        (0)
#endif
#endif

#ifndef UX_TIMER
#ifdef  TX_API_H
#define UX_TIMER                                                        TX_TIMER
#else
#define UX_TIMER                                                        VOID* /* Dummy type for standalone compile.  */
#endif
#endif

#ifndef UX_THREAD
#ifdef  TX_API_H
#define UX_THREAD                                                       TX_THREAD
#else
#define UX_THREAD                                                       VOID* /* Dummy type for standalone compile.  */
#endif
#endif

#ifndef UX_AUTO_START
#ifdef  TX_AUTO_START
#define UX_AUTO_START                                                   TX_AUTO_START
#else
#define UX_AUTO_START                                                   (1ul)
#endif
#endif

#ifndef UX_DONT_START
#ifdef  TX_DONT_START
#define UX_DONT_START                                                   TX_DONT_START
#else
#define UX_DONT_START                                                   (0ul)
#endif
#endif

#ifndef UX_AUTO_ACTIVATE
#ifdef  TX_AUTO_ACTIVATE
#define UX_AUTO_ACTIVATE                                                TX_AUTO_ACTIVATE
#else
#define UX_AUTO_ACTIVATE                                                (1ul)
#endif
#endif

#ifndef UX_NO_ACTIVATE
#ifdef  TX_NO_ACTIVATE
#define UX_NO_ACTIVATE                                                  TX_NO_ACTIVATE
#else
#define UX_NO_ACTIVATE                                                  (0ul)
#endif
#endif

#ifndef UX_THREAD_GET_SYSTEM_STATE
#define UX_THREAD_GET_SYSTEM_STATE                                      TX_THREAD_GET_SYSTEM_STATE
#endif

#ifndef UX_WAIT_FOREVER
#ifdef  TX_WAIT_FOREVER
#define UX_WAIT_FOREVER                                                 TX_WAIT_FOREVER
#else
#define UX_WAIT_FOREVER                                                 (0xFFFFFFFFul)
#endif
#endif

#ifndef UX_NO_WAIT
#ifdef  TX_NO_WAIT
#define UX_NO_WAIT                                                      TX_NO_WAIT
#else
#define UX_NO_WAIT                                                      (0ul)
#endif
#endif

#ifndef UX_NO_TIME_SLICE
#ifdef  TX_NO_TIME_SLICE
#define UX_NO_TIME_SLICE                                                TX_NO_TIME_SLICE
#else
#define UX_NO_TIME_SLICE                                                (0ul)
#endif
#endif


/* Define semaphore related things.  */

#ifndef UX_SEMAPHORE
#ifdef  TX_API_H
#define UX_SEMAPHORE                                                    TX_SEMAPHORE
#else
#define UX_SEMAPHORE                                                    VOID* /* Dummy type for standalone compile.  */
#endif
#endif


/* Define mutex related things.  */

#ifndef UX_MUTEX
#ifdef  TX_API_H
#define UX_MUTEX                                                        TX_MUTEX
#else
#define UX_MUTEX                                                        VOID* /* Dummy type for standalone compile.  */
#endif
#endif


/* Define event flags group related things.  */

#ifndef UX_EVENT_FLAGS_GROUP
#ifdef  TX_API_H
#define UX_EVENT_FLAGS_GROUP                                            TX_EVENT_FLAGS_GROUP
#else
#define UX_EVENT_FLAGS_GROUP                                            VOID* /* Dummy type for standalone compile.  */
#endif
#endif

#ifndef UX_OR_CLEAR
#ifdef  TX_OR_CLEAR
#define UX_OR_CLEAR                                                     TX_OR_CLEAR
#else
#define UX_OR_CLEAR                                                     (1u)
#endif
#endif

#ifndef UX_OR
#ifdef  TX_OR
#define UX_OR                                                           TX_OR
#else
#define UX_OR                                                           (0u)
#endif
#endif

#ifndef UX_AND_CLEAR
#ifdef  TX_AND_CLEAR
#define UX_AND_CLEAR                                                    TX_AND_CLEAR
#else
#define UX_AND_CLEAR                                                    (3u)
#endif
#endif

#ifndef UX_AND
#ifdef  TX_AND
#define UX_AND                                                          TX_AND
#else
#define UX_AND                                                          (2u)
#endif
#endif

#ifndef UX_NO_EVENTS
#ifdef  TX_NO_EVENTS
#define UX_NO_EVENTS                                                    TX_NO_EVENTS
#else
#define UX_NO_EVENTS                                                    (7u)
#endif
#endif

/* Define the version ID of USBX.  This may be utilized by the application.  */

#ifdef  UX_SYSTEM_INIT
CHAR                            _ux_version_id[] =
                                    "Copyright (c) Microsoft Corporation. All rights reserved. * USBX Generic Version 6.4.1 *";
#else
extern  CHAR                    _ux_version_id[];
#endif

#endif

