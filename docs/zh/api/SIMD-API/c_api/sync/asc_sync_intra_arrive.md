# asc_sync_intra_arrive

## 产品支持情况

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

## 功能说明

在分离模式下，AIC（Cube Core）与AIV（Vector Core）按group划分，一个group由1个block（主核）和N个subblock（从核）组成，比例1:N。block内同步指同一block内block（AIC）与单个subblock（AIV）之间的同步。

block内同步指令，与[asc_sync_intra_wait](asc_sync_intra_wait.md)配合使用，是block内不同Core的流水线同步指令，用于向每个Core的同步寄存器发送同步信号。每个sync_id对应Vector Core与Cube Core中的一个初始值为0的计数器。asc_sync_intra_arrive指令后其他Core对应的计数器增加1；执行asc_sync_intra_wait时如果对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。

block内同步场景可细分为两类：

- AIC执行asc_sync_intra_arrive，对应的AIV执行asc_sync_intra_wait。当AIC执行完asc_sync_intra_arrive时，AIV对应sync_id的计数器增加1。若AIV该sync_id的计数器为非0，则解除AIV的阻塞，asc_sync_intra_wait后的指令继续执行，该sync_id的计数器减1。总计AIC 1次调用asc_sync_intra_arrive，单个AIV完成1次asc_sync_intra_wait才算配对使用。

- 1个AIV执行asc_sync_intra_arrive，对应的AIC执行asc_sync_intra_wait。当AIV执行完asc_sync_intra_arrive时，AIC对应sync_id的计数器增加1。若AIC该sync_id的计数器为非0，则解除AIC的阻塞，asc_sync_intra_wait后的指令继续执行，该sync_id的计数器减1。总计单个AIV 1次asc_sync_intra_arrive，对应AIC 1次调用asc_sync_intra_wait才算配对使用。

以图1为例，演示1个AI Core中AIV1与AIC进行同步（AIV1-2发起asc_sync_intra_arrive）。

AIC 1中在执行asc_sync_intra_wait后，此时AIC 1 sync_id=0的计数器为0，后续所有指令全部被阻塞，需要等到1个AIV执行完asc_sync_intra_arrive。

- AIV 1-1不需要执行asc_sync_intra_arrive。
- AIV 1-2的PIPE_MTE3指令全部执行完毕后，asc_sync_intra_arrive生效。此时调度模块感知1个AIV已执行完asc_sync_intra_arrive，因此将AIC 1 sync_id=0的计数器值增加为1。AIC 1检测到对应的sync_id=0的计数器变为1，则AIC 1核解除阻塞，继续执行后续PIPE_FIX的指令，并且将计数器值减去1。

**图1**  block内同步时序图（AIV进行asc_sync_intra_arrive）
![](../../../figures/single_ai_core_aic_single_aiv_sync.png "block内同步时序图（AIV进行asc_sync_intra_arrive）")

## 函数原型

```cpp
__aicore__ inline void asc_sync_intra_arrive(pipe_t pipe, uint8_t sync_id)
__aicore__ inline void asc_sync_intra_arrive(pipe_t pipe, uint64_t sync_id)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 该指令的执行流水线。 |
| sync_id | 输入 | 事件标号。AIC的取值范围为[0, 31]，AIV的取值范围为[0, 15]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需要保证每一个ID的计数器不会溢出，否则会出现异常。
- 需要保证相同的ID只会被一个流水线控制。
- 每个ID对应一个计数器，特定CORE的同步ID计数器可通过其他CORE发起的asc_sync_intra_arrive递增；也可通过本身发起的asc_sync_intra_wait递减。 
- 三个CORE的ID映射规则如下：
    - ID为0~15的Vector Core 0的asc_sync_intra_arrive指令会被映射到Cube Core的0~15的ID。 
    - ID为0~15的Vector Core 1的asc_sync_intra_arrive指令会被映射到Cube Core的16~31的ID。 
    - ID为0~15的Cube Core的asc_sync_intra_arrive指令会被映射到Vector Core 0的0~15的ID。 
    - ID为16~31的Cube Core的asc_sync_intra_arrive指令会被映射到Vector Core 1的0~15的ID。 

- 必须保证配套使用asc_sync_intra_arrive和[asc_sync_intra_wait](asc_sync_intra_wait.md)，否则会出现未定义行为。
- pipe支持的流水类型为PIPE_V、PIPE_M、PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX、PIPE_S，不支持PIPE_ALL。
- 一个核连续发出的asc_sync_intra_arrive，硬件不保证执行顺序。

## 调用示例

本示例采用`__mix__(1, 2)`声明核函数。AIV0在UB中构造矩阵A和矩阵B，将矩阵搬运至共享L1 Buffer，然后在MTE3流水上调用`asc_sync_intra_arrive`。AIC在MTE1流水上调用`asc_sync_intra_wait`，等待AIV0到达同步点后，再读取L1 Buffer中的数据执行矩阵乘。AIV1不参与该次同步。

将代码保存为`examples.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md#npu-arch)。

