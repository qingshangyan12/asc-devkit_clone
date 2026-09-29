# asc_loadalign（对齐搬入，非连续模式，NPU架构版本3510）

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

从Unified Buffer（UB）中32字节对齐的起始地址读取数据，单条指令按配置的步长非连续搬入8个DataBlock，每个DataBlock的数据量为32字节，并将结果写入目的矢量数据寄存器。搬运过程中数据格式和内容保持不变，源地址由用户更新。

通过函数返回值返回矢量数据寄存器时，请使用[asc_loadalign_datablock_strided（对齐搬入，非连续模式，NPU架构版本3510）](asc_loadalign_datablock_strided.md)。连续对齐搬入请使用[asc_loadalign（对齐搬入，连续模式，NPU架构版本3510）](asc_loadalign_continuous.md)。

**图1** 连续搬入矢量数据寄存器

![](../../figures/capi_loadalign_norm.png)

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

### 非连续对齐搬入模式

```c
__simd_callee__ inline void asc_loadalign(vector_<dtype>& dst,
                                          __ubuf__ <dtype>* src,
                                          uint16_t block_stride,
                                          uint16_t repeat_stride,
                                          vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。当dtype为`int4b_t`时，dst的实际类型为`vector_int4x2_t`。

#### 函数原型典型示例

```c
// 示例：float类型。
__simd_callee__ inline void asc_loadalign(vector_float& dst,
                                          __ubuf__ float* src,
                                          uint16_t block_stride,
                                          uint16_t repeat_stride,
                                          vector_bool mask)
```

## 参数说明

### 非连续对齐搬入模式

**表4** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype必须与`src`一致，搬入VL长度数据。 |
| src | 输入 | 源UB地址，实际读取地址必须按32字节对齐。 |
| block_stride | 输入 | 源操作数相邻`DataBlock`之间起始地址的步长，单位为32字节。 |
| repeat_stride | 输入 | 本次搬入的起始读取地址相对`src`的偏移，单位为32字节。实际起始读取地址为`src`偏移`repeat_stride × 32`字节。 |
| mask | 输入 | 掩码寄存器，用于指示在计算过程中哪些元素参与计算。该接口以`DataBlock`为数据搬运单元。<br>&bull; 当`DataBlock`中的任意一个元素被`mask`筛选成有效元素时，该`DataBlock`中所有数据都会搬入至矢量数据寄存器。<br>&bull; 当`DataBlock`中所有元素都被`mask`筛选成无效元素时，该`DataBlock`中的数据不会搬入到矢量数据寄存器，对应位置的元素设置为0，即使UB越界也不会报错。 |

矢量数据寄存器和作为执行掩码的掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 实际读取地址必须按32字节对齐，且实际读取范围必须在UB地址空间内且不越界。当某个`DataBlock`的`mask`包含的元素全部无效时，该`DataBlock`即使越界也不会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 非连续对齐搬入模式

- 当dtype为`int64_t`时，由于掩码创建接口不支持b64模式，需要先通过[asc_create_mask_b32](../reg_mask/asc_create_mask.md)生成b32掩码，再通过[asc_unpack_lower](../reg_permute_sel/asc_unpack.md)将其展开为适用于b64数据的掩码后传入本接口。展开后的掩码以连续8个bit为一组，仅每组最低位的bit有效，用于控制对应的一个b64元素。
    ```c
    vector_int64_t src_reg;
    vector_bool mask_b32 = asc_create_mask_b32(PAT_ALL);
    vector_bool mask_b64;
    asc_unpack_lower(mask_b64, mask_b32);
    addr_reg load_offset;
    addr_reg store_offset;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        load_offset = asc_update_addr_reg_b32(elements_vl * 2);
        // 搬出到UB后数据类型为B32，偏移不需要*2
        store_offset = asc_update_addr_reg_b32(elements_vl);
        asc_loadalign(src_reg, input, load_offset);
        asc_storealign_pack(output, src_reg, store_offset, mask_b64);
    }
    ```

## 调用示例

以下示例从32字节对齐的UB地址非连续搬入8个DataBlock。相邻DataBlock的起始地址间隔为32字节，本次搬入相对源起始地址不偏移。

```c
__simd_vf__ inline void loadalign_noncontinuous(
    __ubuf__ uint8_t* dst, __ubuf__ uint8_t* src)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_uint8_t data;
    asc_loadalign(data, src, 1, 0, mask);
    asc_storealign(dst, data, mask);
}
```
