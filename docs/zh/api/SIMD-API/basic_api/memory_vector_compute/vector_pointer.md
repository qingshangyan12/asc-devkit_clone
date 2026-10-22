# VECTOR Basic API指针接口

## 产品支持情况

指针接口复用对应Basic API的底层实现。各接口支持的产品、数据类型、指令模式及限制以对应API资料为准，指针形式不增加硬件能力。Ascend 950PR上的调用和构建示例见[指针样例](../../../../../../examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic/README.md)。

公开头文件提供指针重载声明；声明可见不代表该产品支持执行该接口。例如，Copy连续搬运与掩码搬运的产品支持范围不同，Fill指针形式仅支持Ascend 950的Cube核L1 Buffer，GetAbsAddr指针形式仅用于CPU_DEBUG。

## 功能说明

在原Basic API基础上，允许数据操作数分别使用裸指针或[LocalTensor](../data_structures/LocalTensor/LocalTensor_intro.md)。全LocalTensor调用继续选择原接口；至少一个数据操作数为指针时，使用指针重载。数值计算、掩码、步长、舍入模式和工作空间的语义保持不变。

普通VECTOR操作数使用`__ubuf__ T*`，指向UB中的数据。该接口不会分配存储空间，也不会将GM地址转换为UB地址。使用前应完成[TPipe](../resource_management/TPipe/TPipe.md)与缓冲区初始化，并遵守原API的同步要求。

按需包含下表中的头文件，或包含`"kernel_operator.h"`。头文件路径均以`basic_api/`开头。

## 接口范围

本次扩展覆盖85个API名称，对应256条原始签名。下表按头文件列出范围；同名接口的各级调用方式、数据类型和产品支持情况可在[基础API列表](../basic_api_list.md)中查阅。头文件链接提供完整模板声明。

| 头文件 | 接口名称 |
| --- | --- |
| [kernel_operator_data_copy_intf.h](../../../../../../include/basic_api/kernel_operator_data_copy_intf.h) | Copy |
| [kernel_operator_limits_intf.h](../../../../../../include/basic_api/kernel_operator_limits_intf.h) | DeNormMin、Infinity、Lowest、NegativeInfinity、QuietNaN、SignalingNaN |
| [kernel_operator_mm_intf.h](../../../../../../include/basic_api/kernel_operator_mm_intf.h) | Fill |
| [kernel_operator_proposal_intf.h](../../../../../../include/basic_api/kernel_operator_proposal_intf.h) | MrgSort、Sort |
| [kernel_operator_vec_binary_intf.h](../../../../../../include/basic_api/kernel_operator_vec_binary_intf.h) | AbsSub、Add、AddDeqRelu、AddRelu、And、Div、ExpSub、FusedAbsSub、FusedExpSub、FusedMulAdd、FusedMulAddRelu、Max、Min、Mul、MulAddDst、MulAddRelu、Mull、Or、Prelu、ShiftLeft、ShiftRight、Sub、SubRelu |
| [kernel_operator_vec_binary_scalar_intf.h](../../../../../../include/basic_api/kernel_operator_vec_binary_scalar_intf.h) | Adds、LeakyRelu、Maxs、Mins、Muls、ShiftLeft、ShiftRight、Divs、Subs、MulsCast、Ands、Ors |
| [kernel_operator_vec_brcb_intf.h](../../../../../../include/basic_api/kernel_operator_vec_brcb_intf.h) | Brcb |
| [kernel_operator_vec_cmpsel_intf.h](../../../../../../include/basic_api/kernel_operator_vec_cmpsel_intf.h) | Compare、Compares、CompareScalar、GetCmpMask、Select、SetCmpMask |
| [kernel_operator_vec_createvecindex_intf.h](../../../../../../include/basic_api/kernel_operator_vec_createvecindex_intf.h) | CreateVecIndex |
| [kernel_operator_vec_duplicate_intf.h](../../../../../../include/basic_api/kernel_operator_vec_duplicate_intf.h) | DeInterleave、Duplicate、Interleave |
| [kernel_operator_vec_gather_intf.h](../../../../../../include/basic_api/kernel_operator_vec_gather_intf.h) | Gather、Gatherb |
| [kernel_operator_vec_gather_mask_intf.h](../../../../../../include/basic_api/kernel_operator_vec_gather_mask_intf.h) | GatherMask |
| [kernel_operator_vec_mulcast_intf.h](../../../../../../include/basic_api/kernel_operator_vec_mulcast_intf.h) | MulCast |
| [kernel_operator_vec_reduce_intf.h](../../../../../../include/basic_api/kernel_operator_vec_reduce_intf.h) | ReduceDataBlock、ReduceMax、ReduceMin、ReducePairElem、ReduceRepeat、ReduceSum |
| [kernel_operator_vec_scatter_intf.h](../../../../../../include/basic_api/kernel_operator_vec_scatter_intf.h) | Scatter |
| [kernel_operator_vec_ternary_scalar_intf.h](../../../../../../include/basic_api/kernel_operator_vec_ternary_scalar_intf.h) | Axpy |
| [kernel_operator_vec_transpose_intf.h](../../../../../../include/basic_api/kernel_operator_vec_transpose_intf.h) | TransDataTo5HD、Transpose |
| [kernel_operator_vec_unary_intf.h](../../../../../../include/basic_api/kernel_operator_vec_unary_intf.h) | Abs、Exp、Ln、Neg、Not、Reciprocal、Relu、Rsqrt、Sqrt |
| [kernel_operator_vec_vconv_intf.h](../../../../../../include/basic_api/kernel_operator_vec_vconv_intf.h) | AddReluCast、Cast、CastDeq、CastDequant、SetDeqScale、SubReluCast、Truncate |
| [kernel_tpipe.h](../../../../../../include/basic_api/kernel_tpipe.h) | GetAbsAddr |

