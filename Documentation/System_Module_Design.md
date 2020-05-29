# Plat4m System Module Design

## Background

The System module is intended to provide a minimal subset of functionality that is common to all types of synchronous systems: bare-metal with a tick timer, cooperative scheduler, preemptive RTOS, or full-blown OS (like Linux or Windows). It provides an abstracted interface for the following features:

- Getting system run time
- Timed delay on the calling task/thread
- Creation of OS modules
- Running the the scheduler and/or background loop/task/thread

## Driver Design

A System driver should include and instantiate all files and modules necessary to run the underlying system or operating system that the driver is written for. For example, when `System::run()` is called on a `SystemFreeRtos` object, it should make the necessary FreeRTOS calls to start the scheduler and run the idle task.

### Note

The System module uses the singleton design pattern combined with static methods to allow for System calls anywhere inside of the application at any layer without needing to have a reference to the created System object. For example, if a `SystemLinux` object was instantiated by user code at the application level, any code at any layer within the application could call `System::getTimeMs()` and the underlying call to Linux would be made.

Because of this, the System module should be the first one to be instantiated in an application so that it is initialized and ready to be used by other modules.

![""](Images/System_Module.svg)
