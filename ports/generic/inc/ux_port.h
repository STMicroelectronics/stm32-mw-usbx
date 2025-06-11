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
/*  10-31-2023     Chaoqiong Xiao           Modified comment(s),          */
/*                                            added basic types guards,   */
/*                                            improved SLONG typedef,     */
/*                                            resulting in version 6.3.0  */
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


#include <stdint.h>

#ifndef VOID_DEFINED
#define VOID_DEFINED
typedef void                                    VOID;
#endif

#ifndef CHAR_DEFINED
#define CHAR_DEFINED
typedef char                                    CHAR;
#endif

#ifndef BOOL_DEFINED
#define BOOL_DEFINED
typedef char                                    BOOL;
#endif

#ifndef UCHAR_DEFINED
#define UCHAR_DEFINED
typedef unsigned char                           UCHAR;
#endif

#ifndef INT_DEFINED
#define INT_DEFINED
typedef int                                     INT;
#endif

#ifndef UINT_DEFINED
#define UINT_DEFINED
typedef unsigned int                            UINT;
#endif

#ifndef LONG_DEFINED
#define LONG_DEFINED
typedef long                                    LONG;
#endif

#ifndef ULONG_DEFINED
#define ULONG_DEFINED
typedef unsigned long                           ULONG;
#endif

#ifndef SHORT_DEFINED
#define SHORT_DEFINED
typedef short                                   SHORT;
#endif

#ifndef USHORT_DEFINED
#define USHORT_DEFINED
typedef unsigned short                          USHORT;
#endif

#ifndef ULONG64_DEFINED
#define ULONG64_DEFINED
typedef unsigned long long                      ULONG64;
#endif

#ifndef ALIGN_TYPE_DEFINED
#define ALIGN_TYPE                              ULONG
#endif

/* Define additional generic USBX types.  */
#ifndef SLONG_DEFINED
typedef LONG                                    SLONG;
#define SLONG_DEFINED
#endif

#define UX_TRACE_OBJECT_REGISTER(t,p,n,a,b)
#define UX_TRACE_OBJECT_UNREGISTER(o)
#define UX_TRACE_IN_LINE_INSERT(i,a,b,c,d,f,g,h)
#define UX_TRACE_EVENT_UPDATE(e,t,i,a,b,c,d)

#ifndef             _ux_utility_interrupt_disable
extern ALIGN_TYPE   _ux_utility_interrupt_disable(VOID);
#endif

#ifndef             _ux_utility_interrupt_restore
extern VOID         _ux_utility_interrupt_restore(ALIGN_TYPE);
#endif

#ifndef             _ux_utility_time_get
extern  ULONG       _ux_utility_time_get(VOID);
#endif

#ifndef             _ux_utility_time_elapsed
#define             _ux_utility_time_elapsed(a,b)          (((b)>=(a)) ? ((b)-(a)) : (0xFFFFFFFFul-(b)+(a)+1))
#else
extern  ALIGN_TYPE  _ux_utility_time_elapsed(ALIGN_TYPE, ALIGN_TYPE);
#endif

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


/* Defines the number of timer ticks per seconds. */
#ifndef UX_PERIODIC_RATE
#define UX_PERIODIC_RATE                                                100
#endif

/* Define interrupts lockout constructs to protect the memory allocation/release which could happen
   under ISR in the device stack. */
#ifndef UX_INTERRUPT_SAVE_AREA
#define UX_INTERRUPT_SAVE_AREA                                          ALIGN_TYPE saved_flags;
#endif

#ifndef UX_DISABLE
#define UX_DISABLE                                                      saved_flags = _ux_utility_interrupt_disable();
#endif

#ifndef UX_RESTORE
#define UX_RESTORE                                                      _ux_utility_interrupt_restore(saved_flags);
#endif

#ifndef UX_INT_SAVE_AREA    /* Backward compatible. */
#define UX_INT_SAVE_AREA                                                UX_INTERRUPT_SAVE_AREA
#endif
#ifndef UX_DISABLE_INTS     /* Backward compatible. */
#define UX_DISABLE_INTS                                                 UX_DISABLE
#endif
#ifndef UX_RESTORE_INTS     /* Backward compatible. */
#define UX_RESTORE_INTS                                                 UX_RESTORE
#endif

#ifndef UX_EMPTY
#define UX_EMPTY                                                        (0)
#endif

#ifndef UX_TIMER
#define UX_TIMER                                                        VOID* /* Dummy type for standalone compile. */
#endif


#ifndef UX_AUTO_START
#define UX_AUTO_START                                                   (1ul)
#endif

#ifndef UX_DONT_START
#define UX_DONT_START                                                   (0ul)
#endif

#ifndef UX_AUTO_ACTIVATE
#define UX_AUTO_ACTIVATE                                                (1ul)
#endif

#ifndef UX_NO_ACTIVATE
#define UX_NO_ACTIVATE                                                  (0ul)
#endif

#ifndef UX_WAIT_FOREVER
#define UX_WAIT_FOREVER                                                 (0xFFFFFFFFul)
#endif

#ifndef UX_NO_WAIT
#define UX_NO_WAIT                                                      (0ul)
#endif

#ifndef UX_NO_TIME_SLICE
#define UX_NO_TIME_SLICE                                                (0ul)
#endif

/* Define event flags group related things. */
#ifndef UX_EVENT_FLAGS_GROUP
#define UX_EVENT_FLAGS_GROUP                                            VOID* /* Dummy type for standalone compile. */
#endif

#ifndef UX_OR_CLEAR
#define UX_OR_CLEAR                                                     (1u)
#endif

#ifndef UX_OR
#define UX_OR                                                           (0u)
#endif

#ifndef UX_AND_CLEAR
#define UX_AND_CLEAR                                                    (3u)
#endif

#ifndef UX_AND
#define UX_AND                                                          (2u)
#endif

#ifndef UX_NO_EVENTS
#define UX_NO_EVENTS                                                    (7u)
#endif

/* Define the version ID of USBX.  This may be utilized by the application.  */
#ifdef  UX_SYSTEM_INIT
CHAR                            _ux_version_id[] =
                                    "Copyright (c) Microsoft Corporation. All rights reserved. * USBX Generic Version 6.4.1 *";
#else
extern  CHAR                    _ux_version_id[];
#endif

#endif

