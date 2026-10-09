# asc_storealign（Mask对齐搬出，NPU架构版本3510）

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

将掩码寄存器中的32字节数据直接搬出到Unified Buffer（UB），搬运过程中数据格式和内容保持不变。本接口不支持配置额外的执行掩码，提供以下三种模式：

- **连续对齐搬出模式**：搬出到UB目的地址，由用户自行更新目的地址。
- **立即数偏移搬出模式**：搬出到相对目的起始地址偏移指定距离的位置，偏移单位为字节。
- **地址寄存器偏移搬出模式**：通过地址寄存器指定相对目的起始地址的偏移。需要与[asc_update_addr_reg](../reg_addr_reg/asc_update_addr_reg.md)配合使用。

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

### 连续对齐搬出模式

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_bool src)
```

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst,
                                           vector_bool src)
```

### 立即数偏移搬出模式

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_bool src,
                                           int32_t offset)
```

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst,
                                           vector_bool src,
                                           int32_t offset)
```

### 地址寄存器偏移搬出模式

```c
__simd_callee__ inline void asc_storealign(__ubuf__ <dtype>* dst,
                                           vector_bool src,
                                           addr_reg offset)
```

#### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign(__ubuf__ uint8_t* dst,
                                           vector_bool src,
                                           addr_reg offset)
```

### dtype支持数据类型

`dtype`支持的数据类型为`uint8_t`、`uint16_t`、`uint32_t`。

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的UB地址，类型为`__ubuf__ <dtype>*`，实际写入地址必须按32字节对齐。 |
| src | 输入 | 源掩码寄存器，类型为`vector_bool`，搬出32字节数据。 |
| offset | 输入 | 仅偏移搬出模式包含该参数。`int32_t`类型的偏移单位为字节；`addr_reg`类型的偏移由配套的地址寄存器构造接口预先构造。 |

掩码寄存器和地址寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 各功能模式下的实际写入地址必须按32字节对齐，且写入范围必须在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化。目的地址偏移后的写入范围不可超过实际可用容量，否则会报错。详细说明请参见[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 使用地址寄存器偏移搬出模式前，必须先调用`asc_update_addr_reg`接口生成地址寄存器。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

```c
__simd_vf__ inline void store_mask(__ubuf__ uint32_t* output,
                                   __ubuf__ uint32_t* input)
{
    vector_bool mask = asc_loadalign_mask(input);
    // 将掩码寄存器中的32字节数据直接搬出到UB。
    asc_storealign(output, mask);
}
```
