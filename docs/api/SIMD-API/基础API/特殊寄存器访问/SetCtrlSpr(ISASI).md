# SetCtrlSpr(ISASI)

## 产品支持情况

| 产品 | 是否支持 |
| --- | --- |
| <!-- npu="950" id1 -->Ascend 950PR/Ascend 950DT | √<!-- end id1 --> |
| <!-- npu="A3" id2 -->Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √<!-- end id2 --> |
| <!-- npu="910b" id3 -->Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √<!-- end id3 --> |
| <!-- npu="310b" id4 -->Atlas 200I/500 A2 推理产品 | x<!-- end id4 --> |
| <!-- npu="310p" id5 -->Atlas 推理系列产品 AI Core | x<!-- end id5 --> |
| <!-- npu="310p" id6 -->Atlas 推理系列产品 Vector Core | x<!-- end id6 --> |
| <!-- npu="910" id7 -->Atlas 训练系列产品 | x<!-- end id7 --> |

## 功能说明

头文件路径为：`"basic_api/kernel_operator_common_intf.h"`。

对 CTRL 寄存器（控制寄存器）的特定比特位进行设置。通过模板参数 startBit 和 endBit 指定目标比特位范围，将运行参数 value 写入 CTRL 寄存器对应位置。

## 函数原型

```cpp
template <int8_t startBit, int8_t endBit>
__aicore__ static inline void SetCtrlSpr(int64_t value)
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
| --- | --- |
| startBit | 起始比特位索引。 |
| endBit | 终止比特位索引。 |

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| value | 输入 | 起止比特位上新设置的值。 |

<!-- npu="950" id8 -->

**表3** Ascend 950PR/Ascend 950DT 常用 CTRL 寄存器比特位说明<a id="tab-950"></a>

| CTRL 寄存器比特位 | 功能 | 默认值 | 配合使用的 API |
| --- | --- | --- | --- |
| CTRL[8:6] | 用于控制数据从 L0C Buffer/Unified Buffer/L1 Buffer 搬运至 Global Memory 时原子操作的启用及数据类型选择。<br>&bull; 3'b000：不开启原子操作；<br>&bull; 3'b001：开启原子操作，数据类型为 float；<br>&bull; 3'b010：开启原子操作，数据类型为 half；<br>&bull; 3'b011：开启原子操作，数据类型为 int16_t；<br>&bull; 3'b100：开启原子操作，数据类型为 int32_t；<br>&bull; 3'b101：开启原子操作，数据类型为 int8_t；<br>&bull; 3'b110：开启原子操作，数据类型为 bfloat16_t。 | 3'b000 | 配合使用 API：<br>&bull; 数据搬运 API。 |
| CTRL[10:9] | 用于控制原子操作的类型，仅在 CTRL[8:6]开启原子操作时生效。<br>&bull; 2'b00：选择 ADD 操作；<br>&bull; 2'b01：选择 MAX 操作；<br>&bull; 2'b10：选择 MIN 操作。 | 2'b00 | 不涉及 |
| CTRL[45] | 用于控制左右矩阵数据做 Mmad 计算时的处理方式。<br>&bull; 1'b0：按照原数据类型进行处理；<br>&bull; 1'b1：左右矩阵数据均为 fp8_e4m3fn_t 时，数据视为 hifloat8_t 进行矩阵乘法计算。其他场景按照原数据类型进行处理。 | 1'b0 | 不涉及 |
| CTRL[48] | 用于控制浮点数计算和浮点数精度转换时的饱和模式，仅在 CTRL[60]开启时生效。<br>&bull; 1'b0：饱和模式，inf 输出会被饱和为±MAX，NaN 输出会被饱和为0；<br>&bull; 1'b1：非饱和模式，inf/NaN 保持原输出。<br><br>该控制位仅支持如下数据类型：<br>&bull;浮点数计算时支持 half 数据类型；<br>&bull;浮点数精度转换时支持如下数据类型：hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、half、bfloat16_t。 | 1'b0 | 配合使用的 API：<br>&bull; 矢量计算 API；<br>&bull; 原子操作 API；<br>&bull; 精度转换指令。<br><br>使用约束：<br>&bull; 需要满足数据类型限制。<br>&bull; 执行原子操作过程中，如果需要重新配置该控制位，需要调用[DataCacheCleanAndInvalid](../缓存控制/DataCacheCleanAndInvalid.md)先清除当前 Cache Line 状态并将当前数据写出，防止饱和模式变更影响当前数据。具体调用示例可参考[原子操作中，half 类型配置全局非饱和模式示例。](#example2)。 |
| CTRL[50] | 用于控制浮点数精度转换时的 NaN 饱和模式，在 CTRL[48]设置为饱和模式时生效。<br>&bull; 1'b0：NaN 输出会被转换为0.0；<br>&bull; 1'b1：NaN 输出会保持 NaN。<br><br>该控制位仅支持如下数据类型：<br>fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t。 | 1'b0 | 精度转换指令（需要满足数据类型限制）。 |
| CTRL[53] | 用于控制整数计算指令的饱和模式。<br>&bull; 1'b0：截断模式，溢出值按目标数据类型位数截断，保留低位，舍弃高位；<br>&bull; 1'b1：饱和模式，溢出值饱和到±MAX。 | 1'b0 | 矢量计算 API（输入输出数据类型为整数）。 |
| CTRL[59] | 用于控制浮点数转整数或整数转整数时的精度转换饱和模式，仅在 CTRL[60]开启时生效。<br>&bull; 1'b0：饱和模式：溢出值饱和到±MAX；<br>&bull; 1'b1：截断模式：溢出值按目标数据类型位数截断，保留低位，舍弃高位。 | 1'b0 | 精度转换指令。 |
| CTRL[60] | 用于控制饱和模式的全局生效方式。<br>&bull; 1'b0：单指令设置饱和；<br>&bull; 1'b1：全局设置饱和。 | 1'b1 | 该控制位可与 Reg 矢量计算 API [Cast](../Reg 矢量计算/类型转换/Cast.md)配合使用，或与 CTRL[48]、CTRL[59]配合使用，具体配置信息参考[Ascend 950PR/Ascend 950DT 饱和模式全局或单指令生效配置表](#table231122118201)。 |

<!-- end id8 -->

<!-- npu="A3" id9 -->

**表4** Atlas A3 训练系列产品/Atlas A3 推理系列产品常用 CTRL 寄存器比特位说明

| CTRL 寄存器比特位 | 功能 | 默认值 | 配合使用的 API |
| --- | --- | --- | --- |
| CTRL[48] | 用于控制浮点数计算和浮点数精度转换时的饱和模式。<br>&bull; 1'b0：饱和模式，inf 输出会被饱和为±MAX， NaN 输出会被饱和为0；<br>&bull; 1'b1：非饱和模式，inf/NaN 保持原输出。<br><br>该控制位仅支持如下数据类型：<br>&bull;浮点数计算时支持 half、bfloat16_t 数据类型；<br>&bull;浮点数精度转换时支持如下数据类型：half、bfloat16_t。 | 1'b0 | 不涉及 |

**注：针对 Atlas A3 训练系列产品/Atlas A3 推理系列产品，推荐使用[SetSaturationFlag](./SetSaturationFlag(ISASI).md)和[GetSaturationFlag](./GetSaturationFlag(ISASI).md)来设置和获取 CTRL[48]以控制饱和模式。**

<!-- end id9 -->

<!-- npu="910b" id10 -->

**表5** Atlas A2 训练系列产品/Atlas A2 推理系列产品常用 CTRL 寄存器比特位说明

| CTRL 寄存器比特位 | 功能 | 默认值 | 配合使用的 API |
| --- | --- | --- | --- |
| CTRL[48] | 用于控制浮点数计算和浮点数精度转换时的饱和模式。<br>&bull; 1'b0：饱和模式，inf 输出会被饱和为±MAX， NaN 输出会被饱和为0；<br>&bull; 1'b1：非饱和模式，inf/NaN 保持原输出。<br><br>该控制位仅支持如下数据类型：<br>&bull;浮点数计算时支持 half、bfloat16_t 数据类型；<br>&bull;浮点数精度转换时支持如下数据类型：half、bfloat16_t。 | 1'b0 | 不涉及 |

**注：针对 Atlas A2 训练系列产品/Atlas A2 推理系列产品，推荐使用[SetSaturationFlag](./SetSaturationFlag(ISASI).md)和[GetSaturationFlag](./GetSaturationFlag(ISASI).md)来设置和获取 CTRL[48]以控制饱和模式。**

<!-- end id10 -->

<!-- npu="950" id11 -->

**表6** Ascend 950PR/Ascend 950DT 饱和模式全局或单指令生效配置表<a id="table231122118201"></a>

| 全局开启位 | 控制位 | 功能描述 |
| --- | --- | --- |
| CTRL[60] = 1'b0 | Reg 矢量计算 Cast API 的 trait 模板参数中 satMode 设置为 SatMode::NO_SAT。 | 单指令非饱和模式。 |
| CTRL[60] = 1'b0 | Reg 矢量计算 Cast API 的 trait 模板参数中 satMode 设置为 SatMode::SAT。 | 单指令饱和模式。 |
| CTRL[60] = 1'b1 | CTRL[48] = 1'b1 | 全局非饱和模式（浮点数计算和浮点数精度转换）。 |
| CTRL[60] = 1'b1 | CTRL[48] = 1'b0 | 全局饱和模式（浮点数计算和浮点数精度转换）。 |
| CTRL[60] = 1'b1 | CTRL[59] = 1'b1 | 全局非饱和模式（浮点数转整数或整数转整数时的精度转换）。 |
| CTRL[60] = 1'b1 | CTRL[59] = 1'b0 | 全局饱和模式（浮点数转整数或整数转整数时的精度转换）。 |

<!-- end id11 -->

## 数据类型

本接口为寄存器访问接口，不涉及张量数据类型操作。参数 value 类型固定为 int64_t。

## 返回值说明

无

## 约束说明

- <!-- npu="950" id12 -->Ascend 950PR/Ascend 950DT 仅支持 CTRL[8:6]、CTRL[10:9]、CTRL[45]、CTRL[48]、CTRL[50]、CTRL[53]、CTRL[59]、CTRL[60]比特位。<!-- end id12 -->
- <!-- npu="A3" id13 -->Atlas A3 训练系列产品/Atlas A3 推理系列产品仅支持 CTRL[48]比特位。<!-- end id13 -->
- <!-- npu="910b" id14 -->Atlas A2 训练系列产品/Atlas A2 推理系列产品仅支持 CTRL[48]比特位。<!-- end id14 -->

<!-- npu="950" id15 -->

- Ascend 950PR/Ascend 950DT 对于 CTRL[8:6]和 CTRL[10:9]的设置，已封装原子操作 API，建议通过这些原子操作 API 进行配置。
  - [SetAtomicType](../原子操作/SetAtomicType.md)
  - [DisableDmaAtomic](../原子操作/DisableDmaAtomic.md)
  - [SetAtomicAdd](../原子操作/SetAtomicAdd.md)
  - [SetAtomicMax](../原子操作/SetAtomicMax(ISASI).md)
  - [SetAtomicMin](../原子操作/SetAtomicMin(ISASI).md)

<!-- end id15 -->

## 调用示例

- 如下示例中开启非饱和模式，数据类型为 half。

    ```cpp
    AscendC::SetCtrlSpr<48, 48>(1);
    ```

- <a name="example2"></a>原子操作中，half 类型配置全局非饱和模式示例。

    ```cpp
    AscendC::SetCtrlSpr<6, 8>(2);
    AscendC::SetAtomicAdd<half>();
    AscendC::DataCacheCleanAndInvalid<half, AscendC::CacheLine::ENTIRE_DATA_CACHE, AscendC::DcciDst::CACHELINE_ATOMIC>(dstTensor);
    AscendC::SetCtrlSpr<48, 48>(1);
    ...
    ```

更多示例请参考[CtrlSpr 样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/09_utils/ctrl_spr)。
