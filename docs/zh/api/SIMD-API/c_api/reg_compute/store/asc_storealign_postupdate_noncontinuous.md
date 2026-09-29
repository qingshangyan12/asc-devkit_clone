# asc_storealign_postupdate（对齐搬出，非连续模式，NPU架构版本3510）

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

头文件路径为：`"c_api/reg_compute/store/storealign.h"`。

将矢量数据寄存器中的数据非连续对齐搬出到Unified Buffer（UB）。单条指令搬运8个DataBlock，每个DataBlock的数据量为32字节，通过参数配置相邻DataBlock的步长。`mask`用于指示参与搬出的元素，掩码为1的元素写入目的地址，掩码为0的元素保留目的地址原数据。搬出完成后基于立即数自动更新目的地址指针，单位为32字节。

连续对齐Post Update搬出请使用[asc_storealign_postupdate（对齐搬出，连续模式，NPU架构版本3510）](asc_storealign_postupdate_continuous.md)。

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

### 矢量数据寄存器非连续对齐搬出模式

```c
// 占位符形式
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ <dtype>*& dst,
                                                       vector_<dtype> src,
                                                       uint16_t block_stride,
                                                       uint16_t repeat_stride,
                                                       vector_bool mask)
```

#### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ int8_t*& dst,
                                                       vector_int8_t src,
                                                       uint16_t block_stride,
                                                       uint16_t repeat_stride,
                                                       vector_bool mask)
```

## 参数说明

### 矢量数据寄存器非连续对齐搬出模式

**表 2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输入/输出 | 目的操作数（矢量）的起始地址，按指针引用传入（`__ubuf__ <dtype>*&`）。dtype须与src一致，起始地址需32字节对齐。搬出完成后该指针由硬件自动更新。 |
| src | 输入 | 源操作数（矢量数据寄存器）。dtype须与dst一致。 |
| block_stride | 输入 | 目的操作数相邻数据块之间起始地址的步长，类型为`uint16_t`，单位为32字节。 |
| repeat_stride | 输入 | 目的地址Post Update后更新的偏移量，类型为`uint16_t`，单位为32字节。 |
| mask | 输入 | 源操作数掩码（掩码寄存器），用于指示参与搬出的元素。对应位置为1时参与搬出，为0时不参与搬出。 |

矢量数据寄存器和作为执行掩码的掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

### 通用约束

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- dst起始地址需32字节对齐，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈+2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。目的操作数的实际访问地址和Post Update后的地址不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

### 矢量数据寄存器搬出场景

- mask比特位为0的位置采用保持模式：dst对应位置保持原值不变，不写入src数据。
- 步长参数模式下，通过block_stride和repeat_stride确定的实际访问地址需落在UB地址范围内，且Post Update后的dst新地址仍需32字节对齐，否则会报错。

## 调用示例

以下示例将矢量数据寄存器中的数据非连续搬出到8个相邻DataBlock，并在搬出后将目的地址更新8个DataBlock。

```c
__simd_vf__ inline void storealign_postupdate_noncontinuous(
    __ubuf__ uint8_t* dst, __ubuf__ uint8_t* src)
{
    vector_bool mask = asc_create_mask_b8(PAT_ALL);
    vector_uint8_t data = asc_loadalign(src);
    __ubuf__ uint8_t* dst_addr = dst;
    asc_storealign_postupdate(dst_addr, data, 1, 8, mask);
}
```
