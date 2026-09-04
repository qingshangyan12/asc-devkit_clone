# Validation Report

## Scope

The samples in this directory target the Vector/UB C APIs supported by Atlas A2/A3 (`dav-2201`). This is intentional: the corresponding C API documentation marks Ascend 950PR/950DT (`dav-3510`) unsupported.

## Static Build Results

All commands below completed successfully with the local CANN 9.2.0 toolchain. The compiler emitted only its existing deprecated-attribute warning from `c_api/atomic/atomic.h`.

| Sample | Configuration | Result |
| --- | --- | --- |
| `element_wise_arithmetic` | `-DCMAKE_ASC_ARCHITECTURES=dav-2201` | Built |
| `reduce` | `-DCMAKE_ASC_ARCHITECTURES=dav-2201` | Built |
| `compare` | `-DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=1..4` | Built for all four scenarios |
| `cast` | `-DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=0,1` | Built for both scenarios |

## Runtime Validation Status

Not run. The available NPUSim environment is A5 (`dav-3510`) only, while these samples require A2/A3 (`dav-2201`). Running them under A5 would contradict the C API product-support constraints and would not validate the supported target.

Run the self-checking executables on a `dav-2201` device or a `dav-2201`-capable simulator before claiming runtime or precision validation. Each successful run prints `test pass!` and returns zero.
