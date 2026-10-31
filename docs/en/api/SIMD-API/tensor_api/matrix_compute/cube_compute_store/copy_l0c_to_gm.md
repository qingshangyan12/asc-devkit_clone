# copy (Movement from L0C Buffer to Global Memory)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-27T10:34:31.105Z pushedAt=2026-10-27T13:04:46.130Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- AI Core in Atlas inference products: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Vector Core in Atlas inference products: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `tensor_api/tensor.h`.

The Tensor API uses the `copy` API to uniformly perform movement across different paths. This API moves the cube computation results in the L0C Buffer to Global Memory. The data in the L0C Buffer is usually the output of `mmad`, in the `NZ` data format. When moving to Global Memory, the API automatically selects the inline format conversion from `NZ` to `ND`, `DN`, `NZ`, `NC1HWC0`, `NHWC`, or `NCHW` based on the destination tensor layout. Among these, `NC1HWC0`, `NHWC`, and `NCHW` are used for convolution output feature maps.

The API supports complete tensor movement, and also supports regional movement by specifying the start coordinate in the destination tensor, the start coordinate in the source tensor, and the shape of the movement region through `dst_coord`, `src_coord`, and `copy_shape`. Both non-quantized movement and quantized movement support this form.

L0C Buffer-to-Global Memory movement supports non-quantized output, direct conversion output from `float` to `half` or `bfloat16_t`, and real-time quantized output with scalar or tensor quantization parameters. Real-time Relu, channel split, and the quantization rounding method are configured through `l0c_to_gm_trait`. The `unit_flag` related to fine-grained parallelism between `mmad` and `Fixpipe` is configured through `l0c_to_gm_params`.

The API supports batch mode. Batch mode is used to complete the movement of multiple cube computation results at once. The layout adds a Batch dimension to the outermost layer of the original cube layout. The source tensor is in the `NZ` format, with the fractal fixed at 16×16, and can be constructed through `make_frame_layout<nz_layout_ptn>(batch, m, n)`. The destination tensor can be constructed through `make_frame_layout<nd_layout_ptn>(batch, m, n)`, `make_frame_layout<dn_layout_ptn>(batch, m, n)`, `make_frame_layout<nd_ext_layout_ptn>(batch, m, n)`, `make_frame_layout<dn_ext_layout_ptn>(batch, m, n)`, or `make_frame_layout<nz_layout_ptn, DstType>(batch, m, n)`. For the `NZ` format, the destination data type can be specified through the template parameter `DstType`, and `C0` indicates the column fractal size of the NZ format, which defaults to 16. The convolution output path currently supports only N equal to 1 and does not support batch mode.

The valid combinations, intermediate data types, and movement paths of real-time quantization, real-time Relu, real-time format conversion, real-time channel split, and real-time channel merge are shown in the following figure. In the figure, F32 to F16 and F32 to BF16 are non-quantization modes that perform only casting. The remaining paths are non-quantization mode, real-time scalar quantization mode, or real-time tensor quantization mode.

For Ascend 950PR&950DT products:

**Figure 1** L0C2GM flow diagram

![L0C2GM flow diagram](../../../../figures/L0C2GM_Function_Combination_950.png)

## Function Prototype

- Performs non-quantized movement from the L0C Buffer to Global Memory.

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src)
    ```

- Automatically derives the movement path based on the storage locations of the source tensor and destination tensor, and performs non-quantized movement from the L0C Buffer to Global Memory using the default trait.

    ```cpp
    template <typename DstTensor, typename SrcTensor>
    __aicore__ inline void copy(const DstTensor& dst, const SrcTensor& src)
    ```

- Performs quantized movement from the L0C Buffer to Global Memory.

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor, typename Quant>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst, const SrcTensor& src,
        const Quant& quant)
    ```

