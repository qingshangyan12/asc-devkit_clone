# asc_bfloat162half

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

将bfloat16_t类型转化为half类型，并支持多种舍入模式、饱和/非饱和模式。

参考伪代码如下：

```python
def asc_float2half_rn(dst, src, mask):
    for i in range(64):
        if mask[i]:
            dst[i] = half(src[i])   # float -> half, RINT舍入
        else:
            dst[i] = 0
```

## 函数原型

### 模板原型（占位符形式）

本接口为支持多种舍入模式、饱和模式组合的复合接口，模板原型如下：

```c
__simd_callee__ inline void asc_bfloat162half_<round_mode><sat_mode>(vector_half& dst,
                                                                     vector_bfloat16_t src,
                                                                     vector_bool mask)
```

### 模式说明

- `<round_mode>`支持`rd`（FLOOR）、`rn`（RINT）、`rna`（ROUND）、`ru`（CEIL）和`rz`（TRUNC）。
- `<sat_suffix>`为空时表示非饱和模式，取值为`_sat`时表示饱和模式。

关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式与饱和模式](../data_type_convert/rounding_mode.md)。

### 典型示例

```c
// RINT舍入模式，非饱和模式
__simd_callee__ inline void asc_float2half_rn(vector_half& dst,
                                              vector_bfloat16_t src,
                                              vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，src、dst为矢量数据寄存器，不涉及UB地址，无UB容量与地址对齐约束。
- src的dtype必须为`bfloat16_t`，dst的dtype必须为`half`，dtype固定不可混用，否则触发异常。
- mask为`vector_bool`类型掩码寄存器，需通过掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- 开启接口单指令饱和模式和非饱和模式时，需配置ctrl寄存器CTRL[60]为`1'b0`，ctrl寄存器的详细说明请参见[asc_set_ctrl.md](../../sys_var/asc_set_ctrl.md)。

## 调用示例

```cpp
__simd_vf__ inline void bfloat162half_vf(__ubuf__ half* dst_addr, __ubuf__ bfloat16_t* src_addr, int32_t one_repeat_size, uint16_t repeat_time)
{
    vector_bfloat16_t src;
    vector_half dst;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    for (uint16_t i = 0; i < repeat_time; ++i) {
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_bfloat162half_rn(dst, src, mask);
        asc_storealign_postupdate(dst_addr, dst, one_repeat_size, mask);
    }
}
```
