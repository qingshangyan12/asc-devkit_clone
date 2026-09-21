# FIFO super Tensor verification

Run from the repository root:

```sh
python3 -m pytest -q tests/api/basic_api/dump_super_tensor/test_sender.py
```

Requires g++, pytest and AddressSanitizer/UndefinedBehaviorSanitizer. The test
executes the production serialization and sender functions with host substitutions
for DMA and transport. It checks ordinary/super selection, tiny FIFO capacities,
fragment continuity and contents, exact UB tails, insufficient local source capacity,
failed reservations and the fixed wire layout. This does not emulate concurrent
runtime collection or hardware DMA.

`compile.cpp` instantiates the public DumpTensor overloads for device compilation.
Use the repository includes before the installed devkit headers; the ASC driver may
prepend its own SDK include paths. Verify the actual dependency/preprocessor output.

The paired parser tests are in asc-tools:
`tests/py_ut/testcase/show_kernel_debug_data/test_super_tensor.py`.

Wire format: types 11/12, little endian packed headers of 88/36 bytes, byte lengths
and offsets, total record span rounded to 32 bytes, TLV length excluding the first
8 bytes. Local non-UB copies additionally require their aligned read range to fit
in the supplied LocalTensor capacity; unprovable ranges are rejected before a first
packet is published. Ordinary Tensor paths are unchanged.
