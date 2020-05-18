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

### Unreleased Changes

Released: XXXX-XX-XX

- `[TEMPLATE]` Description of changes. [Resolves #issue]. [Merge !mr].
- `[CRITICAL BUG FIX]` Fixed ComLink initialization order to prevent segfault on Linux. [Resolves 13]. [Merge !15].
- `[NEW FEATURE]` Added maskBits() inline function to Plat4m.h. [Resolves 17]. [Merge !18].
- `[BUG FIX]` Made all FreeRTOS paths relative. [Resolves 19]. [Merge !20].
- `[BUG FIX]` Fixed FreeRTOS include path case. [Resolves 21]. [Merge !22].
- `[BUG FIX]` Fixed FreeRTOS include path case in ThreadFreeRtos.cpp. [Resolves 23]. [Merge !24].
- `[BUG FIX]` Fixed enabling and disabling of Linux pthreads. [Resolves 14]. [Merge !25].
- `[NEW FEATURE]` Added GitLab CI/CD pipeline. [Resolves 26]. [Merge !27].
- `[BUG FIX]` Fixed QueueDriverLinux message queue keys to be unique. Added delay to SystemLinux idle thread. [Resolves 30]. [Merge !31].