## 函数原型

指针重载使用模板推导每个数据操作数的类型。`Operand0`、`Operand1`等可分别为LocalTensor或对应地址空间的指针；`VectorOperand`为实现辅助类型，不需要由调用者显式指定。

以Add的连续元素形式为例，以下声明来自`basic_api/kernel_operator_vec_binary_intf.h`：

```cpp
template <
    typename T = VectorOperand::DefaultType, typename Operand0, typename Operand1, typename Operand2,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0, Operand1, Operand2>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand1>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand2>(),
        int>::type = 0>
__aicore__ inline void Add(Operand0 dst, Operand1 src0, Operand2 src1, const int32_t& count);
```

可以写成`Add(dstPtr, src0, src1, count)`或`Add<float>(dstPtr, src0, src1, count)`。`T`默认从目的操作数推导，显式指定时须与实际元素类型一致。Add的掩码数组、掩码连续模式同样允许各数据操作数独立选择指针或LocalTensor。

Copy的三种指针原型分别见[连续搬运](data_move/Copy_UBToUB_continuous.md)和[掩码式高维搬运](data_move/Copy_UBToUB_mask_highdim_split.md)。

## 参数说明

| 参数 | 输入/输出 | 指针形式的要求 |
| --- | --- | --- |
| dst或其他目的操作数 | 输出或输入/输出，沿用原API语义 | 必须可写。普通VECTOR接口使用UB地址；不得传入指向const元素的目的指针。 |
| src、src0、src1等源操作数 | 输入 | 各源操作数可独立使用指针或LocalTensor。元素类型及地址重叠要求沿用原API。 |
| 工作空间 | 输入/输出 | 若原接口要求工作空间，调用者仍须提供相同大小、位置和对齐的有效存储。 |
| count/calCount | 输入 | 保持原接口的单位与取值范围；通常表示元素个数。指针本身不携带容量信息。 |
| mask、repeatTime、repeatParams | 输入 | 保持原接口含义。指针偏移不能替代掩码与迭代步长的配置。 |
| scalarValue、roundMode、config等 | 输入或模板参数 | 保持原接口类型、合法值及配置约束。类型转换API仍按其原有规则区分输入和输出元素类型。 |

## 返回值说明

计算、搬运和填充接口返回void。GetAbsAddr返回CPU调试模型中的地址，类型为uint64_t。辅助函数GetUnderlyingPtr返回底层UB指针，不分配或复制数据。

## 约束说明

- 保留原API的数据类型、对齐、容量、地址空间、地址重叠及同步限制。例如Copy要求UB起始地址32字节对齐；其他接口的具体要求请查阅各自资料。
- 对指针做`ptr + n`时，n的单位为元素。偏移后的地址仍需满足原API的对齐要求。
- 适配层不持有指针的容量或生命周期信息，不能替调用者完成LocalTensor大小检查。调用者须保证所有访问和工作空间均在有效缓冲区内。
- `__gm__`、`__cbuf__`、L0地址不能作为普通VECTOR接口的UB操作数。不要通过强制转换掩盖错误地址空间。
- 指针重载不扩大原接口允许的数值域。例如Ln应使用有效输入，除法应避开非法除数；排序和转置仍需满足块大小要求。
- `isSetMask=false`时，仍须按原API要求预先设置掩码。输入/输出之间的流水同步由调用者负责。
- 原Tensor调用的重载选择保持不变。带config的Div、Exp、Ln、Reciprocal、Rsqrt、Sqrt形式由实现层按目标架构选择；不要在不支持的架构上显式请求该配置形式。

## 特殊接口

