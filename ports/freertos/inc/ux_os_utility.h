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
/*    ux_utility.h                                        PORTABLE C      */
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


static inline UINT _ux_utility_semaphore_get_count(UX_SEMAPHORE *semaphore)
{
  UINT count;

  UX_ASSERT(semaphore != NULL);
  UX_ASSERT(semaphore->semaphore_handle != NULL);

#if defined (UX_UTILITY_ENABLE_PARAM_CHECKING)
  if ((semaphore == NULL) || (semaphore->semaphore_handle == NULL))
  {
    return 0U;
  }
#endif

  if (xPortIsInsideInterrupt())
  {
    count = (UINT)uxQueueMessagesWaitingFromISR(semaphore->semaphore_handle);
  }
  else
  {
    count = (UINT)uxSemaphoreGetCount(semaphore->semaphore_handle);
  }

  return (count);
}

#define _ux_utility_semaphore_waiting(sem)                          (_ux_utility_semaphore_get_count(sem) != 0)
#define _ux_utility_semaphore_created(sem)                          (sem.semaphore_handle != NULL)
#define _ux_utility_thread_created(thr)                             (thr.task_handle != NULL)
#define _ux_utility_event_flags_created(event)                      (event.event_group_handle != NULL)
#define _ux_utility_thread_entry(thr)                               (thr->pvTaskCode)
#define _ux_utility_thread_highest_priority()                       (configMAX_PRIORITIES - 1)

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
UINT             _ux_os_utility_event_flags_create(UX_EVENT_FLAGS_GROUP *group_ptr, CHAR *name);
UINT             _ux_os_utility_event_flags_delete(UX_EVENT_FLAGS_GROUP *group_ptr);
UINT             _ux_os_utility_event_flags_get(UX_EVENT_FLAGS_GROUP *group_ptr, ULONG requested_flags,
                                                UINT get_option, ULONG *actual_flags_ptr, ULONG wait_option);
UINT             _ux_os_utility_event_flags_set(UX_EVENT_FLAGS_GROUP *group_ptr, ULONG flags_to_set,
                                                UINT set_option);
VOID             _ux_os_utility_sleep_ms(ULONG ms_wait);

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

#endif /* UX_OS_UTILITY_H */