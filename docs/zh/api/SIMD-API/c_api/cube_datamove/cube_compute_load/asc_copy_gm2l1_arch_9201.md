# asc_copy_gm2l1_arch_9201

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR/Ascend 960DT：支持
<!-- end id1 -->
<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id2 -->
<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id4 -->
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->

## 功能说明

头文件路径为：`"c_api/cube_datamove/cube_datamove.h"`。

将数据从Global Memory搬运到L1 Buffer。接口支持按连续数据块搬运，也支持以512字节分形为单位的2D矩阵搬运。2D矩阵搬运根据源矩阵行列方向的起始位置和步长切分分形矩阵，搬运过程中不进行分形格式转换。

本接口仅在AIC上执行有效。

## 函数原型

### 连续数据块搬运

```cpp
__aicore__ inline void asc_copy_gm2l1(__cbuf__ void* dst,
                                      __gm__ void* src,
                                      uint32_t burst_count,
                                      uint32_t burst_len,
                                      asc_channel_pad_mode pad_mode,
                                      bool enable_pre_load_ignore_sync,
                                      uint64_t src_stride,
                                      uint32_t dst_stride)
```

### 2D矩阵搬运

```cpp
__aicore__ inline void asc_copy_gm2l1(__cbuf__ <dtype>* dst,
                                      __gm__ <dtype>* src,
                                      uint32_t m_start_position,
                                      uint32_t k_start_position,
                                      uint16_t dst_stride,
                                      uint16_t m_step,
                                      uint16_t k_step,
                                      bool enable_pre_load_ignore_sync,
                                      asc_load_l2_cache_mode l2_cache_mode)
```

`dtype`支持的数据类型为`int8_t`、`uint8_t`、`fp4x2_e1m2_t`、`fp4x2_e2m1_t`、`hifloat8_t`、`fp8_e4m3fn_t`、`fp8_e5m2_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数，存储位置为L1 Buffer。起始地址需32字节对齐。 |
| src | 输入 | 源操作数，存储位置为GM。起始地址需1字节对齐。 |
| enable_pre_load_ignore_sync | 输入 | 是否开启预加载忽略同步功能。`false`表示关闭；`true`表示开启。 |
| burst_count | 输入 | 连续数据块搬运重载的搬运数据块个数。 |
| burst_len | 输入 | 连续数据块搬运重载的每个数据块长度，单位为32字节。 |
| pad_mode | 输入 | 连续数据块搬运重载的通道填充模式，类型为`asc_channel_pad_mode`。 |
| src_stride | 输入 | 连续数据块搬运重载中源端相邻数据块起始地址之间的间隔，单位为字节。 |
| dst_stride | 输入 | 连续数据块搬运重载中目的端相邻数据块起始地址之间的间隔，单位为32字节；2D矩阵搬运重载中目的矩阵列方向相邻分形起始地址之间的间隔，单位为512字节。 |
| m_start_position | 输入 | 源矩阵中首个分形在行方向的起始位置，单位为16个元素。取值范围：$[0, 2^{32}-1]$。 |
| k_start_position | 输入 | 源矩阵中首个分形在列方向的起始位置，单位为32字节。取值范围：$[0, 2^{32}-1]$。 |
| m_step | 输入 | 源矩阵行方向搬运的分形行数，单位为16个元素。取值范围：[1, 4095]。 |
| k_step | 输入 | 源矩阵列方向搬运的分形列数，单位为32字节。取值范围：[1, 4095]。 |
| l2_cache_mode | 输入 | 配置源端GM数据在L2 Cache中的管理策略，类型为[asc_load_l2_cache_mode](../../defs/enum/asc_load_l2_cache_mode.md)，具体取值及含义请参见该枚举说明。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 本接口非AIC调用直接返回。
- `dst`起始地址需32字节对齐，`src`起始地址需1字节对齐。
- 使用连续数据块搬运重载时，搬运参数和步长需满足硬件指令取值范围。
- 使用2D矩阵搬运重载时，以512字节完整分形为最小搬运粒度，不支持搬运分形内的部分元素。
- `m_step`或`k_step`为0时不执行搬运，本接口被视为NOP（空操作）。
- `dst_stride`、`m_step`和`k_step`超出取值范围时，高位会被截断，搬运结果可能不符合预期。
- 开启`enable_pre_load_ignore_sync`前，需确保该搬运与前序访问相同地址的指令不存在数据依赖，否则可能产生异常数据。
- `dst`偏移量与搬运大小之和不可超出L1 Buffer容量。

## 调用示例

以下示例使用2D矩阵搬运重载，将GM中的一个分形搬运到L1 Buffer，并显式关闭预加载忽略同步功能。

```cpp
#include "c_api/asc_simd.h"

__global__ __cube__ void CopyGm2l1Kernel(__gm__ half* src)
{
    asc_init();
    __cbuf__ half dst[256];

    // 从GM起始位置搬运一个512字节分形到L1 Buffer。
    asc_copy_gm2l1(dst, src, 0, 0, 1, 1, 1, false,
        asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM);
    asc_sync_pipe(PIPE_MTE2);
}
```
