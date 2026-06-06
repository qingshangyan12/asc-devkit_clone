# Log

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

头文件路径：`"basic_api/reg_compute/kernel_reg_compute_vec_unary_intf.h"`。

该接口根据mask逐元素对源操作数求自然对数，将结果写入目的操作数。计算公式如下：

$$dstReg_i = \log(srcReg_i)$$

## 函数原型

```cpp
template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void Log(U& dstReg, U& srcReg, MaskReg& mask)
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |
| mode | 可配置为[MaskMergeMode](../数据类型/MaskMergeMode.md)的枚举或LogSpecificMode的结构体指针。<br>&bull; MaskMergeMode，用于选择MERGING模式或ZEROING模式。<br>&nbsp;&nbsp;&bull; ZEROING，mask未筛选的元素在dstReg中置零。<br>&nbsp;&nbsp;&bull; MERGING，暂不支持。<br>&bull; LogSpecificMode，定义如下：<pre><code>enum class LogAlgo {<br>    INTRINSIC = 0,<br>    PRECISION_1ULP_FTZ_TRUE,<br>    PRECISION_1ULP_FTZ_FALSE,<br>};<br>struct LogSpecificMode{<br>    MaskMergeMode mrgMode = MaskMergeMode::ZEROING,<br>    LogAlgo algo = LogAlgo::INTRINSIC;<br>};</code></pre>&bull; mrgMode：选择MERGING模式或ZEROING模式。<br>&bull; algo：用于配置Subnormal模式。<br>&nbsp;&nbsp;&bull; LogAlgo::INTRINSIC、LogAlgo::PRECISION_1ULP_FTZ_TRUE，使用单指令计算得出结果，最大精度误差为1ULP，目前，该算法支持half、float数据类型。<br>&nbsp;&nbsp;&bull; LogAlgo::PRECISION_1ULP_FTZ_FALSE，软仿实现，支持Subnormal数据计算，最大精度误差为1ULP，目前，该算法支持half、float数据类型。 |
| U | 源操作数和目的操作数的RegTensor类型，例如RegTensor&lt;half&gt;，由编译器自动推导，用户不需要填写。 |

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
- 本接口不涉及地址对齐。
- src输入为-0时，输出-inf。

## 理论性能说明

**表 4**  LogAlgo::INTRINSIC、LogAlgo::PRECISION_1ULP_FTZ_TRUE下理论性能

| srcReg | dstReg | 理论并行度(element/cycle) |
| --- | --- | --- |
| half | half | 16 |
| float | float | 16 |

**LogAlgo::PRECISION_1ULP_FTZ_FALSE为软仿实现。**

## 关键特性说明

**最大精度误差**

- LogAlgo::INTRINSIC、LogAlgo::PRECISION_1ULP_FTZ_TRUE，最大精度误差为1ULP。
- LogAlgo::PRECISION_1ULP_FTZ_FALSE，软仿实现，最大精度误差为1ULP。

**配置Subnormal模式**

- LogAlgo::INTRINSIC、LogAlgo::PRECISION_1ULP_FTZ_TRUE，不支持Subnormal计算，所有Subnormal被近似为0。
- LogAlgo::PRECISION_1ULP_FTZ_FALSE，软仿实现，最大精度误差为1ULP，支持Subnormal计算。

## 调用示例

完整调用示例待补充。

```cpp
template<typename T>
__simd_vf__ inline void LogVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count,
 uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    // Subnormal模式
    // static constexpr AscendC::Reg::LogSpecificMode mode = {MaskMergeMode::ZEROING, LogAlgo::PRECISION_1ULP_FTZ_FALSE};
    for (uint16_t i = 0; i < repeatTimes; i++) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Log(dstReg, srcReg, mask);
        // Subnormal模式
        // AscendC::Reg::Log<DTYPE, &mode>(dstReg, srcReg, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}
```

