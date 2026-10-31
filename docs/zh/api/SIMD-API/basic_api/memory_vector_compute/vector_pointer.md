# VECTOR Basic API指针接口

## 产品支持情况

指针接口复用对应Basic API的底层实现。各接口支持的产品、数据类型、指令模式及限制以对应API资料为准，指针形式不增加硬件能力，各产品不一定支持下表中的全部接口。Ascend 950PR上的调用和构建示例见[指针样例](../../../../../../examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/element_wise_arithmetic/README.md)。

公开头文件提供指针重载声明；声明可见不代表该产品支持执行该接口。例如，Copy连续搬运与掩码搬运的产品支持范围不同，Fill指针形式仅支持Ascend 950的Cube核L1 Buffer，GetAbsAddr指针形式仅用于CPU_DEBUG。

## 功能说明

在原Basic API基础上，允许数据操作数分别使用裸指针或[LocalTensor](../data_structures/LocalTensor/LocalTensor_intro.md)。全LocalTensor调用继续选择原接口；至少一个数据操作数为指针时，使用指针重载。数值计算、掩码、步长、舍入模式和工作空间的语义保持不变。

普通VECTOR操作数使用`__ubuf__ T*`，指向UB中的数据。该接口不会分配存储空间，也不会将GM地址转换为UB地址。使用前应完成[TPipe](../resource_management/TPipe/TPipe.md)与缓冲区初始化，并遵守原API的同步要求。

按需包含下表中的头文件，或包含`"kernel_operator.h"`。头文件路径均以`basic_api/`开头。

本次扩展覆盖85个API名称，对应256条原始签名。各接口的功能、参数取值和硬件约束仍以各自API页面为准。下表按头文件列出范围；同名接口的各级调用方式、数据类型和产品支持情况可在[基础API列表](../basic_api_list.md)中查阅。头文件链接提供完整模板声明。

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

本页介绍各指针接口的公共调用规则，以Add的连续元素形式为例，以下声明来自`basic_api/kernel_operator_vec_binary_intf.h`：

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
__aicore__ inline void Add(Operand0 dst, Operand1 src0, Operand2 src1, const int32_t& count)
```

可以写成`Add(dstPtr, src0, src1, count)`或`Add<float>(dstPtr, src0, src1, count)`。`T`默认从目的操作数推导，显式指定时须与实际元素类型一致。Add的掩码数组、掩码连续模式同样允许各数据操作数独立选择指针或LocalTensor。

Copy的三种指针原型分别见[连续搬运](data_move/Copy_UBToUB_continuous.md)和[掩码式高维搬运](data_move/Copy_UBToUB_mask_highdim_split.md)。

**GetUnderlyingPtr**：由上述公开头文件间接提供，无需直接包含impl目录。

```cpp
template <typename T>
__aicore__ inline __ubuf__ T* GetUnderlyingPtr(__ubuf__ T* value)
```

```cpp
template <typename T>
__aicore__ inline __ubuf__ PrimT<T>* GetUnderlyingPtr(const LocalTensor<T>& value)
```

**Fill**：以下指针原型仅支持Ascend 950的Cube核L1 Buffer。

```cpp
template <
    typename T, typename U = PrimT<T>,
    typename Std::enable_if<Std::is_same<PrimT<T>, U>::value && !Std::is_const<T>::value, bool>::type = true>
