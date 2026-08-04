# overview
reg_vector_computeAPI是面向reg_vector_compute架构开发的API，用户可以通过该API直接对芯片中涉及Vector计算的寄存器进行操作，实现更大的灵活性和更好的性能。reg_vector_computeAPI与memory_vector_computeAPI功能相似，但与memory_vector_computeAPI输入和输出数据必须为LocalTensor不同，reg_vector_computeAPI的输入或输出数据均为reg_vector_compute寄存器。对于计算类API，其功能是从给定的寄存器获取数据，进行计算，并将结果保存在给定的寄存器。对于搬运类API，其功能是实现UB和寄存器的data_move。由此可见，reg_vector_computeAPI相较于memory_vector_computeAPI，将data_move和Reg计算过程交给用户自主控制，从而实现更大的开发自由度。

## 如何使用reg_vector_computeAPI
基于寄存器的programming_model是指将数据从LocalTensor通过Reg搬运指令（reg_data_load）加载到寄存器中，进行复杂的math_computeCompute后通过Reg搬运指令（reg_data_store）搬出到LocalTensor中，所有的计算逻辑均在寄存器中完成，从而减少LocalTensor间的data_move，大大提升了整体性能，具体流程如下所示：

**图1**  reg_vector_compute  
![](../../../figures/overviewreg_vector_compute.png "reg_vector_compute")

以[reg_vector_compute编程示例](#reg矢量计算编程示例)为例，完整的VectorFunction计算过程由以下几部分组成：
- 编写和调用VectorFunction（向量函数）。这些函数使用__simd_vf__标记，通过[asc\_vf\_call](./vf_call/asc_vf_call.md)被调用；
- 定义矢量数据寄存器[RegTensor](./register_data_types/RegTensor.md)、mask寄存器[MaskReg](./register_data_types/MaskReg.md)；
- 编写for循环映射到硬件循环（[HardwareLoop](./key_features/key_features.md#硬件循环hardwareloop)）；[UpdateMask](./register_data_types/MaskReg.md#mask设置方式)函数用于更新参与计算的mask，每次循环都会消耗一个VL长度的元素。
- 循环内调用reg_data_load接口连续对齐搬入（[LoadAlign](./reg_data_load/LoadAlign_continuous.md)）从UB中搬入单个VL长度数据；
- 循环内调用Reg计算接口Add完成单次Repeat计算；
- 循环内调用reg_data_store接口连续对齐搬出（[StoreAlign](./reg_data_store/StoreAlign_continuous.md)）往UB中搬出计算后的数据；

## reg_vector_compute编程示例

```cpp
template <typename T>
__simd_vf__ inline void AddVF(__ubuf__ T* dstAddr, __ubuf__ T* src0Addr, __ubuf__ T* src1Addr, uint32_t count, uint16_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg0;
    AscendC::Reg::RegTensor<T> srcReg1;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
        AscendC::Reg::Add(dstReg, srcReg0, srcReg1, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}

template <typename T>
__aicore__ inline void Compute()
{
    // GMdata_move至UB地址src0Addr及src1Addr
    ...
    // 调用vf函数
    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr, count, oneRepeatSize, repeatTimes);
    // UB地址dstAddrdata_move至GM
    ...
}
```

>[!NOTE]说明 
>- GM与UB间的data_move需通过[GM_UB_data_move](../memory_vector_compute/data_move/data_move.md)完成；
>- Vector Function的流水类型为PIPE_V，Vector Function内部的同步依赖通过[LocalMemBar](./sync_control/LocalMemBar.md)接口插入。
