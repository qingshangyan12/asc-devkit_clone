# Log2

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
| <cann-filter npu-type="950"> Ascend 950PR/Ascend 950DT | √ </cann-filter> |
| <cann-filter npu-type="A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="310b"> Atlas 200I/500 A2 推理产品 | x </cann-filter> |
| <cann-filter npu-type="310p"> Atlas 推理系列产品AI Core | x </cann-filter> |
| <cann-filter npu-type="310p"> Atlas 推理系列产品Vector Core | x </cann-filter> |
| <cann-filter npu-type="910"> Atlas 训练系列产品 | x </cann-filter> |

## 功能说明

头文件路径：`"basic_api/reg_compute/kernel_reg_compute_vec_unary_intf.h"`。该接口用于根据mask对输入数据srcReg进行求以2为底的对数操作，将结果写入dstReg。

计算公式如下：

$$dstReg_i = \log_2(srcReg_i)$$

## 函数原型

```cpp
template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void Log2(U& dstReg, U& srcReg, MaskReg& mask)
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |
| mode | 可配置为MaskMergeMode的枚举或Log2SpecificMode的结构体实例。<br>&bull; MaskMergeMode，选择MERGING模式或ZEROING模式。<br>&nbsp;&nbsp;&bull; ZEROING，mask未筛选的元素在dstReg中置零。<br>&nbsp;&nbsp;&bull; MERGING，暂不支持。<br>&bull; Log2SpecificMode，定义如下：<pre><code>enum class Log2Algo {<br>    INTRINSIC = 0,<br>    PRECISION_1ULP_FTZ_TRUE,<br>    PRECISION_1ULP_FTZ_FALSE,<br>};<br>struct Log2SpecificMode{<br>    MaskMergeMode mrgMode = MaskMergeMode::ZEROING,<br>    Log2Algo algo = Log2Algo::INTRINSIC;<br>};</code></pre>&bull; mrgMode：选择MERGING模式或ZEROING模式。<br>&bull; algo：用于配置Subnormal模式。<br>&nbsp;&nbsp;&bull; Log2Algo::INTRINSIC、Log2Algo::PRECISION_1ULP_FTZ_TRUE，所有Subnormal被近似为0。<br>&nbsp;&nbsp;&bull; Log2Algo::PRECISION_1ULP_FTZ_FALSE，支持Subnormal数据计算。 |
| U | 目的操作数的RegTensor类型，例如RegTensor&lt;half&gt;，由编译器自动推导，用户不需要填写。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstReg | 输出 | 目的操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| srcReg | 输入 | 源操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| mask | 输入 | 源操作数元素操作的有效指示，详细说明请参考[MaskReg](../寄存器数据类型/MaskReg.md)。 |

## 返回值说明

无

## 数据类型

**表 3**  数据类型组合

| srcReg | dstReg |
| --- | --- |
| half | half |
| float | float |

## 约束说明

- 支持操作数重叠。
- src输入为-0时，输出-inf。

## 理论性能说明

该接口为软仿实现，不提供理论性能。

## 关键特性说明

**精度提升**

当源操作数数据类型为half时，将half提升精度到float后进行计算，在将结果转为half后输出。

**最大精度误差**

- Log2Algo::INTRINSIC、Log2Algo::PRECISION_1ULP_FTZ_TRUE，最大精度误差为1ULP。
- Log2Algo::PRECISION_1ULP_FTZ_FALSE，最大精度误差为1ULP。

**配置Subnormal模式**

- Log2Algo::INTRINSIC、Log2Algo::PRECISION_1ULP_FTZ_TRUE，不支持Subnormal计算，所有Subnormal被近似为0。
- Log2Algo::PRECISION_1ULP_FTZ_FALSE，最大精度误差为1ULP，支持Subnormal计算。

## 调用示例


```cpp
template<typename T>
__simd_vf__ inline void Log2VF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count,
uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    // Subnormal模式
    // static constexpr AscendC::Reg::Log2SpecificMode mode = {MaskMergeMode::ZEROING, Log2Algo::PRECISION_1ULP_FTZ_FALSE};
    for (uint16_t i = 0; i < repeatTimes; i++) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Log2(dstReg, srcReg, mask);
        // Subnormal模式
        // AscendC::Reg::Log2<T, &mode>(dstReg, srcReg, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}
```