- Automatically derives the movement path based on the storage locations of the source tensor and destination tensor, and performs quantized movement from the L0C Buffer to Global Memory using the default trait.

    ```cpp
    template <typename DstTensor, typename SrcTensor, typename Quant>
    __aicore__ inline void copy(const DstTensor& dst, const SrcTensor& src, const Quant& quant)
    ```

- Executes non-quantized movement according to the specified source coordinate, destination coordinate, and movement shape.

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor, typename DstCoord,
        typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst,
        const SrcTensor& src, const DstCoord& dst_coord, const SrcCoord& src_coord,
        const CopyShape& copy_shape)
    ```

- Automatically derives the movement path based on the storage locations of the source and destination tensors, and executes non-quantized movement according to the specified source coordinate, destination coordinate, and movement shape using the default trait.

    ```cpp
    template <typename DstTensor, typename SrcTensor, typename DstCoord,
        typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const DstTensor& dst,
        const SrcTensor& src, const DstCoord& dst_coord, const SrcCoord& src_coord,
        const CopyShape& copy_shape)
    ```

- Executes quantized movement according to the specified source coordinate, destination coordinate, and movement shape.

    ```cpp
    template <typename Atom, typename DstTensor, typename SrcTensor, typename Quant,
        typename DstCoord, typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const copy_atom<Atom>& atom, const DstTensor& dst,
        const SrcTensor& src, const Quant& quant, const DstCoord& dst_coord,
        const SrcCoord& src_coord, const CopyShape& copy_shape)
    ```

- Automatically derives the movement path based on the storage locations of the source and destination tensors, and executes quantized movement according to the specified source coordinate, destination coordinate, and movement shape using the default trait.

    ```cpp
    template <typename DstTensor, typename SrcTensor, typename Quant,
        typename DstCoord, typename SrcCoord, typename CopyShape>
    __aicore__ inline void copy(const DstTensor& dst,
        const SrcTensor& src, const Quant& quant, const DstCoord& dst_coord,
        const SrcCoord& src_coord, const CopyShape& copy_shape)
    ```

- Constructs a movement atomic object using the default trait.

    ```cpp
    template <typename CopyOperation>
    __aicore__ inline constexpr auto make_copy(const CopyOperation& operation)
    ```

- Constructs a movement atomic object using the specified trait.

    ```cpp
    template <typename CopyOperation, typename CopyTrait>
    __aicore__ inline constexpr auto make_copy(const CopyOperation& operation, const CopyTrait& trait)
    ```

## Parameters

**Table 1** Template parameter description

| Parameter | Description |
| :--- | :--- |
| Atom | Template parameter of `copy_atom`, derived by the `make_copy` API. |
| DstTensor | Destination tensor type, which must be a Tensor API Tensor type. |
| SrcTensor | Source tensor type, which must be a Tensor API Tensor type. |
| Quant | Quantization parameter type, which can be `uint64_t` or a Tensor API tensor type that meets the requirements. |
| DstCoord | Start coordinate type of the destination tensor, which can also be `zero_coord`. |
| SrcCoord | Start coordinate type of the source tensor, which can also be `zero_coord`. |
| CopyShape | Type of the movement region shape. |
| CopyOperation | Type of the movement operation object. |
| CopyTrait | Type of the movement trait object. |

**Table 2**  `copy` API parameter description

|Parameter|Input/Output|Description|
|--------|--------|--------|
|atom|Input|Movement atom object, which can be constructed by the `make_copy` API.|
|dst|Output|Destination tensor, whose storage location is `location::gm`. Supports the `ND`, `DN`, `NZ`, `NC1HWC0`, `NHWC`, and `NCHW` data formats.|
|src|Input|Source tensor, stored at `location::l0c`, with the data format `NZ`, typically the computation result of `mmad`.|
|quant|Input|Optional quantization parameter. Passing `uint64_t` indicates a scalar quantization parameter; passing a tensor indicates a tensor quantization parameter. The tensor is located in the L1 Buffer, with the element type `uint64_t`.|
|dst_coord|Input|Start coordinate of the movement region in the destination tensor; can also be `zero_coord`.|
|src_coord|Input|Start coordinate of the movement region in the source tensor; can also be `zero_coord`.|
|copy_shape|Input|Shape of the movement region, used to specify the size of the movement region.|

To configure runtime movement parameters, first bind the `l0c_to_gm_params` object to the movement atomic object through `atom.with(params)`; if not bound, the default values are used.

**Table 3**  `make_copy` API parameter description

| Parameter | Input/Output | Description |
| :--- | :---: | :--- |
| operation | Input | Movement operation object. The L0C Buffer to Global Memory movement uses `copy_l0c_to_gm{}`. |
| trait | Input | Movement static trait object. The L0C Buffer to Global Memory movement uses `l0c_to_gm_trait_default{}` by default. |



### copy_l0c_to_gm Description

`copy_l0c_to_gm` identifies the L0C Buffer-to-Global Memory movement path. Only its public members are listed:

```cpp
struct copy_l0c_to_gm {
    template <typename Trait, const Trait& trait, typename... Args>
    __aicore__ inline static void copy(const Args&... args);
};
```

The `copy` static member function receives the Trait and the movement parameters, and dispatches the L0C Buffer-to-Global Memory movement.

### l0c_to_gm_trait Description

`l0c_to_gm_trait` configures the static characteristics of L0C Buffer to Global Memory movement.

```cpp
enum class round_mode : uint8_t { default_round = 0, hybrid = 1 };

