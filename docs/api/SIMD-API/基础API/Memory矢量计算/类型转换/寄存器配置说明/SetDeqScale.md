# SetDeqScale<a name="ZH-CN_TOPIC_0000002573904763"></a>

## 产品支持情况<a name="zh-cn_topic_0000002563051145_section815614567413"></a>

<a name="zh-cn_topic_0000002563051145_table38301303189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002563051145_row20831180131817"><th class="cellrowborder" valign="top" width="57.99999999999999%" id="mcps1.1.3.1.1"><p id="zh-cn_topic_0000002563051145_p1883113061818"><a name="zh-cn_topic_0000002563051145_p1883113061818"></a><a name="zh-cn_topic_0000002563051145_p1883113061818"></a><span id="zh-cn_topic_0000002563051145_ph20833205312295"><a name="zh-cn_topic_0000002563051145_ph20833205312295"></a><a name="zh-cn_topic_0000002563051145_ph20833205312295"></a>产品</span></p>
</th>
<th class="cellrowborder" align="center" valign="top" width="42%" id="mcps1.1.3.1.2"><p id="zh-cn_topic_0000002563051145_p783113012187"><a name="zh-cn_topic_0000002563051145_p783113012187"></a><a name="zh-cn_topic_0000002563051145_p783113012187"></a>是否支持</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002563051145_row220181016240"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002563051145_p48327011813"><a name="zh-cn_topic_0000002563051145_p48327011813"></a><a name="zh-cn_topic_0000002563051145_p48327011813"></a><span id="zh-cn_topic_0000002563051145_ph583230201815"><a name="zh-cn_topic_0000002563051145_ph583230201815"></a><a name="zh-cn_topic_0000002563051145_ph583230201815"></a><term id="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term1253731311225"><a name="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term1253731311225"></a><a name="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term1253731311225"></a>Atlas A3 训练系列产品</term>/<term id="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term131434243115"><a name="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term131434243115"></a><a name="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term131434243115"></a>Atlas A3 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002563051145_p7948163910184"><a name="zh-cn_topic_0000002563051145_p7948163910184"></a><a name="zh-cn_topic_0000002563051145_p7948163910184"></a>√</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002563051145_row173226882415"><td class="cellrowborder" valign="top" width="57.99999999999999%" headers="mcps1.1.3.1.1 "><p id="zh-cn_topic_0000002563051145_p14832120181815"><a name="zh-cn_topic_0000002563051145_p14832120181815"></a><a name="zh-cn_topic_0000002563051145_p14832120181815"></a><span id="zh-cn_topic_0000002563051145_ph1483216010188"><a name="zh-cn_topic_0000002563051145_ph1483216010188"></a><a name="zh-cn_topic_0000002563051145_ph1483216010188"></a><term id="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term11962195213215"><a name="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term11962195213215"></a><a name="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term11962195213215"></a>Atlas A2 训练系列产品</term>/<term id="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term184716139811"><a name="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term184716139811"></a><a name="zh-cn_topic_0000002563051145_zh-cn_topic_0000001312391781_term184716139811"></a>Atlas A2 推理系列产品</term></span></p>
</td>
<td class="cellrowborder" align="center" valign="top" width="42%" headers="mcps1.1.3.1.2 "><p id="zh-cn_topic_0000002563051145_p19948143911820"><a name="zh-cn_topic_0000002563051145_p19948143911820"></a><a name="zh-cn_topic_0000002563051145_p19948143911820"></a>√</p>
</td>
</tr>
</tbody>
</table>

## 功能说明<a name="zh-cn_topic_0000002563051145_section748346451"></a>

头文件为\#include  "kernel\_operator\_vec\_vconv\_intf.h"

本接口用于设置DEQSCALE寄存器的值，DEQSCALE寄存器位宽为64bit，用于vector计算单元上的量化计算，寄存器中存放的参数在不同场景下的含义不同：

**表 1**  DEQSCALE寄存器比特位含义映射表

