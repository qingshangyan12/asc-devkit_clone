# BilinearInterpolation\(ISASI\)<a name="ZH-CN_TOPIC_0000001786742102"></a>

## 产品支持情况<a name="section1550532418810"></a>

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
- Atlas 推理系列产品 AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品 Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_vec_bilinearinterpolation_intf.h"`。

BilinearInterpolation 属于双目矢量类计算接口，核心通过水平迭代+垂直迭代的双层循环完成乘累加运算，适用于非规则数据访问+权重乘加的向量运算场景（数据 Gather+Multiply+Reduce）。

注：该接口是为了兼容旧款产品接口而添加的软仿接口。

具体计算流程参考下图：

**图1**  BilinearInterpolation 计算流程示意图

![](../../../../figures/bilinearInterpolation_1.png)

![](../../../../figures/bilinearInterpolation_2.png)

结合上面的计算流程示意图，下面以一个具体示例，来介绍 BilinearInterpolation 整个计算过程。

假设计算场景的输入如下：

```text
src0 = [1, 2, 3, ..., 512]，共计512 个 half 数据；
src0Offset = [0, 32, 64, ..., 992]，共计32 个 uint32 数据；
src1 = [2, 3, 4, ..., 33]，共计32 个 half 数据；
hRepeat = 2；vRepeat = 2；mask = 128；vROffset = 128。
```

具体计算流程拆解分析如下：

- 步骤1. 按 DataBlock 粒度，收集输入 src0 张量到临时 Buffer。

    总共 hRepeat \* vRepeat 次迭代，每个迭代从输入的 src0Offset 读取8 个偏移值，每个偏移值指向 src0 的一个 DataBlock 起始地址。

    调用[Gatherb](../离散与聚合/Gatherb(ISASI).md)接口，Gatherb 接口接受输入张量（src0）、DataBlock 偏移张量（src0Offset），根据 src0Offset 将输入张量 src0 按 DataBlock（32Bytes）收集到临时张量（sharedTmpBuffer）中。

    该步骤处理过程如下：

    - 总迭代次数：hRepeat \* vRepeat = 2 \* 2 = 4；
    - 每次迭代处理的 src0 元素个数：8 \* 32 / sizeof\(half\) = 128（8：DataBlock 的数量；32：每个 DataBlock 的大小为32B）；
    - 每次迭代收集结果
        - 迭代0：sharedTmpBuffer\[0:127\] = \[1, 2, ..., 128\]；
        - 迭代1：sharedTmpBuffer\[128:255\] = \[129, 130, ..., 256\]；
        - 迭代2：sharedTmpBuffer\[256:383\] = \[257, 258, ..., 384\]；
        - 迭代3：sharedTmpBuffer\[384:511\] = \[385, 386, ..., 512\]。

- 步骤2. 将输入的张量 src1 进行广播填充。

    调用[Brcb](../数据填充/Brcb.md)接口，每次迭代从输入张量 src1 中取8 个数，填充到结果张量 sharedTmpBuffer 的8 个 DataBlock 中（每个 DataBlock 的大小为32B）。

    该步骤处理过程如下：

    - 步骤1 收集的 src0 张量占用的空间大小 = 总迭代次数 \* 每次迭代处理的元素个数 = 4 \* 128 = 512。所以广播 src1 的元素从 sharedTmpBuffer\[512\]开始存放；
    - Brcb 指令迭代次数 repeatTime = src1 的数据长度 / 8 = 16 / 8 = 2 次（Brcb 指令每次迭代取8 个数）；
    - 将输入 src1 张量中的8 个数据，填充到 sharedTmpBuffer\[512\]位置起的8 个 DataBlock 中去。输入是 half 数据类型，故每个元素复制32 / sizeof\(half\) = 16 次。

        结果：sharedTmpBuffer\[512:1024\] = \[2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, ..., 33, 33\]。

