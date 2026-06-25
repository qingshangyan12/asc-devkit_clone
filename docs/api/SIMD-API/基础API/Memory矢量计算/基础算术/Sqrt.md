# Sqrt<a name="ZH-CN_TOPIC_0000001379163546"></a>

## 产品支持情况<a name="section1550532418810"></a>

### 不传入 config 的原型

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品 AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品 Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：支持
<!-- end id9 -->

### 传入 config 的原型

<!-- npu="950" id10 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id10 -->
<!-- npu="A3" id11 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id11 -->
<!-- npu="910b" id12 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id12 -->
<!-- npu="310b" id13 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id13 -->
<!-- npu="310p" id14 -->
- Atlas 推理系列产品 AI Core：不支持
<!-- end id14 -->
<!-- npu="310p" id15 -->
- Atlas 推理系列产品 Vector Core：不支持
<!-- end id15 -->
<!-- npu="910" id16 -->
- Atlas 训练系列产品：不支持
<!-- end id16 -->
<!-- npu="x90" id17 -->
- Kirin X90：不支持
<!-- end id17 -->
<!-- npu="9030" id18 -->
- Kirin 9030：不支持
<!-- end id18 -->

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_vec_unary_intf.h"`。

Sqrt 属于单目矢量类计算接口，负责将输入的 tensor 按元素做开方。计算公式如下：

$dst_i = \sqrt{src_i}$

## 函数原型<a name="section620mcpsimp"></a>

- 传入 config 的原型

    - tensor 前 n 个数据连续计算

        ```cpp
        template <typename T, const SqrtConfig& config = DEFAULT_SQRT_CONFIG>
        __aicore__ inline void Sqrt(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
        ```

    - tensor 高维切分计算
        - mask 逐 bit 模式

            ```cpp
            template <typename T, bool isSetMask = true, const SqrtConfig& config = DEFAULT_SQRT_CONFIG>
            __aicore__ inline void Sqrt(const LocalTensor<T>& dst, const LocalTensor<T>& src, uint64_t mask[], const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
            ```

        - mask 连续模式

            ```cpp
            template <typename T, bool isSetMask = true, const SqrtConfig& config = DEFAULT_SQRT_CONFIG>
            __aicore__ inline void Sqrt(const LocalTensor<T>& dst, const LocalTensor<T>& src, uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
            ```

- 不传入 config 的原型

    - tensor 前 n 个数据连续计算

        ```cpp
        template <typename T>
        __aicore__ inline void Sqrt(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)
        ```

    - tensor 高维切分计算
        - mask 逐 bit 模式

            ```cpp
            template <typename T, bool isSetMask = true>
            __aicore__ inline void Sqrt(const LocalTensor<T>& dst, const LocalTensor<T>& src, uint64_t mask[], const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
            ```

        - mask 连续模式

            ```cpp
            template <typename T, bool isSetMask = true>
            __aicore__ inline void Sqrt(const LocalTensor<T>& dst, const LocalTensor<T>& src, uint64_t mask, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
            ```

## 参数说明<a name="section622mcpsimp"></a>

**表1** 模板参数说明

