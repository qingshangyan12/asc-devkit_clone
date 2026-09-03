# OOM Non-Contiguous Tensor Support

## Goal

Extend OOM address-range validation so a kernel compiled with OOM support can use
runtime-provided storage shapes for non-contiguous tensor views. One binary must
continue to accept the legacy argument layout when the new metadata is absent.

## Scope

- Preserve the existing tiling data, dynamic TensorList second-level-pointer
  format, output-shape, and workspace layout.
- Append an optional OOM extension after the existing OOM argument payload.
- Describe OOM metadata capability in the single-operator JSON.
- Keep the device-side OOM access-validation entry points unchanged. Only the
  range-registration source changes according to the optional extension.

## Capability JSON

An OOM-enabled single-operator JSON contains:

```json
"oom": {
  "version": 1,
  "tensor_version": 1
}
```

The field says the binary understands extension layout version 1 and
`TensorViewDesc` version 1. A JSON without this field remains a legacy binary.

## Extension Layout

The extension is appended after the established layout:

```text
[TilingData ... IN_DYN ... OUT_0 ... OUTSHAPE ... WS]
[u64 magic = 1][u32 version = 1][u32 reserved]
[OOM descriptor block]...
```

All integers use the existing runtime ABI byte order and natural 8-byte offsets
for `u64` values. `size` includes the complete descriptor block, including its
own `size` field.

### Single Tensor Descriptor

```text
[u64 size][u32 kind = 1][u32 reserved][TensorViewDesc]
```

### TensorList Descriptor

```text
[u64 size][u32 kind = 2][u32 flag][u64 count]
[TensorViewDesc] * count
```

`flag` identifies TensorList U/F mode. This extension is independent of and
does not alter the existing dynamic TensorList second-level-pointer layout.

### TensorViewDesc

```text
[u32 version = 1][u32 nDim][u64 storage_shape] * nDim
```

The byte length for one view is the product of `storage_shape` dimensions and
the tensor element size. The implementation validates descriptor version,
minimum block size, `nDim`, dimension values, multiplication overflow, count,
and block bounds before using a descriptor.

## Mapping and Runtime Selection

Descriptor blocks are appended and consumed in the current OOM registration
order: inputs followed by outputs. Output-shape and workspace retain their
existing legacy fields and do not receive `TensorViewDesc` blocks.

At runtime, use storage-shape parsing only when both conditions are true:

1. The loaded JSON declares `oom.version = 1`.
2. The trailing extension begins with `magic = 1` and has supported version 1.

For each ordinary tensor, the parsed storage-shape byte length replaces the
legacy view/logical-shape-derived length. TensorList descriptors are parsed
from their own `kind = 2` blocks, while the legacy second-level-pointer block is
left untouched. A missing magic, missing capability JSON, or unsupported
extension version selects the existing OOM argument interpretation.

Malformed metadata after a valid magic is an invalid OOM extension and must not
be interpreted as legacy data; parsing must fail safely before range use.

## Compatibility

- New OOM binary with legacy runtime args: magic absent, use legacy path.
- New OOM binary with extension-capable runtime args: JSON and magic present,
  use storage-shape path.
- Legacy OOM binary: no JSON capability; runtime uses prior args layout.
- Non-OOM binary: no OOM JSON field or extension behavior.
- `aclnn` loader must accept both JSON forms and must not require the `oom`
  property.

## Validation

- Existing static and dynamic ordinary-tensor OOM tests remain unchanged.
- Existing TensorList U and F tests remain unchanged with no extension.
- Add extension tests for a padded storage shape, rank 0/1/max-rank values, and
  several TensorList entries.
- Add negative tests for bad magic, unsupported version, undersized `size`,
  invalid `kind`, truncated block, invalid count, invalid dimension, and size
  multiplication overflow.
- Verify JSON output with and without OOM capability and loader acceptance of
  both forms.
