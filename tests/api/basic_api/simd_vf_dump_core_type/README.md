# SIMD VF dump core-type regression (dav-3510)

This standalone ASC test executes the production VF producer and GM publisher.
It requires AIV labels, correct REG/UB data, and a GM FIFO flag transition from
0 to 1 without a scalar printf warmup. It never writes the FIFO flag itself.
The publisher uses the standard get_block_info() to validate the GM header and
set its core type. The test observes the header through its own read-only
ObserveDebugHead helper, which computes the address without initializing the
flag or calling a production getter. This prevents observation from masking
the regression and keeps test-only access out of production headers.
The optional control mode runs scalar printf first, demonstrating why existing
examples with scalar logging can mask a missing VF initialization.

From the repository root:

```bash
source /usr/local/Ascend/cann/bin/setenv.bash
cmake -S tests/api/basic_api/simd_vf_dump_core_type -B /tmp/vf-core-type \
  -DASC_SIMULATOR_LIB=/usr/local/Ascend/cann/x86_64-linux/simulator/Ascend950PR_9599/lib
cmake --build /tmp/vf-core-type -j4
ctest --test-dir /tmp/vf-core-type --output-on-failure
python3 tests/api/basic_api/simd_vf_dump_core_type/verify.py /tmp/vf-core-type/demo \
  --config /absolute/path/to/acl.json --runs 3 --output /tmp/vf-core-type-pure
python3 tests/api/basic_api/simd_vf_dump_core_type/verify.py /tmp/vf-core-type/demo \
  --config /absolute/path/to/acl.json --control 1 --output /tmp/vf-core-type-control
python3 tests/api/basic_api/simd_vf_dump_core_type/verify.py /tmp/vf-core-type/demo \
  --config /absolute/path/to/acl.json --blocks 2 --repeats 2 --output /tmp/vf-core-type-multicore
```

Use an ACL config containing `{"simd_printf_fifo_size_per_core":65536}`.
Each invocation starts a fresh process. Output directories must not contain the
requested run subdirectories; previous evidence is never overwritten.
The runner saves logs, exit status, timeout and verification results as JSON.
AIC labels, wrong flags/data, missing/duplicate records, nonzero exits and
timeouts fail the regression. Data is checked independently per block; block
interleaving is allowed. Python `-O` does not disable verification.

For hardware, omit `ASC_SIMULATOR_LIB` in a fresh build directory. To use the
cycle model, set `ASC_SIMULATOR_PV=OFF`. The default per-run timeout is 180 seconds.
Model success is not a claim of hardware validation.

CTest runs only the output-verifier unit tests. Device execution is explicit;
this directory is not added to the host-only basic_api build because it requires
an ASC compiler and a device/model runtime. Existing host dump tests validate
UB TLVs, not the GM header or runtime AIV prefix.

The ASC test also checks the standard DebugBlockHeadInfo size/alignment and
flag/ring/debug-bus offsets at device compile time. The VF publisher and its
GM head helpers use this same definition, without a cross-struct head cast.
These checks prevent ABI drift; they do not prove hardware cache ordering.
