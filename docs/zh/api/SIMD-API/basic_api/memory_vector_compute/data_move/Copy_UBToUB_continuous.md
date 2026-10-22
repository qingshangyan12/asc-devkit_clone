# Copy（UBToUB连续数据搬运）<a name="ZH-CN_TOPIC_0000002575088175"></a>

## 产品支持情况<a name="zh-cn_topic_0000002567699435_section796754519912"></a>

<!-- npu="950" id11 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id11 -->
<!-- npu="A3" id12 -->
- Atlas A3系列产品：不支持
<!-- end id12 -->
<!-- npu="910b" id13 -->
- Atlas A2系列产品：不支持
<!-- end id13 -->
<!-- npu="310b" id14 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id14 -->
<!-- npu="310p" id15 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id15 -->
<!-- npu="310p" id16 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id16 -->
<!-- npu="910" id17 -->
- Atlas训练系列产品：不支持
<!-- end id17 -->
<!-- npu="x90" id18 -->
- Kirin X90处理器系列产品：不支持
<!-- end id18 -->
<!-- npu="9030" id19 -->
- Kirin 9030处理器系列产品：不支持
<!-- end id19 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/Copy_UBToUB_continuous_res.md#id1 -->

## 功能说明<a name="zh-cn_topic_0000002567699435_section106841136114319"></a>

头文件路径为：`"basic_api/kernel_operator_data_copy_intf.h"`。

支持Unified Buffer（UB）内部的连续数据搬运，数据搬运时格式和内容保持不变。

具体支持的数据通路为（以[逻辑位置TPosition](../../aux_data_structures/TPosition.md)表示）：

- UB -> UB
    - VECIN -> VECCALC
    - VECIN -> VECOUT
    - VECCALC -> VECIN
    - VECCALC -> VECOUT
    - VECOUT -> VECIN
    - VECOUT -> VECCALC

## 函数原型<a name="zh-cn_topic_0000002567699435_section82039854412"></a>

```cpp
template <typename T, bool isSetMask = true>
__aicore__ inline void Copy(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
```

### 指针与LocalTensor混合调用

本接口增加以下指针重载。dst和src可独立选择`__ubuf__ T*`或LocalTensor，至少一个操作数为指针；全LocalTensor调用继续使用原接口。指针形式沿用本页的产品支持、数据类型、32字节对齐和地址重叠约束。调用者须保证指针所指缓冲区容量有效。

```cpp
template <
    typename T = VectorOperand::DefaultType, bool isSetMask = true, typename Operand0, typename Operand1,
    typename Std::enable_if<
        VectorOperand::HasPointer<Operand0, Operand1>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand0>() &&
            VectorOperand::IsWritable<Operand0>() &&
            VectorOperand::Matches<VectorOperand::ElementT<T, Operand0>, Operand1>(),
        int>::type = 0>
__aicore__ inline __inout_pipe__(V) void Copy(Operand0 dst, Operand1 src, const uint32_t count);
```

模板参数T默认从目的操作数推导，显式指定时须与源、目的元素类型一致；目的操作数必须可写。Operand0/Operand1由实参推导，enable_if用于检查合法组合，无需调用者填写。其他参数及返回值与原形式一致。公共约束和完整核函数示例见[VECTOR Basic API指针接口](../vector_pointer.md)。

## 参数说明<a name="zh-cn_topic_0000002567699435_section16128134420472"></a>

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 源操作数或者目的操作数的数据类型。支持的数据类型请参考[数据类型](#zh-cn_topic_0000002567699435_section42191353048181)。 |
| isSetMask | 参数不生效，保持默认值true即可。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。<br>类型为[LocalTensor](../../data_structures/LocalTensor/LocalTensor_intro.md)，支持的TPosition为VECIN/VECCALC/VECOUT。起始地址需要保证32字节对齐。 |
| src | 输入 | 源操作数。<br>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT。起始地址需要保证32字节对齐。<br>源操作数的数据类型需要与目的操作数保持一致。 |
| count | 输入 | 参与搬运的元素个数。 |

## 数据类型<a name="zh-cn_topic_0000002567699435_section42191353048181"></a>

源操作数和目的操作数支持的数据类型保持一致。
<!-- npu="950" id1 -->
Ascend 950PR&950DT系列产品，支持的数据类型为：int8_t、uint8_t、fp4x2_e2m1_t、fp4x2_e1m2_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t。
<!-- end id1 -->

## 返回值说明

无

## 约束说明<a name="zh-cn_topic_0000002567699435_section2045914466492"></a>

- 位于UB的地址必须32字节对齐。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../general_description_and_constraints.md#通用地址重叠约束)。
- isSetMask参数不生效，保持默认值true即可。

## 调用示例<a name="zh-cn_topic_0000002567699435_section088124295117"></a>

```cpp
AscendC::Copy(dstLocal, srcLocal, 512);
```

### 指针调用示例

```cpp
// dstLocal/srcLocal为按本页要求分配并同步的UB缓冲区，容量至少为64个float。
__ubuf__ float* dstPtr = (__ubuf__ float*)dstLocal.GetPhyAddr();
__ubuf__ float* srcPtr = (__ubuf__ float*)srcLocal.GetPhyAddr();
// 三种组合为可选的独立调用示例。
AscendC::Copy(dstPtr, srcLocal, uint32_t(64));
AscendC::PipeBarrier<PIPE_V>();
AscendC::Copy(dstLocal, srcPtr, uint32_t(64));
AscendC::PipeBarrier<PIPE_V>();
AscendC::Copy<float>(dstPtr, srcPtr, uint32_t(64));
```
