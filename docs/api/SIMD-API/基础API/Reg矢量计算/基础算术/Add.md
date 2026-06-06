# Add

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

头文件路径：`"basic_api/reg_compute/kernel_reg_compute_vec_binary_intf.h"`。

该接口根据mask，对源操作数srcReg0、srcReg1进行按元素求和操作，将结果写入目的操作数dstReg，计算公式如下：

$$dstReg_i = srcReg0_i + srcReg1_i$$

同时可以在carry（MaskReg寄存器）中标记每次加法是否产生进位，若srcReg0，srcReg1输入按位相加后最高位有进位，在MaskReg carry中对应位置每4bit的最低位写1，否则写0。

![](figures/流水任务运行示意图-(1).png)

输出carry适用场景请参考[AddC功能说明](AddC.md#zh-cn_topic_0000002565603904_section103431213193917)。

## 函数原型

-   计算结果不保留进位

    ```cpp
    template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
    __simd_callee__ inline void Add(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
    ```

-   计算结果保留进位

    ```cpp
    template <typename T = DefaultType, typename U>
    __simd_callee__ inline void Add(MaskReg& carry, U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
    ```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |
| mode | [MaskMergeMode](../数据类型/MaskMergeMode.md)，选择MERGING模式或ZEROING模式。<br>&bull; ZEROING, mask未筛选的元素在dstReg中置零。<br>&bull; MERGING, mask未选择的元素对应dstReg元素中保留dstReg原值。 |
| U | 目的操作数的RegTensor类型，例如RegTensor&lt;half&gt;，由编译器自动推导，用户不需要填写。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstReg | 输出 | 目的操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| srcReg0 | 输入 | 源操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| srcReg1 | 输入 | 源操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| mask | 输入 | 源操作数元素操作的有效指示，详细说明请参考[MaskReg](../寄存器数据类型/MaskReg.md)。 |
| carry | 输出 | 目的操作数。输出借位值。<br>类型为[MaskReg](../寄存器数据类型/MaskReg.md)。 |

## 返回值说明

无

## 数据类型

**表 3**  计算结果不保留借位

| dstReg | srcReg0 | srcReg1 |
| --- | --- | --- |
| int8_t | int8_t | int8_t |
| uint8_t | uint8_t | uint8_t |
| int16_t | int16_t | int16_t |
| uint16_t | uint16_t | uint16_t |
| half | half | half |
| bfloat16_t | bfloat16_t | bfloat16_t |
| int32_t | int32_t | int32_t |
| uint32_t | uint32_t | uint32_t |
| float | float | float |
| complex32 | complex32 | complex32 |
| int64_t | int64_t | int64_t |
| uint64_t | uint64_t | uint64_t |
| complex64 | complex64 | complex64 |

**complex32、uint64_t、int64_t、complex64为软仿数据类型。**

**表 4**  计算结果保留借位，计算时按位相加

| dstReg | srcReg0 | srcReg1 |
| --- | --- | --- |
| int32_t | int32_t | int32_t |
| uint32_t | uint32_t | uint32_t |

**complex32、uint64_t、int64_t、complex64为软仿数据类型。**

## 约束说明

寄存器的非饱和/饱和模式具体配置方式参考待补充。

## 理论性能说明

**表 5**  理论并行度

| dstReg | srcReg0 | srcReg1 | 理论并行度（element / cycle） |
| --- | --- | --- | --- |
| int8_t | int8_t | int8_t | 128 |
| uint8_t | uint8_t | uint8_t | 128 |
| int16_t | int16_t | int16_t | 64 |
| uint16_t | uint16_t | uint16_t | 64 |
| half | half | half | 128 |
| bfloat16_t | bfloat16_t | bfloat16_t | 128 |
| int32_t | int32_t | int32_t | 32 |
| uint32_t | uint32_t | uint32_t | 32 |
| float | float | float | 64 |

## 调用示例


-   计算结果不保留进位

    ```cpp
    template <typename T>
    __simd_vf__ inline void AddVF(__ubuf__ T* dstAddr, __ubuf__ T* src0Addr, __ubuf__ T* src1Addr, uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
    {
        AscendC::Reg::RegTensor<T> srcReg0;
        AscendC::Reg::RegTensor<T> srcReg1;
        AscendC::Reg::RegTensor<T> dstReg;
        AscendC::Reg::MaskReg mask;
        for (uint16_t i = 0; i < repeatTimes; i++) {
            mask = AscendC::Reg::UpdateMask<T>(count);
            AscendC::Reg::LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
            AscendC::Reg::LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
            AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
            AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
        }
    }
    ```

-   计算结果保留进位

    ```cpp
    template <typename T>
    static __simd_vf__ inline void AddVF(__ubuf__ T* dst0Addr, __ubuf__ T* dst1Addr, __ubuf__ T* src0Addr, __ubuf__ T* src1Addr, uint32_t count, uint16_t repeatTimes, uint32_t oneRepeatSize){
        AscendC::Reg::RegTensor<T> srcReg0;
        AscendC::Reg::RegTensor<T> srcReg1;
        AscendC::Reg::RegTensor<T> dstReg0;
        AscendC::Reg::MaskReg mask;
        AscendC::Reg::MaskReg carry = AscendC::Reg::CreateMask<uint8_t>();
        for (uint16_t i = 0; i < repeatTimes; i++) {
            mask = AscendC::Reg::UpdateMask<T>(count);
            AscendC::Reg::LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
            AscendC::Reg::LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
            AscendC::Reg::Add(carry, dstReg0, srcReg0, srcReg1, mask);
            // 8*4B=32B align
            AscendC::Reg::StoreAlign<uint32_t, AscendC::Reg::MaskDist::DIST_NORM>(dst1Addr + i * 8, carry);
            AscendC::Reg::StoreAlign(dst0Addr + i * oneRepeatSize, dstReg0, mask);
        }
    }
    ```

