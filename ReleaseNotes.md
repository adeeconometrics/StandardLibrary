# Release Notes:

## v0.0.2a

- Added the `SinglyList` and `DoublyList` implementation
- Created comprehensive test suite for `SinglyList` and `DoublyList`
- Fixed issue with bidirectional iterators for doubly list
- Cleaned entire codebase with Clang Format
- Introduced new technique for handling `operator--` and `operator--(int)` in iterators with `IteratorProxy`
- Updated documentation with current List APIs

## v0.0.2

- Updated API documentation
- Added contribution guidelines for creating and reviewing unit tests
- Removed bash scripts as it is written in GH Actions already
- Enhanced code coverage for testing Array-based data structures
- Completed base implementation for Queue, Stack, and Deque in Array container

## v0.0.1

- Added autodoc via doxygen and sphinx
- Created a build configuration for setting up debug and release modes
- Added workflow for continuous testing and deployment of API reference
- Moved legacy data structures and utilities modules back and exposed only the linear data structure implementation for review and design inspection; at the moment main changes are reorganized. Design spec shall be followed in future releases.