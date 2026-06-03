# ResetLoopModePara<a name="ZH-CN_TOPIC_0000002380348618"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持  |
| :----------------------- | :------: |
| <cann-filter npu-type="950"><term>Ascend 950PR/Ascend 950DT</term> | √</cann-filter> |
| <cann-filter npu-type="A3"><term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | x</cann-filter> |
| <cann-filter npu-type="910b"><term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | x</cann-filter> |
| <cann-filter npu-type="310b"><term>Atlas 200I/500 A2 推理产品</term> | x</cann-filter> |
| <cann-filter npu-type="310p"><term>Atlas 推理系列产品 AI Core</term> | x</cann-filter> |
| <cann-filter npu-type="310p"><term>Atlas 推理系列产品 Vector Core</term> | x</cann-filter> |
| <cann-filter npu-type="910"><term>Atlas 训练系列产品</term> | x</cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_data_copy_intf.h"`。

重置loop mode的参数。与[SetLoopModePara](SetLoopModePara.md)搭配使用，在使能loop mode并且设置loop mode的参数的数据搬运场景下，数据搬运结束后需要调用该函数来重置loop mode参数。支持的通路如下：

- GM-\>VECIN
- VECOUT-\>GM

## 函数原型<a name="section620mcpsimp"></a>

```cpp
__aicore__ inline void ResetLoopModePara(DataCopyMVType type)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|----------|------|
| type | 输入 | 数据搬运模式。DataCopyMVType为枚举类型，定义如下，具体参数说明请参考<a href="SetLoopModePara.md#table1166074612214">表3</a>。<pre class="screen" codetype="Cpp">enum class DataCopyMVType : uint8_t {<br>    UB_TO_OUT = 0,<br>    OUT_TO_UB = 1,<br>};</pre> |

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- 与SetLoopModePara搭配使用。在使能loop mode并完成数据搬运后，必须调用该接口重置loop mode参数，否则会影响下一次对应通路的数据搬运，引发异常。

## 调用示例<a name="section1227835243314"></a>

本示例中操作数数据类型为int8\_t。

```cpp
AscendC::LocalTensor<int8_t> srcLocal = inQueueSrc.AllocTensor<int8_t>();
AscendC::DataCopyExtParams copyParams{2, 48 * sizeof(int8_t), 0, 0, 0}; // 结构体DataCopyExtParams最后一个参数是rsv保留位。
AscendC::DataCopyPadExtParams<half> padParams{false, 0, 0, 0};
AscendC::LoopModeParams loopParam2Ub {2, 2, 96, 128, 192, 288};
AscendC::SetLoopModePara(loopParam2Ub, DataCopyMVType::OUT_TO_UB);
AscendC::DataCopyPad<int8_t, PaddingMode::Compact>(srcLocal, srcGlobal, copyParams, padParams); // 从GM->VECIN搬运的数据总量为：48 * 2 * 2 * 2 = 384Bytes。
AscendC::ResetLoopModePara(DataCopyMVType::OUT_TO_UB);
AscendC::LoopModeParams loopParam2Gm {2, 2, 128, 96, 288, 192};
AscendC::SetLoopModePara(loopParam2Gm, DataCopyMVType::UB_TO_OUT);
DataCopyPad<T, PaddingMode::Compact>(dstGlobal, srcLocal, copyParams);
AscendC::ResetLoopModePara(DataCopyMVType::UB_TO_OUT);
```
