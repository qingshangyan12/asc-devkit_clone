# Overview

<!-- md-trans-meta sourceCommit=7ef8ad7bdbdb0cef0f561d391759ecb8b4e4043a translatedAt=2026-09-23T15:31:06.080Z pushedAt=2026-10-26T07:41:44.424Z nl=2 -->

The **Reg vector computation API** is developed for the Reg vector computation architecture. With this API, users can directly operate the registers involved in vector computation on the chip, achieving greater flexibility and better performance. The **Reg vector computation API** is functionally similar to the **Memory vector computation API**. However, unlike the **Memory vector computation API**, whose input and output data must be **LocalTensor**, the input or output data of the **Reg vector computation API** are all Reg vector computation registers. For computation APIs, the function is to obtain data from the given registers, perform computation, and save the result in the given registers. For data movement APIs, the function is to transfer data between the **Unified Buffer** (UB) and the registers. Therefore, compared with the **Memory vector computation API**, the **Reg vector computation API** hands over the data transfer and Reg computation processes to users for autonomous control, thereby achieving greater development freedom.

## How to Use the Reg Vector Computation API
The register-based programming model loads data from a LocalTensor into registers through Reg data movement instructions (Reg data load-in), performs complex mathematical computation (Compute), and then moves the data out to the LocalTensor through Reg data movement instructions (Reg data load-out). All computation logic is completed in registers, which reduces data movement between LocalTensors and greatly improves overall performance. The specific process is as follows:

**Figure 1**  Reg vector computation  
![](../../../figures/reg_vector_overview.png "Reg vector computation")

Taking the [Reg vector computation programming example](#reg-vector-computation-programming-example) as an example, the complete VectorFunction computation process consists of the following parts:
- Write and call VectorFunctions. These functions are marked with __simd_vf__ and are called through [asc\_vf\_call](./vf_call/asc_vf_call.md);
- Define the vector data register [RegTensor](./register_data_types/RegTensor.md) and the mask register [MaskReg](./register_data_types/MaskReg.md);
- Write a for loop mapped to a hardware loop ([HardwareLoop](./key_features/key_features.md#hardware-loop-hardwareloop)); the [UpdateMask](./register_data_types/MaskReg.md#mask-setting-method) function is used to update the mask involved in the computation, and each loop consumes one VL-length element.
- Call the Reg data load-in API in the loop for continuous aligned load-in ([LoadAlign](./reg_data_load/LoadAlign_continuous.md)) to load a single VL-length of data from the UB;
- Within the loop, call the Reg computation API Add to complete a single Repeat computation;
- In the loop, call the Reg data store-out API for continuous aligned store-out ([StoreAlign](./reg_data_store/StoreAlign_continuous.md)) to move the computed data out to the UB;

## Reg Vector Computation Programming Example

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
    // Move GM data to UB addresses src0Addr and src1Addr.
    ...
    // Call the vf function.
    asc_vf_call<AddVF<T>>(dstAddr, src0Addr, src1Addr, count, oneRepeatSize, repeatTimes);
    // Move data from UB address dstAddr to GM.
    ...
}
```

>[!NOTE]Note 
>- Data movement between GM and UB must be completed through [GM and UB Data Movement](../memory_vector_compute/data_move/data_move.md).
>- The pipeline type of the Vector Function is PIPE_V. The synchronization dependency inside the Vector Function is inserted through the [LocalMemBar](./sync_control/LocalMemBar.md) API.
