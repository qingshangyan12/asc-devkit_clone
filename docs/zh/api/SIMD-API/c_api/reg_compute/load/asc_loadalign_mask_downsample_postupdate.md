# asc_loadalign_mask_downsample_postupdate

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/reg_compute/load/loadalign.h"`。

从Unified Buffer（UB）中32字节对齐的起始地址读取VL/4（64字节）长度数据，保留偶数下标bit，将得到的VL/8长度数据写入目的掩码寄存器。接口采用Post Update模式，搬运完成后按指定的字节偏移自动更新源地址指针。

**图1** 下采样搬入掩码寄存器

![](../../figures/capi_loadalign_mask_downsample.png)

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

```c
__simd_callee__ inline void asc_loadalign_downsample_postupdate(vector_bool& dst,
                                                                __ubuf__ uint32_t*& src,
                                                                int32_t offset)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的掩码寄存器，类型为`vector_bool`，搬入VL/8长度数据。 |
| src | 输入/输出 | 源UB地址，当前实际读取地址必须按32字节对齐。搬运完成后，指针指向更新后的地址。 |
| offset | 输入 | Post Update步长，单位为字节。接口从当前`src`地址完成搬入后，`src`增加`offset`字节。 |

掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 当前实际读取地址及Post Update更新后的下一次实际读取地址必须按32字节对齐，且实际读取范围必须在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化。UB地址偏移后不可超过实际可用容量，否则会报错。详细说明请参见[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

以下示例从UB读取64字节掩码数据，进行2倍下采样，并在搬入后将源地址更新64字节。

```c
__simd_vf__ inline void load_mask_downsample_postupdate(
    __ubuf__ uint32_t* output, __ubuf__ uint32_t* input)
{
    vector_bool mask;
    __ubuf__ uint32_t* src = input;
    // 读取64字节数据并下采样为32字节掩码，同时将源地址更新64字节。
    asc_loadalign_downsample_postupdate(mask, src, 64);
    asc_storealign(output, mask);
}
```
