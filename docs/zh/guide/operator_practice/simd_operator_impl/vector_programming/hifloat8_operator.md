# hifloat8_t数据类型算子实践

`hifloat8_t`是Ascend 950PR/Ascend 950DT支持的一种8位浮点数据类型。与指数位宽固定的浮点数据类型不同，`hifloat8_t`使用指数位宽控制字段动态调整指数位和尾数位的编码方式，兼顾数据精度和取值范围。数据格式的详细说明请参见[内置数据类型](../../../../api/SIMD-API/basic_api/data_structures/builtin_data_types.md)。

本节以输入、输出均为`hifloat8_t`的Add算子为例，介绍低精度数据搬运、片上缓冲区规划、计算类型转换以及结果精度验证的实现方法。

## 适用产品

Ascend 950PR/Ascend 950DT

## 实现思路

`hifloat8_t`适合降低数据存储和搬运开销，但[Add](../../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Add.md)接口不支持直接对`hifloat8_t`类型执行加法。因此，本例采用“低精度存储、较高精度计算”的实现方式：

1. 将两个`hifloat8_t`输入从Global Memory搬入Unified Buffer（UB）。
2. 使用[Cast](../../../../api/SIMD-API/basic_api/memory_vector_compute/type_conversion/Cast.md)接口将两个输入转换为`half`类型。
3. 使用Add接口对`half`类型的数据执行逐元素加法。
4. 使用Cast接口将结果转换回`hifloat8_t`类型，再搬出到Global Memory。

设$D_{HiF8}$表示`hifloat8_t`到`half`的转换，$Q_{HiF8}$表示`half`到`hifloat8_t`的转换，则计算关系为：

$$
z_i = Q_{HiF8}(D_{HiF8}(x_i) + D_{HiF8}(y_i))
$$

