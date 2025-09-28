# Agents guide for automated contributors

## Project Overview
Lib.Contract is a header-only C++ library for contract programming supporting preconditions, postconditions, and invariants. Requires C++17 or later (set via `cxx_std_17` feature). Main API is `include/contract/contract.hpp`.

## Build / Lint / Test Commands
**Quick workflow (configure + build + test):**
- `cmake --workflow --preset default`

**Individual commands:**
- Configure (Debug): `cmake --preset default .`
- Configure (Release): `cmake --preset release .`
- Configure (Clang Debug): `cmake --preset default-clang .`
- Configure (Clang Release): `cmake --preset release-clang .`
- Build default: `cmake --build --preset default`
- Build release: `cmake --build --preset release`
- Build clang default: `cmake --build --preset default-clang`
- Build clang release: `cmake --build --preset release-clang`
- Run all tests: `ctest --preset default` (or `--preset release` for release)
- Run individual test: `./build/default/test/test_<name>` (e.g., `./build/default/test/test_examples`, `./build/default/test/test_funcontract`)
- Clang test location: `./build-clang/default/test/test_<name>`

**Available test targets:** classcontract, ctorcontract, derivedcontract, disableinvariants, disablepostconditions, disablepreconditions, dtorcontract, examples, funcontract, loopcontract, methcontract, violationhandler

## Code Style & Conventions
- **Language:** C++17 or later (enforced via `cxx_std_17`). Header-only library under `include/contract/`.
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
- Tests in `test/*.t.cpp` using Boost.Test framework (Boost 1.88.0)
- Test targets: `test_<basename>` (e.g., `test_funcontract`)
- Test executables in: `build/default/test/` (Debug) or `build/release/test/` (Release)
- Clang build tests in: `build-clang/default/test/` or `build-clang/release/test/`
- Dependencies managed via CPM (C++ Package Manager)
- CMake 3.28+, Ninja build system, GCC 10+ recommended
- Keep tests deterministic, small, and independent

## Repository Rules
- Respect `.gitignore` - never commit `build/`, `build-*/` directories or generated files
- Follow existing file structure and naming conventions
- Use `clang-format` for consistent source code formatting
- Each source file has to end with a trailing new line
- If `.cursor/rules`, `.cursorrules`, or `.github/copilot-instructions.md` exist, follow those rules
- GitHub Actions CI runs on Ubuntu, Windows, and macOS with both Debug and RelWithDebInfo builds

## Modification Workflow
1. **Read** relevant files first using view/str_replace tools
2. **Configure:** `cmake --preset default .` (or `--preset default-clang` for Clang)
3. **Build:** `cmake --build --preset default` (or use workflow: `cmake --workflow --preset default`)
4. **Test:** Run affected tests (`./build/default/test/test_<name>` or `ctest --preset default`)
5. **Commit:** Imperative messages explaining why changes were made

## Quick Checklist for Agents
- [ ] Read files before modifying
- [ ] Run `cmake --workflow --preset default` to verify changes
- [ ] Test specific features: `./build/default/test/test_<relevant_test>`
- [ ] Verify no build artifacts committed (no `build/` or `build-*` directories)
- [ ] Write clear commit messages
