# ReducePairElem<a name="ZH-CN_TOPIC_0000002603544911"></a>

## 产品支持情况<a name="zh-cn_topic_0000002601742751_section796754519912"></a>

| 产品 | 是否支持 |
| ---- | -------- |
| <cann-filter npu-type = "950">Ascend 950PR/Ascend 950DT | √</cann-filter> |
| <cann-filter npu-type = "A3">Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √</cann-filter> |
| <cann-filter npu-type = "910b">Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √</cann-filter> |
| <cann-filter npu-type = "310b">Atlas 200I/500 A2 推理产品 | √</cann-filter> |
| <cann-filter npu-type = "310p">Atlas 推理系列产品AI Core | √</cann-filter> |
| <cann-filter npu-type = "310p">Atlas 推理系列产品Vector Core | x</cann-filter> |
| <cann-filter npu-type = "910">Atlas 训练系列产品 | √</cann-filter> |
| <cann-filter npu-type = "x90">Kirin X90 | √</cann-filter> |
| <cann-filter npu-type = "9030">Kirin 9030 | √</cann-filter> |

## 功能说明<a name="zh-cn_topic_0000002601742751_section106841136114319"></a>

头文件路径为：`"basic_api/kernel_operator_vec_reduce_intf.h"`。

ReducePairElem接口对相邻两个元素进行归约操作，根据模板参数reduceType，对两个元素求和/求最大值/求最小值，结果按顺序写入目标地址。**当前仅支持求和操作。**

例如，[a1, a2, a3, a4, a5, a6, ...] 的求和结果为 [a1+a2, a3+a4, a5+a6, ...]。计算过程如下图所示，灰色部分代表无效元素。

**图 1**  ReducePairElem<ReduceType::SUM\>示意图<a name="zh-cn_topic_0000002601742751_fig2615641133311"></a>  
![](figures/ReducePairElem-ReduceType-SUM-示意图.png "ReducePairElem-ReduceType-SUM-示意图")

## 函数原型<a name="zh-cn_topic_0000002601742751_section82039854412"></a>

