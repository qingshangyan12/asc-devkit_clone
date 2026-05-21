# ReduceRepeat<a name="ZH-CN_TOPIC_0000002572705828"></a>

## 产品支持情况<a name="zh-cn_topic_0000002599326429_section796754519912"></a>

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

## 功能说明<a name="zh-cn_topic_0000002599326429_section106841136114319"></a>

头文件路径为：`"basic_api/kernel_operator_vec_reduce_intf.h"`。

ReduceRepeat接口用于对每个repeat内所有数据进行归约操作，根据模板参数reduceType进行求和/求最大值/求最小值操作，结果按顺序写入目标地址。

在求最大值或最小值时可以返回对应的索引，返回的索引值为每个repeat内部索引。支持高维切分计算。

**图 1**  ReduceRepeat计算示意图<a name="zh-cn_topic_0000002599326429_fig7302181114201"></a>  
![](figures/ReduceRepeat计算示意图.png "ReduceRepeat计算示意图")

完整示例请参考：[ReduceRepeat样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。

## 函数原型<a name="zh-cn_topic_0000002599326429_section82039854412"></a>

- mask逐bit模式

    ```cpp
    template <ReduceType reduceType, typename T, typename U, bool isSetMask = true>
    __aicore__ inline void ReduceRepeat(const LocalTensor<T>& dst, const LocalTensor<U>& src, const uint64_t mask[], const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride, ReduceOrder order = ReduceOrder::ORDER_VALUE_INDEX)
    ```

- mask连续模式

    ```cpp
    template <ReduceType reduceType, typename T, typename U, bool isSetMask = true>
    __aicore__ inline void ReduceRepeat(const LocalTensor<T>& dst, const LocalTensor<U>& src, const int32_t mask, const int32_t repeatTime, const int32_t dstRepStride, const int32_t srcBlkStride, const int32_t srcRepStride, ReduceOrder order = ReduceOrder::ORDER_VALUE_INDEX)
    ```

## 参数说明<a name="zh-cn_topic_0000002599326429_section16128134420472"></a>

**表 1**  模板参数说明

<a name="zh-cn_topic_0000002599326429_table1085443112919"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002599326429_row1185104317291"><th class="cellrowborder" valign="top" width="23.18%" id="mcps1.2.3.1.1"><p id="zh-cn_topic_0000002599326429_p178504310290"><a name="zh-cn_topic_0000002599326429_p178504310290"></a><a name="zh-cn_topic_0000002599326429_p178504310290"></a>参数名</p>
</th>
<th class="cellrowborder" valign="top" width="76.82%" id="mcps1.2.3.1.2"><p id="zh-cn_topic_0000002599326429_p1585134332912"><a name="zh-cn_topic_0000002599326429_p1585134332912"></a><a name="zh-cn_topic_0000002599326429_p1585134332912"></a>描述</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002599326429_row640018521344"><td class="cellrowborder" valign="top" width="23.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002599326429_p197161530217"><a name="zh-cn_topic_0000002599326429_p197161530217"></a><a name="zh-cn_topic_0000002599326429_p197161530217"></a>reduceType</p>
</td>
<td class="cellrowborder" valign="top" width="76.82%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002599326429_p47161338218"><a name="zh-cn_topic_0000002599326429_p47161338218"></a><a name="zh-cn_topic_0000002599326429_p47161338218"></a>归约操作类型，ReduceType类型，支持SUM、MAX、MIN。</p>
<pre class="screen" id="zh-cn_topic_0000002599326429_screen029833194313"><a name="zh-cn_topic_0000002599326429_screen029833194313"></a><a name="zh-cn_topic_0000002599326429_screen029833194313"></a>enum class ReduceType {
    NONE = -1, // 无效类型，当前API不支持该类型
    SUM = 0,   // 求和
    MAX,       // 求最大值
    MIN,       // 求最小值
};</pre>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row194001352103414"><td class="cellrowborder" valign="top" width="23.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_p5835457165816"><a name="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_p5835457165816"></a><a name="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_p5835457165816"></a>T</p>
</td>
<td class="cellrowborder" valign="top" width="76.82%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002599326429_p14399237308"><a name="zh-cn_topic_0000002599326429_p14399237308"></a><a name="zh-cn_topic_0000002599326429_p14399237308"></a>目的操作数数据类型。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row140065215346"><td class="cellrowborder" valign="top" width="23.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002599326429_p15577134219316"><a name="zh-cn_topic_0000002599326429_p15577134219316"></a><a name="zh-cn_topic_0000002599326429_p15577134219316"></a>U</p>
</td>
<td class="cellrowborder" valign="top" width="76.82%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002599326429_p1457734215312"><a name="zh-cn_topic_0000002599326429_p1457734215312"></a><a name="zh-cn_topic_0000002599326429_p1457734215312"></a>源操作数数据类型。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row13400125214341"><td class="cellrowborder" valign="top" width="23.18%" headers="mcps1.2.3.1.1 "><p id="zh-cn_topic_0000002599326429_p7398155263413"><a name="zh-cn_topic_0000002599326429_p7398155263413"></a><a name="zh-cn_topic_0000002599326429_p7398155263413"></a>isSetMask</p>
</td>
<td class="cellrowborder" valign="top" width="76.82%" headers="mcps1.2.3.1.2 "><p id="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_p77520541653"><a name="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_p77520541653"></a><a name="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_p77520541653"></a>是否在接口内部设置mask。</p>
<a name="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_ul1163765616511"></a><a name="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_ul1163765616511"></a><ul id="zh-cn_topic_0000002599326429_zh-cn_topic_0000001429830437_ul1163765616511"><li>true，表示在接口内部设置mask。</li><li>false，表示在接口外部设置mask，开发者需要使用<a href="SetVectorMask.md#ZH-CN_TOPIC_0000002574024783">SetVectorMask</a>接口设置mask值。这种模式下，本接口入参中的mask值必须设置为占位符MASK_PLACEHOLDER。</li></ul>
</td>
</tr>
</tbody>
</table>

**表 2**  参数说明

<a name="zh-cn_topic_0000002599326429_table8955841508"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002599326429_row15956194105014"><th class="cellrowborder" valign="top" width="13.661366136613662%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002599326429_p7956144195014"><a name="zh-cn_topic_0000002599326429_p7956144195014"></a><a name="zh-cn_topic_0000002599326429_p7956144195014"></a>参数名称</p>
</th>
<th class="cellrowborder" valign="top" width="12.591259125912593%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002599326429_p1295624145013"><a name="zh-cn_topic_0000002599326429_p1295624145013"></a><a name="zh-cn_topic_0000002599326429_p1295624145013"></a>输入/输出</p>
</th>
<th class="cellrowborder" valign="top" width="73.74737473747375%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002599326429_p16956144145011"><a name="zh-cn_topic_0000002599326429_p16956144145011"></a><a name="zh-cn_topic_0000002599326429_p16956144145011"></a>含义</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002599326429_row5956546509"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002599326429_p284425844311"><a name="zh-cn_topic_0000002599326429_p284425844311"></a><a name="zh-cn_topic_0000002599326429_p284425844311"></a>dst</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002599326429_p158449584436"><a name="zh-cn_topic_0000002599326429_p158449584436"></a><a name="zh-cn_topic_0000002599326429_p158449584436"></a>输出</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002599326429_p14372104925312"><a name="zh-cn_topic_0000002599326429_p14372104925312"></a><a name="zh-cn_topic_0000002599326429_p14372104925312"></a>目的操作数。</p>
<p id="zh-cn_topic_0000002599326429_p1737216493533"><a name="zh-cn_topic_0000002599326429_p1737216493533"></a><a name="zh-cn_topic_0000002599326429_p1737216493533"></a>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT。</p>
<p id="zh-cn_topic_0000002599326429_p0372749125320"><a name="zh-cn_topic_0000002599326429_p0372749125320"></a><a name="zh-cn_topic_0000002599326429_p0372749125320"></a>注：LocalTensor的起始地址对齐要求参考<a href="#zh-cn_topic_0000002599326429_table16987516135013">表3</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row4956154125018"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002599326429_p142871414131614"><a name="zh-cn_topic_0000002599326429_p142871414131614"></a><a name="zh-cn_topic_0000002599326429_p142871414131614"></a>src</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002599326429_p628711148165"><a name="zh-cn_topic_0000002599326429_p628711148165"></a><a name="zh-cn_topic_0000002599326429_p628711148165"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002599326429_p0287191420164"><a name="zh-cn_topic_0000002599326429_p0287191420164"></a><a name="zh-cn_topic_0000002599326429_p0287191420164"></a>源操作数。</p>
<p id="zh-cn_topic_0000002599326429_p334081584319"><a name="zh-cn_topic_0000002599326429_p334081584319"></a><a name="zh-cn_topic_0000002599326429_p334081584319"></a>类型为LocalTensor，支持的TPosition为VECIN/VECCALC/VECOUT。</p>
<p id="zh-cn_topic_0000002599326429_p85402043466"><a name="zh-cn_topic_0000002599326429_p85402043466"></a><a name="zh-cn_topic_0000002599326429_p85402043466"></a>LocalTensor的起始地址需要32字节对齐。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row104381524115610"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002599326429_p183123816565"><a name="zh-cn_topic_0000002599326429_p183123816565"></a><a name="zh-cn_topic_0000002599326429_p183123816565"></a>mask[]/mask</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002599326429_p1631193825611"><a name="zh-cn_topic_0000002599326429_p1631193825611"></a><a name="zh-cn_topic_0000002599326429_p1631193825611"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002599326429_p94543261085"><a name="zh-cn_topic_0000002599326429_p94543261085"></a><a name="zh-cn_topic_0000002599326429_p94543261085"></a>mask用于控制每次迭代内参与计算的元素。详细设置参考<a href="掩码.md#ZH-CN_TOPIC_0000002573904669">掩码</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row11414102418567"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002599326429_p028761412166"><a name="zh-cn_topic_0000002599326429_p028761412166"></a><a name="zh-cn_topic_0000002599326429_p028761412166"></a>repeatTime</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002599326429_p1928716142163"><a name="zh-cn_topic_0000002599326429_p1928716142163"></a><a name="zh-cn_topic_0000002599326429_p1928716142163"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002599326429_p7856291148"><a name="zh-cn_topic_0000002599326429_p7856291148"></a><a name="zh-cn_topic_0000002599326429_p7856291148"></a>重复迭代次数。</p>
<p id="zh-cn_topic_0000002599326429_p18568951410"><a name="zh-cn_topic_0000002599326429_p18568951410"></a><a name="zh-cn_topic_0000002599326429_p18568951410"></a>矢量计算单元，每次读取连续的256Bytes数据进行计算，为完成对输入数据的处理，必须通过多次迭代（repeat）才能完成所有数据的读取与计算。repeatTime表示迭代的次数。</p>
<p id="zh-cn_topic_0000002599326429_p208566981416"><a name="zh-cn_topic_0000002599326429_p208566981416"></a><a name="zh-cn_topic_0000002599326429_p208566981416"></a>关于该参数的具体描述请参考<a href="高维切分.md#ZH-CN_TOPIC_0000002556644319">高维切分</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row1443383222216"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002599326429_p19575838172212"><a name="zh-cn_topic_0000002599326429_p19575838172212"></a><a name="zh-cn_topic_0000002599326429_p19575838172212"></a>dstRepStride</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002599326429_p35752385220"><a name="zh-cn_topic_0000002599326429_p35752385220"></a><a name="zh-cn_topic_0000002599326429_p35752385220"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002599326429_p14319182111213"><a name="zh-cn_topic_0000002599326429_p14319182111213"></a><a name="zh-cn_topic_0000002599326429_p14319182111213"></a>目的操作数相邻迭代间的地址步长，以一个repeatTime归约后的长度为单位。取值范围为[0, 2<sup id="zh-cn_topic_0000002599326429_zh-cn_topic_0000001530181537_sup181195111019"><a name="zh-cn_topic_0000002599326429_zh-cn_topic_0000001530181537_sup181195111019"></a><a name="zh-cn_topic_0000002599326429_zh-cn_topic_0000001530181537_sup181195111019"></a>16</sup>-1]。</p>
<p id="zh-cn_topic_0000002599326429_p10424194314163"><a name="zh-cn_topic_0000002599326429_p10424194314163"></a><a name="zh-cn_topic_0000002599326429_p10424194314163"></a>注：dstRepStride的单位受order参数的影响，参考<a href="#zh-cn_topic_0000002599326429_table16987516135013">表3</a>。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row548531101613"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002599326429_p62871914191620"><a name="zh-cn_topic_0000002599326429_p62871914191620"></a><a name="zh-cn_topic_0000002599326429_p62871914191620"></a>srcBlkStride</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002599326429_p5287181416164"><a name="zh-cn_topic_0000002599326429_p5287181416164"></a><a name="zh-cn_topic_0000002599326429_p5287181416164"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002599326429_p96745398125"><a name="zh-cn_topic_0000002599326429_p96745398125"></a><a name="zh-cn_topic_0000002599326429_p96745398125"></a>单次迭代内DataBlock的地址步长，单位为32字节。取值范围为[0, 2<sup id="zh-cn_topic_0000002599326429_sup7642102165215"><a name="zh-cn_topic_0000002599326429_sup7642102165215"></a><a name="zh-cn_topic_0000002599326429_sup7642102165215"></a>16</sup>-1]。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row1774899165"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002599326429_p15288514111614"><a name="zh-cn_topic_0000002599326429_p15288514111614"></a><a name="zh-cn_topic_0000002599326429_p15288514111614"></a>srcRepStride</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002599326429_p62888148164"><a name="zh-cn_topic_0000002599326429_p62888148164"></a><a name="zh-cn_topic_0000002599326429_p62888148164"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002599326429_p1624214011488"><a name="zh-cn_topic_0000002599326429_p1624214011488"></a><a name="zh-cn_topic_0000002599326429_p1624214011488"></a>源操作数相邻迭代间的地址步长，即源操作数每次迭代跳过的DataBlock数目。取值范围为[0, 2<sup id="zh-cn_topic_0000002599326429_sup15498122516521"><a name="zh-cn_topic_0000002599326429_sup15498122516521"></a><a name="zh-cn_topic_0000002599326429_sup15498122516521"></a>16</sup>-1]。</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row6312164523511"><td class="cellrowborder" valign="top" width="13.661366136613662%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002599326429_p4312145173512"><a name="zh-cn_topic_0000002599326429_p4312145173512"></a><a name="zh-cn_topic_0000002599326429_p4312145173512"></a>order</p>
</td>
<td class="cellrowborder" valign="top" width="12.591259125912593%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002599326429_p23126457357"><a name="zh-cn_topic_0000002599326429_p23126457357"></a><a name="zh-cn_topic_0000002599326429_p23126457357"></a>输入</p>
</td>
<td class="cellrowborder" valign="top" width="73.74737473747375%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002599326429_p10761173414524"><a name="zh-cn_topic_0000002599326429_p10761173414524"></a><a name="zh-cn_topic_0000002599326429_p10761173414524"></a>在归约操作类型为MAX或MIN时，指定dst中最大值value和索引值index的相对位置以及返回结果行为，参数类型为ReduceOrder枚举类型，默认值为ORDER_VALUE_INDEX。</p>
<p id="zh-cn_topic_0000002599326429_p24413312467"><a name="zh-cn_topic_0000002599326429_p24413312467"></a><a name="zh-cn_topic_0000002599326429_p24413312467"></a>ReduceOrder取值如下：</p>
<a name="zh-cn_topic_0000002599326429_ul19429814578"></a><a name="zh-cn_topic_0000002599326429_ul19429814578"></a><ul id="zh-cn_topic_0000002599326429_ul19429814578"><li>ORDER_VALUE_INDEX：表示value位于低半部，返回结果存储顺序为[value, index]。</li><li>ORDER_INDEX_VALUE：表示index位于低半部，返回结果存储顺序为[index, value]。</li><li>ORDER_ONLY_VALUE：表示只返回最值，返回结果存储顺序为[value]。</li><li>ORDER_ONLY_INDEX：表示只返回最值索引，返回结果存储顺序为[index]。</li></ul>
<p id="zh-cn_topic_0000002599326429_p8867938135216"><a name="zh-cn_topic_0000002599326429_p8867938135216"></a><a name="zh-cn_topic_0000002599326429_p8867938135216"></a><strong id="zh-cn_topic_0000002599326429_b1099712218532"><a name="zh-cn_topic_0000002599326429_b1099712218532"></a><a name="zh-cn_topic_0000002599326429_b1099712218532"></a>注：归约操作类型为SUM时，该参数不生效。</strong></p>
</td>
</tr>
</tbody>
</table>

**注：以上高维切分相关参数mask，repeatTime，dstRepStride，srcBlkStride，srcRepStride请参考[高维切分](高维切分.md#ZH-CN_TOPIC_0000002556644319)中的介绍。**

**表 3**  地址对齐和步长单位

<a name="zh-cn_topic_0000002599326429_table16987516135013"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002599326429_row11988111615501"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002599326429_p576333517507"><a name="zh-cn_topic_0000002599326429_p576333517507"></a><a name="zh-cn_topic_0000002599326429_p576333517507"></a>归约操作类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002599326429_p176343585012"><a name="zh-cn_topic_0000002599326429_p176343585012"></a><a name="zh-cn_topic_0000002599326429_p176343585012"></a>数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002599326429_p57631035105013"><a name="zh-cn_topic_0000002599326429_p57631035105013"></a><a name="zh-cn_topic_0000002599326429_p57631035105013"></a>dst地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002599326429_p157638353509"><a name="zh-cn_topic_0000002599326429_p157638353509"></a><a name="zh-cn_topic_0000002599326429_p157638353509"></a>dstRepStride单位</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002599326429_row798881610503"><td class="cellrowborder" rowspan="2" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002599326429_p756472120501"><a name="zh-cn_topic_0000002599326429_p756472120501"></a><a name="zh-cn_topic_0000002599326429_p756472120501"></a>SUM</p>
<p id="zh-cn_topic_0000002599326429_p156412115509"><a name="zh-cn_topic_0000002599326429_p156412115509"></a><a name="zh-cn_topic_0000002599326429_p156412115509"></a></p>
<p id="zh-cn_topic_0000002599326429_p15566172115019"><a name="zh-cn_topic_0000002599326429_p15566172115019"></a><a name="zh-cn_topic_0000002599326429_p15566172115019"></a></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002599326429_p15564621155018"><a name="zh-cn_topic_0000002599326429_p15564621155018"></a><a name="zh-cn_topic_0000002599326429_p15564621155018"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002599326429_p14877122975015"><a name="zh-cn_topic_0000002599326429_p14877122975015"></a><a name="zh-cn_topic_0000002599326429_p14877122975015"></a>2B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002599326429_p1187752975010"><a name="zh-cn_topic_0000002599326429_p1187752975010"></a><a name="zh-cn_topic_0000002599326429_p1187752975010"></a>2B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row1698831613504"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002599326429_p1565921195013"><a name="zh-cn_topic_0000002599326429_p1565921195013"></a><a name="zh-cn_topic_0000002599326429_p1565921195013"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002599326429_p1687782917508"><a name="zh-cn_topic_0000002599326429_p1687782917508"></a><a name="zh-cn_topic_0000002599326429_p1687782917508"></a>4B</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002599326429_p4877142915015"><a name="zh-cn_topic_0000002599326429_p4877142915015"></a><a name="zh-cn_topic_0000002599326429_p4877142915015"></a>4B</p>
</td>
</tr>
</tbody>
</table>

<a name="zh-cn_topic_0000002599326429_table16937759189"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002599326429_row10938145191814"><th class="cellrowborder" valign="top" width="16.66833316668333%" id="mcps1.1.6.1.1"><p id="zh-cn_topic_0000002599326429_p1587484719486"><a name="zh-cn_topic_0000002599326429_p1587484719486"></a><a name="zh-cn_topic_0000002599326429_p1587484719486"></a>归约操作类型</p>
</th>
<th class="cellrowborder" valign="top" width="16.66833316668333%" id="mcps1.1.6.1.2"><p id="zh-cn_topic_0000002599326429_p14938156189"><a name="zh-cn_topic_0000002599326429_p14938156189"></a><a name="zh-cn_topic_0000002599326429_p14938156189"></a>数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="16.66833316668333%" id="mcps1.1.6.1.3"><p id="zh-cn_topic_0000002599326429_p9938125161814"><a name="zh-cn_topic_0000002599326429_p9938125161814"></a><a name="zh-cn_topic_0000002599326429_p9938125161814"></a>order</p>
</th>
<th class="cellrowborder" valign="top" width="24.997500249975%" id="mcps1.1.6.1.4"><p id="zh-cn_topic_0000002599326429_p693914510189"><a name="zh-cn_topic_0000002599326429_p693914510189"></a><a name="zh-cn_topic_0000002599326429_p693914510189"></a>dst地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="24.997500249975%" id="mcps1.1.6.1.5"><p id="zh-cn_topic_0000002599326429_p593920515180"><a name="zh-cn_topic_0000002599326429_p593920515180"></a><a name="zh-cn_topic_0000002599326429_p593920515180"></a>dstRepStride单位</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002599326429_row693914571810"><td class="cellrowborder" rowspan="8" valign="top" width="16.66833316668333%" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002599326429_p6611152295010"><a name="zh-cn_topic_0000002599326429_p6611152295010"></a><a name="zh-cn_topic_0000002599326429_p6611152295010"></a>MAX/MIN</p>
</td>
<td class="cellrowborder" rowspan="4" valign="top" width="16.66833316668333%" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002599326429_p2939185131816"><a name="zh-cn_topic_0000002599326429_p2939185131816"></a><a name="zh-cn_topic_0000002599326429_p2939185131816"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="16.66833316668333%" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002599326429_p79391954186"><a name="zh-cn_topic_0000002599326429_p79391954186"></a><a name="zh-cn_topic_0000002599326429_p79391954186"></a>ORDER_VALUE_INDEX</p>
</td>
<td class="cellrowborder" valign="top" width="24.997500249975%" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0000002599326429_p5939135121816"><a name="zh-cn_topic_0000002599326429_p5939135121816"></a><a name="zh-cn_topic_0000002599326429_p5939135121816"></a>4B</p>
</td>
<td class="cellrowborder" valign="top" width="24.997500249975%" headers="mcps1.1.6.1.5 "><p id="zh-cn_topic_0000002599326429_p9841055102715"><a name="zh-cn_topic_0000002599326429_p9841055102715"></a><a name="zh-cn_topic_0000002599326429_p9841055102715"></a>4B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row1093911561812"><td class="cellrowborder" valign="top" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002599326429_p49396515184"><a name="zh-cn_topic_0000002599326429_p49396515184"></a><a name="zh-cn_topic_0000002599326429_p49396515184"></a>ORDER_INDEX_VALUE</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002599326429_p19939145121812"><a name="zh-cn_topic_0000002599326429_p19939145121812"></a><a name="zh-cn_topic_0000002599326429_p19939145121812"></a>4B</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002599326429_p884655102717"><a name="zh-cn_topic_0000002599326429_p884655102717"></a><a name="zh-cn_topic_0000002599326429_p884655102717"></a>4B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row16939856181"><td class="cellrowborder" valign="top" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002599326429_p209391159181"><a name="zh-cn_topic_0000002599326429_p209391159181"></a><a name="zh-cn_topic_0000002599326429_p209391159181"></a>ORDER_ONLY_VALUE</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002599326429_p293915513187"><a name="zh-cn_topic_0000002599326429_p293915513187"></a><a name="zh-cn_topic_0000002599326429_p293915513187"></a>2B</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002599326429_p084175513277"><a name="zh-cn_topic_0000002599326429_p084175513277"></a><a name="zh-cn_topic_0000002599326429_p084175513277"></a>2B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row1893913551811"><td class="cellrowborder" valign="top" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002599326429_p17939125101813"><a name="zh-cn_topic_0000002599326429_p17939125101813"></a><a name="zh-cn_topic_0000002599326429_p17939125101813"></a>ORDER_ONLY_INDEX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002599326429_p79392551819"><a name="zh-cn_topic_0000002599326429_p79392551819"></a><a name="zh-cn_topic_0000002599326429_p79392551819"></a>4B</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002599326429_p138425532716"><a name="zh-cn_topic_0000002599326429_p138425532716"></a><a name="zh-cn_topic_0000002599326429_p138425532716"></a>4B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row894014516185"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002599326429_p1852454102215"><a name="zh-cn_topic_0000002599326429_p1852454102215"></a><a name="zh-cn_topic_0000002599326429_p1852454102215"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002599326429_p19390175812239"><a name="zh-cn_topic_0000002599326429_p19390175812239"></a><a name="zh-cn_topic_0000002599326429_p19390175812239"></a>ORDER_VALUE_INDEX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002599326429_p159404501817"><a name="zh-cn_topic_0000002599326429_p159404501817"></a><a name="zh-cn_topic_0000002599326429_p159404501817"></a>8B</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.4 "><p id="zh-cn_topic_0000002599326429_p08475518273"><a name="zh-cn_topic_0000002599326429_p08475518273"></a><a name="zh-cn_topic_0000002599326429_p08475518273"></a>8B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row2940257183"><td class="cellrowborder" valign="top" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002599326429_p73900584233"><a name="zh-cn_topic_0000002599326429_p73900584233"></a><a name="zh-cn_topic_0000002599326429_p73900584233"></a>ORDER_INDEX_VALUE</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002599326429_p169408521815"><a name="zh-cn_topic_0000002599326429_p169408521815"></a><a name="zh-cn_topic_0000002599326429_p169408521815"></a>8B</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002599326429_p884155552710"><a name="zh-cn_topic_0000002599326429_p884155552710"></a><a name="zh-cn_topic_0000002599326429_p884155552710"></a>8B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row15940125191817"><td class="cellrowborder" valign="top" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002599326429_p16390145812320"><a name="zh-cn_topic_0000002599326429_p16390145812320"></a><a name="zh-cn_topic_0000002599326429_p16390145812320"></a>ORDER_ONLY_VALUE</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002599326429_p199401050185"><a name="zh-cn_topic_0000002599326429_p199401050185"></a><a name="zh-cn_topic_0000002599326429_p199401050185"></a>4B</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002599326429_p1884205542710"><a name="zh-cn_topic_0000002599326429_p1884205542710"></a><a name="zh-cn_topic_0000002599326429_p1884205542710"></a>4B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002599326429_row1029417461222"><td class="cellrowborder" valign="top" headers="mcps1.1.6.1.1 "><p id="zh-cn_topic_0000002599326429_p15390195818232"><a name="zh-cn_topic_0000002599326429_p15390195818232"></a><a name="zh-cn_topic_0000002599326429_p15390195818232"></a>ORDER_ONLY_INDEX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.2 "><p id="zh-cn_topic_0000002599326429_p229494632217"><a name="zh-cn_topic_0000002599326429_p229494632217"></a><a name="zh-cn_topic_0000002599326429_p229494632217"></a>4B</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.1.6.1.3 "><p id="zh-cn_topic_0000002599326429_p28425510271"><a name="zh-cn_topic_0000002599326429_p28425510271"></a><a name="zh-cn_topic_0000002599326429_p28425510271"></a>4B</p>
</td>
</tr>
</tbody>
</table>

## 数据类型<a name="zh-cn_topic_0000002599326429_section4219135304818"></a>

操作数支持的数据类型为：half/float。

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品  ，目的操作数与源操作数的数据类型需要保持一致。

## 约束说明<a name="zh-cn_topic_0000002599326429_section2045914466492"></a>

**归约操作类型取SUM/MAX/MIN时的约束**：

-   操作数地址重叠约束请参考[地址重叠约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section668772811100)。

-   源操作数地址对齐约束请参考[地址对齐约束](Unified-Buffer介绍.md#zh-cn_topic_0000002524627718_section796754519912)，dst的起始地址对齐要求请参考[表3](#zh-cn_topic_0000002599326429_table16987516135013)。
-   repeatTime = 0表示不会执行归约操作，不会对目的操作数进行写入，该接口将被视为 NOP（空操作）。
-   dstRepStride, srcBlkStride, srcRepStride取值范围为\[0, 2<sup>16</sup>-1\]，需要结合UB的实际大小避免出现越界。
-   dstRepStride的单位受order参数的影响，请参考[表3](#zh-cn_topic_0000002599326429_table16987516135013)。
-   针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品  ，目的操作数与源操作数的数据类型需要保持一致。

**归约操作类型取MAX/MIN时的约束：**

-   如果存在多个最值，该指令会将最小索引写入目的操作数。
-   索引按操作数的数据类型存储，读取索引需要将类型转换到整型。

## 关键特性说明<a name="zh-cn_topic_0000002599326429_section637617193310"></a>

-   **索引值需要强制类型转换**：

    dst的最值索引按照最值的数据类型存储，比如dst为half类型时，索引按照half类型存储，因此读取索引需要使用reinterpret\_cast方法转换到整数类型。若最值数据类型是half，需要使用reinterpret\_cast<uint16\_t\*\>；若最值数据类型是float，需要使用reinterpret\_cast<uint32\_t\*\>。特别地，ORDER\_ONLY\_INDEX（仅返回最值索引）情况下，读取索引值index时都需要使用reinterpret\_cast<uint32\_t\*\>。

    例如，输入数据是half类型，计算结果为\[0.9985,  6.8e-06\]，6.8e-06需要使用reinterpret\_cast<uint16\_t\*\>方法转换得到索引值114。转换示例如下：

    ```
    half maxIndex = dst.GetValue(1);
    uint16_t realIndex = *reinterpret_cast<uint16_t*>(&maxIndex);
    ```

-   归约操作类型取SUM时接口以二叉树累加的方式完成每个repeat内的求和，详情请参考[ReduceDataBlock关键特性说明](ReduceDataBlock.md#zh-cn_topic_0000002599206475_section1124342410319)。

## 调用示例<a name="zh-cn_topic_0000002599326429_section088124295117"></a>

完整示例可参考[ReduceRepeat样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/whole_reduce_min_max_sum)。