### GetUnderlyingPtr

该辅助函数由上述公开头文件间接提供，无需直接包含impl目录。

```cpp
template <typename T>
__aicore__ inline __ubuf__ T* GetUnderlyingPtr(__ubuf__ T* value);

template <typename T>
__aicore__ inline __ubuf__ PrimT<T>* GetUnderlyingPtr(const LocalTensor<T>& value);
```

指针输入原样返回；LocalTensor输入返回其底层地址。仅在LocalTensor实际位于UB时使用此结果进行VECTOR计算。返回值的有效期与原缓冲区一致。

### Fill

Fill指针形式用于Ascend 950 Cube核上的L1 Buffer，使用`__cbuf__`地址，不属于普通UB矢量计算。参数含义参见[Fill](../cube_compute_ISASI/cube_load_aux_config/Fill.md)。

```cpp
template <
    typename T, typename U = PrimT<T>,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value && !Std::is_const<T>::value, bool>::type = true>
__aicore__ inline void Fill(__cbuf__ T* dst, const InitConstValueParams<U>& initConstValueParams);
```

目的指针必须可写，T和U须满足模板约束；存储大小、初始化参数和同步约束沿用原Fill接口。此指针实现不支持CPU_DEBUG，不接受UB或L0A/L0B指针。

### MrgSort与Transpose

`MrgSortSrcList`允许四个源分别为指针或LocalTensor。C++17下可由构造参数推导各源类型；原有`MrgSortSrcList<T>`的默认源类型仍为LocalTensor。

```cpp
// src0Ptr指向UB，其余三个源为同元素类型的LocalTensor。
AscendC::MrgSortSrcList sources(src0Ptr, src1, src2, src3);
```

排序记录布局、每路长度、有效位和工作空间约束沿用原接口。Transpose/TransDataTo5HD的指针列表和地址列表仍需满足原有排列与对齐要求，不改变列表中地址的含义。

### NumericLimits与GetAbsAddr

DeNormMin、Infinity、Lowest、NegativeInfinity、QuietNaN、SignalingNaN为`NumericLimits<T>`的成员函数，其指针形式向目的UB地址写入对应特殊值，元素类型和数量限制沿用原接口。

GetAbsAddr的指针形式包括TPipe成员和同名自由函数，仅在CPU_DEBUG中用于查询模拟地址。默认地址空间为Hardware::UB；查询L1时需显式指定Hardware::L1。不要将该CPU调试接口用于设备侧地址转换。具体使用及UB/L1偏移检查见[CPU地址测试](../../../../../../tests/api/basic_api/vector_pointer/get_abs_addr_cpu.cpp)。

## 调用示例

下面的核函数在Ascend 950上分配三个UB缓冲区，用指针/LocalTensor混合调用Add和Copy。该示例演示核内调用，完整Host侧启动、构建和结果校验见[可运行样例](../../../../../../examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic/README.md)。

```cpp
#include "kernel_operator.h"

extern "C" __global__ __vector__ void VectorPointerExample()
{
    AscendC::InitSocState();
    AscendC::TPipe pipe;
    AscendC::TBuf<AscendC::TPosition::VECCALC> aBuf, bBuf, outBuf;
    constexpr uint32_t count = 64;
    pipe.InitBuffer(aBuf, count * sizeof(float));
    pipe.InitBuffer(bBuf, count * sizeof(float));
    pipe.InitBuffer(outBuf, count * sizeof(float));
    auto a = aBuf.Get<float>();
    auto b = bBuf.Get<float>();
    auto out = outBuf.Get<float>();
    __ubuf__ float* aPtr = (__ubuf__ float*)a.GetPhyAddr();
    __ubuf__ float* outPtr = (__ubuf__ float*)out.GetPhyAddr();
    AscendC::Duplicate(a, 1.0f, count);
    AscendC::Duplicate(b, 2.0f, count);
    AscendC::PipeBarrier<PIPE_V>();
    AscendC::Add(outPtr, aPtr, b, count);
    AscendC::PipeBarrier<PIPE_V>();
    AscendC::Copy(a, outPtr, count);
    AscendC::PipeBarrier<PIPE_V>();
}
```

## 测试与复现

[测试说明](../../../../../../tests/api/basic_api/vector_pointer/README.md)提供范围检查、主机适配检查、设备编译、非法操作数检查、设备回归、精度记录和CPU_DEBUG测试命令。

`ascendc_header_checker`在各配置中包含公开声明并检查通用调用；`vector_pointer/target_arithmetic.cpp`和`target_conversion.cpp`保留Ascend 950的完整实例化组合，由`run_target_headers.sh`编译。设备回归与精度检查另行运行；头文件编译通过不能替代设备执行结果。
