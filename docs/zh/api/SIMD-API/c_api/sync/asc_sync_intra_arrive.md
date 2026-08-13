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

头文件路径：`"c_api/sync/sync.h"`。

块内（intra-block）跨核同步投递原语，用于在mix-mode block内部建立CubeCore与VecCore之间的流水同步依赖。本接口在 `pipe` 指定流水的前序指令全部提交后执行，将对端核对应同步ID上的4位计数器加1，且不阻塞本流水线的后续指令。

本接口为非阻塞语义的投递操作——对端核在对应计数器为0时阻塞其所在流水，直到本接口使该计数器变为非零后将其减1并放行，从而构成一对跨核"投递-等待"同步闭环。在mix-mode block内建立CubeCore与VecCore之间的跨核流水同步依赖时，需与 `asc_sync_intra_wait` 配合使用：本接口负责投递计数，配对的等待接口负责在对端核消费计数。

## 函数原型

```c
__aicore__ inline void asc_sync_intra_arrive(pipe_t pipe,
                                             uint8_t sync_id)
__aicore__ inline void asc_sync_intra_arrive(pipe_t pipe,
                                             uint64_t sync_id)
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

__global__ __mix__(1, 2) void asc_sync_intra_kernel(__gm__ float* output)
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
    asc_sync_intra_kernel<<<1, 0>>>(output_device);
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
