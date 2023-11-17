# Plat4m_Core Change Log

All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## Change Log Categories

- [BUG FIX] - Bugs are classified as software problems that cause system degradation or performance issues.
- [CRITICAL BUG FIX] - Critical bugs are classified as software problems that cause system failures.
- [NEW FEATURE] - New functionality has been added.
- [IMPROVEMENT] - Existing functionality has been improved.
- [DEPRECATED] - Existing functionality has been marked for removal in a future release.
- [REMOVED] - Existing functionality has been removed.
- [NONFUNCTIONAL] - No functionality has been added.

### Unreleased Changes

Released: XXXX-XX-XX

- `[TEMPLATE]` Description of changes. [Resolves #issue]. [Merge !mr].

- `[IMPROVEMENT]` Updated plat4m_linux_dev container to v2.0.0.
- `[IMPROVEMENT]` Added remapBit() helper function. Added second template parameter to limitValue() for usage clarity.
- `[BUG FIX]` Fixed 480 MHz core clock configuration for STM32H7xx. Also enabled I and D cache now that clock configuration is correct.
- `[NEW FEATURE]` Added STM32H7xx drivers for Processor, Interrupt, GpioPort, GpioPin, and Uart. Dma drivers added, but not fully functional.
- `[IMPROVEMENT]` Added bitMask() and power() constant expression helper functions.
- `[IMPROVEMENT]` Added int return type to Application::run().
- `[IMPROVEMENT]` Added ability to internally update DataObjects.
- `[IMPROVEMENT]` Added static casts to resolve MSVS compiler warnings for implicit type conversion.
- `[IMPROVEMENT]` Added ability for List to store reference types.
- `[NEW FEATURE]` Added DataObject class as a data-centric interface for accessing application data. Added DataObjectTopicService as a DataObject implementation that uses Topics and Services to access the data.
- `[BUG FIX]` Fixed syntax in callback method version of createSerice() function inside Service.h.
- `[IMPROVEMENT]` Updated Topic and Service APIs to receive user data types directly in publish() and request(), respectively. Handling callbacks receive user data types wrapped in TopicSample<> and ServiceRequest<> / ServiceResponse<> templated types. Added Service and ServiceClient tests.
- `[BUG FIX]` Fixed Service finding bug that could return a service of an incompatible type if the wrong ID was provided. Fix uses dynamic_cast which requires RTTI.
- `[IMPROVEMENT]` Added modulus operator to TimeStamp class.
- `[BUG FIX]` Fixed Topic subscription bug that could subscribe to a topic of an incompatible type if the wrong ID was provided. Fix uses dynamic_cast which requires RTTI.
- `[IMPROVEMENT]` Added reset of singleton drivers in Manager classes.
- `[IMPROVEMENT]` Updated default FreeRTOSConfig.h to record stack high address. Also increased length of string task name and set heap size to zero. Moved into subfolder ./Config.
- `[IMPROVEMENT]` Updated GitLab CI yaml file to use bminerd/plat4m_linux_dev container directly to improve job speed.
- `[IMPROVEMENT]` Added string name parameter to Thread and Stopwatch classes. Updated all references to pass string name to Thread and Stopwatch. Updated StopwatchStatisticsPrinter to use internal Stopwatch name and added heartbeat output configuration option.
- `[IMPROVEMENT]` Added isInterruptActive() methods to Processor class. Added implementations to STM32F4xx, STM32F30x, and NRF5340 subclasses.
- `[IMPROVEMENT]` Added virtual and override keywords to overridden methods in subclasses. This improves compatiblity with some compilers and is a defensive programming technique.
- `[BUG FIX]` Fixed syntax issues in Service class.
- `[BUG FIX]` Fixed implicit conversion error in TimeStamp for ARM v5 compiler.
- `[IMPROVEMENT]` Added fromTimeSFloat(), fromTimeSDouble(), and fromTimeSValueType() templated method. Also added variants that accept a rounding precision parameter.
- `[BUG FIX]` Fixed backward link and null pointer bugs in List::remove(). Updated first() and last() to return pointers. Added unit tests for List class.
- `[BUG FIX]` Fixed null pointers in List::first() and List::last().
- `[IMPROVEMENT]` Added private copy constructor to Service class. This prevents accidental copying of a Service, which must be avoided.
- `[IMPROVEMENT]` Added method toTimeSDouble() and template method toTimeSValueType() to TimeStamp.
- `[BUG FIX]` Fixed call time increment in ThreadSimulationTick.
- `[IMPROVEMENT]` Added TopicManager and ServiceManager classes to allow subscribing to a topic/service before it has been instantiated. Note: Topics and Services can no longer be instantiated directly, Topic::create() and Service::create() must be used instead.
- `[NEW FEATURE]` Created ChildApplication class for applications that exist beneath a main Application.
- `[IMPROVEMENT]` Added getDataAs() template method to Array that allows accessing the item pointer and casting it to a new type in a single step.
- `[IMPROVEMENT]` Replaced manual time stamp calculation in System::driverGetTimeStamp() with TimeStamp::fromTimeUs().
- `[BUG FIX]` Fixed stack size calculation in ThreadFreeRtos constructor.
- `[BUG FIX]` Fixed syntax in Service::request().
- `[IMPROVEMENT]` Removed zero array creations to improve compiler compatibility.
- `[IMPROVEMENT]` Changed variadic template argument to use a forwarding reference in MemoryAllocator::allocate().
- `[IMPROVEMENT]` Updated all STM32F4xx related files to use updated file syntax and formatting.
- `[IMPROVEMENT]` Improved clarity of signed integer time values in TimeStamp to/from time methods. Deprecated old methods that didn't indicate sign.
- `[IMPROVEMENT]` Added copy of Standard Peripheral Library v1.8.0 for STM32F4xx.
- `[BUG FIX]` Fixed TimeStamp data member initialization in SystemSimulation.
- `[IMPROVEMENT]` Modified TimeStamp class so that it can represent negative values. Added TimeStamp unit tests.
- `[IMPROVEMENT]` Addressed additional compiler warnings with -Wall enabled.
- `[IMPROVEMENT]` Addressed compiler warnings with -Wall enabled.
- `[IMPROVEMENT]` Renamed BoardNRF5340PDK to BoardNRF5340DK and updated virtual COM port uart ID.
- `[IMPROVEMENT]` Moved time stamp methods from SystemFreeRtos to SystemFreeRtosCortexM so that the nanosecond portion could be calculated using the SysTick timer counter.
- `[IMPROVEMENT]` Updated NRF5340 drivers. Uart and interrupt classes are now fully functional.
- `[IMPROVEMENT]` Added hardware flow control enabled option to Uart.
- `[NEW FEATURE]` Added Stopwatch interface class for accurate timing measurement. Added InterruptPolicy and ThreadPolicy implementations that use Stopwatch objects. Added StopwatchStatisticsPrinter for outputting timining statistics.
- `[IMPROVEMENT]` Added enter and exit critical section methods to System.
- `[NEW FEATURE]` Created ThreadPolicy class for wrapping a thread run function with definable policy behavior.
- `[IMPROVEMENT]` Implemented InterruptPolicy class for wrapping an interrupt handler with policy behavior.
- `[NEW FEATURE]` Added Printer interface for printing output. Added console and ComInterface implementations.
- `[IMPROVEMENT]` Added implementations for driverGetTimeStamp() and driverGetWallTimeStamp() to SystemLinux and SystemFreeRtos.
- `[IMPROVEMENT]` Added override keyword to CallbackFunction::call() and CallbackMethod::call().
- `[IMPROVEMENT]` Added toTimeSFloat() to TimeStamp class.
- `[IMPROVEMENT]` Added integerDivideRound() function to Plat4m.h for fast integer divides where the result should be rounded to the next closest multiple of the divisor.
- `[IMPROVEMENT]` Added appendCast() and prependCast() methods to Array class.
- `[BUG FIX]` Fixed TimeStamp::toTime() calculations and added const qualifiers.
- `[IMPROVEMENT]` Changed SystemSimulation time tracking so that it doesn't roll over often due to microsecond counter. Now won't roll over for 139 years (2^32 seconds). Added additional convenience methods to TimeStamp.
- `[IMPROVEMENT]` Added not-equals operator and fromTime() methods to TimeStamp class.
- `[IMPROVEMENT]` Changed default value of insert timestamp parameter in Topic::publish().
- `[BUG FIX]` Fixed undedefined function warning in Callback class.
- `[IMPROVEMENT]` Added addition operators to TimeStamp class.
- `[IMPROVEMENT]` Added setTime() method to System and added an implmentation in SystemSimulation.
- `[BUG FIX]` Fixed bug in Topic::publish() to use the sample copy.
- `[NEW FEATURE]` Added Service and supporting classes.
- `[IMPROVEMENT]` Changed Callback to be a variadic template. Updated CallbackMethod and CallbackFunction. Deprecated Callback parameter variants.
- `[IMPROVEMENT]` Added flag to Topic::publish() to give option for prepopulated time stamps.
- `[IMPROVEMENT]` Made TimeStamp struct a separate class with comparison operators.
- `[IMPROVEMENT]` Added unsubscribe() method to Topic class. TopicSubscriber destructor now calls Topic::unsubscribe().
- `[BUG FIX]` Fixed enable/disable race condition in ThreadLinux. Improved WaitConditionLinux notify logic.
- `[NEW FEATURE]` Added SystemSimulation wrapper class that adds simulation capabilities to any System implementation. Removed old Linux simulation classes.
- `[NEW FEATURE]` Added Semaphore interface class. Updated System interface and all implementations.
- `[IMPROVEMENT]` Added simulated flag and stack bytes count parameters to TopicSubscriberThread constructors.
- `[BUG FIX]` Fixed missing driverExit() methods in SystemLite, SystemWindows, and SystemFreeRtos.
- `[IMPROVEMENT]` Changed TopicSubscriber to be a subclass of Module to allow enabling/disabling.
- `[IMPROVEMENT]` Added VS Code build and debug support.
- `[NEW FEATURE]` Added Linux simulation System implementation. Added time stamp and wall time stamp retrieval to System interface. Added Topic header with sequence ID and time stamp.
- `[IMPROVEMENT]` Fixed missing FreeRTOS-Kernel dependency and MemoryAllocation reference syntax for failing CI builds.
- `[IMPROVEMENT]` Changed build scripts to report errors.
- `[BUG FIX]` Fixed allocate() syntax in BoardNRF5340PDK.
- `[BUG FIX]` Fixed bug in List::remove() for 2-element lists.
- `[IMPROVEMENT]` Added clearing of driver pointer in Processor destructor.
- `[IMPROVEMENT]` Added call in Topic destructor to remove it from static List in Topic class.
- `[IMPROVEMENT]` Added remove() method to List.
- `[NEW FEATURE]` Added MemoryAllocator class that uses placement new operator. Removed previous new/delete references and global new/delete operator overloading.
- `[IMPROVEMENT]` Added call to Thread destructor in HardwareTimerSimulated destructor.
- `[IMPROVEMENT]` Made Thread destructor public.
- `[IMPROVEMENT]` Added clearing of driver pointer in System destructor.
- `[IMPROVEMENT]` Added getThread() accessor to HardwareTimerSimulated.
- `[IMPROVEMENT]` Added exit logic to ThreadLinux.
- `[NEW FEATURE]` Added enable()/disable() methods to AllocationMemory by making it subclass of Module. Cleaned up nested driver logic.
- `[NEW FEATURE]` Added resubscribe() method to TopicSubscriber.
- `[IMPROVEMENT]` Updated QueueDriverLinux implementation to handle any data type. [Resolves 54]. [Merge !59].
- `[NEW FEATURE]` Added Topic, TopicSubscriber, and TopicSubscriberThread modules for lightweight internal publish/subscribe messaging. [Resolves 51]. [Merge !60].
- `[IMPROVEMENT]` Added docker tag to GitLab CI jobs. [Resolves 63]. [Merge !64].
- `[IMPROVEMENT]` Changed SampleType parameter in Topic::SampleCallback to be const reference. [Resolves 67]. [Merge !69].
- `[IMPROVEMENT]` Added getThread() method to TopicSubscriberThread class. [Resolves 68]. [Merge !70].
- `[BUG FIX]` Fixed syntax issue in Plat4m::isBitSet(). [Resolves 71]. [Merge !72].
- `[IMPROVEMENT]` Removed unneeded files from ST Standard Peripheral Library for F3 and F4.
- `[IMPROVEMENT]` Removed unused CMSIS math libraries.
- `[IMPROVEMENT]` Added single nested driver in AllocationMemory.
- `[IMPROVEMENT]` Added copy constructor and assignment operator to TopicSubscriber.
- `[NEW FEATURE]` Added HardwareTimerSimulated as a HardwareTimer driver for a simulated timer.
- `[IMPROVEMENT]` Updated FreeRTOS relative path references to allow using GitHub submodule.
- `[NEW FEATURE]` Added minimal drivers for NRF5340 microcontroller.
- `[NONFUNCTIONAL]` Added design documentation for System, Thread, and ComInterface modules. [Resolves 8]. [Merge !77].

### 2.0.0

- `[CRITICAL BUG FIX]` Fixed ComLink initialization order to prevent segfault on Linux. [Resolves 13]. [Merge !15].
- `[NEW FEATURE]` Added maskBits() inline function to Plat4m.h. [Resolves 17]. [Merge !18].
- `[BUG FIX]` Made all FreeRTOS paths relative. [Resolves 19]. [Merge !20].
- `[BUG FIX]` Fixed FreeRTOS include path case. [Resolves 21]. [Merge !22].
- `[BUG FIX]` Fixed FreeRTOS include path case in ThreadFreeRtos.cpp. [Resolves 23]. [Merge !24].
- `[BUG FIX]` Fixed enabling and disabling of Linux pthreads. [Resolves 14]. [Merge !25].
- `[NEW FEATURE]` Added GitLab CI/CD pipeline. [Resolves 26]. [Merge !27].
- `[BUG FIX]` Fixed QueueDriverLinux message queue keys to be unique. Added delay to SystemLinux idle thread. [Resolves 30]. [Merge !31].
- `[REMOVED]` Removed legacy files that are no longer maintained. [Resolves #34]. [Merge !35].
- `[BUG FIX]` Fixed inclue paths in AsciiMessageHandlerTemplate. [Resolves 36]. [Merge !37].
- `[BUG FIX]` Fixed template instantiation syntax in MemoryRegion. [Resolves 38]. [Merge !39].
- `[IMPROVEMENT]` Updated build tests to not include removed legacy code. [Resolves 40]. [Merge !41].
- `[IMPROVEMENT]` Updated folder structure to improve readability and organization. Split out CI builds. [Resolves 42]. [Merge !43].
- `[REMOVED]` Removed unused CI and build scripts. [Resolves 45]. [Merge !46].
- `[REMOVED]` Removed Travis CI file. [Resolves 47]. [Merge !48].
