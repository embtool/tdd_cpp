# TDD C/C++ Project Template - CppUTest and Unity

by [Djones A. Boni](https://github.com/djboni)

C/C++ Project Template for Test Driven Development using
[CppUTest](https://github.com/cpputest/cpputest) and
[Unity](https://github.com/ThrowTheSwitch/Unity) test frameworks.

Unity test runners are generated automatically.

Tests for CppUTest are automatically converted to Unity, so they can
run on platforms that only have a C compiler.

It is possible to disable the compilation and execution of CppUTest
tests and Unity tests by setting `CPPUTEST_COMPILE_TESTS`,
`CPPUTEST_CONVERT_TESTS`, and `UNITY_COMPILE_TESTS` variables on CMake
(see the table below).

| CMake definitions               | Description                           |
| ------------------------------- | ------------------------------------- |
| `-D DEV_BUILD=OFF`              | Disable developer build.              |
| `-D CPPUTEST_COMPILE_TESTS=OFF` | Disable CppUTest tests.               |
| `-D CPPUTEST_CONVERT_TESTS=OFF` | Disable CppUTest conversion to Unity. |
| `-D UNITY_COMPILE_TESTS=OFF`    | Disable Unity tests.                  |

## Table of Contents

- [Quick-start](#quick-start)
- [Developing an Application](#developing-an-application)

## Quick-start

Create a build directory:

```console
$ mkdir build
$ cd build
```

Build:

```console
$ cmake ..
$ cmake --build .
```

Run the tests:

```console
$ ctest
```

Run the application:

```console
$ ./main
```

## Developing an Application

1. Add header files in include/.
2. Add source files in src/.
3. Add test files in tests/.
4. Add header and source files to the _embedded_ application library on
   CMakeLists.txt.
5. Add one test for each test file on tests/CMakeLists.txt.

One-liner to build and test:

```console
$ cmake .. && cmake --build . && ctest --output-on-error
```

## AVR toolchain

CppUTest does not build with avr-g++, so we disable it.

```console
$ cmake .. --toolchain ../cmake/toolchains/avr.cmake -DCPPUTEST_COMPILE_TESTS=OFF
$ cmake --build .
```

## AVR toolchain

CppUTest uses too much dynamic memory (heap RAM), so we disable it.

```console
$ cmake .. --toolchain ../cmake/toolchains/arm.cmake -DCPPUTEST_COMPILE_TESTS=OFF
$ cmake --build .
```
