# ASC CPU Architecture Mapping Refactor

## Scope

Refactor only the `CMAKE_ASC_RUN_MODE STREQUAL "cpu"` branch in
`CMakeASCInformation.cmake`. The simulator branch and other ASC language rules
remain unchanged.

## Design

Replace the flat three-column `_ARCH_TO_SOC_DIR_MAP` and index arithmetic with
an explicit `if`/`elseif` mapping. Each supported architecture assigns both the
CPU runtime directory and model library:

| Architecture | CPU runtime directory | Model library |
| --- | --- | --- |
| `dav-2002` | `Ascend310P1` | `_pvmodel` |
| `dav-2201` | `Ascend910B1` | `pem_davinci` |
| `dav-3510` | `Ascend950PR_9599` | `pem_davinci` |

The link library list will consume the selected model library directly instead
of reevaluating architecture generator expressions. `_SHORT_NPU_ARCH`, list
indices, and modulo validation will be removed because they no longer serve a
consumer.

## Behavior

Supported architectures, library search paths, linked libraries, compile
flags, include directories, and error behavior remain unchanged. Unsupported
architectures still fail during configuration with the existing diagnostic.

## Verification

- Configure CPU mode for all three supported architectures.
- Confirm unsupported architecture configuration fails.
- Build and run the official CPU static-library and shared-library examples.
- Run `git diff --check`.