- 步骤3. 收集的 src0 张量与广播后的 src1 张量，调用[Mul](Mul.md)执行逐元素乘法，结果覆盖 sharedTmpBuffer。

    迭代计算次数：hRepeat \* vRepeat = 2 \* 2 = 4。

    支持两种计算方式，由接口输入参数 repeatMode 控制（配置 true 或 false）。

    - repeatMode=false

        每次迭代，src0（数据存放位置为 sharedTmpBuffer\[0:511\]）读取8 个 DataBlock，每个 DataBlock 的每个值均与 src1（数据存放位置为 sharedTmpBuffer\[512:1024\]）的单个数值相乘。

        - 整个处理过程如下：
            - 迭代0 计算过程：第1 个 DataBlock（sharedTmpBuffer\[0:15\]）到第8 个 DataBlock（sharedTmpBuffer\[112:127\]），均与 src1 的2 相乘。

                即 sharedTmpBuffer\[0:127\] = \[1 \* 2, ..., 128 \* 2\]；

            - 迭代1 计算过程：第1 个 DataBlock（sharedTmpBuffer\[128:143\]）到第8 个 DataBlock（sharedTmpBuffer\[240:255\]），均与 src1 的3 相乘。

                即 sharedTmpBuffer\[128:255\] = \[129 \* 3, ..., 256 \* 3\]；

            - 迭代2 计算过程：第1 个 DataBlock（sharedTmpBuffer\[256:271\]）到第8 个 DataBlock（sharedTmpBuffer\[368:383\]），均与 src1 的4 相乘。

                即 sharedTmpBuffer\[256:383\] = \[257 \* 4, ..., 384 \* 4\]；

            - 迭代3 计算过程：第1 个 DataBlock（sharedTmpBuffer\[384:399\]）到第8 个 DataBlock（sharedTmpBuffer\[496:511\]），均与 src1 的5 相乘。

                即 sharedTmpBuffer\[384:511\] = \[385 \* 5, ..., 512 \* 5\]。

    - repeatMode=true

        每次迭代，src0（数据存放位置为 sharedTmpBuffer\[0:511\]）读取8 个 DataBlock，每个 DataBlock 分别与 src1（存放位置为 sharedTmpBuffer\[512:1024\]）的1 个数值相乘，即 src0 的8 个 DataBlock 分别对应 src1 的8 个数值。

        - 整个处理过程如下：
            - 迭代0 计算过程：第1 个 DataBlock（sharedTmpBuffer\[0:15\]）与 src1 的2 相乘，第2 个 DataBlock（sharedTmpBuffer\[16:31\]）与 src1 的3 相乘；...依次类推，第8 个 DataBlock（sharedTmpBuffer\[112:127\]）与 src1 的9 相乘。

                即 sharedTmpBuffer\[0:127\] = \[1 \* 2, ..., 16 \* 2, 17 \* 3, ..., 128 \* 9\]；

            - 迭代1 计算过程：第1 个 DataBlock（sharedTmpBuffer\[128:143\]）与 src1 的10 相乘，第2 个 DataBlock（sharedTmpBuffer\[144:159\]）与 src1 的11 相乘；...依次类推，第8 个 DataBlock（sharedTmpBuffer\[240:255\]）与 src1 的17 相乘。

                即 sharedTmpBuffer\[128:255\] = \[129 \* 10, ..., 144 \* 10, 145 \* 11, ..., 256 \* 17\]；

            - 迭代2 计算过程：第1 个 DataBlock（sharedTmpBuffer\[256:271\]）与 src1 的18 相乘，第2 个 DataBlock（sharedTmpBuffer\[272:287\]）与 src1 的19 相乘；...依次类推，第8 个 DataBlock（sharedTmpBuffer\[368:383\]）与 src1 的25 相乘。

                即 sharedTmpBuffer\[256:383\] = \[257 \* 18, ..., 272 \* 18, 273 \* 19, ..., 384 \* 25\]；

            - 迭代3 计算过程：第1 个 DataBlock（sharedTmpBuffer\[384:399\]）与 src1 的26 相乘，第2 个 DataBlock（sharedTmpBuffer\[400:415\]）与 src1 的27 相乘；...依次类推，第8 个 DataBlock（sharedTmpBuffer\[496:511\]）与 src1 的33 相乘。

                即 sharedTmpBuffer\[384:511\] = \[385 \* 26, ..., 390 \* 26, 391 \* 27, ..., 512 \* 33\]。

