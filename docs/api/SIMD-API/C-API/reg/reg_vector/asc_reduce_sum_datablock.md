# asc_reduce_sum_datablock

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
<!-- npu="x90" id8 -->
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明

头文件为：`#include "c_api/reg_compute/reg_vector.h"`

归约求和功能，用于将src每个DataBlock（32B）中参与计算的元素求和，得到的结果依次保存在dst中。

底层调用 `vcgadd` 指令（`MODE_ZEROING` 模式），对src中每个32B DataBlock内被mask选中的元素执行求和，求和结果写入dst对应位置；mask未选中的元素在输出中置零。

## 函数原型
```cpp
__simd_callee__ inline void asc_reduce_sum_datablock(vector_half& dst, vector_half src, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_reduce_sum_datablock(vector_float& dst, vector_float src, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_reduce_sum_datablock(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_reduce_sum_datablock(vector_int32_t& dst, vector_int16_t src, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_reduce_sum_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);
```

```cpp
__simd_callee__ inline void asc_reduce_sum_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask);
```

## 参数说明

**表1**  参数说明


| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器），用于保存每个DataBlock的求和结果。 |
| `src` | 输入 | 源操作数（矢量数据寄存器），按32B DataBlock划分参与求和。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算；mask未筛选的元素在输出中置零。 |

矢量数据寄存器和掩码寄存器的详细说明请参见 [reg数据类型定义](../reg数据类型定义.md)。


## 数据类型支持


| 输入A类型 | 输入B类型/标量类型 | 输出类型 |
|---|---|---|
| `half` | — | `half` |
| `float` | — | `float` |
| `uint16_t` | — | `uint32_t` |
| `int16_t` | — | `int32_t` |
| `uint32_t` | — | `uint32_t` |
| `int32_t` | — | `int32_t` |


## 返回值说明

无返回值。

## 约束说明

- 地址重叠约束：源操作数 `src` 与目的操作数 `dst` 均为矢量数据寄存器，不存在地址重叠问题。
- 地址对齐约束：不涉及内存地址，无对齐要求。
- 参数组合约束：`dst` 与 `src` 的数据类型必须符合「数据类型支持」表中的组合，不支持其它类型组合。
- 环境影响约束：无。
- 特殊数据类型约束：无。
- 性能约束：无。
- 累加顺序约束：指令内累加顺序采用二叉树累加方式，在每个DataBlock（32B）内两两相加逐层归约。
- 异常和边界值处理：当DataBlock中的元素均不参与计算（mask全为0）时，将0写入dst对应位置；`mask` 对应位置为0的元素不参与计算。

## 理论性能


| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| `int16_t` | `int32_t` | 64 |
| `uint16_t` | `uint32_t` | 64 |
| `half` | `half` | 128 |
| `int32_t` | `int32_t` | 64 |
| `uint32_t` | `uint32_t` | 64 |
| `float` | `float` | 64 |



## 调用示例


```cpp
// 对 half 类型矢量数据按 DataBlock(32B) 归约求和
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL); // 所有元素参与计算
asc_loadalign(src, src_addr);                    // src_addr 是外部输入的 UB 内存空间地址
asc_reduce_sum_datablock(dst, src, mask);
```

- 完整样例链接：`tests/api/c_api/npu_arch_3510/vector_compute/test_asc_reduce_sum_datablock.cpp`

