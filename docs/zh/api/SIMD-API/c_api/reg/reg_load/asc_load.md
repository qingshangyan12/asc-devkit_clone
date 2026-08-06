# asc_load

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

头文件路径：`"c_api/reg_compute/reg_load.h"`。

reg计算数据搬运接口，支持从UB非32字节对齐的源地址src搬运至矢量数据寄存器，搬运量为VL（256字节）。

连续搬运时，用户需手动更新src地址。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### 模板原型

```c
__simd_callee__ inline void asc_load(vector_<dtype>& dst,
                                     __ubuf__ <dtype>* src)
```

### dtype支持数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t`、`half`、`float`、`int64_t`、`bfloat16_t`、`fp8_e4m3fn_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e8m0_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`int4b_t`

### 典型示例

```c
// 示例：float类型加载
__simd_callee__ inline void asc_load(vector_float& dst,
                                     __ubuf__ float* src)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量）的起始地址。地址可以非32字节对齐，但必须在UB地址空间内，且src起始地址+256字节不可超过UB可用容量上限。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../reg_data_types/data_type_definition.md)。

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，dst为矢量数据寄存器、src为UB地址，UB地址空间外的指针不可作为src传入。
- UB容量上限：UB整体容量为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈+2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB~128KB作Data Cache，可用容量进一步减少）。src偏移+256字节不可超过实际可用容量，否则会报错。
- 搬运量固定为VL字节（默认256字节），由矢量长度配置决定，不可由本接口参数调整；dst写入元素个数=VL/sizeof(dtype)。
- dst与src的dtype必须完全一致（同名重载按dtype选择），dtype不一致触发异常。
- 如果需要执行多条本接口指令，且源地址存在重叠，需通过同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)保证串行化，防止读到非预期数据。

## 调用示例

```cpp
vector_half dst;
__ubuf__ half* src = (__ubuf__ half*)asc_get_phy_buf_addr(0);
asc_load(dst, src);
```
