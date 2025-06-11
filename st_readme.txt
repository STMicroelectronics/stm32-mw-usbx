  /**
  ******************************************************************************
  * @file    st_readme.txt
  * @author  GPM Application Team
  * @brief   This file lists the main changes done by STMicroelectronics on
  *          USBX for STM32 devices.
  ******************************************************************************
  */

### V6.4.1 (30-05-2025) ###
============================
- Implement RTOS agnostic capabilities to support FreeRTOS and standalone modes:
  - Add ux_port.h under ports/freertos/inc directory.
  - Remove all macros related to ThreadX from ux_port.h located under ports/generic/inc.

- Add USBX 6.4.1 from Eclipse ThreadX