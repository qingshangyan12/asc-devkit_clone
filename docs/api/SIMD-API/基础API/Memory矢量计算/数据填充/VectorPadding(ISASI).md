# VectorPadding\(ISASI\)<a name="ZH-CN_TOPIC_0000001870220489"></a>

## 产品支持情况<a name="section1550532418810"></a>

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：不支持
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
- Atlas 推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明<a name="section618mcpsimp"></a>

根据padMode（pad模式）与padSide（pad方向）对源操作数按照datablock进行填充操作。

假设源操作数的一个datablock有16个数，datablock\[0:15\]=a\~p：

-   padSide==false：从datablock的左边开始填充，即datablock的起始值方向\(a-\>p\)

-   padSide==true：从datablock的右边开始填充，即datablock的结束值方向\(p-\>a\)
-   padMode==0：用邻近数作为填充值，例：aaa|abc\(padSide=false\)、nop|ppp\(padSide=true\)
-   padMode==1：用邻近datablock值对称填充，例：cba|abc\(padSide=false\)、nop|pon\(padSide=true\)
-   padMode==2：用邻近datablock值填充，偏移一个数，做对称填充，例：
    -   padSide=false：xcb|abc，xcb被填充，填充过程描述：a被丢弃，对称填充，x处填充0
    -   padSide=true：nop|onx，onx被填充，填充过程描述：p被丢弃，对称填充，x处填充0

## 函数原型<a name="section620mcpsimp"></a>

-   tensor前n个数据计算

    ```
    template <typename T>
    __aicore__ inline void VectorPadding(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint8_t padMode, const bool padSide, const uint32_t count)
    ```

-   tensor高维切分计算
    -   mask逐bit模式

        ```
        template <typename T, bool isSetMask = true>
        __aicore__ inline void VectorPadding(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint8_t padMode, const bool padSide, const uint64_t mask[], const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
        ```

    -   mask连续模式

        ```
        template <typename T, bool isSetMask = true>
        __aicore__ inline void VectorPadding(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint8_t padMode, const bool padSide, const uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
        ```

## 参数说明<a name="section622mcpsimp"></a>

**表1**  模板参数说明

<a name="table4835205712588"></a>
- T：不支持
- isSetMask：不支持

**表2**  参数说明

<a name="table1055216132132"></a>
- dst：不支持
- src：不支持
- padMode：不支持
- padSide：不支持
- count：不支持
- mask[]/mask：不支持
- repeatTime：不支持
- repeatParams：不支持

## 返回值说明<a name="section38228281712"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。

-   mask仅控制目的操作数中的哪些元素要写入，源操作数的读取与mask无关。
-   count表示写入目的操作数中的元素总数，源操作数的读取与count无关。

## 调用示例<a name="section642mcpsimp"></a>

样例的srcLocal和dstLocal均为half类型。

更多样例可参考[LINK](../基础算术/更多样例-9.md)。

-   tensor高维切分计算样例-mask连续模式

    ```
    uint64_t mask = 256 / sizeof(half);
    uint8_t padMode = 0;
    bool padSide = false;
    // repeatTime = 4, 128 elements one repeat, 512 elements total
    // dstBlkStride, srcBlkStride = 1, no gap between blocks in one repeat
    // dstRepStride, srcRepStride = 8, no gap between repeats
    AscendC::VectorPadding(dstLocal, srcLocal, padMode, padSide, mask, 4, { 1, 1, 8, 8 });
    ```

-   tensor高维切分计算样例-mask逐bit模式

    ```
    uint64_t mask[2] = { UINT64_MAX, UINT64_MAX };
    uint8_t padMode = 0;
    bool padSide = false;
    // repeatTime = 4, 128 elements one repeat, 512 elements total
    // dstBlkStride, srcBlkStride = 1, no gap between blocks in one repeat
    // dstRepStride, srcRepStride = 8, no gap between repeats
    AscendC::VectorPadding(dstLocal, srcLocal, padMode, padSide, mask, 4, { 1, 1, 8, 8 });
    ```

-   tensor前n个数据计算样例

    ```
    uint8_t padMode = 0;
    bool padSide = false;
    AscendC::VectorPadding(dstLocal, srcLocal, padMode, padSide, 512);
    ```

结果示例如下：

```
// 以srcLocal的一个datablock的值为例，有16个数
输入数据(srcLocal): [6.938 -8.86 -0.2263 ... 1.971 1.778]
输出数据(dstLocal): 
[6.938 6.938 6.938 ... 6.938 6.938]
```
