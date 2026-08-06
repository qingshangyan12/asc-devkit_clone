# asc_unsqueeze

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

将dst中数据根据mask进行解压缩。解压缩方式：dst中第0个元素置为0，dst中的第i个元素等于mask中从第0个到第(i-1)个元素中1的数量。mask最高位被忽略不参与统计。
具体算法如图1所示，dst的首位为0，后续mask[i]对应mask值为1时，dst[i]的值为dst[i-1] + 1；mask[i]对应mask值为0时，dst[i]的值为dst[i-1]。

**图1** unsqueeze流程

![unsqueeze流程](../../figures/capi_unsqueeze.png)

## 函数原型

```cpp
__simd_callee__ inline void asc_unsqueeze(vector_<dtype>& dst,
                                          vector_bool mask)
```

支持的数据类型：int8_t、uint8_t、int16_t、uint16_t、int32_t、uint32_t。

- 典型示例

```cpp
// 示例：对int8_t矢量数据寄存器执行解压缩
__simd_callee__ inline void asc_unsqueeze(vector_int8_t& dst,
                                          vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输入/输出 | 源操作数和目的操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数（掩码寄存器），用于提供dst解压缩信息。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- **本接口仅在AIV上生效，非AIV调用直接返回。**
- dst的dtype必须为`uint8_t`/`int8_t`/`uint16_t`/`int16_t`/`uint32_t`/`int32_t`之一（6种整数dtype），不支持浮点dtype；mask必须为`vector_bool`掩码寄存器，类型不匹配触发数据类型异常。
- mask为`vector_bool`类型掩码寄存器，需通过配套的掩码设置接口预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致前缀计数结果错误。
- mask比特位按元素大小步进过滤：每个元素占用`sizeof(dst dtype)`个连续比特位，元素`i`的有效mask比特起始于位置`i × sizeof(dst dtype)`。如`uint16_t`（2字节）元素`i`的mask比特位于`i × 2`位置，`int32_t`（4字节）元素`i`的mask比特位于`i × 4`位置。
- 参与计算的元素个数由矢量长度（VL）决定，元素个数 = VL ÷ sizeof(dst dtype)。一个矢量数据寄存器共256字节，1字节dtype（`uint8_t`/`int8_t`）为256个元素，2字节dtype（`uint16_t`/`int16_t`）为128个元素，4字节dtype（`uint32_t`/`int32_t`）为64个元素。
- dst写入值为非负整数计数，dtype选`uint8_t`/`uint16_t`/`uint32_t`时计数直接表达；选`int8_t`/`int16_t`/`int32_t`时计数以有符号整数表达，因计数非负，实际占用有符号dtype的非负区间，不会出现负值。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst与mask为矢量数据寄存器/掩码寄存器，不涉及UB地址，无UB容量与地址对齐约束。

## 调用示例

```cpp
__simd_vf__ inline void unsqueeze_vf(__ubuf__ int8_t* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_int8_t dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b8(count);
        asc_unsqueeze(dst, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