- 步骤4. 水平迭代累加，输出到 dst。

    调用[Add](Add.md)接口完成以下处理：

    - dst\[0:127\] = sharedTmpBuffer\[0:127\] + sharedTmpBuffer\[128:255\]；
    - dst\[128:255\] = sharedTmpBuffer\[256:383\] + sharedTmpBuffer\[384:511\]。

## 函数原型<a name="zh-cn_topic_0000002560891889_section82039854412"></a>

- mask 逐 bit 模式：

    ```cpp
    template <typename T>
    __aicore__ inline void BilinearInterpolation(const LocalTensor<T> &dst, const LocalTensor<T> &src0,
        const LocalTensor<uint32_t> &src0Offset, const LocalTensor<T> &src1, uint64_t mask[], uint8_t hRepeat,
        bool repeatMode, uint16_t dstBlkStride, uint16_t vROffset, uint8_t vRepeat,
        const LocalTensor<uint8_t> &sharedTmpBuffer);
    ```

- mask 连续模式：

    ```cpp
    template <typename T>
    __aicore__ inline void BilinearInterpolation(const LocalTensor<T> &dst, const LocalTensor<T> &src0,
        const LocalTensor<uint32_t> &src0Offset, const LocalTensor<T> &src1, uint64_t mask, uint8_t hRepeat,
        bool repeatMode, uint16_t dstBlkStride, uint16_t vROffset, uint8_t vRepeat,
        const LocalTensor<uint8_t> &sharedTmpBuffer);
    ```

## 参数说明<a name="section622mcpsimp"></a>

