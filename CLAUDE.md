# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is Lib.Contract, a header-only C++ library for contract programming supporting C++17 and later. It enables expressing preconditions, postconditions, and invariants for functions, methods, classes, constructors, destructors, and loops.

## Build System & Commands

### CMake Configuration
- Configure: `cmake --preset default .`
- Build: `cmake --build build --config Debug --target all`
- Test: `ctest --preset default`
- Workflow (configure + build + test): `cmake --workflow --preset default`

### Available Presets
- `default`: Debug build configuration
- `release`: RelWithDebInfo build configuration

### Single Test Execution
Individual tests can be run directly from the build directory:
- `./build/test_examples` - runs examples test
- `./build/test_funcontract` - runs function contract tests
- `./build/test_classcontract` - runs class contract tests

## Architecture

### Core Components

1. **Main Header**: `include/contract/contract.hpp` - Single header providing all contract macros
2. **Contract Types**: Supports 7 contract scopes:
   - `contract(fun)` - free function contracts
   - `contract(this)` - method contracts
   - `contract(class)` - class invariant contracts
   - `contract(derived)` - derived class contracts with base class invariant enforcement
   - `contract(ctor)` - constructor contracts
   - `contract(dtor)` - destructor contracts
   - `contract(loop)` - loop invariant contracts

3. **Contract Checks**: Three types of assertions within contract blocks:
   - `precondition(condition [, message])` - checked on entry
   - `postcondition(condition [, message])` - checked on exit (unless exception)
   - `invariant(condition [, message])` - checked on entry and exit

4. **Violation Handling**: Custom violation handlers via `contract::set_handler()`

### Project Structure
- `include/contract/` - Main library headers
- `include/contract/detail/` - Implementation details and version info
- `test/` - Comprehensive test suite with feature-specific tests
- `cmake/` - CMake configuration files
- `src/` - Currently empty (header-only library)

### Contract Enforcement Rules
- Preconditions: Always checked on entry
- Postconditions: Checked on exit unless function/method exits via exception
- Invariants: Timing varies by contract type (see README for details)

## Compile-time Configuration

### Disabling Contract Types
Use these macros to selectively disable contract checking:
- `CONTRACT_DISABLE_PRECONDITIONS`
- `CONTRACT_DISABLE_POSTCONDITIONS`
- `CONTRACT_DISABLE_INVARIANTS`

## Requirements
- C++17 compiler (GCC 10+ recommended)
- Boost 1.81+ (for tests)
- CMake 3.28+
- Ninja build system

## Testing Strategy
- Each contract type has dedicated test files (e.g., `funcontract.t.cpp`, `classcontract.t.cpp`)
- Tests for disabling individual contract types
- Violation handler testing
- Examples showing real-world usage patterns

## Coding conventions

- Use `clang-format -i <files>` for consistent source code formatting
- Each source file has to end with trailing new line
