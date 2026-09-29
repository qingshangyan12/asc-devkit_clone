# asc_storealign_postupdate（Mask对齐搬出，NPU架构版本3510）

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

将掩码寄存器中的32字节数据连续对齐搬出到Unified Buffer（UB）。接口采用Post Update模式，搬运完成后按指定的字节偏移自动更新目的地址指针。本接口不支持配置额外的执行掩码。

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

```c
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ <dtype>*& dst,
                                                       vector_bool src,
                                                       int32_t offset)
```

### dtype支持数据类型

`dtype`支持的数据类型为`uint8_t`、`uint16_t`、`uint32_t`。

### 函数原型典型示例

```c
__simd_callee__ inline void asc_storealign_postupdate(__ubuf__ uint8_t*& dst,
                                                       vector_bool src,
                                                       int32_t offset)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输入/输出 | 目的UB地址，按指针引用传入，实际写入地址必须按32字节对齐。搬运完成后，指针指向更新后的地址。 |
| src | 输入 | 源掩码寄存器，类型为`vector_bool`，搬出32字节数据。 |
| offset | 输入 | Post Update步长，单位为字节。接口搬出数据后，`dst`增加`offset`字节。 |

掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 当前实际写入地址及Post Update更新后的下一次实际写入地址必须按32字节对齐，且写入范围和更新后的地址必须在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化。写入范围和Post Update更新后的地址不可超过实际可用容量，否则会报错。详细说明请参见[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

```c
__simd_vf__ inline void store_mask_postupdate(__ubuf__ uint32_t* output,
                                              __ubuf__ uint32_t* input)
{
    vector_bool mask = asc_loadalign_mask(input);
    __ubuf__ uint32_t* dst = output;
    // 搬出32字节掩码数据，并将目的地址更新32字节。
    asc_storealign_postupdate(dst, mask, 32);
}
```