### 模板参数及接口参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
|---|---|
| T | 操作数数据类型。 |

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数。<br>类型为[LocalTensor](../../数据结构/LocalTensor 和 GlobalTensor 定义/LocalTensor/LocalTensor.md)，支持的 TPosition 为 VECIN/VECCALC/VECOUT。<br>LocalTensor 的起始地址需要32 字节对齐。 |
| src0、src1 | 输入 | 源操作数。<br>类型为[LocalTensor](../../数据结构/LocalTensor 和 GlobalTensor 定义/LocalTensor/LocalTensor.md)，支持的 TPosition 为 VECIN/VECCALC/VECOUT。<br>LocalTensor 的起始地址需要32 字节对齐。<br>两个源操作数的数据类型需要与目的操作数保持一致。 |
| src0Offset | 输入 | 源操作数。<br>类型为[LocalTensor](../../数据结构/LocalTensor 和 GlobalTensor 定义/LocalTensor/LocalTensor.md)，支持的 TPosition 为 VECIN/VECCALC/VECOUT。<br>LocalTensor 的起始地址需要32 字节对齐。 |
| mask[]/mask | 输入 | mask 用于控制每次迭代内参与计算的元素。详细设置参考[掩码](../SIMD 计算说明/掩码/掩码.md)。 |
| hRepeat | 输入 | 水平方向迭代次数，取值范围为[1, 255]。 |
| repeatMode | 输入 | 迭代模式：<br>&bull; false：每次迭代 src0 读取的8 个 DataBlock 中每个值均与 src1 的单个数值相乘。<br>&bull; true：每次迭代 src0 的每个 DataBlock 分别与 src1 的1 个数值相乘，即1 次迭代共消耗 src0 的8 个 DataBlock 和 src1 的8 个 element。 |
| dstBlkStride | 输入 | 单次迭代内，目的操作数不同 DataBlock 间地址步长，以32B 为单位。 |
| vROffset | 输入 | 垂直迭代间，目的操作数地址偏移量，以元素为单位，取值范围为[128, 65535)，vROffset * sizeof(T)需要保证32 字节对齐。 |
| vRepeat | 输入 | 垂直方向迭代次数，取值范围为[1, 255]。 |
| sharedTmpBuffer | 输入 | 临时空间。具体空间说明请参考下面的[sharedTmpBuffer 空间说明](#sharedtmpbuffer 空间说明)。 |

### sharedTmpBuffer 空间说明

<!-- npu="950" id10 -->
- 针对 Ascend 950PR/Ascend 950DT，不需要分配临时空间。
<!-- end id10 -->
<!-- npu="A3" id11 -->
- 针对 Atlas A3 训练系列产品/Atlas A3 推理系列产品，需要保证至少分配了 src0.GetSize() * 32 + src1.GetSize() * 32 字节的空间。
<!-- end id11 -->
<!-- npu="910b" id12 -->
- 针对 Atlas A2 训练系列产品/Atlas A2 推理系列产品，需要保证至少分配了 src0.GetSize() * 32 + src1.GetSize() * 32 字节的空间。
<!-- end id12 -->
<!-- npu="310p" id13 -->
- 针对 Atlas 推理系列产品 AI Core，需要保证至少分配了 src0OffsetLocal.GetSize() * sizeof(uint32_t)字节的空间。
<!-- end id13 -->

## 数据类型

支持的数据类型为：half。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md)。
- src0、src1、src0Offset 之间不允许地址重叠，且两个垂直 repeat 的目的地址之间不允许地址重叠。
- 参数 vROffset 的取值范围为\[128, 65535\)。
- 水平方向迭代次数，取值范围为\[1, 255\]。
- 垂直方向迭代次数，取值范围为\[1, 255\]。
- 输入的临时空间 sharedTmpBuffer，空间大小要求请参考[sharedTmpBuffer 空间说明](#sharedtmpbuffer 空间说明)。
- 参数 src0Offset 的取值要求如下：
    - 取值应保证 src0 元素类型位宽对齐。
    - 偏移地址后需要32 字节对齐。
    - 偏移地址后不能超出 UB 大小数据的范围。
    - 地址偏移的取值范围：不能超出 uint32\_t 的范围。
- 当参数 repeatMode 配置为 true 时，需确保 src1 的有效数据长度*16 ≥ src0 的数据长度。

## 调用示例<a name="section642mcpsimp"></a>

- 接口样例-mask 连续模式

    ```cpp
    AscendC::LocalTensor<half> dstLocal, src0Local, src1Local; // repeatMode = false，src1Local 的数据长度为16 即可
    AscendC::LocalTensor<uint32_t> src0OffsetLocal;
    AscendC::LocalTensor<uint8_t> tmpLocal;
    uint64_t mask = 128;        // mask 连续模式
    uint8_t hRepeat = 2;        // 水平迭代2 次
    bool repeatMode = false;    // 迭代模式
    uint16_t dstBlkStride = 1;  // 单次迭代内数据连续写入
    uint16_t vROffset = 128;    // 相邻迭代间数据连续写入
    uint8_t vRepeat = 2;        // 垂直迭代2 次
    
    AscendC::BilinearInterpolation(dstLocal, src0Local, src0OffsetLocal, src1Local, mask, hRepeat, repeatMode,
                dstBlkStride, vROffset, vRepeat, tmpLocal);
    ```

- 接口样例-mask 逐 bit 模式

    ```cpp
    AscendC::LocalTensor<half> dstLocal, src0Local, src1Local;
    AscendC::LocalTensor<uint32_t> src0OffsetLocal;
    AscendC::LocalTensor<uint8_t> tmpLocal;
    uint64_t mask[2] = { UINT64_MAX, UINT64_MAX }; // mask 逐 bit 模式
    uint8_t hRepeat = 2;        // 水平迭代2 次
    bool repeatMode = false;    // 迭代模式
    uint16_t dstBlkStride = 1;  // 单次迭代内数据连续写入
    uint16_t vROffset = 128;    // 相邻迭代间数据连续写入
    uint8_t vRepeat = 2;        // 垂直迭代2 次
    
    AscendC::BilinearInterpolation(dstLocal, src0Local, src0OffsetLocal, src1Local, mask, hRepeat, repeatMode,
                dstBlkStride, vROffset, vRepeat, tmpLocal);
    ```

结果示例如下：

```text
输入数据(src0Local,half): [1,2,3,...,512]
输入数据(src1Local,half): [2,3,4,...,17]
输入数据(src0OffsetLocal,uint32_t): [0,32,64,...,992]
输出数据(dstLocal,half): [389, 394, 399, 404, ...,4096]
```
