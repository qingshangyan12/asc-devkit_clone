# asc_bfloat162e2m1x2

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR&960DT系列产品：支持
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3系列产品：不支持
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2系列产品：不支持
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas训练系列产品：不支持
<!-- end id8 -->


## 功能说明

根据`mask`将`src`中参与计算的`bfloat16_t`类型元素转换为`fp4x2_e2m1_t`类型，结果写入`dst`或通过函数返回值返回。由于源操作数与目的操作数的类型位宽比为4:1，`dst`中的元素按索引分为四组，参数`dst_quarter_pos`从`dst`中离散选取每隔4个元素的索引位置写入转换结果，具体为索引0、4、8、…，索引1、5、9、…，索引2、6、10、…或索引3、7、11、…。以置零模式为例，伪代码如下：

```python
# 位置判断仅用于说明接口语义，实际位置参数为编译期常量。
# VL表示矢量数据寄存器位宽，取值256字节。
def asc_bfloat162e2m1x2_rn(dst, src, mask, dst_quarter_pos):
    # ASC_DISPERSE_FIRST_QUARTER、ASC_DISPERSE_SECOND_QUARTER、ASC_DISPERSE_THIRD_QUARTER和
    # ASC_DISPERSE_FOURTH_QUARTER分别对应offset取0、1、2、3
    offset = {ASC_DISPERSE_FIRST_QUARTER: 0, ASC_DISPERSE_SECOND_QUARTER: 1,
              ASC_DISPERSE_THIRD_QUARTER: 2, ASC_DISPERSE_FOURTH_QUARTER: 3}[dst_quarter_pos]
    for i in range(VL // 2):
        dst[4 * i] = 0
        dst[4 * i + 1] = 0
        dst[4 * i + 2] = 0
        dst[4 * i + 3] = 0
        if mask[i]:
            dst[4 * i + offset] = fp4x2_e2m1(src[i])  # bfloat16 -> fp4x2_e2m1_t，RINT舍入
```

合并模式下，上述伪代码中置0的元素保留`dst`中原值。

关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式与饱和模式](rounding_mode.md)。

本接口为Reg矢量计算接口，仅在AIV上生效。

## 函数原型

```c
// 通过引用参数输出结果（占位符形式）
__simd_callee__ inline void asc_bfloat162e2m1x2_<round_mode><merge_mode>(vector_fp4x2_e2m1_t& dst,
                                     vector_bfloat16_t src,
                                     vector_bool mask,
                                     std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)

// 通过函数返回值返回结果
__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_<round_mode>(vector_bfloat16_t src,
                                     vector_bool mask,
                                     std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)
```

**占位符说明如下：**
- `<round_mode>`表示支持的舍入模式，支持`rd`（FLOOR）、`rn`（RINT）、`rna`（ROUND）、`ru`（CEIL）和`rz`（TRUNC）。
- `<merge_mode>`表示掩码未选中元素的处理模式：为空时表示置零模式，`mask`未选中的元素和组内未写入结果的位置置0；取值为`_merge`时表示合并模式，`mask`未选中的元素和组内未写入结果的位置保留`dst`中原值。
<!-- npu="960" id9 -->
- 合并模式（`<merge_mode>`取值为`_merge`）仅在Ascend 960PR&960DT系列产品上支持。
<!-- end id9 -->
- `<quarter_pos>`支持`ASC_DISPERSE_FIRST_QUARTER`、`ASC_DISPERSE_SECOND_QUARTER`、`ASC_DISPERSE_THIRD_QUARTER`和`ASC_DISPERSE_FOURTH_QUARTER`。

### 函数原型典型示例

```c
// 通过引用参数输出结果
__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst,
    vector_bfloat16_t src,
    vector_bool mask,
    std::integral_constant<asc_position_quarter_mode,
                           asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_quarter_pos)

// 通过函数返回值返回结果
__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src,
    vector_bool mask,
    std::integral_constant<asc_position_quarter_mode,
                           asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_quarter_pos)
```

<!-- npu="960" id10 -->

```c
// RINT舍入模式，合并模式，数据写入每4个元素中第3个的位置，结果通过函数返回值返回
__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn_merge(vector_bfloat16_t src,
                                                                         vector_bool mask,
                                                                         std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
                                                                             dst_quarter_pos)
```

<!-- end id10 -->

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。合并模式下，未写入结果的位置保留`dst`原值。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |
| dst_quarter_pos | 输入 | 四分之一位置选择标签（编译期标签分发，通过编译期重载选择对应实现），用于选择目的操作数的写入地址，其他位置清零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

- 通过引用参数输出结果的函数原型无返回值。
- 通过函数返回值输出结果的函数原型返回计算结果，返回值类型与对应引用输出函数原型中`dst`参数的类型一致（去除引用）。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 位置选择标签参数仅能使用编译期常量，编译器据此在编译期分发至对应的重载。
- 位置选择标签选择目的操作数的写入地址，其他位置清零。
- 置零模式下，`mask`掩码位为0的元素和组内未写入结果的位置置0；合并模式下保留`dst`原值。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[__NPU_ARCH__](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

<!-- npu="950" id11 -->
以Ascend 950PR&950DT系列产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

<!-- end id11 -->

<!-- npu="960" id12 -->
以Ascend 960PR&960DT系列产品（对应NPU架构为`dav-9201`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-9201 && ./main
```

<!-- end id12 -->

```c
#include <cstdint>
#include <iostream>
#include <vector>
#include "acl/acl.h"
#include "c_api/asc_simd.h"

namespace {
constexpr uint32_t BUFFER_BYTES = 256;

__simd_vf__ inline void convert(__ubuf__ uint8_t* output, __ubuf__ uint8_t* input)
{
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    vector_fp4x2_e2m1_t dst;
    vector_bfloat16_t src;
    asc_loadalign(dst, reinterpret_cast<__ubuf__ fp4x2_e2m1_t*>(output));
    asc_loadalign(src, reinterpret_cast<__ubuf__ bfloat16_t*>(input));
    asc_bfloat162e2m1x2_rn(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
    asc_storealign(reinterpret_cast<__ubuf__ fp4x2_e2m1_t*>(output), dst, mask);
}

__global__ __vector__ void asc_bfloat162e2m1x2_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint8_t output_local[BUFFER_BYTES];
    __ubuf__ uint8_t input_local[BUFFER_BYTES];
    asc_copy_gm2ub_align(input_local, input, BUFFER_BYTES);
    asc_copy_gm2ub_align(output_local, input, BUFFER_BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    convert(output_local, input_local);
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(output, output_local, BUFFER_BYTES);
    asc_sync();
}
} // namespace

int main()
{
    std::vector<uint8_t> input(BUFFER_BYTES, 0);
    std::vector<uint8_t> output(BUFFER_BYTES, 0xff);
    const std::vector<uint8_t> golden(BUFFER_BYTES, 0);
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t* input_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BUFFER_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BUFFER_BYTES, input.data(), BUFFER_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_bfloat162e2m1x2_kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BUFFER_BYTES, output_device, BUFFER_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_bfloat162e2m1x2_rn passed." : "[Failed] asc_bfloat162e2m1x2_rn failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
