# asc_dcci_entire_ub Example

## Overview

This example validates that `asc_dcci_entire_ub` refreshes the DCache entries associated with UB on Ascend 950 and the `dav-3510` architecture. The kernel is declared as `__vector__ __global__` and runs only on the Vector Core.

## Test Design

The AIV kernel observes two UB locations separated by 128 bytes. It writes the old value `11` with the Vector pipeline, sets CTRL[49] to make Scalar UB reads cacheable, and warms both DCache lines. The Vector pipeline then overwrites UB with `101`.

Both Scalar reads must still return `11` before `asc_dcci_entire_ub()`. After the API call and `asc_sync_data_barrier(mem_dsb_t::DSB_UB)`, both reads must return `101`. The original CTRL value is restored before the kernel exits. A run where the pre-DCCI reads already return `101` is rejected because it does not prove cache inconsistency.

## Expected Observations

| Observation | Expected value |
| --- | --- |
| `ctrl_bit49` | 1 |
| `warm_line0`, `warm_line1` | 11, 11 |
| `before_line0`, `before_line1` | 11, 11 |
| `after_line0`, `after_line1` | 101, 101 |

## CANN Symlink Prerequisite

`test.sh` validates but never changes the CANN installation. The following paths must be symbolic links to this repository:

```text
/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/include/c_api
  -> /home/g00954335/asc-devkit_GRJ/include/c_api
/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/impl/c_api
  -> /home/g00954335/asc-devkit_GRJ/impl/c_api
```

Set `ASCEND_CANN_PACKAGE_PATH` to use another CANN installation path.

## Build and Run

Run the following command in this directory:

```bash
bash test.sh
```

The script checks the links, recreates only this example's `build` directory, builds for `dav-3510` in NPU mode, runs the executable, and requires an exact `test pass!` line.

## Merge Decision

The original CANN 9.2.0-beta.1 tree fails to compile the call because the API is not declared for 3510. With this repository linked, the sample builds, all six neighboring DCCI unit tests pass, and three clean NPU runs consistently observe `11, 11` before DCCI and `101, 101` after DCCI plus DSB. Based on this evidence, the API is ready to merge.
