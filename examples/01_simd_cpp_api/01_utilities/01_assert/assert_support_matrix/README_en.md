# SIMD VF Assert Support Matrix

## Overview

This example groups the currently compilable SIMD VF `assert` forms and the known compiler limitations. The default code enables one effective assert site and deliberately fails its runtime condition, verifying that the assertion records are published through the FIFO before a Vector Core exception is raised.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---------|-----------------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
├── assert_support_matrix
│   ├── CMakeLists.txt                // Build project file
│   ├── assert_support_matrix.asc     // Supported and currently limited forms
│   ├── README.md                     // Chinese description
│   └── README_en.md                  // English description
```

## Support Matrix

The enabled `SupportedAssert` combines these compilable forms into one effective assert site:

- Combined scalar conditions and a Boolean intermediate.
- An assert with a format string and arguments.
- Comparing an `__ubuf__` pointer with integer `0`.

The following forms are grouped in uninstantiated templates. Their call sites remain commented out so the example builds by default:

- An assert predicate that directly compares an `__ubuf__` pointer with `nullptr`. The current BiSheng backend reports `Do not know how to expand this operator's operand`.
- Two or more effective assert sites in one SIMD VF function. The current BiSheng compiler reports `stack smashing` during VF DAG instruction selection and exits.

These are current compiler limitations, not assert API semantic restrictions. To reproduce one, uncomment only its corresponding call site.

## Build and Run

Run the following commands in this example directory:

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
./demo
```

The program passes `5` to the kernel so `value < 4U` fails. The log should contain the custom assertion message and the `Assertion ... failed` record before stream synchronization returns a Vector Core exception. The program treats this expected exception as success and returns 0.
