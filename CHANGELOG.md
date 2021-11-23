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

- `[IMPROVEMENT]` Added toTimeSFloat() to TimeStamp class. [Resolves 64].
- `[IMPROVEMENT]` Added integerDivideRound() function to Plat4m.h for fast integer divides where the result should be rounded to the next closest multiple of the divisor. [Resolves 62].
- `[IMPROVEMENT]` Added appendCast() and prependCast() methods to Array class. [Resolves 61].
- `[BUG FIX]` Fixed TimeStamp::toTime() calculations and added const qualifiers. [Resolves 60].
- `[IMPROVEMENT]` Changed SystemSimulation time tracking so that it doesn't roll over often due to microsecond counter. Now won't roll over for 139 years (2^32 seconds). Added additional convenience methods to TimeStamp. [Resolves 58].
- `[IMPROVEMENT]` Added not-equals operator and fromTime() methods to TimeStamp class. [Resolves 57].
- `[IMPROVEMENT]` Changed default value of insert timestamp parameter in Topic::publish(). [Resolves 55].
- `[BUG FIX]` Fixed undedefined function warning in Callback class. [Resolves 54].
- `[IMPROVEMENT]` Added addition operators to TimeStamp class. [Resolves 52].
- `[IMPROVEMENT]` Added setTime() method to System and added an implmentation in SystemSimulation. [Resolves 53].
- `[BUG FIX]` Fixed bug in Topic::publish() to use the sample copy. [Resolves 51].
- `[NEW FEATURE]` Added Service and supporting classes. [Resolves 48].
- `[IMPROVEMENT]` Changed Callback to be a variadic template. Updated CallbackMethod and CallbackFunction. Deprecated Callback parameter variants. [Resolves 48].
- `[IMPROVEMENT]` Added flag to Topic::publish() to give option for prepopulated time stamps. [Resolves 47].
- `[IMPROVEMENT]` Made TimeStamp struct a separate class with comparison operators. [Resolves 46].
- `[IMPROVEMENT]` Added unsubscribe() method to Topic class. TopicSubscriber destructor now calls Topic::unsubscribe(). [Resolves 45].
- `[BUG FIX]` Fixed enable/disable race condition in ThreadLinux. Improved WaitConditionLinux notify logic. [Resolves 44].
- `[NEW FEATURE]` Added SystemSimulation wrapper class that adds simulation capabilities to any System implementation. Removed old Linux simulation classes. [Resolves 41].
- `[NEW FEATURE]` Added Semaphore interface class. Updated System interface and all implementations. [Resolves 42].
- `[IMPROVEMENT]` Added simulated flag and stack bytes count parameters to TopicSubscriberThread constructors. [Resolves 43].
- `[BUG FIX]` Fixed missing driverExit() methods in SystemLite, SystemWindows, and SystemFreeRtos. [Resolves 40].
- `[IMPROVEMENT]` Changed TopicSubscriber to be a subclass of Module to allow enabling/disabling. [Resolves 38].
- `[IMPROVEMENT]` Added VS Code build and debug support. [Resolves 39].
- `[NEW FEATURE]` Added Linux simulation System implementation. Added time stamp and wall time stamp retrieval to System interface. Added Topic header with sequence ID and time stamp. [Resolves 31].
- `[IMPROVEMENT]` Fixed missing FreeRTOS-Kernel dependency and MemoryAllocation reference syntax for failing CI builds. [Resolves 37].
- `[IMPROVEMENT]` Changed build scripts to report errors. [Resolves 36].
- `[BUG FIX]` Fixed allocate() syntax in BoardNRF5340PDK. [Resolves 29].
- `[BUG FIX]` Fixed bug in List::remove() for 2-element lists. [Resolves 28].
- `[IMPROVEMENT]` Added clearing of driver pointer in Processor destructor. [Resolves 27].
- `[IMPROVEMENT]` Added call in Topic destructor to remove it from static List in Topic class. [Resolves 26].
- `[IMPROVEMENT]` Added remove() method to List. [Resolves 25].
- `[NEW FEATURE]` Added MemoryAllocator class that uses placement new operator. Removed previous new/delete references and global new/delete operator overloading. [Resolves 24].
- `[IMPROVEMENT]` Added call to Thread destructor in HardwareTimerSimulated destructor. [Resolves 23].
- `[IMPROVEMENT]` Made Thread destructor public. [Resolves 22].
- `[IMPROVEMENT]` Added clearing of driver pointer in System destructor. [Resolves 21].
- `[IMPROVEMENT]` Added getThread() accessor to HardwareTimerSimulated. [Resolves 20].
- `[IMPROVEMENT]` Added exit logic to ThreadLinux. [Resolves 19].
- `[NEW FEATURE]` Added enable()/disable() methods to AllocationMemory by making it subclass of Module. Cleaned up nested driver logic. [Resolves 16].
- `[NEW FEATURE]` Added resubscribe() method to TopicSubscriber. [Resolves 15].
- `[IMPROVEMENT]` Updated QueueDriverLinux implementation to handle any data type. [Resolves 54]. [Merge !59].
- `[NEW FEATURE]` Added Topic, TopicSubscriber, and TopicSubscriberThread modules for lightweight internal publish/subscribe messaging. [Resolves 51]. [Merge !60].
- `[IMPROVEMENT]` Added docker tag to GitLab CI jobs. [Resolves 63]. [Merge !64].
- `[IMPROVEMENT]` Changed SampleType parameter in Topic::SampleCallback to be const reference. [Resolves 67]. [Merge !69].
- `[IMPROVEMENT]` Added getThread() method to TopicSubscriberThread class. [Resolves 68]. [Merge !70].
- `[BUG FIX]` Fixed syntax issue in Plat4m::isBitSet(). [Resolves 71]. [Merge !72].
- `[NONFUNCTIONAL]` Added design documentation for System, Thread, and ComInterface modules. [Resolves 8]. [Merge !77].
- `[IMPROVEMENT]` Removed unneeded files from ST Standard Peripheral Library for F3 and F4. [Resolves 6].
- `[IMPROVEMENT]` Removed unused CMSIS math libraries. [Resolves 7].
- `[IMPROVEMENT]` Added single nested driver in AllocationMemory. [Resolves 10].
- `[IMPROVEMENT]` Added copy constructor and assignment operator to TopicSubscriber. [Resolves 12].
- `[NEW FEATURE]` Added HardwareTimerSimulated as a HardwareTimer driver for a simulated timer. [Resolves 11].
- `[IMPROVEMENT]` Updated FreeRTOS relative path references to allow using GitHub submodule. [Resolves 13].
- `[NEW FEATURE]` Added minimal drivers for NRF5340 microcontroller. [Resolves 4].

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
