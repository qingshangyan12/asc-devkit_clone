# asc_relu

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
- Kirin X90：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：支持
<!-- end id9 -->

## 功能说明

- 接口声明文件：`c_api/reg_compute/reg_vector.h`
- 核心功能：根据mask对源操作数src进行按元素relu操作，将结果写入目的操作数dst。
- 实现原理/公式：

$$
dst_i = (src_i >= 0) ? src_i : 0
$$

## 函数原型

<!-- npu="950,x90,9030" id10 -->

```cpp
__simd_callee__ inline void asc_relu(vector_half& dst, vector_half src, vector_bool mask)

__simd_callee__ inline void asc_relu(vector_int32_t& dst, vector_int32_t src, vector_bool mask)

__simd_callee__ inline void asc_relu(vector_float& dst, vector_float src, vector_bool mask)
```

<!-- end id10 -->


## 参数说明

**表 1**  参数说明

<!-- npu="950,x90,9030" id11 -->

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数（矢量数据寄存器）。 |
| `src` | 输入 | 源操作数（矢量数据寄存器）。 |
| `mask` | 输入 | 源操作数掩码（掩码寄存器）。用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。 |

<!-- end id11 -->

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义.md](../reg数据类型定义.md)。

## 数据类型支持

<!-- npu="950,x90,9030" id12 -->

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|---|---|---|
| half | 不涉及 | half |
| int32_t | 不涉及 | int32_t |
| float | 不涉及 | float |

<!-- end id12 -->


## 返回值说明

无返回值。

## 流水类型

未从公开函数原型中直接识别到流水类型。

## 资源占用情况

- 临时空间占用：不占用UB/临时空间，操作直接在矢量数据寄存器上完成。
- 同步资源占用：占用Vector计算流水。
- 资源占用与参数/数据量关系：资源占用固定，不随参数或数据量变化。

## 约束说明

<!-- npu="950,x90,9030" id13 -->
- 地址重叠约束：操作在矢量数据寄存器上完成，不涉及内存地址重叠。
- 地址对齐约束：不涉及地址对齐（矢量数据寄存器操作）。
- 参数组合约束：无。
- 环境影响约束：无。
- 特殊数据类型约束：无。
- 性能约束：无。
- 异常和边界值处理：`src_i >= 0` 时 `dst_i = src_i`；`src_i < 0` 时 `dst_i = 0`。
<!-- end id13 -->

## 理论性能

<!-- npu="950,x90,9030" id14 -->

| 输入类型 | 输出类型 | 理论并行度(element/cycle) |
|---|---|---|
| half | half | 128 |
| int32_t | int32_t | 64 |
| float | float | 64 |

<!-- end id14 -->

## 调用示例

<!-- npu="950,x90,9030" id15 -->

```cpp
vector_half dst;
vector_half src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr是外部输入的UB内存空间地址。
// mask为源操作数掩码
asc_relu(dst, src, mask);
```

- 完整样例链接：`<TODO>`

<!-- end id15 -->