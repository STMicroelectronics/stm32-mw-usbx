  /**
  ******************************************************************************
  * @file    st_readme.txt
  * @brief   This file lists the main changes done by STMicroelectronics on
  *          USBX for STM32 devices.
  * Copyright (c) 2025 STMicroelectronics.
  *
  * This program and the accompanying materials are made available under the
  * terms of the MIT License which is available at
  * https://opensource.org/licenses/MIT.
  *
  * SPDX-License-Identifier: MIT
  *****************************************************************************/
  */

### V6.4.3 (03-04-2026) ###
============================
- Upgrade to Eclipse USBX v6.4.3.
- Add SW_Security_Level.md file.
- Add HAL sequence stop on host controllers in ux_host_stack_hcd_unregister.c.
- Update USBX file copyright headers.
- Remove duplicated code from ux_device_class_hid_deactivate.c.
- Add mutex creation macro in UX utility.
- Add USB host controller stop sequence before unregistering HCD in ux_host_stack_hcd_unregister.c.
- Add EHCI support.
- Remove duplicated code in ux_os_utility.h.
- Add a partition entry counter (ux_host_class_storage_mounted_partitions_count) and a configurable maximum (UX_HOST_CLASS_STORAGE_MAX_PARTITIONS_COUNT) to limit the number of partition entries processed during mounting.
- Fix code spelling issues.

### V6.4.1 (12-11-2025) ###
============================
- Fix compilation warning in ux_host_class_audio_activate.
- Fix handling of debug data in _ux_utility_debug_log.
- Fix device pointer issue and premature thread start.
- Fix thread start timing for USBX audio read thread.

### V6.4.1 (30-05-2025) ###
============================
- Implement RTOS agnostic capabilities to support FreeRTOS and standalone modes:
  - Add ux_port.h under ports/freertos/inc directory.
  - Remove all macros related to ThreadX from ux_port.h located under ports/generic/inc.

- Add USBX 6.4.1 from Eclipse ThreadX
