# asc_mrgsort4

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/vector_compute/compute/vector_sort.h"`。

本接口实现将2、3或4条已经按score从大到小排列的队列合并为一条有序队列，保持按照score从大到小排序。

该接口不负责将无序的输入队列排序，输入一般是经过[asc_bitsort](asc_bitsort.md)指令处理后的输出数据。

## 函数原型

```cpp
// 占位符形式
__aicore__ inline void asc_mrgsort4(__ubuf__ <dtype>* dst,
                                    __ubuf__ <dtype>* src[ASC_C_API_MRGSORT_ELEMENT_LEN],
                                    uint8_t repeat,
                                    uint16_t element_length_0,
                                    uint16_t element_length_1,
                                    uint16_t element_length_2,
                                    uint16_t element_length_3,
                                    bool if_exhausted_suspension,
                                    uint8_t valid_bit)
```

### dtype支持数据类型

`dtype`支持的数据类型为`half`、`float`.

### 函数原型典型示例

```cpp
__aicore__ inline void asc_mrgsort4(__ubuf__ float* dst,
                                    __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN],
                                    uint8_t repeat,
                                    uint16_t element_length_0,
                                    uint16_t element_length_1,
                                    uint16_t element_length_2,
                                    uint16_t element_length_3,
                                    bool if_exhausted_suspension,
                                    uint8_t valid_bit)
```

## 参数说明

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | UB中的目的队列起始地址，要求32字节对齐。 |
| src | 输入 | 包含4条UB队列起始地址的指针数组，`ASC_C_API_MRGSORT_ELEMENT_LEN`为`4`。每个队列起始地址要求8字节对齐，数组下标0～3分别对应4条队列。 |
| repeat | 输入 | 迭代次数，取值为[1, 255]。多次迭代产生多条分别有序的结果队列，不会自动将它们合为一条。 |
| element_length_0 | 输入 | 起始地址为`src[0]`的队列的长度，单位为8字节。<br>对于有效队列，取值范围为[1, 4095]；对于无效队列，取值为0。 |
| element_length_1 | 输入 | 起始地址为`src[1]`的队列的长度，单位及范围同`element_length_0`。 |
| element_length_2 | 输入 | 起始地址为`src[2]`的队列的长度，单位及范围同`element_length_0`。 |
| element_length_3 | 输入 | 起始地址为`src[3]`的队列的长度，单位及范围同`element_length_0`。 |
| if_exhausted_suspension | 输入 | 某个队列耗尽时，控制指令是否停止执行，类型为bool，取值说明如下：<br>&nbsp;&nbsp;&bull; `false`：归并排序过程中某条有效队列耗尽后继续处理其余队列，输出所有有效队列的值，单次迭代的输出长度为有效队列长度之和。<br>&nbsp;&nbsp;&bull; `true`：归并排序过程中任意一有效队列耗尽时停止排序，只输出已经完成排序的有序前缀，不能将有效队列长度之和作为单次迭代的实际输出长度。 |
| valid_bit | 输入 | 有效队列位掩码，有效取值为`3`、`7`、`15`，含义如下：<br>&nbsp;&nbsp;&bull; `3`（`0b0011`）：有效队列为起始地址为`src[0]`和`src[1]`的队列。<br>&nbsp;&nbsp;&bull; `7`（`0b0111`）：有效队列为起始地址为`src[0]`、`src[1]`和`src[2]`的队列。<br>&nbsp;&nbsp;&bull; `15`（`0b1111`）：全部4条队列均有效。 |

## 返回值说明

无

## 约束说明

- 不支持源操作数和目的操作数地址重叠。
- dst的起始地址要求32字节对齐，src数组中的起始地址要求8字节对齐。
- 如果存在score\[i\]==score\[j\]并且i\>j，将优先选取score\[j\]排在前面。
- 每次迭代内的数据会进行排序，不同迭代间的数据不会进行排序。

<!-- npu="950" id8 -->
## 调用示例

以下示例对8条已排序的float队列进行两级归并，每条队列32条记录。第一步以`repeat = 2`产生两条128记录队列；第二步归并为一条256记录队列。Host端逐条比较score和index，验证归并顺序和索引对应关系。

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需根据实际产品型号指定对应的NPU架构，具体产品与NPU架构的映射关系请参考[\_\_NPU\_ARCH\_\_](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)。

以Ascend 950PR/Ascend 950DT产品（对应NPU架构为`dav-3510`）为例，编译运行命令如下：

```bash
bisheng example.asc -o main --npu-arch=dav-3510 && ./main
```