<!-- npu="950" id8 -->
以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng examples.asc -o main --npu-arch=dav-3510; ./main
```
<!-- end id8 -->

```c
#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {

constexpr uint32_t MATRIX_DIM = 16;
constexpr uint32_t MATRIX_ELEMENTS = MATRIX_DIM * MATRIX_DIM;
constexpr uint32_t MATRIX_BYTES = MATRIX_ELEMENTS * sizeof(half);
constexpr uint32_t OUTPUT_BYTES = MATRIX_ELEMENTS * sizeof(float);
constexpr uint64_t SYNC_ID = 8;

__global__ __mix__(1, 2) void AscSyncIntraKernel(__gm__ float* output)
{
    asc_init();
    __cbuf__ half matrix_a_l1[MATRIX_ELEMENTS], matrix_b_l1[MATRIX_ELEMENTS];
    __ubuf__ half input_ub[MATRIX_ELEMENTS];
    __ca__ half matrix_a_l0[MATRIX_ELEMENTS];
    __cb__ half matrix_b_l0[MATRIX_ELEMENTS];
    __cc__ float output_l0[MATRIX_ELEMENTS];

    if ASC_IS_AIV {
        if (asc_get_sub_block_id() == 0) {
            for (uint32_t i = 0; i < MATRIX_ELEMENTS; ++i) {
                input_ub[i] = static_cast<half>(1.0f);
            }
            asc_sync_notify(PIPE_S, PIPE_MTE3, EVENT_ID0);
            asc_sync_wait(PIPE_S, PIPE_MTE3, EVENT_ID0);
            asc_copy_ub2l1(matrix_a_l1, input_ub, MATRIX_BYTES);
            asc_copy_ub2l1(matrix_b_l1, input_ub, MATRIX_BYTES);
            asc_sync_intra_arrive(PIPE_MTE3, SYNC_ID);
        }
    }

    if ASC_IS_AIC {
        asc_sync_intra_wait(PIPE_MTE1, SYNC_ID);
        asc_copy_l12l0a(matrix_a_l0, matrix_a_l1, 0, 0, 1, 1, 1, 1);
        asc_copy_l12l0b_transpose(matrix_b_l0, matrix_b_l1, 0, 0, 1, 1, 1, 1);
        asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_mmad(output_l0, matrix_a_l0, matrix_b_l0, MATRIX_DIM, MATRIX_DIM, MATRIX_DIM, 0, false, false, true);
        asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
        asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
        asc_set_l0c2gm_nz2nd(1, 2, MATRIX_ELEMENTS);
        asc_copy_l0c2gm(output, output_l0, MATRIX_DIM, MATRIX_DIM, MATRIX_DIM, MATRIX_DIM, 0, 0, 0,
                        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
                        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    }
    asc_sync_pipe(PIPE_ALL);
}

} // namespace

int main()
{
    std::vector<float> output(MATRIX_ELEMENTS, 0.0f);
    float* output_device = nullptr;

    aclInit(nullptr);
    aclrtSetDevice(0);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    AscSyncIntraKernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    bool passed = true;
    for (float value : output) {
        passed = passed && (value == static_cast<float>(MATRIX_DIM));
    }
    std::cout << (passed ? "[Success] asc_sync_intra_arrive passed."
                          : "[Failed] asc_sync_intra_arrive failed.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
