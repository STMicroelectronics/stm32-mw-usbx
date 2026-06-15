

# USBX Software Pack

![tag](https://img.shields.io/badge/tag-2.1.0-brightgreen.svg)
[![release note](https://img.shields.io/badge/release_note-view_html-gold.svg)](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/stm32-mw-usbx/blob/hal2/ST_Release_Notes.html)

## Overview
The Middleware USBX component is a STM32 tailored fork of the [Eclipse ThreadX USBX](https://github.com/eclipse-threadx/usbx).

USBX is a complete USB Host/Device stack designed for embedded systems.

It provides a complete set of USB device and host classes like CDC-ACM, Mass storage, HID.

The goal of this component is to integrate STM32 USB Hardware IP through the STM32 HAL/USB drivers.

## RTOS support

The folder `ports/freertos` provides the porting APIs and macros to make USBX run on top of FreeRTOS.

## Low level drivers

The folder `interfaces` contains the following usbx controllers:

* **usbx_stm32_device_controllers**

    An STM32 reference implementation for USBX device based on **STM32 HAL2 PCD driver.**

* **usbx_stm32_device_decriptors**
    A configurable STM32 reference implementation of the device descriptors.

* **usbx_stm32_host_controllers**

    An STM32 reference implementation for USBX host based on **STM32 HAL2 HCD driver.**
	