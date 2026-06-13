# Move<a name="ZH-CN_TOPIC_0000001985577785"></a>

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

将src中的元素复制到dst中的对应位置。如果有输入mask，则仅复制被mask选定的有效元素，无效元素填0。

## 函数原型<a name="section620mcpsimp"></a>

- 有输入mask

    ```cpp
    __simd_callee__ inline void Move(MaskReg& dst, MaskReg& src, MaskReg& mask)
    ```

- 无输入mask

    ```cpp
    __simd_callee__ inline void Move(MaskReg& dst, MaskReg& src)
    ```

## 参数说明<a name="section622mcpsimp"></a>

**表 1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。 |
| src | 输入 | 源操作数。 |
| mask | 输入 | 指示在复制过程中哪些bit有效。 |

## 数据类型

支持的数据类型为：MaskReg。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section932512912207"></a>

```cpp
template <typename T>
__simd_vf__ inline void MoveVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::MaskReg src = AscendC::Reg::CreateMask<T, AscendC::Reg::MaskPattern::ALLF>();
    AscendC::Reg::MaskReg dst;
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Move(dst, src, mask);
        AscendC::Reg::Adds(srcReg, srcReg, 0, dst);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, srcReg, mask);
    }
}
```
