# ASC Inherit CXX Standard Library Design

## Goal

Prevent bisheng from compiling ASC host code with a different libstdc++ header version than the C++ compiler
selected by CMake.

## Scope

The experiment changes only the PyTorch `pybind` sample target. It does not alter global ASC flags or other
examples.

## Design

- Read `CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES` after CMake detects the C++ compiler.
- Select directories whose paths contain `/c++/`; these are the libstdc++ include directories used by that exact
  compiler.
- For `COMPILE_LANGUAGE:ASC`, add `-nostdinc++` and re-add the selected directories as system include paths in
  their original order.
- Fail CMake configuration when no libstdc++ directory can be identified, instead of silently allowing bisheng to
  select another toolchain.

## Verification

Use the original two-input pybind implementation where `at::empty_like` remains in `add_custom.asc`. In
`zgx_ms1025`, verify that the generated ASC command uses `/usr/include/c++/9` instead of devtoolset-7 and that the
case no longer reports `Unknown device`. Also verify the sample in `zgx_smoke` with its native GCC 13 paths.