```cpp
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "c_api/asc_simd.h"
#include "acl/acl.h"

#define ACL_CHECK(call) do { \
    aclError error = (call); \
    if (error != ACL_SUCCESS) { \
        std::cerr << #call << " failed: " << error << std::endl; \
        std::exit(1); \
    } \
} while (0)

constexpr uint16_t QUEUE_LENGTH = 32;
constexpr uint32_t QUEUE_COUNT = 8;
constexpr uint32_t RECORD_COUNT = QUEUE_LENGTH * QUEUE_COUNT;
// index存储uint32_t位模式，不是将索引数值转换成float。
struct Record {
    float score;
    uint32_t index;
};
static_assert(sizeof(Record) == 8, "Each record must occupy 8 bytes.");
constexpr uint32_t WORD_COUNT = RECORD_COUNT * 2;
constexpr uint32_t BYTE_COUNT = RECORD_COUNT * sizeof(Record);

__global__ __vector__ void mrgsort_kernel(__gm__ float* dst, __gm__ float* src)
{
    asc_init();
    __ubuf__ float src_local[WORD_COUNT];
    __ubuf__ float tmp_local[WORD_COUNT];
    __ubuf__ float dst_local[WORD_COUNT];

    asc_copy_gm2ub_align(src_local, src, BYTE_COUNT);
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

    // float记录占2个float位置；每组4条队列，repeat=2得到两条128记录队列。
    __ubuf__ float* queues[ASC_C_API_MRGSORT_ELEMENT_LEN] = {
        src_local, src_local + 2 * QUEUE_LENGTH,
        src_local + 4 * QUEUE_LENGTH, src_local + 6 * QUEUE_LENGTH
    };
    asc_mrgsort4(tmp_local, queues, 2, QUEUE_LENGTH, QUEUE_LENGTH,
        QUEUE_LENGTH, QUEUE_LENGTH, false, 0b1111);
    // 下一次归并读取本次结果，先完成同步。
    asc_sync();

    queues[0] = tmp_local;
    queues[1] = tmp_local + 2 * (4 * QUEUE_LENGTH);
    // 无效队列也初始化为合法的UB地址，长度设为0。
    queues[2] = tmp_local;
    queues[3] = tmp_local;
    asc_mrgsort4(dst_local, queues, 1, 4 * QUEUE_LENGTH,
        4 * QUEUE_LENGTH, 0, 0, false, 0b0011);

    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_copy_ub2gm_align(dst, dst_local, BYTE_COUNT);
    asc_sync();
}

int main()
{
    std::vector<Record> input(RECORD_COUNT), output(RECORD_COUNT);
    for (uint32_t q = 0; q < QUEUE_COUNT; ++q) {
        for (uint32_t i = 0; i < QUEUE_LENGTH; ++i) {
            // 每条队列内部降序，不同队列的score相互交错。
            uint32_t index = q * QUEUE_LENGTH + i;
            input[index] = {static_cast<float>(RECORD_COUNT - i * QUEUE_COUNT - q), index};
        }
    }
    auto golden = input;
    std::stable_sort(golden.begin(), golden.end(),
        [](const Record& a, const Record& b) { return a.score > b.score; });

    ACL_CHECK(aclInit(nullptr));
    ACL_CHECK(aclrtSetDevice(0));
    float* src_device = nullptr;
    float* dst_device = nullptr;
    ACL_CHECK(aclrtMalloc(reinterpret_cast<void**>(&src_device), BYTE_COUNT,
        ACL_MEM_MALLOC_HUGE_FIRST));
    ACL_CHECK(aclrtMalloc(reinterpret_cast<void**>(&dst_device), BYTE_COUNT,
        ACL_MEM_MALLOC_HUGE_FIRST));
    ACL_CHECK(aclrtMemcpy(src_device, BYTE_COUNT, input.data(), BYTE_COUNT,
        ACL_MEMCPY_HOST_TO_DEVICE));
    mrgsort_kernel<<<1, 0>>>(dst_device, src_device);
    ACL_CHECK(aclrtSynchronizeDevice());
    ACL_CHECK(aclrtMemcpy(output.data(), BYTE_COUNT, dst_device, BYTE_COUNT,
        ACL_MEMCPY_DEVICE_TO_HOST));

    bool passed = true;
    for (uint32_t i = 0; i < RECORD_COUNT; ++i) {
        if (output[i].score != golden[i].score || output[i].index != golden[i].index) {
            std::cerr << "Mismatch at record " << i << std::endl;
            passed = false;
            break;
        }
    }
    std::cout << "First 8 (score, index):";
    for (uint32_t i = 0; i < 8; ++i) {
        std::cout << " (" << output[i].score << ", " << output[i].index << ")";
    }
    std::cout << '\n' << (passed ? "[Success] asc_mrgsort4 passed."
                                : "[Failed] asc_mrgsort4 failed.") << std::endl;
    ACL_CHECK(aclrtFree(dst_device));
    ACL_CHECK(aclrtFree(src_device));
    ACL_CHECK(aclrtResetDevice(0));
    ACL_CHECK(aclFinalize());
    return passed ? 0 : 1;
}
```

上述示例在Ascend 950PR（`dav-3510`）上的输出为：

```text
First 8 (score, index): (256, 0) (255, 32) (254, 64) (253, 96) (252, 128) (251, 160) (250, 192) (249, 224)
[Success] asc_mrgsort4 passed.
```
<!-- end id8 -->
