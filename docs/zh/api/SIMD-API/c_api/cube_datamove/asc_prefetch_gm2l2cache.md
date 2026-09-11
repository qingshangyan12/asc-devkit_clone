# asc_prefetch_gm2l2cache

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

将Global Memory中的数据预加载到L2 Cache中，不实际搬进内核（L1 Buffer/Unified Buffer）。预加载的数据后续被核间指令访问时可命中L2 Cache，从而降低访问延迟。

## 函数原型

```c
__aicore__ inline void asc_prefetch_gm2l2cache(__gm__ <dtype>* src,
                                                uint32_t burst_num,
                                                uint32_t burst_len,
                                                uint64_t burst_src_stride)
```

### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：half类型数据的burst模式预取。
__aicore__ inline void asc_prefetch_gm2l2cache(__gm__ half* src,
                                                uint32_t burst_num,
                                                uint32_t burst_len,
                                                uint64_t burst_src_stride)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 含义 |
| --- | --- | --- |
| src | 输入 | 源操作数，存储位置为GM。起始地址需要按照32字节对齐。 |
| burst_num | 输入 | 预取的连续传输数据块个数。 |
| burst_len | 输入 | 预取的每个连续传输数据块的长度，单位为字节。 |
| burst_src_stride | 输入 | 源操作数相邻连续数据块的距离（前面一个数据块的头与后面一个数据块的头的间隔），单位为字节。<br>只预取1个数据块，即burst_num=1时，可以将此参数设置为0。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 本接口在AIC和AIV上均执行有效。
- 本接口仅将GM数据预取到L2 Cache，不写入L1 Buffer、Unified Buffer等片上存储单元，也不改变GM中的数据内容。

## 调用示例

```cpp
// 设置源操作数，A矩阵为128×128的half类型矩阵，在GM中按行连续存放
constexpr uint32_t M = 128;
constexpr uint32_t K = 128;
__gm__ half* a;

// 预加载过程中的配置：每行为1个连续传输数据块（128×2=256字节），
// 矩阵共128行，即128个数据块；相邻数据块首地址的间隔为256字节
uint32_t burstNum = M;                       // 数据块个数：128
uint32_t burstLen = K * sizeof(half);        // 每个数据块长度：256字节
uint64_t burstSrcStride = K * sizeof(half);  // 相邻数据块间隔：256字节

// 预加载过程：将A矩阵预加载到L2 Cache，不实际搬进L1 Buffer/Unified Buffer
asc_prefetch_gm2l2cache(a, burstNum, burstLen, burstSrcStride);
// 后续核间指令访问A矩阵所在的GM地址时，可命中L2 Cache中已预加载的数据，
// 降低访问延迟；此处可继续下发当前轮的搬运、计算等指令
```