struct l0c_to_gm_trait {
    __aicore__ constexpr l0c_to_gm_trait() = default;
    __aicore__ constexpr l0c_to_gm_trait(asc::te::round_mode round_mode,
        bool enable_relu, bool enable_channel_split);

    asc::te::round_mode round_mode = asc::te::round_mode::default_round;
    bool enable_relu = false;
    bool enable_channel_split = false;
};
```

**Table 4**  `l0c_to_gm_trait` member description

|Member|Default Value|Description|
|--------|--------|--------|
|round_mode|`round_mode::default_round`|Rounding mode. `round_mode::hybrid` is supported only in the quantized output scenario where the source type is `float` and the destination type is `hifloat8_t`.|
|enable_relu|`false`|Whether to enable real-time ReLU.|
|enable_channel_split|`false`|Whether to enable output data channel split. Only the NZ format scenario where both the source type and the destination type are `float` is supported.|

Example of using a custom trait:

```cpp
constexpr l0c_to_gm_trait l0c_to_gm_trait_value = {
    round_mode::default_round,
    false,
    true
};

struct copy_l0c_to_gm_trait_custom {
    using trait_type = l0c_to_gm_trait;
    static constexpr const trait_type value = l0c_to_gm_trait_value;
};
```

`l0c_to_gm_trait_default` provides the default trait type and default value for this path:

```cpp
struct l0c_to_gm_trait_default {
    using trait_type = l0c_to_gm_trait;
    static constexpr const trait_type value = {};
};
```

### l0c_to_gm_params Description

`l0c_to_gm_params` is used to configure runtime movement parameters.

```cpp
enum class unit_flag_mode : uint8_t { disable = 0, enable_keep = 2, enable_update = 3 };

struct l0c_to_gm_params {
    __aicore__ constexpr l0c_to_gm_params() = default;
    __aicore__ constexpr l0c_to_gm_params(unit_flag_mode unit_flag) : unit_flag(unit_flag) {}

