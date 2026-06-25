# GetSpr<a name="ZH-CN_TOPIC_0000002537557502"></a>

## 产品支持情况<a name="section364964311912"></a>

| 产品 | 是否支持 |
|------|:------:|
| Ascend 950PR/Ascend 950DT | √ |

## 功能说明<a name="section11971757181915"></a>

头文件路径为：`"basic\_api/kernel\_operator\_sys\_var\_intf.h"`。

获取指定特殊寄存器的值。当前支持[表SpecialPurposeReg模板参数说明](#table37531617424)。

## 函数原型<a name="section113251712205"></a>

```cpp
template <SpecialPurposeReg spr>
__aicore__ inline int64_t GetSpr()
```

## 参数说明<a name="section1116018170208"></a>

**表1** 模板参数说明

| 参数名 | 描述 |
|--------|------|
| spr | 特殊寄存器，类型为SpecialPurposeReg枚举类，具体的取值请参考[表SpecialPurposeReg模板参数说明](#table37531617424)。 |

**表2** SpecialPurposeReg模板参数说明<a name="table37531617424"></a>

| 取值 | 含义 |
|------|------|
| AR | 通常配合[Squeeze](../Reg矢量计算/比较与选择/Squeeze.md)，Reg矢量计算API一起使用；[Squeeze](../Reg矢量计算/比较与选择/Squeeze.md)，Reg矢量计算API会存储有效元素的总字节数到AR特殊寄存器。 |

## 数据类型<a name="section3853852112218"></a>

接口返回数据类型为int64\_t。

## 返回值说明<a name="section16895132314202"></a>

返回int64\_t类型的特殊寄存器中的数值。

## 约束说明<a name="section162221734202016"></a>

本接口只能在VF函数外调用，命名空间为AscendC，函数标记符为\_\_aicore\_\_。

## 调用示例<a name="section849174212202"></a>

如下示例中[Squeeze](../Reg矢量计算/比较与选择/Squeeze.md)，Reg矢量计算API会存储有效元素的总字节数到AR寄存器中，宏函数结束后通过GetSpr获取AR寄存器的值（单位为字节）。

```cpp
template<typename DTYPE, typename UTYPE>
static __aicore__ inline void AddVf(__local_mem__ DTYPE* dstPtr, __local_mem__ DTYPE* src0Ptr, __local_mem__ UTYPE* src1Ptr, uint32_t count, uint16_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<DTYPE> vSrcReg0;
    AscendC::Reg::RegTensor<DTYPE> vDstReg0;
    AscendC::Reg::MaskReg maskReg0;
    AscendC::Reg::MaskReg maskReg1;
    AscendC::Reg::MaskReg maskReg2;
    AscendC::Reg::UnalignRegForStore ureg;
    AscendC::Reg::ClearSpr<AscendC::SpecialPurposeReg::AR>();
    AscendC::Reg::LoadAlign(maskReg1, src1Ptr);
    uint64_t hoistLetVar = ((uint64_t)dstPtr);
    for (uint16_t i = 0; i < repeatTimes; i++) {
        maskReg0 = AscendC::Reg::UpdateMask<DTYPE>(count);
        AscendC::Reg::LoadAlign(vSrcReg0, src0Ptr + i * oneRepeatSize);
        AscendC::Reg::Move(maskReg2, maskReg1, maskReg0);
        AscendC::Reg::Squeeze<DTYPE, AscendC::Reg::GatherMaskMode::STORE_REG>(vDstReg0, vSrcReg0, maskReg2);
        Reg::StoreUnAlign((__ubuf__ DTYPE *&)hoistLetVar, vDstReg0, ureg);
    }
    Reg::StoreUnAlignPost((__ubuf__ DTYPE *&)hoistLetVar, ureg);
}

__aicore__ inline void Compute()
{
    LocalTensor<T> dst0Local = outQueue.AllocTensor<T>();
    LocalTensor<T> src0Local = inQueueX.DeQue<T>();
    LocalTensor<U> src1Local = inQueueX2.DeQue<U>();
    constexpr uint16_t oneRepeatSize = AscendC::GetVecLen()/sizeof(T);
    uint32_t count = 512;
    // 向上取整，计算repeat
    uint16_t repeatTimes = CeilDivision(count, oneRepeatSize);
    __local_mem__ T* dstPtr = (__local_mem__ T*)dst0Local.GetPhyAddr();
    __local_mem__ T* src0Ptr = (__local_mem__ T*)src0Local.GetPhyAddr();
    __local_mem__ U* src1Ptr = (__local_mem__ U*)src1Local.GetPhyAddr();
    asc_vf_call<AddVf<T, U>>(dstPtr, src0Ptr, src1Ptr, count, oneRepeatSize, repeatTimes);
    int64_t arNum = AscendC::GetSpr<AscendC::SpecialPurposeReg::AR>();    
    outQueue.EnQue<T>(dst0Local);
    inQueueX.FreeTensor(src0Local);
    inQueueX2.FreeTensor(src1Local);
}
```

结果示例如下：

```cpp输入512个uint8_t的数据(src0Local): [1 2 3 ... ] 
输入32个uint8_t的数据(src1Local): [1 1 1 ... ] // 数据为全1的数arNum的值为64
```
