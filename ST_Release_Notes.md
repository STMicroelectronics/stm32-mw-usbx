

# Release Notes for
# <mark>USBX SW Pack</mark>

# Purpose

STM32Cube enables developers to achieve design success. With a comprehensive suite of professional development tools and embedded software components, STM32Cube allows developers to differentiate products, streamline design cycles, and reduce costs. STM32Cube ecosystem supports all design steps, including selection, configuration, development, debugging, programming, and monitoring.
The STM32Cube embedded software offer provides ready-to-use software components that can be added to a project. It includes STM32 peripheral driver APIs with two levels of abstraction, middleware, board drivers, and examples. There are several distribution channels, including the STM32CubeMX2 tool, the ST website, and GitHub. All embedded software comes with enhanced online documentation, with flowcharts and user sequences.

**USBX** is an USB stack middleware for STM32 microcontrollers, supporting both device and host roles. It provides a configurable set of USB classes and basic USB protocol functionality. USBX is RTOS‑agnostic and can be integrated in bare‑metal or RTOS‑based applications.
software comes with enhanced online documentation, with flowcharts and user sequences.

More documentation is available at [**USBX SW Pack online documentation**](https://dev.st.com/stm32cube-docs/mw-usbx/latest/en/index.html)

# Update history

<label for="collapse-section-2.0.0" aria-hidden="true">__2.0.0 / 13-March-2026__</label>
<div>

## Main changes
First Official release of **USBX SW Pack**.\
This pack is based on [**USBX V6.4.3 version**](https://github.com/eclipse-threadx/usbx/releases/tag/v6.4.3.202503_rel)

## Contents
- USBX full stack support (device and host classes), for both bare‑metal and RTOS, with STM32 PCD/HCD integration and USB device descriptor management.

## Known limitations

- None

## Development toolchains and compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V9.60.3 + ST-LINK
- MDK-ARM Keil uVision V5.42
- STM32CubeIDE for Visual Studio Code (GCC13 compiler)

## Supported devices and boards

- STM32C5 series.

## Backward compatibility

- None

## Dependencies

- STM32C5xx HAL Drivers V2.0.0
- FreeRTOS SW Pack V2.0.0 (required when selecting RTOS mode)
- FileX SW Pack V2.0.0 (required when selecting MSC Host class)

</div>


For complete documentation on STM32 microcontrollers,
visit: [www.st.com/stm32](http://www.st.com/stm32)

This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
