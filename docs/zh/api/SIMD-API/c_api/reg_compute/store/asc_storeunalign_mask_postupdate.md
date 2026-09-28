# asc_storeunalign_mask_postupdate

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

头文件路径为：`"c_api/reg_compute/store/storeunalign.h"`。

将掩码寄存器中的数据搬出到Unified Buffer（UB），并自动更新目的地址指针：

- 当目的地址类型为`uint16_t`时，从每2bit数据中提取最低有效位（LSB），将掩码寄存器中的32字节数据打包为16字节写入UB，目的地址更新16字节。
- 当目的地址类型为`uint32_t`时，从每4bit数据中提取最低有效位（LSB），将掩码寄存器中的32字节数据打包为8字节写入UB，目的地址更新8字节。

本接口使用非对齐寄存器暂存不能立即写出的尾块，需配合[asc_storeunalign_post](asc_storeunalign_post.md)的立即数偏移搬出模式完成收尾。

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

```c
__simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ <dtype>*& dst,
                                                        vector_store_unalign& unalign_reg,
                                                        vector_bool src)
```

### dtype支持数据类型

`dtype`支持的数据类型为`uint16_t`、`uint32_t`。

### 函数原型典型示例

```c
__simd_callee__ inline void asc_storeunalign_postupdate(__ubuf__ uint16_t*& dst,
                                                        vector_store_unalign& unalign_reg,
                                                        vector_bool src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输入/输出 | 目的UB地址。`uint16_t`类型目的地址必须按16字节对齐，`uint32_t`类型目的地址必须按8字节对齐。搬运完成后，指针指向本次搬出的结束地址。 |
| unalign_reg | 输入/输出 | 非对齐寄存器，类型为`vector_store_unalign&`，长度为32字节。连续调用和后处理时必须复用同一个寄存器。 |
| src | 输入 | 源掩码寄存器，类型为`vector_bool`。 |

掩码寄存器和非对齐寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- `uint16_t`类型目的地址必须按16字节对齐，`uint32_t`类型目的地址必须按8字节对齐。
- 搬出完成后，必须调用[asc_storeunalign_post](asc_storeunalign_post.md)的立即数偏移搬出模式收尾，并复用同一个`unalign_reg`。
- 目的操作数的写入范围和更新后的地址必须在UB地址空间内且不越界，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化。写入范围和更新后的地址不可超过实际可用容量，否则会报错。详细说明请参见[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../reg_sync/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。

## 调用示例

```c
__simd_vf__ inline void storeunalign_mask_postupdate(
    __ubuf__ uint16_t* output, __ubuf__ uint32_t* input)
{
    vector_bool mask = asc_loadalign_mask(input);
    vector_store_unalign unalign_reg;
    __ubuf__ uint16_t* dst = output;
    // 将掩码数据按b16格式打包搬出，并更新目的地址。
    asc_storeunalign_postupdate(dst, unalign_reg, mask);
    // 复用同一个非对齐寄存器写回尾块。
    asc_storeunalign_post(dst, unalign_reg, 0);
}
```
