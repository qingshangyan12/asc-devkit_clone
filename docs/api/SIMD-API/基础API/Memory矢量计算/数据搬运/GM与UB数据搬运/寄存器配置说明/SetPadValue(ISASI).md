# SetPadValue\(ISASI\)<a name="ZH-CN_TOPIC_0000001786582454"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持 |
|------|----------|
| <cann-filter npu-type="950"><term>Ascend 950PR/Ascend 950DT</term> | √</cann-filter> |
| <cann-filter npu-type="A3"><term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | √</cann-filter> |
| <cann-filter npu-type="910b"><term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | √</cann-filter> |
| <cann-filter npu-type="310b"><term>Atlas 200I/500 A2 推理产品</term> | x</cann-filter> |
| <cann-filter npu-type="310p"><term>Atlas 推理系列产品 AI Core</term> | x</cann-filter> |
| <cann-filter npu-type="310p"><term>Atlas 推理系列产品 Vector Core</term> | x</cann-filter> |
| <cann-filter npu-type="910"><term>Atlas 训练系列产品</term> | x</cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

[DataCopyPad](GM2UB非对齐数据搬运(DataCopyPad).md)从Global Memory将数据非对齐搬运至UB时，可根据开发者的需要自行填充数据。SetPadValue用于设置DataCopyPad需要填充的数值。支持的通路如下：

-   GM-\>VECIN/GM-\>VECOUT

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename T, TPosition pos = TPosition::MAX>
__aicore__ inline void SetPadValue(T paddingValue)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| T | 输入 | 填充值的数据类型，与DataCopyPad接口搬运的数据类型一致。 |
| pos | 输入 | 用于指定DataCopyPad接口搬运过程中从GM搬运数据到哪一个目的地址，目的地址通过逻辑位置来表达。支持的取值为：TPosition::VECIN、TPosition::VECOUT、TPosition::MAX，默认值为TPosition::MAX，等效于TPosition::VECIN或TPosition::VECOUT。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :---: | :--- |
| paddingValue | 输入 | DataCopyPad接口填充的数值，数据类型与DataCopyPad接口搬运的数据类型一致。 |

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

无

## 调用示例<a name="section1227835243314"></a>

```
uint32_t m_n1 = 32;
uint32_t m_n2 = 31;
uint32_t m_n2Align = n2 % 32 == 0 ? n2 : (n2 / 32 + 1) * 32;

AscendC::LocalTensor<float> srcLocal = m_queInSrc.AllocTensor<float>();
AscendC::DataCopyExtParams dataCopyExtParams;
AscendC::DataCopyPadExtParams<float> padParams;

dataCopyExtParams.blockCount = m_n1; // Block个数，拷贝的的次数
dataCopyExtParams.blockLen = m_n2 * sizeof(float); // 一次拷贝多少个32B
dataCopyExtParams.srcStride = 0;
dataCopyExtParams.dstStride = 0;

padParams.isPad = true;
padParams.leftPadding = 0;
padParams.rightPadding = 1;

AscendC::SetPadValue((float)37); // 设置Pad的值为37
AscendC::DataCopyPad(srcLocal, m_srcGlobal, dataCopyExtParams, padParams);
```

```
输入数据（srcGm, shape = [32, 31]）：[[1, 1, 1, ..., 1], [1, 1, 1, ..., 1], ... , [1, 1, 1, ..., 1]]
输出数据（dstGm, shape = [32, 32]）：[[1, 1, 1, ..., 1, 37], [1, 1, 1, ..., 1, 37], ... , [1, 1, 1, ..., 1, 37]]
```

```
// 对于不支持使用立即数进行赋值和初始化的数据类型，如下是一个输入类型bfloat16_t的示例：
AscendC::SetPadValue(m_srcGlobal.GetValue(0));
AscendC::DataCopyPad(srcLocal, m_srcGlobal, dataCopyExtParams, padParams);

输入数据（srcGm, shape = [32, 31]）：[[1, 2, 3, ..., 31], [1, 2, 3, ..., 31], ... , [1, 2, 3, ..., 31]]
输出数据（dstGm, shape = [32, 32]）：[[1, 2, 3, ..., 31, 1], [1, 2, 3, ..., 31, 1], ... , [1, 2, 3, ..., 31, 1]]
```

完整样例请参考[DataCopyPad样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_pad_gm2ub_ub2gm)。
