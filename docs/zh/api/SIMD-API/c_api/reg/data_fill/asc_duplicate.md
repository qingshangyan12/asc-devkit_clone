# asc_duplicate

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

将源操作数src的最低位元素（即下标为0的元素）广播到目的操作数dst中被mask筛选的位置。mask仅用于筛选dst中的元素位置，不影响从src中读取的元素；dst中未被mask筛选的位置置零。

## 函数原型

以下使用`<dtype>`作为数据类型占位符表示各重载的函数原型：

```c
__simd_callee__ inline void asc_duplicate(vector_<dtype>& dst,
                                          vector_<dtype> src,
                                          vector_bool mask)
```

其中，`<dtype>`可取`int8_t`、`uint8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数（矢量数据寄存器），保存广播结果。数据类型需要与src保持一致。 |
| src | 输入 | 源操作数（矢量数据寄存器），其最低位元素作为待广播的数据。数据类型需要与dst保持一致。 |
| mask | 输入 | 源操作数元素操作的有效指示（掩码寄存器）。mask筛选的元素在dst中填充为src的最低位元素，未筛选的元素在dst中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 本接口为Reg矢量API，只能在使用`__simd_vf__`标记的VF函数内调用，不支持在`__aicore__`函数中直接调用；计算在AIV上执行。
- dst和src的数据类型需要保持一致，仅支持[函数原型](#函数原型)中列出的数据类型。其他数据类型组合无法匹配函数重载，编译不通过。
- dst、src和mask均为寄存器，不涉及UB地址和地址对齐。
- 同一寄存器的数据依赖由硬件保序，无需额外插入同步指令。本接口与前后Reg数据搬运接口之间，如果不同寄存器访问同一UB地址且存在写后读或写后写依赖，需要调用[asc_mem_bar](../sync_control/asc_mem_bar.md)进行同步。
- 使用mask前，需要通过掩码设置或搬入接口完成初始化；未初始化的掩码寄存器内容不确定。
- mask仅筛选dst中写入广播值的位置，不筛选src中的元素。无论mask的最低位是否有效，待广播的数据均为src的最低位元素。
- mask未筛选的元素在dst中置零。

## 调用示例

```cpp
__simd_vf__ inline void duplicate_vf(__ubuf__ half* dst_addr, __ubuf__ half* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_half dst;
    vector_half src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_duplicate(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