__aicore__ inline void Fill(__cbuf__ T* dst, const InitConstValueParams<U>& initConstValueParams)
```

## 参数说明

以下表格分别说明本页列出的Add连续计算、GetUnderlyingPtr及Fill原型。其他接口的模板参数、掩码、步长、工作空间和配置参数请参见对应API资料。

**表1** Add连续计算模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数元素类型。默认值VectorOperand::DefaultType表示从Operand0推导；显式指定时须与三个操作数的元素类型一致。支持的数据类型见[Add](basic_arithmetic/Add.md)。 |
| Operand0 | 目的操作数类型，由dst推导。可为可写的UB指针或LocalTensor。 |
| Operand1、Operand2 | 源操作数类型，分别由src0、src1推导。可独立使用UB指针或LocalTensor；至少一个操作数须为指针。 |
| 未命名的enable_if参数 | 重载选择约束，检查指针参与、元素类型一致及目的操作数可写。默认值为0，调用者无需指定。 |

**表2** Add连续计算参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数，类型为Operand0，存储位置为UB。须可写且满足Add的对齐、容量及地址重叠约束。 |
| src0、src1 | 输入 | 源操作数，类型分别为Operand1、Operand2，存储位置为UB。元素类型须与dst一致，各源操作数可独立选择指针或LocalTensor。 |
| count | 输入 | 参与计算的元素个数，类型为int32_t。取值及容量要求见[连续计算](SIMD_compute/continuous_compute.md)。指针本身不携带容量信息，调用者须保证count个元素均有效。 |

**表3** GetUnderlyingPtr模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 从value推导的元素类型。指针输入返回T类型的指针，LocalTensor输入返回`PrimT<T>`类型的指针；`PrimT<T>`表示T对应的基础数据类型。 |

**表4** GetUnderlyingPtr参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| value | 输入 | UB指针或LocalTensor。LocalTensor须已初始化且位于UB；函数不分配或复制数据。返回指针的有效期与原缓冲区一致。 |

**表5** Fill模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | dst的元素类型，由目的指针推导，不得为const类型。支持的元素类型见[Fill](../cube_compute_ISASI/cube_load_aux_config/Fill.md)。 |
| U | 初始化值的类型，默认值为`PrimT<T>`，须与T对应的基础数据类型一致。 |
| 未命名的enable_if参数 | 检查`PrimT<T>`与U相同且T非const。默认值为true，调用者无需指定。 |

**表6** Fill参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数，类型为`__cbuf__ T*`，指向Cube核L1 Buffer。须满足Fill的对齐、存储大小及同步约束。 |
| initConstValueParams | 输入 | 初始化相关参数，类型为`InitConstValueParams<U>`。结构定义位于`basic_api/kernel_struct_mm.h`；成员含义、单位及取值范围见[Fill参数说明](../cube_compute_ISASI/cube_load_aux_config/Fill.md#section622mcpsimp)。 |

## 返回值说明

计算、搬运和填充接口返回void。GetAbsAddr返回CPU调试模型中的地址，类型为uint64_t。辅助函数GetUnderlyingPtr在指针输入时原样返回，在LocalTensor输入时返回其底层UB指针，不分配或复制数据。

## 约束说明

- 保留原API的数据类型、对齐、容量、地址空间、地址重叠及同步限制。例如Copy要求UB起始地址32字节对齐；其他接口的具体要求请查阅各自资料。
- 对指针做`ptr + n`时，n的单位为元素。偏移后的地址仍需满足原API的对齐要求。
- 开启ASCENDC_DEBUG、CPU_DEBUG或MSTX时，指针调用通过临时Tensor视图复用原接口的参数检查及上报；混用的LocalTensor保留原有位置、长度和偏移。原始指针不携带分配长度，视图仅记录从该地址到UB末尾的范围，无法检查某次分配的边界。调用者仍须保证所有访问和工作空间均在有效缓冲区内。
- `__gm__`、`__cbuf__`、L0地址不能作为普通VECTOR接口的UB操作数。不要通过强制转换掩盖错误地址空间。
- 指针重载不扩大原接口允许的数值域。例如Ln应使用有效输入，除法应避开非法除数；排序和转置仍需满足块大小要求。
- `isSetMask=false`时，仍须按原API要求预先设置掩码。输入/输出之间的流水同步由调用者负责。
- 原Tensor调用的重载选择保持不变。带config的Div、Exp、Ln、Reciprocal、Rsqrt、Sqrt形式由实现层按目标架构选择；不要在不支持的架构上显式请求该配置形式。

**Fill**：使用`__cbuf__`地址，不属于普通UB矢量计算。此指针实现不支持CPU_DEBUG，不接受UB或L0A/L0B指针。

**MrgSort与Transpose**

`MrgSortSrcList`允许四个源分别为指针或LocalTensor。C++17下可由构造参数推导各源类型；原有`MrgSortSrcList<T>`的默认源类型仍为LocalTensor。

排序记录布局、每路长度、有效位和工作空间约束沿用原接口。MrgSort的元素类型可从源列表推导，也可显式指定。

Transpose/TransDataTo5HD的指针列表和地址列表仍需满足原有排列与对齐要求。类型化指针数组可推导元素类型；uint64_t地址表只保存地址，必须显式指定实际数据类型，例如`TransDataTo5HD<half>(dstAddressTable, srcAddressTable, params)`。CPU_DEBUG沿用原Tensor接口的参数检查及调用分支，不将地址表改写为设备VA寄存器的打包格式；设备侧仍按原接口规则打包。

**NumericLimits与GetAbsAddr**

DeNormMin、Infinity、Lowest、NegativeInfinity、QuietNaN、SignalingNaN为`NumericLimits<T>`的成员函数，其指针形式向目的UB地址写入对应特殊值，元素类型和数量限制沿用原接口。

GetAbsAddr的指针形式包括TPipe成员和同名自由函数，仅在CPU_DEBUG中用于查询模拟地址。默认地址空间为Hardware::UB；查询L1时需显式指定Hardware::L1。不要将该CPU调试接口用于设备侧地址转换。UB/L1偏移、参数检查和转置地址表行为见[CPU单元测试](../../../../../../tests/api/basic_api/ascendc_case_ascend950pr_9599/ascendc_case_ascend950pr_9599_aiv_basic/test_operator_vector_pointer.cpp)。

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

四路归并排序可按如下方式构造混合源列表，元素类型由源列表推导：

```cpp
// src0Ptr指向UB，其余三个源为同元素类型的LocalTensor。
AscendC::MrgSortSrcList sources(src0Ptr, src1, src2, src3);
AscendC::MrgSort(dstPtr, sources, params);
```

完整构建和设备执行步骤见样例README中的CMake与CTest入口，包含8组指针与Tensor对照用例。
