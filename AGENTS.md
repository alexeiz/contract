# Agents guide for automated contributors

## Project Overview
Lib.Contract is a C++17 header-only library for contract programming supporting preconditions, postconditions, and invariants. Version 0.4.3. Main API is `include/contract/contract.hpp`.

## Build / Lint / Test Commands
**Quick workflow (configure + build + test):**
- `cmake --workflow --preset default`

**Individual commands:**
- Configure (Debug): `cmake --preset default .`
- Build all: `cmake --build build --config Debug --target all`
- Run all tests: `ctest --preset default`
- Run individual test: `./build/test_<name>` (e.g., `./build/test_examples`, `./build/test_funcontract`)
- Release build: `cmake --preset release .` then `cmake --build build --config RelWithDebInfo`

**Available test targets:** classcontract, ctorcontract, derivedcontract, disableinvariants, disablepostconditions, disablepreconditions, dtorcontract, examples, funcontract, loopcontract, methcontract, violationhandler

## Code Style & Conventions
- **Language:** C++17. Header-only library under `include/contract/`.
- **Indentation:** 4 spaces; max ~100 chars per line.
- **File naming:** Public headers `include/contract/*.hpp`, internal `include/contract/detail/*.hpp` (snake_case).
- **Includes:** Angle-brackets for external libs, quoted includes for internal; prefer `#include "contract/contract.hpp"`.
- **Types:** Standard library types, constexpr/noexcept where appropriate.
- **Naming:** PascalCase for types/classes, camelCase for functions/variables, UPPER_SNAKE_CASE for macros/config defines.
- **Error handling:** Exceptions for fatal errors in tests; library headers avoid throwing; tests use Boost.Test macros.
- **Formatting:** Follow existing indentation and brace style.

## Contract API Overview
**Seven contract scopes:**
- `contract(fun)` - free function contracts
- `contract(this)` - method contracts
- `contract(class)` - class invariant contracts
- `contract(derived)` - derived class contracts
- `contract(ctor)` - constructor contracts
- `contract(dtor)` - destructor contracts
- `contract(loop)` - loop invariant contracts

**Three assertion types within contract blocks:**
- `precondition(condition [, message])` - checked on entry
- `postcondition(condition [, message])` - checked on exit (unless exception)
- `invariant(condition [, message])` - checked on entry/exit (timing varies by scope)

**Compile-time disabling:**
- `CONTRACT_DISABLE_PRECONDITIONS`
- `CONTRACT_DISABLE_POSTCONDITIONS`
- `CONTRACT_DISABLE_INVARIANTS`

## Tests and Dependencies
- Tests in `test/*.t.cpp` using Boost.Test framework (Boost 1.88+)
- Test targets: `test_<basename>` (e.g., `test_funcontract`)
- CMake 3.28+, Ninja build system, GCC 10+ recommended
- Keep tests deterministic, small, and independent

## Repository Rules
- Respect `.gitignore` - never commit `build/` directory or generated files
- Follow existing file structure and naming conventions
- Use `clang-format` for consistent source code formatting
- Each source file has to end with a trailing new line
- If `.cursor/rules`, `.cursorrules`, or `.github/copilot-instructions.md` exist, follow those rules

## Modification Workflow
1. **Read** relevant files first using view/str_replace tools
2. **Configure:** `cmake --preset default .`
3. **Build:** `cmake --build build --config Debug --target all`
4. **Test:** Run affected tests (`./build/test_<name>` or `ctest --preset default`)
5. **Commit:** Imperative messages explaining why changes were made

## Quick Checklist for Agents
- [ ] Read files before modifying
- [ ] Run `cmake --workflow --preset default` to verify changes
- [ ] Test specific features: `./build/test_<relevant_test>`
- [ ] Verify no build artifacts committed
- [ ] Write clear commit messages