    unit_flag_mode unit_flag = unit_flag_mode::disable;
};
```

**Table 5**  `l0c_to_gm_params` member description

|Member|Default Value|Description|
|--------|--------|--------|
|unit_flag|`unit_flag_mode::disable`|Controls fine-grained parallelism between the mmad instruction and the Fixpipe instruction. `unit_flag_mode::disable` means it is not enabled, `unit_flag_mode::enable_keep` means it is enabled and the unit flag is not reset after execution, and `unit_flag_mode::enable_update` means it is enabled and the unit flag is reset after execution.|

## Data Type

The L0C Buffer to Global Memory movement automatically selects the quantization mode based on whether a quantization parameter is passed in.

**Table 6**  Data type description

|Source Tensor Type (L0C Buffer)|Destination Tensor Type (Global Memory)|Call Form|Description|
|--------|--------|--------|--------|
|`int32_t`|`int8_t`, `uint8_t`|`copy(atom, dst, src, quant)`|Scalar or tensor quantized output.|
|`float`|`int8_t`, `uint8_t`|`copy(atom, dst, src, quant)`|Scalar or tensor quantized output.|
|`float`|`fp8_e4m3fn_t`, `hifloat8_t`|`copy(atom, dst, src, quant)`|Scalar or tensor quantized output. `hifloat8_t` supports `round_mode::default_round` and `round_mode::hybrid`.|
|`int32_t`|`half`, `bfloat16_t`|`copy(atom, dst, src, quant)`|Scalar or tensor quantized output.|
|`float`|`half`|`copy(atom, dst, src)`|Direct conversion output.|
|`float`|`bfloat16_t`|`copy(atom, dst, src)`|Direct conversion output.|
|`float`|`half`, `bfloat16_t`|`copy(atom, dst, src, quant)`|Scalar or tensor quantized output.|
|`int32_t`|`int32_t`|`copy(atom, dst, src)`|Non-quantized output.|
|`float`|`float`|`copy(atom, dst, src)`|Non-quantized output.|
|`float`|`float`|`copy(atom, dst, src, quant)`|Scalar or tensor quantized output.|

When `quant` is not passed in, the API selects the non-quantized or direct conversion mode; when `uint64_t` is passed in, it selects the scalar quantization mode; and when a tensor is passed in, it selects the tensor quantization mode.

## Return Value

`copy` has no return value. `make_copy` returns a `copy_atom` object.

## Constraints

- The quantization and ReLU parameters cannot be inf/nan or denormalized numbers.
- The source cube is in `NZ` format, with an address requirement of 64-byte alignment. When the destination cube is in `DN` or `ND` format, the address requirement is 1-byte alignment; when it is in `NZ` format, the address requirement is 32-byte alignment.
- When the destination cube is output in `NZ` format, the size in the N direction must be a multiple of 16; for the `b8` type channel merge scenario, it must be a multiple of 32; and for the `float` type with `enable_channel_split` channel split enabled, it must be a multiple of 8.
- The tensor quantization parameter tensor should be located in the L1 Buffer, with an element type of `uint64_t` and an address requirement of 32-byte alignment. See details in [real-time quantization](../cube_store_key_features/quant_pre.md).
- When `unit_flag` is enabled, `mmad` must be enabled at the same time.
- When `round_mode::hybrid` is used, the source type must be `float` and the destination type must be `hifloat8_t`.
- `enable_channel_split` takes effect only when both the source type and the destination type are `float` and the destination format is `NZ`. See details in [F32 Channel Split](../cube_store_key_features/f32_channel_split.md).
- The channel merge feature is automatically enabled by hardware and cannot be configured through parameters. See [Int8 Channel Merge](../cube_store_key_features/int8_channel_merge.md) for details.
- The convolution output path from `NZ` to `NC1HWC0`, `NHWC`, or `NCHW` currently supports only N = 1. The path from `NZ` to `NHWC` or `NCHW` supports only the call form that does not pass in `quant`.
- In the convolution output path, the logical shape of the source `NZ` tensor must be `(H * W, C)`. When the destination tensor is `NC1HWC0`, `C = C1 * C0` must be satisfied.

## Key Features

The movement from L0C Buffer to Global Memory involves key features such as [real-time quantization](../cube_store_key_features/quant_pre.md), [inline Relu](../cube_store_key_features/relu_pre.md), [F32 Channel Split](../cube_store_key_features/f32_channel_split.md), [Int8 Channel Merge](../cube_store_key_features/int8_channel_merge.md), and [batch movement](../cube_store_key_features/batch_copy.md).

## Example

### Convolution Output

The following example moves the NZ cube of shape `(H * W, C)` in the L0C Buffer to Global Memory and converts it to the NHWC format. Replace the destination layout with `nchw_layout_ptn` or `nc1hwc0_layout_ptn` to output in the NCHW or NC1HWC0 format, respectively.

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

constexpr uint32_t h = 8;
constexpr uint32_t w = 8;
constexpr uint32_t c = 16;

__aicore__ inline void copy_conv_output_l0c_to_gm(__gm__ float* gm_addr)
{
    __cc__ float l0c_buf[h * w * c];

    auto l0c = make_tensor(
        make_mem_ptr(l0c_buf),
        make_frame_layout<nz_layout_ptn, 16>(h * w, c));
    auto gm = make_tensor(
        make_mem_ptr(gm_addr), make_frame_layout<nhwc_layout_ptn>(1, h, w, c));

    auto copy_l0c_to_gm_atom = make_copy(copy_l0c_to_gm{}, l0c_to_gm_trait_default{});
    copy(copy_l0c_to_gm_atom, gm, l0c);
}
```

