# asc_prefetch_gm2l2cache_dn2nz

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

将矩阵数据从Global Memory预加载到L2 Cache中，在此过程中执行DN->NZ/NCHW->NC1HWC0/NCHW->C1HWNC0操作。仅预加载到L2 Cache，不实际搬进L1 Buffer。仅Cube核支持。

GM中的数据以DN类型存储；预加载后在L2 Cache中以NZ形式缓存。
DN->NZ的搬运形式参考[asc_copy_gm2l1_dn2nz](asc_copy_gm2l1_dn2nz.md)。

## 函数原型

```c
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(__gm__ <dtype>* src,
                                                      uint64_t loop1_src_stride,
                                                      uint16_t n_value,
                                                      uint32_t d_value,
                                                      uint64_t loop4_src_stride)
```

### dtype支持的数据类型

dtype支持的数据类型为`int8_t`、`uint8_t`、`hifloat8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

### 典型示例

```c
// 示例：half类型数据的DN2NZ模式预取。
__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz(__gm__ half* src,
                                                      uint64_t loop1_src_stride,
                                                      uint16_t n_value,
                                                      uint32_t d_value,
                                                      uint64_t loop4_src_stride)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 含义 |
| --- | --- | --- |
| src | 输入 | 源操作数，存储位置为GM。起始地址需要按照1字节对齐。 |
| loop1_src_stride | 输入 | 源操作数同一DN矩阵的相邻行起始地址间的偏移，单位字节。取值范围：[0, $2^{40}-1$]。 |
| n_value | 输入 | 源矩阵中DN矩阵的列数，单位为元素。取值范围：[1, 65535]。 |
| d_value | 输入 | 源矩阵中DN矩阵的行数，单位为元素。取值范围：[1, $2^{21}-1$]。 |
| loop4_src_stride | 输入 | 源矩阵相邻DN矩阵起始地址间的偏移，单位为字节。取值范围：[0, $2^{40}-1$]。<br>&nbsp;&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数为1时，此参数无意义，设置为0即可。<br>&nbsp;&nbsp;&nbsp;&bull; 源矩阵DN矩阵个数大于1时，当`loop4_src_stride = 0`时，表示重复预取源矩阵的第一个DN矩阵。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 本接口仅Cube核支持。
- 本接口仅将GM数据预取到L2 Cache，不写入L1 Buffer等片上存储单元，也不改变GM中的数据内容。

## 调用示例

```cpp
// 设置源操作数，A矩阵按DN格式存放：共128行（D维），每行128个元素（N维），
// 即1个128×128的DN矩阵，N维连续存放
constexpr uint32_t M = 128;
constexpr uint32_t K = 128;
__gm__ half* a;

// 预加载过程中的配置
uint64_t loop1SrcStride = M * sizeof(half);  // 同一DN矩阵相邻行起始地址间隔：256字节
uint16_t nValue = M;                         // DN矩阵的列数：128
uint32_t dValue = K;                         // DN矩阵的行数：128
uint64_t loop4SrcStride = 0;                 // 仅预加载1个DN矩阵，设置为0

// 预加载过程：预加载过程中执行DN->NZ格式转换，
// A矩阵在L2 Cache中以NZ形式缓存，不实际搬进L1 Buffer
asc_prefetch_gm2l2cache_dn2nz(a, loop1SrcStride, nValue, dValue, loop4SrcStride);
// 后续执行DN->NZ搬运的指令访问相同GM地址时，可直接命中L2 Cache中
// 已转换为NZ形式的数据，降低访问延迟
```
