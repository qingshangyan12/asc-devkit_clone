# hifloat8_t数据类型算子实践

hifloat8_t是面向低精度量化场景的8-bit浮点存储类型。本文介绍hifloat8_t的类型定位及相关硬件支持能力，包括数据搬运、向量计算、矩阵乘和Fixpipe随路量化，并以SwiGLU激活算子为例，基于静态Tensor编程方式讲解“低精度存储、高精度计算”的完整开发流程与精度验证方法。

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->

## 背景：为什么需要hifloat8_t

大模型推理的性能瓶颈通常不在计算，而在访存：Transformer的FFN（Feed-Forward Network）块中，中间激活张量的规模通常为hidden size的数倍，相邻层之间的激活读写占据了大量带宽和显存。将激活、权重等大张量以8-bit低精度格式存储和搬运，可以将访存量与显存占用降低到half的一半。

hifloat8_t正是为此设计的低精度数据类型，其核心特性包括：

- **1字节浮点编码**：相比half（2字节），存储和搬运开销减半。
- **宽动态范围**：hifloat8_t采用"符号位+指数位宽控制字段D+指数E+尾数M"的编码，通过D字段（点域）动态划分指数位和尾数位，以浮点方式覆盖宽动态范围，Normal数的绝对值最大达$2^{15}$（32768）。与int8等定点量化不同，hifloat8_t无需校准scale即可直接量化大模型激活这类动态范围较宽的数据，省去量化校准流程。
- **与高精度类型无损互转**：hifloat8_t转half或float不产生精度损失，这使得"低精度搬运、高精度计算"的模式没有反量化误差。

