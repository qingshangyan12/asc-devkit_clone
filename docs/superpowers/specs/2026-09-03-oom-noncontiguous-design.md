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
[u4 magic = 1][u4 version = 1][u8 reserved]
[OOM descriptor block]...
```

The first byte stores `magic` in bits 3:0 and `version` in bits 7:4; the second
byte stores `reserved`. All multi-byte integers use the existing runtime ABI
byte order. Descriptor blocks are packed exactly as specified below and contain no implicit alignment padding.
`size` includes the complete descriptor block, including its own `size` field.

### Single Tensor Descriptor

```text
offset 0:  u64 size = 19
offset 8:  u8 type = 1
offset 9:  u8 reserved
offset 10: TensorViewDesc
```

### TensorList Descriptor

```text
offset 0:  u64 size = 12 + count * 9
offset 8:  u8 type = 2
offset 9:  u8 flag
offset 10: u16 count
offset 12: TensorViewDesc[count]
```

`flag` identifies TensorList U/F mode. This extension is independent of and
does not alter the existing dynamic TensorList second-level-pointer layout.

### TensorViewDesc

```text
offset 0, bits 3:0: u4 version = 1
offset 0, bits 7:4: u4 reserved
offset 1: u64 storageShapeSize
```

`storageShapeSize` is the storage shape's element count. The byte length for
one view is `storageShapeSize * elementSize`. The implementation validates the
descriptor version, exact block size, count, nonzero element count and size
multiplication overflow before using a descriptor.

## Mapping and Runtime Selection

Descriptor blocks are appended and consumed in the current OOM registration
order: inputs followed by outputs. Output-shape and workspace retain their
existing legacy fields and do not receive `TensorViewDesc` blocks.

At runtime, use storage-shape parsing only when both conditions are true:

1. The loaded JSON declares `oom.version = 1`.
2. The trailing extension begins with `magic = 1` and has supported version 1.

For each ordinary tensor, the byte length calculated from the parsed storage
shape element count replaces the legacy view/logical-shape-derived length.
TensorList descriptors are parsed from their own `type = 2` blocks, while the
legacy second-level-pointer block is left untouched. A missing magic, missing
capability JSON, or unsupported extension version selects the existing OOM
argument interpretation.

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
- Add extension tests for a larger storage shape element count and several
  TensorList entries in both U and F modes.
- Add negative tests for bad magic, unsupported version, undersized `size`,
  invalid `type`, truncated block, invalid count, zero storage shape size, and
  size multiplication overflow.
- Verify JSON output with and without OOM capability and loader acceptance of
  both forms.
