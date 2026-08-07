# asc_float2bfloat16

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"c_api/reg_compute/reg_convert.h"`。

将float类型数据转换成bfloat16_t类型，并支持多种舍入模式与饱和模式。

关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式与饱和模式](rounding_mode.md)。

由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个VL大小的数据分为两部分，根据不同接口选择数据写入索引为奇数的位置或偶数的位置。

```python
# src: float[64]       (vector_float, 256B = 64个float元素)
# dst: bfloat16[128]   (vector_bfloat16_t, 256B = 128个bfloat16元素)
# mask: bool[64]        (按src的b32位宽)
#
# 窄化转换(1:2): 64个float全部读取, 转换后写入dst的偶数或奇数索引位置
# asc_float2bfloat16_<round_mode>    结果写入dst偶数索引 dst[0,2,...,126], 奇数位置置零
# asc_float2bfloat16_<round_mode>_v2 结果写入dst奇数索引 dst[1,3,...,127], 偶数位置置零
# <round_mode>为舍入模式(rd/rn/rna/ru/rz), _sat后缀表示饱和模式

def asc_float2bfloat16_rn(dst, src, mask):
    for i in range(64):
        if mask[i]:
            dst[2 * i] = bfloat16(src[i])   # float -> bfloat16, RINT舍入, 写入偶数索引
        else:
            dst[2 * i] = 0
    for i in range(64):
        dst[2 * i + 1] = 0                  # 奇数索引置零

def asc_float2bfloat16_rn_v2(dst, src, mask):
    for i in range(64):
        dst[2 * i] = 0                      # 偶数索引置零
        if mask[i]:
            dst[2 * i + 1] = bfloat16(src[i])  # float -> bfloat16, RINT舍入, 写入奇数索引
        else:
            dst[2 * i + 1] = 0
```

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_float2bfloat16_<round_mode><sat_suffix><v2_suffix>(vector_bfloat16_t& dst,
                                                                                   vector_float src,
                                                                                   vector_bool mask)
```

其中：

- `<round_mode>`支持`rd`（FLOOR）、`rn`（RINT）、`rna`（ROUND）、`ru`（CEIL）和`rz`（TRUNC）。
- `<sat_suffix>`为空时表示非饱和模式，取值为`_sat`时表示饱和模式。
- `<v2_suffix>`为空时，结果写入dst的偶数索引位置；取值为`_v2`时，结果写入dst的奇数索引位置。

src支持的数据类型为`float`，dst支持的数据类型为`bfloat16_t`

### 典型示例

```c
// RINT舍入模式，非饱和模式，数据写入索引为偶数的位置
__simd_callee__ inline void asc_float2bfloat16_rn(vector_bfloat16_t& dst,
                                                  vector_float src,
                                                  vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述                                                                                                             |
| ------ | --------- | ---------------------------------------------------------------------------------------------------------------- |
| dst    | 输出      | 目的操作数（矢量数据寄存器）。                                                                                   |
| src    | 输入      | 源操作数（矢量数据寄存器）。                                                                                     |
| mask   | 输入      | 掩码寄存器，用于控制各元素是否参与计算。mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- src与dst的数据类型需要与函数原型匹配。
- 掩码位为0时，dst对应元素置0。
- 结果写入dst的奇数索引位置时，偶数索引位置置0；结果写入偶数索引位置时，奇数索引位置置0。
- 使用饱和或非饱和模式时，需要配置ctrl寄存器，详细说明请参见[asc_set_ctrl](../../sys_var/asc_set_ctrl.md)。

## 调用示例

```cpp
vector_bfloat16_t dst;
vector_float src;
vector_bool mask;
mask = asc_create_mask_b32(PAT_ALL);
asc_float2bfloat16_rd(dst, src, mask);    // FLOOR舍入模式，不饱和模式，将src转换成vector_bfloat16_t类型并写入dst的偶数索引位置
asc_float2bfloat16_rd_sat(dst, src, mask);    // FLOOR舍入模式，饱和模式，将src转换成vector_bfloat16_t类型并写入dst的偶数索引位置
asc_float2bfloat16_rd_v2(dst, src, mask);    // FLOOR舍入模式，不饱和模式，将src转换成vector_bfloat16_t类型并写入dst的奇数索引位置
asc_float2bfloat16_rd_sat_v2(dst, src, mask);    // FLOOR舍入模式，饱和模式，将src转换成vector_bfloat16_t类型并写入dst的奇数索引位置
```
