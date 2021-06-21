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