hifloat8_t的字段分布、取值范围、Normal/Subnormal计算公式和特殊值的详细说明请参见[内置数据类型中的hifloat8_t说明](../../../../api/SIMD-API/basic_api/data_structures/builtin_data_types.md#fig261021710100)。

## HIF8能力总览

hifloat8_t的定位是**低精度存储和搬运类型**，不是计算类型。HIF8相关能力分布如下：

**表1** HIF8能力总览

| 能力域 | 支持情况 | 主要接口 |
| --- | --- | --- |
| 数据搬运 | GM与UB之间、UB内部均可按字节搬运 | [DataCopy](../../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMAndUB_continuous.md)系列 |
| 向量计算 | 不支持hifloat8_t直接参与算术运算，需先转换为half/float | [Cast](../../../../api/SIMD-API/basic_api/memory_vector_compute/type_conversion/Cast.md)+矢量计算API |
| 矢量量化 | 支持float/half量化输出hifloat8_t | [Cast](../../../../api/SIMD-API/basic_api/reg_vector_compute/type_conversion/Cast.md) |
| 矩阵乘 | A/B矩阵支持hifloat8_t输入，L0C以float累加，C矩阵支持hifloat8_t输出 | [Matmul](../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/Matmul_usage.md)、[Mmad](../../../../api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/Mmad.md) |
| Fixpipe随路量化 | L0C的float结果搬出时直接量化为hifloat8_t | [Fixpipe](../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)、[SetQuantScalar](../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetQuantScalar.md) |

### 数据搬运

[DataCopy](../../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMAndUB_continuous.md)系列接口在GM与UB之间搬运hifloat8_t数据时，按1字节元素处理，即`count * sizeof(hifloat8_t)`字节的连续数据块。约束与通用规则一致：

- 搬运量须为32字节的整数倍，否则实际搬运量会向下取整到32字节边界；
- UB侧地址须32字节对齐。

UB内部的Copy、GM到UB的ND2NZ等切分搬运同样支持hifloat8_t。注意[DataCopyPad](../../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)不支持hifloat8_t，非对齐数据的处理方式请参见[非对齐场景](../../simd_operator_impl/vector_programming/unaligned_scenario.md)。

### 向量计算与类型转换

矢量计算API（[Add](../../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Add.md)、[Mul](../../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Mul.md)、[Exp](../../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Exp.md)等）的操作数不支持hifloat8_t，这是hifloat8_t与half最大的使用差异。hifloat8_t上的矢量算子须遵循"**反量化-计算-再量化**"（DQ-Compute-Q）模式：

1. 使用Cast接口将hifloat8_t输入转换为half或float（CAST_NONE模式，无损）；
2. 在half或float上完成计算；
3. 使用Cast接口将结果转换回hifloat8_t（CAST_ROUND或CAST_HYBRID模式）。

除Basic API的Cast外，C API（Reg）还提供[asc_hif82half](../../../../api/SIMD-API/c_api/reg_compute/reg_convert/asc_hif82half.md)、[asc_hif82float](../../../../api/SIMD-API/c_api/reg_compute/reg_convert/asc_hif82float.md)、[asc_half2hif8](../../../../api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2hif8.md)和[asc_float2hif8](../../../../api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2hif8.md)等类型转换接口。

### 矩阵乘与Fixpipe随路量化

矩阵乘是HIF8能力最完整的计算域，也是hifloat8_t的核心使用场景：

- **输入**：Matmul高阶API的A、B矩阵支持hifloat8_t（A、B数据类型必须一致），tiling中通过[SetAType](../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Tiling/SetAType.md)/SetBType配置`DataType::DT_HIFLOAT8`；bias支持float、half、bfloat16_t。
- **计算精度**：hifloat8_t输入在Cube中以float完成累加，L0C中的结果为float，量化误差只来自输入端的一次量化。
- **输出**：C矩阵支持hifloat8_t、half、bfloat16_t、float。当C为hifloat8_t时，L0C的float结果经Fixpipe随路量化直接输出，无需单独的反量化算子。
- **Fixpipe量化模式**：`QF322HIF8_PRE`/`VQF322HIF8_PRE`（scalar/tensor量化，Half to Away舍入）和`QF322HIF8_PRE_HYBRID`/`VQF322HIF8_PRE_HYBRID`（Hybrid舍入），量化参数scale/offset的配置请参见[Fixpipe随路量化](../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md)和[SetQuantScalar](../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetQuantScalar.md)。
- **溢出行为**：hifloat8_t的溢出判断阈值为$1.25 \times 2^{15} = 40960$，量化后的值绝对值不小于40960时输出inf，而不是饱和到最大值。各特殊值的边界行为请参见[矩阵计算结果搬出接口边界值说明](../../../../api/appendix/matrix_compute_move_out_interface_boundary_value_summary.md)。

A、B矩阵为hifloat8_t的Matmul完整可运行样例请参见[Matmul FP8直调样例](../../../../../../examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_fp8/README.md)。C API场景下，[asc_enable_hif8](../../../../api/SIMD-API/c_api/cube_compute/asc_enable_hif8.md)可以使能fp8_e4m3fn_t位模式输入按hifloat8_t解释参与矩阵乘。

由此可以勾勒出以HIF8执行量化推理的典型数据流：**hif8输入矩阵乘（L0C float累加）→ Fixpipe随路量化输出hif8激活 → 矢量算子以DQ-Compute-Q模式消费hif8激活 → 下一个hif8矩阵乘**。层间激活全程以hifloat8_t流转，访存开销相比half减半。

## 舍入模式：CAST_ROUND与CAST_HYBRID

从hifloat8_t转为half或float属于数据类型小转大，使用CAST_NONE模式，无精度损失。从half或float转为hifloat8_t属于数据类型大转小，支持CAST_ROUND和CAST_HYBRID两种舍入模式，两者的区别是hifloat8_t量化中最关键的决策点。

### CAST_ROUND

CAST_ROUND是确定性的四舍五入（Half to Away）模式：按照HIF8转换规则对被截断部分四舍五入，当被截断部分恰好为可表示间隔的一半时，向远离零的方向进位。相同输入和相同控制配置下，输出永远相同，结果可逐比特复现，适合功能验证、逐比特真值比较以及需要稳定输出的推理流程。

### CAST_HYBRID

CAST_HYBRID是HIF8专用的混合随机舍入模式。其舍入行为与数值的指数$E_v$有关：

- 当$|E_v| < 4$时，按保护位（Guard bit）确定性进位，行为与CAST_ROUND一致。
- 当$4 \leq |E_v| \leq 15$（Normal数）或$-22 \leq E_v \leq -16$（Subnormal数）时，将尾数被截断部分`frac`与阈值`thr`比较：`frac >= thr`则尾数加1并处理进位，否则直接截断。阈值`thr`取自源操作数的低位（float转hifloat8_t时取源数据低14位，half转hifloat8_t时按`{tmp2[0], 1'b1}`拼接），因此舍入结果随源数据低位变化，等价于随机舍入。
- 当$E_v = -23$时，进位结果决定输出最小Subnormal还是Zero。

随机舍入的价值在于量化噪声的统计特性：当源数据低位近似均匀分布时，进位概率与被截断部分的大小成正比，量化误差在统计上近似无偏（期望趋于0）。对大模型量化而言，逐层累积的系统性量化偏差是影响精度的因素之一，随机舍入可以避免误差方向固定；而确定性舍入在同一数值上永远朝同一方向取整，大量累加后可能引入固定偏置。

使用CAST_HYBRID时注意：

- 输出依赖源数据低位，相同输入数值的舍入结果不可逐比特复现，**不能以固定bit串作为唯一真值**。验证时应将输出反量化后统计误差分布（均值偏差、方差或相对误差）。
- 需要结果可复现的场景（功能调试、逐比特比对）应使用CAST_ROUND。
- Fixpipe随路量化提供对应的`QF322HIF8_PRE_HYBRID`/`VQF322HIF8_PRE_HYBRID`模式，舍入行为与CAST_HYBRID一致，矩阵乘结果可直接以混合舍入模式量化输出。

含NaN、inf、下溢等边界处理的完整转换规则请参见[Cast（Reg矢量）的转换规则](../../../../api/SIMD-API/basic_api/reg_vector_compute/type_conversion/Cast.md)，各舍入模式的定义请参见[RoundMode](../../../../api/SIMD-API/basic_api/reg_vector_compute/aux_data_types/RoundMode.md)。

### 类型转换接口选择

**表2** hifloat8_t常用类型转换接口

| 源类型 | 目的类型 | Basic API | C API Reg类型转换接口 | 推荐舍入模式 |
| --- | --- | --- | --- | --- |
| hifloat8_t | half | [Cast](../../../../api/SIMD-API/basic_api/memory_vector_compute/type_conversion/Cast.md) | [asc_hif82half](../../../../api/SIMD-API/c_api/reg_compute/reg_convert/asc_hif82half.md) | CAST_NONE |
| hifloat8_t | float | [Cast](../../../../api/SIMD-API/basic_api/memory_vector_compute/type_conversion/Cast.md) | [asc_hif82float](../../../../api/SIMD-API/c_api/reg_compute/reg_convert/asc_hif82float.md) | CAST_NONE |
| half | hifloat8_t | [Cast](../../../../api/SIMD-API/basic_api/memory_vector_compute/type_conversion/Cast.md) | [asc_half2hif8](../../../../api/SIMD-API/c_api/reg_compute/reg_convert/asc_half2hif8.md) | CAST_ROUND或CAST_HYBRID |
| float | hifloat8_t | [Cast](../../../../api/SIMD-API/basic_api/memory_vector_compute/type_conversion/Cast.md) | [asc_float2hif8](../../../../api/SIMD-API/c_api/reg_compute/reg_convert/asc_float2hif8.md) | CAST_ROUND或CAST_HYBRID |

## 实践：SwiGLU激活算子

### 场景与收益

现代大模型的FFN块普遍采用SwiGLU结构：

$$FFN(x) = W_{down} \cdot SwiGLU(W_{gate} \cdot x,\ W_{up} \cdot x)$$

$$SwiGLU(g, u) = g \cdot \sigma(g) \cdot u,\quad \sigma(x) = \frac{1}{1 + e^{-x}}$$

以HIF8执行量化推理时，SwiGLU正处于上文数据流的中间环节：gate_proj、up_proj两个矩阵乘以hifloat8_t输入、Fixpipe随路量化输出hifloat8_t激活，SwiGLU算子消费这两个hifloat8_t激活并输出hifloat8_t，供down_proj的hifloat8_t矩阵乘消费。

选择SwiGLU作为hifloat8_t实践算子的理由：

- **访存收益直接**：SwiGLU是纯elementwise算子（2读1写），无任何数据复用，性能完全受访存带宽限制。输入输出从half换成hifloat8_t后，读入和写出的数据量均减半，算子访存量接近减半。
- **激活规模最大**：FFN中间激活是大模型中规模最大的激活张量之一（通常为hidden size的数倍），hifloat8_t使其显存占用减半，对batch推理和多并发场景的显存收益显著。
- **浮点量化的天然适配**：hifloat8_t是浮点格式，激活无需校准scale即可直接量化；且转float无损，反量化不引入额外误差。
- **计算必须在float上完成**：一方面矢量计算API不支持hifloat8_t直接运算；另一方面sigmoid、exp等非线性计算对精度敏感，在量化后的低精度上直接计算会放大量化误差，因此DQ-Compute-Q是标准模式。

设$D$表示hifloat8_t到float的反量化（CAST_NONE），$Q$表示float到hifloat8_t的再量化（CAST_ROUND），本例的计算关系为：

$$z_i = Q(D(g_i) \cdot \sigma(D(g_i)) \cdot D(u_i))$$

### 算子分析

本例在单核上一次处理2048个元素，输入、输出均为ND格式，shape均为`(1, 2048)`。

**表3** SwiGLU算子输入输出规格

| 名称 | 输入/输出 | shape | 数据类型 | format |
| --- | --- | --- | --- | --- |
| g | 输入 | (1, 2048) | hifloat8_t | ND |
| u | 输入 | (1, 2048) | hifloat8_t | ND |
| z | 输出 | (1, 2048) | hifloat8_t | ND |

算子实现使用的主要接口如下（采用[静态Tensor编程](../../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/static_tensor_programming.md)方式，直接构造LocalTensor并由开发者自行管理UB空间与同步）：

- [LocalTensor构造函数](../../../../api/SIMD-API/basic_api/data_structures/LocalTensor/LocalTensor_constructor.md)：按逻辑位置、字节偏移和元素个数直接构造静态Tensor。
- [DataCopy](../../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMAndUB_continuous.md)：在GM和UB之间搬运hifloat8_t数据。
- [asc_vf_call](../../../../api/SIMD-API/basic_api/reg_vector_compute/vf_call/asc_vf_call.md)：调用RegBase矢量计算函数。
- [LoadAlign](../../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/LoadAlign_continuous.md)/[StoreAlign](../../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_store/StoreAlign_continuous.md)：在UB与矢量寄存器之间加载和存储数据，并完成b8与b32的展开和压缩。
- [Cast](../../../../api/SIMD-API/basic_api/reg_vector_compute/type_conversion/Cast.md)：在寄存器中完成hifloat8_t与float之间的双向转换。
- [Muls](../../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Muls.md)/[Adds](../../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Adds.md)：寄存器与标量乘/加，用于构造$\sigma(x)=\frac{1}{1+e^{-x}}$。
- [Exp](../../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Exp.md)/[Div](../../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Div.md)/[Mul](../../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Mul.md)：完成指数、除法和逐元素乘法。
- [SetFlag](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md)/[WaitFlag](../../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md)：搬运流水（MTE2/MTE3）与计算流水（V）之间的同步。

### UB空间规划

hifloat8_t每个元素占1字节。静态Tensor编程方式下，UB空间由开发者以字节偏移自行规划；float中间数据仅驻留在矢量寄存器，不需要分配UB缓冲区。本例的缓冲区布局如下。

**表4** UB空间规划

| 缓冲区 | 数据类型 | 起始偏移（字节） | 大小（字节） | 用途 |
| --- | --- | ---: | ---: | --- |
| gLocal | hifloat8_t | 0 | 2048 | 输入g |
| uLocal | hifloat8_t | 2048 | 2048 | 输入u |
| zLocal | hifloat8_t | 4096 | 2048 | 输出z |

本例共使用6144字节UB空间。规划要点：

- 各缓冲区起始偏移必须满足32字节对齐，且偏移与大小之和不能超出UB物理上限。
- RegBase函数每轮处理64个元素，`DIST_UNPACK4_B8`将64个hifloat8_t元素展开后加载到寄存器，`DIST_PACK4_B32`将量化结果压缩为连续的64字节后写回UB。
- 每次搬运2048个hifloat8_t元素，搬运量为2048字节，满足DataCopy的32字节对齐要求。若修改数据长度，应保证`count * sizeof(hifloat8_t)`为32字节的整数倍。

### 核函数实现

按照[静态Tensor编程](../../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/static_tensor_programming.md)方式，核函数不经过TPipe/TQue，由算子类直接构造静态Tensor、规划UB偏移，并通过SetFlag/WaitFlag在搬入（MTE2）、计算（V）、搬出（MTE3）三条流水之间插入同步。

#### 初始化

初始化阶段仅设置GlobalTensor的Global Memory地址。UB空间不需要显式申请，在Process中通过字节偏移直接构造LocalTensor。

```cpp
constexpr uint32_t TOTAL_LENGTH = 2048;

AscendC::GlobalTensor<hifloat8_t> gGm;  // 管理GM上hifloat8_t输入g
AscendC::GlobalTensor<hifloat8_t> uGm;  // 管理GM上hifloat8_t输入u
AscendC::GlobalTensor<hifloat8_t> zGm;  // 管理GM上hifloat8_t输出z
gGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(g), TOTAL_LENGTH);
uGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(u), TOTAL_LENGTH);
zGm.SetGlobalBuffer(reinterpret_cast<__gm__ hifloat8_t*>(z), TOTAL_LENGTH);
```

#### 构造静态Tensor

在Process中按表4的字节偏移构造静态Tensor。VECCALC在硬件上映射为UB。

```cpp
// VECCALC在硬件上映射为UB，直接构造静态Tensor（位置、起始字节偏移、元素个数）
AscendC::LocalTensor<hifloat8_t> gLocal(AscendC::TPosition::VECCALC, gAddr, TOTAL_LENGTH);
AscendC::LocalTensor<hifloat8_t> uLocal(AscendC::TPosition::VECCALC, uAddr, TOTAL_LENGTH);
AscendC::LocalTensor<hifloat8_t> zLocal(AscendC::TPosition::VECCALC, zAddr, TOTAL_LENGTH);
```

各偏移以编译期常量定义在算子类中，保证32字节对齐：

```cpp
// 静态UB空间规划（字节偏移）
static constexpr uint32_t gAddr = 0;
static constexpr uint32_t uAddr = gAddr + TOTAL_LENGTH * sizeof(hifloat8_t);
static constexpr uint32_t zAddr = uAddr + TOTAL_LENGTH * sizeof(hifloat8_t);
```

#### CopyIn

CopyIn阶段将两个hifloat8_t输入从Global Memory搬入UB，随后通过同步事件通知计算流水数据就绪。`EVENT_ID0`为编译器内置的事件ID。搬运方式与half一致，仅字节数减半。

```cpp
AscendC::DataCopy(gLocal, gGm, TOTAL_LENGTH);
AscendC::DataCopy(uLocal, uGm, TOTAL_LENGTH);
// MTE2（搬入流水）完成后唤醒V（计算流水）
AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
```

#### Compute

Compute阶段调用RegBase函数，按每轮64个元素完成反量化、float计算和再量化。`tmpReg`依次复用为$-g$、$e^{-g}$、$1+e^{-g}$、$g/(1+e^{-g})$和最终结果，中间数据不写回UB。以下代码展示核心计算流程，完整代码请参见[样例](#完整样例)。

```cpp
mask = AscendC::Reg::UpdateMask<float>(remaining);
AscendC::Reg::LoadAlign<hifloat8_t, AscendC::Reg::LoadDist::DIST_UNPACK4_B8>(gHif8Reg, gAddr);
AscendC::Reg::LoadAlign<hifloat8_t, AscendC::Reg::LoadDist::DIST_UNPACK4_B8>(uHif8Reg, uAddr);
AscendC::Reg::Cast<float, hifloat8_t, HIF8_TO_FLOAT_TRAIT>(gReg, gHif8Reg, mask);
AscendC::Reg::Cast<float, hifloat8_t, HIF8_TO_FLOAT_TRAIT>(uReg, uHif8Reg, mask);
AscendC::Reg::Muls(tmpReg, gReg, -1.0f, mask);
AscendC::Reg::Exp(tmpReg, tmpReg, mask);
AscendC::Reg::Adds(tmpReg, tmpReg, 1.0f, mask);
AscendC::Reg::Div(tmpReg, gReg, tmpReg, mask);
AscendC::Reg::Mul(tmpReg, tmpReg, uReg, mask);
AscendC::Reg::Cast<hifloat8_t, float, FLOAT_TO_HIF8_TRAIT>(zHif8Reg, tmpReg, mask);
AscendC::Reg::StoreAlign<hifloat8_t, AscendC::Reg::StoreDist::DIST_PACK4_B32>(zAddr, zHif8Reg, mask);
```

`HIF8_TO_FLOAT_TRAIT`的舍入模式配置为`UNKNOWN`，该转换不会损失精度；`FLOAT_TO_HIF8_TRAIT`配置为`CAST_ROUND`。若算子对量化噪声有统计无偏要求，可将后者替换为`CAST_HYBRID`，验证方法参见[精度验证](#精度验证)。调用RegBase函数后，再通过`SetFlag/WaitFlag`通知MTE3流水搬出结果。

#### CopyOut

CopyOut阶段等待计算流水完成通知后，将hifloat8_t结果搬出到Global Memory。

```cpp
AscendC::DataCopy(zGm, zLocal, TOTAL_LENGTH);
```

#### 完整样例

完整的Kernel、Host调用、输入生成和精度校验代码请参见[hif8_swiglu_high_performance样例](../../../../../../examples/01_simd_cpp_api/05_best_practices/03_fusion_compute/hif8_swiglu_high_performance/README.md)。

### 精度验证

验证hifloat8_t算子时，**不能将float域的SwiGLU结果直接作为真值**，因为输出还经过一次float到hifloat8_t的量化。推荐流程如下：

1. 将输入的hifloat8_t数据解码为float（无损）；
2. 在float域计算SwiGLU，得到参考结果；
3. 按CAST_ROUND规则将参考结果量化为hifloat8_t，生成真值；
4. CAST_ROUND模式下，对输出进行逐比特比较；或将输出反量化为float后，按业务精度要求比较绝对误差和相对误差。

Host侧的hifloat8_t编解码可参考[Matmul FP8直调样例](../../../../../../examples/01_simd_cpp_api/04_advanced_api/00_matmul/matmul_fp8/README.md)中的`scripts/hif8.py`，该脚本提供了hifloat8_t的比特级编解码和真值生成参考实现。

CAST_HYBRID模式下，输出依赖输入低位，不存在唯一的比特级真值，应改用统计验证：将输出反量化后与float参考结果比较，统计误差的均值偏差（应接近0，验证无偏性）、方差和相对误差分布，结合业务精度要求设定阈值。

此外，Host侧仅支持以指针方式声明hifloat8_t，不支持Host侧hifloat8_t标量运算，真值计算应在float域完成。相关约束请参见[算子编译约束](../../../programming_guide/compilation_and_execution/operator_compilation/constraints.md)。