| 模式 | 比特位数 | 变量名 | 含义 |
|---|---|---|---|
| [AddDeqRelu](../../复合计算/AddDeqRelu.md#AddDeqRelu) / [Cast](Cast.md) / [CastDequant](../../复合计算/CastDequant.md#CastDequant)的s322f16场景 | 0~15 | scale | 一个half类型数据。 |
| CastDequant不使能向量量化的s162b8场景 | 0~31 | scale | 一个float类型数据 M（硬件在计算时将其视为 (1, 8, 10) 格式，即 1 个符号位、8 个指数位和 10 个尾数位）。 |
| CastDequant不使能向量量化的s162b8场景 | 37~45 | offset | 一个有符号的9位整数 |
| CastDequant不使能向量量化的s162b8场景 | 46 | signMode | 用于指示量化结果是否有符号（其中0表示无符号，1表示有符号） |
| CastDequant使能向量量化的s162b8场景 | 0~13 | vdeqAddr | 被硬件视为一个地址，指向UB中的一块空间，该空间大小为128B。空间被切分为16个64bit，每64bit可用于1次量化计算。 |

## 函数原型<a name="zh-cn_topic_0000002563051145_section21861260618"></a>

- 用于AddDeqRelu / Cast / CastDequant的s322f16场景

  ```cpp
  __aicore__ inline void SetDeqScale(half scale)
  ```

- 用于CastDequant不使能向量量化的s162b8场景

  ```cpp
  __aicore__ inline void SetDeqScale(float scale, int16_t offset, bool signMode)
  ```

- 用于CastDequant使能向量量化的s162b8场景

  ```cpp
  template <typename T>
  __aicore__ inline void SetDeqScale(const LocalTensor<T>& vdeq, const VdeqInfo& vdeqInfo)
  ```

## 参数说明<a name="zh-cn_topic_0000002563051145_section138547361666"></a>

**表 2**  模板参数说明

| 参数名 | 描述 |
| :-- | :-- |
| T | 输入量化Tensor的数据类型。支持的数据类型为uint64_t。 |

**表 3**  参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| scale | 输入 | scale量化参数。<br/>AddDeqRelu / Cast / CastDequant的s322f16场景下数据类型为half。<br/>CastDequant（isVecDeq=false）场景设置下数据类型为float。 |
| offset | 输入 | offset量化参数，int16_t类型，只有前9位有效。<br/>用于CastDequant（isVecDeq=false）的场景，设置offset。 |
| signMode | 输入 | bool类型，表示量化结果是否带符号。<br/>用于CastDequant（isVecDeq=false）的场景，设置signMode。 |
| vdeq | 输入 | 类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT。<br/>用于CastDequant（isVecDeq=true）的场景，输入量化tensor，大小为128Byte。 |
| vdeqInfo | 输入 | [VdeqInfo](#table4)类型，用于CastDequant（isVecDeq=true）的场景，结构体内包含量化tensor中的16组量化参数，接口会读取结构体内的参数，填充到vdeq。 |

**表 4**  VdeqInfo结构体内参数说明<a id="table4"></a>

| 参数名 | 描述 |
| :--- | :--- |
| vdeqScale | float类型的数组，用于存储量化tensor中的scale参数scale<sub>0</sub>-scale<sub>15</sub> |
| vdeqOffset | int16_t类型的数组，用于存储量化tensor中的offset参数offset<sub>0</sub>-offset<sub>15</sub> |
| vdeqSignMode | bool类型的数组，用于存储量化tensor中的signMode参数signMode<sub>0</sub>-signMode<sub>15</sub> |

## 约束说明<a name="zh-cn_topic_0000002563051145_section1323412155712"></a>

- offset量化参数，int16\_t类型，只有前9位有效。
- vdeq地址对齐约束参考[地址对齐约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section796754519912)。

## 调用示例<a name="zh-cn_topic_0000002563051145_section5349145316712"></a>

完整示例待补充。

- SetDeqScale(half scale)

  ```cpp
  // 配合Cast的s322f16场景使用
  // dstLocal为half类型的LocalTensor，srcLocal为int32_t类型的LocalTensor
  uint32_t srcSize = 256; // 参与计算的元素个数
  half scale = 1.0; // 量化参数为1
  AscendC::SetDeqScale(scale);
  // dst = src
  AscendC::Cast(dstLocal, srcLocal, AscendC::RoundMode::CAST_NONE, srcSize);
  ```

- SetDeqScale\(float scale, int16\_t offset, bool signMode\)

  ```cpp
  // 配合CastDeq（isVecDeq=false）场景使用
  // dstLocal为int8_t类型的LocalTensor，srcLocal为int16_t类型的LocalTensor
  uint32_t srcSize = 256; // 参与计算的元素个数
  float scale = 1.0; // 量化参数为1
  int16_t offset = 0; // 不带偏移
  bool signMode = true; // dstLocal为int8_t类型，为有符号数
  AscendC::SetDeqScale(scale, offset, signMode);
  // dst = src
  AscendC::CastDeq<int8_t, int16_t, false, false>(dstLocal, srcLocal, srcSize);
  ```
