# Ors<a name="ZH-CN_TOPIC_0000002132550008"></a>

## 产品支持情况<a name="section1550532418810"></a>

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

## 功能说明<a name="section618mcpsimp"></a>

矢量内每个元素和标量间做或操作，支持标量在前和标量在后两种场景，其中标量输入支持配置LocalTensor单点元素。计算公式如下，idx表示LocalTensor单点元素的位置系数。

![](../../../../figures/zh-cn_formulaimage_0000002392198093.png)

## 函数原型<a name="section620mcpsimp"></a>

-   Tensor前n个数据计算

    ```
    template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG, typename U, typename S, typename V>
    __aicore__ inline void Ors(const U& dst, const S& src0, const V& src1, const int32_t& count)
    ```

-   Tensor高维切分计算
    -   mask逐bit模式

        ```
        template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG, typename U, typename S, typename V>
        __aicore__ inline void Ors(const U& dst, const S& src0, const V& src1, uint64_t mask[], const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
        ```

    -   mask连续模式

        ```
        template <typename T = BinaryDefaultType, bool isSetMask = true, const BinaryConfig& config = DEFAULT_BINARY_CONFIG, typename U, typename S, typename V>
        __aicore__ inline void Ors(const U& dst, const S& src0, const V& src1, uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
        ```

## 参数说明<a name="section622mcpsimp"></a>

**表1**  模板参数说明

<a name="table4835205712588"></a>
- T：不支持
- isSetMask：不支持
- config：不支持
- U：不支持
- S：不支持
- V：不支持

**表2**  参数说明

<a name="table5422161464"></a>
- dst：不支持
- src0/src1：不支持
- count：不支持
- mask/mask[]：不支持
- repeatTime：不支持
- repeatParams：不支持

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   使用Tensor高维切分计算接口时，节省地址空间，开发者可以定义一个Tensor，供源操作数与目的操作数同时使用（即地址重叠），相关约束如下：
    -   对于单次repeat（repeatTime=1），且源操作数与目的操作数之间要求100%完全重叠，不支持部分重叠。
    -   对于多次repeat（repeatTime\>1），操作数与目的操作数之间存在依赖的情况下，即第N次迭代的目的操作数是第N+1次的源操作数，不支持地址重叠。
    -   源操作数为LocalTensor单点元素的场景，不支持源操作数和目的操作数地址重叠。

-   操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。

-   针对Ascend 950PR/Ascend 950DT，uint64\_t/int64\_t数据类型仅支持Tensor前n个数据计算接口。
-   左操作数及右操作数中，必须有一个为矢量；当前不支持左右操作数同时为标量。
-   本接口传入LocalTensor单点数据作为标量时，idx参数需要传入编译期已知的常量，传入变量时需要声明为constexpr。

## 调用示例<a name="section642mcpsimp"></a>

-   Tensor高维切分计算样例-mask连续模式

    ```
    // dstLocal：输出Tensor
    // src0Local：输入Tensor
    // src1Local：输入Tensor
    
    uint64_t mask = 128;
    // repeatTime = 4, 单次迭代处理128个数，计算512个数需要迭代4次
    // dstBlkStride, srcBlkStride = 1, 每个迭代内src0参与计算的数据地址间隔为1个datablock，表示单次迭代内数据连续读取和写入
    // dstRepStride, srcRepStride = 8, 相邻迭代间的地址间隔为8个datablock，表示相邻迭代间数据连续读取和写入
    // 标量在后示例
    AscendC::Ors(dstLocal, src0Local, src1Local[0], mask, 4, { 1, 1, 8, 8 });
    
    // 标量在前示例
    static constexpr AscendC::BinaryConfig config = { 0 };
    AscendC::Ors<BinaryDefaultType, true, config>(dstLocal, src0Local[0], src1Local, mask, 4, {1, 1, 8, 8});
    ```

-   Tensor高维切分计算样例-mask逐bit模式

    ```
    // dstLocal：输出Tensor
    // src0Local：输入Tensor
    // src1Local：输入Tensor
    
    uint64_t mask[2] = { UINT64_MAX, UINT64_MAX };
    // repeatTime = 4, 单次迭代处理128个数，计算512个数需要迭代4次
    // dstBlkStride, srcBlkStride = 1, 每个迭代内src0参与计算的数据地址间隔为1个datablock，表示单次迭代内数据连续读取和写入
    // dstRepStride, srcRepStride = 8, 相邻迭代间的地址间隔为8个datablock，表示相邻迭代间数据连续读取和写入
    // 标量在后示例
    AscendC::Ors(dstLocal, src0Local, src1Local[0], mask, 4, {1, 1, 8, 8});
    
    // 标量在前示例
    static constexpr AscendC::BinaryConfig config = { 0 };
    AscendC::Ors<BinaryDefaultType, true, config>(dstLocal, src0Local[0], src1Local, mask, 4, {1, 1, 8, 8});
    ```

-   Tensor前n个数据计算样例

    ```
    // 标量在后示例
    AscendC::Ors(dstLocal, src0Local, src1Local[0], 512);
    
    // 标量在前示例
    static constexpr AscendC::BinaryConfig config = { 0 };
    AscendC::Ors<BinaryDefaultType, true, config>(dstLocal, src0Local[0], src1Local, 512);
    ```

结果示例如下：

```
// 标量在后，src1Local[0]作为标量
输入数据src0Local: [1 2 3 ... 512]
输入数据src1Local: [1] 
输出数据dstLocal: [1 2 3 ... 512]

// 标量在前，src0Local[0]作为标量
输入数据src0Local: [1]
输入数据src1Local: [1 2 3 ... 512] 
输出数据dstLocal: [1 2 3 ... 512]
```
