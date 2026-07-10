# Select（灵活标量位置）<a name="ZH-CN_TOPIC_0000002171712357"></a>

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

给定两个源操作数src0和src1，根据selMask（用于选择的Mask掩码）的比特位值选取元素，得到目的操作数dst。选择的规则为：当selMask的比特位是1时，从src0中选取，比特位是0时从src1选取。

对于tensor高维切分计算接口，支持根据mask参数对上述选取结果，再次进行过滤，有效位填入最终的dst，无效位则保持dst原始值。例如：src0为\[1,2,3,4,5,6,7,8\]，src1为\[9,10,11,12,13,14,15,16\]，selMask为\[0,0,0,0,1,1,1,1\]，mask为\[1,1,1,1,0,0,0,0\]，dst原始值为\[-1,-2,-3,-4,-5,-6,-7,-8\]，则根据selMask的比特位选取后的结果dst\_temp为：\[9,10,11,12,5,6,7,8\]，然后再根据mask进行过滤，dst的最终输出结果为\[9,10,11,12,-5,-6,-7,-8\]。

本选择功能支持三种模式：

-   模式0：根据selMask在两个tensor中选取元素。selMask中有效数据的个数存在限制，具体取决于源操作数的数据类型。在每一轮迭代中，根据selMask的有效位数据进行选择操作，每一轮迭代采用的selMask，均为相同数值，即selMask的有效数值。
-   模式1：根据selMask在1个tensor和1个scalar标量中选取元素，selMask无有效数据限制。多轮迭代时，每轮迭代连续使用selMask的不同部分。
-   模式2：根据selMask在两个tensor中选取元素，selMask无有效数据限制。多轮迭代时，每轮迭代连续使用selMask的不同部分。

针对模式1，提供灵活标量位置的接口。

## 函数原型<a name="section620mcpsimp"></a>

-   tensor前n个数据计算

    ```
    template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
    __aicore__ inline void Select(const T2& dst, const LocalTensor<T1>& selMask, const T3& src0, const T4& src1, SELMODE selMode, uint32_t count)
    ```

-   tensor高维切分计算
    -   mask逐bit模式

        ```
        template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true, const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
        __aicore__ inline void Select(const T2& dst, const LocalTensor<T1>& selMask, const T3& src0, const T4& src1, SELMODE selMode, uint64_t mask[], uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
        ```

    -   mask连续模式

        ```
        template <typename T0 = BinaryDefaultType, typename T1 = BinaryDefaultType, bool isSetMask = true, const BinaryConfig &config = DEFAULT_BINARY_CONFIG, typename T2, typename T3, typename T4>
        __aicore__ inline void Select(const T2& dst, const LocalTensor<T1>& selMask, const T3& src0, const T4& src1, SELMODE selMode, uint64_t mask, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
        ```

## 参数说明<a name="section622mcpsimp"></a>

**表1**  模板参数说明

<a name="table1183814716169"></a>
- T0：不支持
- T1：不支持
- isSetMask：不支持
- selMode：不支持
- config：不支持
- T2：不支持
- T3：不支持
- T4：不支持

**表2**  参数说明

<a name="table8955841508"></a>
- dst：不支持
- selMask：不支持
- src0：不支持
- src1：不支持
- selMode：不支持
- mask/mask[]：不支持
- repeatTime：不支持
- repeatParams：不支持
- count：不支持

## 返回值说明<a name="section198548421851"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   调用灵活标量位置接口且源操作数为LocalTensor单点元素的场景，不支持源操作数和目的操作数地址重叠。
-   操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。
-   针对Ascend 950PR/Ascend 950DT，int8\_t/uint8\_t/uint64\_t/int64\_t/complex32/complex64数据类型仅支持tensor前n个数据计算接口。
-   左操作数及右操作数中，必须有一个为矢量；当前不支持左右操作数同时为标量。
-   本接口传入LocalTensor单点数据作为标量时，idx参数需要传入编译期已知的常量，传入变量时需要声明为constexpr。
-   模式1场景使用灵活标量位置接口时需要填写模板参数config避免接口匹配到其他模式。

## 调用示例<a name="section642mcpsimp"></a>

-   Select-tensor前n个数据计算样例（模式1）

    ```
    // 灵活标量位置，src1Local[0]作为标量
    static constexpr AscendC::BinaryConfig config = { 1 };
    AscendC::Select<BinaryDefaultType, uint8_t, config>(dstLocal, maskLocal, src0Local, src1Local[0], AscendC::SELMODE::VSEL_TENSOR_SCALAR_MODE, dataSize);
    
    // 灵活标量位置，src0Local[0]作为标量
    static constexpr AscendC::BinaryConfig config = { 0 };
    AscendC::Select<BinaryDefaultType, uint8_t, config>(dstLocal, maskLocal, src0Local[0], src1Local, AscendC::SELMODE::VSEL_TENSOR_SCALAR_MODE, dataSize);
    ```
