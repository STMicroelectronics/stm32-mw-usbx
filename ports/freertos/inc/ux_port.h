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
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains data type definitions that make USBX function    */
/*    identically on a variety of different processor architectures.      */
/*                                                                        */
/**************************************************************************/

#ifndef UX_PORT_H
#define UX_PORT_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"

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

#if (configSUPPORT_DYNAMIC_ALLOCATION == 0)
  #error "configSUPPORT_DYNAMIC_ALLOCATION must be set to 1"
#endif


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

/* Generic USBX Project constants follow. */
#ifndef UX_PERIODIC_RATE
#define UX_PERIODIC_RATE                                    100
#endif

#ifndef UX_MAX_SLAVE_CLASS_DRIVER
#define UX_MAX_SLAVE_CLASS_DRIVER                           2
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

#ifndef UX_THREAD_STACK_SIZE
#define UX_THREAD_STACK_SIZE                                (1*1024)
#endif

#ifndef UX_THREAD_PRIORITY_ENUM
#define UX_THREAD_PRIORITY_ENUM                             5
#endif

#ifndef UX_THREAD_PRIORITY_CLASS
#define UX_THREAD_PRIORITY_CLASS                            5
#endif

#ifndef UX_THREAD_PRIORITY_KEYBOARD
#define UX_THREAD_PRIORITY_KEYBOARD                         5
#endif

#ifndef UX_HOST_OS_CLASS_STORAGE_THREAD_PRIORITY_CLASS
#define UX_HOST_OS_CLASS_STORAGE_THREAD_PRIORITY_CLASS      5
#endif

#ifndef UX_PICTBRIDGE_OS_THREAD_PRIORITY_CLASS
#define UX_PICTBRIDGE_OS_THREAD_PRIORITY_CLASS              5
#endif

#ifndef UX_THREAD_PRIORITY_HCD
#define UX_THREAD_PRIORITY_HCD                              configMAX_PRIORITIES - 2
#endif

#ifndef UX_THREAD_PRIORITY_DCD
#define UX_THREAD_PRIORITY_DCD                              configMAX_PRIORITIES - 2
#endif

#if UX_THREAD_PRIORITY_DCD < UX_THREAD_PRIORITY_ENUM
#warning "UX_THREAD_PRIORITY_DCD should be greater than UX_THREAD_PRIORITY_ENUM"
#endif

#if UX_THREAD_PRIORITY_DCD < UX_THREAD_PRIORITY_CLASS
#warning "UX_THREAD_PRIORITY_DCD should be greater than UX_THREAD_PRIORITY_CLASS"
#endif

#if UX_THREAD_PRIORITY_HCD < UX_THREAD_PRIORITY_ENUM
#warning "UX_THREAD_PRIORITY_HCD should be greater than UX_THREAD_PRIORITY_ENUM"
#endif

#if UX_THREAD_PRIORITY_HCD < UX_THREAD_PRIORITY_CLASS
#warning "UX_THREAD_PRIORITY_HCD should be greater than UX_THREAD_PRIORITY_CLASS"
#endif

#if UX_THREAD_PRIORITY_HCD < UX_THREAD_PRIORITY_KEYBOARD
#warning "UX_THREAD_PRIORITY_HCD should be greater than UX_THREAD_PRIORITY_KEYBOARD"
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

#define UX_THREAD_POINTER_INDEX                             0U

/* Define USBX RTOS structures */
typedef struct
{
  SemaphoreHandle_t semaphore_handle;
  UINT              ux_semaphore_suspended_count;
} ux_semaphore_t;

typedef struct
{
  SemaphoreHandle_t mutex_handle;
} ux_mutex_t;

typedef struct
{
  TaskHandle_t task_handle;
} ux_thread_t;

typedef struct
{
  TimerHandle_t timer_handle;
  ULONG timer_callback_input;
  VOID (*timer_callback_function) (ULONG);
} ux_timer_t;

typedef struct
{
  EventGroupHandle_t event_group_handle;
} ux_event_group_t;


#define UX_MUTEX                                           ux_mutex_t
#define UX_SEMAPHORE                                       ux_semaphore_t
#define UX_THREAD                                          ux_thread_t
#define UX_TIMER                                           ux_timer_t
#define UX_EVENT_FLAGS_GROUP                               ux_event_group_t


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


/* Define interrupt lockout constructs to protect the memory allocation/release which could happen
   under ISR in the device stack.  */
#define UX_INT_SAVE_AREA        unsigned int  old_interrupt_posture;

/* Define trace. */
#define UX_TRACE_OBJECT_REGISTER(t,p,n,a,b)
#define UX_TRACE_OBJECT_UNREGISTER(o)
#define UX_TRACE_IN_LINE_INSERT(i,a,b,c,d,f,g,h)
#define UX_TRACE_EVENT_UPDATE(e,t,i,a,b,c,d)

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


/* Define thread/timer related things.  */
#ifndef UX_EMPTY
#define UX_EMPTY                                                        (0)
#endif

#ifndef UX_TIMER
#define UX_TIMER                                                        VOID* /* Dummy type for standalone compile. */
#endif

#ifndef UX_THREAD
#define UX_THREAD                                                       VOID* /* Dummy type for standalone compile. */
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

#ifndef UX_THREAD_GET_SYSTEM_STATE
#define UX_THREAD_GET_SYSTEM_STATE                                      (((ULONG) 0xF0F0F0F0UL) | __get_IPSR)
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

/* Define semaphore related things. */
#ifndef UX_SEMAPHORE
#define UX_SEMAPHORE                                                    VOID* /* Dummy type for standalone compile. */
#endif

/* Define mutex related things. */
#ifndef UX_MUTEX
#define UX_MUTEX                                                        VOID* /* Dummy type for standalone compile. */
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

/* Define the version ID of USBX.  This may be utilized by the application. */
#ifdef  UX_SYSTEM_INIT
CHAR                            _ux_version_id[] =
                                    "Copyright (c) Microsoft Corporation. All rights reserved. * USBX Generic Version 6.4.1 *";
#else
extern  CHAR                    _ux_version_id[];
#endif

#endif

