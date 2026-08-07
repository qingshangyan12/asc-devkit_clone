# asc_loadunalign_pre

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

头文件路径为：`"c_api/reg_compute/reg_load.h"`。

将Unified Buffer（UB）中起始地址任意字节对齐的数据搬入`vector_load_unalign`临时寄存器，作为非对齐搬入的预处理阶段。本接口需要与[asc_loadunalign](asc_loadunalign.md)或[asc_loadunalign_postupdate](asc_loadunalign_postupdate.md)配合使用。本接口提供两种功能模式：

- **无偏移预处理搬入**：从`src`起始地址搬入数据。
- **带地址寄存器偏移预处理搬入**：通过`addr_reg`类型的地址寄存器偏移指定起始位置开始搬入数据。需要与[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)配合使用。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 无偏移预处理搬入

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ <dtype>* src)
```

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ float* src)
```

### 带地址寄存器偏移预处理搬入

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ <dtype>* src,
                                                addr_reg offset)
```

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

```c
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst,
                                                __ubuf__ float* src,
                                                addr_reg offset)
```

## 参数说明

**表1** 参数说明（无偏移预处理搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的`vector_load_unalign`临时寄存器，写入单个VL长度的对齐32字节块数据，作为后续非对齐加载拼接接口的输入源。 |
| src | 输入 | 源UB地址。起始地址只需1字节对齐（本接口用于非对齐加载场景），但须落在UB地址范围内。 |

**表2** 参数说明（带地址寄存器偏移预处理搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的`vector_load_unalign`临时寄存器，写入单个VL长度的对齐32字节块数据，作为后续非对齐加载拼接接口的输入源。 |
| src | 输入 | 源UB地址。起始地址只需1字节对齐（本接口用于非对齐加载场景），但须落在UB地址范围内。 |
| offset | 输入 | 地址寄存器偏移，类型为`addr_reg`，由[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)预先构造。相对`src`起始地址的偏移，单位为元素个数（非字节），底层按dtype字节大小把AR中的元素偏移换算为字节偏移。偏移后实际访问地址须落在UB地址范围内。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为UB地址，目的操作数为`vector_load_unalign`临时寄存器。
- 本接口无对应底层指令。
- 实际访问地址在内部向低对齐到32字节边界后从UB读取数据，传入的`src`（或`src + offset * sizeof(dtype)`）起始地址无需32字节对齐，对齐由接口内部完成。
- 单次搬入的数据量为一个矢量长度（VL），按dtype字节大小换算为元素个数；VL由当前矢量计算配置决定。
- UB容量上限：UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。src偏移+单次搬入数据量不可超过实际可用容量，否则触发读越界异常。
- 如果需要执行多条本接口指令，且源地址存在重叠，需要插入内存屏障同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)（如`asc_mem_bar(VV_ALL)`或`asc_mem_bar(VST_VLD)`，按实际生产-消费关系选择合适的`mem_type`），保证多个指令串行化，防止出现异常数据。

### 带地址寄存器偏移预处理搬入约束

- offset必须由[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)预先构造，传入其他类型的地址寄存器会触发异常。
- offset取值需使`src + offset * sizeof(dtype)`落在UB地址范围内，否则会报错。

## 调用示例

```cpp
vector_load_unalign dst;
__ubuf__ int8_t* src;
asc_loadunalign_pre(dst, src);
```