`hifloat8_t`转换为`half`时不存在窄化导致的精度损失，舍入模式使用`CAST_NONE`。`half`转换为`hifloat8_t`时存在精度损失，本例使用结果可复现的`CAST_ROUND`模式。关于`CAST_HYBRID`模式的选择建议，请参见[舍入模式与精度验证](#舍入模式与精度验证)。

## 算子分析

本例在单核上一次处理2048个元素，输入、输出均为ND格式，shape均为`(1, 2048)`。

**表1** hifloat8_t Add算子输入输出规格

| 名称 | 输入/输出 | shape | 数据类型 | format |
| --- | --- | --- | --- | --- |
| x | 输入 | `(1, 2048)` | `hifloat8_t` | ND |
| y | 输入 | `(1, 2048)` | `hifloat8_t` | ND |
| z | 输出 | `(1, 2048)` | `hifloat8_t` | ND |

算子实现使用的主要接口如下：

- [DataCopy](../../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMAndUB_continuous.md)：在Global Memory和UB之间搬运连续数据。
- Cast：完成`hifloat8_t`和`half`之间的精度转换。
- Add：完成`half`类型的逐元素加法。
- [TQue](../../../../api/SIMD-API/basic_api/resource_management/TQue/TQue_intro.md)：管理输入和输出LocalTensor，并实现CopyIn、Compute和CopyOut任务间的同步。
- [TBuf](../../../../api/SIMD-API/basic_api/resource_management/TBuf/TBuf.md)：管理类型转换后用于计算的`half`临时数据。

## UB空间规划

`hifloat8_t`类型的每个元素占1字节，`half`类型的每个元素占2字节。两个输入Queue、一个输出Queue和两个计算临时缓冲区的空间规划如下。

**表2** UB空间规划

| 缓冲区 | 数量 | 单个缓冲区大小 | 用途 |
| --- | ---: | ---: | --- |
| 输入Queue | 2 | 2048字节 | 存储x和y的`hifloat8_t`数据 |
| 输出Queue | 1 | 2048字节 | 存储z的`hifloat8_t`数据 |
| 计算TBuf | 2 | 4096字节 | 存储x和y转换后的`half`数据，其中一个缓冲区复用为Add结果 |

本例共使用14336字节的UB空间。计算缓冲区必须按照转换后的`half`类型规划，不能仍按`hifloat8_t`的1字节元素宽度分配。

本例每次搬运2048个`hifloat8_t`元素，搬运量为2048字节，满足DataCopy的32字节对齐要求。如果修改数据长度，应保证`count * sizeof(hifloat8_t)`为32字节的整数倍；非对齐数据的处理方式请参见[非对齐场景](unaligned_scenario.md)。

## 核函数实现

### 初始化

初始化阶段设置GlobalTensor的Global Memory地址，通过TQue为输入和输出分配空间，并通过TBuf为转换后的`half`数据分配临时空间。

```cpp
constexpr uint32_t TOTAL_LENGTH = 2048;

AscendC::GlobalTensor<hifloat8_t> xGm;
AscendC::GlobalTensor<hifloat8_t> yGm;
AscendC::GlobalTensor<hifloat8_t> zGm;
xGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(x), TOTAL_LENGTH);
yGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(y), TOTAL_LENGTH);
zGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(z), TOTAL_LENGTH);

AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueX;
AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueY;
AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueZ;
pipe.InitBuffer(inQueueX, 1, TOTAL_LENGTH * sizeof(hifloat8_t));
pipe.InitBuffer(inQueueY, 1, TOTAL_LENGTH * sizeof(hifloat8_t));
pipe.InitBuffer(outQueueZ, 1, TOTAL_LENGTH * sizeof(hifloat8_t));

AscendC::TBuf<AscendC::TPosition::VECCALC> calcBufX;
AscendC::TBuf<AscendC::TPosition::VECCALC> calcBufY;
pipe.InitBuffer(calcBufX, TOTAL_LENGTH * sizeof(half));
pipe.InitBuffer(calcBufY, TOTAL_LENGTH * sizeof(half));
```

### CopyIn

CopyIn阶段将两个`hifloat8_t`输入从Global Memory搬入UB，并分别放入输入Queue。

```cpp
AscendC::LocalTensor<hifloat8_t> xLocal = inQueueX.AllocTensor<hifloat8_t>();
AscendC::LocalTensor<hifloat8_t> yLocal = inQueueY.AllocTensor<hifloat8_t>();
AscendC::DataCopy(xLocal, xGm, TOTAL_LENGTH);
AscendC::DataCopy(yLocal, yGm, TOTAL_LENGTH);
inQueueX.EnQue(xLocal);
inQueueY.EnQue(yLocal);
```

### Compute

Compute阶段先将输入扩展为`half`类型，再执行加法，最后将结果转换回`hifloat8_t`类型。`xCalc`在完成输入转换后复用为Add结果，避免额外申请第三个`half`临时缓冲区。

```cpp
AscendC::LocalTensor<hifloat8_t> xLocal = inQueueX.DeQue<hifloat8_t>();
AscendC::LocalTensor<hifloat8_t> yLocal = inQueueY.DeQue<hifloat8_t>();
AscendC::LocalTensor<hifloat8_t> zLocal = outQueueZ.AllocTensor<hifloat8_t>();
AscendC::LocalTensor<half> xCalc = calcBufX.Get<half>();
AscendC::LocalTensor<half> yCalc = calcBufY.Get<half>();

AscendC::Cast(xCalc, xLocal, AscendC::RoundMode::CAST_NONE, TOTAL_LENGTH);
AscendC::Cast(yCalc, yLocal, AscendC::RoundMode::CAST_NONE, TOTAL_LENGTH);
AscendC::Add(xCalc, xCalc, yCalc, TOTAL_LENGTH);
AscendC::Cast(zLocal, xCalc, AscendC::RoundMode::CAST_ROUND, TOTAL_LENGTH);

outQueueZ.EnQue(zLocal);
inQueueX.FreeTensor(xLocal);
inQueueY.FreeTensor(yLocal);
```

### CopyOut

CopyOut阶段从输出Queue取出结果并搬运到Global Memory。

```cpp
AscendC::LocalTensor<hifloat8_t> zLocal = outQueueZ.DeQue<hifloat8_t>();
AscendC::DataCopy(zGm, zLocal, TOTAL_LENGTH);
outQueueZ.FreeTensor(zLocal);
```

### 完整代码

将以上阶段组合后的核函数代码如下。

```cpp
#include "kernel_operator.h"

constexpr uint32_t TOTAL_LENGTH = 2048;

class KernelHifloat8Add {
public:
    __aicore__ inline KernelHifloat8Add() {}

    __aicore__ inline void Init(
        __gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z, AscendC::TPipe* pipeIn)
    {
        pipe = pipeIn;
        xGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(x), TOTAL_LENGTH);
        yGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(y), TOTAL_LENGTH);
        zGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(z), TOTAL_LENGTH);

        pipe->InitBuffer(inQueueX, 1, TOTAL_LENGTH * sizeof(hifloat8_t));
        pipe->InitBuffer(inQueueY, 1, TOTAL_LENGTH * sizeof(hifloat8_t));
        pipe->InitBuffer(outQueueZ, 1, TOTAL_LENGTH * sizeof(hifloat8_t));
        pipe->InitBuffer(calcBufX, TOTAL_LENGTH * sizeof(half));
        pipe->InitBuffer(calcBufY, TOTAL_LENGTH * sizeof(half));
    }

    __aicore__ inline void Process()
    {
        CopyIn();
        Compute();
        CopyOut();
    }

private:
    __aicore__ inline void CopyIn()
    {
        AscendC::LocalTensor<hifloat8_t> xLocal = inQueueX.AllocTensor<hifloat8_t>();
        AscendC::LocalTensor<hifloat8_t> yLocal = inQueueY.AllocTensor<hifloat8_t>();
        AscendC::DataCopy(xLocal, xGm, TOTAL_LENGTH);
        AscendC::DataCopy(yLocal, yGm, TOTAL_LENGTH);
        inQueueX.EnQue(xLocal);
        inQueueY.EnQue(yLocal);
    }

    __aicore__ inline void Compute()
    {
        AscendC::LocalTensor<hifloat8_t> xLocal = inQueueX.DeQue<hifloat8_t>();
        AscendC::LocalTensor<hifloat8_t> yLocal = inQueueY.DeQue<hifloat8_t>();
        AscendC::LocalTensor<hifloat8_t> zLocal = outQueueZ.AllocTensor<hifloat8_t>();
        AscendC::LocalTensor<half> xCalc = calcBufX.Get<half>();
        AscendC::LocalTensor<half> yCalc = calcBufY.Get<half>();

        AscendC::Cast(xCalc, xLocal, AscendC::RoundMode::CAST_NONE, TOTAL_LENGTH);
        AscendC::Cast(yCalc, yLocal, AscendC::RoundMode::CAST_NONE, TOTAL_LENGTH);
        AscendC::Add(xCalc, xCalc, yCalc, TOTAL_LENGTH);
        AscendC::Cast(zLocal, xCalc, AscendC::RoundMode::CAST_ROUND, TOTAL_LENGTH);

        outQueueZ.EnQue(zLocal);
        inQueueX.FreeTensor(xLocal);
        inQueueY.FreeTensor(yLocal);
    }

    __aicore__ inline void CopyOut()
    {
        AscendC::LocalTensor<hifloat8_t> zLocal = outQueueZ.DeQue<hifloat8_t>();
        AscendC::DataCopy(zGm, zLocal, TOTAL_LENGTH);
        outQueueZ.FreeTensor(zLocal);
    }

private:
    AscendC::TPipe* pipe;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueX;
    AscendC::TQue<AscendC::TPosition::VECIN, 1> inQueueY;
    AscendC::TQue<AscendC::TPosition::VECOUT, 1> outQueueZ;
    AscendC::TBuf<AscendC::TPosition::VECCALC> calcBufX;
    AscendC::TBuf<AscendC::TPosition::VECCALC> calcBufY;
    AscendC::GlobalTensor<hifloat8_t> xGm;
    AscendC::GlobalTensor<hifloat8_t> yGm;
    AscendC::GlobalTensor<hifloat8_t> zGm;
};

extern "C" __global__ __vector__ void hifloat8_add_custom(
    __gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* z)
{
    AscendC::TPipe pipe;
    KernelHifloat8Add op;
    op.Init(x, y, z, &pipe);
    op.Process();
}
```

调用核函数时，应为x、y和z分别分配`TOTAL_LENGTH`字节的Device侧内存，并使用单核启动：

```cpp
hifloat8_add_custom<<<1, 0, stream>>>(xDevice, yDevice, zDevice);
```

## 舍入模式与精度验证

从`half`转换为`hifloat8_t`时，可以选择以下舍入模式：

- `CAST_ROUND`：使用确定性舍入，相同输入得到相同的输出，适合功能调试、结果复现和逐比特比较。
- `CAST_HYBRID`：`hifloat8_t`专用的随机舍入模式，适用于算法明确要求随机舍入的场景。随机舍入结果不适合与单一真值进行逐比特比较。

验证本例时，不能直接将`half`加法结果作为真值，因为输出还经过一次`hifloat8_t`量化。建议采用以下流程：

1. 将输入的`hifloat8_t`数据转换为高精度数据。
2. 使用高精度数据计算逐元素加法。
3. 按照`CAST_ROUND`规则将加法结果量化为`hifloat8_t`，生成输出真值。
4. 对确定性舍入的输出进行逐比特比较，或将输出反量化为`half`后，根据业务精度要求比较绝对误差和相对误差。

Host侧仅支持以指针方式声明`hifloat8_t`，不应依赖Host侧`hifloat8_t`标量运算生成真值。相关约束请参见[算子编译约束](../../../programming_guide/compilation_and_execution/operator_compilation/constraints.md)。

## 常见问题

- **将`hifloat8_t`当作`uint8_t`参与计算**：两者都占1字节，但编码和数值语义不同。`uint8_t`只适合在不解析数值的场景中承载原始字节，不能代替`hifloat8_t`执行数值计算。
- **直接调用Add处理`hifloat8_t`**：Add接口不支持该数据类型，需要先转换为`half`或`float`。本例选择`half`以减少UB占用；对精度要求更高时，可以转换为`float`，同时需要按4字节/元素重新规划计算缓冲区。
- **按元素个数判断搬运对齐**：DataCopy的对齐约束按字节计算。对于`hifloat8_t`，连续搬运的元素个数需要是32的整数倍。
- **临时缓冲区按源类型分配**：转换后的LocalTensor为`half`时，TBuf必须按`sizeof(half)`分配，否则会发生内存越界。
- **随机舍入使用逐比特真值校验**：使用`CAST_HYBRID`时，应根据业务目标设计误差或统计特性检查，不能要求每次输出的比特位完全一致。

## 扩展方向

在本例基础上，可以继续扩展以下场景：

- 数据长度不能被32字节整除时，结合DataCopyPad或非对齐搬运接口处理尾块。
- 数据量超过单核单次处理能力时，结合[多核Tiling切分](multi_core_tiling/overview.md)和[DoubleBuffer场景](double_buffer_scenario.md)进行分块与流水优化。
- 需要执行更多逐元素计算时，将输入统一扩展为`half`或`float`，完成计算后只在输出阶段转换回`hifloat8_t`，减少重复类型转换。
- 需要矩阵乘计算时，使用支持`hifloat8_t`输入和`float`累加的矩阵计算接口，并单独评估矩阵数据排布、量化尺度和累加精度。
