# asc_set_atomic_min

<!-- md-trans-meta sourceCommit=c891f0c5d5b2ac25c5683ff40810fa381d654ece translatedAt=2026-10-20T11:39:57.539Z pushedAt=2026-10-28T08:52:01.600Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/atomic/datamove_atomic.h"`.

Compares the data to be moved to GM with the existing data on GM, and then writes the smaller of the two to GM. This API enables atomic compare-and-min for subsequent data move instructions whose destination address is GM. For the data move paths supported by different products, see [Constraints](#constraints).

The API provides different function prototypes for setting different data types for compare-and-min.

## Prototype

```c
__aicore__ inline void asc_set_atomic_min_int8()
__aicore__ inline void asc_set_atomic_min_int16()
__aicore__ inline void asc_set_atomic_min_float16()
__aicore__ inline void asc_set_atomic_min_bfloat()
__aicore__ inline void asc_set_atomic_min_int32()
__aicore__ inline void asc_set_atomic_min_float()
```

## Parameters

None

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- Due to different hardware architectures, the supported data paths vary by product, as follows:
    <!-- npu="950" id11 -->
    - For Ascend 950PR&950DT products, the supported data path is UB/L0C Buffer->GM.
    <!-- end id11 -->
    <!-- npu="A3" id8 -->
    - For Atlas A3 products, the supported data path is UB/L0C Buffer/L1 Buffer->GM.
    <!-- end id8 -->
    <!-- npu="910b" id9 -->
    - For Atlas A2 products, the supported data path is UB/L0C Buffer/L1 Buffer->GM.
    <!-- end id9 -->
- After this API is called, atomic operations are enabled for all subsequent move instructions whose destination address is GM. You can call [asc_disable_dma_atomic](asc_disable_dma_atomic.md) to disable the atomic operations.
- This API does not automatically zero the existing data on GM before execution. If you expect the original data on GM to be zero before the atomic compare-and-min, you must clear it manually.
- This API takes effect only on subsequent move instructions whose destination address is GM (move through the MTE1/MTE2/MTE3 units). It does not take effect on instructions that write to GM through scalar operations, such as [asc_store_dev](../../scalar_compute/scalar_store/asc_store_dev.md).
- The synchronization between this API and the immediately following move instruction is guaranteed by hardware. Therefore, the multi-pipeline synchronization inserted in the following example is unnecessary:

    ```c
    asc_set_atomic_min_int8();

    /*
    The synchronization between asc_set_atomic_min_int8 and asc_copy_ub2gm is guaranteed by hardware. Therefore, the following synchronization is unnecessary.
    asc_sync_notify(PIPE_S, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_S, PIPE_MTE3, EVENT_ID0);
    */

    asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
    // Disable the atomic operation.
    asc_disable_dma_atomic();
    ```

- The operation data type of the subsequent move instructions must be consistent with the data type set by the selected API.
- This API cannot guarantee the execution order of subsequent move instructions. To ensure a deterministic execution order, see [Key Feature Description](../key_features.md).

## Examples

After saving the code as `example.asc`, you can compile and run it using the `bisheng` command. The `--npu-arch` parameter must be set to the NPU architecture corresponding to the actual product model. For the mapping between products and NPU architectures, see [\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch).

<!-- npu="950" id10 -->
Taking the Ascend 950PR&950DT products (whose corresponding NPU architecture is `dav-3510`) as an example, the compile and run command is as follows:

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```
<!-- end id10 -->

```cpp
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {
constexpr uint32_t ELEMENTS = 64;
constexpr uint32_t BYTES = ELEMENTS * sizeof(float);

template <typename T>
void print_data(const char* label, const std::vector<T>& data)
{
    std::cout << label << ":";
    const size_t count = data.size() < 8 ? data.size() : 8;
    for (size_t i = 0; i < count; ++i) std::cout << ' ' << +data[i];
    if (data.size() > count) std::cout << " ...";
    std::cout << std::endl;
}

__global__ __vector__ void asc_set_atomic_min_kernel(__gm__ float* output, __gm__ float* input0, __gm__ float* input1)
{
    asc_init();
    __ubuf__ float local0[ELEMENTS];
    __ubuf__ float local1[ELEMENTS];
    asc_copy_gm2ub_align(local0, input0, BYTES);
    asc_copy_gm2ub_align(local1, input1, BYTES);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm(output, local0, BYTES);
    asc_set_atomic_min_float();
    asc_sync_pipe(PIPE_MTE3);
    asc_copy_ub2gm(output, local1, BYTES);
    asc_disable_dma_atomic();
    asc_sync();
}

}

int main()
{
    std::vector<float> input0(ELEMENTS), input1(ELEMENTS), output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t i = 0; i < ELEMENTS; ++i) {
        input0[i] = static_cast<float>(i % 8 + 1);
        input1[i] = 2.0f;
        golden[i] = input0[i] < input1[i] ? input0[i] : input1[i];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    float *input0_device = nullptr, *input1_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input0_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&input1_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input0_device, BYTES, input0.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(input1_device, BYTES, input1.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    asc_set_atomic_min_kernel<<<1, 0>>>(output_device, input0_device, input1_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    print_data("Input0", input0);
    print_data("Input1", input1);
    print_data("Output", output);
    print_data("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_atomic_min passed." : "[Failed] asc_set_atomic_min failed.") << std::endl;
    aclrtFree(input0_device);
    aclrtFree(input1_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
