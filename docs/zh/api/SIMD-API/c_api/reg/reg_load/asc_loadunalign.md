# asc_loadunalign

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

将Unified Buffer（UB）中起始地址任意字节对齐的连续数据搬入矢量数据寄存器，搬入数据量为VL。本接口是非对齐搬入的第二阶段，需要先调用[asc_loadunalign_pre](asc_loadunalign_pre.md)初始化`src0`。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

```c
__simd_callee__ inline void asc_loadunalign(vector_<dtype>& dst,
                                            vector_load_unalign& src0,
                                            __ubuf__ <dtype>* src1)
```

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

```c
// 示例：int8_t类型非对齐搬入。
__simd_callee__ inline void asc_loadunalign(vector_int8_t& dst,
                                            vector_load_unalign& src0,
                                            __ubuf__ int8_t* src1)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器，写入单个VL长度的非对齐加载拼接结果。dtype由重载决定，须与src1一致。 |
| src0 | 输入 | `vector_load_unalign`临时寄存器，保存上一阶段加载的VL长度数据，须通过配套接口预先初始化为有效状态。 |
| src1 | 输入 | 源UB地址，本次搬入的相邻下一块VL长度数据。dtype须与dst一致。起始地址无32字节对齐要求（本接口用于非对齐加载场景），但须落在UB地址范围内。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

### 通用约束

- src1起始地址只需1字节对齐。
- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用，源操作数为UB地址，目的操作数为矢量数据寄存器。
- src1与dst的dtype必须完全一致，dtype不一致触发异常。
- src0必须为`vector_load_unalign`类型，且须通过配套接口预先初始化为有效状态；未初始化的src0内容不确定，会导致dst拼接结果错误。
- 本接口须在配套接口完成首个VL长度数据加载后调用，二者需成对使用以保证非对齐拼接语义正确。
- 本接口无对应底层指令。
- UB容量上限：UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。src1偏移+单次搬入数据量不可超过实际可用容量，否则会报错。
- 单次搬入的数据量为一个矢量长度（VL），按dtype字节大小换算为元素个数；VL由当前矢量计算配置决定。
- 如果需要执行多条本接口指令，且源地址存在重叠，需要插入内存屏障同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)（如`asc_mem_bar(VV_ALL)`或`asc_mem_bar(VST_VLD)`，按实际生产-消费关系选择合适的`mem_type`），保证多个指令串行化，防止出现异常数据。

## 调用示例

```cpp
vector_load_unalign ureg;
__ubuf__ half* src = (__ubuf__ half*)asc_get_phy_buf_addr(0);
uint32_t offset = 5;
asc_loadunalign_pre(ureg, src + offset);
vector_half dst;
asc_loadunalign(dst, ureg, src + offset);
```
