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


/**************************************************************************/
/*                                                                        */
/*  COMPONENT DEFINITION                                   RELEASE        */
/*                                                                        */
/*    ux_os_utility.h                                     PORTABLE C      */
/*                                                           6.4.1        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    STMicroelectronics                                                  */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file contains all the header and extern functions used by the  */
/*    USBX components that utilize utility functions.                     */
/*                                                                        */
/**************************************************************************/

#ifndef UX_OS_UTILITY_H
#define UX_OS_UTILITY_H


#define _ux_utility_semaphore_waiting(sem)                          ((sem)->tx_semaphore_count != UX_EMPTY)
#define _ux_utility_semaphore_created(sem)                          ((sem)->tx_semaphore_id != UX_EMPTY)
#define _ux_utility_thread_created(thr)                             ((thr)->tx_thread_id != UX_EMPTY)
#define _ux_utility_event_flags_created(e)                          ((e)->tx_event_flags_group_id != UX_EMPTY)
#define _ux_utility_thread_entry(thr)                               ((thr)->tx_thread_entry)
#define _ux_utility_thread_highest_priority()                       (0)

/* Define Utility component function prototypes.  */

UINT             _ux_os_utility_mutex_create(UX_MUTEX *mutex, CHAR *mutex_name);
UINT             _ux_os_utility_mutex_delete(UX_MUTEX *mutex);
VOID             _ux_os_utility_mutex_off(UX_MUTEX *mutex);
VOID             _ux_os_utility_mutex_on(UX_MUTEX *mutex);
UINT             _ux_os_utility_semaphore_create(UX_SEMAPHORE *semaphore, CHAR *semaphore_name, UINT initial_count);
UINT             _ux_os_utility_semaphore_delete(UX_SEMAPHORE *semaphore);
UINT             _ux_os_utility_semaphore_get(UX_SEMAPHORE *semaphore, ULONG semaphore_signal);
UINT             _ux_os_utility_semaphore_put(UX_SEMAPHORE *semaphore);
UINT             _ux_os_utility_thread_create(UX_THREAD *thread_ptr, CHAR *name,
                                              VOID (*entry_function)(ULONG), ULONG entry_input,
                                              VOID *stack_start, ULONG stack_size,
                                              UINT priority, UINT preempt_threshold,
                                              ULONG time_slice, UINT auto_start);
UINT             _ux_os_utility_thread_delete(UX_THREAD *thread_ptr);
VOID             _ux_os_utility_thread_relinquish(VOID);
UINT             _ux_os_utility_thread_schedule_other(UINT caller_priority);
UINT             _ux_os_utility_thread_resume(UX_THREAD *thread_ptr);
UINT             _ux_os_utility_thread_sleep(ULONG ticks);
UINT             _ux_os_utility_thread_suspend(UX_THREAD *thread_ptr);
UX_THREAD       *_ux_os_utility_thread_identify(VOID);
UINT             _ux_os_utility_timer_create(UX_TIMER *timer, CHAR *timer_name, VOID (*expiration_function) (ULONG),
                                             ULONG expiration_input, ULONG initial_ticks, ULONG reschedule_ticks,
                                             UINT activation_flag);
UINT             _ux_os_utility_timer_delete(UX_TIMER *timer);
UINT             _ux_os_utility_timer_start(UX_TIMER *timer, ULONG initial_ticks);
UINT             _ux_os_utility_event_flags_create(UX_EVENT_FLAGS_GROUP*group_ptr, CHAR *name);
UINT             _ux_os_utility_event_flags_delete(UX_EVENT_FLAGS_GROUP*group_ptr);
UINT             _ux_os_utility_event_flags_get(UX_EVENT_FLAGS_GROUP*group_ptr, ULONG requested_flags,
                                                UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option);
UINT             _ux_os_utility_event_flags_set(UX_EVENT_FLAGS_GROUP*group_ptr, ULONG flags_to_set,
                                                UINT set_option);
VOID             _ux_os_utility_sleep_ms(ULONG ms_wait);

#ifndef             _ux_utility_interrupt_disable
#define             _ux_utility_interrupt_disable()             _tx_thread_interrupt_disable()
#endif

#ifndef             _ux_utility_interrupt_restore
#define             _ux_utility_interrupt_restore(flags)        _tx_thread_interrupt_restore(flags)
#endif

#ifndef             _ux_utility_time_get
#define             _ux_utility_time_get()                      tx_time_get()
#endif

#ifndef             _ux_utility_time_elapsed
#define             _ux_utility_time_elapsed(a,b)          (((b)>=(a)) ? ((b)-(a)) : (0xFFFFFFFFul-(b)+(a)+1))
#else
extern  ALIGN_TYPE  _ux_utility_time_elapsed(ALIGN_TYPE, ALIGN_TYPE);
#endif

#endif /* UX_OS_UTILITY_H */