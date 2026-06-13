# MoveMask<a name="ZH-CN_TOPIC_0000001985457933"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
| --- | :---: |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter> |
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | x </cann-filter> |
| <cann-filter npu-type="310p">Atlas 推理系列产品AI Core | x </cann-filter> |
| <cann-filter npu-type="310p">Atlas 推理系列产品Vector Core | x </cann-filter> |
| <cann-filter npu-type="910">Atlas 训练系列产品 | x </cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h。

从[SetVectorMask](../../Memory矢量计算/掩码操作/SetVectorMask.md#ZH-CN_TOPIC_0000002574024783)设置的掩码寄存器{MASK1, MASK0}中读取Mask值，并按模板参数T对应的数据格式转换后写入返回值MaskReg。具体实现方式如下：

b16类型读取完整128bit的{MASK1, MASK0}，将每个bit复制为2bit；b32类型读取64bit的MASK0，并将每个bit复制为4bit。

![A5和A3mask对比](../../../../figures/reg_move_mask.png)

Mask与MaskReg的机制对比，具体请参考[A2/A3与950 Mask机制对比](../寄存器数据类型/MaskReg.md#zh-cn_topic_0000002548210505_section132959508011)。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <typename T> 
__simd_callee__ inline MaskReg MoveMask()
```

## 参数说明<a name="section622mcpsimp"></a>

**表1**  模板参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| T | 输入 | 数据类型，决定mask值写入格式：<br>&bull; 对于b16类型，会读取完整的128bit {MASK1, MASK0}数据，并将每bit复制为2bit，写入函数返回值MaskReg。<br>&bull; 对于b32类型，会读取64bit MASK0数据，并将每bit复制为4bit，写入函数返回值MaskReg。 |

## 数据类型

支持的数据类型：b16/b32。

## 返回值说明<a name="section640mcpsimp"></a>

特殊寄存器中读取的MaskReg。

## 约束说明<a name="section633mcpsimp"></a>

- 本接口为兼容性接口，建议优先采用[CreateMask接口](../寄存器数据类型/MaskReg.md#zh-cn_topic_0000002548210505_li28192393216)和[UpdateMask接口](../寄存器数据类型/MaskReg.md#zh-cn_topic_0000002548210505_li5571142593212)进行MaskReg计算。
- 使用前需要先调用[SetMaskCount](../../Memory矢量计算/掩码操作/SetMaskCount.md#ZH-CN_TOPIC_0000002543264880)/[SetMaskNorm](../../Memory矢量计算/掩码操作/SetMaskNorm.md#ZH-CN_TOPIC_0000002573904805)手动设置mask模式，并调用[SetVectorMask](../../Memory矢量计算/掩码操作/SetVectorMask.md#ZH-CN_TOPIC_0000002574024783)设置掩码寄存器SPR{MASK1, MASK0}。

## 调用示例<a name="section932512912207"></a>

```cpp
template <typename T>
__simd_vf__ inline void MoveMaskVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::MaskReg addMask = AscendC::Reg::MoveMask<T>();
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Adds(srcReg, srcReg, 0, addMask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, srcReg, mask);
    }
}
```
