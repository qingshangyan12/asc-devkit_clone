# asc_relu_pre_mode

<!-- md-trans-meta sourceCommit=d04c597734e437e5ee87a6dd3b47f91fe427e33f translatedAt=2026-10-23T00:04:23.338Z pushedAt=2026-10-28T12:47:03.103Z nl=2 -->

`asc_relu_pre_mode` specifies the inline ReLU mode applied when data is moved out of the L0C Buffer, and serves as the type of the `relu_pre_mode` parameter in the matrix data transfer API.

The header file path is `"c_api/defs/enum.h"`.

## Enum Class Definition

```cpp
enum class asc_relu_pre_mode : uint8_t {
    NONE = 0,
    NORMAL = 1,
    SCALAR = 2,
    VECTOR = 3,
};
```

## Enumeration Value Details

**Table 1** Enumeration value description

| Enumeration Value | Value | Description |
| --- | --- | --- |
| NONE | 0 | Does not execute inline ReLU. |
| NORMAL | 1 | Executes standard ReLU. |
| SCALAR | 2 | Executes inline ReLU using Scalar parameters. |
| VECTOR | 3 | Executes inline ReLU using Vector parameters. |
