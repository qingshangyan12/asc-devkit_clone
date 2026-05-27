# RepeatReduceSum

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <cann-filter npu-type="950">Ascend 950PR/Ascend 950DT | √ </cann-filter>|
| <cann-filter npu-type="A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √ </cann-filter>|
| <cann-filter npu-type="310b">Atlas 200I/500 A2 推理产品 | x </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品AI Core | √ </cann-filter>|
| <cann-filter npu-type="310p">Atlas 推理系列产品Vector Core | x </cann-filter>|
| <cann-filter npu-type="910">Atlas 训练系列产品 | √ </cann-filter>|

## 功能说明

每个repeat内所有数据求和。和[WholeReduceSum](WholeReduceSum.md)接口相比，不支持mask逐bit模式。建议使用功能更全面的[WholeReduceSum](WholeReduceSum.md)接口。

## 函数原型

该原型仅支持如下产品型号：

Atlas A2 训练系列产品/Atlas A2 推理系列产品

Atlas 推理系列产品AI Core

Atlas 训练系列产品

```cpp
template <typename T, bool isSetMask = true>
__aicore__ inline void RepeatReduceSum(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const int32_t mask, const int32_t dstBlkStride, const int32_t srcBlkStride, const int32_t dstRepStride, const int32_t srcRepStride);
```

该原型仅支持如下产品型号：

Ascend 950PR/Ascend 950DT

```cpp
template <typename T, bool isSetMask = true, typename U = T>
__aicore__ inline void RepeatReduceSum(const LocalTensor<U>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const int32_t mask, const int32_t dstBlkStride, const int32_t srcBlkStride, const int32_t dstRepStride, const int32_t srcRepStride);
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 源操作数数据类型。 |
| U | 目的操作数数据类型。 |
| isSetMask | 是否在接口内部设置mask。<br>true，表示在接口内部设置mask。false，表示在接口外部设置mask，开发者需要使用[SetVectorMask](../掩码操作/SetVectorMask.md)接口设置mask值。这种模式下，接口入参中的mask值设置为占位符MASK_PLACEHOLDER，用于占位，无实际含义。 |

**表 2**  参数说明

| 参数名称 | 输入/输出 | 含义 |
| --- | --- | --- |
| dst | 输出 | 目的操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要保证2字节对齐（针对half数据类型），4字节对齐（针对float数据类型）。<br><br>Ascend 950PR/Ascend 950DT，支持的数据类型为：uint32_t/int32_t/half/float<br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：half/float<br><br>Atlas 推理系列产品AI Core，支持的数据类型为：half/float<br><br>Atlas 训练系列产品，支持的数据类型为：half/float |
| src | 输入 | 源操作数。<br><br>类型为[LocalTensor](../../../基础数据结构/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。<br><br>LocalTensor的起始地址需要32字节对齐。<br><br>源操作数的数据类型需要与目的操作数保持一致。<br><br>针对Ascend 950PR/Ascend 950DT：src数据类型uint16_t和int16_t时，dst数据类型分别为uint32_t和int32_t，其他情况下，dst数据类型均与src数据类型相同。<br><br>Ascend 950PR/Ascend 950DT，支持的数据类型为：uint16_t/int16_t/uint32_t/int32_t/half/float<br><br>Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：half/float<br><br>Atlas 推理系列产品AI Core，支持的数据类型为：half/float<br><br>Atlas 训练系列产品，支持的数据类型为：half/float |
| repeatTime | 输入 | 重复迭代次数。取值范围为[0, 255]。矢量计算单元，每次读取连续的256Bytes数据进行计算，为完成对输入数据的处理，必须通过多次迭代才能完成所有数据的读取与计算。repeatTime表示迭代的次数。<br><br>关于该参数的具体描述请参考[高维切分API](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| mask | 输入 | 用于控制每次迭代内连续的多少个元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，mask∈[1, 128]；当操作数为32位时，mask∈[1, 64]。 |
| dstBlkStride | 输入 | 此参数无效，可以配置任意值。 |
| srcBlkStride | 输入 | 单次迭代内datablock的地址步长。详细说明请参考[dataBlockStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |
| dstRepStride | 输入 | 目的操作数相邻迭代间的地址步长。以一个repeat归约后的长度为单位。<br><br>单位为dst数据类型所占字节长度。比如当dst为half时，单位为2Bytes。<br><br>注意，此参数值Atlas 训练系列产品不支持配置0。 |
| srcRepStride | 输入 | 源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的datablock数目。详细说明请参考[repeatStride](https://gitcode.com/cann/asc-devkit/blob/master/docs/guide/编程指南/C++类库API/基础API/接口分类说明/高维切分API.md)。 |

## 返回值说明

无

## 约束说明

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../通用说明和约束.md#section668772811100)。

- 对于RepeatReduceSum，其内部的相加方式采用二叉树方式，两两相加：

    假设源操作数为128个half类型的数据\[data0,data1,data2...data127\]，一个repeat可以计算完，计算过程如下。

    1.  data0和data1相加得到data00，data2和data3相加得到data01...data124和data125相加得到data62，data126和data127相加得到data63；
    2.  data00和data01相加得到data000，data02和data03相加得到data001...data62和data63相加得到data031；
    3.  以此类推，得到目的操作数为1个half类型的数据\[data\]。

    需要注意的是两两相加的计算过程中，计算结果大于65504时结果保存为65504。例如源操作数为\[60000,60000,-30000,100\]，首先60000+60000溢出，结果为65504，第二步计算-30000+100=-29900，第四步计算65504-29900=35604。

## 调用示例

本样例中只展示Compute流程中的部分代码。完整样例可参考[WholeReduce系列归约指令样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。

```cpp
// dstLocal，srcLocal均为half类型，srcLocal的计算数据量为512，连续排布，计算结果也需要连续排布，使用tensor高维切分计算接口，设定mask为最多的128个全部元素参与计算
// 根据以上信息，推断出repeat为4，dstRepStride为1，srcBlkStride为1，srcRepStride为8，dstBlkStride无效，此处配置0，因此接口示例为：
int mask = 128; // 参与计算的有效数字
int repeat = 4; // repeat = 512 / mask
AscendC::RepeatReduceSum<half>(dstLocal, srcLocal, repeat, mask, 0, 1, 1, 8);
```

结果示例如下：

```cpp
输入数据(src_gm):
[1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2
 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2
 ...
 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3
 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3 3
]
输出数据(dst_gm):
[128 256 ... 384]
```
