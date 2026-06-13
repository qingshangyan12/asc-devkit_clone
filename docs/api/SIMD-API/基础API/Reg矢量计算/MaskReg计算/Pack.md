# Pack<a name="ZH-CN_TOPIC_0000001952378504"></a>

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

根据所选的低位模式或高位模式，将源操作数src的偶数位bit，提取到目的操作数dst的低半部分或高半部分。

![Pack示意图](../../../../figures/reg_pack.png)

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <HighLowPart part = HighLowPart::LOWEST> 
__simd_callee__ inline void Pack(MaskReg& dst, MaskReg& src)
```

## 参数说明<a name="section622mcpsimp"></a>

**表1** 模板参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| part | 输入 | 枚举类型，低位模式或高位模式。<br>&bull; LOWEST低位模式；<br>&bull; HIGHEST高位模式。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。 |
| src | 输入 | 源操作数。 |

## 数据类型

支持的数据类型为：MaskReg。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section932512912207"></a>

```cpp
template <typename T>
__simd_vf__ inline void PackVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::MaskReg maskFull = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALL>();
    AscendC::Reg::MaskReg mask0;
    AscendC::Reg::MaskReg mask1;
    AscendC::Reg::Pack<AscendC::Reg::HighLowPart::LOWEST>(mask0, maskFull);
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask1 = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Adds(srcReg, srcReg, 0, mask0);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, srcReg, mask1);
    }
}
```
