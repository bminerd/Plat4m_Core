# Plat4m

## Project Overview

Plat4m ("platform") is a cross-platform C++ application framework designed for real-time embedded systems that's portable to anything from an 8-bit microcontroller to an x86 PC running Linux or Windows. This project maximizes code portability and reuse across systems comprised of varying different hardware and underlying operating systems, enabling fast prototyping and advanced software-in-the-loop testing.

## Features

- Clean, lightweight API that uses polymorphism to abstract hardware and software
- Portable to any system with a C++ compiler (C++03)
- Flexible OS abstraction that supports many different scheduler paradigms including round-robin, run-to-completion, preemptive, etc
- Each module has its own independent abstraction with minimal cross-project interdependency (i.e not a monolithic library)

## Supported hardware

- STM32 F1/F3/F4 families
  - GPIO
  - ADC
  - Timers (basic and advanced)
  - UART
  - SPI
  - I2c
  - CAN
  - DMA (some peripherals)
  - Interrupts
- Coming soon
  - STM32 F7
  - Atmel AVR
  - SiFive FE310

## Supported operating systems

- Bare metal
- Lite (simple cooperative scheduler)
- FreeRTOS (Cortex-M3 and M4)
- Linux
- Windows

## Design Documents

- [High-Level Design](Documentation/High_Level_Design.md)
- [System Module Design](Documentation/System_Module_Design.md)
- [Thread Module Design](Documentation/Thread_Module_Design.md)
- [ComInterface Module Design](Documentation/ComInterface_Module_Design.md)