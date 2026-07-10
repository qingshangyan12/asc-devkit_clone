# Pybind C++ Host Wrapper Design

## Goal

Verify whether the `Unknown device` failure is caused by invoking PyTorch C++ tensor factory APIs from an
ASC compilation unit. The `pybind` sample will allocate its output in a normal C++ compilation unit while keeping
the AscendC compilation unit responsible only for the device kernel and launch operation.

## Structure

- `add_custom.cpp` owns the pybind module, validates the input tensors, allocates the output with
  `at::empty_like`, obtains the current NPU stream, and calls the ASC launch bridge.
- `add_custom.asc` owns the AscendC kernel and an `extern "C"` launch bridge that accepts raw pointers, element
  count, and the NPU stream. It does not include PyTorch or pybind headers.
- `add_custom_test.py` returns to the original two-input call so the C++ wrapper allocation path is exercised.
- `CMakeLists.txt` compiles and links both source files into the same Python extension.

## Verification

Build and run only the `pybind` presmoke case in the existing `zgx_ms1025` environment. The experiment passes if
the two-input Python call succeeds and result comparison passes without `Unknown device`.
