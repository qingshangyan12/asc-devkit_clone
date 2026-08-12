# asc_sync_block_arrive

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

在分离模式下，AIC（Cube Core）与AIV（Vector Core）按group划分，一个group由1个block（主核）和N个subblock（从核）组成，比例1:N。组内block和subblock间同步指同一group内block（AIC）与subblock（AIV）之间的同步。

和[asc_sync_block_wait](asc_sync_block_wait.md)配合使用（通过flag_id关联），用于组内block和subblock间的信号同步。通过向核间同步寄存器发送同步信号，标识当前pipeline已到达flag_id对应的同步点。每个flag_id对应一个初始值为0的计数器，执行asc_sync_block_arrive后flag_id对应的计数器增加1；执行asc_sync_block_wait时如果对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。

## 函数原型

```cpp
__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 设置这条指令所在的流水类型。|
| flag_id | 输入 | 事件标号，用于标识同一组同步信号。取值范围为[0,15]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 使用该同步接口时，需要按照如下规则设置Kernel类型：
  - 在纯Vector/Cube场景下，需设置Kernel类型为KERNEL_TYPE_MIX_AIV_1_0或KERNEL_TYPE_MIX_AIC_1_0。
  - 对于Vector和Cube混合场景，需根据实际情况灵活配置Kernel类型。
- 不建议开发者同时使用该接口和Matmul高阶API，否则会有flag_id冲突的风险。
- 同一flag_id的计数器最多设置15次。
- pipe支持的流水类型为PIPE_V、PIPE_M、PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX，不支持PIPE_ALL和PIPE_S。
- flag_id的值超出取值范围时，会截取最低4位为准。
- 一个核连续发出的asc_sync_block_arrive，硬件不保证执行顺序。

## 调用示例

本示例采用`__mix__(1, 2)`声明核函数，同一group内包含1个AIC和2个AIV。AIV0和AIV1分别在UB中构造矩阵A和矩阵B，并将矩阵搬运至共享L1 Buffer，然后在MTE3流水上调用`asc_sync_block_arrive`。AIC在MTE1流水上调用`asc_sync_block_wait`，等待两个AIV均到达同步点后，再读取L1 Buffer中的数据执行矩阵乘。

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
constexpr int64_t FLAG_ID = 8;

__global__ __mix__(1, 2) void AscSyncBlockKernel(__gm__ float* output)
{
    asc_init();
    __cbuf__ half matrix_a_l1[MATRIX_ELEMENTS], matrix_b_l1[MATRIX_ELEMENTS];
    __ubuf__ half input_ub[MATRIX_ELEMENTS];
    __ca__ half matrix_a_l0[MATRIX_ELEMENTS];
    __cb__ half matrix_b_l0[MATRIX_ELEMENTS];
    __cc__ float output_l0[MATRIX_ELEMENTS];

    if ASC_IS_AIV {
        for (uint32_t i = 0; i < MATRIX_ELEMENTS; ++i) {
            input_ub[i] = static_cast<half>(1.0f);
        }
        asc_sync_notify(PIPE_S, PIPE_MTE3, EVENT_ID0);
        asc_sync_wait(PIPE_S, PIPE_MTE3, EVENT_ID0);
        if (asc_get_sub_block_id() == 0) {
            asc_copy_ub2l1(matrix_a_l1, input_ub, MATRIX_BYTES);
        } else {
            asc_copy_ub2l1(matrix_b_l1, input_ub, MATRIX_BYTES);
        }
        asc_sync_block_arrive(PIPE_MTE3, FLAG_ID);
    }

    if ASC_IS_AIC {
        asc_sync_block_wait(PIPE_MTE1, FLAG_ID);
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
    AscSyncBlockKernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    bool passed = true;
    for (float value : output) {
        passed = passed && (value == static_cast<float>(MATRIX_DIM));
    }
    std::cout << (passed ? "[Success] asc_sync_block_arrive passed."
                          : "[Failed] asc_sync_block_arrive failed.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
```
