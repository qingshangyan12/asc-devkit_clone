# asc_pair_reduce_sum

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持
- Kirin X90：不支持
- Kirin 9030：不支持

## 功能说明

头文件为：

```cpp
#include "c_api/asc_simd.h"
```

`asc_pair_reduce_sum`是RegBase相邻元素归约接口。接口按照`mask`将`src`中每两个相邻元素相加，结果连续写入`dst`的低半部分，`dst`的高半部分置0。掩码未选中的源元素按0参与求和。结果可以写入引用参数，也可以通过返回值取得。

对于包含$N$个元素的源寄存器，计算公式为：

$$
dst_i = mask_{2i}\cdot src_{2i} + mask_{2i+1}\cdot src_{2i+1},
\quad 0 \le i < \frac{N}{2}
$$

其中，掩码位为真时取1，否则取0；$dst_i=0$适用于$N/2 \le i < N$。

## 函数原型

[适用版本：950]

对`half`元素执行相邻求和，通过引用参数输出结果。

```cpp
__simd_callee__ inline void asc_pair_reduce_sum(
    vector_half& dst, vector_half src, vector_bool mask)
```

[适用版本：950]

对`float`元素执行相邻求和，通过引用参数输出结果。

```cpp
__simd_callee__ inline void asc_pair_reduce_sum(
    vector_float& dst, vector_float src, vector_bool mask)
```

[适用版本：950]

对`half`元素执行相邻求和，通过返回值输出结果。

```cpp
__simd_callee__ inline vector_half asc_pair_reduce_sum(
    vector_half src, vector_bool mask)
```

[适用版本：950]

对`float`元素执行相邻求和，通过返回值输出结果。

```cpp
__simd_callee__ inline vector_float asc_pair_reduce_sum(
    vector_float src, vector_bool mask)
```

## 参数说明

本接口没有模板参数。

**表 1** 参数说明

| 参数名 | 输入/输出 | 含义 |
|--------|-----------|------|
| `dst` | 输出 | 相邻求和结果寄存器，仅引用输出原型包含该参数。类型必须与`src`一致。寄存器参数不涉及`TPosition`或地址对齐。 |
| `src` | 输入 | 待归约的`vector_half`或`vector_float`源寄存器。寄存器参数不涉及`TPosition`或地址对齐。 |
| `mask` | 输入 | 源元素有效掩码。未选中的源元素按0参与对应的两元素求和。 |

## 数据类型支持

[适用版本：950]

| 输入 A 类型 | 输入 B 类型/标量类型 | 输出类型 |
|-------------|---------------------|----------|
| `vector_half` | 不涉及 | `vector_half` |
| `vector_float` | 不涉及 | `vector_float` |

## 返回值说明

| 返回值类型 | 含义 |
|-----------|------|
| `vector_half` | 保存`half`相邻元素求和结果，可直接传给后续寄存器计算接口。 |
| `vector_float` | 保存`float`相邻元素求和结果，可直接传给后续寄存器计算接口。 |

引用输出原型的返回值类型为`void`，结果写入`dst`。

## 约束说明

- 本接口只能在Vector Function中调用，并且仅在AIV上生效。
- `dst`和`src`的数据类型必须一致。
- `mask`必须在调用前赋值；未初始化的掩码寄存器内容不确定。
- 每对源元素独立应用掩码：两者均无效时结果为0，仅一个有效时结果等于该有效元素。
- `dst`仅低半部分包含有效归约结果，高半部分填0；写回UB时应根据后续数据排布避免覆盖相邻有效数据。
- 接口封装Vector流水上的`vcpadd`零填充模式，不占用UB或L1临时空间，也不分配eventID。

## 关键特性说明

### 单级相邻归约

一次调用只把元素数量归约为原来的一半，不会直接得到整个寄存器的总和。需要全寄存器求和时，应选择全归约接口，或在确认数据排布和掩码语义后继续执行归约。

## 理论性能

本接口的理论性能请参考[C-API Reg矢量计算接口理论性能汇总](../../../../../docs/zh/api/appendix/c_api_reg_vector_compute_theoretical_perf_summary.md)。

## 调用示例

下面的Vector Function对64个`float`元素进行相邻求和。有效结果位于`dst[0]`至`dst[31]`。

```cpp
#include "c_api/asc_simd.h"

constexpr uint32_t ELEMENT_COUNT = 64;

__simd_vf__ inline void pair_sum_vf(__ubuf__ float* dst, __ubuf__ float* src)
{
    vector_float src_reg;
    uint32_t count = ELEMENT_COUNT;
    vector_bool mask = asc_update_mask_b32(count);
    asc_loadalign(src_reg, src);

    vector_float dst_reg = asc_pair_reduce_sum(src_reg, mask);
    asc_storealign(dst, dst_reg, mask);
}
```

该调用形式由本仓库的3510单元测试`test_asc_pair_reduce_sum.cpp`和样例`examples/02_simd_c_api/03_c_api/02_reg_vector_compute/reduce_pair/reduce_pair.asc`覆盖。当前环境未提供`bisheng`可执行文件，因此本次未执行离线编译。

## 关联API

| 相似API | 与当前API区别 | 选择建议 |
|---------|---------------|----------|
| `asc_reduce_sum` | 对全部有效元素执行归约。 | 目标是单个总和时使用`asc_reduce_sum`。 |
| `asc_reduce_sum_datablock` | 按数据块执行归约。 | 需要保持数据块粒度时使用该接口。 |