| 参数名 | 描述 |
|---|---|
| T | 操作数数据类型。 |
| isSetMask | 是否在接口内部设置 mask。<br>&bull; true，表示在接口内部设置 mask。<br>&bull; false，表示在接口外部设置 mask，开发者需要使用[SetVectorMask](../掩码操作/SetVectorMask.md)接口设置 mask 值。这种模式下，接口入参中的 mask 值设置为占位符`MASK_PLACEHOLDER`，用于占位，无实际含义。 |
| <!-- npu="950" id22 -->config | 该参数仅支持 Ascend 950PR/Ascend 950DT。<br>用于配置精度计算模式，SqrtConfig 类型，定义如下：<br><pre>enum&nbsp;class&nbsp;SqrtAlgo&nbsp;{<br>&nbsp;&nbsp;&nbsp;&nbsp;INTRINSIC&nbsp;=&nbsp;0,<br>&nbsp;&nbsp;&nbsp;&nbsp;FAST_INVERSE,<br>&nbsp;&nbsp;&nbsp;&nbsp;PRECISION_1ULP_FTZ_TRUE,<br>&nbsp;&nbsp;&nbsp;&nbsp;PRECISION_0ULP_FTZ_FALSE,<br>&nbsp;&nbsp;&nbsp;&nbsp;PRECISION_1ULP_FTZ_FALSE,<br>};<br>struct&nbsp;SqrtConfig&nbsp;{<br>&nbsp;&nbsp;&nbsp;&nbsp;SqrtAlgo&nbsp;algo&nbsp;=&nbsp;SqrtAlgo::INTRINSIC;<br>};</pre>通过 SqrtConfig 结构体的参数 algo 来配置精度计算模式。algo 取值如下：<br>&bull; SqrtAlgo::INTRINSIC、SqrtAlgo::PRECISION_1ULP_FTZ_TRUE，使用单指令计算得出结果，最大精度误差为1ulp。<br>&bull; SqrtAlgo::FAST_INVERSE、SqrtAlgo::PRECISION_0ULP_FTZ_FALSE，使用快速求逆算法得出结果。适用于输入值在[0, 85070596800837026223494223584045301760]范围内的计算。在该范围内，算法保证输出的最大精度误差为0ulp；当输入值大于85070596800837026223494223584045301760 时，输出为0。目前，该算法仅支持 float 数据类型，并在该模式下支持 Subnormal 数据计算。<br>&bull; SqrtAlgo::PRECISION_1ULP_FTZ_FALSE，仅支持 half 类型的 Subnormal 数据计算，此时最大精度误差为1ulp。<br>该参数的默认值 DEFAULT_SQRT_CONFIG 的取值如下：<br><pre>constexpr&nbsp;SqrtConfig&nbsp;DEFAULT_SQRT_CONFIG&nbsp;=&nbsp;{&nbsp;SqrtAlgo::INTRINSIC&nbsp;};</pre><!-- end id22 --> |

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的操作数。<br>类型为 LocalTensor，支持的 TPosition 为 VECIN/VECCALC/VECOUT。<br>地址对齐约束参考[通用地址对齐约束](../../../通用说明和约束.md)。 |
| src | 输入 | 源操作数。<br>类型为 LocalTensor，支持的 TPosition 为 VECIN/VECCALC/VECOUT。<br>地址对齐约束参考[通用地址对齐约束](../../../通用说明和约束.md)。 |
| count | 输入 | 参与计算的元素个数。关于该参数的具体说明请参考[连续计算](../SIMD 计算说明/连续计算.md)。 |
| mask[]/mask | 输入 | mask 用于控制每次迭代内参与计算的元素。详细设置参考[掩码](../SIMD 计算说明/掩码/掩码.md)。 |
| repeatTime | 输入 | 重复迭代次数。矢量计算单元，每次读取连续的256Bytes 数据进行计算，为完成对输入数据的处理，必须通过多次迭代（repeat）才能完成所有数据的读取与计算。repeatTime 表示迭代的次数。<br>关于该参数的具体说明请参考[高维切分](../SIMD 计算说明/高维切分.md)。 |
| repeatParams | 输入 | 控制操作数地址步长的参数。[UnaryRepeatParams](../../数据结构/辅助数据结构//UnaryRepeatParams.md)类型，包含操作数相邻迭代间相同 DataBlock 的地址步长，操作数同一迭代内不同 DataBlock 的地址步长等参数。<br>相邻迭代间的地址步长参数说明请参考[repeatStride](../SIMD 计算说明/高维切分.md)；同一迭代内 DataBlock 的地址步长参数说明请参考[dataBlockStride](../SIMD 计算说明/高维切分.md)。 |

## 数据类型

T 支持的数据类型为：half、float。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../../通用说明和约束.md)。

<!-- npu="A3,910b" id21 -->
- 针对如下型号，当参数 count 或 repeatTime 取值为0 时，不会执行计算操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。
  <!-- npu="A3" id19 -->
  - Atlas A3 训练系列产品/Atlas A3 推理系列产品
  <!-- end id19 -->
  <!-- npu="910b" id20 -->
  - Atlas A2 训练系列产品/Atlas A2 推理系列产品
  <!-- end id20 -->
<!-- end id21 -->

- 如果 src 中的数值为非正数，可能会产生未知结果。

