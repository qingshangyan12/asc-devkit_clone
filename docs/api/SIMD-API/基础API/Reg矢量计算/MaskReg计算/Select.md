# Select<a name="ZH-CN_TOPIC_0000001985457929"></a>

## 产品支持情况<a name="section1550532418810"></a>

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

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h。

给定两个源操作数src0和src1，根据mask的比特位值选取元素，得到目的操作数dst。选择的规则为：当mask的比特位是1时，从src0中选取对应位置的数，比特位是0时从src1选取对应位置的数。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
__simd_callee__ inline void Select(MaskReg& dst, MaskReg& src0, MaskReg& src1, MaskReg& mask);
```

## 参数说明<a name="section622mcpsimp"></a>

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。 |
| src0 | 输入 | 源操作数。 |
| src1 | 输入 | 源操作数。 |
| mask | 输入 | 指示选择src0或src1。 |

## 数据类型

支持的数据类型为：MaskReg。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section932512912207"></a>

```cpp
template <typename T>
__simd_vf__ inline void SelectVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::MaskReg maskFull = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALL>();
    AscendC::Reg::MaskReg maskNone = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALLF>();
    AscendC::Reg::MaskReg newMask;
    AscendC::Reg::MaskReg mask;
    AscendC::Reg::Select(newMask, maskFull, maskNone, maskFull);
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Adds(srcReg, srcReg, 0, newMask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, srcReg, mask);
    }
}
```