-   mask逐bit模式

    ```
    template <ReduceType reduceType, typename T, typename U, bool isSetMask = true>
    __aicore__ inline void ReducePairElem(const LocalTensor<T>& dst, const LocalTensor<U>& src, const uint64_t mask[],const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

-   mask连续模式

    ```
    template <ReduceType reduceType, typename T, typename U, bool isSetMask = true>
    __aicore__ inline void ReducePairElem(const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t mask, const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride)
    ```

## 参数说明<a name="zh-cn_topic_0000002601742751_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002601742751_table1085443112919"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002601742751_row1185104317291"><th class="cellrowborder" valign="top" width="23.18%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002601742751_p178504310290"><a name="zh-cn_topic_0000002601742751_p178504310290"></a><a name="zh-cn_topic_0000002601742751_p178504310290"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="76.82%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002601742751_p1585134332912"><a name="zh-cn_topic_0000002601742751_p1585134332912"></a><a name="zh-cn_topic_0000002601742751_p1585134332912"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002601742751_row13689193610811"><td class="cellrowborder" valign="top" width="23.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002601742751_p197161530217"><a name="zh-cn_topic_0000002601742751_p197161530217"></a><a name="zh-cn_topic_0000002601742751_p197161530217"></a>reduceType</p>
</td>
<td class="cellrowborder" valign="top" width="76.82%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002601742751_p47161338218"><a name="zh-cn_topic_0000002601742751_p47161338218"></a><a name="zh-cn_topic_0000002601742751_p47161338218"></a>归约操作类型，ReduceType类型，当前仅支持SUM。</p>
<pre class="screen" id="zh-cn_topic_0000002601742751_screen029833194313"><a name="zh-cn_topic_0000002601742751_screen029833194313"></a><a name="zh-cn_topic_0000002601742751_screen029833194313"></a>enum class ReduceType {
    NONE = -1, // 无效类型，当前API不支持该类型
    SUM = 0,   // 求和
    MAX,       // 求最大值，当前API不支持该类型
    MIN,       // 求最小值，当前API不支持该类型
};</pre>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row14689193612810"><td class="cellrowborder" valign="top" width="23.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002601742751_zh-cn_topic_0000001429830437_p5835457165816"><a name="zh-cn_topic_0000002601742751_zh-cn_topic_0000001429830437_p5835457165816"></a><a name="zh-cn_topic_0000002601742751_zh-cn_topic_0000001429830437_p5835457165816"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="76.82%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002601742751_p948018536348"><a name="zh-cn_topic_0000002601742751_p948018536348"></a><a name="zh-cn_topic_0000002601742751_p948018536348"></a>目的操作数数据类型。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row76898367817"><td class="cellrowborder" valign="top" width="23.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002601742751_p15577134219316"><a name="zh-cn_topic_0000002601742751_p15577134219316"></a><a name="zh-cn_topic_0000002601742751_p15577134219316"></a>U</p>
</td>
<td class="cellrowborder" valign="top" width="76.82%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002601742751_p179431255143419"><a name="zh-cn_topic_0000002601742751_p179431255143419"></a><a name="zh-cn_topic_0000002601742751_p179431255143419"></a>源操作数数据类型。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row749445684"><td class="cellrowborder" valign="top" width="23.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002601742751_zh-cn_topic_0000001429830437_p1383515717581"><a name="zh-cn_topic_0000002601742751_zh-cn_topic_0000001429830437_p1383515717581"></a><a name="zh-cn_topic_0000002601742751_zh-cn_topic_0000001429830437_p1383515717581"></a>isSetMask</p>
</td>
<td class="cellrowborder" valign="top" width="76.82%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002601742751_p20440115575315"><a name="zh-cn_topic_0000002601742751_p20440115575315"></a><a name="zh-cn_topic_0000002601742751_p20440115575315"></a>是否在接口内部设置mask，默认为true。</p>
<a name="zh-cn_topic_0000002601742751_ul04084161544"></a><a name="zh-cn_topic_0000002601742751_ul04084161544"></a><ul id="zh-cn_topic_0000002601742751_ul04084161544"><li>true，表示在接口内部设置mask。</li><li>false，表示在接口外部设置mask，开发者需要使用<a href="SetVectorMask.md#ZH-CN_TOPIC_0000002574024783">SetVectorMask</a>接口设置mask值。这种模式下，本接口入参中的mask值必须设置为占位符MASK_PLACEHOLDER。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002601742751_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002601742751_row15956194105014"><th class="cellrowborder" valign="top" width="13.661366136613662%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002601742751_p7956144195014"><a name="zh-cn_topic_0000002601742751_p7956144195014"></a><a name="zh-cn_topic_0000002601742751_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="12.591259125912593%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002601742751_p1295624145013"><a name="zh-cn_topic_0000002601742751_p1295624145013"></a><a name="zh-cn_topic_0000002601742751_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="73.74737473747375%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002601742751_p16956144145011"><a name="zh-cn_topic_0000002601742751_p16956144145011"></a><a name="zh-cn_topic_0000002601742751_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002601742751_row5956546509"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002601742751_p284425844311"><a name="zh-cn_topic_0000002601742751_p284425844311"></a><a name="zh-cn_topic_0000002601742751_p284425844311"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002601742751_p158449584436"><a name="zh-cn_topic_0000002601742751_p158449584436"></a><a name="zh-cn_topic_0000002601742751_p158449584436"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002601742751_p14372104925312"><a name="zh-cn_topic_0000002601742751_p14372104925312"></a><a name="zh-cn_topic_0000002601742751_p14372104925312"></a>目的操作数。</p>
<p id="zh-cn_topic_0000002601742751_p1737216493533"><a name="zh-cn_topic_0000002601742751_p1737216493533"></a><a name="zh-cn_topic_0000002601742751_p1737216493533"></a>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT。</p>
<p id="zh-cn_topic_0000002601742751_p85402043466"><a name="zh-cn_topic_0000002601742751_p85402043466"></a><a name="zh-cn_topic_0000002601742751_p85402043466"></a>LocalTensor的起始地址需要32字节对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row4956154125018"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002601742751_p142871414131614"><a name="zh-cn_topic_0000002601742751_p142871414131614"></a><a name="zh-cn_topic_0000002601742751_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002601742751_p628711148165"><a name="zh-cn_topic_0000002601742751_p628711148165"></a><a name="zh-cn_topic_0000002601742751_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002601742751_p0287191420164"><a name="zh-cn_topic_0000002601742751_p0287191420164"></a><a name="zh-cn_topic_0000002601742751_p0287191420164"></a>源操作数。</p>
<p id="zh-cn_topic_0000002601742751_p334081584319"><a name="zh-cn_topic_0000002601742751_p334081584319"></a><a name="zh-cn_topic_0000002601742751_p334081584319"></a>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT。</p>
<p id="zh-cn_topic_0000002601742751_p1121905571420"><a name="zh-cn_topic_0000002601742751_p1121905571420"></a><a name="zh-cn_topic_0000002601742751_p1121905571420"></a>LocalTensor的起始地址需要32字节对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row104381524115610"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002601742751_p183123816565"><a name="zh-cn_topic_0000002601742751_p183123816565"></a><a name="zh-cn_topic_0000002601742751_p183123816565"></a>mask[]/mask</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002601742751_p1631193825611"><a name="zh-cn_topic_0000002601742751_p1631193825611"></a><a name="zh-cn_topic_0000002601742751_p1631193825611"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002601742751_p94543261085"><a name="zh-cn_topic_0000002601742751_p94543261085"></a><a name="zh-cn_topic_0000002601742751_p94543261085"></a>mask用于控制每次迭代内参与计算的元素。详细设置参考<a href="掩码.md#ZH-CN_TOPIC_0000002573904669">掩码</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row11414102418567"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002601742751_p028761412166"><a name="zh-cn_topic_0000002601742751_p028761412166"></a><a name="zh-cn_topic_0000002601742751_p028761412166"></a>repeatTime</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002601742751_p1928716142163"><a name="zh-cn_topic_0000002601742751_p1928716142163"></a><a name="zh-cn_topic_0000002601742751_p1928716142163"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002601742751_p353564621520"><a name="zh-cn_topic_0000002601742751_p353564621520"></a><a name="zh-cn_topic_0000002601742751_p353564621520"></a>重复迭代次数。</p>
<p id="zh-cn_topic_0000002601742751_p15733456113418"><a name="zh-cn_topic_0000002601742751_p15733456113418"></a><a name="zh-cn_topic_0000002601742751_p15733456113418"></a>矢量计算单元，每次读取连续的256Bytes数据进行计算，为完成对输入数据的处理，必须通过多次迭代（repeat）才能完成所有数据的读取与计算。repeatTime表示迭代的次数。</p>
<p id="zh-cn_topic_0000002601742751_p585315111226"><a name="zh-cn_topic_0000002601742751_p585315111226"></a><a name="zh-cn_topic_0000002601742751_p585315111226"></a>关于该参数的具体描述请参考<a href="高维切分.md#ZH-CN_TOPIC_0000002556644319">高维切分</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row1443383222216"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002601742751_p19575838172212"><a name="zh-cn_topic_0000002601742751_p19575838172212"></a><a name="zh-cn_topic_0000002601742751_p19575838172212"></a>dstRepStride</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002601742751_p35752385220"><a name="zh-cn_topic_0000002601742751_p35752385220"></a><a name="zh-cn_topic_0000002601742751_p35752385220"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002601742751_p14319182111213"><a name="zh-cn_topic_0000002601742751_p14319182111213"></a><a name="zh-cn_topic_0000002601742751_p14319182111213"></a>目的操作数相邻迭代间的地址步长，以一个repeatTime归约后的长度为单位。取值范围为[0, 2<sup id="zh-cn_topic_0000002601742751_zh-cn_topic_0000001530181537_sup181195111019"><a name="zh-cn_topic_0000002601742751_zh-cn_topic_0000001530181537_sup181195111019"></a><a name="zh-cn_topic_0000002601742751_zh-cn_topic_0000001530181537_sup181195111019"></a>16</sup>-1]。</p>
<p id="zh-cn_topic_0000002601742751_p10115165915149"><a name="zh-cn_topic_0000002601742751_p10115165915149"></a><a name="zh-cn_topic_0000002601742751_p10115165915149"></a>注：dstRepStride单位为128字节。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row548531101613"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002601742751_p62871914191620"><a name="zh-cn_topic_0000002601742751_p62871914191620"></a><a name="zh-cn_topic_0000002601742751_p62871914191620"></a>srcBlkStride</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002601742751_p5287181416164"><a name="zh-cn_topic_0000002601742751_p5287181416164"></a><a name="zh-cn_topic_0000002601742751_p5287181416164"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002601742751_p96745398125"><a name="zh-cn_topic_0000002601742751_p96745398125"></a><a name="zh-cn_topic_0000002601742751_p96745398125"></a>单次迭代内DataBlock的地址步长，单位为32字节。取值范围为[0, 2<sup id="zh-cn_topic_0000002601742751_sup7642102165215"><a name="zh-cn_topic_0000002601742751_sup7642102165215"></a><a name="zh-cn_topic_0000002601742751_sup7642102165215"></a>16</sup>-1]。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002601742751_row1774899165"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002601742751_p15288514111614"><a name="zh-cn_topic_0000002601742751_p15288514111614"></a><a name="zh-cn_topic_0000002601742751_p15288514111614"></a>srcRepStride</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002601742751_p62888148164"><a name="zh-cn_topic_0000002601742751_p62888148164"></a><a name="zh-cn_topic_0000002601742751_p62888148164"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002601742751_p1624214011488"><a name="zh-cn_topic_0000002601742751_p1624214011488"></a><a name="zh-cn_topic_0000002601742751_p1624214011488"></a>源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的DataBlock数目。取值范围为[0, 2<sup id="zh-cn_topic_0000002601742751_sup15498122516521"><a name="zh-cn_topic_0000002601742751_sup15498122516521"></a><a name="zh-cn_topic_0000002601742751_sup15498122516521"></a>16</sup>-1]。</p>
</td>
</tr>
</tbody>
</table>

**注：以上高维切分相关参数mask，repeatTime，dstRepStride，srcBlkStride，srcRepStride请参考[高维切分](高维切分.md#ZH-CN_TOPIC_0000002556644319)中的介绍。**

## 数据类型<a name="zh-cn_topic_0000002601742751_section4219135304818"></a>

操作数支持的数据类型为：half/float。

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品，目的操作数与源操作数的数据类型需要保持一致。

## API映射关系<a name="zh-cn_topic_0000002601742751_section5469132317498"></a>

与built-in接口映射关系：

PairReduceSum接口在built-in接口（set\_vector\_mask和vcpadd）的基础上进行抽象封装实现的，其对应的底层build-in接口为：

```
// 设置mask
void set_vector_mask(uint64_t mask1, uint64_t mask0);

// half数据类型输入
void vcpadd(__ubuf__ half *dst, __ubuf__ half *src, uint8_t repeat, uint16_t dstBlockStride, uint16_t srcBlockStride, uint16_t srcRepeatStride);

// float数据类型输入
void vcpadd(__ubuf__ float *dst, __ubuf__ float *src, uint8_t repeat, uint16_t dstBlockStride, uint16_t srcBlockStride, uint16_t srcRepeatStride);
```

## 约束说明<a name="zh-cn_topic_0000002601742751_section2045914466492"></a>

-   操作数地址重叠约束请参考[地址重叠约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section668772811100)。
-   操作数地址对齐约束请参考[地址对齐约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section796754519912)。

-   repeatTime = 0表示不会执行归约操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。
-   当一对相邻元素的掩码均为0，其对应的目的元素将跳过写入，保持不变。
-   dstRepStride, srcBlkStride, srcRepStride取值范围为\[0, 2<sup>16</sup>-1\]，需要结合UB的实际大小避免出现越界。
-   针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品，目的操作数与源操作数的数据类型需要保持一致。

## 调用示例<a name="zh-cn_topic_0000002601742751_section088124295117"></a>

完整示例可参考[PairReduceSum样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/pair_reduce_sum)。