### Non-quantized Output

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void copy_l0c_to_gm_example()
{
    auto l0c = make_tensor(make_mem_ptr(l0c_addr), make_frame_layout<nz_layout_ptn, 16>(m, n));
    auto gm = make_tensor(make_mem_ptr(gm_addr), make_frame_layout<nd_ext_layout_ptn>(m, n));

    l0c_to_gm_params params;
    auto atom = make_copy(copy_l0c_to_gm{}).with(params);
    copy(atom, gm, l0c);
}
```

### Scalar Quantized Output

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void copy_l0c_to_gm_example()
{
    auto l0c = make_tensor(make_mem_ptr(l0c_addr), make_frame_layout<nz_layout_ptn, 16>(m, n));
    auto gm = make_tensor(make_mem_ptr(gm_addr), make_frame_layout<nd_ext_layout_ptn>(m, n));

    uint64_t quant = 1;
    auto atom = make_copy(copy_l0c_to_gm{}).with(l0c_to_gm_params{});
    copy(atom, gm, l0c, quant);
}
```

### Tensor Quantized Output

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void copy_l0c_to_gm_example()
{
    auto l0c = make_tensor(make_mem_ptr(l0c_addr), make_frame_layout<nz_layout_ptn, 16>(m, n));
    auto gm = make_tensor(make_mem_ptr(gm_addr), make_frame_layout<nd_ext_layout_ptn>(m, n));
    auto quant = make_tensor(make_mem_ptr(l1_quant_addr), make_frame_layout<nd_ext_layout_ptn>(1, n));

    auto atom = make_copy(copy_l0c_to_gm{}).with(l0c_to_gm_params{});
    copy(atom, gm, l0c, quant);
}
```

### Batch Mode Output

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__aicore__ inline void copy_batch_l0c_to_gm(__gm__ half* gm_addr)
{
    constexpr uint32_t src_batch = 3;
    constexpr uint32_t dst_batch = 9;
    constexpr uint32_t m = 32;
    constexpr uint32_t n = 64;

    __cc__ float l0c_buf[src_batch * m * n];

    auto l0c = make_tensor(make_mem_ptr(l0c_buf), make_frame_layout<nz_layout_ptn>(src_batch, m, n));
    auto gm = make_tensor(make_mem_ptr(gm_addr), make_frame_layout<nd_ext_layout_ptn>(dst_batch, m, n));

    auto atom = make_copy(copy_l0c_to_gm{}).with(l0c_to_gm_params{});
    for (uint32_t i = 0; i < dst_batch / src_batch; ++i) {
        auto gm_slice = gm.slice(make_coord(i * src_batch, make_coord(0, 0)), make_shape(src_batch, make_shape(m, n)));
        copy(atom, gm_slice, l0c);
    }
}
```
