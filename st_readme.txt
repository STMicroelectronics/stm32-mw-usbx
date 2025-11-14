  /**
  ******************************************************************************
  * @file    st_readme.txt
  * @brief   This file lists the main changes done by STMicroelectronics on
  *          USBX for STM32 devices.
  /*****************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  *
  * This program and the accompanying materials are made available under the
  * terms of the MIT License which is available at
  * https://opensource.org/licenses/MIT.
  *
  * SPDX-License-Identifier: MIT
  *****************************************************************************/

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