<!-- npu="950" id23 -->
- 针对 Ascend 950PR/Ascend 950DT，如下约束说明：
  - SqrtAlgo::FAST\_INVERSE、SqrtAlgo::PRECISION\_0ULP\_FTZ\_FALSE，使用快速求逆算法得出结果。适用于输入值在\[0, 85070596800837026223494223584045301760\]范围内的计算。在该范围内，算法保证输出的最大精度误差为0ulp；当输入值大于85070596800837026223494223584045301760 时，输出为0。目前，该算法仅支持 float 数据类型，并在该模式下支持 Subnormal 数据计算。
  - tensor 高维切分计算调用 GetTemporaryBufferAddr，占用8KB Unified Buffer。
  - tensor 前 n 个数据连续计算未调用 GetTemporaryBufferAddr，不涉及8KB Unified Buffer。
  - tensor 前 n 个数据计算 API 中的 isSetMask 参数不生效，保持默认值即可。
  - 当参数 count 或 repeatTime 取值为0 时，软仿行为不保证该接口被视为 NOP（空操作）。
<!-- end id23 -->

## 关键特性<a name="section18972943153217"></a>

<!-- npu="950" id24 -->
针对 Ascend 950PR/Ascend 950DT，有如下关键特性：

**最大精度误差**：

- SqrtAlgo::INTRINSIC、SqrtAlgo::PRECISION\_1ULP\_FTZ\_TRUE，最大精度误差为1ulp。
- SqrtAlgo::PRECISION\_1ULP\_FTZ\_FALSE，软仿实现，最大精度误差为1ulp。
- SqrtAlgo::FAST\_INVERSE、SqrtAlgo::PRECISION\_0ULP\_FTZ\_FALSE，软仿实现，最大精度误差为0ulp。

**配置 Subnormal 模式**：

FTZ（Flush To Zero）：一种浮点运算模式，当结果为[Subnormal](内置数据类型.md#p7381131713310)时，将其直接清零（近似为0），而非保留其精确的微小数值。

只有将 algo 设置为 SqrtAlgo::FAST\_INVERSE、SqrtAlgo::PRECISION\_0ULP\_FTZ\_FALSE、SqrtAlgo::PRECISION\_1ULP\_FTZ\_FALSE 时，Sqrt 接口才会保留并正确输出 Subnormal 结果；其他模式下 Subnormal 均被 FTZ。

由于 Subnormal 的计算行为是通过软件仿真算法实现，一般场景推荐使用性能更好的 SqrtAlgo::INTRINSIC、SqrtAlgo::PRECISION\_1ULP\_FTZ\_TRUE；需要精确 Subnormal 输出的场景（如特定数据精度要求的算法、避免除零错误）使用 SqrtAlgo::FAST\_INVERSE、SqrtAlgo::PRECISION\_0ULP\_FTZ\_FALSE、SqrtAlgo::PRECISION\_1ULP\_FTZ\_FALSE。
<!-- end id24 -->

## 调用示例<a name="section176061616102911"></a>

- tensor 高维切分计算样例-mask 连续模式

    ```cpp
    uint64_t mask = 256 / sizeof(half);
    // repeatTime = 4, 128 elements one repeat, 512 elements total.
    // dstBlkStride, srcBlkStride = 1, no gap between blocks in one repeat.
    // dstRepStride, srcRepStride = 8, no gap between repeats.
    AscendC::Sqrt(dstLocal, srcLocal, mask, 4, { 1, 1, 8, 8 });
    ```

- tensor 高维切分计算样例-mask 逐 bit 模式

    ```cpp
    uint64_t mask[2] = { UINT64_MAX, UINT64_MAX };
    // repeatTime = 4, 128 elements one repeat, 512 elements total.
    // dstBlkStride, srcBlkStride = 1, no gap between blocks in one repeat.
    // dstRepStride, srcRepStride = 8, no gap between repeats.
    AscendC::Sqrt(dstLocal, srcLocal, mask, 4, { 1, 1, 8, 8 });
    ```

- tensor 前 n 个数据计算接口样例

    ```cpp
    AscendC::Sqrt(dstLocal, srcLocal, 512);
    ```

<!-- npu="950" id25 -->
以下示例仅支持 Ascend 950PR/Ascend 950DT

    ```cpp
    static constexpr SqrtConfig config = { SqrtAlgo::PRECISION_1ULP_FTZ_FALSE };
    AscendC::Sqrt<T, config>(dstLocal, srcLocal, 512);
    ```
<!-- end id25 -->

结果示例如下：

```bash
输入数据 srcLocal：[1.0 2.0 3.0 4.0 ...]
输出数据 dstLocal：[1.0 1.414 1.732 2.0 ...]
```

完整样例请参考[单目矢量计算类 API 样例](https://gitcode.com/cann/asc-devkit/pull/873)。
