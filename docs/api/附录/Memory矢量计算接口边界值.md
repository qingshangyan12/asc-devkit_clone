# Memory矢量计算接口边界值<a name="ZH-CN_TOPIC_0000002533514388"></a>

## 概述<a name="section3674181114910"></a>

本节汇总介绍了Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品Memory 矢量计算接口在边界值输入下的两类输出结果：

-   INF/NAN 模式（非饱和模式）

    计算结果是inf、-inf、nan的三类场景，按原样输出。

-   饱和模式

    通过寄存器配置接口进行设置（**具体接口内容待补充**），饱和模式下：inf和-inf会被饱和为MAX和MIN进行输出，nan会被饱和为0进行输出。

注：

-   float 数据类型输入仅支持 INF/NAN 模式（饱和模式与INF/NAN模式输出结果一致）；half、int16\_t 及 int32\_t 类型默认输出 INF/NAN 模式。
-   **绝大部分场景下，INF/NAN 模式的输出结果与IEEE754标准结果一致。如存在不一致的情况，会在对应接口处备注说明。**

矢量计算接口输出inf/nan的场景总结如下表：

**表 1**  浮点数计算输出inf/nan的场景

<a name="table1645210351498"></a>
<table><thead align="left"><tr id="row94522351595"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p15452835792"><a name="p15452835792"></a><a name="p15452835792"></a>场景描述</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p04527359919"><a name="p04527359919"></a><a name="p04527359919"></a>浮点数计算输出结果</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p1245319351496"><a name="p1245319351496"></a><a name="p1245319351496"></a>说明</p>
</th>
</tr>
</thead>
<tbody><tr id="row154531635991"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p171420406136"><a name="p171420406136"></a><a name="p171420406136"></a><strong id="b0715174041313"><a name="b0715174041313"></a><a name="b0715174041313"></a>输入包含inf</strong></p>
<p id="p10453173516916"><a name="p10453173516916"></a><a name="p10453173516916"></a>例外场景：</p>
<a name="ul459720552113"></a><a name="ul459720552113"></a><ul id="ul459720552113"><li>exp(-inf)=0</li><li>inf * 0 = nan</li><li>inf + (-inf) = nan</li></ul>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p164530358920"><a name="p164530358920"></a><a name="p164530358920"></a>inf</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1345353512914"><a name="p1345353512914"></a><a name="p1345353512914"></a>inf区分+inf和-inf</p>
</td>
</tr>
<tr id="row124531335299"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p2045353516916"><a name="p2045353516916"></a><a name="p2045353516916"></a><strong id="b206181419141210"><a name="b206181419141210"></a><a name="b206181419141210"></a>计算结果发生溢出</strong></p>
<p id="p1542113931120"><a name="p1542113931120"></a><a name="p1542113931120"></a>例外情况：</p>
<a name="ul579724891111"></a><a name="ul579724891111"></a><ul id="ul579724891111"><li>非nan值（不包含0）除以0，比如Div/Sqrt/Reciprocal等指令</li><li>ln(0.0) = -inf；ln(-0.0) = -inf</li><li>rsqrt(0.0) = inf; rsqrt(-0.0) = -inf</li></ul>
</td>
</tr>
<tr id="row19398521121317"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p23981421151313"><a name="p23981421151313"></a><a name="p23981421151313"></a><strong id="b74531156135211"><a name="b74531156135211"></a><a name="b74531156135211"></a>输入包含nan</strong></p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p19398142141314"><a name="p19398142141314"></a><a name="p19398142141314"></a>nan</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p5399192112139"><a name="p5399192112139"></a><a name="p5399192112139"></a>nan不区分正负</p>
</td>
</tr>
<tr id="row18826192721413"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1982610275147"><a name="p1982610275147"></a><a name="p1982610275147"></a><strong id="b468445965215"><a name="b468445965215"></a><a name="b468445965215"></a>计算结果产生nan</strong></p>
<p id="p105154218143"><a name="p105154218143"></a><a name="p105154218143"></a>举例：</p>
<a name="ul9790152013163"></a><a name="ul9790152013163"></a><ul id="ul9790152013163"><li>(-inf) - (-inf) = nan，比如Add/Sub/Reduce等指令</li><li>(-inf) + inf = nan，比如Add/Sub/Reduce等指令</li><li>0 * (+/-inf) = nan，比如Mul等指令</li><li>(+/-inf) / (+/-inf) = nan</li><li>0 / 0 = nan</li><li>ln/Sqrt/Rsqrt指令输入为负值</li></ul>
</td>
</tr>
</tbody>
</table>

## 基础算术<a name="section12606472427"></a>

**表 2**  Exp特殊值/边界值输入的计算结果

<a name="table864650154918"></a>
<table><thead align="left"><tr id="row56415015495"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p18515195464915"><a name="p18515195464915"></a><a name="p18515195464915"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p7515145434913"><a name="p7515145434913"></a><a name="p7515145434913"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p12515195494910"><a name="p12515195494910"></a><a name="p12515195494910"></a>dst的元素取值（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p364616579495"><a name="p364616579495"></a><a name="p364616579495"></a>dst的元素取值（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row1865350194920"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p11445151315500"><a name="p11445151315500"></a><a name="p11445151315500"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p194451013145019"><a name="p194451013145019"></a><a name="p194451013145019"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1944519133508"><a name="p1944519133508"></a><a name="p1944519133508"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p27229172504"><a name="p27229172504"></a><a name="p27229172504"></a>0.0</p>
</td>
</tr>
<tr id="row15651450194918"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1644521365012"><a name="p1644521365012"></a><a name="p1644521365012"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2445131305017"><a name="p2445131305017"></a><a name="p2445131305017"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1972271711508"><a name="p1972271711508"></a><a name="p1972271711508"></a><strong id="b1972221755016"><a name="b1972221755016"></a><a name="b1972221755016"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row106610509496"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1444521318505"><a name="p1444521318505"></a><a name="p1444521318505"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19445413205019"><a name="p19445413205019"></a><a name="p19445413205019"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19722717185016"><a name="p19722717185016"></a><a name="p19722717185016"></a>1.0</p>
</td>
</tr>
<tr id="row866850174915"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p18445151310501"><a name="p18445151310501"></a><a name="p18445151310501"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p10445131325016"><a name="p10445131325016"></a><a name="p10445131325016"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1772214177504"><a name="p1772214177504"></a><a name="p1772214177504"></a>1.0</p>
</td>
</tr>
<tr id="row206617502498"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p14445131316507"><a name="p14445131316507"></a><a name="p14445131316507"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p944541317505"><a name="p944541317505"></a><a name="p944541317505"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p17221717175014"><a name="p17221717175014"></a><a name="p17221717175014"></a><strong id="b17221117185012"><a name="b17221117185012"></a><a name="b17221117185012"></a>0.0</strong></p>
</td>
</tr>
<tr id="row566135017499"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p444510136508"><a name="p444510136508"></a><a name="p444510136508"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p34452131504"><a name="p34452131504"></a><a name="p34452131504"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p272261795013"><a name="p272261795013"></a><a name="p272261795013"></a><strong id="b872214177500"><a name="b872214177500"></a><a name="b872214177500"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row3668505498"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p12445131319505"><a name="p12445131319505"></a><a name="p12445131319505"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p124469130509"><a name="p124469130509"></a><a name="p124469130509"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p37223173502"><a name="p37223173502"></a><a name="p37223173502"></a>0.0</p>
</td>
</tr>
<tr id="row1247215016508"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p17061340165011"><a name="p17061340165011"></a><a name="p17061340165011"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p6706104015010"><a name="p6706104015010"></a><a name="p6706104015010"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1470604013503"><a name="p1470604013503"></a><a name="p1470604013503"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p280864318502"><a name="p280864318502"></a><a name="p280864318502"></a>0.0</p>
</td>
</tr>
<tr id="row89044413501"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p170644010503"><a name="p170644010503"></a><a name="p170644010503"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p13706940135015"><a name="p13706940135015"></a><a name="p13706940135015"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11808543175018"><a name="p11808543175018"></a><a name="p11808543175018"></a>inf</p>
</td>
</tr>
<tr id="row777017617505"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p570684045015"><a name="p570684045015"></a><a name="p570684045015"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17706144011509"><a name="p17706144011509"></a><a name="p17706144011509"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p128080436508"><a name="p128080436508"></a><a name="p128080436508"></a>1.0</p>
</td>
</tr>
<tr id="row837514755015"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p187061240165010"><a name="p187061240165010"></a><a name="p187061240165010"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p67065402503"><a name="p67065402503"></a><a name="p67065402503"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1980813433505"><a name="p1980813433505"></a><a name="p1980813433505"></a>1.0</p>
</td>
</tr>
<tr id="row194008165019"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1770714012507"><a name="p1770714012507"></a><a name="p1770714012507"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p187071240195019"><a name="p187071240195019"></a><a name="p187071240195019"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p128081743205015"><a name="p128081743205015"></a><a name="p128081743205015"></a>nan</p>
</td>
</tr>
<tr id="row2248132855016"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1870718409504"><a name="p1870718409504"></a><a name="p1870718409504"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p570712401507"><a name="p570712401507"></a><a name="p570712401507"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p380814312508"><a name="p380814312508"></a><a name="p380814312508"></a>inf</p>
</td>
</tr>
<tr id="row158472281509"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p870711409509"><a name="p870711409509"></a><a name="p870711409509"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1370744025020"><a name="p1370744025020"></a><a name="p1370744025020"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11808144385018"><a name="p11808144385018"></a><a name="p11808144385018"></a>0.0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 3**  Ln特殊值/边界值输入的计算结果

<a name="table10972164142410"></a>
<table><thead align="left"><tr id="row209727416241"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p2972124110245"><a name="p2972124110245"></a><a name="p2972124110245"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p397284142419"><a name="p397284142419"></a><a name="p397284142419"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p2972164110247"><a name="p2972164110247"></a><a name="p2972164110247"></a>dst的元素取值（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p1797234110244"><a name="p1797234110244"></a><a name="p1797234110244"></a>dst的元素取值（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row139721641182413"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p159724418241"><a name="p159724418241"></a><a name="p159724418241"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p5973104132419"><a name="p5973104132419"></a><a name="p5973104132419"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1465075410385"><a name="p1465075410385"></a><a name="p1465075410385"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1164915413388"><a name="p1164915413388"></a><a name="p1164915413388"></a><strong id="b471033185016"><a name="b471033185016"></a><a name="b471033185016"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1697313410241"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13973124172420"><a name="p13973124172420"></a><a name="p13973124172420"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1564985473819"><a name="p1564985473819"></a><a name="p1564985473819"></a>11.09</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p4648195493810"><a name="p4648195493810"></a><a name="p4648195493810"></a>11.09</p>
</td>
</tr>
<tr id="row197312413242"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p997324110243"><a name="p997324110243"></a><a name="p997324110243"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1664817542384"><a name="p1664817542384"></a><a name="p1664817542384"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16647155414383"><a name="p16647155414383"></a><a name="p16647155414383"></a><strong id="b191401118504"><a name="b191401118504"></a><a name="b191401118504"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row097319411241"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p897334152411"><a name="p897334152411"></a><a name="p897334152411"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9647105433817"><a name="p9647105433817"></a><a name="p9647105433817"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p20646145493816"><a name="p20646145493816"></a><a name="p20646145493816"></a><strong id="b11141161155017"><a name="b11141161155017"></a><a name="b11141161155017"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1497316413241"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1197314182411"><a name="p1197314182411"></a><a name="p1197314182411"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9646185413819"><a name="p9646185413819"></a><a name="p9646185413819"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2645105483816"><a name="p2645105483816"></a><a name="p2645105483816"></a><strong id="b182758204917"><a name="b182758204917"></a><a name="b182758204917"></a>0.0</strong></p>
</td>
</tr>
<tr id="row897315414249"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1997354182415"><a name="p1997354182415"></a><a name="p1997354182415"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9645105410388"><a name="p9645105410388"></a><a name="p9645105410388"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18644105415388"><a name="p18644105415388"></a><a name="p18644105415388"></a><strong id="b18618581492"><a name="b18618581492"></a><a name="b18618581492"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row129735415248"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16973124120243"><a name="p16973124120243"></a><a name="p16973124120243"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p126446545382"><a name="p126446545382"></a><a name="p126446545382"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p66436544388"><a name="p66436544388"></a><a name="p66436544388"></a><strong id="b1790115844912"><a name="b1790115844912"></a><a name="b1790115844912"></a>0.0</strong></p>
</td>
</tr>
<tr id="row555214895811"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1455214813582"><a name="p1455214813582"></a><a name="p1455214813582"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p5705172725815"><a name="p5705172725815"></a><a name="p5705172725815"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p17705727115814"><a name="p17705727115814"></a><a name="p17705727115814"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p194651442115818"><a name="p194651442115818"></a><a name="p194651442115818"></a>nan</p>
</td>
</tr>
<tr id="row1752514995813"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p107050277587"><a name="p107050277587"></a><a name="p107050277587"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p177051327185810"><a name="p177051327185810"></a><a name="p177051327185810"></a>88.72</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14465342155810"><a name="p14465342155810"></a><a name="p14465342155810"></a>88.72284</p>
</td>
</tr>
<tr id="row399331512586"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p97051027185815"><a name="p97051027185815"></a><a name="p97051027185815"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16706202725815"><a name="p16706202725815"></a><a name="p16706202725815"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19466342135813"><a name="p19466342135813"></a><a name="p19466342135813"></a>-inf</p>
</td>
</tr>
<tr id="row97451516205820"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p167061527155812"><a name="p167061527155812"></a><a name="p167061527155812"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1470692710586"><a name="p1470692710586"></a><a name="p1470692710586"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p046617424582"><a name="p046617424582"></a><a name="p046617424582"></a>-inf</p>
</td>
</tr>
<tr id="row12433201716583"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p7706182717581"><a name="p7706182717581"></a><a name="p7706182717581"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p137061127155819"><a name="p137061127155819"></a><a name="p137061127155819"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1946684225819"><a name="p1946684225819"></a><a name="p1946684225819"></a>nan</p>
</td>
</tr>
<tr id="row57481845819"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1706192735813"><a name="p1706192735813"></a><a name="p1706192735813"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p3706142735817"><a name="p3706142735817"></a><a name="p3706142735817"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p20466442165815"><a name="p20466442165815"></a><a name="p20466442165815"></a>inf</p>
</td>
</tr>
<tr id="row1577091817584"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p15706132785818"><a name="p15706132785818"></a><a name="p15706132785818"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p770642755815"><a name="p770642755815"></a><a name="p770642755815"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p114661042205813"><a name="p114661042205813"></a><a name="p114661042205813"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 4**  Abs特殊值/边界值输入的计算结果

<a name="table1975185932512"></a>
<table><thead align="left"><tr id="row1597555942518"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p14975459192515"><a name="p14975459192515"></a><a name="p14975459192515"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p59764597259"><a name="p59764597259"></a><a name="p59764597259"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p5976165972518"><a name="p5976165972518"></a><a name="p5976165972518"></a>dst的元素取值（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p797625912516"><a name="p797625912516"></a><a name="p797625912516"></a>dst的元素取值（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row79764594251"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p109761859112515"><a name="p109761859112515"></a><a name="p109761859112515"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p29761759102511"><a name="p29761759102511"></a><a name="p29761759102511"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p34431310185912"><a name="p34431310185912"></a><a name="p34431310185912"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p676192713556"><a name="p676192713556"></a><a name="p676192713556"></a>65504.0</p>
</td>
</tr>
<tr id="row12976105916253"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p197645912251"><a name="p197645912251"></a><a name="p197645912251"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18596878554"><a name="p18596878554"></a><a name="p18596878554"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p076627155520"><a name="p076627155520"></a><a name="p076627155520"></a>65504.0</p>
</td>
</tr>
<tr id="row1197635914250"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8976559142517"><a name="p8976559142517"></a><a name="p8976559142517"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p544116104598"><a name="p544116104598"></a><a name="p544116104598"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1376427105520"><a name="p1376427105520"></a><a name="p1376427105520"></a>0.0</p>
</td>
</tr>
<tr id="row3976759122520"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p09761559192515"><a name="p09761559192515"></a><a name="p09761559192515"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16440191017593"><a name="p16440191017593"></a><a name="p16440191017593"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p676142705514"><a name="p676142705514"></a><a name="p676142705514"></a>0.0</p>
</td>
</tr>
<tr id="row1397618591250"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16976195910259"><a name="p16976195910259"></a><a name="p16976195910259"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p124391104596"><a name="p124391104596"></a><a name="p124391104596"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p192922029115512"><a name="p192922029115512"></a><a name="p192922029115512"></a><strong id="b3760163345513"><a name="b3760163345513"></a><a name="b3760163345513"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1976759142520"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p14976259142511"><a name="p14976259142511"></a><a name="p14976259142511"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p5438110185914"><a name="p5438110185914"></a><a name="p5438110185914"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p20318143018557"><a name="p20318143018557"></a><a name="p20318143018557"></a><strong id="b88998552154"><a name="b88998552154"></a><a name="b88998552154"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1897620592251"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p397695916256"><a name="p397695916256"></a><a name="p397695916256"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p343761055910"><a name="p343761055910"></a><a name="p343761055910"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p182034313557"><a name="p182034313557"></a><a name="p182034313557"></a><strong id="b11900175511159"><a name="b11900175511159"></a><a name="b11900175511159"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row15524134616114"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p14279536825"><a name="p14279536825"></a><a name="p14279536825"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p137538315215"><a name="p137538315215"></a><a name="p137538315215"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p11753431029"><a name="p11753431029"></a><a name="p11753431029"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p575316316214"><a name="p575316316214"></a><a name="p575316316214"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row145910487117"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p47533311627"><a name="p47533311627"></a><a name="p47533311627"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p475314311325"><a name="p475314311325"></a><a name="p475314311325"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p675310311214"><a name="p675310311214"></a><a name="p675310311214"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row1173517491114"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2753153110214"><a name="p2753153110214"></a><a name="p2753153110214"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p37538311128"><a name="p37538311128"></a><a name="p37538311128"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1075318312211"><a name="p1075318312211"></a><a name="p1075318312211"></a>0.0</p>
</td>
</tr>
<tr id="row15362250714"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16753431329"><a name="p16753431329"></a><a name="p16753431329"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19753183113218"><a name="p19753183113218"></a><a name="p19753183113218"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14753143112211"><a name="p14753143112211"></a><a name="p14753143112211"></a>0.0</p>
</td>
</tr>
<tr id="row1649105114113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p187537311123"><a name="p187537311123"></a><a name="p187537311123"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1775323116220"><a name="p1775323116220"></a><a name="p1775323116220"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p17753831724"><a name="p17753831724"></a><a name="p17753831724"></a>0.0</p>
</td>
</tr>
<tr id="row1490417511411"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1575312319211"><a name="p1575312319211"></a><a name="p1575312319211"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2754431826"><a name="p2754431826"></a><a name="p2754431826"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p97542311223"><a name="p97542311223"></a><a name="p97542311223"></a>inf</p>
</td>
</tr>
<tr id="row13594531416"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p197546311828"><a name="p197546311828"></a><a name="p197546311828"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4754113119210"><a name="p4754113119210"></a><a name="p4754113119210"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p97542311212"><a name="p97542311212"></a><a name="p97542311212"></a>inf</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 5**  Reciprocal特殊值/边界值输入的计算结果

<a name="table1046894562620"></a>
<table><thead align="left"><tr id="row3468134572618"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p74681745102614"><a name="p74681745102614"></a><a name="p74681745102614"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p1646874512268"><a name="p1646874512268"></a><a name="p1646874512268"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p11468345162611"><a name="p11468345162611"></a><a name="p11468345162611"></a>dst的元素取值（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p1946854542616"><a name="p1946854542616"></a><a name="p1946854542616"></a>dst的元素取值（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row6468204572612"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p8468104511267"><a name="p8468104511267"></a><a name="p8468104511267"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p246864510261"><a name="p246864510261"></a><a name="p246864510261"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p14655193216236"><a name="p14655193216236"></a><a name="p14655193216236"></a>-0.000015</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p18296461634"><a name="p18296461634"></a><a name="p18296461634"></a>-0.000015</p>
</td>
</tr>
<tr id="row34683452265"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p246834513267"><a name="p246834513267"></a><a name="p246834513267"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1329246636"><a name="p1329246636"></a><a name="p1329246636"></a>0.000015</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11281146834"><a name="p11281146834"></a><a name="p11281146834"></a>0.000015</p>
</td>
</tr>
<tr id="row94681045112618"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p346818459262"><a name="p346818459262"></a><a name="p346818459262"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11281346238"><a name="p11281346238"></a><a name="p11281346238"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5272462036"><a name="p5272462036"></a><a name="p5272462036"></a><strong id="b15693614134517"><a name="b15693614134517"></a><a name="b15693614134517"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row546810455267"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1946854515261"><a name="p1946854515261"></a><a name="p1946854515261"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p12712461134"><a name="p12712461134"></a><a name="p12712461134"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16976514453"><a name="p16976514453"></a><a name="p16976514453"></a><strong id="b67078148459"><a name="b67078148459"></a><a name="b67078148459"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row15468545182613"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p846917455262"><a name="p846917455262"></a><a name="p846917455262"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p326134619319"><a name="p326134619319"></a><a name="p326134619319"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p635281194512"><a name="p635281194512"></a><a name="p635281194512"></a><strong id="b9711914174520"><a name="b9711914174520"></a><a name="b9711914174520"></a>0.0</strong></p>
</td>
</tr>
<tr id="row14697459262"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1246914532613"><a name="p1246914532613"></a><a name="p1246914532613"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18253461133"><a name="p18253461133"></a><a name="p18253461133"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1818190457"><a name="p1818190457"></a><a name="p1818190457"></a>0.0</p>
</td>
</tr>
<tr id="row11469745102615"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p15469345142618"><a name="p15469345142618"></a><a name="p15469345142618"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p132410460313"><a name="p132410460313"></a><a name="p132410460313"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1881812944511"><a name="p1881812944511"></a><a name="p1881812944511"></a>-0.0</p>
</td>
</tr>
<tr id="row54066111449"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p202271423146"><a name="p202271423146"></a><a name="p202271423146"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1990418301941"><a name="p1990418301941"></a><a name="p1990418301941"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p139045301041"><a name="p139045301041"></a><a name="p139045301041"></a>-2.938736e-39</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p149041830842"><a name="p149041830842"></a><a name="p149041830842"></a>-2.938736e-39</p>
</td>
</tr>
<tr id="row1312717127411"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p129041230148"><a name="p129041230148"></a><a name="p129041230148"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16904153016416"><a name="p16904153016416"></a><a name="p16904153016416"></a>2.938736e-39</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p09048300413"><a name="p09048300413"></a><a name="p09048300413"></a>2.938736e-39</p>
</td>
</tr>
<tr id="row27402012347"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13904930148"><a name="p13904930148"></a><a name="p13904930148"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p199051330340"><a name="p199051330340"></a><a name="p199051330340"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p890510308411"><a name="p890510308411"></a><a name="p890510308411"></a>inf</p>
</td>
</tr>
<tr id="row18411013745"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p159050301848"><a name="p159050301848"></a><a name="p159050301848"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p129051330643"><a name="p129051330643"></a><a name="p129051330643"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p119051530644"><a name="p119051530644"></a><a name="p119051530644"></a>-inf</p>
</td>
</tr>
<tr id="row355214845"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p139056305410"><a name="p139056305410"></a><a name="p139056305410"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p14905123012414"><a name="p14905123012414"></a><a name="p14905123012414"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p109051730644"><a name="p109051730644"></a><a name="p109051730644"></a>nan</p>
</td>
</tr>
<tr id="row206572014445"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1590517301040"><a name="p1590517301040"></a><a name="p1590517301040"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16905193011411"><a name="p16905193011411"></a><a name="p16905193011411"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11905193019411"><a name="p11905193019411"></a><a name="p11905193019411"></a>0.0</p>
</td>
</tr>
<tr id="row930513153413"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p109059301949"><a name="p109059301949"></a><a name="p109059301949"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p199052301249"><a name="p199052301249"></a><a name="p199052301249"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p99054301848"><a name="p99054301848"></a><a name="p99054301848"></a>-0.0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 6**  Sqrt特殊值/边界值输入的计算结果

<a name="table1648943062718"></a>
<table><thead align="left"><tr id="row10489173052719"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p1748912309279"><a name="p1748912309279"></a><a name="p1748912309279"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p948963052715"><a name="p948963052715"></a><a name="p948963052715"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p19489173011279"><a name="p19489173011279"></a><a name="p19489173011279"></a>dst的元素取值（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p19489133042713"><a name="p19489133042713"></a><a name="p19489133042713"></a>dst的元素取值（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row17489930162715"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p7489123011272"><a name="p7489123011272"></a><a name="p7489123011272"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p54891530182720"><a name="p54891530182720"></a><a name="p54891530182720"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p18581104316376"><a name="p18581104316376"></a><a name="p18581104316376"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1758054313715"><a name="p1758054313715"></a><a name="p1758054313715"></a><strong id="b114121725174817"><a name="b114121725174817"></a><a name="b114121725174817"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1048903082710"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p448916303276"><a name="p448916303276"></a><a name="p448916303276"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1200655184713"><a name="p1200655184713"></a><a name="p1200655184713"></a>256.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p195791743173715"><a name="p195791743173715"></a><a name="p195791743173715"></a>256.0</p>
</td>
</tr>
<tr id="row549012304272"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p349043032714"><a name="p349043032714"></a><a name="p349043032714"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p125791243133720"><a name="p125791243133720"></a><a name="p125791243133720"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p38375864811"><a name="p38375864811"></a><a name="p38375864811"></a>0.0</p>
</td>
</tr>
<tr id="row10490193015270"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2490193042716"><a name="p2490193042716"></a><a name="p2490193042716"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1757814315378"><a name="p1757814315378"></a><a name="p1757814315378"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p15837688481"><a name="p15837688481"></a><a name="p15837688481"></a>-0.0</p>
</td>
</tr>
<tr id="row1049013014274"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1649013052714"><a name="p1649013052714"></a><a name="p1649013052714"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p75771343153717"><a name="p75771343153717"></a><a name="p75771343153717"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p10720171214814"><a name="p10720171214814"></a><a name="p10720171214814"></a><strong id="b1567022017482"><a name="b1567022017482"></a><a name="b1567022017482"></a>0.0</strong></p>
</td>
</tr>
<tr id="row14490173018278"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8490123012710"><a name="p8490123012710"></a><a name="p8490123012710"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2576843133717"><a name="p2576843133717"></a><a name="p2576843133717"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p357604313372"><a name="p357604313372"></a><a name="p357604313372"></a><strong id="b19674320124813"><a name="b19674320124813"></a><a name="b19674320124813"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1649018301271"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p549013019270"><a name="p549013019270"></a><a name="p549013019270"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p45751943153712"><a name="p45751943153712"></a><a name="p45751943153712"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p198551774815"><a name="p198551774815"></a><a name="p198551774815"></a><strong id="b186784207486"><a name="b186784207486"></a><a name="b186784207486"></a>0.0</strong></p>
</td>
</tr>
<tr id="row125191036611"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p7845810461"><a name="p7845810461"></a><a name="p7845810461"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p49123191064"><a name="p49123191064"></a><a name="p49123191064"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p8912819963"><a name="p8912819963"></a><a name="p8912819963"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p3912319468"><a name="p3912319468"></a><a name="p3912319468"></a>nan</p>
</td>
</tr>
<tr id="row11273741618"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13912019769"><a name="p13912019769"></a><a name="p13912019769"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1791214191262"><a name="p1791214191262"></a><a name="p1791214191262"></a>1.8446744e+19</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p59125198614"><a name="p59125198614"></a><a name="p59125198614"></a>1.8446744e+19</p>
</td>
</tr>
<tr id="row09191441767"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p791271912617"><a name="p791271912617"></a><a name="p791271912617"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p291271915610"><a name="p291271915610"></a><a name="p291271915610"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p09126191268"><a name="p09126191268"></a><a name="p09126191268"></a>0.0</p>
</td>
</tr>
<tr id="row19503159612"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p169121619766"><a name="p169121619766"></a><a name="p169121619766"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p791215196612"><a name="p791215196612"></a><a name="p791215196612"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p169121198616"><a name="p169121198616"></a><a name="p169121198616"></a>-0.0</p>
</td>
</tr>
<tr id="row12187156962"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p591215192067"><a name="p591215192067"></a><a name="p591215192067"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2912141912615"><a name="p2912141912615"></a><a name="p2912141912615"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p391213191663"><a name="p391213191663"></a><a name="p391213191663"></a>nan</p>
</td>
</tr>
<tr id="row12798156561"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p39121619765"><a name="p39121619765"></a><a name="p39121619765"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1691219196616"><a name="p1691219196616"></a><a name="p1691219196616"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p139121519564"><a name="p139121519564"></a><a name="p139121519564"></a>inf</p>
</td>
</tr>
<tr id="row8801385611"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p19121191664"><a name="p19121191664"></a><a name="p19121191664"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1491216191760"><a name="p1491216191760"></a><a name="p1491216191760"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1991219191163"><a name="p1991219191163"></a><a name="p1991219191163"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 7**  Rsqrt特殊值/边界值输入的计算结果

<a name="table12701922112818"></a>
<table><thead align="left"><tr id="row1070172222814"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p17011122112819"><a name="p17011122112819"></a><a name="p17011122112819"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p167011822102818"><a name="p167011822102818"></a><a name="p167011822102818"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p12701922112815"><a name="p12701922112815"></a><a name="p12701922112815"></a>dst的元素取值（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p9701222172811"><a name="p9701222172811"></a><a name="p9701222172811"></a>dst的元素取值（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row1670142212286"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p870182213282"><a name="p870182213282"></a><a name="p870182213282"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p5702102217283"><a name="p5702102217283"></a><a name="p5702102217283"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1580917171814"><a name="p1580917171814"></a><a name="p1580917171814"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p158081417419"><a name="p158081417419"></a><a name="p158081417419"></a><strong id="b19618518515"><a name="b19618518515"></a><a name="b19618518515"></a>0.0</strong></p>
</td>
</tr>
<tr id="row11702822202812"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p12702112217284"><a name="p12702112217284"></a><a name="p12702112217284"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2808617814"><a name="p2808617814"></a><a name="p2808617814"></a>0.003906</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p980741711114"><a name="p980741711114"></a><a name="p980741711114"></a>0.003906</p>
</td>
</tr>
<tr id="row67023221286"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p57021922122814"><a name="p57021922122814"></a><a name="p57021922122814"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p68071917613"><a name="p68071917613"></a><a name="p68071917613"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p080611712114"><a name="p080611712114"></a><a name="p080611712114"></a><strong id="b1343017319517"><a name="b1343017319517"></a><a name="b1343017319517"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row5702322152814"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1670242212811"><a name="p1670242212811"></a><a name="p1670242212811"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1280613175117"><a name="p1280613175117"></a><a name="p1280613175117"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p208051172117"><a name="p208051172117"></a><a name="p208051172117"></a><strong id="b1443483459"><a name="b1443483459"></a><a name="b1443483459"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row11702192213287"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p5702922162813"><a name="p5702922162813"></a><a name="p5702922162813"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1805317612"><a name="p1805317612"></a><a name="p1805317612"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p325475715415"><a name="p325475715415"></a><a name="p325475715415"></a><strong id="b3555151110513"><a name="b3555151110513"></a><a name="b3555151110513"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1670262219282"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p167021022162820"><a name="p167021022162820"></a><a name="p167021022162820"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p980419171916"><a name="p980419171916"></a><a name="p980419171916"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p73497589416"><a name="p73497589416"></a><a name="p73497589416"></a>0.0</p>
</td>
</tr>
<tr id="row157021622172811"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p7702102232814"><a name="p7702102232814"></a><a name="p7702102232814"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1180314171013"><a name="p1180314171013"></a><a name="p1180314171013"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p103301759349"><a name="p103301759349"></a><a name="p103301759349"></a><strong id="b2095771318510"><a name="b2095771318510"></a><a name="b2095771318510"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1367110404519"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p2067144194512"><a name="p2067144194512"></a><a name="p2067144194512"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1953514283452"><a name="p1953514283452"></a><a name="p1953514283452"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p16535112814517"><a name="p16535112814517"></a><a name="p16535112814517"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p16535122824519"><a name="p16535122824519"></a><a name="p16535122824519"></a>nan</p>
</td>
</tr>
<tr id="row15057612458"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p15536112815453"><a name="p15536112815453"></a><a name="p15536112815453"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p55361728184512"><a name="p55361728184512"></a><a name="p55361728184512"></a>5.421011e-20</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p145361728184513"><a name="p145361728184513"></a><a name="p145361728184513"></a>5.421011e-20</p>
</td>
</tr>
<tr id="row107045774514"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p453602812455"><a name="p453602812455"></a><a name="p453602812455"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p65361828154520"><a name="p65361828154520"></a><a name="p65361828154520"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p253615289459"><a name="p253615289459"></a><a name="p253615289459"></a>inf</p>
</td>
</tr>
<tr id="row2018131044512"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17536228174512"><a name="p17536228174512"></a><a name="p17536228174512"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16536172834514"><a name="p16536172834514"></a><a name="p16536172834514"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18536112824515"><a name="p18536112824515"></a><a name="p18536112824515"></a>-inf</p>
</td>
</tr>
<tr id="row8914810194514"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p135361228114513"><a name="p135361228114513"></a><a name="p135361228114513"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2536228134519"><a name="p2536228134519"></a><a name="p2536228134519"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p553672834514"><a name="p553672834514"></a><a name="p553672834514"></a>nan</p>
</td>
</tr>
<tr id="row961819110458"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p10536928204516"><a name="p10536928204516"></a><a name="p10536928204516"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p553672812453"><a name="p553672812453"></a><a name="p553672812453"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p165362281453"><a name="p165362281453"></a><a name="p165362281453"></a>0.0</p>
</td>
</tr>
<tr id="row52846126458"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1353612894516"><a name="p1353612894516"></a><a name="p1353612894516"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p25361928134515"><a name="p25361928134515"></a><a name="p25361928134515"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8536132844513"><a name="p8536132844513"></a><a name="p8536132844513"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 8**  Relu特殊值/边界值输入的计算结果

<a name="table135522523284"></a>
<table><thead align="left"><tr id="row14552195215285"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p155521652102816"><a name="p155521652102816"></a><a name="p155521652102816"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p0552145212815"><a name="p0552145212815"></a><a name="p0552145212815"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p8552195212810"><a name="p8552195212810"></a><a name="p8552195212810"></a>dst的元素取值（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p1552752152811"><a name="p1552752152811"></a><a name="p1552752152811"></a>dst的元素取值（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row25528524286"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p5552952132810"><a name="p5552952132810"></a><a name="p5552952132810"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p05521952142814"><a name="p05521952142814"></a><a name="p05521952142814"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p11161182011918"><a name="p11161182011918"></a><a name="p11161182011918"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p0160122014914"><a name="p0160122014914"></a><a name="p0160122014914"></a>0.0</p>
</td>
</tr>
<tr id="row55531852172811"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1155318527281"><a name="p1155318527281"></a><a name="p1155318527281"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9160620897"><a name="p9160620897"></a><a name="p9160620897"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p81608201394"><a name="p81608201394"></a><a name="p81608201394"></a>65504.0</p>
</td>
</tr>
<tr id="row6553852162815"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p25531252162814"><a name="p25531252162814"></a><a name="p25531252162814"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p171591020997"><a name="p171591020997"></a><a name="p171591020997"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19159112011910"><a name="p19159112011910"></a><a name="p19159112011910"></a>0.0</p>
</td>
</tr>
<tr id="row20553105212812"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13553175242817"><a name="p13553175242817"></a><a name="p13553175242817"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4158520291"><a name="p4158520291"></a><a name="p4158520291"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1615812201690"><a name="p1615812201690"></a><a name="p1615812201690"></a>0.0</p>
</td>
</tr>
<tr id="row355316523285"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16553145292819"><a name="p16553145292819"></a><a name="p16553145292819"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1615711201796"><a name="p1615711201796"></a><a name="p1615711201796"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1215717201695"><a name="p1215717201695"></a><a name="p1215717201695"></a><strong id="b163251306194"><a name="b163251306194"></a><a name="b163251306194"></a>0.0</strong></p>
</td>
</tr>
<tr id="row655320522289"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17553105213288"><a name="p17553105213288"></a><a name="p17553105213288"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p191568201293"><a name="p191568201293"></a><a name="p191568201293"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p121569201915"><a name="p121569201915"></a><a name="p121569201915"></a><strong id="b997863218190"><a name="b997863218190"></a><a name="b997863218190"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1155315527287"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4553352112818"><a name="p4553352112818"></a><a name="p4553352112818"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1615514206919"><a name="p1615514206919"></a><a name="p1615514206919"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p715512202914"><a name="p715512202914"></a><a name="p715512202914"></a>0.0</p>
</td>
</tr>
<tr id="row134614225461"><td class="cellrowborder" rowspan="7" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p156204817460"><a name="p156204817460"></a><a name="p156204817460"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1478518457463"><a name="p1478518457463"></a><a name="p1478518457463"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p17851545144616"><a name="p17851545144616"></a><a name="p17851545144616"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p27851845134612"><a name="p27851845134612"></a><a name="p27851845134612"></a>0.0</p>
</td>
</tr>
<tr id="row18857183520461"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4785134554613"><a name="p4785134554613"></a><a name="p4785134554613"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p117857459464"><a name="p117857459464"></a><a name="p117857459464"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7785184504612"><a name="p7785184504612"></a><a name="p7785184504612"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row1376203317462"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p19785545134610"><a name="p19785545134610"></a><a name="p19785545134610"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p87856450466"><a name="p87856450466"></a><a name="p87856450466"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19785114512461"><a name="p19785114512461"></a><a name="p19785114512461"></a>0.0</p>
</td>
</tr>
<tr id="row52921329462"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p18785184516465"><a name="p18785184516465"></a><a name="p18785184516465"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p5785845154616"><a name="p5785845154616"></a><a name="p5785845154616"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p177856453468"><a name="p177856453468"></a><a name="p177856453468"></a>0.0</p>
</td>
</tr>
<tr id="row19557173054618"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p15785154544618"><a name="p15785154544618"></a><a name="p15785154544618"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17785174513469"><a name="p17785174513469"></a><a name="p17785174513469"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1078594517467"><a name="p1078594517467"></a><a name="p1078594517467"></a>nan</p>
</td>
</tr>
<tr id="row14970940184611"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p19786104574619"><a name="p19786104574619"></a><a name="p19786104574619"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1078614512462"><a name="p1078614512462"></a><a name="p1078614512462"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p178654524615"><a name="p178654524615"></a><a name="p178654524615"></a>inf</p>
</td>
</tr>
<tr id="row538116253469"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2786545124617"><a name="p2786545124617"></a><a name="p2786545124617"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2078613452469"><a name="p2078613452469"></a><a name="p2078613452469"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p77861945144620"><a name="p77861945144620"></a><a name="p77861945144620"></a>0.0</p>
</td>
</tr>
<tr id="row1252317150814"><td class="cellrowborder" rowspan="2" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p252319151487"><a name="p252319151487"></a><a name="p252319151487"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1186113517223"><a name="p1186113517223"></a><a name="p1186113517223"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p75239151482"><a name="p75239151482"></a><a name="p75239151482"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p2052391511816"><a name="p2052391511816"></a><a name="p2052391511816"></a>0</p>
</td>
</tr>
<tr id="row1533463452019"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1186155114224"><a name="p1186155114224"></a><a name="p1186155114224"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p93349342200"><a name="p93349342200"></a><a name="p93349342200"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1279172016618"><a name="p1279172016618"></a><a name="p1279172016618"></a>2147483647</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 9**  Add特殊值/边界值输入的计算结果

<a name="table071981613"></a>
<table><thead align="left"><tr id="row972148914"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.1"><p id="p6296114418299"><a name="p6296114418299"></a><a name="p6296114418299"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.2"><p id="p1229684442916"><a name="p1229684442916"></a><a name="p1229684442916"></a>src0的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.3"><p id="p499216210"><a name="p499216210"></a><a name="p499216210"></a>src1的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p1420617617216"><a name="p1420617617216"></a><a name="p1420617617216"></a>dst的元素取值</p>
<p id="p6625955618"><a name="p6625955618"></a><a name="p6625955618"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p19961581823"><a name="p19961581823"></a><a name="p19961581823"></a>dst的元素取值</p>
<p id="p186254559119"><a name="p186254559119"></a><a name="p186254559119"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row772128415"><td class="cellrowborder" rowspan="21" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p172581617"><a name="p172581617"></a><a name="p172581617"></a>half</p>
</td>
<td class="cellrowborder" rowspan="5" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p2296104482917"><a name="p2296104482917"></a><a name="p2296104482917"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p6160511202414"><a name="p6160511202414"></a><a name="p6160511202414"></a>-65504.0（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p5869647111216"><a name="p5869647111216"></a><a name="p5869647111216"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p8869164751210"><a name="p8869164751210"></a><a name="p8869164751210"></a><strong id="b1268122617132"><a name="b1268122617132"></a><a name="b1268122617132"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row37238215"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p10788214122415"><a name="p10788214122415"></a><a name="p10788214122415"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p148688475126"><a name="p148688475126"></a><a name="p148688475126"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14402723201316"><a name="p14402723201316"></a><a name="p14402723201316"></a>0.0</p>
</td>
</tr>
<tr id="row452211317438"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p16139162711433"><a name="p16139162711433"></a><a name="p16139162711433"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1286714474123"><a name="p1286714474123"></a><a name="p1286714474123"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2023714296131"><a name="p2023714296131"></a><a name="p2023714296131"></a>-65504.0</p>
</td>
</tr>
<tr id="row17231215144312"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9139132716439"><a name="p9139132716439"></a><a name="p9139132716439"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1886564771219"><a name="p1886564771219"></a><a name="p1886564771219"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1967917347134"><a name="p1967917347134"></a><a name="p1967917347134"></a><strong id="b184341738141311"><a name="b184341738141311"></a><a name="b184341738141311"></a>0.0</strong></p>
</td>
</tr>
<tr id="row55721684314"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1613972713436"><a name="p1613972713436"></a><a name="p1613972713436"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9864114716127"><a name="p9864114716127"></a><a name="p9864114716127"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p8680193471311"><a name="p8680193471311"></a><a name="p8680193471311"></a><strong id="b04502381139"><a name="b04502381139"></a><a name="b04502381139"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row325163734819"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p1330895814919"><a name="p1330895814919"></a><a name="p1330895814919"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1179861662411"><a name="p1179861662411"></a><a name="p1179861662411"></a>65504.0（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15862124791218"><a name="p15862124791218"></a><a name="p15862124791218"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p16862194731214"><a name="p16862194731214"></a><a name="p16862194731214"></a><strong id="b20588858153"><a name="b20588858153"></a><a name="b20588858153"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1798243944814"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11114246124916"><a name="p11114246124916"></a><a name="p11114246124916"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p08611147101216"><a name="p08611147101216"></a><a name="p08611147101216"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16281877159"><a name="p16281877159"></a><a name="p16281877159"></a>65504.0</p>
</td>
</tr>
<tr id="row18501541134813"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p111141546124911"><a name="p111141546124911"></a><a name="p111141546124911"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2845155311417"><a name="p2845155311417"></a><a name="p2845155311417"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1224414191516"><a name="p1224414191516"></a><a name="p1224414191516"></a><strong id="b152444131519"><a name="b152444131519"></a><a name="b152444131519"></a>0.0</strong></p>
</td>
</tr>
<tr id="row16836104210486"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p111513467497"><a name="p111513467497"></a><a name="p111513467497"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18845453121412"><a name="p18845453121412"></a><a name="p18845453121412"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2244151171510"><a name="p2244151171510"></a><a name="p2244151171510"></a><strong id="b72447101511"><a name="b72447101511"></a><a name="b72447101511"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1151553184917"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p1515133144916"><a name="p1515133144916"></a><a name="p1515133144916"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2063434413503"><a name="p2063434413503"></a><a name="p2063434413503"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1685594791216"><a name="p1685594791216"></a><a name="p1685594791216"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p3306322155"><a name="p3306322155"></a><a name="p3306322155"></a>0.0</p>
</td>
</tr>
<tr id="row14186133164910"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14439718512"><a name="p14439718512"></a><a name="p14439718512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10261326181616"><a name="p10261326181616"></a><a name="p10261326181616"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p8741134161613"><a name="p8741134161613"></a><a name="p8741134161613"></a><strong id="b1974143411611"><a name="b1974143411611"></a><a name="b1974143411611"></a>0.0</strong></p>
</td>
</tr>
<tr id="row16396191541618"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p0396101561614"><a name="p0396101561614"></a><a name="p0396101561614"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1261526111616"><a name="p1261526111616"></a><a name="p1261526111616"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p975534181617"><a name="p975534181617"></a><a name="p975534181617"></a><strong id="b13751234121618"><a name="b13751234121618"></a><a name="b13751234121618"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row37833126161"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p47843121169"><a name="p47843121169"></a><a name="p47843121169"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11261826101616"><a name="p11261826101616"></a><a name="p11261826101616"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p6751434101614"><a name="p6751434101614"></a><a name="p6751434101614"></a><strong id="b1175534131612"><a name="b1175534131612"></a><a name="b1175534131612"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row02243413491"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p222153404917"><a name="p222153404917"></a><a name="p222153404917"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3222536175111"><a name="p3222536175111"></a><a name="p3222536175111"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9453719111717"><a name="p9453719111717"></a><a name="p9453719111717"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1983018318187"><a name="p1983018318187"></a><a name="p1983018318187"></a>-0.0</p>
</td>
</tr>
<tr id="row10212184317519"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18375174851718"><a name="p18375174851718"></a><a name="p18375174851718"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1375124811711"><a name="p1375124811711"></a><a name="p1375124811711"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p437584811176"><a name="p437584811176"></a><a name="p437584811176"></a><strong id="b0375174851718"><a name="b0375174851718"></a><a name="b0375174851718"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1562162515170"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p137534811714"><a name="p137534811714"></a><a name="p137534811714"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p337513488171"><a name="p337513488171"></a><a name="p337513488171"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2375848151715"><a name="p2375848151715"></a><a name="p2375848151715"></a><strong id="b3375348191718"><a name="b3375348191718"></a><a name="b3375348191718"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row97418272177"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15375124891714"><a name="p15375124891714"></a><a name="p15375124891714"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p123751048181715"><a name="p123751048181715"></a><a name="p123751048181715"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p183751448151710"><a name="p183751448151710"></a><a name="p183751448151710"></a><strong id="b037544831719"><a name="b037544831719"></a><a name="b037544831719"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row433955505115"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p217116465217"><a name="p217116465217"></a><a name="p217116465217"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13339175545117"><a name="p13339175545117"></a><a name="p13339175545117"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p13850144771213"><a name="p13850144771213"></a><a name="p13850144771213"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p093711119184"><a name="p093711119184"></a><a name="p093711119184"></a><strong id="b99371115186"><a name="b99371115186"></a><a name="b99371115186"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1220751415523"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p11339162425316"><a name="p11339162425316"></a><a name="p11339162425316"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p169643211533"><a name="p169643211533"></a><a name="p169643211533"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p98493478125"><a name="p98493478125"></a><a name="p98493478125"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p64345599183"><a name="p64345599183"></a><a name="p64345599183"></a><strong id="b74348596182"><a name="b74348596182"></a><a name="b74348596182"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row71480199535"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9964221125313"><a name="p9964221125313"></a><a name="p9964221125313"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1484811477122"><a name="p1484811477122"></a><a name="p1484811477122"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p197092574183"><a name="p197092574183"></a><a name="p197092574183"></a><strong id="b14709457171815"><a name="b14709457171815"></a><a name="b14709457171815"></a>0.0</strong></p>
</td>
</tr>
<tr id="row110516260537"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p072303975314"><a name="p072303975314"></a><a name="p072303975314"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13995103616535"><a name="p13995103616535"></a><a name="p13995103616535"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1984744791210"><a name="p1984744791210"></a><a name="p1984744791210"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p14631320171910"><a name="p14631320171910"></a><a name="p14631320171910"></a><strong id="b4463102031910"><a name="b4463102031910"></a><a name="b4463102031910"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row144261723204816"><td class="cellrowborder" rowspan="21" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p477256144914"><a name="p477256144914"></a><a name="p477256144914"></a>float</p>
</td>
<td class="cellrowborder" rowspan="5" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p14426923114817"><a name="p14426923114817"></a><a name="p14426923114817"></a>-3.4028235e+38 (MIN)</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p1592241025119"><a name="p1592241025119"></a><a name="p1592241025119"></a>-3.4028235e+38 (MIN)/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p9922161025113"><a name="p9922161025113"></a><a name="p9922161025113"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p20922121019512"><a name="p20922121019512"></a><a name="p20922121019512"></a>-inf</p>
</td>
</tr>
<tr id="row22152414511"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p5922151085117"><a name="p5922151085117"></a><a name="p5922151085117"></a>3.4028235e+38 (MAX)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p139227107515"><a name="p139227107515"></a><a name="p139227107515"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p17922310185112"><a name="p17922310185112"></a><a name="p17922310185112"></a>0.0</p>
</td>
</tr>
<tr id="row117791025104819"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9922161013514"><a name="p9922161013514"></a><a name="p9922161013514"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15922151095119"><a name="p15922151095119"></a><a name="p15922151095119"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p12922181065115"><a name="p12922181065115"></a><a name="p12922181065115"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row10483194717484"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8922201018519"><a name="p8922201018519"></a><a name="p8922201018519"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6922101017515"><a name="p6922101017515"></a><a name="p6922101017515"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p592221005113"><a name="p592221005113"></a><a name="p592221005113"></a>nan</p>
</td>
</tr>
<tr id="row10871145664810"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14922010115114"><a name="p14922010115114"></a><a name="p14922010115114"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p192211102519"><a name="p192211102519"></a><a name="p192211102519"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19221710125113"><a name="p19221710125113"></a><a name="p19221710125113"></a>inf</p>
</td>
</tr>
<tr id="row18872856154811"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p12564163314501"><a name="p12564163314501"></a><a name="p12564163314501"></a>3.4028235e+38 (MAX)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p756443355013"><a name="p756443355013"></a><a name="p756443355013"></a>3.4028235e+38 (MAX) /inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p45641633205017"><a name="p45641633205017"></a><a name="p45641633205017"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p19564203365011"><a name="p19564203365011"></a><a name="p19564203365011"></a>inf</p>
</td>
</tr>
<tr id="row1987211569489"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p19564433135013"><a name="p19564433135013"></a><a name="p19564433135013"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1956423311500"><a name="p1956423311500"></a><a name="p1956423311500"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p45641933105019"><a name="p45641933105019"></a><a name="p45641933105019"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row219716124920"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p956483315503"><a name="p956483315503"></a><a name="p956483315503"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12564163385012"><a name="p12564163385012"></a><a name="p12564163385012"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p175640331506"><a name="p175640331506"></a><a name="p175640331506"></a>nan</p>
</td>
</tr>
<tr id="row119457128491"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14564113375013"><a name="p14564113375013"></a><a name="p14564113375013"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p756433314502"><a name="p756433314502"></a><a name="p756433314502"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10564103320501"><a name="p10564103320501"></a><a name="p10564103320501"></a>-inf</p>
</td>
</tr>
<tr id="row7945141254913"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p18564233205016"><a name="p18564233205016"></a><a name="p18564233205016"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1956473345016"><a name="p1956473345016"></a><a name="p1956473345016"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18564103313506"><a name="p18564103313506"></a><a name="p18564103313506"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p20565233195019"><a name="p20565233195019"></a><a name="p20565233195019"></a>0.0</p>
</td>
</tr>
<tr id="row2946191234917"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p756533319502"><a name="p756533319502"></a><a name="p756533319502"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18565833145013"><a name="p18565833145013"></a><a name="p18565833145013"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p105651333145019"><a name="p105651333145019"></a><a name="p105651333145019"></a>nan</p>
</td>
</tr>
<tr id="row189461212194917"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p13565163312501"><a name="p13565163312501"></a><a name="p13565163312501"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1856513312504"><a name="p1856513312504"></a><a name="p1856513312504"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1556523310505"><a name="p1556523310505"></a><a name="p1556523310505"></a>inf</p>
</td>
</tr>
<tr id="row1294614126494"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12565183317503"><a name="p12565183317503"></a><a name="p12565183317503"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p195651233115018"><a name="p195651233115018"></a><a name="p195651233115018"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p956513335012"><a name="p956513335012"></a><a name="p956513335012"></a>-inf</p>
</td>
</tr>
<tr id="row1794641211491"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p12565113365014"><a name="p12565113365014"></a><a name="p12565113365014"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1356593311507"><a name="p1356593311507"></a><a name="p1356593311507"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1656515334507"><a name="p1656515334507"></a><a name="p1656515334507"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p656553315508"><a name="p656553315508"></a><a name="p656553315508"></a>-0.0</p>
</td>
</tr>
<tr id="row3948141218499"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1356518335508"><a name="p1356518335508"></a><a name="p1356518335508"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p19565933115020"><a name="p19565933115020"></a><a name="p19565933115020"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1256583310505"><a name="p1256583310505"></a><a name="p1256583310505"></a>nan</p>
</td>
</tr>
<tr id="row7811852154910"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15652333505"><a name="p15652333505"></a><a name="p15652333505"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14565733205010"><a name="p14565733205010"></a><a name="p14565733205010"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p956510339501"><a name="p956510339501"></a><a name="p956510339501"></a>inf</p>
</td>
</tr>
<tr id="row5811115214490"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p156523316503"><a name="p156523316503"></a><a name="p156523316503"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p05651733145011"><a name="p05651733145011"></a><a name="p05651733145011"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p105651033125010"><a name="p105651033125010"></a><a name="p105651033125010"></a>-inf</p>
</td>
</tr>
<tr id="row1381110522494"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p7565193385015"><a name="p7565193385015"></a><a name="p7565193385015"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1856516336504"><a name="p1856516336504"></a><a name="p1856516336504"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1565113313502"><a name="p1565113313502"></a><a name="p1565113313502"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p756673335013"><a name="p756673335013"></a><a name="p756673335013"></a>nan</p>
</td>
</tr>
<tr id="row1811155211493"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p25661433105017"><a name="p25661433105017"></a><a name="p25661433105017"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1356663316501"><a name="p1356663316501"></a><a name="p1356663316501"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1566173375014"><a name="p1566173375014"></a><a name="p1566173375014"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p18566533195018"><a name="p18566533195018"></a><a name="p18566533195018"></a>inf</p>
</td>
</tr>
<tr id="row081212522492"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p356693325017"><a name="p356693325017"></a><a name="p356693325017"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p95661933135012"><a name="p95661933135012"></a><a name="p95661933135012"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p956663310504"><a name="p956663310504"></a><a name="p956663310504"></a>nan</p>
</td>
</tr>
<tr id="row4812552144912"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p356613314506"><a name="p356613314506"></a><a name="p356613314506"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p55661033135011"><a name="p55661033135011"></a><a name="p55661033135011"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1756643319503"><a name="p1756643319503"></a><a name="p1756643319503"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1556693314501"><a name="p1556693314501"></a><a name="p1556693314501"></a>-inf</p>
</td>
</tr>
<tr id="row1586453410224"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p1586019512222"><a name="p1586019512222"></a><a name="p1586019512222"></a>int16_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1586018510225"><a name="p1586018510225"></a><a name="p1586018510225"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p569104813245"><a name="p569104813245"></a><a name="p569104813245"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p1239624451211"><a name="p1239624451211"></a><a name="p1239624451211"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p15396644151217"><a name="p15396644151217"></a><a name="p15396644151217"></a><strong id="b01001237182115"><a name="b01001237182115"></a><a name="b01001237182115"></a>-32768</strong></p>
</td>
</tr>
<tr id="row129076369225"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2506175115242"><a name="p2506175115242"></a><a name="p2506175115242"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1939544401218"><a name="p1939544401218"></a><a name="p1939544401218"></a>-1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18395244181210"><a name="p18395244181210"></a><a name="p18395244181210"></a>-1</p>
</td>
</tr>
<tr id="row1527223982210"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12860205152212"><a name="p12860205152212"></a><a name="p12860205152212"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p185461353172412"><a name="p185461353172412"></a><a name="p185461353172412"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10394744111210"><a name="p10394744111210"></a><a name="p10394744111210"></a>-2</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p93948443122"><a name="p93948443122"></a><a name="p93948443122"></a><strong id="b19874183412118"><a name="b19874183412118"></a><a name="b19874183412118"></a>32767</strong></p>
</td>
</tr>
<tr id="row925844310228"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p786195132215"><a name="p786195132215"></a><a name="p786195132215"></a>int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1929818442293"><a name="p1929818442293"></a><a name="p1929818442293"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p14873358152413"><a name="p14873358152413"></a><a name="p14873358152413"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p2393124419128"><a name="p2393124419128"></a><a name="p2393124419128"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p457415113220"><a name="p457415113220"></a><a name="p457415113220"></a><strong id="b534711818220"><a name="b534711818220"></a><a name="b534711818220"></a>-2147483648</strong></p>
</td>
</tr>
<tr id="row15239047112217"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p74031024255"><a name="p74031024255"></a><a name="p74031024255"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6391444111215"><a name="p6391444111215"></a><a name="p6391444111215"></a>-1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p205748142210"><a name="p205748142210"></a><a name="p205748142210"></a>-1</p>
</td>
</tr>
<tr id="row793104917229"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p529919440292"><a name="p529919440292"></a><a name="p529919440292"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18790154259"><a name="p18790154259"></a><a name="p18790154259"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p143882044171217"><a name="p143882044171217"></a><a name="p143882044171217"></a>-2</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p155741119225"><a name="p155741119225"></a><a name="p155741119225"></a><strong id="b1156432018226"><a name="b1156432018226"></a><a name="b1156432018226"></a>2147483647</strong></p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 10**  Sub特殊值/边界值输入的计算结果

<a name="table12357151012179"></a>
<table><thead align="left"><tr id="row23576103175"><th class="cellrowborder" valign="top" width="12.76%" id="mcps1.2.6.1.1"><p id="p20357141021714"><a name="p20357141021714"></a><a name="p20357141021714"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="12.49%" id="mcps1.2.6.1.2"><p id="p163571310131719"><a name="p163571310131719"></a><a name="p163571310131719"></a>src0的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="38.61%" id="mcps1.2.6.1.3"><p id="p1935719105177"><a name="p1935719105177"></a><a name="p1935719105177"></a>src1的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="16.14%" id="mcps1.2.6.1.4"><p id="p83571110181720"><a name="p83571110181720"></a><a name="p83571110181720"></a>dst的元素取值</p>
<p id="p10357161016179"><a name="p10357161016179"></a><a name="p10357161016179"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p1235791015178"><a name="p1235791015178"></a><a name="p1235791015178"></a>dst的元素取值</p>
<p id="p9357410101719"><a name="p9357410101719"></a><a name="p9357410101719"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row183571910151715"><td class="cellrowborder" rowspan="25" valign="top" width="12.76%" headers="mcps1.2.6.1.1 "><p id="p1235720108176"><a name="p1235720108176"></a><a name="p1235720108176"></a>half</p>
</td>
<td class="cellrowborder" rowspan="6" valign="top" width="12.49%" headers="mcps1.2.6.1.2 "><p id="p83573108179"><a name="p83573108179"></a><a name="p83573108179"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="38.61%" headers="mcps1.2.6.1.3 "><p id="p11357161041717"><a name="p11357161041717"></a><a name="p11357161041717"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="16.14%" headers="mcps1.2.6.1.4 "><p id="p625701311811"><a name="p625701311811"></a><a name="p625701311811"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1810104234016"><a name="p1810104234016"></a><a name="p1810104234016"></a>0.0</p>
</td>
</tr>
<tr id="row1357910171712"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p635771041716"><a name="p635771041716"></a><a name="p635771041716"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1925617130187"><a name="p1925617130187"></a><a name="p1925617130187"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20334174784017"><a name="p20334174784017"></a><a name="p20334174784017"></a><strong id="b5377135274014"><a name="b5377135274014"></a><a name="b5377135274014"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row33587102179"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p235811109177"><a name="p235811109177"></a><a name="p235811109177"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p4620182234014"><a name="p4620182234014"></a><a name="p4620182234014"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p5101184215409"><a name="p5101184215409"></a><a name="p5101184215409"></a>-65504.0</p>
</td>
</tr>
<tr id="row635841031715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p193580101179"><a name="p193580101179"></a><a name="p193580101179"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p825311312189"><a name="p825311312189"></a><a name="p825311312189"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10101242164017"><a name="p10101242164017"></a><a name="p10101242164017"></a><strong id="b8122181524117"><a name="b8122181524117"></a><a name="b8122181524117"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1435821017178"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p535817108178"><a name="p535817108178"></a><a name="p535817108178"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p132521413191816"><a name="p132521413191816"></a><a name="p132521413191816"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p6590196194110"><a name="p6590196194110"></a><a name="p6590196194110"></a><strong id="b212311574110"><a name="b212311574110"></a><a name="b212311574110"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row133581109170"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p435811091710"><a name="p435811091710"></a><a name="p435811091710"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11486333124019"><a name="p11486333124019"></a><a name="p11486333124019"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p54883954110"><a name="p54883954110"></a><a name="p54883954110"></a><strong id="b948849194118"><a name="b948849194118"></a><a name="b948849194118"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row942912264428"><td class="cellrowborder" rowspan="6" valign="top" headers="mcps1.2.6.1.1 "><p id="p2429182617423"><a name="p2429182617423"></a><a name="p2429182617423"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p04291726194212"><a name="p04291726194212"></a><a name="p04291726194212"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p44290266427"><a name="p44290266427"></a><a name="p44290266427"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p067184144317"><a name="p067184144317"></a><a name="p067184144317"></a><strong id="b106711749438"><a name="b106711749438"></a><a name="b106711749438"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row163581410141714"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1358181011175"><a name="p1358181011175"></a><a name="p1358181011175"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p182508133188"><a name="p182508133188"></a><a name="p182508133188"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p123741102436"><a name="p123741102436"></a><a name="p123741102436"></a>0.0</p>
</td>
</tr>
<tr id="row19358410171713"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p235861017173"><a name="p235861017173"></a><a name="p235861017173"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9249151311183"><a name="p9249151311183"></a><a name="p9249151311183"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16374908436"><a name="p16374908436"></a><a name="p16374908436"></a>65504.0</p>
</td>
</tr>
<tr id="row6359610111711"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p23592105175"><a name="p23592105175"></a><a name="p23592105175"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5247141320183"><a name="p5247141320183"></a><a name="p5247141320183"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1351911864311"><a name="p1351911864311"></a><a name="p1351911864311"></a><strong id="b1351912844318"><a name="b1351912844318"></a><a name="b1351912844318"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1935981012179"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p23599104179"><a name="p23599104179"></a><a name="p23599104179"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15246413131819"><a name="p15246413131819"></a><a name="p15246413131819"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p165197813437"><a name="p165197813437"></a><a name="p165197813437"></a><strong id="b2519138104310"><a name="b2519138104310"></a><a name="b2519138104310"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row335912101170"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p113590107178"><a name="p113590107178"></a><a name="p113590107178"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p189111256124215"><a name="p189111256124215"></a><a name="p189111256124215"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p17519168154318"><a name="p17519168154318"></a><a name="p17519168154318"></a><strong id="b135191884316"><a name="b135191884316"></a><a name="b135191884316"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row173591105179"><td class="cellrowborder" rowspan="5" valign="top" headers="mcps1.2.6.1.1 "><p id="p1535991012175"><a name="p1535991012175"></a><a name="p1535991012175"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p629385144410"><a name="p629385144410"></a><a name="p629385144410"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p715010521466"><a name="p715010521466"></a><a name="p715010521466"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p14515964715"><a name="p14515964715"></a><a name="p14515964715"></a>65504.0</p>
</td>
</tr>
<tr id="row1975283613438"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1529314511445"><a name="p1529314511445"></a><a name="p1529314511445"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18417185484616"><a name="p18417185484616"></a><a name="p18417185484616"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p104514964716"><a name="p104514964716"></a><a name="p104514964716"></a>-65504.0</p>
</td>
</tr>
<tr id="row1335961018176"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4293851124414"><a name="p4293851124414"></a><a name="p4293851124414"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9433248471"><a name="p9433248471"></a><a name="p9433248471"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p194924115476"><a name="p194924115476"></a><a name="p194924115476"></a><strong id="b149231174713"><a name="b149231174713"></a><a name="b149231174713"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1896652910435"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1529395144414"><a name="p1529395144414"></a><a name="p1529395144414"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p114331047472"><a name="p114331047472"></a><a name="p114331047472"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p84927113474"><a name="p84927113474"></a><a name="p84927113474"></a><strong id="b14923118479"><a name="b14923118479"></a><a name="b14923118479"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row20704127164315"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p192931451194419"><a name="p192931451194419"></a><a name="p192931451194419"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1543311424712"><a name="p1543311424712"></a><a name="p1543311424712"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1449211111474"><a name="p1449211111474"></a><a name="p1449211111474"></a><strong id="b549211113471"><a name="b549211113471"></a><a name="b549211113471"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1881416308465"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15814330134613"><a name="p15814330134613"></a><a name="p15814330134613"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p108142309467"><a name="p108142309467"></a><a name="p108142309467"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p148141430154613"><a name="p148141430154613"></a><a name="p148141430154613"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p148143304465"><a name="p148143304465"></a><a name="p148143304465"></a>0.0</p>
</td>
</tr>
<tr id="row1811633312469"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p17116143314610"><a name="p17116143314610"></a><a name="p17116143314610"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p21167334465"><a name="p21167334465"></a><a name="p21167334465"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16449788471"><a name="p16449788471"></a><a name="p16449788471"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p49691111472"><a name="p49691111472"></a><a name="p49691111472"></a>-0.0</p>
</td>
</tr>
<tr id="row18284335154611"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1761816611478"><a name="p1761816611478"></a><a name="p1761816611478"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p244031013471"><a name="p244031013471"></a><a name="p244031013471"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p179691211184719"><a name="p179691211184719"></a><a name="p179691211184719"></a>0.0</p>
</td>
</tr>
<tr id="row8360201091715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p0360131031717"><a name="p0360131031717"></a><a name="p0360131031717"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p113601110141712"><a name="p113601110141712"></a><a name="p113601110141712"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18239213141817"><a name="p18239213141817"></a><a name="p18239213141817"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p13256311144911"><a name="p13256311144911"></a><a name="p13256311144911"></a><strong id="b152569116495"><a name="b152569116495"></a><a name="b152569116495"></a>0.0</strong></p>
</td>
</tr>
<tr id="row13601110181716"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p53601710161716"><a name="p53601710161716"></a><a name="p53601710161716"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1935917335013"><a name="p1935917335013"></a><a name="p1935917335013"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p823831312182"><a name="p823831312182"></a><a name="p823831312182"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p278612335509"><a name="p278612335509"></a><a name="p278612335509"></a><strong id="b1678683385019"><a name="b1678683385019"></a><a name="b1678683385019"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row536051017172"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1936019107175"><a name="p1936019107175"></a><a name="p1936019107175"></a>nan/+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p82371135185"><a name="p82371135185"></a><a name="p82371135185"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p13786103355013"><a name="p13786103355013"></a><a name="p13786103355013"></a><strong id="b678612334505"><a name="b678612334505"></a><a name="b678612334505"></a>0.0</strong></p>
</td>
</tr>
<tr id="row53601710171713"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p3360610181712"><a name="p3360610181712"></a><a name="p3360610181712"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11722184318502"><a name="p11722184318502"></a><a name="p11722184318502"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p144201017165113"><a name="p144201017165113"></a><a name="p144201017165113"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p6420121717512"><a name="p6420121717512"></a><a name="p6420121717512"></a><strong id="b164202017105113"><a name="b164202017105113"></a><a name="b164202017105113"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row14163740155014"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p187221433508"><a name="p187221433508"></a><a name="p187221433508"></a>nan/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12420121719513"><a name="p12420121719513"></a><a name="p12420121719513"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1542081735118"><a name="p1542081735118"></a><a name="p1542081735118"></a><strong id="b842001785119"><a name="b842001785119"></a><a name="b842001785119"></a>0.0</strong></p>
</td>
</tr>
<tr id="row097614334549"><td class="cellrowborder" rowspan="25" valign="top" width="12.76%" headers="mcps1.2.6.1.1 "><p id="p11106181465514"><a name="p11106181465514"></a><a name="p11106181465514"></a>float</p>
</td>
<td class="cellrowborder" rowspan="6" valign="top" width="12.49%" headers="mcps1.2.6.1.2 "><p id="p12777112695520"><a name="p12777112695520"></a><a name="p12777112695520"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="38.61%" headers="mcps1.2.6.1.3 "><p id="p1477718269553"><a name="p1477718269553"></a><a name="p1477718269553"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="16.14%" headers="mcps1.2.6.1.4 "><p id="p7777112615555"><a name="p7777112615555"></a><a name="p7777112615555"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p5777162616558"><a name="p5777162616558"></a><a name="p5777162616558"></a>0.0</p>
</td>
</tr>
<tr id="row5361641125410"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p167771026125516"><a name="p167771026125516"></a><a name="p167771026125516"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1177712267558"><a name="p1177712267558"></a><a name="p1177712267558"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p8777726175511"><a name="p8777726175511"></a><a name="p8777726175511"></a>-inf</p>
</td>
</tr>
<tr id="row125341388541"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p157778266558"><a name="p157778266558"></a><a name="p157778266558"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1477752645518"><a name="p1477752645518"></a><a name="p1477752645518"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1277732611556"><a name="p1277732611556"></a><a name="p1277732611556"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row1789075015542"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p6777112614551"><a name="p6777112614551"></a><a name="p6777112614551"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1577716261551"><a name="p1577716261551"></a><a name="p1577716261551"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1777792615552"><a name="p1777792615552"></a><a name="p1777792615552"></a>nan</p>
</td>
</tr>
<tr id="row1217724825414"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p187779266550"><a name="p187779266550"></a><a name="p187779266550"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p477719268559"><a name="p477719268559"></a><a name="p477719268559"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p977722618559"><a name="p977722618559"></a><a name="p977722618559"></a>-inf</p>
</td>
</tr>
<tr id="row6919125715543"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11777926145512"><a name="p11777926145512"></a><a name="p11777926145512"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p0777182613553"><a name="p0777182613553"></a><a name="p0777182613553"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14777122618553"><a name="p14777122618553"></a><a name="p14777122618553"></a>inf</p>
</td>
</tr>
<tr id="row491916576543"><td class="cellrowborder" rowspan="6" valign="top" headers="mcps1.2.6.1.1 "><p id="p977742618556"><a name="p977742618556"></a><a name="p977742618556"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12778192635515"><a name="p12778192635515"></a><a name="p12778192635515"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p197781626165518"><a name="p197781626165518"></a><a name="p197781626165518"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p18778426175512"><a name="p18778426175512"></a><a name="p18778426175512"></a>inf</p>
</td>
</tr>
<tr id="row15920185775414"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p47781626145510"><a name="p47781626145510"></a><a name="p47781626145510"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8778226115510"><a name="p8778226115510"></a><a name="p8778226115510"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1977813260553"><a name="p1977813260553"></a><a name="p1977813260553"></a>0.0</p>
</td>
</tr>
<tr id="row18920175795411"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p0778142635515"><a name="p0778142635515"></a><a name="p0778142635515"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p27783266556"><a name="p27783266556"></a><a name="p27783266556"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p67781126135513"><a name="p67781126135513"></a><a name="p67781126135513"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row1092065714541"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p877817262557"><a name="p877817262557"></a><a name="p877817262557"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p19778026115516"><a name="p19778026115516"></a><a name="p19778026115516"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18778142618551"><a name="p18778142618551"></a><a name="p18778142618551"></a>nan</p>
</td>
</tr>
<tr id="row196852034553"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1077819267554"><a name="p1077819267554"></a><a name="p1077819267554"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16778172655513"><a name="p16778172655513"></a><a name="p16778172655513"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p177815269558"><a name="p177815269558"></a><a name="p177815269558"></a>-inf</p>
</td>
</tr>
<tr id="row86869311559"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p19778112611553"><a name="p19778112611553"></a><a name="p19778112611553"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1777811268557"><a name="p1777811268557"></a><a name="p1777811268557"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18778826105511"><a name="p18778826105511"></a><a name="p18778826105511"></a>inf</p>
</td>
</tr>
<tr id="row56863375513"><td class="cellrowborder" rowspan="5" valign="top" headers="mcps1.2.6.1.1 "><p id="p14778122614550"><a name="p14778122614550"></a><a name="p14778122614550"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p177812645517"><a name="p177812645517"></a><a name="p177812645517"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15778142685513"><a name="p15778142685513"></a><a name="p15778142685513"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p177814262556"><a name="p177814262556"></a><a name="p177814262556"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row26865375515"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4778162612559"><a name="p4778162612559"></a><a name="p4778162612559"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p87788264558"><a name="p87788264558"></a><a name="p87788264558"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p177814265554"><a name="p177814265554"></a><a name="p177814265554"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row56869305519"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1877811265554"><a name="p1877811265554"></a><a name="p1877811265554"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9778026185510"><a name="p9778026185510"></a><a name="p9778026185510"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p677811269550"><a name="p677811269550"></a><a name="p677811269550"></a>nan</p>
</td>
</tr>
<tr id="row968614345510"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p277918266553"><a name="p277918266553"></a><a name="p277918266553"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11779102665515"><a name="p11779102665515"></a><a name="p11779102665515"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1177911262557"><a name="p1177911262557"></a><a name="p1177911262557"></a>-inf</p>
</td>
</tr>
<tr id="row4687933556"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12779182645513"><a name="p12779182645513"></a><a name="p12779182645513"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1577932695510"><a name="p1577932695510"></a><a name="p1577932695510"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19779132625517"><a name="p19779132625517"></a><a name="p19779132625517"></a>inf</p>
</td>
</tr>
<tr id="row1268723135517"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17779526205513"><a name="p17779526205513"></a><a name="p17779526205513"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1477902611557"><a name="p1477902611557"></a><a name="p1477902611557"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9779182615553"><a name="p9779182615553"></a><a name="p9779182615553"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p14779192615556"><a name="p14779192615556"></a><a name="p14779192615556"></a>0.0</p>
</td>
</tr>
<tr id="row10687143175518"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p19779162611553"><a name="p19779162611553"></a><a name="p19779162611553"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p147791826125520"><a name="p147791826125520"></a><a name="p147791826125520"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18779226175517"><a name="p18779226175517"></a><a name="p18779226175517"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p67791826145516"><a name="p67791826145516"></a><a name="p67791826145516"></a>-0.0</p>
</td>
</tr>
<tr id="row468714312558"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p187799267552"><a name="p187799267552"></a><a name="p187799267552"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p107791826185520"><a name="p107791826185520"></a><a name="p107791826185520"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15779182619556"><a name="p15779182619556"></a><a name="p15779182619556"></a>0.0</p>
</td>
</tr>
<tr id="row1817910145514"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14779526125512"><a name="p14779526125512"></a><a name="p14779526125512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p77794269555"><a name="p77794269555"></a><a name="p77794269555"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7779132615550"><a name="p7779132615550"></a><a name="p7779132615550"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p67796267550"><a name="p67796267550"></a><a name="p67796267550"></a>nan</p>
</td>
</tr>
<tr id="row0171810105513"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p6779172620558"><a name="p6779172620558"></a><a name="p6779172620558"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10779142615517"><a name="p10779142615517"></a><a name="p10779142615517"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p47791826155511"><a name="p47791826155511"></a><a name="p47791826155511"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p2077912266557"><a name="p2077912266557"></a><a name="p2077912266557"></a>inf</p>
</td>
</tr>
<tr id="row717141017558"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11779526145518"><a name="p11779526145518"></a><a name="p11779526145518"></a>nan/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1677972617559"><a name="p1677972617559"></a><a name="p1677972617559"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2779112617551"><a name="p2779112617551"></a><a name="p2779112617551"></a>nan</p>
</td>
</tr>
<tr id="row317710195516"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p27791526185520"><a name="p27791526185520"></a><a name="p27791526185520"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1278020265553"><a name="p1278020265553"></a><a name="p1278020265553"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p137801226115516"><a name="p137801226115516"></a><a name="p137801226115516"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p87802266558"><a name="p87802266558"></a><a name="p87802266558"></a>-inf</p>
</td>
</tr>
<tr id="row1418181016555"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p197801626175516"><a name="p197801626175516"></a><a name="p197801626175516"></a>nan/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p27801226195510"><a name="p27801226195510"></a><a name="p27801226195510"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p578020263551"><a name="p578020263551"></a><a name="p578020263551"></a>nan</p>
</td>
</tr>
<tr id="row1836110108171"><td class="cellrowborder" rowspan="4" valign="top" width="12.76%" headers="mcps1.2.6.1.1 "><p id="p1536111020173"><a name="p1536111020173"></a><a name="p1536111020173"></a>int16_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="12.49%" headers="mcps1.2.6.1.2 "><p id="p1361131019177"><a name="p1361131019177"></a><a name="p1361131019177"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="38.61%" headers="mcps1.2.6.1.3 "><p id="p17361310151715"><a name="p17361310151715"></a><a name="p17361310151715"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="16.14%" headers="mcps1.2.6.1.4 "><p id="p123415138184"><a name="p123415138184"></a><a name="p123415138184"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1234413151817"><a name="p1234413151817"></a><a name="p1234413151817"></a>0</p>
</td>
</tr>
<tr id="row13361131016172"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p3361910161716"><a name="p3361910161716"></a><a name="p3361910161716"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p0233131391814"><a name="p0233131391814"></a><a name="p0233131391814"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p056412117523"><a name="p056412117523"></a><a name="p056412117523"></a><strong id="b47183812523"><a name="b47183812523"></a><a name="b47183812523"></a>-32768</strong></p>
</td>
</tr>
<tr id="row13361191012178"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p8361110111710"><a name="p8361110111710"></a><a name="p8361110111710"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p548475495519"><a name="p548475495519"></a><a name="p548475495519"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p17742131314520"><a name="p17742131314520"></a><a name="p17742131314520"></a>-1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p4741913185211"><a name="p4741913185211"></a><a name="p4741913185211"></a><strong id="b1472133825216"><a name="b1472133825216"></a><a name="b1472133825216"></a>32767</strong></p>
</td>
</tr>
<tr id="row1666132515549"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11484125475516"><a name="p11484125475516"></a><a name="p11484125475516"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5738131317523"><a name="p5738131317523"></a><a name="p5738131317523"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p17738171305215"><a name="p17738171305215"></a><a name="p17738171305215"></a>0</p>
</td>
</tr>
<tr id="row1036110101170"><td class="cellrowborder" rowspan="4" valign="top" width="12.76%" headers="mcps1.2.6.1.1 "><p id="p1336111106172"><a name="p1336111106172"></a><a name="p1336111106172"></a>int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="12.49%" headers="mcps1.2.6.1.2 "><p id="p18361101051719"><a name="p18361101051719"></a><a name="p18361101051719"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="38.61%" headers="mcps1.2.6.1.3 "><p id="p173619102177"><a name="p173619102177"></a><a name="p173619102177"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="16.14%" headers="mcps1.2.6.1.4 "><p id="p11991415185216"><a name="p11991415185216"></a><a name="p11991415185216"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p11120539536"><a name="p11120539536"></a><a name="p11120539536"></a>0</p>
</td>
</tr>
<tr id="row193611110101711"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2036114103173"><a name="p2036114103173"></a><a name="p2036114103173"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p17981154528"><a name="p17981154528"></a><a name="p17981154528"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p122051942538"><a name="p122051942538"></a><a name="p122051942538"></a><strong id="b232863025310"><a name="b232863025310"></a><a name="b232863025310"></a>-2147483648</strong></p>
</td>
</tr>
<tr id="row133621310111717"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p19362910151712"><a name="p19362910151712"></a><a name="p19362910151712"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p391292255317"><a name="p391292255317"></a><a name="p391292255317"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p79881515215"><a name="p79881515215"></a><a name="p79881515215"></a>-1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p320516445310"><a name="p320516445310"></a><a name="p320516445310"></a><strong id="b14329630135316"><a name="b14329630135316"></a><a name="b14329630135316"></a>2147483647</strong></p>
</td>
</tr>
<tr id="row1624013614210"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p967214418429"><a name="p967214418429"></a><a name="p967214418429"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p139721520528"><a name="p139721520528"></a><a name="p139721520528"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p121203313532"><a name="p121203313532"></a><a name="p121203313532"></a>0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 11**  Mul特殊值/边界值输入的计算结果

<a name="table89442511309"></a>
<table><thead align="left"><tr id="row109441751113017"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.1"><p id="p1494415517309"><a name="p1494415517309"></a><a name="p1494415517309"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.2"><p id="p139441951153011"><a name="p139441951153011"></a><a name="p139441951153011"></a>src0的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.3"><p id="p89449514301"><a name="p89449514301"></a><a name="p89449514301"></a>src1的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p694445117305"><a name="p694445117305"></a><a name="p694445117305"></a>dst的元素取值</p>
<p id="p5944195173018"><a name="p5944195173018"></a><a name="p5944195173018"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p99441351153012"><a name="p99441351153012"></a><a name="p99441351153012"></a>dst的元素取值</p>
<p id="p119440517302"><a name="p119440517302"></a><a name="p119440517302"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row594485111308"><td class="cellrowborder" rowspan="22" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p1194416514308"><a name="p1194416514308"></a><a name="p1194416514308"></a>half</p>
</td>
<td class="cellrowborder" rowspan="7" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p12944165113018"><a name="p12944165113018"></a><a name="p12944165113018"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p20944135114306"><a name="p20944135114306"></a><a name="p20944135114306"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p872515208437"><a name="p872515208437"></a><a name="p872515208437"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p10726620134312"><a name="p10726620134312"></a><a name="p10726620134312"></a><strong id="b4726152024316"><a name="b4726152024316"></a><a name="b4726152024316"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1794516513309"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p13945551143016"><a name="p13945551143016"></a><a name="p13945551143016"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p87262209437"><a name="p87262209437"></a><a name="p87262209437"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16726122015439"><a name="p16726122015439"></a><a name="p16726122015439"></a><strong id="b1672642015437"><a name="b1672642015437"></a><a name="b1672642015437"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row494510519303"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1294519512303"><a name="p1294519512303"></a><a name="p1294519512303"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3726132054318"><a name="p3726132054318"></a><a name="p3726132054318"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p272692014433"><a name="p272692014433"></a><a name="p272692014433"></a>-0.0</p>
</td>
</tr>
<tr id="row12210171413431"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p51391327164319"><a name="p51391327164319"></a><a name="p51391327164319"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p672632074318"><a name="p672632074318"></a><a name="p672632074318"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p07261720164316"><a name="p07261720164316"></a><a name="p07261720164316"></a>0.0</p>
</td>
</tr>
<tr id="row18945145111304"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12945165117303"><a name="p12945165117303"></a><a name="p12945165117303"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p137269203433"><a name="p137269203433"></a><a name="p137269203433"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p27265209434"><a name="p27265209434"></a><a name="p27265209434"></a><strong id="b16726720154316"><a name="b16726720154316"></a><a name="b16726720154316"></a>0.0</strong></p>
</td>
</tr>
<tr id="row119453512306"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p99451451183018"><a name="p99451451183018"></a><a name="p99451451183018"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p7726620164315"><a name="p7726620164315"></a><a name="p7726620164315"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p157263208435"><a name="p157263208435"></a><a name="p157263208435"></a><strong id="b3726220134310"><a name="b3726220134310"></a><a name="b3726220134310"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row674112163438"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p7139727154317"><a name="p7139727154317"></a><a name="p7139727154317"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18726122054317"><a name="p18726122054317"></a><a name="p18726122054317"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p37268207431"><a name="p37268207431"></a><a name="p37268207431"></a><strong id="b13726192054317"><a name="b13726192054317"></a><a name="b13726192054317"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row794575163020"><td class="cellrowborder" rowspan="6" valign="top" headers="mcps1.2.6.1.1 "><p id="p15945125133011"><a name="p15945125133011"></a><a name="p15945125133011"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6945175114309"><a name="p6945175114309"></a><a name="p6945175114309"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p135197229492"><a name="p135197229492"></a><a name="p135197229492"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p6519222184915"><a name="p6519222184915"></a><a name="p6519222184915"></a><strong id="b14519192234914"><a name="b14519192234914"></a><a name="b14519192234914"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row10946251193018"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p189466517304"><a name="p189466517304"></a><a name="p189466517304"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p352002213497"><a name="p352002213497"></a><a name="p352002213497"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1352016229495"><a name="p1352016229495"></a><a name="p1352016229495"></a>0.0</p>
</td>
</tr>
<tr id="row17887440174813"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18114546134912"><a name="p18114546134912"></a><a name="p18114546134912"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1952016222497"><a name="p1952016222497"></a><a name="p1952016222497"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1252082218499"><a name="p1252082218499"></a><a name="p1252082218499"></a>-0.0</p>
</td>
</tr>
<tr id="row3946195143016"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4946205163019"><a name="p4946205163019"></a><a name="p4946205163019"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p205201822124919"><a name="p205201822124919"></a><a name="p205201822124919"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p95208224495"><a name="p95208224495"></a><a name="p95208224495"></a><strong id="b65204227493"><a name="b65204227493"></a><a name="b65204227493"></a>0.0</strong></p>
</td>
</tr>
<tr id="row17169114254810"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p111155461495"><a name="p111155461495"></a><a name="p111155461495"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1752042218499"><a name="p1752042218499"></a><a name="p1752042218499"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p175206229495"><a name="p175206229495"></a><a name="p175206229495"></a><strong id="b25204228492"><a name="b25204228492"></a><a name="b25204228492"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row594685119304"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p594645114303"><a name="p594645114303"></a><a name="p594645114303"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10520152254914"><a name="p10520152254914"></a><a name="p10520152254914"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18520192214914"><a name="p18520192214914"></a><a name="p18520192214914"></a><strong id="b0520722194911"><a name="b0520722194911"></a><a name="b0520722194911"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1294625111300"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p994625113013"><a name="p994625113013"></a><a name="p994625113013"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12946151173015"><a name="p12946151173015"></a><a name="p12946151173015"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p763534412504"><a name="p763534412504"></a><a name="p763534412504"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p18936133712503"><a name="p18936133712503"></a><a name="p18936133712503"></a>0.0</p>
</td>
</tr>
<tr id="row13486113234916"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p763594445018"><a name="p763594445018"></a><a name="p763594445018"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p186353444507"><a name="p186353444507"></a><a name="p186353444507"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11936537155011"><a name="p11936537155011"></a><a name="p11936537155011"></a>-0.0</p>
</td>
</tr>
<tr id="row6947155123010"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p149471651153011"><a name="p149471651153011"></a><a name="p149471651153011"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6186193314920"><a name="p6186193314920"></a><a name="p6186193314920"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p998614815118"><a name="p998614815118"></a><a name="p998614815118"></a><strong id="b85241917205114"><a name="b85241917205114"></a><a name="b85241917205114"></a>0.0</strong></p>
</td>
</tr>
<tr id="row4947165133016"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p894711512307"><a name="p894711512307"></a><a name="p894711512307"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8947115110306"><a name="p8947115110306"></a><a name="p8947115110306"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7222193614514"><a name="p7222193614514"></a><a name="p7222193614514"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p10222236185110"><a name="p10222236185110"></a><a name="p10222236185110"></a>0.0</p>
</td>
</tr>
<tr id="row1694717510306"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p10867745125110"><a name="p10867745125110"></a><a name="p10867745125110"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p0213243115119"><a name="p0213243115119"></a><a name="p0213243115119"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1431412536512"><a name="p1431412536512"></a><a name="p1431412536512"></a><strong id="b12314853115115"><a name="b12314853115115"></a><a name="b12314853115115"></a>0.0</strong></p>
</td>
</tr>
<tr id="row12947145118304"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1094785113307"><a name="p1094785113307"></a><a name="p1094785113307"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3947551193010"><a name="p3947551193010"></a><a name="p3947551193010"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p6408161095213"><a name="p6408161095213"></a><a name="p6408161095213"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p240831018524"><a name="p240831018524"></a><a name="p240831018524"></a><strong id="b2408710105215"><a name="b2408710105215"></a><a name="b2408710105215"></a>0.0</strong></p>
</td>
</tr>
<tr id="row159473516303"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p39475515301"><a name="p39475515301"></a><a name="p39475515301"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p894715173019"><a name="p894715173019"></a><a name="p894715173019"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p3964221195315"><a name="p3964221195315"></a><a name="p3964221195315"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1996414214538"><a name="p1996414214538"></a><a name="p1996414214538"></a><strong id="b5964102125312"><a name="b5964102125312"></a><a name="b5964102125312"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1947115153018"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p13947551173015"><a name="p13947551173015"></a><a name="p13947551173015"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p39641621185316"><a name="p39641621185316"></a><a name="p39641621185316"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1964421115311"><a name="p1964421115311"></a><a name="p1964421115311"></a><strong id="b9964132145316"><a name="b9964132145316"></a><a name="b9964132145316"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1094875193015"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8948115112302"><a name="p8948115112302"></a><a name="p8948115112302"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p194805116306"><a name="p194805116306"></a><a name="p194805116306"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p999533685312"><a name="p999533685312"></a><a name="p999533685312"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p139951736165317"><a name="p139951736165317"></a><a name="p139951736165317"></a><strong id="b18995143615530"><a name="b18995143615530"></a><a name="b18995143615530"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1334015014575"><td class="cellrowborder" rowspan="22" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p1941574385718"><a name="p1941574385718"></a><a name="p1941574385718"></a>float</p>
</td>
<td class="cellrowborder" rowspan="7" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p375114045716"><a name="p375114045716"></a><a name="p375114045716"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p187516407578"><a name="p187516407578"></a><a name="p187516407578"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p2751140195720"><a name="p2751140195720"></a><a name="p2751140195720"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1575194095715"><a name="p1575194095715"></a><a name="p1575194095715"></a>inf</p>
</td>
</tr>
<tr id="row14663107574"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1075940185712"><a name="p1075940185712"></a><a name="p1075940185712"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p197584065713"><a name="p197584065713"></a><a name="p197584065713"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p87534055710"><a name="p87534055710"></a><a name="p87534055710"></a>-inf</p>
</td>
</tr>
<tr id="row155521572578"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p675340195716"><a name="p675340195716"></a><a name="p675340195716"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p87584017571"><a name="p87584017571"></a><a name="p87584017571"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p3757408575"><a name="p3757408575"></a><a name="p3757408575"></a>-0.0</p>
</td>
</tr>
<tr id="row161646515719"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p475184045719"><a name="p475184045719"></a><a name="p475184045719"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p137624018577"><a name="p137624018577"></a><a name="p137624018577"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p776640145714"><a name="p776640145714"></a><a name="p776640145714"></a>0.0</p>
</td>
</tr>
<tr id="row1918432135713"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p127624075715"><a name="p127624075715"></a><a name="p127624075715"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1761140135717"><a name="p1761140135717"></a><a name="p1761140135717"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1276124017578"><a name="p1276124017578"></a><a name="p1276124017578"></a>nan</p>
</td>
</tr>
<tr id="row839961695710"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p127624065710"><a name="p127624065710"></a><a name="p127624065710"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1876144019579"><a name="p1876144019579"></a><a name="p1876144019579"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p57618405579"><a name="p57618405579"></a><a name="p57618405579"></a>-inf</p>
</td>
</tr>
<tr id="row1740018169571"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1376104011574"><a name="p1376104011574"></a><a name="p1376104011574"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1976640185717"><a name="p1976640185717"></a><a name="p1976640185717"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1376340195718"><a name="p1376340195718"></a><a name="p1376340195718"></a>inf</p>
</td>
</tr>
<tr id="row12400131605712"><td class="cellrowborder" rowspan="6" valign="top" headers="mcps1.2.6.1.1 "><p id="p7761440155716"><a name="p7761440155716"></a><a name="p7761440155716"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p17612408574"><a name="p17612408574"></a><a name="p17612408574"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p107654010578"><a name="p107654010578"></a><a name="p107654010578"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p57620405572"><a name="p57620405572"></a><a name="p57620405572"></a>inf</p>
</td>
</tr>
<tr id="row1840011605715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p107624085718"><a name="p107624085718"></a><a name="p107624085718"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13761440175715"><a name="p13761440175715"></a><a name="p13761440175715"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1676144014578"><a name="p1676144014578"></a><a name="p1676144014578"></a>0.0</p>
</td>
</tr>
<tr id="row19400121616575"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15761040145717"><a name="p15761040145717"></a><a name="p15761040145717"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p97604005712"><a name="p97604005712"></a><a name="p97604005712"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p127654095718"><a name="p127654095718"></a><a name="p127654095718"></a>-0.0</p>
</td>
</tr>
<tr id="row177131721165713"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p676184014578"><a name="p676184014578"></a><a name="p676184014578"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p157618401574"><a name="p157618401574"></a><a name="p157618401574"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p776340195715"><a name="p776340195715"></a><a name="p776340195715"></a>nan</p>
</td>
</tr>
<tr id="row271418215572"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1376164013573"><a name="p1376164013573"></a><a name="p1376164013573"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15761640195710"><a name="p15761640195710"></a><a name="p15761640195710"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7761040125718"><a name="p7761040125718"></a><a name="p7761040125718"></a>inf</p>
</td>
</tr>
<tr id="row3714102125715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1577144020574"><a name="p1577144020574"></a><a name="p1577144020574"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p7779406576"><a name="p7779406576"></a><a name="p7779406576"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2077440125715"><a name="p2077440125715"></a><a name="p2077440125715"></a>-inf</p>
</td>
</tr>
<tr id="row171492116573"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p67774015571"><a name="p67774015571"></a><a name="p67774015571"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9775409577"><a name="p9775409577"></a><a name="p9775409577"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2774409574"><a name="p2774409574"></a><a name="p2774409574"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p117714403575"><a name="p117714403575"></a><a name="p117714403575"></a>0.0</p>
</td>
</tr>
<tr id="row2071472165713"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17714012577"><a name="p17714012577"></a><a name="p17714012577"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p157764035715"><a name="p157764035715"></a><a name="p157764035715"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p5771640115714"><a name="p5771640115714"></a><a name="p5771640115714"></a>-0.0</p>
</td>
</tr>
<tr id="row147141021185710"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17794013573"><a name="p17794013573"></a><a name="p17794013573"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p777194011576"><a name="p777194011576"></a><a name="p777194011576"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p167717406575"><a name="p167717406575"></a><a name="p167717406575"></a>nan</p>
</td>
</tr>
<tr id="row20715102175711"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p3771440105714"><a name="p3771440105714"></a><a name="p3771440105714"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p577740125710"><a name="p577740125710"></a><a name="p577740125710"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p677124095718"><a name="p677124095718"></a><a name="p677124095718"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p12774409575"><a name="p12774409575"></a><a name="p12774409575"></a>0.0</p>
</td>
</tr>
<tr id="row67151721165716"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1577164035714"><a name="p1577164035714"></a><a name="p1577164035714"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8771840115712"><a name="p8771840115712"></a><a name="p8771840115712"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9771140105717"><a name="p9771140105717"></a><a name="p9771140105717"></a>nan</p>
</td>
</tr>
<tr id="row8715152115578"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p477164035713"><a name="p477164035713"></a><a name="p477164035713"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2077124019571"><a name="p2077124019571"></a><a name="p2077124019571"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15771340135720"><a name="p15771340135720"></a><a name="p15771340135720"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p4771340165714"><a name="p4771340165714"></a><a name="p4771340165714"></a>nan</p>
</td>
</tr>
<tr id="row15715192110578"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p127715408572"><a name="p127715408572"></a><a name="p127715408572"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3771240175717"><a name="p3771240175717"></a><a name="p3771240175717"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1277640165720"><a name="p1277640165720"></a><a name="p1277640165720"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p277840185716"><a name="p277840185716"></a><a name="p277840185716"></a>inf</p>
</td>
</tr>
<tr id="row1466702616573"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p677154017577"><a name="p677154017577"></a><a name="p677154017577"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16775408578"><a name="p16775408578"></a><a name="p16775408578"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p157711403576"><a name="p157711403576"></a><a name="p157711403576"></a>-inf</p>
</td>
</tr>
<tr id="row12667426105715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p677124035715"><a name="p677124035715"></a><a name="p677124035715"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p167814401570"><a name="p167814401570"></a><a name="p167814401570"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p47814405573"><a name="p47814405573"></a><a name="p47814405573"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1978194095717"><a name="p1978194095717"></a><a name="p1978194095717"></a>inf</p>
</td>
</tr>
<tr id="row7948165193019"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p1948351163011"><a name="p1948351163011"></a><a name="p1948351163011"></a>int16_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p894811511309"><a name="p894811511309"></a><a name="p894811511309"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p794825163013"><a name="p794825163013"></a><a name="p794825163013"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p10860125118228"><a name="p10860125118228"></a><a name="p10860125118228"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p786085112212"><a name="p786085112212"></a><a name="p786085112212"></a><strong id="b178607519225"><a name="b178607519225"></a><a name="b178607519225"></a>32767</strong></p>
</td>
</tr>
<tr id="row194855183014"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p109481851153014"><a name="p109481851153014"></a><a name="p109481851153014"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p68601351182215"><a name="p68601351182215"></a><a name="p68601351182215"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20860351172213"><a name="p20860351172213"></a><a name="p20860351172213"></a>-32768</p>
</td>
</tr>
<tr id="row129481851153014"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p0948251153014"><a name="p0948251153014"></a><a name="p0948251153014"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p17948135133015"><a name="p17948135133015"></a><a name="p17948135133015"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16861145111225"><a name="p16861145111225"></a><a name="p16861145111225"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p5861751192219"><a name="p5861751192219"></a><a name="p5861751192219"></a><strong id="b8861751152210"><a name="b8861751152210"></a><a name="b8861751152210"></a>32767</strong></p>
</td>
</tr>
<tr id="row1794865153018"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p1894815513309"><a name="p1894815513309"></a><a name="p1894815513309"></a>int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1994935183017"><a name="p1994935183017"></a><a name="p1994935183017"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p9949165143010"><a name="p9949165143010"></a><a name="p9949165143010"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p88611951132217"><a name="p88611951132217"></a><a name="p88611951132217"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p186113514223"><a name="p186113514223"></a><a name="p186113514223"></a><strong id="b1886105132217"><a name="b1886105132217"></a><a name="b1886105132217"></a>2147483647</strong></p>
</td>
</tr>
<tr id="row14949175163018"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11949115193015"><a name="p11949115193015"></a><a name="p11949115193015"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p986115152211"><a name="p986115152211"></a><a name="p986115152211"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p386118516225"><a name="p386118516225"></a><a name="p386118516225"></a>-2147483648</p>
</td>
</tr>
<tr id="row1794935153019"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p199491351133016"><a name="p199491351133016"></a><a name="p199491351133016"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p394965117307"><a name="p394965117307"></a><a name="p394965117307"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1086135112216"><a name="p1086135112216"></a><a name="p1086135112216"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p2086115111225"><a name="p2086115111225"></a><a name="p2086115111225"></a><strong id="b20861195112223"><a name="b20861195112223"></a><a name="b20861195112223"></a>2147483647</strong></p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 12**  Div特殊值/边界值输入的计算结果

<a name="table118329223319"></a>
<table><thead align="left"><tr id="row6833422173113"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.1"><p id="p983315221315"><a name="p983315221315"></a><a name="p983315221315"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="10.13%" id="mcps1.2.6.1.2"><p id="p128331722203111"><a name="p128331722203111"></a><a name="p128331722203111"></a>src0的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="29.87%" id="mcps1.2.6.1.3"><p id="p138331122183114"><a name="p138331122183114"></a><a name="p138331122183114"></a>src1的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p1383362214317"><a name="p1383362214317"></a><a name="p1383362214317"></a>dst的元素取值</p>
<p id="p1833142293115"><a name="p1833142293115"></a><a name="p1833142293115"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p1683372283118"><a name="p1683372283118"></a><a name="p1683372283118"></a>dst的元素取值</p>
<p id="p2083311227312"><a name="p2083311227312"></a><a name="p2083311227312"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row283372214319"><td class="cellrowborder" rowspan="27" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p683312273114"><a name="p683312273114"></a><a name="p683312273114"></a>half</p>
</td>
<td class="cellrowborder" rowspan="7" valign="top" width="10.13%" headers="mcps1.2.6.1.2 "><p id="p3833202218315"><a name="p3833202218315"></a><a name="p3833202218315"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="29.87%" headers="mcps1.2.6.1.3 "><p id="p3833122215318"><a name="p3833122215318"></a><a name="p3833122215318"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p58331722143115"><a name="p58331722143115"></a><a name="p58331722143115"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p4961131545512"><a name="p4961131545512"></a><a name="p4961131545512"></a>1.0</p>
</td>
</tr>
<tr id="row18833322173114"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p128338227318"><a name="p128338227318"></a><a name="p128338227318"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1483322263111"><a name="p1483322263111"></a><a name="p1483322263111"></a>-1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p996161545515"><a name="p996161545515"></a><a name="p996161545515"></a>-1.0</p>
</td>
</tr>
<tr id="row2833152220316"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1283302203113"><a name="p1283302203113"></a><a name="p1283302203113"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p62556130185"><a name="p62556130185"></a><a name="p62556130185"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16051920105515"><a name="p16051920105515"></a><a name="p16051920105515"></a><strong id="b142292710554"><a name="b142292710554"></a><a name="b142292710554"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1535814101175"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p0358161012173"><a name="p0358161012173"></a><a name="p0358161012173"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p102541113101815"><a name="p102541113101815"></a><a name="p102541113101815"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20784132165518"><a name="p20784132165518"></a><a name="p20784132165518"></a><strong id="b2042302710554"><a name="b2042302710554"></a><a name="b2042302710554"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row88341225319"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p783452215318"><a name="p783452215318"></a><a name="p783452215318"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p19834322133118"><a name="p19834322133118"></a><a name="p19834322133118"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p109617158553"><a name="p109617158553"></a><a name="p109617158553"></a><strong id="b142414276551"><a name="b142414276551"></a><a name="b142414276551"></a>0.0</strong></p>
</td>
</tr>
<tr id="row5834322153116"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p483416229319"><a name="p483416229319"></a><a name="p483416229319"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5834152216310"><a name="p5834152216310"></a><a name="p5834152216310"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p396111157556"><a name="p396111157556"></a><a name="p396111157556"></a>-0.0</p>
</td>
</tr>
<tr id="row20834322173118"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p583417221311"><a name="p583417221311"></a><a name="p583417221311"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15251113181816"><a name="p15251113181816"></a><a name="p15251113181816"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10961815135516"><a name="p10961815135516"></a><a name="p10961815135516"></a>0.0</p>
</td>
</tr>
<tr id="row4692133618558"><td class="cellrowborder" rowspan="7" valign="top" headers="mcps1.2.6.1.1 "><p id="p1769213615520"><a name="p1769213615520"></a><a name="p1769213615520"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p175251441205511"><a name="p175251441205511"></a><a name="p175251441205511"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1769213685511"><a name="p1769213685511"></a><a name="p1769213685511"></a>-1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p2809192589"><a name="p2809192589"></a><a name="p2809192589"></a>-1.0</p>
</td>
</tr>
<tr id="row19834132218316"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1383542213311"><a name="p1383542213311"></a><a name="p1383542213311"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p183552213313"><a name="p183552213313"></a><a name="p183552213313"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p148098955813"><a name="p148098955813"></a><a name="p148098955813"></a>1.0</p>
</td>
</tr>
<tr id="row083562215312"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11835152218314"><a name="p11835152218314"></a><a name="p11835152218314"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p283514228318"><a name="p283514228318"></a><a name="p283514228318"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18622141414582"><a name="p18622141414582"></a><a name="p18622141414582"></a><strong id="b16622111412583"><a name="b16622111412583"></a><a name="b16622111412583"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row12358131010173"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1835981015171"><a name="p1835981015171"></a><a name="p1835981015171"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1248191314187"><a name="p1248191314187"></a><a name="p1248191314187"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p56228140584"><a name="p56228140584"></a><a name="p56228140584"></a><strong id="b1062251412587"><a name="b1062251412587"></a><a name="b1062251412587"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row11835182214315"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p138352228310"><a name="p138352228310"></a><a name="p138352228310"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1583522273110"><a name="p1583522273110"></a><a name="p1583522273110"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p13622111465815"><a name="p13622111465815"></a><a name="p13622111465815"></a><strong id="b136220149587"><a name="b136220149587"></a><a name="b136220149587"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1183582213310"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14835122193111"><a name="p14835122193111"></a><a name="p14835122193111"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p183542211313"><a name="p183542211313"></a><a name="p183542211313"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2810095589"><a name="p2810095589"></a><a name="p2810095589"></a>0.0</p>
</td>
</tr>
<tr id="row10835622193115"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p283532211314"><a name="p283532211314"></a><a name="p283532211314"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18245111381815"><a name="p18245111381815"></a><a name="p18245111381815"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p0810149125814"><a name="p0810149125814"></a><a name="p0810149125814"></a>-0.0</p>
</td>
</tr>
<tr id="row683514227315"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p483512227317"><a name="p483512227317"></a><a name="p483512227317"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p4970153921915"><a name="p4970153921915"></a><a name="p4970153921915"></a>-65504.0（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20244613131814"><a name="p20244613131814"></a><a name="p20244613131814"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p186572037152012"><a name="p186572037152012"></a><a name="p186572037152012"></a>-0.0</p>
</td>
</tr>
<tr id="row8359101031714"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p94815482019"><a name="p94815482019"></a><a name="p94815482019"></a>65504.0（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p152431013161813"><a name="p152431013161813"></a><a name="p152431013161813"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14658143718203"><a name="p14658143718203"></a><a name="p14658143718203"></a>0.0</p>
</td>
</tr>
<tr id="row983652243112"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17243101413209"><a name="p17243101413209"></a><a name="p17243101413209"></a>0.0/-0.0/nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p102421213101811"><a name="p102421213101811"></a><a name="p102421213101811"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p203394072012"><a name="p203394072012"></a><a name="p203394072012"></a><strong id="b12339408209"><a name="b12339408209"></a><a name="b12339408209"></a>0.0</strong></p>
</td>
</tr>
<tr id="row18360191010175"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p193606102179"><a name="p193606102179"></a><a name="p193606102179"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p340118152112"><a name="p340118152112"></a><a name="p340118152112"></a>-65504.0（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14389171662114"><a name="p14389171662114"></a><a name="p14389171662114"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1163041732119"><a name="p1163041732119"></a><a name="p1163041732119"></a>0.0</p>
</td>
</tr>
<tr id="row1360171014174"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p74019820219"><a name="p74019820219"></a><a name="p74019820219"></a>65504.0（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1872411192118"><a name="p1872411192118"></a><a name="p1872411192118"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2872311132120"><a name="p2872311132120"></a><a name="p2872311132120"></a>-0.0</p>
</td>
</tr>
<tr id="row7848194582013"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p20401682210"><a name="p20401682210"></a><a name="p20401682210"></a>0.0/-0.0/nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p48729119216"><a name="p48729119216"></a><a name="p48729119216"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p118721611102117"><a name="p118721611102117"></a><a name="p118721611102117"></a><strong id="b587220110211"><a name="b587220110211"></a><a name="b587220110211"></a>0.0</strong></p>
</td>
</tr>
<tr id="row9836122153110"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1783612222317"><a name="p1783612222317"></a><a name="p1783612222317"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1664814394227"><a name="p1664814394227"></a><a name="p1664814394227"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1783652216310"><a name="p1783652216310"></a><a name="p1783652216310"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p4830230132315"><a name="p4830230132315"></a><a name="p4830230132315"></a><strong id="b16831173092314"><a name="b16831173092314"></a><a name="b16831173092314"></a>0.0</strong></p>
</td>
</tr>
<tr id="row483720222312"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p1883715225313"><a name="p1883715225313"></a><a name="p1883715225313"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18737912242"><a name="p18737912242"></a><a name="p18737912242"></a>-65504.0（MIN）/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p683722217317"><a name="p683722217317"></a><a name="p683722217317"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p194651215182518"><a name="p194651215182518"></a><a name="p194651215182518"></a><strong id="b146510150255"><a name="b146510150255"></a><a name="b146510150255"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row17386640192314"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17195111722418"><a name="p17195111722418"></a><a name="p17195111722418"></a>65504.0（MAX）/0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10387144015235"><a name="p10387144015235"></a><a name="p10387144015235"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p136891316102518"><a name="p136891316102518"></a><a name="p136891316102518"></a><strong id="b1568914168251"><a name="b1568914168251"></a><a name="b1568914168251"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row8940164272314"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p13941164292315"><a name="p13941164292315"></a><a name="p13941164292315"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1894124211238"><a name="p1894124211238"></a><a name="p1894124211238"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p46611218251"><a name="p46611218251"></a><a name="p46611218251"></a><strong id="b136652142516"><a name="b136652142516"></a><a name="b136652142516"></a>0.0</strong></p>
</td>
</tr>
<tr id="row383722215312"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p126071651142411"><a name="p126071651142411"></a><a name="p126071651142411"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p648674732418"><a name="p648674732418"></a><a name="p648674732418"></a>-65504.0（MIN）/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1271020235256"><a name="p1271020235256"></a><a name="p1271020235256"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1471022302519"><a name="p1471022302519"></a><a name="p1471022302519"></a><strong id="b1871032372518"><a name="b1871032372518"></a><a name="b1871032372518"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row17837322183119"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4486847162412"><a name="p4486847162412"></a><a name="p4486847162412"></a>65504.0（MAX）/0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1971002392514"><a name="p1971002392514"></a><a name="p1971002392514"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1710102342518"><a name="p1710102342518"></a><a name="p1710102342518"></a><strong id="b7711323152515"><a name="b7711323152515"></a><a name="b7711323152515"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row12442946102412"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1948694713241"><a name="p1948694713241"></a><a name="p1948694713241"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13711132342510"><a name="p13711132342510"></a><a name="p13711132342510"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p871112392518"><a name="p871112392518"></a><a name="p871112392518"></a><strong id="b0711192312255"><a name="b0711192312255"></a><a name="b0711192312255"></a>0.0</strong></p>
</td>
</tr>
<tr id="row313312125917"><td class="cellrowborder" rowspan="27" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p876544920220"><a name="p876544920220"></a><a name="p876544920220"></a>float</p>
</td>
<td class="cellrowborder" rowspan="7" valign="top" width="10.13%" headers="mcps1.2.6.1.2 "><p id="p2080912331824"><a name="p2080912331824"></a><a name="p2080912331824"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="29.87%" headers="mcps1.2.6.1.3 "><p id="p1080919333219"><a name="p1080919333219"></a><a name="p1080919333219"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p198092331219"><a name="p198092331219"></a><a name="p198092331219"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p3809133318216"><a name="p3809133318216"></a><a name="p3809133318216"></a>1.0</p>
</td>
</tr>
<tr id="row1836174195919"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4809133622"><a name="p4809133622"></a><a name="p4809133622"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p158099331729"><a name="p158099331729"></a><a name="p158099331729"></a>-1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p108091733621"><a name="p108091733621"></a><a name="p108091733621"></a>-1.0</p>
</td>
</tr>
<tr id="row1910919675914"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8809103319217"><a name="p8809103319217"></a><a name="p8809103319217"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1280914331824"><a name="p1280914331824"></a><a name="p1280914331824"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1180918334218"><a name="p1180918334218"></a><a name="p1180918334218"></a>-inf</p>
</td>
</tr>
<tr id="row199811510105912"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1180910334213"><a name="p1180910334213"></a><a name="p1180910334213"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1980910336219"><a name="p1980910336219"></a><a name="p1980910336219"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p580913333210"><a name="p580913333210"></a><a name="p580913333210"></a>inf</p>
</td>
</tr>
<tr id="row4981610145915"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p58090336213"><a name="p58090336213"></a><a name="p58090336213"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16809193311218"><a name="p16809193311218"></a><a name="p16809193311218"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p17809183314212"><a name="p17809183314212"></a><a name="p17809183314212"></a>nan</p>
</td>
</tr>
<tr id="row598131012592"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p180912337213"><a name="p180912337213"></a><a name="p180912337213"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p480912339212"><a name="p480912339212"></a><a name="p480912339212"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p58097330212"><a name="p58097330212"></a><a name="p58097330212"></a>-0.0</p>
</td>
</tr>
<tr id="row13750191555915"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p108091331226"><a name="p108091331226"></a><a name="p108091331226"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p781010331924"><a name="p781010331924"></a><a name="p781010331924"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19810163312210"><a name="p19810163312210"></a><a name="p19810163312210"></a>0.0</p>
</td>
</tr>
<tr id="row67501215115919"><td class="cellrowborder" rowspan="7" valign="top" headers="mcps1.2.6.1.1 "><p id="p6810173315215"><a name="p6810173315215"></a><a name="p6810173315215"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1181093317210"><a name="p1181093317210"></a><a name="p1181093317210"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p881013331628"><a name="p881013331628"></a><a name="p881013331628"></a>-1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p781013331925"><a name="p781013331925"></a><a name="p781013331925"></a>-1.0</p>
</td>
</tr>
<tr id="row137509155595"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p381010331621"><a name="p381010331621"></a><a name="p381010331621"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p17810143315216"><a name="p17810143315216"></a><a name="p17810143315216"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p98103331826"><a name="p98103331826"></a><a name="p98103331826"></a>1.0</p>
</td>
</tr>
<tr id="row6751915175915"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14810233629"><a name="p14810233629"></a><a name="p14810233629"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8810183317215"><a name="p8810183317215"></a><a name="p8810183317215"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4810233825"><a name="p4810233825"></a><a name="p4810233825"></a>inf</p>
</td>
</tr>
<tr id="row1575141555911"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p581016331821"><a name="p581016331821"></a><a name="p581016331821"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1810833322"><a name="p1810833322"></a><a name="p1810833322"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p168101933627"><a name="p168101933627"></a><a name="p168101933627"></a>-inf</p>
</td>
</tr>
<tr id="row16751715165916"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p208104332214"><a name="p208104332214"></a><a name="p208104332214"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p7810133317213"><a name="p7810133317213"></a><a name="p7810133317213"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p198109337218"><a name="p198109337218"></a><a name="p198109337218"></a>nan</p>
</td>
</tr>
<tr id="row132325288593"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p188101133523"><a name="p188101133523"></a><a name="p188101133523"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1581016331423"><a name="p1581016331423"></a><a name="p1581016331423"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p781083311212"><a name="p781083311212"></a><a name="p781083311212"></a>0.0</p>
</td>
</tr>
<tr id="row11232132885916"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p881018336213"><a name="p881018336213"></a><a name="p881018336213"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p208101333929"><a name="p208101333929"></a><a name="p208101333929"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p781020330213"><a name="p781020330213"></a><a name="p781020330213"></a>-0.0</p>
</td>
</tr>
<tr id="row10233728125918"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p98101339212"><a name="p98101339212"></a><a name="p98101339212"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p168106336220"><a name="p168106336220"></a><a name="p168106336220"></a>-3.4028235e+38（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1811133926"><a name="p1811133926"></a><a name="p1811133926"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p581153311217"><a name="p581153311217"></a><a name="p581153311217"></a>-0.0</p>
</td>
</tr>
<tr id="row1823472818590"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p158111233923"><a name="p158111233923"></a><a name="p158111233923"></a>3.4028235e+38（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p7811123312211"><a name="p7811123312211"></a><a name="p7811123312211"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p481117331026"><a name="p481117331026"></a><a name="p481117331026"></a>0.0</p>
</td>
</tr>
<tr id="row8234202875917"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1181112338220"><a name="p1181112338220"></a><a name="p1181112338220"></a>0.0/-0.0/nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1481173320211"><a name="p1481173320211"></a><a name="p1481173320211"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p13811193315212"><a name="p13811193315212"></a><a name="p13811193315212"></a>nan</p>
</td>
</tr>
<tr id="row2234228205911"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p1781113313215"><a name="p1781113313215"></a><a name="p1781113313215"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p178112033227"><a name="p178112033227"></a><a name="p178112033227"></a>-3.4028235e+38（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19811173317220"><a name="p19811173317220"></a><a name="p19811173317220"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1681120331222"><a name="p1681120331222"></a><a name="p1681120331222"></a>0.0</p>
</td>
</tr>
<tr id="row19234142855917"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p148118330212"><a name="p148118330212"></a><a name="p148118330212"></a>3.4028235e+38（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p98116336218"><a name="p98116336218"></a><a name="p98116336218"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p78117331529"><a name="p78117331529"></a><a name="p78117331529"></a>-0.0</p>
</td>
</tr>
<tr id="row14235028195911"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p108110336217"><a name="p108110336217"></a><a name="p108110336217"></a>0.0/-0.0/nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p181114338210"><a name="p181114338210"></a><a name="p181114338210"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p168117331229"><a name="p168117331229"></a><a name="p168117331229"></a>nan</p>
</td>
</tr>
<tr id="row923572816596"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1581114331529"><a name="p1581114331529"></a><a name="p1581114331529"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p138112337217"><a name="p138112337217"></a><a name="p138112337217"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p168117336217"><a name="p168117336217"></a><a name="p168117336217"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1881115331826"><a name="p1881115331826"></a><a name="p1881115331826"></a>nan</p>
</td>
</tr>
<tr id="row82351328135916"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p28111833525"><a name="p28111833525"></a><a name="p28111833525"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2811123314212"><a name="p2811123314212"></a><a name="p2811123314212"></a>-3.4028235e+38（MIN）/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p38111933225"><a name="p38111933225"></a><a name="p38111933225"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p5811203311216"><a name="p5811203311216"></a><a name="p5811203311216"></a>-inf</p>
</td>
</tr>
<tr id="row2235152810591"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p88111331021"><a name="p88111331021"></a><a name="p88111331021"></a>3.4028235e+38（MAX）/0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p118119335213"><a name="p118119335213"></a><a name="p118119335213"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p681243311212"><a name="p681243311212"></a><a name="p681243311212"></a>inf</p>
</td>
</tr>
<tr id="row1523512825917"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2812113317215"><a name="p2812113317215"></a><a name="p2812113317215"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p88129331724"><a name="p88129331724"></a><a name="p88129331724"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11812933727"><a name="p11812933727"></a><a name="p11812933727"></a>nan</p>
</td>
</tr>
<tr id="row7713632185910"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p138121233621"><a name="p138121233621"></a><a name="p138121233621"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12812433628"><a name="p12812433628"></a><a name="p12812433628"></a>-3.4028235e+38（MIN）/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p381223319215"><a name="p381223319215"></a><a name="p381223319215"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p7812173310220"><a name="p7812173310220"></a><a name="p7812173310220"></a>inf</p>
</td>
</tr>
<tr id="row5714832135915"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1181211332210"><a name="p1181211332210"></a><a name="p1181211332210"></a>3.4028235e+38（MAX）/0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p981215331922"><a name="p981215331922"></a><a name="p981215331922"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1681215333211"><a name="p1681215333211"></a><a name="p1681215333211"></a>-inf</p>
</td>
</tr>
<tr id="row5714932175912"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2081213331523"><a name="p2081213331523"></a><a name="p2081213331523"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1812133527"><a name="p1812133527"></a><a name="p1812133527"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1812183310216"><a name="p1812183310216"></a><a name="p1812183310216"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 13**  Max特殊值/边界值输入的计算结果

<a name="table172607531318"></a>
<table><thead align="left"><tr id="row32606535315"><th class="cellrowborder" valign="top" width="15.93%" id="mcps1.2.6.1.1"><p id="p11260453133118"><a name="p11260453133118"></a><a name="p11260453133118"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="13.76%" id="mcps1.2.6.1.2"><p id="p142601453113111"><a name="p142601453113111"></a><a name="p142601453113111"></a>src0的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="30.31%" id="mcps1.2.6.1.3"><p id="p13260175313114"><a name="p13260175313114"></a><a name="p13260175313114"></a>src1的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p42602534319"><a name="p42602534319"></a><a name="p42602534319"></a>dst的元素取值</p>
<p id="p19260253193115"><a name="p19260253193115"></a><a name="p19260253193115"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p82606530312"><a name="p82606530312"></a><a name="p82606530312"></a>dst的元素取值</p>
<p id="p17260135314316"><a name="p17260135314316"></a><a name="p17260135314316"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row9260853113114"><td class="cellrowborder" rowspan="18" valign="top" width="15.93%" headers="mcps1.2.6.1.1 "><p id="p18260155320317"><a name="p18260155320317"></a><a name="p18260155320317"></a>half</p>
</td>
<td class="cellrowborder" rowspan="6" valign="top" width="13.76%" headers="mcps1.2.6.1.2 "><p id="p192603539312"><a name="p192603539312"></a><a name="p192603539312"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.31%" headers="mcps1.2.6.1.3 "><p id="p226125315313"><a name="p226125315313"></a><a name="p226125315313"></a>-65504.0（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p16261145315318"><a name="p16261145315318"></a><a name="p16261145315318"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p106462040162812"><a name="p106462040162812"></a><a name="p106462040162812"></a>-65504.0</p>
</td>
</tr>
<tr id="row3261653163120"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p326195383114"><a name="p326195383114"></a><a name="p326195383114"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p22611453133110"><a name="p22611453133110"></a><a name="p22611453133110"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p86461440202815"><a name="p86461440202815"></a><a name="p86461440202815"></a>65504.0</p>
</td>
</tr>
<tr id="row5261853163112"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17261853173116"><a name="p17261853173116"></a><a name="p17261853173116"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1026120538316"><a name="p1026120538316"></a><a name="p1026120538316"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9646124022818"><a name="p9646124022818"></a><a name="p9646124022818"></a>0.0</p>
</td>
</tr>
<tr id="row72614532310"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p42611537316"><a name="p42611537316"></a><a name="p42611537316"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1126119537315"><a name="p1126119537315"></a><a name="p1126119537315"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p464684018289"><a name="p464684018289"></a><a name="p464684018289"></a>-0.0</p>
</td>
</tr>
<tr id="row162611653163112"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p20261253163111"><a name="p20261253163111"></a><a name="p20261253163111"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p112612053193113"><a name="p112612053193113"></a><a name="p112612053193113"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p186464402281"><a name="p186464402281"></a><a name="p186464402281"></a><strong id="b113655402816"><a name="b113655402816"></a><a name="b113655402816"></a>0.0</strong></p>
</td>
</tr>
<tr id="row92611553143119"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p926110535314"><a name="p926110535314"></a><a name="p926110535314"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1526125383110"><a name="p1526125383110"></a><a name="p1526125383110"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p6646144019282"><a name="p6646144019282"></a><a name="p6646144019282"></a><strong id="b2401547289"><a name="b2401547289"></a><a name="b2401547289"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row159125915284"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p121014597282"><a name="p121014597282"></a><a name="p121014597282"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p05441127297"><a name="p05441127297"></a><a name="p05441127297"></a>65504.0（MAX）/0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p310115942812"><a name="p310115942812"></a><a name="p310115942812"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1658231512300"><a name="p1658231512300"></a><a name="p1658231512300"></a>65504.0</p>
</td>
</tr>
<tr id="row13262125353116"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1826295313110"><a name="p1826295313110"></a><a name="p1826295313110"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10262175313110"><a name="p10262175313110"></a><a name="p10262175313110"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1357121193011"><a name="p1357121193011"></a><a name="p1357121193011"></a><strong id="b9851228133010"><a name="b9851228133010"></a><a name="b9851228133010"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1526255317317"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p726285353111"><a name="p726285353111"></a><a name="p726285353111"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8262125333110"><a name="p8262125333110"></a><a name="p8262125333110"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1384492611308"><a name="p1384492611308"></a><a name="p1384492611308"></a><strong id="b88561028193010"><a name="b88561028193010"></a><a name="b88561028193010"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1026295313312"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p3263125317317"><a name="p3263125317317"></a><a name="p3263125317317"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p132102219325"><a name="p132102219325"></a><a name="p132102219325"></a>0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p826345353113"><a name="p826345353113"></a><a name="p826345353113"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p117877241332"><a name="p117877241332"></a><a name="p117877241332"></a>0.0</p>
</td>
</tr>
<tr id="row1126317539312"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11359111014171"><a name="p11359111014171"></a><a name="p11359111014171"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11263253163111"><a name="p11263253163111"></a><a name="p11263253163111"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p426612753316"><a name="p426612753316"></a><a name="p426612753316"></a><strong id="b11266122733314"><a name="b11266122733314"></a><a name="b11266122733314"></a>0.0</strong></p>
</td>
</tr>
<tr id="row20159711143311"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p915961153312"><a name="p915961153312"></a><a name="p915961153312"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p31592011193314"><a name="p31592011193314"></a><a name="p31592011193314"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p13266162711334"><a name="p13266162711334"></a><a name="p13266162711334"></a><strong id="b1926632763315"><a name="b1926632763315"></a><a name="b1926632763315"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row326315343115"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p1526312536314"><a name="p1526312536314"></a><a name="p1526312536314"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p202735043417"><a name="p202735043417"></a><a name="p202735043417"></a>-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15241161313185"><a name="p15241161313185"></a><a name="p15241161313185"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1811135714344"><a name="p1811135714344"></a><a name="p1811135714344"></a>-0.0</p>
</td>
</tr>
<tr id="row1615010466339"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p61501462335"><a name="p61501462335"></a><a name="p61501462335"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16150246133313"><a name="p16150246133313"></a><a name="p16150246133313"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p458165993415"><a name="p458165993415"></a><a name="p458165993415"></a><strong id="b658111599343"><a name="b658111599343"></a><a name="b658111599343"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1026418539314"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p153605109171"><a name="p153605109171"></a><a name="p153605109171"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p824011311181"><a name="p824011311181"></a><a name="p824011311181"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p6581125983415"><a name="p6581125983415"></a><a name="p6581125983415"></a><strong id="b13581185910344"><a name="b13581185910344"></a><a name="b13581185910344"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1726414533311"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1626415393112"><a name="p1626415393112"></a><a name="p1626415393112"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p177989154359"><a name="p177989154359"></a><a name="p177989154359"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1526465315316"><a name="p1526465315316"></a><a name="p1526465315316"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p119031835163517"><a name="p119031835163517"></a><a name="p119031835163517"></a><strong id="b2903123519352"><a name="b2903123519352"></a><a name="b2903123519352"></a>0.0</strong></p>
</td>
</tr>
<tr id="row9264195363117"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17264105319310"><a name="p17264105319310"></a><a name="p17264105319310"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8671141361"><a name="p8671141361"></a><a name="p8671141361"></a>inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p12264053143117"><a name="p12264053143117"></a><a name="p12264053143117"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p570771810368"><a name="p570771810368"></a><a name="p570771810368"></a><strong id="b0708141843610"><a name="b0708141843610"></a><a name="b0708141843610"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row39991049133619"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p126575333114"><a name="p126575333114"></a><a name="p126575333114"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p19991449203612"><a name="p19991449203612"></a><a name="p19991449203612"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p97370459375"><a name="p97370459375"></a><a name="p97370459375"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p107341018383"><a name="p107341018383"></a><a name="p107341018383"></a><strong id="b1073420043817"><a name="b1073420043817"></a><a name="b1073420043817"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row52332528412"><td class="cellrowborder" rowspan="18" valign="top" width="15.93%" headers="mcps1.2.6.1.1 "><p id="p25381528256"><a name="p25381528256"></a><a name="p25381528256"></a>float</p>
</td>
<td class="cellrowborder" rowspan="6" valign="top" width="13.76%" headers="mcps1.2.6.1.2 "><p id="p101445232051"><a name="p101445232051"></a><a name="p101445232051"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.31%" headers="mcps1.2.6.1.3 "><p id="p12144152319513"><a name="p12144152319513"></a><a name="p12144152319513"></a>-3.4028235e+38（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p2144152316517"><a name="p2144152316517"></a><a name="p2144152316517"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1914420231056"><a name="p1914420231056"></a><a name="p1914420231056"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row15266541242"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p314452315512"><a name="p314452315512"></a><a name="p314452315512"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p214492317510"><a name="p214492317510"></a><a name="p214492317510"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p181441239516"><a name="p181441239516"></a><a name="p181441239516"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row16441757748"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p111445238519"><a name="p111445238519"></a><a name="p111445238519"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p714415231756"><a name="p714415231756"></a><a name="p714415231756"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19144132319511"><a name="p19144132319511"></a><a name="p19144132319511"></a>0.0</p>
</td>
</tr>
<tr id="row141662317518"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1514419231259"><a name="p1514419231259"></a><a name="p1514419231259"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1814415234511"><a name="p1814415234511"></a><a name="p1814415234511"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1714411231756"><a name="p1714411231756"></a><a name="p1714411231756"></a>-0.0</p>
</td>
</tr>
<tr id="row11671631859"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1214418231756"><a name="p1214418231756"></a><a name="p1214418231756"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6144172314513"><a name="p6144172314513"></a><a name="p6144172314513"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p131444231453"><a name="p131444231453"></a><a name="p131444231453"></a>nan</p>
</td>
</tr>
<tr id="row111671631359"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1144623652"><a name="p1144623652"></a><a name="p1144623652"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p101444236512"><a name="p101444236512"></a><a name="p101444236512"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9144122311519"><a name="p9144122311519"></a><a name="p9144122311519"></a>inf</p>
</td>
</tr>
<tr id="row4197157452"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p12145182311514"><a name="p12145182311514"></a><a name="p12145182311514"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p41451523652"><a name="p41451523652"></a><a name="p41451523652"></a>3.4028235e+38（MAX）/0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15145623152"><a name="p15145623152"></a><a name="p15145623152"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p614518237520"><a name="p614518237520"></a><a name="p614518237520"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row01974713517"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17145523856"><a name="p17145523856"></a><a name="p17145523856"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1914542320515"><a name="p1914542320515"></a><a name="p1914542320515"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p714519231551"><a name="p714519231551"></a><a name="p714519231551"></a>nan</p>
</td>
</tr>
<tr id="row6198672515"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p111455231153"><a name="p111455231153"></a><a name="p111455231153"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p514572311516"><a name="p514572311516"></a><a name="p514572311516"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p514592314511"><a name="p514592314511"></a><a name="p514592314511"></a>inf</p>
</td>
</tr>
<tr id="row0198127154"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p12145192317517"><a name="p12145192317517"></a><a name="p12145192317517"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1114510232516"><a name="p1114510232516"></a><a name="p1114510232516"></a>0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1114532320515"><a name="p1114532320515"></a><a name="p1114532320515"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p15145172319520"><a name="p15145172319520"></a><a name="p15145172319520"></a>0.0</p>
</td>
</tr>
<tr id="row81987710515"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p114532320510"><a name="p114532320510"></a><a name="p114532320510"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2145923052"><a name="p2145923052"></a><a name="p2145923052"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16145152317520"><a name="p16145152317520"></a><a name="p16145152317520"></a>nan</p>
</td>
</tr>
<tr id="row71981271851"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p10145923457"><a name="p10145923457"></a><a name="p10145923457"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p61458231951"><a name="p61458231951"></a><a name="p61458231951"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p014522313513"><a name="p014522313513"></a><a name="p014522313513"></a>inf</p>
</td>
</tr>
<tr id="row164925131951"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p181459231256"><a name="p181459231256"></a><a name="p181459231256"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1814552312510"><a name="p1814552312510"></a><a name="p1814552312510"></a>-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p191457231858"><a name="p191457231858"></a><a name="p191457231858"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1114552313515"><a name="p1114552313515"></a><a name="p1114552313515"></a>-0.0</p>
</td>
</tr>
<tr id="row849220131357"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p314516239518"><a name="p314516239518"></a><a name="p314516239518"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12145122316516"><a name="p12145122316516"></a><a name="p12145122316516"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1145102312518"><a name="p1145102312518"></a><a name="p1145102312518"></a>nan</p>
</td>
</tr>
<tr id="row149381313515"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p201461523253"><a name="p201461523253"></a><a name="p201461523253"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p61463233519"><a name="p61463233519"></a><a name="p61463233519"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p114632319514"><a name="p114632319514"></a><a name="p114632319514"></a>inf</p>
</td>
</tr>
<tr id="row174931713653"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p16146623757"><a name="p16146623757"></a><a name="p16146623757"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p161461123359"><a name="p161461123359"></a><a name="p161461123359"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p171461123752"><a name="p171461123752"></a><a name="p171461123752"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1614615239517"><a name="p1614615239517"></a><a name="p1614615239517"></a>nan</p>
</td>
</tr>
<tr id="row84931713859"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p21462231354"><a name="p21462231354"></a><a name="p21462231354"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1714617233512"><a name="p1714617233512"></a><a name="p1714617233512"></a>inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p0146182317518"><a name="p0146182317518"></a><a name="p0146182317518"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p14146122314517"><a name="p14146122314517"></a><a name="p14146122314517"></a>inf</p>
</td>
</tr>
<tr id="row1749341310519"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1214619231557"><a name="p1214619231557"></a><a name="p1214619231557"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p21461230514"><a name="p21461230514"></a><a name="p21461230514"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p161461423155"><a name="p161461423155"></a><a name="p161461423155"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1214632311511"><a name="p1214632311511"></a><a name="p1214632311511"></a>-inf</p>
</td>
</tr>
<tr id="row102651539319"><td class="cellrowborder" rowspan="3" valign="top" width="15.93%" headers="mcps1.2.6.1.1 "><p id="p82651953133113"><a name="p82651953133113"></a><a name="p82651953133113"></a>int16_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="13.76%" headers="mcps1.2.6.1.2 "><p id="p326525316314"><a name="p326525316314"></a><a name="p326525316314"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.31%" headers="mcps1.2.6.1.3 "><p id="p172652053193118"><a name="p172652053193118"></a><a name="p172652053193118"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p4816193733817"><a name="p4816193733817"></a><a name="p4816193733817"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1339353143812"><a name="p1339353143812"></a><a name="p1339353143812"></a>-32768</p>
</td>
</tr>
<tr id="row1826545373120"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p152652533317"><a name="p152652533317"></a><a name="p152652533317"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p281617371385"><a name="p281617371385"></a><a name="p281617371385"></a>32767</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1539185314388"><a name="p1539185314388"></a><a name="p1539185314388"></a>32767</p>
</td>
</tr>
<tr id="row426519532311"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2026515533313"><a name="p2026515533313"></a><a name="p2026515533313"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1436113104172"><a name="p1436113104172"></a><a name="p1436113104172"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2056014294017"><a name="p2056014294017"></a><a name="p2056014294017"></a>32767</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p2056174220401"><a name="p2056174220401"></a><a name="p2056174220401"></a>32767</p>
</td>
</tr>
<tr id="row11694105943918"><td class="cellrowborder" rowspan="3" valign="top" width="15.93%" headers="mcps1.2.6.1.1 "><p id="p2694459143917"><a name="p2694459143917"></a><a name="p2694459143917"></a>int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="13.76%" headers="mcps1.2.6.1.2 "><p id="p1069445953919"><a name="p1069445953919"></a><a name="p1069445953919"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.31%" headers="mcps1.2.6.1.3 "><p id="p1584504054112"><a name="p1584504054112"></a><a name="p1584504054112"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p13466165312428"><a name="p13466165312428"></a><a name="p13466165312428"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p164719533424"><a name="p164719533424"></a><a name="p164719533424"></a>-2147483648</p>
</td>
</tr>
<tr id="row2026655373117"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p78451240164119"><a name="p78451240164119"></a><a name="p78451240164119"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p84801556134218"><a name="p84801556134218"></a><a name="p84801556134218"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p194831656134219"><a name="p194831656134219"></a><a name="p194831656134219"></a>2147483647</p>
</td>
</tr>
<tr id="row102669533317"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p62667532315"><a name="p62667532315"></a><a name="p62667532315"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p79687478413"><a name="p79687478413"></a><a name="p79687478413"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p02301313111812"><a name="p02301313111812"></a><a name="p02301313111812"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p15209161316181"><a name="p15209161316181"></a><a name="p15209161316181"></a>2147483647</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 14**  Min特殊值/边界值输入的计算结果说明

<a name="table179652226325"></a>
<table><thead align="left"><tr id="row5966922193217"><th class="cellrowborder" valign="top" width="16.42%" id="mcps1.2.6.1.1"><p id="p8966222173211"><a name="p8966222173211"></a><a name="p8966222173211"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="13.22%" id="mcps1.2.6.1.2"><p id="p9966112283211"><a name="p9966112283211"></a><a name="p9966112283211"></a>src0的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="30.36%" id="mcps1.2.6.1.3"><p id="p129661422133216"><a name="p129661422133216"></a><a name="p129661422133216"></a>src1的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p1966112283213"><a name="p1966112283213"></a><a name="p1966112283213"></a>dst的元素取值</p>
<p id="p796632263214"><a name="p796632263214"></a><a name="p796632263214"></a>（寄存器非饱和模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p149668229323"><a name="p149668229323"></a><a name="p149668229323"></a>dst的元素取值</p>
<p id="p18966522123212"><a name="p18966522123212"></a><a name="p18966522123212"></a>（寄存器饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row4966622123216"><td class="cellrowborder" rowspan="19" valign="top" width="16.42%" headers="mcps1.2.6.1.1 "><p id="p17966162213322"><a name="p17966162213322"></a><a name="p17966162213322"></a>half</p>
</td>
<td class="cellrowborder" rowspan="3" valign="top" width="13.22%" headers="mcps1.2.6.1.2 "><p id="p496692243211"><a name="p496692243211"></a><a name="p496692243211"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.36%" headers="mcps1.2.6.1.3 "><p id="p39661522183217"><a name="p39661522183217"></a><a name="p39661522183217"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p15966822123214"><a name="p15966822123214"></a><a name="p15966822123214"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p425751310181"><a name="p425751310181"></a><a name="p425751310181"></a>-65504.0</p>
</td>
</tr>
<tr id="row179662226321"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11966182293212"><a name="p11966182293212"></a><a name="p11966182293212"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p413681195016"><a name="p413681195016"></a><a name="p413681195016"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7256313131815"><a name="p7256313131815"></a><a name="p7256313131815"></a><strong id="b1630519362506"><a name="b1630519362506"></a><a name="b1630519362506"></a>0.0</strong></p>
</td>
</tr>
<tr id="row696614227328"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p996672283217"><a name="p996672283217"></a><a name="p996672283217"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p121361711145015"><a name="p121361711145015"></a><a name="p121361711145015"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p946211339505"><a name="p946211339505"></a><a name="p946211339505"></a><strong id="b18310163618509"><a name="b18310163618509"></a><a name="b18310163618509"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row16967722143211"><td class="cellrowborder" rowspan="5" valign="top" headers="mcps1.2.6.1.1 "><p id="p21424444618"><a name="p21424444618"></a><a name="p21424444618"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13274105824019"><a name="p13274105824019"></a><a name="p13274105824019"></a>65504.0（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p896752217321"><a name="p896752217321"></a><a name="p896752217321"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p124911301820"><a name="p124911301820"></a><a name="p124911301820"></a>65504.0</p>
</td>
</tr>
<tr id="row119671322113212"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p3967122273215"><a name="p3967122273215"></a><a name="p3967122273215"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p186321726174116"><a name="p186321726174116"></a><a name="p186321726174116"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1553127164119"><a name="p1553127164119"></a><a name="p1553127164119"></a>0.0</p>
</td>
</tr>
<tr id="row10967132218323"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p3967102210323"><a name="p3967102210323"></a><a name="p3967102210323"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1127523312418"><a name="p1127523312418"></a><a name="p1127523312418"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1180103418415"><a name="p1180103418415"></a><a name="p1180103418415"></a>-0.0</p>
</td>
</tr>
<tr id="row17967222153210"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p149671822103213"><a name="p149671822103213"></a><a name="p149671822103213"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1796710221323"><a name="p1796710221323"></a><a name="p1796710221323"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1333564744114"><a name="p1333564744114"></a><a name="p1333564744114"></a><strong id="b17336124774116"><a name="b17336124774116"></a><a name="b17336124774116"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1696717225322"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p0967182223217"><a name="p0967182223217"></a><a name="p0967182223217"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p696792212325"><a name="p696792212325"></a><a name="p696792212325"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14245131318182"><a name="p14245131318182"></a><a name="p14245131318182"></a><strong id="b355625134213"><a name="b355625134213"></a><a name="b355625134213"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1996872263210"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p69741557465"><a name="p69741557465"></a><a name="p69741557465"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8517194124214"><a name="p8517194124214"></a><a name="p8517194124214"></a>0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10968122213324"><a name="p10968122213324"></a><a name="p10968122213324"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p192421153204211"><a name="p192421153204211"></a><a name="p192421153204211"></a>0.0</p>
</td>
</tr>
<tr id="row3968102219325"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p20968322143211"><a name="p20968322143211"></a><a name="p20968322143211"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p0968142212324"><a name="p0968142212324"></a><a name="p0968142212324"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p92421413181819"><a name="p92421413181819"></a><a name="p92421413181819"></a>-0.0</p>
</td>
</tr>
<tr id="row1719831618431"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p7198181664316"><a name="p7198181664316"></a><a name="p7198181664316"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p119881694319"><a name="p119881694319"></a><a name="p119881694319"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p0198151617439"><a name="p0198151617439"></a><a name="p0198151617439"></a><strong id="b6500173604315"><a name="b6500173604315"></a><a name="b6500173604315"></a>0.0</strong></p>
</td>
</tr>
<tr id="row14187101814435"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p16187101884315"><a name="p16187101884315"></a><a name="p16187101884315"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1187171813433"><a name="p1187171813433"></a><a name="p1187171813433"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p29291046104311"><a name="p29291046104311"></a><a name="p29291046104311"></a><strong id="b129299469438"><a name="b129299469438"></a><a name="b129299469438"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row196862211320"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p10280119194711"><a name="p10280119194711"></a><a name="p10280119194711"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1584617120447"><a name="p1584617120447"></a><a name="p1584617120447"></a>-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p433733816447"><a name="p433733816447"></a><a name="p433733816447"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p879773874420"><a name="p879773874420"></a><a name="p879773874420"></a>-0.0</p>
</td>
</tr>
<tr id="row1961665610434"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15616145611439"><a name="p15616145611439"></a><a name="p15616145611439"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1761635694318"><a name="p1761635694318"></a><a name="p1761635694318"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p136162056134312"><a name="p136162056134312"></a><a name="p136162056134312"></a>0.0</p>
</td>
</tr>
<tr id="row9259105844319"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1725965816433"><a name="p1725965816433"></a><a name="p1725965816433"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18259125811435"><a name="p18259125811435"></a><a name="p18259125811435"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p44683984512"><a name="p44683984512"></a><a name="p44683984512"></a><strong id="b129191011104519"><a name="b129191011104519"></a><a name="b129191011104519"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1996911221328"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1796902220327"><a name="p1796902220327"></a><a name="p1796902220327"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5750172934516"><a name="p5750172934516"></a><a name="p5750172934516"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p79694221325"><a name="p79694221325"></a><a name="p79694221325"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p11239111361814"><a name="p11239111361814"></a><a name="p11239111361814"></a><strong id="b1814611463454"><a name="b1814611463454"></a><a name="b1814611463454"></a>0.0</strong></p>
</td>
</tr>
<tr id="row5969142216328"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p253574814712"><a name="p253574814712"></a><a name="p253574814712"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14969162210326"><a name="p14969162210326"></a><a name="p14969162210326"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p29691722123216"><a name="p29691722123216"></a><a name="p29691722123216"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p7516111694711"><a name="p7516111694711"></a><a name="p7516111694711"></a><strong id="b451661694710"><a name="b451661694710"></a><a name="b451661694710"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1613517366460"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p19135193604611"><a name="p19135193604611"></a><a name="p19135193604611"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11359361463"><a name="p11359361463"></a><a name="p11359361463"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p64113163478"><a name="p64113163478"></a><a name="p64113163478"></a><strong id="b2041161674715"><a name="b2041161674715"></a><a name="b2041161674715"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row8970102213321"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4970102218329"><a name="p4970102218329"></a><a name="p4970102218329"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14372122944719"><a name="p14372122944719"></a><a name="p14372122944719"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p182364135186"><a name="p182364135186"></a><a name="p182364135186"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p2541344174717"><a name="p2541344174717"></a><a name="p2541344174717"></a><strong id="b1754118446476"><a name="b1754118446476"></a><a name="b1754118446476"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1056334713620"><td class="cellrowborder" rowspan="19" valign="top" width="16.42%" headers="mcps1.2.6.1.1 "><p id="p1619083319720"><a name="p1619083319720"></a><a name="p1619083319720"></a>float</p>
</td>
<td class="cellrowborder" rowspan="3" valign="top" width="13.22%" headers="mcps1.2.6.1.2 "><p id="p143999301874"><a name="p143999301874"></a><a name="p143999301874"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.36%" headers="mcps1.2.6.1.3 "><p id="p1939913301670"><a name="p1939913301670"></a><a name="p1939913301670"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p1399203018712"><a name="p1399203018712"></a><a name="p1399203018712"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p13996301578"><a name="p13996301578"></a><a name="p13996301578"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row176778491166"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1239915301073"><a name="p1239915301073"></a><a name="p1239915301073"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12399133014712"><a name="p12399133014712"></a><a name="p12399133014712"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p133998309716"><a name="p133998309716"></a><a name="p133998309716"></a>nan</p>
</td>
</tr>
<tr id="row1063455119618"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p63996301475"><a name="p63996301475"></a><a name="p63996301475"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p19399133017717"><a name="p19399133017717"></a><a name="p19399133017717"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2039913013718"><a name="p2039913013718"></a><a name="p2039913013718"></a>-inf</p>
</td>
</tr>
<tr id="row1083715531367"><td class="cellrowborder" rowspan="5" valign="top" headers="mcps1.2.6.1.1 "><p id="p14399163018711"><a name="p14399163018711"></a><a name="p14399163018711"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14001530373"><a name="p14001530373"></a><a name="p14001530373"></a>3.4028235e+38（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15400133013712"><a name="p15400133013712"></a><a name="p15400133013712"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p540013019717"><a name="p540013019717"></a><a name="p540013019717"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row92055116717"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15400930279"><a name="p15400930279"></a><a name="p15400930279"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p104001430975"><a name="p104001430975"></a><a name="p104001430975"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p44001307713"><a name="p44001307713"></a><a name="p44001307713"></a>0.0</p>
</td>
</tr>
<tr id="row62056119715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1040083015713"><a name="p1040083015713"></a><a name="p1040083015713"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p24005301073"><a name="p24005301073"></a><a name="p24005301073"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1340014309713"><a name="p1340014309713"></a><a name="p1340014309713"></a>-0.0</p>
</td>
</tr>
<tr id="row10205612716"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1940012308715"><a name="p1940012308715"></a><a name="p1940012308715"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1540013301773"><a name="p1540013301773"></a><a name="p1540013301773"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p340063015710"><a name="p340063015710"></a><a name="p340063015710"></a>nan</p>
</td>
</tr>
<tr id="row92052118713"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p16400123013719"><a name="p16400123013719"></a><a name="p16400123013719"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6400430677"><a name="p6400430677"></a><a name="p6400430677"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p04009301974"><a name="p04009301974"></a><a name="p04009301974"></a>-inf</p>
</td>
</tr>
<tr id="row1726513715717"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p54001030171"><a name="p54001030171"></a><a name="p54001030171"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1340017301716"><a name="p1340017301716"></a><a name="p1340017301716"></a>0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9400153013718"><a name="p9400153013718"></a><a name="p9400153013718"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p6400103010711"><a name="p6400103010711"></a><a name="p6400103010711"></a>0.0</p>
</td>
</tr>
<tr id="row1526511710715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p3400173012716"><a name="p3400173012716"></a><a name="p3400173012716"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p84008303719"><a name="p84008303719"></a><a name="p84008303719"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p740093014715"><a name="p740093014715"></a><a name="p740093014715"></a>-0.0</p>
</td>
</tr>
<tr id="row12661971715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p144003301377"><a name="p144003301377"></a><a name="p144003301377"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14006302078"><a name="p14006302078"></a><a name="p14006302078"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p64001930378"><a name="p64001930378"></a><a name="p64001930378"></a>nan</p>
</td>
</tr>
<tr id="row026615712718"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18401230676"><a name="p18401230676"></a><a name="p18401230676"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p17401103015712"><a name="p17401103015712"></a><a name="p17401103015712"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1440113301573"><a name="p1440113301573"></a><a name="p1440113301573"></a>-inf</p>
</td>
</tr>
<tr id="row92664713710"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p840115304711"><a name="p840115304711"></a><a name="p840115304711"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p44012302079"><a name="p44012302079"></a><a name="p44012302079"></a>-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9401330473"><a name="p9401330473"></a><a name="p9401330473"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1640116300716"><a name="p1640116300716"></a><a name="p1640116300716"></a>-0.0</p>
</td>
</tr>
<tr id="row32661577715"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p240112301776"><a name="p240112301776"></a><a name="p240112301776"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p540111301079"><a name="p540111301079"></a><a name="p540111301079"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10401230772"><a name="p10401230772"></a><a name="p10401230772"></a>nan</p>
</td>
</tr>
<tr id="row82661074719"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p194015300711"><a name="p194015300711"></a><a name="p194015300711"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1240113301172"><a name="p1240113301172"></a><a name="p1240113301172"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p174013306720"><a name="p174013306720"></a><a name="p174013306720"></a>-inf</p>
</td>
</tr>
<tr id="row162671674711"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p140153017717"><a name="p140153017717"></a><a name="p140153017717"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1140111304716"><a name="p1140111304716"></a><a name="p1140111304716"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10401530272"><a name="p10401530272"></a><a name="p10401530272"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p940163020715"><a name="p940163020715"></a><a name="p940163020715"></a>nan</p>
</td>
</tr>
<tr id="row19597112018714"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p2401123018713"><a name="p2401123018713"></a><a name="p2401123018713"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1540114306717"><a name="p1540114306717"></a><a name="p1540114306717"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1740117303718"><a name="p1740117303718"></a><a name="p1740117303718"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p54019309716"><a name="p54019309716"></a><a name="p54019309716"></a>inf</p>
</td>
</tr>
<tr id="row75981720671"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p040117301875"><a name="p040117301875"></a><a name="p040117301875"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p154014301877"><a name="p154014301877"></a><a name="p154014301877"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p114014302716"><a name="p114014302716"></a><a name="p114014302716"></a>-inf</p>
</td>
</tr>
<tr id="row1598220979"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1140210307718"><a name="p1140210307718"></a><a name="p1140210307718"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p140233017719"><a name="p140233017719"></a><a name="p140233017719"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16402183014716"><a name="p16402183014716"></a><a name="p16402183014716"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p204021230974"><a name="p204021230974"></a><a name="p204021230974"></a>-inf</p>
</td>
</tr>
<tr id="row18970122212320"><td class="cellrowborder" rowspan="2" valign="top" width="16.42%" headers="mcps1.2.6.1.1 "><p id="p697052211325"><a name="p697052211325"></a><a name="p697052211325"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="13.22%" headers="mcps1.2.6.1.2 "><p id="p1197022223213"><a name="p1197022223213"></a><a name="p1197022223213"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.36%" headers="mcps1.2.6.1.3 "><p id="p8970102211324"><a name="p8970102211324"></a><a name="p8970102211324"></a>-32768（MIN）/32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p14970102214327"><a name="p14970102214327"></a><a name="p14970102214327"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p697010229327"><a name="p697010229327"></a><a name="p697010229327"></a>-32768</p>
</td>
</tr>
<tr id="row860820566487"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1145410560508"><a name="p1145410560508"></a><a name="p1145410560508"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14609656184819"><a name="p14609656184819"></a><a name="p14609656184819"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18609175616483"><a name="p18609175616483"></a><a name="p18609175616483"></a>32767</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p118491319194913"><a name="p118491319194913"></a><a name="p118491319194913"></a>32767</p>
</td>
</tr>
<tr id="row1697014227324"><td class="cellrowborder" rowspan="2" valign="top" width="16.42%" headers="mcps1.2.6.1.1 "><p id="p197052213217"><a name="p197052213217"></a><a name="p197052213217"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="13.22%" headers="mcps1.2.6.1.2 "><p id="p159711229327"><a name="p159711229327"></a><a name="p159711229327"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.36%" headers="mcps1.2.6.1.3 "><p id="p189711722123210"><a name="p189711722123210"></a><a name="p189711722123210"></a>-2147483648（MIN）/2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p16232181312181"><a name="p16232181312181"></a><a name="p16232181312181"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p323111315185"><a name="p323111315185"></a><a name="p323111315185"></a>-2147483648</p>
</td>
</tr>
<tr id="row16971162263219"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1047334275112"><a name="p1047334275112"></a><a name="p1047334275112"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p0971132253214"><a name="p0971132253214"></a><a name="p0971132253214"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18269155105119"><a name="p18269155105119"></a><a name="p18269155105119"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p19380155815116"><a name="p19380155815116"></a><a name="p19380155815116"></a>2147483647</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 15**  Adds特殊值/边界值输入的计算结果

<a name="table15619175083314"></a>
<table><thead align="left"><tr id="row1619145018331"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.1"><p id="p1561955012331"><a name="p1561955012331"></a><a name="p1561955012331"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.2"><p id="p1061919502335"><a name="p1061919502335"></a><a name="p1061919502335"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.3"><p id="p1398143481818"><a name="p1398143481818"></a><a name="p1398143481818"></a>scalarValue取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p20619195019336"><a name="p20619195019336"></a><a name="p20619195019336"></a>dst的元素取值</p>
<p id="p1561915017333"><a name="p1561915017333"></a><a name="p1561915017333"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p136195502339"><a name="p136195502339"></a><a name="p136195502339"></a>dst的元素取值</p>
<p id="p36192505333"><a name="p36192505333"></a><a name="p36192505333"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row26196506339"><td class="cellrowborder" rowspan="21" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p06191250123317"><a name="p06191250123317"></a><a name="p06191250123317"></a>half</p>
</td>
<td class="cellrowborder" rowspan="5" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1961935013315"><a name="p1961935013315"></a><a name="p1961935013315"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p13619105053315"><a name="p13619105053315"></a><a name="p13619105053315"></a>-65504.0（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p146190504332"><a name="p146190504332"></a><a name="p146190504332"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p66193502337"><a name="p66193502337"></a><a name="p66193502337"></a><strong id="b5619450133317"><a name="b5619450133317"></a><a name="b5619450133317"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row146197505333"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1362025017337"><a name="p1362025017337"></a><a name="p1362025017337"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1362012505332"><a name="p1362012505332"></a><a name="p1362012505332"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p86201250153311"><a name="p86201250153311"></a><a name="p86201250153311"></a>0.0</p>
</td>
</tr>
<tr id="row1620750123312"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p166201650153316"><a name="p166201650153316"></a><a name="p166201650153316"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1562085093311"><a name="p1562085093311"></a><a name="p1562085093311"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19620125013318"><a name="p19620125013318"></a><a name="p19620125013318"></a>-65504.0</p>
</td>
</tr>
<tr id="row1862005018335"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p86201350103316"><a name="p86201350103316"></a><a name="p86201350103316"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10620135033311"><a name="p10620135033311"></a><a name="p10620135033311"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p162065093317"><a name="p162065093317"></a><a name="p162065093317"></a><strong id="b1462015507339"><a name="b1462015507339"></a><a name="b1462015507339"></a>0.0</strong></p>
</td>
</tr>
<tr id="row162095011333"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p862095016339"><a name="p862095016339"></a><a name="p862095016339"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p7620185023310"><a name="p7620185023310"></a><a name="p7620185023310"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1462017503333"><a name="p1462017503333"></a><a name="p1462017503333"></a><strong id="b16620750113314"><a name="b16620750113314"></a><a name="b16620750113314"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row13620750173311"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p262014506337"><a name="p262014506337"></a><a name="p262014506337"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p462019503333"><a name="p462019503333"></a><a name="p462019503333"></a>65504.0（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10620250173313"><a name="p10620250173313"></a><a name="p10620250173313"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p14620135016333"><a name="p14620135016333"></a><a name="p14620135016333"></a><strong id="b166201450133312"><a name="b166201450133312"></a><a name="b166201450133312"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row196201750103318"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p962011500338"><a name="p962011500338"></a><a name="p962011500338"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1562045010338"><a name="p1562045010338"></a><a name="p1562045010338"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2062085012334"><a name="p2062085012334"></a><a name="p2062085012334"></a>65504.0</p>
</td>
</tr>
<tr id="row1262065083312"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1262125003310"><a name="p1262125003310"></a><a name="p1262125003310"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5621450103318"><a name="p5621450103318"></a><a name="p5621450103318"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p762112507334"><a name="p762112507334"></a><a name="p762112507334"></a><strong id="b196217509332"><a name="b196217509332"></a><a name="b196217509332"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1562165083317"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p0621175017337"><a name="p0621175017337"></a><a name="p0621175017337"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p362165063318"><a name="p362165063318"></a><a name="p362165063318"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1762135011336"><a name="p1762135011336"></a><a name="p1762135011336"></a><strong id="b862105013334"><a name="b862105013334"></a><a name="b862105013334"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row136211950183312"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p162165083314"><a name="p162165083314"></a><a name="p162165083314"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16621650183319"><a name="p16621650183319"></a><a name="p16621650183319"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4621850173311"><a name="p4621850173311"></a><a name="p4621850173311"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1362195063312"><a name="p1362195063312"></a><a name="p1362195063312"></a>0.0</p>
</td>
</tr>
<tr id="row8621135003310"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p362118500334"><a name="p362118500334"></a><a name="p362118500334"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p186219506338"><a name="p186219506338"></a><a name="p186219506338"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p962116508338"><a name="p962116508338"></a><a name="p962116508338"></a><strong id="b2621105019332"><a name="b2621105019332"></a><a name="b2621105019332"></a>0.0</strong></p>
</td>
</tr>
<tr id="row6621135012337"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p362125019331"><a name="p362125019331"></a><a name="p362125019331"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p126211050133319"><a name="p126211050133319"></a><a name="p126211050133319"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14621105014330"><a name="p14621105014330"></a><a name="p14621105014330"></a><strong id="b18621155014335"><a name="b18621155014335"></a><a name="b18621155014335"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row062135012336"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p162155003315"><a name="p162155003315"></a><a name="p162155003315"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16621155003318"><a name="p16621155003318"></a><a name="p16621155003318"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p762195014336"><a name="p762195014336"></a><a name="p762195014336"></a><strong id="b106212050153317"><a name="b106212050153317"></a><a name="b106212050153317"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row16224507334"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p17622950103317"><a name="p17622950103317"></a><a name="p17622950103317"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2062217503333"><a name="p2062217503333"></a><a name="p2062217503333"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p106221950153315"><a name="p106221950153315"></a><a name="p106221950153315"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p5622950173317"><a name="p5622950173317"></a><a name="p5622950173317"></a>-0.0</p>
</td>
</tr>
<tr id="row13622250143310"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8622195015334"><a name="p8622195015334"></a><a name="p8622195015334"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1962235012338"><a name="p1962235012338"></a><a name="p1962235012338"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10622125053318"><a name="p10622125053318"></a><a name="p10622125053318"></a><strong id="b1962275073316"><a name="b1962275073316"></a><a name="b1962275073316"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1862225010333"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p86223501331"><a name="p86223501331"></a><a name="p86223501331"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1762211500337"><a name="p1762211500337"></a><a name="p1762211500337"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1962265093314"><a name="p1962265093314"></a><a name="p1962265093314"></a><strong id="b11622150203319"><a name="b11622150203319"></a><a name="b11622150203319"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row10622750113310"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p126229501337"><a name="p126229501337"></a><a name="p126229501337"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p156225509339"><a name="p156225509339"></a><a name="p156225509339"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p562285013334"><a name="p562285013334"></a><a name="p562285013334"></a><strong id="b8622105073312"><a name="b8622105073312"></a><a name="b8622105073312"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row176221050103319"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18623205011334"><a name="p18623205011334"></a><a name="p18623205011334"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p362316505331"><a name="p362316505331"></a><a name="p362316505331"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20623145010337"><a name="p20623145010337"></a><a name="p20623145010337"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p6623650193314"><a name="p6623650193314"></a><a name="p6623650193314"></a><strong id="b1362325010334"><a name="b1362325010334"></a><a name="b1362325010334"></a>0.0</strong></p>
</td>
</tr>
<tr id="row7623125016337"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p1362345010333"><a name="p1362345010333"></a><a name="p1362345010333"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p26231850193313"><a name="p26231850193313"></a><a name="p26231850193313"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1762335013334"><a name="p1762335013334"></a><a name="p1762335013334"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p8623105023315"><a name="p8623105023315"></a><a name="p8623105023315"></a><strong id="b11623850103312"><a name="b11623850103312"></a><a name="b11623850103312"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row162355012331"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1623165083311"><a name="p1623165083311"></a><a name="p1623165083311"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p17623155011336"><a name="p17623155011336"></a><a name="p17623155011336"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p12623250123310"><a name="p12623250123310"></a><a name="p12623250123310"></a><strong id="b36232050123314"><a name="b36232050123314"></a><a name="b36232050123314"></a>0.0</strong></p>
</td>
</tr>
<tr id="row2062355013331"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12623150113318"><a name="p12623150113318"></a><a name="p12623150113318"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18623165013312"><a name="p18623165013312"></a><a name="p18623165013312"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p176231850123319"><a name="p176231850123319"></a><a name="p176231850123319"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p10623195093312"><a name="p10623195093312"></a><a name="p10623195093312"></a><strong id="b86236504330"><a name="b86236504330"></a><a name="b86236504330"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1631752413204"><td class="cellrowborder" rowspan="21" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p750212116212"><a name="p750212116212"></a><a name="p750212116212"></a>float</p>
</td>
<td class="cellrowborder" rowspan="5" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p131181858142011"><a name="p131181858142011"></a><a name="p131181858142011"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p11187581201"><a name="p11187581201"></a><a name="p11187581201"></a>-3.4028235e+38（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p1911810581204"><a name="p1911810581204"></a><a name="p1911810581204"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p01182585209"><a name="p01182585209"></a><a name="p01182585209"></a>-inf</p>
</td>
</tr>
<tr id="row18453526112018"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1311820587207"><a name="p1311820587207"></a><a name="p1311820587207"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13118158182016"><a name="p13118158182016"></a><a name="p13118158182016"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p141183587202"><a name="p141183587202"></a><a name="p141183587202"></a>0.0</p>
</td>
</tr>
<tr id="row196191631162011"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18118125818207"><a name="p18118125818207"></a><a name="p18118125818207"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1611815584200"><a name="p1611815584200"></a><a name="p1611815584200"></a>-3.40282E+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1011845810207"><a name="p1011845810207"></a><a name="p1011845810207"></a>-3.40282E+38</p>
</td>
</tr>
<tr id="row961913102013"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14118155822014"><a name="p14118155822014"></a><a name="p14118155822014"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16118135814209"><a name="p16118135814209"></a><a name="p16118135814209"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p101181458122013"><a name="p101181458122013"></a><a name="p101181458122013"></a>nan</p>
</td>
</tr>
<tr id="row72851835112013"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p101181958192010"><a name="p101181958192010"></a><a name="p101181958192010"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12118195819207"><a name="p12118195819207"></a><a name="p12118195819207"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1118758122019"><a name="p1118758122019"></a><a name="p1118758122019"></a>inf</p>
</td>
</tr>
<tr id="row3285123502012"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p1511805862019"><a name="p1511805862019"></a><a name="p1511805862019"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15118165802014"><a name="p15118165802014"></a><a name="p15118165802014"></a>3.4028235e+38（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p121181958152019"><a name="p121181958152019"></a><a name="p121181958152019"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p311819589204"><a name="p311819589204"></a><a name="p311819589204"></a>inf</p>
</td>
</tr>
<tr id="row122855350207"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1118195852010"><a name="p1118195852010"></a><a name="p1118195852010"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p111885810206"><a name="p111885810206"></a><a name="p111885810206"></a>3.40282E+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p8118155816204"><a name="p8118155816204"></a><a name="p8118155816204"></a>3.40282E+38</p>
</td>
</tr>
<tr id="row192861935112020"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p3118115818204"><a name="p3118115818204"></a><a name="p3118115818204"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12118125812013"><a name="p12118125812013"></a><a name="p12118125812013"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1911815892012"><a name="p1911815892012"></a><a name="p1911815892012"></a>nan</p>
</td>
</tr>
<tr id="row1210154012202"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p711914583208"><a name="p711914583208"></a><a name="p711914583208"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p011935819208"><a name="p011935819208"></a><a name="p011935819208"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1611925819208"><a name="p1611925819208"></a><a name="p1611925819208"></a>-inf</p>
</td>
</tr>
<tr id="row17210440102010"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p131192585208"><a name="p131192585208"></a><a name="p131192585208"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9119658192019"><a name="p9119658192019"></a><a name="p9119658192019"></a>0.0/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p31191758152019"><a name="p31191758152019"></a><a name="p31191758152019"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p0119358152015"><a name="p0119358152015"></a><a name="p0119358152015"></a>0.0</p>
</td>
</tr>
<tr id="row521116408209"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p7119358152013"><a name="p7119358152013"></a><a name="p7119358152013"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p4119185817201"><a name="p4119185817201"></a><a name="p4119185817201"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p111191958122016"><a name="p111191958122016"></a><a name="p111191958122016"></a>nan</p>
</td>
</tr>
<tr id="row0211184010204"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1611945817209"><a name="p1611945817209"></a><a name="p1611945817209"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16119195810200"><a name="p16119195810200"></a><a name="p16119195810200"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15119195814203"><a name="p15119195814203"></a><a name="p15119195814203"></a>inf</p>
</td>
</tr>
<tr id="row3211194082013"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p611913587208"><a name="p611913587208"></a><a name="p611913587208"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18119205842013"><a name="p18119205842013"></a><a name="p18119205842013"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18119165811201"><a name="p18119165811201"></a><a name="p18119165811201"></a>-inf</p>
</td>
</tr>
<tr id="row0211340122012"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p121191858142012"><a name="p121191858142012"></a><a name="p121191858142012"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p4119145802015"><a name="p4119145802015"></a><a name="p4119145802015"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p211925817202"><a name="p211925817202"></a><a name="p211925817202"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p211910582204"><a name="p211910582204"></a><a name="p211910582204"></a>-0.0</p>
</td>
</tr>
<tr id="row7211540122015"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1211985818209"><a name="p1211985818209"></a><a name="p1211985818209"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11194589209"><a name="p11194589209"></a><a name="p11194589209"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p111918584204"><a name="p111918584204"></a><a name="p111918584204"></a>nan</p>
</td>
</tr>
<tr id="row19212174011202"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p811935814206"><a name="p811935814206"></a><a name="p811935814206"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p91191358162016"><a name="p91191358162016"></a><a name="p91191358162016"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p51196587206"><a name="p51196587206"></a><a name="p51196587206"></a>inf</p>
</td>
</tr>
<tr id="row1467910478206"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12120135842014"><a name="p12120135842014"></a><a name="p12120135842014"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5120958132012"><a name="p5120958132012"></a><a name="p5120958132012"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4120165817203"><a name="p4120165817203"></a><a name="p4120165817203"></a>-inf</p>
</td>
</tr>
<tr id="row11679047112016"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1012015589203"><a name="p1012015589203"></a><a name="p1012015589203"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1812025815201"><a name="p1812025815201"></a><a name="p1812025815201"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p121207583201"><a name="p121207583201"></a><a name="p121207583201"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p0120155892020"><a name="p0120155892020"></a><a name="p0120155892020"></a>nan</p>
</td>
</tr>
<tr id="row86798479206"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p141203581207"><a name="p141203581207"></a><a name="p141203581207"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1812017584208"><a name="p1812017584208"></a><a name="p1812017584208"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p41201758192012"><a name="p41201758192012"></a><a name="p41201758192012"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p16120758182015"><a name="p16120758182015"></a><a name="p16120758182015"></a>inf</p>
</td>
</tr>
<tr id="row136801347172018"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p212018583205"><a name="p212018583205"></a><a name="p212018583205"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1312025814202"><a name="p1312025814202"></a><a name="p1312025814202"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9120145892012"><a name="p9120145892012"></a><a name="p9120145892012"></a>nan</p>
</td>
</tr>
<tr id="row168024710200"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1612014586201"><a name="p1612014586201"></a><a name="p1612014586201"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18120158192012"><a name="p18120158192012"></a><a name="p18120158192012"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11203589200"><a name="p11203589200"></a><a name="p11203589200"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p0121185820204"><a name="p0121185820204"></a><a name="p0121185820204"></a>-inf</p>
</td>
</tr>
<tr id="row9902126112416"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p1623175018331"><a name="p1623175018331"></a><a name="p1623175018331"></a>int16_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1462313502339"><a name="p1462313502339"></a><a name="p1462313502339"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p56231350153314"><a name="p56231350153314"></a><a name="p56231350153314"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p16231950143317"><a name="p16231950143317"></a><a name="p16231950143317"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1762455073319"><a name="p1762455073319"></a><a name="p1762455073319"></a><strong id="b14624150153316"><a name="b14624150153316"></a><a name="b14624150153316"></a>-32768</strong></p>
</td>
</tr>
<tr id="row149027619247"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p136240502333"><a name="p136240502333"></a><a name="p136240502333"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p062445033313"><a name="p062445033313"></a><a name="p062445033313"></a>-1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1862425013338"><a name="p1862425013338"></a><a name="p1862425013338"></a>-1</p>
</td>
</tr>
<tr id="row190113682419"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1862455010338"><a name="p1862455010338"></a><a name="p1862455010338"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1162415020337"><a name="p1162415020337"></a><a name="p1162415020337"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p6624155020336"><a name="p6624155020336"></a><a name="p6624155020336"></a>-2</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1762495014335"><a name="p1762495014335"></a><a name="p1762495014335"></a><strong id="b206244501330"><a name="b206244501330"></a><a name="b206244501330"></a>32767</strong></p>
</td>
</tr>
<tr id="row79011965248"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p2624105015334"><a name="p2624105015334"></a><a name="p2624105015334"></a>int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1762465063310"><a name="p1762465063310"></a><a name="p1762465063310"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p162418501334"><a name="p162418501334"></a><a name="p162418501334"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p46248505334"><a name="p46248505334"></a><a name="p46248505334"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p2624185003314"><a name="p2624185003314"></a><a name="p2624185003314"></a><strong id="b20624550123311"><a name="b20624550123311"></a><a name="b20624550123311"></a>-2147483648</strong></p>
</td>
</tr>
<tr id="row6901106172417"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11624105013318"><a name="p11624105013318"></a><a name="p11624105013318"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2624135053312"><a name="p2624135053312"></a><a name="p2624135053312"></a>-1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p12624150173314"><a name="p12624150173314"></a><a name="p12624150173314"></a>-1</p>
</td>
</tr>
<tr id="row1790196112416"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1262435018337"><a name="p1262435018337"></a><a name="p1262435018337"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2624175016330"><a name="p2624175016330"></a><a name="p2624175016330"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1062512504339"><a name="p1062512504339"></a><a name="p1062512504339"></a>-2</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p16251950123313"><a name="p16251950123313"></a><a name="p16251950123313"></a><strong id="b26251450143313"><a name="b26251450143313"></a><a name="b26251450143313"></a>2147483648</strong></p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 16**  Muls特殊值/边界值输入的计算结果

<a name="table1510462203414"></a>
<table><thead align="left"><tr id="row3104142263417"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.1"><p id="p510462210343"><a name="p510462210343"></a><a name="p510462210343"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.2"><p id="p1104522153412"><a name="p1104522153412"></a><a name="p1104522153412"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.3"><p id="p14104722103414"><a name="p14104722103414"></a><a name="p14104722103414"></a>scalarValue取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p91042224345"><a name="p91042224345"></a><a name="p91042224345"></a>dst的元素取值</p>
<p id="p2104152223415"><a name="p2104152223415"></a><a name="p2104152223415"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p51041922113416"><a name="p51041922113416"></a><a name="p51041922113416"></a>dst的元素取值</p>
<p id="p1510417222341"><a name="p1510417222341"></a><a name="p1510417222341"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row4104192215342"><td class="cellrowborder" rowspan="22" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p1310402219341"><a name="p1310402219341"></a><a name="p1310402219341"></a>half</p>
</td>
<td class="cellrowborder" rowspan="7" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p11041122153413"><a name="p11041122153413"></a><a name="p11041122153413"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p1510411223345"><a name="p1510411223345"></a><a name="p1510411223345"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p1910420225342"><a name="p1910420225342"></a><a name="p1910420225342"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p131043229342"><a name="p131043229342"></a><a name="p131043229342"></a><strong id="b1310472263415"><a name="b1310472263415"></a><a name="b1310472263415"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1210472212342"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11105022123417"><a name="p11105022123417"></a><a name="p11105022123417"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p110542217348"><a name="p110542217348"></a><a name="p110542217348"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p810522263410"><a name="p810522263410"></a><a name="p810522263410"></a><strong id="b810515228344"><a name="b810515228344"></a><a name="b810515228344"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row131051422123417"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p131051322173416"><a name="p131051322173416"></a><a name="p131051322173416"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2105182217344"><a name="p2105182217344"></a><a name="p2105182217344"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1310582214347"><a name="p1310582214347"></a><a name="p1310582214347"></a>-0.0</p>
</td>
</tr>
<tr id="row191055221347"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8105122219347"><a name="p8105122219347"></a><a name="p8105122219347"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6105122203417"><a name="p6105122203417"></a><a name="p6105122203417"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p5105162203413"><a name="p5105162203413"></a><a name="p5105162203413"></a>0.0</p>
</td>
</tr>
<tr id="row010502210342"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1710542213413"><a name="p1710542213413"></a><a name="p1710542213413"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14105142218342"><a name="p14105142218342"></a><a name="p14105142218342"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p81051222173419"><a name="p81051222173419"></a><a name="p81051222173419"></a><strong id="b71051122173416"><a name="b71051122173416"></a><a name="b71051122173416"></a>0.0</strong></p>
</td>
</tr>
<tr id="row51051226348"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p6105422103414"><a name="p6105422103414"></a><a name="p6105422103414"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8105162217343"><a name="p8105162217343"></a><a name="p8105162217343"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p5105122173416"><a name="p5105122173416"></a><a name="p5105122173416"></a><strong id="b1610512213418"><a name="b1610512213418"></a><a name="b1610512213418"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row2010515226345"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p810582243417"><a name="p810582243417"></a><a name="p810582243417"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p810692217344"><a name="p810692217344"></a><a name="p810692217344"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1810682283413"><a name="p1810682283413"></a><a name="p1810682283413"></a><strong id="b191061022173413"><a name="b191061022173413"></a><a name="b191061022173413"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row9106192263412"><td class="cellrowborder" rowspan="6" valign="top" headers="mcps1.2.6.1.1 "><p id="p9106112223413"><a name="p9106112223413"></a><a name="p9106112223413"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11106222163413"><a name="p11106222163413"></a><a name="p11106222163413"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p310622293420"><a name="p310622293420"></a><a name="p310622293420"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p910616229345"><a name="p910616229345"></a><a name="p910616229345"></a><strong id="b210662213410"><a name="b210662213410"></a><a name="b210662213410"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row01061822133417"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1710682215341"><a name="p1710682215341"></a><a name="p1710682215341"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p310620224349"><a name="p310620224349"></a><a name="p310620224349"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18106192273419"><a name="p18106192273419"></a><a name="p18106192273419"></a>0.0</p>
</td>
</tr>
<tr id="row12106162217346"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p71065226344"><a name="p71065226344"></a><a name="p71065226344"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p61069223341"><a name="p61069223341"></a><a name="p61069223341"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11106222153413"><a name="p11106222153413"></a><a name="p11106222153413"></a>-0.0</p>
</td>
</tr>
<tr id="row810612212344"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1106182283416"><a name="p1106182283416"></a><a name="p1106182283416"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18106142210343"><a name="p18106142210343"></a><a name="p18106142210343"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11106182210340"><a name="p11106182210340"></a><a name="p11106182210340"></a><strong id="b610619223341"><a name="b610619223341"></a><a name="b610619223341"></a>0.0</strong></p>
</td>
</tr>
<tr id="row610672243416"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p101061022143412"><a name="p101061022143412"></a><a name="p101061022143412"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1410672293411"><a name="p1410672293411"></a><a name="p1410672293411"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p010722217346"><a name="p010722217346"></a><a name="p010722217346"></a><strong id="b2010752213419"><a name="b2010752213419"></a><a name="b2010752213419"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row18107192273419"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1810717225349"><a name="p1810717225349"></a><a name="p1810717225349"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p91071322173418"><a name="p91071322173418"></a><a name="p91071322173418"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1310762218349"><a name="p1310762218349"></a><a name="p1310762218349"></a><strong id="b141071522163419"><a name="b141071522163419"></a><a name="b141071522163419"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row0107022163419"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p010722293417"><a name="p010722293417"></a><a name="p010722293417"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16107722173413"><a name="p16107722173413"></a><a name="p16107722173413"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1710742215343"><a name="p1710742215343"></a><a name="p1710742215343"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p131071522193414"><a name="p131071522193414"></a><a name="p131071522193414"></a>0.0</p>
</td>
</tr>
<tr id="row810772263420"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9107142214346"><a name="p9107142214346"></a><a name="p9107142214346"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1107222153418"><a name="p1107222153418"></a><a name="p1107222153418"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p910762217342"><a name="p910762217342"></a><a name="p910762217342"></a>-0.0</p>
</td>
</tr>
<tr id="row1210712216348"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p161071922183410"><a name="p161071922183410"></a><a name="p161071922183410"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6107182273415"><a name="p6107182273415"></a><a name="p6107182273415"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p71071122163418"><a name="p71071122163418"></a><a name="p71071122163418"></a><strong id="b161071022153411"><a name="b161071022153411"></a><a name="b161071022153411"></a>0.0</strong></p>
</td>
</tr>
<tr id="row10107122216344"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p191077220348"><a name="p191077220348"></a><a name="p191077220348"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p191071322183410"><a name="p191071322183410"></a><a name="p191071322183410"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p3107132214348"><a name="p3107132214348"></a><a name="p3107132214348"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1810802217343"><a name="p1810802217343"></a><a name="p1810802217343"></a>0.0</p>
</td>
</tr>
<tr id="row1410820221343"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p101089222345"><a name="p101089222345"></a><a name="p101089222345"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p0108322193419"><a name="p0108322193419"></a><a name="p0108322193419"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18108172243413"><a name="p18108172243413"></a><a name="p18108172243413"></a><strong id="b9108722153414"><a name="b9108722153414"></a><a name="b9108722153414"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1010818224344"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p7108142283418"><a name="p7108142283418"></a><a name="p7108142283418"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1910818229346"><a name="p1910818229346"></a><a name="p1910818229346"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p17108132220344"><a name="p17108132220344"></a><a name="p17108132220344"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1610832273418"><a name="p1610832273418"></a><a name="p1610832273418"></a><strong id="b51082022203412"><a name="b51082022203412"></a><a name="b51082022203412"></a>0.0</strong></p>
</td>
</tr>
<tr id="row19108722143411"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p7108112203414"><a name="p7108112203414"></a><a name="p7108112203414"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1210882220343"><a name="p1210882220343"></a><a name="p1210882220343"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p210872273417"><a name="p210872273417"></a><a name="p210872273417"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p10108102212349"><a name="p10108102212349"></a><a name="p10108102212349"></a><strong id="b110815223345"><a name="b110815223345"></a><a name="b110815223345"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1210812210348"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1710872293413"><a name="p1710872293413"></a><a name="p1710872293413"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1910832273416"><a name="p1910832273416"></a><a name="p1910832273416"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19108102273412"><a name="p19108102273412"></a><a name="p19108102273412"></a><strong id="b1910822219348"><a name="b1910822219348"></a><a name="b1910822219348"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row18108822143410"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p710812228346"><a name="p710812228346"></a><a name="p710812228346"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p41088224342"><a name="p41088224342"></a><a name="p41088224342"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p121091722133412"><a name="p121091722133412"></a><a name="p121091722133412"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1410902215341"><a name="p1410902215341"></a><a name="p1410902215341"></a><strong id="b910972220346"><a name="b910972220346"></a><a name="b910972220346"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row2673123482220"><td class="cellrowborder" rowspan="22" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p18923131522312"><a name="p18923131522312"></a><a name="p18923131522312"></a>float</p>
</td>
<td class="cellrowborder" rowspan="7" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p22571511102314"><a name="p22571511102314"></a><a name="p22571511102314"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p1825710115235"><a name="p1825710115235"></a><a name="p1825710115235"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p142571511192319"><a name="p142571511192319"></a><a name="p142571511192319"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p10257211102312"><a name="p10257211102312"></a><a name="p10257211102312"></a>inf</p>
</td>
</tr>
<tr id="row4849143692213"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p825712118234"><a name="p825712118234"></a><a name="p825712118234"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10257611102312"><a name="p10257611102312"></a><a name="p10257611102312"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p12257311192318"><a name="p12257311192318"></a><a name="p12257311192318"></a>-inf</p>
</td>
</tr>
<tr id="row3322840172215"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p162571511152311"><a name="p162571511152311"></a><a name="p162571511152311"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p425741132319"><a name="p425741132319"></a><a name="p425741132319"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1125721114239"><a name="p1125721114239"></a><a name="p1125721114239"></a>-0.0</p>
</td>
</tr>
<tr id="row19941942132215"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p525701120239"><a name="p525701120239"></a><a name="p525701120239"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1225711114230"><a name="p1225711114230"></a><a name="p1225711114230"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1225751112237"><a name="p1225751112237"></a><a name="p1225751112237"></a>0.0</p>
</td>
</tr>
<tr id="row143913820229"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p132571411142313"><a name="p132571411142313"></a><a name="p132571411142313"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p125711112311"><a name="p125711112311"></a><a name="p125711112311"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18257131172310"><a name="p18257131172310"></a><a name="p18257131172310"></a>nan</p>
</td>
</tr>
<tr id="row15827144813223"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p13257111120239"><a name="p13257111120239"></a><a name="p13257111120239"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p182576118232"><a name="p182576118232"></a><a name="p182576118232"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p3257121132313"><a name="p3257121132313"></a><a name="p3257121132313"></a>-inf</p>
</td>
</tr>
<tr id="row178284489229"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p425714112233"><a name="p425714112233"></a><a name="p425714112233"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p42571811102310"><a name="p42571811102310"></a><a name="p42571811102310"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7257201117238"><a name="p7257201117238"></a><a name="p7257201117238"></a>inf</p>
</td>
</tr>
<tr id="row17828134892211"><td class="cellrowborder" rowspan="6" valign="top" headers="mcps1.2.6.1.1 "><p id="p142572011132310"><a name="p142572011132310"></a><a name="p142572011132310"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1257511112310"><a name="p1257511112310"></a><a name="p1257511112310"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p925821182317"><a name="p925821182317"></a><a name="p925821182317"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p112581211162312"><a name="p112581211162312"></a><a name="p112581211162312"></a>inf</p>
</td>
</tr>
<tr id="row14828134819226"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1225841152314"><a name="p1225841152314"></a><a name="p1225841152314"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p52581811182316"><a name="p52581811182316"></a><a name="p52581811182316"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9258111118233"><a name="p9258111118233"></a><a name="p9258111118233"></a>0.0</p>
</td>
</tr>
<tr id="row14828114872210"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1925891122315"><a name="p1925891122315"></a><a name="p1925891122315"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p225861114233"><a name="p225861114233"></a><a name="p225861114233"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p142584117237"><a name="p142584117237"></a><a name="p142584117237"></a>-0.0</p>
</td>
</tr>
<tr id="row71738542224"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1025811122316"><a name="p1025811122316"></a><a name="p1025811122316"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3258511202313"><a name="p3258511202313"></a><a name="p3258511202313"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9258151182318"><a name="p9258151182318"></a><a name="p9258151182318"></a>nan</p>
</td>
</tr>
<tr id="row41731954122217"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p142581911182310"><a name="p142581911182310"></a><a name="p142581911182310"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1025818111235"><a name="p1025818111235"></a><a name="p1025818111235"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14258141182319"><a name="p14258141182319"></a><a name="p14258141182319"></a>inf</p>
</td>
</tr>
<tr id="row17173145462219"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p925816111239"><a name="p925816111239"></a><a name="p925816111239"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16259511182312"><a name="p16259511182312"></a><a name="p16259511182312"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1725961112312"><a name="p1725961112312"></a><a name="p1725961112312"></a>-inf</p>
</td>
</tr>
<tr id="row161734543228"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p6259181172319"><a name="p6259181172319"></a><a name="p6259181172319"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1525921118233"><a name="p1525921118233"></a><a name="p1525921118233"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1125981118239"><a name="p1125981118239"></a><a name="p1125981118239"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p825931118238"><a name="p825931118238"></a><a name="p825931118238"></a>0.0</p>
</td>
</tr>
<tr id="row20173125482217"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p162592011152318"><a name="p162592011152318"></a><a name="p162592011152318"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p525991118232"><a name="p525991118232"></a><a name="p525991118232"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p162591111142318"><a name="p162591111142318"></a><a name="p162591111142318"></a>-0.0</p>
</td>
</tr>
<tr id="row817425412229"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p112593116231"><a name="p112593116231"></a><a name="p112593116231"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p162591311102312"><a name="p162591311102312"></a><a name="p162591311102312"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p125913112234"><a name="p125913112234"></a><a name="p125913112234"></a>nan</p>
</td>
</tr>
<tr id="row617410541226"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p525971113233"><a name="p525971113233"></a><a name="p525971113233"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1325951102320"><a name="p1325951102320"></a><a name="p1325951102320"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p3259111152315"><a name="p3259111152315"></a><a name="p3259111152315"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1725991162310"><a name="p1725991162310"></a><a name="p1725991162310"></a>0.0</p>
</td>
</tr>
<tr id="row121748548225"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12596117238"><a name="p12596117238"></a><a name="p12596117238"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p202594112236"><a name="p202594112236"></a><a name="p202594112236"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p182591711102313"><a name="p182591711102313"></a><a name="p182591711102313"></a>nan</p>
</td>
</tr>
<tr id="row61741954162216"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1525961115235"><a name="p1525961115235"></a><a name="p1525961115235"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p16259201102316"><a name="p16259201102316"></a><a name="p16259201102316"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1525981142314"><a name="p1525981142314"></a><a name="p1525981142314"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1125916112236"><a name="p1125916112236"></a><a name="p1125916112236"></a>nan</p>
</td>
</tr>
<tr id="row101753540228"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p152591611162313"><a name="p152591611162313"></a><a name="p152591611162313"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1425931115232"><a name="p1425931115232"></a><a name="p1425931115232"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10259131115232"><a name="p10259131115232"></a><a name="p10259131115232"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1025941172317"><a name="p1025941172317"></a><a name="p1025941172317"></a>inf</p>
</td>
</tr>
<tr id="row757765911224"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p826001111232"><a name="p826001111232"></a><a name="p826001111232"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1326016112232"><a name="p1326016112232"></a><a name="p1326016112232"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p526091115236"><a name="p526091115236"></a><a name="p526091115236"></a>-inf</p>
</td>
</tr>
<tr id="row757715911226"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4260171142317"><a name="p4260171142317"></a><a name="p4260171142317"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p19260131117239"><a name="p19260131117239"></a><a name="p19260131117239"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p52601311152319"><a name="p52601311152319"></a><a name="p52601311152319"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p17260111192317"><a name="p17260111192317"></a><a name="p17260111192317"></a>inf</p>
</td>
</tr>
<tr id="row1552216138277"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p131094221349"><a name="p131094221349"></a><a name="p131094221349"></a>int16_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1110916221348"><a name="p1110916221348"></a><a name="p1110916221348"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p15109132219341"><a name="p15109132219341"></a><a name="p15109132219341"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p1109122163414"><a name="p1109122163414"></a><a name="p1109122163414"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p14109102283414"><a name="p14109102283414"></a><a name="p14109102283414"></a><strong id="b0109142263417"><a name="b0109142263417"></a><a name="b0109142263417"></a>32767</strong></p>
</td>
</tr>
<tr id="row115227136271"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p151091229343"><a name="p151091229343"></a><a name="p151091229343"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12109122213417"><a name="p12109122213417"></a><a name="p12109122213417"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p510952212348"><a name="p510952212348"></a><a name="p510952212348"></a>-32768</p>
</td>
</tr>
<tr id="row17522111314271"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p141091022193419"><a name="p141091022193419"></a><a name="p141091022193419"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p510902214346"><a name="p510902214346"></a><a name="p510902214346"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10109622193413"><a name="p10109622193413"></a><a name="p10109622193413"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p3109152243419"><a name="p3109152243419"></a><a name="p3109152243419"></a><strong id="b21091622183412"><a name="b21091622183412"></a><a name="b21091622183412"></a>32767</strong></p>
</td>
</tr>
<tr id="row19522121314272"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p11109222183412"><a name="p11109222183412"></a><a name="p11109222183412"></a>int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p10109112220344"><a name="p10109112220344"></a><a name="p10109112220344"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p81091422133411"><a name="p81091422133411"></a><a name="p81091422133411"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p11110112218342"><a name="p11110112218342"></a><a name="p11110112218342"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p10110102211343"><a name="p10110102211343"></a><a name="p10110102211343"></a><strong id="b13110122103415"><a name="b13110122103415"></a><a name="b13110122103415"></a>2147483648</strong></p>
</td>
</tr>
<tr id="row5521113102710"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p81101022123410"><a name="p81101022123410"></a><a name="p81101022123410"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p21101227346"><a name="p21101227346"></a><a name="p21101227346"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p311013226349"><a name="p311013226349"></a><a name="p311013226349"></a>-2147483648</p>
</td>
</tr>
<tr id="row2052161372719"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1911152211341"><a name="p1911152211341"></a><a name="p1911152211341"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15111172283413"><a name="p15111172283413"></a><a name="p15111172283413"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14111222163417"><a name="p14111222163417"></a><a name="p14111222163417"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1611152220348"><a name="p1611152220348"></a><a name="p1611152220348"></a><strong id="b61116222342"><a name="b61116222342"></a><a name="b61116222342"></a>2147483648</strong></p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 17**  Maxs特殊值/边界值输入的计算结果

<a name="table825085423412"></a>
<table><thead align="left"><tr id="row19251454103411"><th class="cellrowborder" valign="top" width="15.93%" id="mcps1.2.6.1.1"><p id="p325145412347"><a name="p325145412347"></a><a name="p325145412347"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="13.76%" id="mcps1.2.6.1.2"><p id="p1625175473416"><a name="p1625175473416"></a><a name="p1625175473416"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="30.31%" id="mcps1.2.6.1.3"><p id="p14251105423412"><a name="p14251105423412"></a><a name="p14251105423412"></a>scalarValue取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p102511054153413"><a name="p102511054153413"></a><a name="p102511054153413"></a>dst的元素取值</p>
<p id="p162511754103412"><a name="p162511754103412"></a><a name="p162511754103412"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p1625114549342"><a name="p1625114549342"></a><a name="p1625114549342"></a>dst的元素取值</p>
<p id="p0251754133410"><a name="p0251754133410"></a><a name="p0251754133410"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row14251125463417"><td class="cellrowborder" rowspan="18" valign="top" width="15.93%" headers="mcps1.2.6.1.1 "><p id="p72518540348"><a name="p72518540348"></a><a name="p72518540348"></a>half</p>
</td>
<td class="cellrowborder" rowspan="6" valign="top" width="13.76%" headers="mcps1.2.6.1.2 "><p id="p13251854203410"><a name="p13251854203410"></a><a name="p13251854203410"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.31%" headers="mcps1.2.6.1.3 "><p id="p14251115463412"><a name="p14251115463412"></a><a name="p14251115463412"></a>-65504.0（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p1525135483410"><a name="p1525135483410"></a><a name="p1525135483410"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p20251154123420"><a name="p20251154123420"></a><a name="p20251154123420"></a>-65504.0</p>
</td>
</tr>
<tr id="row172519546342"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p225155403411"><a name="p225155403411"></a><a name="p225155403411"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p42511954193420"><a name="p42511954193420"></a><a name="p42511954193420"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p8252254133410"><a name="p8252254133410"></a><a name="p8252254133410"></a>65504.0</p>
</td>
</tr>
<tr id="row1525265420347"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15252354123419"><a name="p15252354123419"></a><a name="p15252354123419"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p925205415348"><a name="p925205415348"></a><a name="p925205415348"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p112521054143418"><a name="p112521054143418"></a><a name="p112521054143418"></a>0.0</p>
</td>
</tr>
<tr id="row2252205415340"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1125213541348"><a name="p1125213541348"></a><a name="p1125213541348"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15252165413346"><a name="p15252165413346"></a><a name="p15252165413346"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1725245443411"><a name="p1725245443411"></a><a name="p1725245443411"></a>-0.0</p>
</td>
</tr>
<tr id="row92521544344"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p19252654173418"><a name="p19252654173418"></a><a name="p19252654173418"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13252135419346"><a name="p13252135419346"></a><a name="p13252135419346"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16252135415341"><a name="p16252135415341"></a><a name="p16252135415341"></a><strong id="b162521654173412"><a name="b162521654173412"></a><a name="b162521654173412"></a>0.0</strong></p>
</td>
</tr>
<tr id="row225212547349"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8252354133415"><a name="p8252354133415"></a><a name="p8252354133415"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p625217543348"><a name="p625217543348"></a><a name="p625217543348"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1925215443411"><a name="p1925215443411"></a><a name="p1925215443411"></a><strong id="b725275423410"><a name="b725275423410"></a><a name="b725275423410"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row925214542349"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p4252654123419"><a name="p4252654123419"></a><a name="p4252654123419"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p325285410342"><a name="p325285410342"></a><a name="p325285410342"></a>65504.0（MAX）/0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9252125412347"><a name="p9252125412347"></a><a name="p9252125412347"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p5253135463419"><a name="p5253135463419"></a><a name="p5253135463419"></a>65504.0</p>
</td>
</tr>
<tr id="row12253354163416"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p525355416344"><a name="p525355416344"></a><a name="p525355416344"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1025395443420"><a name="p1025395443420"></a><a name="p1025395443420"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4253754203411"><a name="p4253754203411"></a><a name="p4253754203411"></a><strong id="b725317542347"><a name="b725317542347"></a><a name="b725317542347"></a>0.0</strong></p>
</td>
</tr>
<tr id="row16253105403418"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1925314545348"><a name="p1925314545348"></a><a name="p1925314545348"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1425317546348"><a name="p1425317546348"></a><a name="p1425317546348"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p425315463416"><a name="p425315463416"></a><a name="p425315463416"></a><strong id="b2253185403413"><a name="b2253185403413"></a><a name="b2253185403413"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row725305443420"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p1225335483414"><a name="p1225335483414"></a><a name="p1225335483414"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1925345483417"><a name="p1925345483417"></a><a name="p1925345483417"></a>0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1325315415346"><a name="p1325315415346"></a><a name="p1325315415346"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p16253155423418"><a name="p16253155423418"></a><a name="p16253155423418"></a>0.0</p>
</td>
</tr>
<tr id="row325355412343"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p19253105453411"><a name="p19253105453411"></a><a name="p19253105453411"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1025313545345"><a name="p1025313545345"></a><a name="p1025313545345"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p82535547342"><a name="p82535547342"></a><a name="p82535547342"></a><strong id="b725325493419"><a name="b725325493419"></a><a name="b725325493419"></a>0.0</strong></p>
</td>
</tr>
<tr id="row102530548343"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4254954173415"><a name="p4254954173415"></a><a name="p4254954173415"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12254105453410"><a name="p12254105453410"></a><a name="p12254105453410"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1425416543344"><a name="p1425416543344"></a><a name="p1425416543344"></a><strong id="b2254145413342"><a name="b2254145413342"></a><a name="b2254145413342"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row1225445414345"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p16254175443415"><a name="p16254175443415"></a><a name="p16254175443415"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1225445413419"><a name="p1225445413419"></a><a name="p1225445413419"></a>-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p225485411347"><a name="p225485411347"></a><a name="p225485411347"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p11254254173419"><a name="p11254254173419"></a><a name="p11254254173419"></a>-0.0</p>
</td>
</tr>
<tr id="row1725455415343"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18254155412347"><a name="p18254155412347"></a><a name="p18254155412347"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15254454183411"><a name="p15254454183411"></a><a name="p15254454183411"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19254754163410"><a name="p19254754163410"></a><a name="p19254754163410"></a><strong id="b125415543349"><a name="b125415543349"></a><a name="b125415543349"></a>0.0</strong></p>
</td>
</tr>
<tr id="row12254175413349"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18254125420342"><a name="p18254125420342"></a><a name="p18254125420342"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1025455493420"><a name="p1025455493420"></a><a name="p1025455493420"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1725405415347"><a name="p1725405415347"></a><a name="p1725405415347"></a><strong id="b3254954143412"><a name="b3254954143412"></a><a name="b3254954143412"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row525410544344"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p525419547342"><a name="p525419547342"></a><a name="p525419547342"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p02551054183418"><a name="p02551054183418"></a><a name="p02551054183418"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p13255135403418"><a name="p13255135403418"></a><a name="p13255135403418"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p12255954163418"><a name="p12255954163418"></a><a name="p12255954163418"></a><strong id="b1625505463418"><a name="b1625505463418"></a><a name="b1625505463418"></a>0.0</strong></p>
</td>
</tr>
<tr id="row625505411344"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p5255145414348"><a name="p5255145414348"></a><a name="p5255145414348"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p625575416348"><a name="p625575416348"></a><a name="p625575416348"></a>inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p425513546343"><a name="p425513546343"></a><a name="p425513546343"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p92552054153414"><a name="p92552054153414"></a><a name="p92552054153414"></a><strong id="b725525423415"><a name="b725525423415"></a><a name="b725525423415"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row15255115423413"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p7255165417345"><a name="p7255165417345"></a><a name="p7255165417345"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p725565483420"><a name="p725565483420"></a><a name="p725565483420"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p52551754143418"><a name="p52551754143418"></a><a name="p52551754143418"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p16255105493416"><a name="p16255105493416"></a><a name="p16255105493416"></a><strong id="b7255105413420"><a name="b7255105413420"></a><a name="b7255105413420"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row789103414245"><td class="cellrowborder" rowspan="18" valign="top" width="15.93%" headers="mcps1.2.6.1.1 "><p id="p1012601152518"><a name="p1012601152518"></a><a name="p1012601152518"></a>float</p>
</td>
<td class="cellrowborder" rowspan="6" valign="top" width="13.76%" headers="mcps1.2.6.1.2 "><p id="p52218818252"><a name="p52218818252"></a><a name="p52218818252"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.31%" headers="mcps1.2.6.1.3 "><p id="p7221158152513"><a name="p7221158152513"></a><a name="p7221158152513"></a>-3.4028235e+38（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p322116812250"><a name="p322116812250"></a><a name="p322116812250"></a>-3.40282E+38</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1322198102511"><a name="p1322198102511"></a><a name="p1322198102511"></a>-3.40282E+38</p>
</td>
</tr>
<tr id="row758716368240"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14221588253"><a name="p14221588253"></a><a name="p14221588253"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p112211388251"><a name="p112211388251"></a><a name="p112211388251"></a>3.40282E+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p162219813258"><a name="p162219813258"></a><a name="p162219813258"></a>3.40282E+38</p>
</td>
</tr>
<tr id="row746683816241"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p82215810253"><a name="p82215810253"></a><a name="p82215810253"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2221784257"><a name="p2221784257"></a><a name="p2221784257"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p152216802518"><a name="p152216802518"></a><a name="p152216802518"></a>0.0</p>
</td>
</tr>
<tr id="row15755184232410"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14222485250"><a name="p14222485250"></a><a name="p14222485250"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3222198162519"><a name="p3222198162519"></a><a name="p3222198162519"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15222484258"><a name="p15222484258"></a><a name="p15222484258"></a>-0.0</p>
</td>
</tr>
<tr id="row73221747182412"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p72221832514"><a name="p72221832514"></a><a name="p72221832514"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p22229810251"><a name="p22229810251"></a><a name="p22229810251"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p62228822516"><a name="p62228822516"></a><a name="p62228822516"></a>nan</p>
</td>
</tr>
<tr id="row1322144782412"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p19222689257"><a name="p19222689257"></a><a name="p19222689257"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11222138102518"><a name="p11222138102518"></a><a name="p11222138102518"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11222168162519"><a name="p11222168162519"></a><a name="p11222168162519"></a>inf</p>
</td>
</tr>
<tr id="row1322104716247"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p6222148162516"><a name="p6222148162516"></a><a name="p6222148162516"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p22229842517"><a name="p22229842517"></a><a name="p22229842517"></a>3.4028235e+38（MAX）/0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p122219842511"><a name="p122219842511"></a><a name="p122219842511"></a>3.40282E+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1422213815250"><a name="p1422213815250"></a><a name="p1422213815250"></a>3.40282E+38</p>
</td>
</tr>
<tr id="row1732314476242"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p152226813255"><a name="p152226813255"></a><a name="p152226813255"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p922268162513"><a name="p922268162513"></a><a name="p922268162513"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p8222208192514"><a name="p8222208192514"></a><a name="p8222208192514"></a>nan</p>
</td>
</tr>
<tr id="row14673135362413"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12222138122510"><a name="p12222138122510"></a><a name="p12222138122510"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9222983258"><a name="p9222983258"></a><a name="p9222983258"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p822215814250"><a name="p822215814250"></a><a name="p822215814250"></a>inf</p>
</td>
</tr>
<tr id="row12674753162419"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p1222210816250"><a name="p1222210816250"></a><a name="p1222210816250"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12222128122516"><a name="p12222128122516"></a><a name="p12222128122516"></a>0.0/-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1922215862513"><a name="p1922215862513"></a><a name="p1922215862513"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p14222198102519"><a name="p14222198102519"></a><a name="p14222198102519"></a>0.0</p>
</td>
</tr>
<tr id="row156742053112412"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1222215818253"><a name="p1222215818253"></a><a name="p1222215818253"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12226818252"><a name="p12226818252"></a><a name="p12226818252"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p422218812518"><a name="p422218812518"></a><a name="p422218812518"></a>nan</p>
</td>
</tr>
<tr id="row166741753172410"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p422214814252"><a name="p422214814252"></a><a name="p422214814252"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12221882258"><a name="p12221882258"></a><a name="p12221882258"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p162231987258"><a name="p162231987258"></a><a name="p162231987258"></a>inf</p>
</td>
</tr>
<tr id="row2067445372414"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p3223198172515"><a name="p3223198172515"></a><a name="p3223198172515"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1122315815256"><a name="p1122315815256"></a><a name="p1122315815256"></a>-0.0/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p162231983252"><a name="p162231983252"></a><a name="p162231983252"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1422319872520"><a name="p1422319872520"></a><a name="p1422319872520"></a>-0.0</p>
</td>
</tr>
<tr id="row11674205362412"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1222318832512"><a name="p1222318832512"></a><a name="p1222318832512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p92231789258"><a name="p92231789258"></a><a name="p92231789258"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1522311811254"><a name="p1522311811254"></a><a name="p1522311811254"></a>nan</p>
</td>
</tr>
<tr id="row1667510533248"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1722317872516"><a name="p1722317872516"></a><a name="p1722317872516"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p112234812250"><a name="p112234812250"></a><a name="p112234812250"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1922314820258"><a name="p1922314820258"></a><a name="p1922314820258"></a>inf</p>
</td>
</tr>
<tr id="row3675155352411"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11223148112515"><a name="p11223148112515"></a><a name="p11223148112515"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p102233816258"><a name="p102233816258"></a><a name="p102233816258"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p152238832511"><a name="p152238832511"></a><a name="p152238832511"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p162237814256"><a name="p162237814256"></a><a name="p162237814256"></a>nan</p>
</td>
</tr>
<tr id="row85430573242"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p122233813257"><a name="p122233813257"></a><a name="p122233813257"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p722319852512"><a name="p722319852512"></a><a name="p722319852512"></a>inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p82231687252"><a name="p82231687252"></a><a name="p82231687252"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p102236872510"><a name="p102236872510"></a><a name="p102236872510"></a>inf</p>
</td>
</tr>
<tr id="row354475715248"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p922319832517"><a name="p922319832517"></a><a name="p922319832517"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11223138152519"><a name="p11223138152519"></a><a name="p11223138152519"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p42234814251"><a name="p42234814251"></a><a name="p42234814251"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p10223789253"><a name="p10223789253"></a><a name="p10223789253"></a>-inf</p>
</td>
</tr>
<tr id="row92012327282"><td class="cellrowborder" rowspan="3" valign="top" width="15.93%" headers="mcps1.2.6.1.1 "><p id="p4255154173414"><a name="p4255154173414"></a><a name="p4255154173414"></a>int16_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="13.76%" headers="mcps1.2.6.1.2 "><p id="p18255165412349"><a name="p18255165412349"></a><a name="p18255165412349"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.31%" headers="mcps1.2.6.1.3 "><p id="p925585417342"><a name="p925585417342"></a><a name="p925585417342"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p16255654183415"><a name="p16255654183415"></a><a name="p16255654183415"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p025615423417"><a name="p025615423417"></a><a name="p025615423417"></a>-32768</p>
</td>
</tr>
<tr id="row320183222812"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p7256105418349"><a name="p7256105418349"></a><a name="p7256105418349"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1525615415343"><a name="p1525615415343"></a><a name="p1525615415343"></a>32767</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2256754133415"><a name="p2256754133415"></a><a name="p2256754133415"></a>32767</p>
</td>
</tr>
<tr id="row0202324284"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p52561954173411"><a name="p52561954173411"></a><a name="p52561954173411"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p225615473413"><a name="p225615473413"></a><a name="p225615473413"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15256165417343"><a name="p15256165417343"></a><a name="p15256165417343"></a>32767</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p18256175412343"><a name="p18256175412343"></a><a name="p18256175412343"></a>32767</p>
</td>
</tr>
<tr id="row320133218284"><td class="cellrowborder" rowspan="3" valign="top" width="15.93%" headers="mcps1.2.6.1.1 "><p id="p4256115411344"><a name="p4256115411344"></a><a name="p4256115411344"></a>int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="13.76%" headers="mcps1.2.6.1.2 "><p id="p825610542345"><a name="p825610542345"></a><a name="p825610542345"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.31%" headers="mcps1.2.6.1.3 "><p id="p2025655453417"><a name="p2025655453417"></a><a name="p2025655453417"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p2256145443419"><a name="p2256145443419"></a><a name="p2256145443419"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1625615417349"><a name="p1625615417349"></a><a name="p1625615417349"></a>-2147483648</p>
</td>
</tr>
<tr id="row72013219289"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p725655412346"><a name="p725655412346"></a><a name="p725655412346"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18256154173415"><a name="p18256154173415"></a><a name="p18256154173415"></a>2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15257954143419"><a name="p15257954143419"></a><a name="p15257954143419"></a>2147483648</p>
</td>
</tr>
<tr id="row2208328282"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p125717547341"><a name="p125717547341"></a><a name="p125717547341"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p122571854193417"><a name="p122571854193417"></a><a name="p122571854193417"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1257135417343"><a name="p1257135417343"></a><a name="p1257135417343"></a>2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p225718540348"><a name="p225718540348"></a><a name="p225718540348"></a>2147483648</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 18**  Mins特殊值/边界值输入的计算结果

<a name="table714843013511"></a>
<table><thead align="left"><tr id="row7149730203510"><th class="cellrowborder" valign="top" width="16.42%" id="mcps1.2.6.1.1"><p id="p1714973063513"><a name="p1714973063513"></a><a name="p1714973063513"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="13.22%" id="mcps1.2.6.1.2"><p id="p314953015351"><a name="p314953015351"></a><a name="p314953015351"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="30.36%" id="mcps1.2.6.1.3"><p id="p3149203014357"><a name="p3149203014357"></a><a name="p3149203014357"></a>scalarValue取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p7149030193516"><a name="p7149030193516"></a><a name="p7149030193516"></a>dst的元素取值</p>
<p id="p114963017357"><a name="p114963017357"></a><a name="p114963017357"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p914919302350"><a name="p914919302350"></a><a name="p914919302350"></a>dst的元素取值</p>
<p id="p4149143011351"><a name="p4149143011351"></a><a name="p4149143011351"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row314933016355"><td class="cellrowborder" rowspan="19" valign="top" width="16.42%" headers="mcps1.2.6.1.1 "><p id="p414910301354"><a name="p414910301354"></a><a name="p414910301354"></a>half</p>
</td>
<td class="cellrowborder" rowspan="3" valign="top" width="13.22%" headers="mcps1.2.6.1.2 "><p id="p20149133063511"><a name="p20149133063511"></a><a name="p20149133063511"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.36%" headers="mcps1.2.6.1.3 "><p id="p1914953073514"><a name="p1914953073514"></a><a name="p1914953073514"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p8149143020352"><a name="p8149143020352"></a><a name="p8149143020352"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p314953073519"><a name="p314953073519"></a><a name="p314953073519"></a>-65504.0</p>
</td>
</tr>
<tr id="row17149113019357"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17149183014351"><a name="p17149183014351"></a><a name="p17149183014351"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6149173083516"><a name="p6149173083516"></a><a name="p6149173083516"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p31491030163514"><a name="p31491030163514"></a><a name="p31491030163514"></a><strong id="b17149193013512"><a name="b17149193013512"></a><a name="b17149193013512"></a>0.0</strong></p>
</td>
</tr>
<tr id="row101493303353"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p131498304354"><a name="p131498304354"></a><a name="p131498304354"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p91493302359"><a name="p91493302359"></a><a name="p91493302359"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1214983043515"><a name="p1214983043515"></a><a name="p1214983043515"></a><strong id="b11501930153510"><a name="b11501930153510"></a><a name="b11501930153510"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row101501730133516"><td class="cellrowborder" rowspan="5" valign="top" headers="mcps1.2.6.1.1 "><p id="p14150330123514"><a name="p14150330123514"></a><a name="p14150330123514"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14150103011353"><a name="p14150103011353"></a><a name="p14150103011353"></a>65504.0（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p415023015353"><a name="p415023015353"></a><a name="p415023015353"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p161501630143513"><a name="p161501630143513"></a><a name="p161501630143513"></a>65504.0</p>
</td>
</tr>
<tr id="row3150163012356"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p141501130133511"><a name="p141501130133511"></a><a name="p141501130133511"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2150930153517"><a name="p2150930153517"></a><a name="p2150930153517"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p12150530123510"><a name="p12150530123510"></a><a name="p12150530123510"></a>0.0</p>
</td>
</tr>
<tr id="row16150183010354"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p91507300359"><a name="p91507300359"></a><a name="p91507300359"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9150123003519"><a name="p9150123003519"></a><a name="p9150123003519"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p17150730173512"><a name="p17150730173512"></a><a name="p17150730173512"></a>-0.0</p>
</td>
</tr>
<tr id="row4150133033512"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p7150163010355"><a name="p7150163010355"></a><a name="p7150163010355"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p111509308351"><a name="p111509308351"></a><a name="p111509308351"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p81501230113515"><a name="p81501230113515"></a><a name="p81501230113515"></a><strong id="b1815013017353"><a name="b1815013017353"></a><a name="b1815013017353"></a>0.0</strong></p>
</td>
</tr>
<tr id="row141502305359"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1015033016354"><a name="p1015033016354"></a><a name="p1015033016354"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1915010300356"><a name="p1915010300356"></a><a name="p1915010300356"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p715173016354"><a name="p715173016354"></a><a name="p715173016354"></a><strong id="b1015193023515"><a name="b1015193023515"></a><a name="b1015193023515"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row81518302359"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p1415118305358"><a name="p1415118305358"></a><a name="p1415118305358"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11151123033514"><a name="p11151123033514"></a><a name="p11151123033514"></a>0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p151514303355"><a name="p151514303355"></a><a name="p151514303355"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p915143043513"><a name="p915143043513"></a><a name="p915143043513"></a>0.0</p>
</td>
</tr>
<tr id="row101511730123511"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1715183053512"><a name="p1715183053512"></a><a name="p1715183053512"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p815113019350"><a name="p815113019350"></a><a name="p815113019350"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p191513305352"><a name="p191513305352"></a><a name="p191513305352"></a>-0.0</p>
</td>
</tr>
<tr id="row11151123017355"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p121514304351"><a name="p121514304351"></a><a name="p121514304351"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p915119302355"><a name="p915119302355"></a><a name="p915119302355"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1615153003512"><a name="p1615153003512"></a><a name="p1615153003512"></a><strong id="b215143019355"><a name="b215143019355"></a><a name="b215143019355"></a>0.0</strong></p>
</td>
</tr>
<tr id="row171512030133513"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1315103017352"><a name="p1315103017352"></a><a name="p1315103017352"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p11151930153510"><a name="p11151930153510"></a><a name="p11151930153510"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19151143017358"><a name="p19151143017358"></a><a name="p19151143017358"></a><strong id="b115114306356"><a name="b115114306356"></a><a name="b115114306356"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row1615213033514"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p13152930193516"><a name="p13152930193516"></a><a name="p13152930193516"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p19152330103513"><a name="p19152330103513"></a><a name="p19152330103513"></a>-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p181528304351"><a name="p181528304351"></a><a name="p181528304351"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p16152130123518"><a name="p16152130123518"></a><a name="p16152130123518"></a>-0.0</p>
</td>
</tr>
<tr id="row7152203083515"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1915233012352"><a name="p1915233012352"></a><a name="p1915233012352"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9152173014354"><a name="p9152173014354"></a><a name="p9152173014354"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1715223023512"><a name="p1715223023512"></a><a name="p1715223023512"></a>0.0</p>
</td>
</tr>
<tr id="row5152123017354"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p11522302354"><a name="p11522302354"></a><a name="p11522302354"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p51520304352"><a name="p51520304352"></a><a name="p51520304352"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p41526303358"><a name="p41526303358"></a><a name="p41526303358"></a><strong id="b81521730143510"><a name="b81521730143510"></a><a name="b81521730143510"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row415223011356"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1152133011352"><a name="p1152133011352"></a><a name="p1152133011352"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p181521830183512"><a name="p181521830183512"></a><a name="p181521830183512"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p8152183013516"><a name="p8152183013516"></a><a name="p8152183013516"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p15152103003513"><a name="p15152103003513"></a><a name="p15152103003513"></a><strong id="b615216308352"><a name="b615216308352"></a><a name="b615216308352"></a>0.0</strong></p>
</td>
</tr>
<tr id="row13153830143518"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p415313023515"><a name="p415313023515"></a><a name="p415313023515"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15153193010354"><a name="p15153193010354"></a><a name="p15153193010354"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p18153153018355"><a name="p18153153018355"></a><a name="p18153153018355"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p71531930173518"><a name="p71531930173518"></a><a name="p71531930173518"></a><strong id="b131531830153510"><a name="b131531830153510"></a><a name="b131531830153510"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row3153123073519"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14153183014351"><a name="p14153183014351"></a><a name="p14153183014351"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p4153130173511"><a name="p4153130173511"></a><a name="p4153130173511"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p015353083517"><a name="p015353083517"></a><a name="p015353083517"></a><strong id="b19153430173511"><a name="b19153430173511"></a><a name="b19153430173511"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row51531730183517"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p13153163023513"><a name="p13153163023513"></a><a name="p13153163023513"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p191537301355"><a name="p191537301355"></a><a name="p191537301355"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16153143013519"><a name="p16153143013519"></a><a name="p16153143013519"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p415315307359"><a name="p415315307359"></a><a name="p415315307359"></a><strong id="b1015316307353"><a name="b1015316307353"></a><a name="b1015316307353"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row12179201214264"><td class="cellrowborder" rowspan="19" valign="top" width="16.42%" headers="mcps1.2.6.1.1 "><p id="p11340915298"><a name="p11340915298"></a><a name="p11340915298"></a>float</p>
</td>
<td class="cellrowborder" rowspan="3" valign="top" width="13.22%" headers="mcps1.2.6.1.2 "><p id="p13812265298"><a name="p13812265298"></a><a name="p13812265298"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.36%" headers="mcps1.2.6.1.3 "><p id="p18125610295"><a name="p18125610295"></a><a name="p18125610295"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p1781256192910"><a name="p1781256192910"></a><a name="p1781256192910"></a>-3.40282E+38</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p881246182910"><a name="p881246182910"></a><a name="p881246182910"></a>-3.40282E+38</p>
</td>
</tr>
<tr id="row4317171472619"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p28126618296"><a name="p28126618296"></a><a name="p28126618296"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3812365295"><a name="p3812365295"></a><a name="p3812365295"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4813146102914"><a name="p4813146102914"></a><a name="p4813146102914"></a>nan</p>
</td>
</tr>
<tr id="row8370237112812"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p38131366295"><a name="p38131366295"></a><a name="p38131366295"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1281386182911"><a name="p1281386182911"></a><a name="p1281386182911"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20813969299"><a name="p20813969299"></a><a name="p20813969299"></a>-inf</p>
</td>
</tr>
<tr id="row12213114582817"><td class="cellrowborder" rowspan="5" valign="top" headers="mcps1.2.6.1.1 "><p id="p58134632918"><a name="p58134632918"></a><a name="p58134632918"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p138131164290"><a name="p138131164290"></a><a name="p138131164290"></a>3.4028235e+38（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p681346142920"><a name="p681346142920"></a><a name="p681346142920"></a>3.40E+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p178136692912"><a name="p178136692912"></a><a name="p178136692912"></a>3.40E+38</p>
</td>
</tr>
<tr id="row1213445142815"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p138131060290"><a name="p138131060290"></a><a name="p138131060290"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1881315614299"><a name="p1881315614299"></a><a name="p1881315614299"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p481366172916"><a name="p481366172916"></a><a name="p481366172916"></a>0.0</p>
</td>
</tr>
<tr id="row0213144552815"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p381312616294"><a name="p381312616294"></a><a name="p381312616294"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p178133615298"><a name="p178133615298"></a><a name="p178133615298"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15813116172914"><a name="p15813116172914"></a><a name="p15813116172914"></a>-0.0</p>
</td>
</tr>
<tr id="row5193124932812"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p5813566296"><a name="p5813566296"></a><a name="p5813566296"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p68131569294"><a name="p68131569294"></a><a name="p68131569294"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p381315618293"><a name="p381315618293"></a><a name="p381315618293"></a>nan</p>
</td>
</tr>
<tr id="row1219384952810"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1181313617298"><a name="p1181313617298"></a><a name="p1181313617298"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1481319616297"><a name="p1481319616297"></a><a name="p1481319616297"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p881326162919"><a name="p881326162919"></a><a name="p881326162919"></a>-inf</p>
</td>
</tr>
<tr id="row3193194916287"><td class="cellrowborder" rowspan="4" valign="top" headers="mcps1.2.6.1.1 "><p id="p381410615298"><a name="p381410615298"></a><a name="p381410615298"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8814868294"><a name="p8814868294"></a><a name="p8814868294"></a>0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p481466102911"><a name="p481466102911"></a><a name="p481466102911"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p381416152916"><a name="p381416152916"></a><a name="p381416152916"></a>0.0</p>
</td>
</tr>
<tr id="row91947494284"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p16814146162916"><a name="p16814146162916"></a><a name="p16814146162916"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1881476192916"><a name="p1881476192916"></a><a name="p1881476192916"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1481414610293"><a name="p1481414610293"></a><a name="p1481414610293"></a>-0.0</p>
</td>
</tr>
<tr id="row019411498283"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p98148622915"><a name="p98148622915"></a><a name="p98148622915"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14814163295"><a name="p14814163295"></a><a name="p14814163295"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p208145652915"><a name="p208145652915"></a><a name="p208145652915"></a>nan</p>
</td>
</tr>
<tr id="row919418493283"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1814964296"><a name="p1814964296"></a><a name="p1814964296"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1981476192917"><a name="p1981476192917"></a><a name="p1981476192917"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1781414662913"><a name="p1781414662913"></a><a name="p1781414662913"></a>-inf</p>
</td>
</tr>
<tr id="row134671754192820"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p148141966291"><a name="p148141966291"></a><a name="p148141966291"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6814156172917"><a name="p6814156172917"></a><a name="p6814156172917"></a>-0.0/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p28141967294"><a name="p28141967294"></a><a name="p28141967294"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p13814863299"><a name="p13814863299"></a><a name="p13814863299"></a>-0.0</p>
</td>
</tr>
<tr id="row14682545287"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p208149692916"><a name="p208149692916"></a><a name="p208149692916"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p08148632916"><a name="p08148632916"></a><a name="p08148632916"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p168141963291"><a name="p168141963291"></a><a name="p168141963291"></a>nan</p>
</td>
</tr>
<tr id="row146815411287"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2814761297"><a name="p2814761297"></a><a name="p2814761297"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8814126102910"><a name="p8814126102910"></a><a name="p8814126102910"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1681416652910"><a name="p1681416652910"></a><a name="p1681416652910"></a>-inf</p>
</td>
</tr>
<tr id="row1846815417287"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p581418612913"><a name="p581418612913"></a><a name="p581418612913"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p108141061294"><a name="p108141061294"></a><a name="p108141061294"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p281411602910"><a name="p281411602910"></a><a name="p281411602910"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p881512619297"><a name="p881512619297"></a><a name="p881512619297"></a>nan</p>
</td>
</tr>
<tr id="row946815416283"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p5815869298"><a name="p5815869298"></a><a name="p5815869298"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p181511613290"><a name="p181511613290"></a><a name="p181511613290"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p68152615297"><a name="p68152615297"></a><a name="p68152615297"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p481517616297"><a name="p481517616297"></a><a name="p481517616297"></a>inf</p>
</td>
</tr>
<tr id="row194681654102812"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18155615298"><a name="p18155615298"></a><a name="p18155615298"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p198151652918"><a name="p198151652918"></a><a name="p198151652918"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p88151868295"><a name="p88151868295"></a><a name="p88151868295"></a>-inf</p>
</td>
</tr>
<tr id="row1373320583283"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p198151764299"><a name="p198151764299"></a><a name="p198151764299"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p108154618291"><a name="p108154618291"></a><a name="p108154618291"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p78154612299"><a name="p78154612299"></a><a name="p78154612299"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p481510610299"><a name="p481510610299"></a><a name="p481510610299"></a>-inf</p>
</td>
</tr>
<tr id="row985845752811"><td class="cellrowborder" rowspan="2" valign="top" width="16.42%" headers="mcps1.2.6.1.1 "><p id="p5153183033517"><a name="p5153183033517"></a><a name="p5153183033517"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="13.22%" headers="mcps1.2.6.1.2 "><p id="p91531630113513"><a name="p91531630113513"></a><a name="p91531630113513"></a>-32768（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.36%" headers="mcps1.2.6.1.3 "><p id="p2153193014350"><a name="p2153193014350"></a><a name="p2153193014350"></a>-32768（MIN）/32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p5153143073514"><a name="p5153143073514"></a><a name="p5153143073514"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p141541330123512"><a name="p141541330123512"></a><a name="p141541330123512"></a>-32768</p>
</td>
</tr>
<tr id="row178588577287"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17154143063518"><a name="p17154143063518"></a><a name="p17154143063518"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1115415309351"><a name="p1115415309351"></a><a name="p1115415309351"></a>32767（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p161541304354"><a name="p161541304354"></a><a name="p161541304354"></a>32767</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p415423010352"><a name="p415423010352"></a><a name="p415423010352"></a>32767</p>
</td>
</tr>
<tr id="row585835710288"><td class="cellrowborder" rowspan="2" valign="top" width="16.42%" headers="mcps1.2.6.1.1 "><p id="p2154123010357"><a name="p2154123010357"></a><a name="p2154123010357"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="13.22%" headers="mcps1.2.6.1.2 "><p id="p181543302351"><a name="p181543302351"></a><a name="p181543302351"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="30.36%" headers="mcps1.2.6.1.3 "><p id="p1515453011357"><a name="p1515453011357"></a><a name="p1515453011357"></a>-2147483648（MIN）/2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p1715419304359"><a name="p1715419304359"></a><a name="p1715419304359"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p201541430153510"><a name="p201541430153510"></a><a name="p201541430153510"></a>-2147483648</p>
</td>
</tr>
<tr id="row1885885722810"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1015443063514"><a name="p1015443063514"></a><a name="p1015443063514"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p201542306352"><a name="p201542306352"></a><a name="p201542306352"></a>2147483648（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p41541930123517"><a name="p41541930123517"></a><a name="p41541930123517"></a>2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p141541130193511"><a name="p141541130193511"></a><a name="p141541130193511"></a>2147483648</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 19**  LeakyRelu特殊值/边界值输入的计算结果

<a name="table2399171617392"></a>
<table><thead align="left"><tr id="row439921613918"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.1"><p id="p188941059113517"><a name="p188941059113517"></a><a name="p188941059113517"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.2"><p id="p58941359173518"><a name="p58941359173518"></a><a name="p58941359173518"></a>src的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.3"><p id="p4950325102715"><a name="p4950325102715"></a><a name="p4950325102715"></a>scalarValue取值</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p208946595352"><a name="p208946595352"></a><a name="p208946595352"></a>dst的元素取值</p>
<p id="p10894459113510"><a name="p10894459113510"></a><a name="p10894459113510"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p6894459193514"><a name="p6894459193514"></a><a name="p6894459193514"></a>dst的元素取值</p>
<p id="p168949598359"><a name="p168949598359"></a><a name="p168949598359"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row139919162394"><td class="cellrowborder" rowspan="17" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p19400916143915"><a name="p19400916143915"></a><a name="p19400916143915"></a>half</p>
</td>
<td class="cellrowborder" rowspan="7" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p38201644143914"><a name="p38201644143914"></a><a name="p38201644143914"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p58366484391"><a name="p58366484391"></a><a name="p58366484391"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p240017161398"><a name="p240017161398"></a><a name="p240017161398"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p4400141617394"><a name="p4400141617394"></a><a name="p4400141617394"></a><strong id="b114221749174415"><a name="b114221749174415"></a><a name="b114221749174415"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row17826235103912"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p18827535163911"><a name="p18827535163911"></a><a name="p18827535163911"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p28271357390"><a name="p28271357390"></a><a name="p28271357390"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p28275350397"><a name="p28275350397"></a><a name="p28275350397"></a><strong id="b164347492448"><a name="b164347492448"></a><a name="b164347492448"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row3632143643912"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p154911530395"><a name="p154911530395"></a><a name="p154911530395"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p186321036173910"><a name="p186321036173910"></a><a name="p186321036173910"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p088414463449"><a name="p088414463449"></a><a name="p088414463449"></a>-0.0</p>
</td>
</tr>
<tr id="row036343710392"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p134918531398"><a name="p134918531398"></a><a name="p134918531398"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6363193753916"><a name="p6363193753916"></a><a name="p6363193753916"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p118849465441"><a name="p118849465441"></a><a name="p118849465441"></a>0.0</p>
</td>
</tr>
<tr id="row810353813912"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p14911953113920"><a name="p14911953113920"></a><a name="p14911953113920"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p310363819399"><a name="p310363819399"></a><a name="p310363819399"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10104938143916"><a name="p10104938143916"></a><a name="p10104938143916"></a><strong id="b33172529444"><a name="b33172529444"></a><a name="b33172529444"></a>0.0</strong></p>
</td>
</tr>
<tr id="row18783938113910"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p134916537394"><a name="p134916537394"></a><a name="p134916537394"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p978312387394"><a name="p978312387394"></a><a name="p978312387394"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p182071144134412"><a name="p182071144134412"></a><a name="p182071144134412"></a><strong id="b19318155219442"><a name="b19318155219442"></a><a name="b19318155219442"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row18424203916397"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p104921653173920"><a name="p104921653173920"></a><a name="p104921653173920"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p942419397395"><a name="p942419397395"></a><a name="p942419397395"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1820784413444"><a name="p1820784413444"></a><a name="p1820784413444"></a><strong id="b17319652154413"><a name="b17319652154413"></a><a name="b17319652154413"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row20658405396"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15654409395"><a name="p15654409395"></a><a name="p15654409395"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p116534023917"><a name="p116534023917"></a><a name="p116534023917"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9257407418"><a name="p9257407418"></a><a name="p9257407418"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p134111331417"><a name="p134111331417"></a><a name="p134111331417"></a>65504.0</p>
</td>
</tr>
<tr id="row1291373919405"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p84236954115"><a name="p84236954115"></a><a name="p84236954115"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6479101464117"><a name="p6479101464117"></a><a name="p6479101464117"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p13913133914409"><a name="p13913133914409"></a><a name="p13913133914409"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p856131844113"><a name="p856131844113"></a><a name="p856131844113"></a>0.0</p>
</td>
</tr>
<tr id="row12215128174818"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p1216172815488"><a name="p1216172815488"></a><a name="p1216172815488"></a>-0.0</p>
<p id="p121233316487"><a name="p121233316487"></a><a name="p121233316487"></a></p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5631542104813"><a name="p5631542104813"></a><a name="p5631542104813"></a>-65504.0（MIN）/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p763124284813"><a name="p763124284813"></a><a name="p763124284813"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p206334216483"><a name="p206334216483"></a><a name="p206334216483"></a>0.0</p>
</td>
</tr>
<tr id="row13126338488"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p156314219484"><a name="p156314219484"></a><a name="p156314219484"></a>65504.0（MAX）/0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p763184244813"><a name="p763184244813"></a><a name="p763184244813"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14631642184820"><a name="p14631642184820"></a><a name="p14631642184820"></a>-0.0</p>
</td>
</tr>
<tr id="row86039300487"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p156312424481"><a name="p156312424481"></a><a name="p156312424481"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p86313426485"><a name="p86313426485"></a><a name="p86313426485"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p26384234817"><a name="p26384234817"></a><a name="p26384234817"></a>0.0</p>
</td>
</tr>
<tr id="row18671134013405"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9671144094014"><a name="p9671144094014"></a><a name="p9671144094014"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p182144134216"><a name="p182144134216"></a><a name="p182144134216"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20672204054010"><a name="p20672204054010"></a><a name="p20672204054010"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1167254094011"><a name="p1167254094011"></a><a name="p1167254094011"></a><strong id="b13858195117427"><a name="b13858195117427"></a><a name="b13858195117427"></a>0.0</strong></p>
</td>
</tr>
<tr id="row1042619419402"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p154261641174010"><a name="p154261641174010"></a><a name="p154261641174010"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p426117244319"><a name="p426117244319"></a><a name="p426117244319"></a>-65504.0（MIN）/65504.0（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9426241114015"><a name="p9426241114015"></a><a name="p9426241114015"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p74265411405"><a name="p74265411405"></a><a name="p74265411405"></a><strong id="b9253329104514"><a name="b9253329104514"></a><a name="b9253329104514"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row84318100434"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p243151012432"><a name="p243151012432"></a><a name="p243151012432"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1143910164313"><a name="p1143910164313"></a><a name="p1143910164313"></a>-65504.0（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p043910144312"><a name="p043910144312"></a><a name="p043910144312"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p5184104664515"><a name="p5184104664515"></a><a name="p5184104664515"></a><strong id="b111849463456"><a name="b111849463456"></a><a name="b111849463456"></a>65504.0</strong></p>
</td>
</tr>
<tr id="row11192133564317"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8192133564317"><a name="p8192133564317"></a><a name="p8192133564317"></a>65504.0（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p41921935114316"><a name="p41921935114316"></a><a name="p41921935114316"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14340142410484"><a name="p14340142410484"></a><a name="p14340142410484"></a><strong id="b1834002474811"><a name="b1834002474811"></a><a name="b1834002474811"></a>-65504.0</strong></p>
</td>
</tr>
<tr id="row126731952174311"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p36741452134317"><a name="p36741452134317"></a><a name="p36741452134317"></a>0.0/-0.0/nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12674185211439"><a name="p12674185211439"></a><a name="p12674185211439"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p27291466452"><a name="p27291466452"></a><a name="p27291466452"></a><strong id="b1490164713479"><a name="b1490164713479"></a><a name="b1490164713479"></a>0.0</strong></p>
</td>
</tr>
<tr id="row970710153014"><td class="cellrowborder" rowspan="17" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p17534153017304"><a name="p17534153017304"></a><a name="p17534153017304"></a>float</p>
</td>
<td class="cellrowborder" rowspan="7" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p586062612309"><a name="p586062612309"></a><a name="p586062612309"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p88605262309"><a name="p88605262309"></a><a name="p88605262309"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p148601267301"><a name="p148601267301"></a><a name="p148601267301"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1786162614308"><a name="p1786162614308"></a><a name="p1786162614308"></a>inf</p>
</td>
</tr>
<tr id="row1983815416301"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p138613265306"><a name="p138613265306"></a><a name="p138613265306"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1861172623010"><a name="p1861172623010"></a><a name="p1861172623010"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4861526173019"><a name="p4861526173019"></a><a name="p4861526173019"></a>-inf</p>
</td>
</tr>
<tr id="row1012912912306"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2086110266303"><a name="p2086110266303"></a><a name="p2086110266303"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1486112266305"><a name="p1486112266305"></a><a name="p1486112266305"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p168611226193012"><a name="p168611226193012"></a><a name="p168611226193012"></a>-0.0</p>
</td>
</tr>
<tr id="row101294915305"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1586110267309"><a name="p1586110267309"></a><a name="p1586110267309"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1986122623015"><a name="p1986122623015"></a><a name="p1986122623015"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11861726103018"><a name="p11861726103018"></a><a name="p11861726103018"></a>0.0</p>
</td>
</tr>
<tr id="row49371112163013"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1386114269302"><a name="p1386114269302"></a><a name="p1386114269302"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p786142613017"><a name="p786142613017"></a><a name="p786142613017"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7861626183010"><a name="p7861626183010"></a><a name="p7861626183010"></a>nan</p>
</td>
</tr>
<tr id="row293791273016"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1986192643019"><a name="p1986192643019"></a><a name="p1986192643019"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p198611260309"><a name="p198611260309"></a><a name="p198611260309"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p086192653011"><a name="p086192653011"></a><a name="p086192653011"></a>-inf</p>
</td>
</tr>
<tr id="row593841216304"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8861142619306"><a name="p8861142619306"></a><a name="p8861142619306"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1886111264307"><a name="p1886111264307"></a><a name="p1886111264307"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p158611926103014"><a name="p158611926103014"></a><a name="p158611926103014"></a>inf</p>
</td>
</tr>
<tr id="row893851243019"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p686117268308"><a name="p686117268308"></a><a name="p686117268308"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p2861102612308"><a name="p2861102612308"></a><a name="p2861102612308"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p12862182618306"><a name="p12862182618306"></a><a name="p12862182618306"></a>3.40E+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p10862132693014"><a name="p10862132693014"></a><a name="p10862132693014"></a>3.40E+38</p>
</td>
</tr>
<tr id="row14628181713304"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1586292619304"><a name="p1586292619304"></a><a name="p1586292619304"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p20862182610308"><a name="p20862182610308"></a><a name="p20862182610308"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p786219267304"><a name="p786219267304"></a><a name="p786219267304"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p88627267305"><a name="p88627267305"></a><a name="p88627267305"></a>0.0</p>
</td>
</tr>
<tr id="row116286175309"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p17862122619302"><a name="p17862122619302"></a><a name="p17862122619302"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1186242611301"><a name="p1186242611301"></a><a name="p1186242611301"></a>-3.4028235e+38（MIN）/-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p886242617309"><a name="p886242617309"></a><a name="p886242617309"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p20862626173019"><a name="p20862626173019"></a><a name="p20862626173019"></a>0.0</p>
</td>
</tr>
<tr id="row0628111753011"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p78627268305"><a name="p78627268305"></a><a name="p78627268305"></a>3.4028235e+38（MAX）/0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p18623269303"><a name="p18623269303"></a><a name="p18623269303"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1862126163014"><a name="p1862126163014"></a><a name="p1862126163014"></a>-0.0</p>
</td>
</tr>
<tr id="row16295175300"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4862326113019"><a name="p4862326113019"></a><a name="p4862326113019"></a>nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p6862132673017"><a name="p6862132673017"></a><a name="p6862132673017"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19862172663014"><a name="p19862172663014"></a><a name="p19862172663014"></a>nan</p>
</td>
</tr>
<tr id="row26296175304"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p4862142610301"><a name="p4862142610301"></a><a name="p4862142610301"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p20862152683016"><a name="p20862152683016"></a><a name="p20862152683016"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2862162612305"><a name="p2862162612305"></a><a name="p2862162612305"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p186217269303"><a name="p186217269303"></a><a name="p186217269303"></a>nan</p>
</td>
</tr>
<tr id="row14629101763013"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p386272643020"><a name="p386272643020"></a><a name="p386272643020"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p8862192617304"><a name="p8862192617304"></a><a name="p8862192617304"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/0.0/-0.0/nan/inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1286282633018"><a name="p1286282633018"></a><a name="p1286282633018"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p5862426103014"><a name="p5862426103014"></a><a name="p5862426103014"></a>inf</p>
</td>
</tr>
<tr id="row14629191715306"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.6.1.1 "><p id="p17862172603018"><a name="p17862172603018"></a><a name="p17862172603018"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1486232612308"><a name="p1486232612308"></a><a name="p1486232612308"></a>-3.4028235e+38（MIN）/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p188631267305"><a name="p188631267305"></a><a name="p188631267305"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p11863152693018"><a name="p11863152693018"></a><a name="p11863152693018"></a>inf</p>
</td>
</tr>
<tr id="row163051714309"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9863526103010"><a name="p9863526103010"></a><a name="p9863526103010"></a>3.4028235e+38（MAX）/inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13863182683015"><a name="p13863182683015"></a><a name="p13863182683015"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4863162611302"><a name="p4863162611302"></a><a name="p4863162611302"></a>-inf</p>
</td>
</tr>
<tr id="row3997161815308"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p68631626103017"><a name="p68631626103017"></a><a name="p68631626103017"></a>0.0/-0.0/nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1863132618302"><a name="p1863132618302"></a><a name="p1863132618302"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14863202623019"><a name="p14863202623019"></a><a name="p14863202623019"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

## 复合计算<a name="section1978519408454"></a>

**表 20**  Axpy特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table1819218568247"></a>
<table><thead align="left"><tr id="row51921756102414"><th class="cellrowborder" valign="top" width="22.220000000000002%" id="mcps1.2.7.1.1"><p id="p18192125692414"><a name="p18192125692414"></a><a name="p18192125692414"></a>src取值/scalarValue取值</p>
</th>
<th class="cellrowborder" valign="top" width="18.17%" id="mcps1.2.7.1.2"><p id="p5291959121520"><a name="p5291959121520"></a><a name="p5291959121520"></a>中间阶段(src * scalarValue)</p>
</th>
<th class="cellrowborder" valign="top" width="15.809999999999999%" id="mcps1.2.7.1.3"><p id="p1352219231839"><a name="p1352219231839"></a><a name="p1352219231839"></a>中间阶段（饱和模式，仅输出为half数据类型时支持）</p>
</th>
<th class="cellrowborder" valign="top" width="8.77%" id="mcps1.2.7.1.4"><p id="p16703112912011"><a name="p16703112912011"></a><a name="p16703112912011"></a>dst取值</p>
</th>
<th class="cellrowborder" valign="top" width="14.24%" id="mcps1.2.7.1.5"><p id="p1957389184414"><a name="p1957389184414"></a><a name="p1957389184414"></a>dst输出（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20.79%" id="mcps1.2.7.1.6"><p id="p1915510301453"><a name="p1915510301453"></a><a name="p1915510301453"></a>dst输出（饱和模式，仅输出为half数据类型时支持）</p>
</th>
</tr>
</thead>
<tbody><tr id="row101921656202419"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p1703144516462"><a name="p1703144516462"></a><a name="p1703144516462"></a>+0/+0</p>
<p id="p1898118113515"><a name="p1898118113515"></a><a name="p1898118113515"></a>-0/-0</p>
<p id="p119631346184810"><a name="p119631346184810"></a><a name="p119631346184810"></a>+0/MAX</p>
<p id="p1265112297517"><a name="p1265112297517"></a><a name="p1265112297517"></a>-0/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p1929155913153"><a name="p1929155913153"></a><a name="p1929155913153"></a>+0</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p6941256416"><a name="p6941256416"></a><a name="p6941256416"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p0703142919201"><a name="p0703142919201"></a><a name="p0703142919201"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p09361036182211"><a name="p09361036182211"></a><a name="p09361036182211"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p915510308455"><a name="p915510308455"></a><a name="p915510308455"></a>+0</p>
</td>
</tr>
<tr id="row16505286342"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p165132810345"><a name="p165132810345"></a><a name="p165132810345"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p175162893410"><a name="p175162893410"></a><a name="p175162893410"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p368717416577"><a name="p368717416577"></a><a name="p368717416577"></a>-0</p>
</td>
</tr>
<tr id="row034974392212"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p335074372215"><a name="p335074372215"></a><a name="p335074372215"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p335013434228"><a name="p335013434228"></a><a name="p335013434228"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p06700815714"><a name="p06700815714"></a><a name="p06700815714"></a>MAX</p>
</td>
</tr>
<tr id="row6189450122412"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p41908507244"><a name="p41908507244"></a><a name="p41908507244"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17190145092415"><a name="p17190145092415"></a><a name="p17190145092415"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p12190115032414"><a name="p12190115032414"></a><a name="p12190115032414"></a>MIN</p>
</td>
</tr>
<tr id="row19412139132516"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p20412499250"><a name="p20412499250"></a><a name="p20412499250"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1441218920259"><a name="p1441218920259"></a><a name="p1441218920259"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p74128912514"><a name="p74128912514"></a><a name="p74128912514"></a>MAX</p>
</td>
</tr>
<tr id="row721912189297"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p112206188292"><a name="p112206188292"></a><a name="p112206188292"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1220318132912"><a name="p1220318132912"></a><a name="p1220318132912"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p182201718202916"><a name="p182201718202916"></a><a name="p182201718202916"></a>MIN</p>
</td>
</tr>
<tr id="row203505188310"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p133517181315"><a name="p133517181315"></a><a name="p133517181315"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p635116187316"><a name="p635116187316"></a><a name="p635116187316"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p15351171819319"><a name="p15351171819319"></a><a name="p15351171819319"></a>+0</p>
</td>
</tr>
<tr id="row79813485192"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p6246950134610"><a name="p6246950134610"></a><a name="p6246950134610"></a>+0/-0</p>
<p id="p952711151524"><a name="p952711151524"></a><a name="p952711151524"></a>+0/MIN</p>
<p id="p152748219525"><a name="p152748219525"></a><a name="p152748219525"></a>-0/MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p69804841919"><a name="p69804841919"></a><a name="p69804841919"></a>-0</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p82938279410"><a name="p82938279410"></a><a name="p82938279410"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p1870362918205"><a name="p1870362918205"></a><a name="p1870362918205"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p11936133612220"><a name="p11936133612220"></a><a name="p11936133612220"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p199944891917"><a name="p199944891917"></a><a name="p199944891917"></a>+0</p>
</td>
</tr>
<tr id="row1143911221321"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p154405228324"><a name="p154405228324"></a><a name="p154405228324"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1344062243215"><a name="p1344062243215"></a><a name="p1344062243215"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p14440152253218"><a name="p14440152253218"></a><a name="p14440152253218"></a>-0</p>
</td>
</tr>
<tr id="row1410753018322"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1210753083215"><a name="p1210753083215"></a><a name="p1210753083215"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p201071230143216"><a name="p201071230143216"></a><a name="p201071230143216"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p7107153010322"><a name="p7107153010322"></a><a name="p7107153010322"></a>+inf</p>
</td>
</tr>
<tr id="row1637710423327"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p137784233212"><a name="p137784233212"></a><a name="p137784233212"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p03772420329"><a name="p03772420329"></a><a name="p03772420329"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p143771642123217"><a name="p143771642123217"></a><a name="p143771642123217"></a>-inf</p>
</td>
</tr>
<tr id="row128741450183219"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p387512507321"><a name="p387512507321"></a><a name="p387512507321"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p7875165093211"><a name="p7875165093211"></a><a name="p7875165093211"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p20875135014323"><a name="p20875135014323"></a><a name="p20875135014323"></a>MAX</p>
</td>
</tr>
<tr id="row295612512335"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p895618583313"><a name="p895618583313"></a><a name="p895618583313"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17956857333"><a name="p17956857333"></a><a name="p17956857333"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1695617513338"><a name="p1695617513338"></a><a name="p1695617513338"></a>MIN</p>
</td>
</tr>
<tr id="row178271127336"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p4827151210331"><a name="p4827151210331"></a><a name="p4827151210331"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1282711128334"><a name="p1282711128334"></a><a name="p1282711128334"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1982711124334"><a name="p1982711124334"></a><a name="p1982711124334"></a>+0</p>
</td>
</tr>
<tr id="row1854520293490"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p17951133362"><a name="p17951133362"></a><a name="p17951133362"></a>当源操作数与目的操作数数据类型一致：</p>
<p id="p841311536462"><a name="p841311536462"></a><a name="p841311536462"></a>+inf/+inf</p>
<p id="p118893485213"><a name="p118893485213"></a><a name="p118893485213"></a>+inf/MAX</p>
<p id="p7435153310532"><a name="p7435153310532"></a><a name="p7435153310532"></a>-inf/MIN</p>
<p id="p98290845410"><a name="p98290845410"></a><a name="p98290845410"></a>MAX/MAX</p>
<p id="p1744971345412"><a name="p1744971345412"></a><a name="p1744971345412"></a>MIN/MIN</p>
<p id="p128441831143618"><a name="p128441831143618"></a><a name="p128441831143618"></a>当源操作数与目的操作数数据类型不一致：</p>
<p id="p626755953613"><a name="p626755953613"></a><a name="p626755953613"></a>+inf/+inf</p>
<p id="p52671459103615"><a name="p52671459103615"></a><a name="p52671459103615"></a>+inf/MAX</p>
<p id="p326735920366"><a name="p326735920366"></a><a name="p326735920366"></a>-inf/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p112911593154"><a name="p112911593154"></a><a name="p112911593154"></a>+inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p11893514419"><a name="p11893514419"></a><a name="p11893514419"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p15497125310361"><a name="p15497125310361"></a><a name="p15497125310361"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p59361736202217"><a name="p59361736202217"></a><a name="p59361736202217"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p18545729124919"><a name="p18545729124919"></a><a name="p18545729124919"></a>MAX</p>
</td>
</tr>
<tr id="row18816201383415"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1249865314363"><a name="p1249865314363"></a><a name="p1249865314363"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p198169137349"><a name="p198169137349"></a><a name="p198169137349"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1881611319348"><a name="p1881611319348"></a><a name="p1881611319348"></a>MAX</p>
</td>
</tr>
<tr id="row1944104163411"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p049816531366"><a name="p049816531366"></a><a name="p049816531366"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p69445416340"><a name="p69445416340"></a><a name="p69445416340"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1494414119341"><a name="p1494414119341"></a><a name="p1494414119341"></a>MAX</p>
</td>
</tr>
<tr id="row3647125812341"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17498135383611"><a name="p17498135383611"></a><a name="p17498135383611"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p19648115817344"><a name="p19648115817344"></a><a name="p19648115817344"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p12648195810343"><a name="p12648195810343"></a><a name="p12648195810343"></a>MIN</p>
</td>
</tr>
<tr id="row173941449173418"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p16498195393615"><a name="p16498195393615"></a><a name="p16498195393615"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p2395749123415"><a name="p2395749123415"></a><a name="p2395749123415"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p33951049143410"><a name="p33951049143410"></a><a name="p33951049143410"></a>MAX</p>
</td>
</tr>
<tr id="row17876115512342"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p64981653123620"><a name="p64981653123620"></a><a name="p64981653123620"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p187635512343"><a name="p187635512343"></a><a name="p187635512343"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p18876655203412"><a name="p18876655203412"></a><a name="p18876655203412"></a>+0</p>
</td>
</tr>
<tr id="row21432518349"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p11498165310361"><a name="p11498165310361"></a><a name="p11498165310361"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p171431251153420"><a name="p171431251153420"></a><a name="p171431251153420"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p111431351193411"><a name="p111431351193411"></a><a name="p111431351193411"></a>+0</p>
</td>
</tr>
<tr id="row1710195373412"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p178752464378"><a name="p178752464378"></a><a name="p178752464378"></a>当源操作数与目的操作数数据类型一致：</p>
<p id="p193608573527"><a name="p193608573527"></a><a name="p193608573527"></a>+inf/-inf</p>
<p id="p11722193175313"><a name="p11722193175313"></a><a name="p11722193175313"></a>+inf/MIN</p>
<p id="p8798154575311"><a name="p8798154575311"></a><a name="p8798154575311"></a>-inf/MAX</p>
<p id="p279181795412"><a name="p279181795412"></a><a name="p279181795412"></a>MAX/MIN</p>
<p id="p510925911378"><a name="p510925911378"></a><a name="p510925911378"></a>当源操作数与目的操作数数据类型不一致：</p>
<p id="p39211418389"><a name="p39211418389"></a><a name="p39211418389"></a>+inf/-inf</p>
<p id="p10922104103820"><a name="p10922104103820"></a><a name="p10922104103820"></a>+inf/MIN</p>
<p id="p1492214413381"><a name="p1492214413381"></a><a name="p1492214413381"></a>-inf/MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p17101175343415"><a name="p17101175343415"></a><a name="p17101175343415"></a>-inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p59131571247"><a name="p59131571247"></a><a name="p59131571247"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p149834478376"><a name="p149834478376"></a><a name="p149834478376"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p1710145314345"><a name="p1710145314345"></a><a name="p1710145314345"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p1710210534346"><a name="p1710210534346"></a><a name="p1710210534346"></a>MIN</p>
</td>
</tr>
<tr id="row19414134423414"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p0983114716372"><a name="p0983114716372"></a><a name="p0983114716372"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1141410447343"><a name="p1141410447343"></a><a name="p1141410447343"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p18414544163411"><a name="p18414544163411"></a><a name="p18414544163411"></a>MIN</p>
</td>
</tr>
<tr id="row477234507"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p198314733718"><a name="p198314733718"></a><a name="p198314733718"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17936193662218"><a name="p17936193662218"></a><a name="p17936193662218"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p17921132731512"><a name="p17921132731512"></a><a name="p17921132731512"></a>MAX</p>
</td>
</tr>
<tr id="row1767244318210"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p119831747133713"><a name="p119831747133713"></a><a name="p119831747133713"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p893514367226"><a name="p893514367226"></a><a name="p893514367226"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1267217431029"><a name="p1267217431029"></a><a name="p1267217431029"></a>MIN</p>
</td>
</tr>
<tr id="row1043114571982"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p998313472374"><a name="p998313472374"></a><a name="p998313472374"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p293573662213"><a name="p293573662213"></a><a name="p293573662213"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1043118578816"><a name="p1043118578816"></a><a name="p1043118578816"></a>+0</p>
</td>
</tr>
<tr id="row1923613138501"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1984134711373"><a name="p1984134711373"></a><a name="p1984134711373"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p18934133652214"><a name="p18934133652214"></a><a name="p18934133652214"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p5236213165010"><a name="p5236213165010"></a><a name="p5236213165010"></a>MIN</p>
</td>
</tr>
<tr id="row12606132220500"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p139841647123716"><a name="p139841647123716"></a><a name="p139841647123716"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p116071229506"><a name="p116071229506"></a><a name="p116071229506"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p260762225017"><a name="p260762225017"></a><a name="p260762225017"></a>+0</p>
</td>
</tr>
<tr id="row029252611506"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p33086010479"><a name="p33086010479"></a><a name="p33086010479"></a>使得src * scalarValue = MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p122911759191512"><a name="p122911759191512"></a><a name="p122911759191512"></a>MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p1096701018417"><a name="p1096701018417"></a><a name="p1096701018417"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p077916334410"><a name="p077916334410"></a><a name="p077916334410"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p1729242615011"><a name="p1729242615011"></a><a name="p1729242615011"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p17292126195013"><a name="p17292126195013"></a><a name="p17292126195013"></a>MAX</p>
</td>
</tr>
<tr id="row1794719566508"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p5779123314119"><a name="p5779123314119"></a><a name="p5779123314119"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p10947185612503"><a name="p10947185612503"></a><a name="p10947185612503"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p119476563508"><a name="p119476563508"></a><a name="p119476563508"></a>MAX</p>
</td>
</tr>
<tr id="row4452181084710"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p3779143314114"><a name="p3779143314114"></a><a name="p3779143314114"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p745218101473"><a name="p745218101473"></a><a name="p745218101473"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p645271074717"><a name="p645271074717"></a><a name="p645271074717"></a>MAX</p>
</td>
</tr>
<tr id="row330182555119"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p12780153394115"><a name="p12780153394115"></a><a name="p12780153394115"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1302625155118"><a name="p1302625155118"></a><a name="p1302625155118"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1730242517511"><a name="p1730242517511"></a><a name="p1730242517511"></a>MIN</p>
</td>
</tr>
<tr id="row20854172745118"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p978033318419"><a name="p978033318419"></a><a name="p978033318419"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1585472785119"><a name="p1585472785119"></a><a name="p1585472785119"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p185442715117"><a name="p185442715117"></a><a name="p185442715117"></a>MAX</p>
</td>
</tr>
<tr id="row161291630175120"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p11780183394115"><a name="p11780183394115"></a><a name="p11780183394115"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p212923025110"><a name="p212923025110"></a><a name="p212923025110"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p51290301512"><a name="p51290301512"></a><a name="p51290301512"></a>+0</p>
</td>
</tr>
<tr id="row12198532125114"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p177806331419"><a name="p177806331419"></a><a name="p177806331419"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p01981432145119"><a name="p01981432145119"></a><a name="p01981432145119"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1919823212511"><a name="p1919823212511"></a><a name="p1919823212511"></a>+0</p>
</td>
</tr>
<tr id="row86573485112"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p2107311479"><a name="p2107311479"></a><a name="p2107311479"></a>使得src * scalarValue = MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p13291659141516"><a name="p13291659141516"></a><a name="p13291659141516"></a>MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p1368114162412"><a name="p1368114162412"></a><a name="p1368114162412"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p756011110447"><a name="p756011110447"></a><a name="p756011110447"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p86513348515"><a name="p86513348515"></a><a name="p86513348515"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p865193405117"><a name="p865193405117"></a><a name="p865193405117"></a>MIN</p>
</td>
</tr>
<tr id="row174244283528"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p8560811443"><a name="p8560811443"></a><a name="p8560811443"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p19425122814522"><a name="p19425122814522"></a><a name="p19425122814522"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p242512813523"><a name="p242512813523"></a><a name="p242512813523"></a>MIN</p>
</td>
</tr>
<tr id="row161921156162419"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1856001134413"><a name="p1856001134413"></a><a name="p1856001134413"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p115731791445"><a name="p115731791445"></a><a name="p115731791445"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p515514301452"><a name="p515514301452"></a><a name="p515514301452"></a>MAX</p>
</td>
</tr>
<tr id="row1271034435211"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1956041204410"><a name="p1956041204410"></a><a name="p1956041204410"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p147102449529"><a name="p147102449529"></a><a name="p147102449529"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p117109446528"><a name="p117109446528"></a><a name="p117109446528"></a>MIN</p>
</td>
</tr>
<tr id="row1771584617523"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1560111445"><a name="p1560111445"></a><a name="p1560111445"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p18715144645220"><a name="p18715144645220"></a><a name="p18715144645220"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p871544617528"><a name="p871544617528"></a><a name="p871544617528"></a>+0</p>
</td>
</tr>
<tr id="row454534815215"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p75616194419"><a name="p75616194419"></a><a name="p75616194419"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p11546164865219"><a name="p11546164865219"></a><a name="p11546164865219"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1854674818525"><a name="p1854674818525"></a><a name="p1854674818525"></a>MIN</p>
</td>
</tr>
<tr id="row9318454145213"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1256115112441"><a name="p1256115112441"></a><a name="p1256115112441"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p13318195417523"><a name="p13318195417523"></a><a name="p13318195417523"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p13318454185211"><a name="p13318454185211"></a><a name="p13318454185211"></a>+0</p>
</td>
</tr>
<tr id="row27219576520"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p102531348163819"><a name="p102531348163819"></a><a name="p102531348163819"></a>nan/任意值</p>
<p id="p3995105175615"><a name="p3995105175615"></a><a name="p3995105175615"></a>+0/+inf</p>
<p id="p132522105615"><a name="p132522105615"></a><a name="p132522105615"></a>+0/-inf</p>
<p id="p1624910263567"><a name="p1624910263567"></a><a name="p1624910263567"></a>-0/+inf</p>
<p id="p1233930105613"><a name="p1233930105613"></a><a name="p1233930105613"></a>-0/-inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p11292195921510"><a name="p11292195921510"></a><a name="p11292195921510"></a>nan</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p14019191441"><a name="p14019191441"></a><a name="p14019191441"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p10664823104515"><a name="p10664823104515"></a><a name="p10664823104515"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p172165725215"><a name="p172165725215"></a><a name="p172165725215"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p18721157205211"><a name="p18721157205211"></a><a name="p18721157205211"></a>+0</p>
</td>
</tr>
<tr id="row8193195916520"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p566442315450"><a name="p566442315450"></a><a name="p566442315450"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1119365915522"><a name="p1119365915522"></a><a name="p1119365915522"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p161939595527"><a name="p161939595527"></a><a name="p161939595527"></a>+0</p>
</td>
</tr>
<tr id="row16379102094410"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p5664152313458"><a name="p5664152313458"></a><a name="p5664152313458"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p53791920164413"><a name="p53791920164413"></a><a name="p53791920164413"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3379202013445"><a name="p3379202013445"></a><a name="p3379202013445"></a>MAX</p>
</td>
</tr>
<tr id="row154531814412"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p116641823104512"><a name="p116641823104512"></a><a name="p116641823104512"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p154551810442"><a name="p154551810442"></a><a name="p154551810442"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p75453183443"><a name="p75453183443"></a><a name="p75453183443"></a>MIN</p>
</td>
</tr>
<tr id="row1931516114411"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p36651223174512"><a name="p36651223174512"></a><a name="p36651223174512"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p931201613446"><a name="p931201613446"></a><a name="p931201613446"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p153161684411"><a name="p153161684411"></a><a name="p153161684411"></a>MAX</p>
</td>
</tr>
<tr id="row10127713144413"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1566518237459"><a name="p1566518237459"></a><a name="p1566518237459"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p7127131314441"><a name="p7127131314441"></a><a name="p7127131314441"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p812711318442"><a name="p812711318442"></a><a name="p812711318442"></a>MIN</p>
</td>
</tr>
<tr id="row17656544415"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1866517231453"><a name="p1866517231453"></a><a name="p1866517231453"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p197661857441"><a name="p197661857441"></a><a name="p197661857441"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p876665114415"><a name="p876665114415"></a><a name="p876665114415"></a>+0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 21**  MulAddDst特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table1042313583406"></a>
<table><thead align="left"><tr id="row144231958154013"><th class="cellrowborder" valign="top" width="22.220000000000002%" id="mcps1.2.7.1.1"><p id="p204238586403"><a name="p204238586403"></a><a name="p204238586403"></a>src0取值/src1取值</p>
</th>
<th class="cellrowborder" valign="top" width="18.17%" id="mcps1.2.7.1.2"><p id="p154231058144011"><a name="p154231058144011"></a><a name="p154231058144011"></a>中间阶段(src0 * src1)</p>
</th>
<th class="cellrowborder" valign="top" width="15.809999999999999%" id="mcps1.2.7.1.3"><p id="p842313583406"><a name="p842313583406"></a><a name="p842313583406"></a>中间阶段（饱和模式，仅输出为half数据类型时支持）</p>
</th>
<th class="cellrowborder" valign="top" width="8.77%" id="mcps1.2.7.1.4"><p id="p174239587401"><a name="p174239587401"></a><a name="p174239587401"></a>dst取值</p>
</th>
<th class="cellrowborder" valign="top" width="14.24%" id="mcps1.2.7.1.5"><p id="p642435810402"><a name="p642435810402"></a><a name="p642435810402"></a>dst输出（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20.79%" id="mcps1.2.7.1.6"><p id="p542445864015"><a name="p542445864015"></a><a name="p542445864015"></a>dst输出（饱和模式，仅输出为half数据类型时支持）</p>
</th>
</tr>
</thead>
<tbody><tr id="row342425817401"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p11424135810408"><a name="p11424135810408"></a><a name="p11424135810408"></a>+0/+0</p>
<p id="p6424658194017"><a name="p6424658194017"></a><a name="p6424658194017"></a>-0/-0</p>
<p id="p1942415816407"><a name="p1942415816407"></a><a name="p1942415816407"></a>+0/MAX</p>
<p id="p0424145844011"><a name="p0424145844011"></a><a name="p0424145844011"></a>-0/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p9424195810408"><a name="p9424195810408"></a><a name="p9424195810408"></a>+0</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p142485816407"><a name="p142485816407"></a><a name="p142485816407"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p042475814408"><a name="p042475814408"></a><a name="p042475814408"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p1542425818404"><a name="p1542425818404"></a><a name="p1542425818404"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p5425195894016"><a name="p5425195894016"></a><a name="p5425195894016"></a>+0</p>
</td>
</tr>
<tr id="row16425175819406"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p13425258114016"><a name="p13425258114016"></a><a name="p13425258114016"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p9425758124015"><a name="p9425758124015"></a><a name="p9425758124015"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p14251958204018"><a name="p14251958204018"></a><a name="p14251958204018"></a>-0</p>
</td>
</tr>
<tr id="row342585864018"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p142511585403"><a name="p142511585403"></a><a name="p142511585403"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p16425658174014"><a name="p16425658174014"></a><a name="p16425658174014"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1642575874016"><a name="p1642575874016"></a><a name="p1642575874016"></a>MAX</p>
</td>
</tr>
<tr id="row1842612582405"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p7426115804018"><a name="p7426115804018"></a><a name="p7426115804018"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p0426195813403"><a name="p0426195813403"></a><a name="p0426195813403"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p842665894016"><a name="p842665894016"></a><a name="p842665894016"></a>MIN</p>
</td>
</tr>
<tr id="row11426105894020"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p3426145854016"><a name="p3426145854016"></a><a name="p3426145854016"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p124268582405"><a name="p124268582405"></a><a name="p124268582405"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p10426175816403"><a name="p10426175816403"></a><a name="p10426175816403"></a>MAX</p>
</td>
</tr>
<tr id="row10426175814011"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p124261586401"><a name="p124261586401"></a><a name="p124261586401"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p15426858184016"><a name="p15426858184016"></a><a name="p15426858184016"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p12426175884020"><a name="p12426175884020"></a><a name="p12426175884020"></a>MIN</p>
</td>
</tr>
<tr id="row742711580402"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p742711585401"><a name="p742711585401"></a><a name="p742711585401"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14427658184018"><a name="p14427658184018"></a><a name="p14427658184018"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p842705819404"><a name="p842705819404"></a><a name="p842705819404"></a>+0</p>
</td>
</tr>
<tr id="row15427658154010"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p1427135894015"><a name="p1427135894015"></a><a name="p1427135894015"></a>+0/-0</p>
<p id="p64271058134013"><a name="p64271058134013"></a><a name="p64271058134013"></a>+0/MIN</p>
<p id="p242717581400"><a name="p242717581400"></a><a name="p242717581400"></a>-0/MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p5427175894018"><a name="p5427175894018"></a><a name="p5427175894018"></a>-0</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p1842715874012"><a name="p1842715874012"></a><a name="p1842715874012"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p124271558104014"><a name="p124271558104014"></a><a name="p124271558104014"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p842719586401"><a name="p842719586401"></a><a name="p842719586401"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p13428175854010"><a name="p13428175854010"></a><a name="p13428175854010"></a>+0</p>
</td>
</tr>
<tr id="row1642805844016"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p9428205834012"><a name="p9428205834012"></a><a name="p9428205834012"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p114284588405"><a name="p114284588405"></a><a name="p114284588405"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p8428205818408"><a name="p8428205818408"></a><a name="p8428205818408"></a>-0</p>
</td>
</tr>
<tr id="row242835864014"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p4428105864017"><a name="p4428105864017"></a><a name="p4428105864017"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1342855819407"><a name="p1342855819407"></a><a name="p1342855819407"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p15428358124015"><a name="p15428358124015"></a><a name="p15428358124015"></a>+inf</p>
</td>
</tr>
<tr id="row0428195819406"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p184281758184018"><a name="p184281758184018"></a><a name="p184281758184018"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p24281258184016"><a name="p24281258184016"></a><a name="p24281258184016"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p64288587409"><a name="p64288587409"></a><a name="p64288587409"></a>-inf</p>
</td>
</tr>
<tr id="row4429758104014"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p144291858124020"><a name="p144291858124020"></a><a name="p144291858124020"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14290580405"><a name="p14290580405"></a><a name="p14290580405"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p14429175816408"><a name="p14429175816408"></a><a name="p14429175816408"></a>MAX</p>
</td>
</tr>
<tr id="row1942919583407"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p124292058184018"><a name="p124292058184018"></a><a name="p124292058184018"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p4429145817402"><a name="p4429145817402"></a><a name="p4429145817402"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1442912581404"><a name="p1442912581404"></a><a name="p1442912581404"></a>MIN</p>
</td>
</tr>
<tr id="row64291858114010"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1542915818407"><a name="p1542915818407"></a><a name="p1542915818407"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1042914582405"><a name="p1042914582405"></a><a name="p1042914582405"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p4430358144014"><a name="p4430358144014"></a><a name="p4430358144014"></a>+0</p>
</td>
</tr>
<tr id="row12430105844011"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p243017581401"><a name="p243017581401"></a><a name="p243017581401"></a>当源操作数与目的操作数数据类型一致：</p>
<p id="p4430125819406"><a name="p4430125819406"></a><a name="p4430125819406"></a>+inf/+inf</p>
<p id="p743017584406"><a name="p743017584406"></a><a name="p743017584406"></a>+inf/MAX</p>
<p id="p84301058194015"><a name="p84301058194015"></a><a name="p84301058194015"></a>-inf/MIN</p>
<p id="p164301658154017"><a name="p164301658154017"></a><a name="p164301658154017"></a>MAX/MAX</p>
<p id="p643075824011"><a name="p643075824011"></a><a name="p643075824011"></a>MIN/MIN</p>
<p id="p1143025819404"><a name="p1143025819404"></a><a name="p1143025819404"></a>当源操作数与目的操作数数据类型不一致：</p>
<p id="p114307583408"><a name="p114307583408"></a><a name="p114307583408"></a>+inf/+inf</p>
<p id="p4430258134013"><a name="p4430258134013"></a><a name="p4430258134013"></a>+inf/MAX</p>
<p id="p1143010588402"><a name="p1143010588402"></a><a name="p1143010588402"></a>-inf/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p164309585402"><a name="p164309585402"></a><a name="p164309585402"></a>+inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p204301158124018"><a name="p204301158124018"></a><a name="p204301158124018"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p5430195804012"><a name="p5430195804012"></a><a name="p5430195804012"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p11431155864013"><a name="p11431155864013"></a><a name="p11431155864013"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p15431205824015"><a name="p15431205824015"></a><a name="p15431205824015"></a>MAX</p>
</td>
</tr>
<tr id="row74311458164014"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p443155818406"><a name="p443155818406"></a><a name="p443155818406"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p2431658154011"><a name="p2431658154011"></a><a name="p2431658154011"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p134311258144013"><a name="p134311258144013"></a><a name="p134311258144013"></a>MAX</p>
</td>
</tr>
<tr id="row1843125818402"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1943114587409"><a name="p1943114587409"></a><a name="p1943114587409"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p11431125815403"><a name="p11431125815403"></a><a name="p11431125815403"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1143195810405"><a name="p1143195810405"></a><a name="p1143195810405"></a>MAX</p>
</td>
</tr>
<tr id="row3431175834016"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p543115817400"><a name="p543115817400"></a><a name="p543115817400"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p10432105874019"><a name="p10432105874019"></a><a name="p10432105874019"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p104321658194015"><a name="p104321658194015"></a><a name="p104321658194015"></a>MIN</p>
</td>
</tr>
<tr id="row16432858174015"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p743214582409"><a name="p743214582409"></a><a name="p743214582409"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p64321589401"><a name="p64321589401"></a><a name="p64321589401"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1432458144011"><a name="p1432458144011"></a><a name="p1432458144011"></a>MAX</p>
</td>
</tr>
<tr id="row74325586408"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p0432165818407"><a name="p0432165818407"></a><a name="p0432165818407"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p0432858104020"><a name="p0432858104020"></a><a name="p0432858104020"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1443265812404"><a name="p1443265812404"></a><a name="p1443265812404"></a>+0</p>
</td>
</tr>
<tr id="row543285818405"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p11433958204013"><a name="p11433958204013"></a><a name="p11433958204013"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1343325824014"><a name="p1343325824014"></a><a name="p1343325824014"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p843345884011"><a name="p843345884011"></a><a name="p843345884011"></a>+0</p>
</td>
</tr>
<tr id="row144339586405"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p54331758144020"><a name="p54331758144020"></a><a name="p54331758144020"></a>当源操作数与目的操作数数据类型一致：</p>
<p id="p12433105811400"><a name="p12433105811400"></a><a name="p12433105811400"></a>+inf/-inf</p>
<p id="p3433145812400"><a name="p3433145812400"></a><a name="p3433145812400"></a>+inf/MIN</p>
<p id="p10433458154019"><a name="p10433458154019"></a><a name="p10433458154019"></a>-inf/MAX</p>
<p id="p9433155884020"><a name="p9433155884020"></a><a name="p9433155884020"></a>MAX/MIN</p>
<p id="p843315874018"><a name="p843315874018"></a><a name="p843315874018"></a>当源操作数与目的操作数数据类型不一致：</p>
<p id="p10433165854020"><a name="p10433165854020"></a><a name="p10433165854020"></a>+inf/-inf</p>
<p id="p14433258164014"><a name="p14433258164014"></a><a name="p14433258164014"></a>+inf/MIN</p>
<p id="p84334586409"><a name="p84334586409"></a><a name="p84334586409"></a>-inf/MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p1543312585407"><a name="p1543312585407"></a><a name="p1543312585407"></a>-inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p943345874010"><a name="p943345874010"></a><a name="p943345874010"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p154347586405"><a name="p154347586405"></a><a name="p154347586405"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p84341258184011"><a name="p84341258184011"></a><a name="p84341258184011"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p14434135817408"><a name="p14434135817408"></a><a name="p14434135817408"></a>MIN</p>
</td>
</tr>
<tr id="row34341858114019"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p34346583408"><a name="p34346583408"></a><a name="p34346583408"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1743435815402"><a name="p1743435815402"></a><a name="p1743435815402"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p6434155810409"><a name="p6434155810409"></a><a name="p6434155810409"></a>MIN</p>
</td>
</tr>
<tr id="row4434258124015"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p194341058134015"><a name="p194341058134015"></a><a name="p194341058134015"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p24341958114019"><a name="p24341958114019"></a><a name="p24341958114019"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p7434155817406"><a name="p7434155817406"></a><a name="p7434155817406"></a>MAX</p>
</td>
</tr>
<tr id="row14435145824012"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p6435145812405"><a name="p6435145812405"></a><a name="p6435145812405"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p7435458164017"><a name="p7435458164017"></a><a name="p7435458164017"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p154359580402"><a name="p154359580402"></a><a name="p154359580402"></a>MIN</p>
</td>
</tr>
<tr id="row44351158124014"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p8435145820404"><a name="p8435145820404"></a><a name="p8435145820404"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p44351058144010"><a name="p44351058144010"></a><a name="p44351058144010"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p4435135815402"><a name="p4435135815402"></a><a name="p4435135815402"></a>+0</p>
</td>
</tr>
<tr id="row20435858134012"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p9435155894011"><a name="p9435155894011"></a><a name="p9435155894011"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p12435258114016"><a name="p12435258114016"></a><a name="p12435258114016"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p15436145813402"><a name="p15436145813402"></a><a name="p15436145813402"></a>MIN</p>
</td>
</tr>
<tr id="row3436115817403"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p9436175814016"><a name="p9436175814016"></a><a name="p9436175814016"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17436858194015"><a name="p17436858194015"></a><a name="p17436858194015"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p14436105824012"><a name="p14436105824012"></a><a name="p14436105824012"></a>+0</p>
</td>
</tr>
<tr id="row343605812405"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p1843685814407"><a name="p1843685814407"></a><a name="p1843685814407"></a>使得src0 * src1  = MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p1436205815403"><a name="p1436205815403"></a><a name="p1436205815403"></a>MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p24361458204017"><a name="p24361458204017"></a><a name="p24361458204017"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p194361458114014"><a name="p194361458114014"></a><a name="p194361458114014"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p15436185811401"><a name="p15436185811401"></a><a name="p15436185811401"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p164369588408"><a name="p164369588408"></a><a name="p164369588408"></a>MAX</p>
</td>
</tr>
<tr id="row1443795894011"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p843711589401"><a name="p843711589401"></a><a name="p843711589401"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p443715844012"><a name="p443715844012"></a><a name="p443715844012"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1643715810401"><a name="p1643715810401"></a><a name="p1643715810401"></a>MAX</p>
</td>
</tr>
<tr id="row143715818402"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p15437135816409"><a name="p15437135816409"></a><a name="p15437135816409"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p15437115854013"><a name="p15437115854013"></a><a name="p15437115854013"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p44371580402"><a name="p44371580402"></a><a name="p44371580402"></a>MAX</p>
</td>
</tr>
<tr id="row1643716583409"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p2437175817403"><a name="p2437175817403"></a><a name="p2437175817403"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p343715884019"><a name="p343715884019"></a><a name="p343715884019"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p443755864012"><a name="p443755864012"></a><a name="p443755864012"></a>MIN</p>
</td>
</tr>
<tr id="row184381058194018"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p8438155811401"><a name="p8438155811401"></a><a name="p8438155811401"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p9438185874013"><a name="p9438185874013"></a><a name="p9438185874013"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1843814584402"><a name="p1843814584402"></a><a name="p1843814584402"></a>MAX</p>
</td>
</tr>
<tr id="row184381158104013"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p194381958114010"><a name="p194381958114010"></a><a name="p194381958114010"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p10438558184010"><a name="p10438558184010"></a><a name="p10438558184010"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1243835811402"><a name="p1243835811402"></a><a name="p1243835811402"></a>+0</p>
</td>
</tr>
<tr id="row84384588401"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p443815588403"><a name="p443815588403"></a><a name="p443815588403"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p54387589402"><a name="p54387589402"></a><a name="p54387589402"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p243814581406"><a name="p243814581406"></a><a name="p243814581406"></a>+0</p>
</td>
</tr>
<tr id="row124391858134010"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p74391158134010"><a name="p74391158134010"></a><a name="p74391158134010"></a>使得src0 * src1 = MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p14439558174019"><a name="p14439558174019"></a><a name="p14439558174019"></a>MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p943965814405"><a name="p943965814405"></a><a name="p943965814405"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p843945874012"><a name="p843945874012"></a><a name="p843945874012"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p1043918587408"><a name="p1043918587408"></a><a name="p1043918587408"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p1443935819404"><a name="p1443935819404"></a><a name="p1443935819404"></a>MIN</p>
</td>
</tr>
<tr id="row1943965813402"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p15439165844017"><a name="p15439165844017"></a><a name="p15439165844017"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p144395589403"><a name="p144395589403"></a><a name="p144395589403"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1843911586401"><a name="p1843911586401"></a><a name="p1843911586401"></a>MIN</p>
</td>
</tr>
<tr id="row104391589404"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p7440165864017"><a name="p7440165864017"></a><a name="p7440165864017"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p16440558194010"><a name="p16440558194010"></a><a name="p16440558194010"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3440115884010"><a name="p3440115884010"></a><a name="p3440115884010"></a>MAX</p>
</td>
</tr>
<tr id="row444035864017"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p144401587406"><a name="p144401587406"></a><a name="p144401587406"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p744005884013"><a name="p744005884013"></a><a name="p744005884013"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1944065894018"><a name="p1944065894018"></a><a name="p1944065894018"></a>MIN</p>
</td>
</tr>
<tr id="row14402058154019"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p744020584403"><a name="p744020584403"></a><a name="p744020584403"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1544011580403"><a name="p1544011580403"></a><a name="p1544011580403"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1644015581402"><a name="p1644015581402"></a><a name="p1644015581402"></a>+0</p>
</td>
</tr>
<tr id="row154409584404"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1144175844016"><a name="p1144175844016"></a><a name="p1144175844016"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p944125813407"><a name="p944125813407"></a><a name="p944125813407"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1644165818403"><a name="p1644165818403"></a><a name="p1644165818403"></a>MIN</p>
</td>
</tr>
<tr id="row15441125817409"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p13441125818400"><a name="p13441125818400"></a><a name="p13441125818400"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1144125815404"><a name="p1144125815404"></a><a name="p1144125815404"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1044219582408"><a name="p1044219582408"></a><a name="p1044219582408"></a>+0</p>
</td>
</tr>
<tr id="row1244213583407"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p74421158134017"><a name="p74421158134017"></a><a name="p74421158134017"></a>nan/任意值</p>
<p id="p844217580403"><a name="p844217580403"></a><a name="p844217580403"></a>+0/+inf</p>
<p id="p2442558154014"><a name="p2442558154014"></a><a name="p2442558154014"></a>+0/-inf</p>
<p id="p144218583408"><a name="p144218583408"></a><a name="p144218583408"></a>-0/+inf</p>
<p id="p19442958164019"><a name="p19442958164019"></a><a name="p19442958164019"></a>-0/-inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p1144210583401"><a name="p1144210583401"></a><a name="p1144210583401"></a>nan</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p144305811403"><a name="p144305811403"></a><a name="p144305811403"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p544365834012"><a name="p544365834012"></a><a name="p544365834012"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p1644345817408"><a name="p1644345817408"></a><a name="p1644345817408"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p7443185811405"><a name="p7443185811405"></a><a name="p7443185811405"></a>+0</p>
</td>
</tr>
<tr id="row1244355812403"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1344355864014"><a name="p1344355864014"></a><a name="p1344355864014"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p204444581407"><a name="p204444581407"></a><a name="p204444581407"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1744413585404"><a name="p1744413585404"></a><a name="p1744413585404"></a>+0</p>
</td>
</tr>
<tr id="row1044418582406"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p194441658184016"><a name="p194441658184016"></a><a name="p194441658184016"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p04441058144010"><a name="p04441058144010"></a><a name="p04441058144010"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p104440585403"><a name="p104440585403"></a><a name="p104440585403"></a>MAX</p>
</td>
</tr>
<tr id="row1944417586405"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p174440581409"><a name="p174440581409"></a><a name="p174440581409"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14445758124020"><a name="p14445758124020"></a><a name="p14445758124020"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p344555894014"><a name="p344555894014"></a><a name="p344555894014"></a>MIN</p>
</td>
</tr>
<tr id="row11445145814016"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1944516580404"><a name="p1944516580404"></a><a name="p1944516580404"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1044520582403"><a name="p1044520582403"></a><a name="p1044520582403"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1444518581403"><a name="p1444518581403"></a><a name="p1444518581403"></a>MAX</p>
</td>
</tr>
<tr id="row244525804013"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p84452058184012"><a name="p84452058184012"></a><a name="p84452058184012"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p34451458144019"><a name="p34451458144019"></a><a name="p34451458144019"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1644665812406"><a name="p1644665812406"></a><a name="p1644665812406"></a>MIN</p>
</td>
</tr>
<tr id="row1244611584408"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p34461258104016"><a name="p34461258104016"></a><a name="p34461258104016"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p184460583405"><a name="p184460583405"></a><a name="p184460583405"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p444695834012"><a name="p444695834012"></a><a name="p444695834012"></a>+0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 22**  FusedMulAdd特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table1158211814515"></a>
<table><thead align="left"><tr id="row15821894512"><th class="cellrowborder" valign="top" width="22.220000000000002%" id="mcps1.2.7.1.1"><p id="p55827819456"><a name="p55827819456"></a><a name="p55827819456"></a>src0取值/dst取值</p>
</th>
<th class="cellrowborder" valign="top" width="18.17%" id="mcps1.2.7.1.2"><p id="p1158317884510"><a name="p1158317884510"></a><a name="p1158317884510"></a>中间阶段(src0 * dst)</p>
</th>
<th class="cellrowborder" valign="top" width="15.809999999999999%" id="mcps1.2.7.1.3"><p id="p13583188114516"><a name="p13583188114516"></a><a name="p13583188114516"></a>中间阶段（饱和模式，仅输出为half数据类型时支持）</p>
</th>
<th class="cellrowborder" valign="top" width="8.77%" id="mcps1.2.7.1.4"><p id="p65835818453"><a name="p65835818453"></a><a name="p65835818453"></a>src1取值</p>
</th>
<th class="cellrowborder" valign="top" width="14.24%" id="mcps1.2.7.1.5"><p id="p15838814510"><a name="p15838814510"></a><a name="p15838814510"></a>dst输出（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20.79%" id="mcps1.2.7.1.6"><p id="p258316816457"><a name="p258316816457"></a><a name="p258316816457"></a>dst输出（饱和模式，仅输出为half数据类型时支持）</p>
</th>
</tr>
</thead>
<tbody><tr id="row258378124516"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p185830814454"><a name="p185830814454"></a><a name="p185830814454"></a>+0/+0</p>
<p id="p1858358184516"><a name="p1858358184516"></a><a name="p1858358184516"></a>-0/-0</p>
<p id="p195839817459"><a name="p195839817459"></a><a name="p195839817459"></a>+0/MAX</p>
<p id="p358310874514"><a name="p358310874514"></a><a name="p358310874514"></a>-0/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p1558318824519"><a name="p1558318824519"></a><a name="p1558318824519"></a>+0</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p058458104513"><a name="p058458104513"></a><a name="p058458104513"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p358414884514"><a name="p358414884514"></a><a name="p358414884514"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p4584481458"><a name="p4584481458"></a><a name="p4584481458"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p13584178174511"><a name="p13584178174511"></a><a name="p13584178174511"></a>+0</p>
</td>
</tr>
<tr id="row758448174517"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p358414818457"><a name="p358414818457"></a><a name="p358414818457"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p25849818459"><a name="p25849818459"></a><a name="p25849818459"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3584284459"><a name="p3584284459"></a><a name="p3584284459"></a>-0</p>
</td>
</tr>
<tr id="row9584148194515"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1758438154519"><a name="p1758438154519"></a><a name="p1758438154519"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1058414814458"><a name="p1058414814458"></a><a name="p1058414814458"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p185851282458"><a name="p185851282458"></a><a name="p185851282458"></a>MAX</p>
</td>
</tr>
<tr id="row1458528134516"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p15851786456"><a name="p15851786456"></a><a name="p15851786456"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p65851883456"><a name="p65851883456"></a><a name="p65851883456"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1958515814452"><a name="p1958515814452"></a><a name="p1958515814452"></a>MIN</p>
</td>
</tr>
<tr id="row1458517874518"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p4585118174517"><a name="p4585118174517"></a><a name="p4585118174517"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17585108114513"><a name="p17585108114513"></a><a name="p17585108114513"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p9585128154518"><a name="p9585128154518"></a><a name="p9585128154518"></a>MAX</p>
</td>
</tr>
<tr id="row25851080457"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17585188164512"><a name="p17585188164512"></a><a name="p17585188164512"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p125859834510"><a name="p125859834510"></a><a name="p125859834510"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3586481456"><a name="p3586481456"></a><a name="p3586481456"></a>MIN</p>
</td>
</tr>
<tr id="row45861487453"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p125860834512"><a name="p125860834512"></a><a name="p125860834512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p058613813454"><a name="p058613813454"></a><a name="p058613813454"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p205861787456"><a name="p205861787456"></a><a name="p205861787456"></a>+0</p>
</td>
</tr>
<tr id="row4586884457"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p1858614815459"><a name="p1858614815459"></a><a name="p1858614815459"></a>+0/-0</p>
<p id="p458613818451"><a name="p458613818451"></a><a name="p458613818451"></a>+0/MIN</p>
<p id="p8586208114513"><a name="p8586208114513"></a><a name="p8586208114513"></a>-0/MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p258612834510"><a name="p258612834510"></a><a name="p258612834510"></a>-0</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p65869834515"><a name="p65869834515"></a><a name="p65869834515"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p1258678184510"><a name="p1258678184510"></a><a name="p1258678184510"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p145861682458"><a name="p145861682458"></a><a name="p145861682458"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p1558748154516"><a name="p1558748154516"></a><a name="p1558748154516"></a>+0</p>
</td>
</tr>
<tr id="row058788124512"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p258748154512"><a name="p258748154512"></a><a name="p258748154512"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1458713864520"><a name="p1458713864520"></a><a name="p1458713864520"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1358715864510"><a name="p1358715864510"></a><a name="p1358715864510"></a>-0</p>
</td>
</tr>
<tr id="row18587108144511"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17587888450"><a name="p17587888450"></a><a name="p17587888450"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p85871386454"><a name="p85871386454"></a><a name="p85871386454"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p205871587459"><a name="p205871587459"></a><a name="p205871587459"></a>+inf</p>
</td>
</tr>
<tr id="row18587108164515"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1858817864511"><a name="p1858817864511"></a><a name="p1858817864511"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p105884814516"><a name="p105884814516"></a><a name="p105884814516"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p0588887458"><a name="p0588887458"></a><a name="p0588887458"></a>-inf</p>
</td>
</tr>
<tr id="row17588138124518"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p155881385451"><a name="p155881385451"></a><a name="p155881385451"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1258814818458"><a name="p1258814818458"></a><a name="p1258814818458"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p75885818454"><a name="p75885818454"></a><a name="p75885818454"></a>MAX</p>
</td>
</tr>
<tr id="row858812894511"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1658820813450"><a name="p1658820813450"></a><a name="p1658820813450"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p13588281452"><a name="p13588281452"></a><a name="p13588281452"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p258810874518"><a name="p258810874518"></a><a name="p258810874518"></a>MIN</p>
</td>
</tr>
<tr id="row058810824514"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17589587455"><a name="p17589587455"></a><a name="p17589587455"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p185890854512"><a name="p185890854512"></a><a name="p185890854512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p858911816453"><a name="p858911816453"></a><a name="p858911816453"></a>+0</p>
</td>
</tr>
<tr id="row18589987458"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p165896816456"><a name="p165896816456"></a><a name="p165896816456"></a>+inf/+inf</p>
<p id="p5589386453"><a name="p5589386453"></a><a name="p5589386453"></a>+inf/MAX</p>
<p id="p65891814517"><a name="p65891814517"></a><a name="p65891814517"></a>-inf/MIN</p>
<p id="p358916810455"><a name="p358916810455"></a><a name="p358916810455"></a>MAX/MAX</p>
<p id="p1858988104517"><a name="p1858988104517"></a><a name="p1858988104517"></a>MIN/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p3589484455"><a name="p3589484455"></a><a name="p3589484455"></a>+inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p185906810455"><a name="p185906810455"></a><a name="p185906810455"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p115904819452"><a name="p115904819452"></a><a name="p115904819452"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p95907854514"><a name="p95907854514"></a><a name="p95907854514"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p35901081458"><a name="p35901081458"></a><a name="p35901081458"></a>MAX</p>
</td>
</tr>
<tr id="row145901284451"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p15590687458"><a name="p15590687458"></a><a name="p15590687458"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p135901085451"><a name="p135901085451"></a><a name="p135901085451"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1059012824511"><a name="p1059012824511"></a><a name="p1059012824511"></a>MAX</p>
</td>
</tr>
<tr id="row195902815455"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p18590168104516"><a name="p18590168104516"></a><a name="p18590168104516"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p959016844516"><a name="p959016844516"></a><a name="p959016844516"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p17590158124513"><a name="p17590158124513"></a><a name="p17590158124513"></a>MAX</p>
</td>
</tr>
<tr id="row35913819452"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1559112864512"><a name="p1559112864512"></a><a name="p1559112864512"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p359138154517"><a name="p359138154517"></a><a name="p359138154517"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p145917815451"><a name="p145917815451"></a><a name="p145917815451"></a>MIN</p>
</td>
</tr>
<tr id="row65917814455"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p559111813453"><a name="p559111813453"></a><a name="p559111813453"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p15591682455"><a name="p15591682455"></a><a name="p15591682455"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1959110894511"><a name="p1959110894511"></a><a name="p1959110894511"></a>MAX</p>
</td>
</tr>
<tr id="row125911819455"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p10591158184512"><a name="p10591158184512"></a><a name="p10591158184512"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p15591287456"><a name="p15591287456"></a><a name="p15591287456"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1959213854510"><a name="p1959213854510"></a><a name="p1959213854510"></a>+0</p>
</td>
</tr>
<tr id="row135926818459"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p95928834513"><a name="p95928834513"></a><a name="p95928834513"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p4592983453"><a name="p4592983453"></a><a name="p4592983453"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p55921385457"><a name="p55921385457"></a><a name="p55921385457"></a>+0</p>
</td>
</tr>
<tr id="row45921282452"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p25921686453"><a name="p25921686453"></a><a name="p25921686453"></a>+inf/-inf</p>
<p id="p55929844518"><a name="p55929844518"></a><a name="p55929844518"></a>+inf/MIN</p>
<p id="p7592158204519"><a name="p7592158204519"></a><a name="p7592158204519"></a>-inf/MAX</p>
<p id="p1059218854515"><a name="p1059218854515"></a><a name="p1059218854515"></a>MAX/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p559368194520"><a name="p559368194520"></a><a name="p559368194520"></a>-inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p1159313813454"><a name="p1159313813454"></a><a name="p1159313813454"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p13593484456"><a name="p13593484456"></a><a name="p13593484456"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p259318854512"><a name="p259318854512"></a><a name="p259318854512"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p15931886457"><a name="p15931886457"></a><a name="p15931886457"></a>MIN</p>
</td>
</tr>
<tr id="row185931787457"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1959319854513"><a name="p1959319854513"></a><a name="p1959319854513"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p7593288454"><a name="p7593288454"></a><a name="p7593288454"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p859317819454"><a name="p859317819454"></a><a name="p859317819454"></a>MIN</p>
</td>
</tr>
<tr id="row19593488457"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p175932844519"><a name="p175932844519"></a><a name="p175932844519"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1259415814458"><a name="p1259415814458"></a><a name="p1259415814458"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p19594881456"><a name="p19594881456"></a><a name="p19594881456"></a>MAX</p>
</td>
</tr>
<tr id="row459488144517"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p19594484453"><a name="p19594484453"></a><a name="p19594484453"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p459412814513"><a name="p459412814513"></a><a name="p459412814513"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p259418204519"><a name="p259418204519"></a><a name="p259418204519"></a>MIN</p>
</td>
</tr>
<tr id="row459420817455"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p259416813458"><a name="p259416813458"></a><a name="p259416813458"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1459416812452"><a name="p1459416812452"></a><a name="p1459416812452"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p165942874520"><a name="p165942874520"></a><a name="p165942874520"></a>+0</p>
</td>
</tr>
<tr id="row4594883459"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p65949818455"><a name="p65949818455"></a><a name="p65949818455"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p359513813455"><a name="p359513813455"></a><a name="p359513813455"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p205951881458"><a name="p205951881458"></a><a name="p205951881458"></a>MIN</p>
</td>
</tr>
<tr id="row75951188452"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p145952844514"><a name="p145952844514"></a><a name="p145952844514"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p115955894512"><a name="p115955894512"></a><a name="p115955894512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p16595168154515"><a name="p16595168154515"></a><a name="p16595168154515"></a>+0</p>
</td>
</tr>
<tr id="row65959810457"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p195951481454"><a name="p195951481454"></a><a name="p195951481454"></a>使得src0 * dst = MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p1659511810450"><a name="p1659511810450"></a><a name="p1659511810450"></a>MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p1059518818452"><a name="p1059518818452"></a><a name="p1059518818452"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p459519811451"><a name="p459519811451"></a><a name="p459519811451"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p12596581456"><a name="p12596581456"></a><a name="p12596581456"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p459611817455"><a name="p459611817455"></a><a name="p459611817455"></a>MAX</p>
</td>
</tr>
<tr id="row259611894513"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p16596208154518"><a name="p16596208154518"></a><a name="p16596208154518"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p185967874510"><a name="p185967874510"></a><a name="p185967874510"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1559618814454"><a name="p1559618814454"></a><a name="p1559618814454"></a>MAX</p>
</td>
</tr>
<tr id="row1559615813454"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p65961483453"><a name="p65961483453"></a><a name="p65961483453"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14596780452"><a name="p14596780452"></a><a name="p14596780452"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p8596383453"><a name="p8596383453"></a><a name="p8596383453"></a>MAX</p>
</td>
</tr>
<tr id="row12596138154512"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p175967804520"><a name="p175967804520"></a><a name="p175967804520"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1159715824518"><a name="p1159715824518"></a><a name="p1159715824518"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p145971884510"><a name="p145971884510"></a><a name="p145971884510"></a>MIN</p>
</td>
</tr>
<tr id="row55975854517"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p559720814459"><a name="p559720814459"></a><a name="p559720814459"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p25971854519"><a name="p25971854519"></a><a name="p25971854519"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p115974804519"><a name="p115974804519"></a><a name="p115974804519"></a>MAX</p>
</td>
</tr>
<tr id="row8597198144512"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p195975819459"><a name="p195975819459"></a><a name="p195975819459"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1759710814512"><a name="p1759710814512"></a><a name="p1759710814512"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p175971685456"><a name="p175971685456"></a><a name="p175971685456"></a>+0</p>
</td>
</tr>
<tr id="row259714812458"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p185978810451"><a name="p185978810451"></a><a name="p185978810451"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p859888164518"><a name="p859888164518"></a><a name="p859888164518"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p159815815452"><a name="p159815815452"></a><a name="p159815815452"></a>+0</p>
</td>
</tr>
<tr id="row1259878154515"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p105983864514"><a name="p105983864514"></a><a name="p105983864514"></a>使得src0 * dst = MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p5598198194516"><a name="p5598198194516"></a><a name="p5598198194516"></a>MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p259811834512"><a name="p259811834512"></a><a name="p259811834512"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p1159817814513"><a name="p1159817814513"></a><a name="p1159817814513"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p959828124516"><a name="p959828124516"></a><a name="p959828124516"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p14598181451"><a name="p14598181451"></a><a name="p14598181451"></a>MIN</p>
</td>
</tr>
<tr id="row195981183453"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p359811844516"><a name="p359811844516"></a><a name="p359811844516"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p559915818453"><a name="p559915818453"></a><a name="p559915818453"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p359918815453"><a name="p359918815453"></a><a name="p359918815453"></a>MIN</p>
</td>
</tr>
<tr id="row165993844516"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1959911844512"><a name="p1959911844512"></a><a name="p1959911844512"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p659913815455"><a name="p659913815455"></a><a name="p659913815455"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p0599188164517"><a name="p0599188164517"></a><a name="p0599188164517"></a>MAX</p>
</td>
</tr>
<tr id="row6599188114519"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p14599389458"><a name="p14599389458"></a><a name="p14599389458"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1959920844513"><a name="p1959920844513"></a><a name="p1959920844513"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p8599785456"><a name="p8599785456"></a><a name="p8599785456"></a>MIN</p>
</td>
</tr>
<tr id="row559978164512"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p95991687458"><a name="p95991687458"></a><a name="p95991687458"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p19600178134520"><a name="p19600178134520"></a><a name="p19600178134520"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p560019894514"><a name="p560019894514"></a><a name="p560019894514"></a>+0</p>
</td>
</tr>
<tr id="row176000844512"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p2060028194515"><a name="p2060028194515"></a><a name="p2060028194515"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p136000814458"><a name="p136000814458"></a><a name="p136000814458"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3600108104510"><a name="p3600108104510"></a><a name="p3600108104510"></a>MIN</p>
</td>
</tr>
<tr id="row2060017815453"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1860048194514"><a name="p1860048194514"></a><a name="p1860048194514"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1960016844513"><a name="p1960016844513"></a><a name="p1960016844513"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p36001817458"><a name="p36001817458"></a><a name="p36001817458"></a>+0</p>
</td>
</tr>
<tr id="row20600148144510"><td class="cellrowborder" rowspan="7" valign="top" width="22.220000000000002%" headers="mcps1.2.7.1.1 "><p id="p960111814510"><a name="p960111814510"></a><a name="p960111814510"></a>nan/任意值</p>
<p id="p26011686457"><a name="p26011686457"></a><a name="p26011686457"></a>+0/+inf</p>
<p id="p560110818453"><a name="p560110818453"></a><a name="p560110818453"></a>+0/-inf</p>
<p id="p1460117814514"><a name="p1460117814514"></a><a name="p1460117814514"></a>-0/+inf</p>
<p id="p2060168124516"><a name="p2060168124516"></a><a name="p2060168124516"></a>-0/-inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="18.17%" headers="mcps1.2.7.1.2 "><p id="p360117816452"><a name="p360117816452"></a><a name="p360117816452"></a>nan</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="15.809999999999999%" headers="mcps1.2.7.1.3 "><p id="p1260111804517"><a name="p1260111804517"></a><a name="p1260111804517"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="8.77%" headers="mcps1.2.7.1.4 "><p id="p660119820459"><a name="p660119820459"></a><a name="p660119820459"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.7.1.5 "><p id="p1601182453"><a name="p1601182453"></a><a name="p1601182453"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="20.79%" headers="mcps1.2.7.1.6 "><p id="p6601118174516"><a name="p6601118174516"></a><a name="p6601118174516"></a>+0</p>
</td>
</tr>
<tr id="row126018814514"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1360117834520"><a name="p1360117834520"></a><a name="p1360117834520"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p11602888455"><a name="p11602888455"></a><a name="p11602888455"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p186028817453"><a name="p186028817453"></a><a name="p186028817453"></a>+0</p>
</td>
</tr>
<tr id="row1760219884513"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p36020844520"><a name="p36020844520"></a><a name="p36020844520"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p8602887452"><a name="p8602887452"></a><a name="p8602887452"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p116025813455"><a name="p116025813455"></a><a name="p116025813455"></a>MAX</p>
</td>
</tr>
<tr id="row10602486456"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p960218104520"><a name="p960218104520"></a><a name="p960218104520"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p20602198194512"><a name="p20602198194512"></a><a name="p20602198194512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1560212818453"><a name="p1560212818453"></a><a name="p1560212818453"></a>MIN</p>
</td>
</tr>
<tr id="row1860214817453"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1602208104520"><a name="p1602208104520"></a><a name="p1602208104520"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p06031834518"><a name="p06031834518"></a><a name="p06031834518"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p10603283459"><a name="p10603283459"></a><a name="p10603283459"></a>MAX</p>
</td>
</tr>
<tr id="row196038864511"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p96037812452"><a name="p96037812452"></a><a name="p96037812452"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p126031488451"><a name="p126031488451"></a><a name="p126031488451"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p760368134518"><a name="p760368134518"></a><a name="p760368134518"></a>MIN</p>
</td>
</tr>
<tr id="row146038834512"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p960319818455"><a name="p960319818455"></a><a name="p960319818455"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14603683456"><a name="p14603683456"></a><a name="p14603683456"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p15603158114510"><a name="p15603158114510"></a><a name="p15603158114510"></a>+0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 23**  MulAddRelu特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table126194714473"></a>
<table><thead align="left"><tr id="row1861747174714"><th class="cellrowborder" valign="top" width="12.34%" id="mcps1.2.9.1.1"><p id="p4611247184717"><a name="p4611247184717"></a><a name="p4611247184717"></a>src0取值/dst取值</p>
</th>
<th class="cellrowborder" valign="top" width="10.36%" id="mcps1.2.9.1.2"><p id="p136112478479"><a name="p136112478479"></a><a name="p136112478479"></a>中间阶段1(src0 * dst)</p>
</th>
<th class="cellrowborder" valign="top" width="12.27%" id="mcps1.2.9.1.3"><p id="p1662174718474"><a name="p1662174718474"></a><a name="p1662174718474"></a>中间阶段1（饱和模式，仅输出为half数据类型时支持）</p>
</th>
<th class="cellrowborder" valign="top" width="6.39%" id="mcps1.2.9.1.4"><p id="p12621147114715"><a name="p12621147114715"></a><a name="p12621147114715"></a>src1取值</p>
</th>
<th class="cellrowborder" valign="top" width="12.42%" id="mcps1.2.9.1.5"><p id="p2622479476"><a name="p2622479476"></a><a name="p2622479476"></a>中间阶段2（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="15.4%" id="mcps1.2.9.1.6"><p id="p126224734713"><a name="p126224734713"></a><a name="p126224734713"></a>中间阶段2（饱和模式，仅输出为half数据类型时支持）</p>
</th>
<th class="cellrowborder" valign="top" width="17.84%" id="mcps1.2.9.1.7"><p id="p3572359394"><a name="p3572359394"></a><a name="p3572359394"></a>dst输出（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="12.98%" id="mcps1.2.9.1.8"><p id="p204441524191718"><a name="p204441524191718"></a><a name="p204441524191718"></a>dst输出（饱和模式，仅输出为half数据类型时支持）</p>
</th>
</tr>
</thead>
<tbody><tr id="row186213473474"><td class="cellrowborder" rowspan="7" valign="top" width="12.34%" headers="mcps1.2.9.1.1 "><p id="p15621847164710"><a name="p15621847164710"></a><a name="p15621847164710"></a>+0/+0</p>
<p id="p262124710474"><a name="p262124710474"></a><a name="p262124710474"></a>-0/-0</p>
<p id="p4621647134719"><a name="p4621647134719"></a><a name="p4621647134719"></a>+0/MAX</p>
<p id="p116212477475"><a name="p116212477475"></a><a name="p116212477475"></a>-0/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="10.36%" headers="mcps1.2.9.1.2 "><p id="p15623476477"><a name="p15623476477"></a><a name="p15623476477"></a>+0</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="12.27%" headers="mcps1.2.9.1.3 "><p id="p18635474473"><a name="p18635474473"></a><a name="p18635474473"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="6.39%" headers="mcps1.2.9.1.4 "><p id="p20631847154719"><a name="p20631847154719"></a><a name="p20631847154719"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.42%" headers="mcps1.2.9.1.5 "><p id="p20631474479"><a name="p20631474479"></a><a name="p20631474479"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="15.4%" headers="mcps1.2.9.1.6 "><p id="p263164794719"><a name="p263164794719"></a><a name="p263164794719"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="17.84%" headers="mcps1.2.9.1.7 "><p id="p857165910913"><a name="p857165910913"></a><a name="p857165910913"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.98%" headers="mcps1.2.9.1.8 "><p id="p744412413171"><a name="p744412413171"></a><a name="p744412413171"></a>+0</p>
</td>
</tr>
<tr id="row06324714712"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p8631347124712"><a name="p8631347124712"></a><a name="p8631347124712"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1763184720478"><a name="p1763184720478"></a><a name="p1763184720478"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p116314475477"><a name="p116314475477"></a><a name="p116314475477"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p55715592910"><a name="p55715592910"></a><a name="p55715592910"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p19410132217223"><a name="p19410132217223"></a><a name="p19410132217223"></a>+0</p>
</td>
</tr>
<tr id="row6630475473"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p106384715471"><a name="p106384715471"></a><a name="p106384715471"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1463194784715"><a name="p1463194784715"></a><a name="p1463194784715"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p16484715477"><a name="p16484715477"></a><a name="p16484715477"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p257114591892"><a name="p257114591892"></a><a name="p257114591892"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p64441424181712"><a name="p64441424181712"></a><a name="p64441424181712"></a>MAX</p>
</td>
</tr>
<tr id="row664174794712"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p36414476473"><a name="p36414476473"></a><a name="p36414476473"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1464194719475"><a name="p1464194719475"></a><a name="p1464194719475"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p186416473478"><a name="p186416473478"></a><a name="p186416473478"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p124646482208"><a name="p124646482208"></a><a name="p124646482208"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1844472411178"><a name="p1844472411178"></a><a name="p1844472411178"></a>+0</p>
</td>
</tr>
<tr id="row7644473470"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p196416472478"><a name="p196416472478"></a><a name="p196416472478"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1564104717477"><a name="p1564104717477"></a><a name="p1564104717477"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p16646475479"><a name="p16646475479"></a><a name="p16646475479"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p387825010203"><a name="p387825010203"></a><a name="p387825010203"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p12444824191713"><a name="p12444824191713"></a><a name="p12444824191713"></a>MAX</p>
</td>
</tr>
<tr id="row76414794718"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p76534724713"><a name="p76534724713"></a><a name="p76534724713"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p186554716478"><a name="p186554716478"></a><a name="p186554716478"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p1965194712472"><a name="p1965194712472"></a><a name="p1965194712472"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p1387715362019"><a name="p1387715362019"></a><a name="p1387715362019"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p74441724121716"><a name="p74441724121716"></a><a name="p74441724121716"></a>+0</p>
</td>
</tr>
<tr id="row186564774717"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p4651047134717"><a name="p4651047134717"></a><a name="p4651047134717"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p465164764710"><a name="p465164764710"></a><a name="p465164764710"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p136524710474"><a name="p136524710474"></a><a name="p136524710474"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p1756919591691"><a name="p1756919591691"></a><a name="p1756919591691"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p3444424111715"><a name="p3444424111715"></a><a name="p3444424111715"></a>+0</p>
</td>
</tr>
<tr id="row1365104704714"><td class="cellrowborder" rowspan="7" valign="top" width="12.34%" headers="mcps1.2.9.1.1 "><p id="p1565174715475"><a name="p1565174715475"></a><a name="p1565174715475"></a>+0/-0</p>
<p id="p3655474475"><a name="p3655474475"></a><a name="p3655474475"></a>+0/MIN</p>
<p id="p11651747194711"><a name="p11651747194711"></a><a name="p11651747194711"></a>-0/MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="10.36%" headers="mcps1.2.9.1.2 "><p id="p465947174714"><a name="p465947174714"></a><a name="p465947174714"></a>-0</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="12.27%" headers="mcps1.2.9.1.3 "><p id="p2665473471"><a name="p2665473471"></a><a name="p2665473471"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" width="6.39%" headers="mcps1.2.9.1.4 "><p id="p566347144712"><a name="p566347144712"></a><a name="p566347144712"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.42%" headers="mcps1.2.9.1.5 "><p id="p106664716478"><a name="p106664716478"></a><a name="p106664716478"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="15.4%" headers="mcps1.2.9.1.6 "><p id="p96624719476"><a name="p96624719476"></a><a name="p96624719476"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="17.84%" headers="mcps1.2.9.1.7 "><p id="p556910591691"><a name="p556910591691"></a><a name="p556910591691"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.98%" headers="mcps1.2.9.1.8 "><p id="p1444122419171"><a name="p1444122419171"></a><a name="p1444122419171"></a>+0</p>
</td>
</tr>
<tr id="row466144717479"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1266144744713"><a name="p1266144744713"></a><a name="p1266144744713"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p16614471476"><a name="p16614471476"></a><a name="p16614471476"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p17661471477"><a name="p17661471477"></a><a name="p17661471477"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p656818592913"><a name="p656818592913"></a><a name="p656818592913"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p844411243178"><a name="p844411243178"></a><a name="p844411243178"></a>+0</p>
</td>
</tr>
<tr id="row86664719472"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1066194754717"><a name="p1066194754717"></a><a name="p1066194754717"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p5661347204711"><a name="p5661347204711"></a><a name="p5661347204711"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p1067647174714"><a name="p1067647174714"></a><a name="p1067647174714"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p799411512255"><a name="p799411512255"></a><a name="p799411512255"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1244472412178"><a name="p1244472412178"></a><a name="p1244472412178"></a>MAX</p>
</td>
</tr>
<tr id="row176724724711"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1567347164712"><a name="p1567347164712"></a><a name="p1567347164712"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p2067184754716"><a name="p2067184754716"></a><a name="p2067184754716"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p167114711476"><a name="p167114711476"></a><a name="p167114711476"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p175682591498"><a name="p175682591498"></a><a name="p175682591498"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p94451924151712"><a name="p94451924151712"></a><a name="p94451924151712"></a>+0</p>
</td>
</tr>
<tr id="row2671147184715"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p6673474471"><a name="p6673474471"></a><a name="p6673474471"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p176764717476"><a name="p176764717476"></a><a name="p176764717476"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p126711478478"><a name="p126711478478"></a><a name="p126711478478"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p1756711597917"><a name="p1756711597917"></a><a name="p1756711597917"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p844552421718"><a name="p844552421718"></a><a name="p844552421718"></a>MAX</p>
</td>
</tr>
<tr id="row867194713470"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p146764718476"><a name="p146764718476"></a><a name="p146764718476"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p136844744710"><a name="p136844744710"></a><a name="p136844744710"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p1868154716473"><a name="p1868154716473"></a><a name="p1868154716473"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p7567175919910"><a name="p7567175919910"></a><a name="p7567175919910"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1445182431719"><a name="p1445182431719"></a><a name="p1445182431719"></a>+0</p>
</td>
</tr>
<tr id="row16816473479"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1268134754710"><a name="p1268134754710"></a><a name="p1268134754710"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1681147194712"><a name="p1681147194712"></a><a name="p1681147194712"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p86810475478"><a name="p86810475478"></a><a name="p86810475478"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p25671959197"><a name="p25671959197"></a><a name="p25671959197"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p14445324141710"><a name="p14445324141710"></a><a name="p14445324141710"></a>+0</p>
</td>
</tr>
<tr id="row106804719472"><td class="cellrowborder" rowspan="7" valign="top" width="12.34%" headers="mcps1.2.9.1.1 "><p id="p16819471474"><a name="p16819471474"></a><a name="p16819471474"></a>+inf/+inf</p>
<p id="p568247164712"><a name="p568247164712"></a><a name="p568247164712"></a>+inf/MAX</p>
<p id="p56816478476"><a name="p56816478476"></a><a name="p56816478476"></a>-inf/MIN</p>
<p id="p136884734719"><a name="p136884734719"></a><a name="p136884734719"></a>MAX/MAX</p>
<p id="p1168184774710"><a name="p1168184774710"></a><a name="p1168184774710"></a>MIN/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="10.36%" headers="mcps1.2.9.1.2 "><p id="p56864710470"><a name="p56864710470"></a><a name="p56864710470"></a>+inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="12.27%" headers="mcps1.2.9.1.3 "><p id="p1569147194710"><a name="p1569147194710"></a><a name="p1569147194710"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="6.39%" headers="mcps1.2.9.1.4 "><p id="p4698470479"><a name="p4698470479"></a><a name="p4698470479"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.42%" headers="mcps1.2.9.1.5 "><p id="p16697472473"><a name="p16697472473"></a><a name="p16697472473"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" width="15.4%" headers="mcps1.2.9.1.6 "><p id="p8693478474"><a name="p8693478474"></a><a name="p8693478474"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="17.84%" headers="mcps1.2.9.1.7 "><p id="p1056619592911"><a name="p1056619592911"></a><a name="p1056619592911"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" width="12.98%" headers="mcps1.2.9.1.8 "><p id="p7445202441711"><a name="p7445202441711"></a><a name="p7445202441711"></a>MAX</p>
</td>
</tr>
<tr id="row869194718477"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1269747124718"><a name="p1269747124718"></a><a name="p1269747124718"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p18699474478"><a name="p18699474478"></a><a name="p18699474478"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p116954754719"><a name="p116954754719"></a><a name="p116954754719"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p1556619591198"><a name="p1556619591198"></a><a name="p1556619591198"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1744532415175"><a name="p1744532415175"></a><a name="p1744532415175"></a>MAX</p>
</td>
</tr>
<tr id="row1069154716475"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p970194754712"><a name="p970194754712"></a><a name="p970194754712"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p87084716477"><a name="p87084716477"></a><a name="p87084716477"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p37084714478"><a name="p37084714478"></a><a name="p37084714478"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p056611596912"><a name="p056611596912"></a><a name="p056611596912"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p5445324141715"><a name="p5445324141715"></a><a name="p5445324141715"></a>MAX</p>
</td>
</tr>
<tr id="row67019477478"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1670194719471"><a name="p1670194719471"></a><a name="p1670194719471"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p47044714478"><a name="p47044714478"></a><a name="p47044714478"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p770164714711"><a name="p770164714711"></a><a name="p770164714711"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p656513598910"><a name="p656513598910"></a><a name="p656513598910"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p6524917193311"><a name="p6524917193311"></a><a name="p6524917193311"></a>+0</p>
</td>
</tr>
<tr id="row170134718473"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1770747164718"><a name="p1770747164718"></a><a name="p1770747164718"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p14701147174720"><a name="p14701147174720"></a><a name="p14701147174720"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p177164754717"><a name="p177164754717"></a><a name="p177164754717"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p15565359199"><a name="p15565359199"></a><a name="p15565359199"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p19445624121713"><a name="p19445624121713"></a><a name="p19445624121713"></a>MAX</p>
</td>
</tr>
<tr id="row167119473471"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p11713479472"><a name="p11713479472"></a><a name="p11713479472"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p14711347204711"><a name="p14711347204711"></a><a name="p14711347204711"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p107114713471"><a name="p107114713471"></a><a name="p107114713471"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p556513597920"><a name="p556513597920"></a><a name="p556513597920"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p144562461715"><a name="p144562461715"></a><a name="p144562461715"></a>+0</p>
</td>
</tr>
<tr id="row137116475474"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p77184712475"><a name="p77184712475"></a><a name="p77184712475"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p47194754712"><a name="p47194754712"></a><a name="p47194754712"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p37164734718"><a name="p37164734718"></a><a name="p37164734718"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p8564175914912"><a name="p8564175914912"></a><a name="p8564175914912"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1944510245178"><a name="p1944510245178"></a><a name="p1944510245178"></a>+0</p>
</td>
</tr>
<tr id="row1971174714715"><td class="cellrowborder" rowspan="7" valign="top" width="12.34%" headers="mcps1.2.9.1.1 "><p id="p1972104715473"><a name="p1972104715473"></a><a name="p1972104715473"></a>+inf/-inf</p>
<p id="p77220473474"><a name="p77220473474"></a><a name="p77220473474"></a>+inf/MIN</p>
<p id="p672114744718"><a name="p672114744718"></a><a name="p672114744718"></a>-inf/MAX</p>
<p id="p6721647104717"><a name="p6721647104717"></a><a name="p6721647104717"></a>MAX/MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="10.36%" headers="mcps1.2.9.1.2 "><p id="p672204754713"><a name="p672204754713"></a><a name="p672204754713"></a>-inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="12.27%" headers="mcps1.2.9.1.3 "><p id="p572194754711"><a name="p572194754711"></a><a name="p572194754711"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="6.39%" headers="mcps1.2.9.1.4 "><p id="p1472184711475"><a name="p1472184711475"></a><a name="p1472184711475"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.42%" headers="mcps1.2.9.1.5 "><p id="p1272104712470"><a name="p1272104712470"></a><a name="p1272104712470"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="15.4%" headers="mcps1.2.9.1.6 "><p id="p167284784716"><a name="p167284784716"></a><a name="p167284784716"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="17.84%" headers="mcps1.2.9.1.7 "><p id="p195641597919"><a name="p195641597919"></a><a name="p195641597919"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.98%" headers="mcps1.2.9.1.8 "><p id="p154451624151715"><a name="p154451624151715"></a><a name="p154451624151715"></a>+0</p>
</td>
</tr>
<tr id="row2721347164711"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p872194744714"><a name="p872194744714"></a><a name="p872194744714"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p10731647194712"><a name="p10731647194712"></a><a name="p10731647194712"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p073164710474"><a name="p073164710474"></a><a name="p073164710474"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p6564115917914"><a name="p6564115917914"></a><a name="p6564115917914"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p12445924101713"><a name="p12445924101713"></a><a name="p12445924101713"></a>+0</p>
</td>
</tr>
<tr id="row1773154718478"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p273114754720"><a name="p273114754720"></a><a name="p273114754720"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p10731647194716"><a name="p10731647194716"></a><a name="p10731647194716"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p1773144720474"><a name="p1773144720474"></a><a name="p1773144720474"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p155631559398"><a name="p155631559398"></a><a name="p155631559398"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1644562411178"><a name="p1644562411178"></a><a name="p1644562411178"></a>MAX</p>
</td>
</tr>
<tr id="row47314774717"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p27364718476"><a name="p27364718476"></a><a name="p27364718476"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p673144714476"><a name="p673144714476"></a><a name="p673144714476"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p11731747154712"><a name="p11731747154712"></a><a name="p11731747154712"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p1256314591892"><a name="p1256314591892"></a><a name="p1256314591892"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p9445202413171"><a name="p9445202413171"></a><a name="p9445202413171"></a>+0</p>
</td>
</tr>
<tr id="row173154764717"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p137494710478"><a name="p137494710478"></a><a name="p137494710478"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1774134714710"><a name="p1774134714710"></a><a name="p1774134714710"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p774124716474"><a name="p774124716474"></a><a name="p774124716474"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p115631591914"><a name="p115631591914"></a><a name="p115631591914"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p64451424141714"><a name="p64451424141714"></a><a name="p64451424141714"></a>+0</p>
</td>
</tr>
<tr id="row87444704716"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p97412478475"><a name="p97412478475"></a><a name="p97412478475"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p13749478475"><a name="p13749478475"></a><a name="p13749478475"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p197434717474"><a name="p197434717474"></a><a name="p197434717474"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p75621759298"><a name="p75621759298"></a><a name="p75621759298"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p17445624151710"><a name="p17445624151710"></a><a name="p17445624151710"></a>+0</p>
</td>
</tr>
<tr id="row2741547174717"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p117484794713"><a name="p117484794713"></a><a name="p117484794713"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1174147164719"><a name="p1174147164719"></a><a name="p1174147164719"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p175124714473"><a name="p175124714473"></a><a name="p175124714473"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p115629591691"><a name="p115629591691"></a><a name="p115629591691"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p16445824121712"><a name="p16445824121712"></a><a name="p16445824121712"></a>+0</p>
</td>
</tr>
<tr id="row375134734718"><td class="cellrowborder" rowspan="7" valign="top" width="12.34%" headers="mcps1.2.9.1.1 "><p id="p157504714471"><a name="p157504714471"></a><a name="p157504714471"></a>使得src0 * dst = MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="10.36%" headers="mcps1.2.9.1.2 "><p id="p3751647154718"><a name="p3751647154718"></a><a name="p3751647154718"></a>MAX</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="12.27%" headers="mcps1.2.9.1.3 "><p id="p475174764710"><a name="p475174764710"></a><a name="p475174764710"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="6.39%" headers="mcps1.2.9.1.4 "><p id="p19751547104711"><a name="p19751547104711"></a><a name="p19751547104711"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.42%" headers="mcps1.2.9.1.5 "><p id="p12751247114711"><a name="p12751247114711"></a><a name="p12751247114711"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="15.4%" headers="mcps1.2.9.1.6 "><p id="p13751547184715"><a name="p13751547184715"></a><a name="p13751547184715"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="17.84%" headers="mcps1.2.9.1.7 "><p id="p12562145911911"><a name="p12562145911911"></a><a name="p12562145911911"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="12.98%" headers="mcps1.2.9.1.8 "><p id="p4445112418171"><a name="p4445112418171"></a><a name="p4445112418171"></a>MAX</p>
</td>
</tr>
<tr id="row207510477477"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p187612471477"><a name="p187612471477"></a><a name="p187612471477"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p276174734712"><a name="p276174734712"></a><a name="p276174734712"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p1576134764712"><a name="p1576134764712"></a><a name="p1576134764712"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p25611959695"><a name="p25611959695"></a><a name="p25611959695"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1044542411715"><a name="p1044542411715"></a><a name="p1044542411715"></a>MAX</p>
</td>
</tr>
<tr id="row576104744710"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1276184714475"><a name="p1276184714475"></a><a name="p1276184714475"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p876947114718"><a name="p876947114718"></a><a name="p876947114718"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p976174714716"><a name="p976174714716"></a><a name="p976174714716"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p85611459396"><a name="p85611459396"></a><a name="p85611459396"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p174458243176"><a name="p174458243176"></a><a name="p174458243176"></a>MAX</p>
</td>
</tr>
<tr id="row18761947104714"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p15761647164716"><a name="p15761647164716"></a><a name="p15761647164716"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p676194774720"><a name="p676194774720"></a><a name="p676194774720"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p197615471475"><a name="p197615471475"></a><a name="p197615471475"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p10561759891"><a name="p10561759891"></a><a name="p10561759891"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1444514246173"><a name="p1444514246173"></a><a name="p1444514246173"></a>+0</p>
</td>
</tr>
<tr id="row1077947154717"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p377747154713"><a name="p377747154713"></a><a name="p377747154713"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p177704714713"><a name="p177704714713"></a><a name="p177704714713"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p18771047104714"><a name="p18771047104714"></a><a name="p18771047104714"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p65602591199"><a name="p65602591199"></a><a name="p65602591199"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p94451924161712"><a name="p94451924161712"></a><a name="p94451924161712"></a>MAX</p>
</td>
</tr>
<tr id="row1377154724715"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p37724712476"><a name="p37724712476"></a><a name="p37724712476"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p11771472474"><a name="p11771472474"></a><a name="p11771472474"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p1977947174711"><a name="p1977947174711"></a><a name="p1977947174711"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p056020591192"><a name="p056020591192"></a><a name="p056020591192"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p044519241179"><a name="p044519241179"></a><a name="p044519241179"></a>+0</p>
</td>
</tr>
<tr id="row15771347154717"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p377154716475"><a name="p377154716475"></a><a name="p377154716475"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1578154784717"><a name="p1578154784717"></a><a name="p1578154784717"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p27814784713"><a name="p27814784713"></a><a name="p27814784713"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p1256085911912"><a name="p1256085911912"></a><a name="p1256085911912"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p744552415174"><a name="p744552415174"></a><a name="p744552415174"></a>+0</p>
</td>
</tr>
<tr id="row207811470475"><td class="cellrowborder" rowspan="7" valign="top" width="12.34%" headers="mcps1.2.9.1.1 "><p id="p078347154710"><a name="p078347154710"></a><a name="p078347154710"></a>使得src0 * dst = MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="10.36%" headers="mcps1.2.9.1.2 "><p id="p37814774715"><a name="p37814774715"></a><a name="p37814774715"></a>MIN</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="12.27%" headers="mcps1.2.9.1.3 "><p id="p197824744710"><a name="p197824744710"></a><a name="p197824744710"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="6.39%" headers="mcps1.2.9.1.4 "><p id="p6785473478"><a name="p6785473478"></a><a name="p6785473478"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.42%" headers="mcps1.2.9.1.5 "><p id="p1578747124710"><a name="p1578747124710"></a><a name="p1578747124710"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="15.4%" headers="mcps1.2.9.1.6 "><p id="p378547114712"><a name="p378547114712"></a><a name="p378547114712"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="17.84%" headers="mcps1.2.9.1.7 "><p id="p65598596917"><a name="p65598596917"></a><a name="p65598596917"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.98%" headers="mcps1.2.9.1.8 "><p id="p1044592421717"><a name="p1044592421717"></a><a name="p1044592421717"></a>+0</p>
</td>
</tr>
<tr id="row1778144715476"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p12796474478"><a name="p12796474478"></a><a name="p12796474478"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p279124744710"><a name="p279124744710"></a><a name="p279124744710"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p107910472475"><a name="p107910472475"></a><a name="p107910472475"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p6559959291"><a name="p6559959291"></a><a name="p6559959291"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p164450244174"><a name="p164450244174"></a><a name="p164450244174"></a>+0</p>
</td>
</tr>
<tr id="row1879154764716"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p11791347194712"><a name="p11791347194712"></a><a name="p11791347194712"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p67914734713"><a name="p67914734713"></a><a name="p67914734713"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p57914717479"><a name="p57914717479"></a><a name="p57914717479"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p455917591892"><a name="p455917591892"></a><a name="p455917591892"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1244510241173"><a name="p1244510241173"></a><a name="p1244510241173"></a>MAX</p>
</td>
</tr>
<tr id="row4791947124712"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p1579547144713"><a name="p1579547144713"></a><a name="p1579547144713"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1879647114715"><a name="p1879647114715"></a><a name="p1879647114715"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p178010471478"><a name="p178010471478"></a><a name="p178010471478"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p1455818593916"><a name="p1455818593916"></a><a name="p1455818593916"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p104451524101712"><a name="p104451524101712"></a><a name="p104451524101712"></a>+0</p>
</td>
</tr>
<tr id="row18094714716"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p4808470479"><a name="p4808470479"></a><a name="p4808470479"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p78016472479"><a name="p78016472479"></a><a name="p78016472479"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p14801247154714"><a name="p14801247154714"></a><a name="p14801247154714"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p105583594917"><a name="p105583594917"></a><a name="p105583594917"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1445524101720"><a name="p1445524101720"></a><a name="p1445524101720"></a>+0</p>
</td>
</tr>
<tr id="row880347134712"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p28064716476"><a name="p28064716476"></a><a name="p28064716476"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p58034714475"><a name="p58034714475"></a><a name="p58034714475"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p118010477476"><a name="p118010477476"></a><a name="p118010477476"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p955817591092"><a name="p955817591092"></a><a name="p955817591092"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p1244542412175"><a name="p1244542412175"></a><a name="p1244542412175"></a>+0</p>
</td>
</tr>
<tr id="row8801747184718"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p8812473470"><a name="p8812473470"></a><a name="p8812473470"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p981164734716"><a name="p981164734716"></a><a name="p981164734716"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p08164744719"><a name="p08164744719"></a><a name="p08164744719"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p155716592916"><a name="p155716592916"></a><a name="p155716592916"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p17445112418174"><a name="p17445112418174"></a><a name="p17445112418174"></a>+0</p>
</td>
</tr>
<tr id="row1481647104711"><td class="cellrowborder" rowspan="7" valign="top" width="12.34%" headers="mcps1.2.9.1.1 "><p id="p1181547114713"><a name="p1181547114713"></a><a name="p1181547114713"></a>nan/任意值</p>
<p id="p178154744719"><a name="p178154744719"></a><a name="p178154744719"></a>+0/+inf</p>
<p id="p28124724715"><a name="p28124724715"></a><a name="p28124724715"></a>+0/-inf</p>
<p id="p5815479474"><a name="p5815479474"></a><a name="p5815479474"></a>-0/+inf</p>
<p id="p481124764715"><a name="p481124764715"></a><a name="p481124764715"></a>-0/-inf</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="10.36%" headers="mcps1.2.9.1.2 "><p id="p118111478472"><a name="p118111478472"></a><a name="p118111478472"></a>nan</p>
</td>
<td class="cellrowborder" rowspan="7" align="center" valign="middle" width="12.27%" headers="mcps1.2.9.1.3 "><p id="p481144724719"><a name="p481144724719"></a><a name="p481144724719"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="6.39%" headers="mcps1.2.9.1.4 "><p id="p14829470476"><a name="p14829470476"></a><a name="p14829470476"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="12.42%" headers="mcps1.2.9.1.5 "><p id="p1382194711478"><a name="p1382194711478"></a><a name="p1382194711478"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="15.4%" headers="mcps1.2.9.1.6 "><p id="p28217479474"><a name="p28217479474"></a><a name="p28217479474"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="17.84%" headers="mcps1.2.9.1.7 "><p id="p18557175910914"><a name="p18557175910914"></a><a name="p18557175910914"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" width="12.98%" headers="mcps1.2.9.1.8 "><p id="p17445224141713"><a name="p17445224141713"></a><a name="p17445224141713"></a>+0</p>
</td>
</tr>
<tr id="row1782847104714"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p168234744717"><a name="p168234744717"></a><a name="p168234744717"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p138244715479"><a name="p138244715479"></a><a name="p138244715479"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p12821947144716"><a name="p12821947144716"></a><a name="p12821947144716"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p955716592919"><a name="p955716592919"></a><a name="p955716592919"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p104459248170"><a name="p104459248170"></a><a name="p104459248170"></a>+0</p>
</td>
</tr>
<tr id="row08274724711"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p682847134719"><a name="p682847134719"></a><a name="p682847134719"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1882164712478"><a name="p1882164712478"></a><a name="p1882164712478"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p1782847194717"><a name="p1782847194717"></a><a name="p1782847194717"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p1455616599914"><a name="p1455616599914"></a><a name="p1455616599914"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p11445182471719"><a name="p11445182471719"></a><a name="p11445182471719"></a>MAX</p>
</td>
</tr>
<tr id="row168313474474"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p48312477476"><a name="p48312477476"></a><a name="p48312477476"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p2083144716474"><a name="p2083144716474"></a><a name="p2083144716474"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p10831471475"><a name="p10831471475"></a><a name="p10831471475"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p455645912914"><a name="p455645912914"></a><a name="p455645912914"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p744582471711"><a name="p744582471711"></a><a name="p744582471711"></a>+0</p>
</td>
</tr>
<tr id="row383204714713"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p20831247134714"><a name="p20831247134714"></a><a name="p20831247134714"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p128354714475"><a name="p128354714475"></a><a name="p128354714475"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p18838478470"><a name="p18838478470"></a><a name="p18838478470"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p65566594911"><a name="p65566594911"></a><a name="p65566594911"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p9445624191715"><a name="p9445624191715"></a><a name="p9445624191715"></a>MAX</p>
</td>
</tr>
<tr id="row08354719471"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p15831947184716"><a name="p15831947184716"></a><a name="p15831947184716"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p1184164754712"><a name="p1184164754712"></a><a name="p1184164754712"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p2845472479"><a name="p2845472479"></a><a name="p2845472479"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p165557592912"><a name="p165557592912"></a><a name="p165557592912"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p20445202481712"><a name="p20445202481712"></a><a name="p20445202481712"></a>+0</p>
</td>
</tr>
<tr id="row18417475474"><td class="cellrowborder" valign="top" headers="mcps1.2.9.1.1 "><p id="p168494711478"><a name="p168494711478"></a><a name="p168494711478"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.2 "><p id="p284124794715"><a name="p284124794715"></a><a name="p284124794715"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.3 "><p id="p1384547134719"><a name="p1384547134719"></a><a name="p1384547134719"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.4 "><p id="p135532596910"><a name="p135532596910"></a><a name="p135532596910"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.9.1.5 "><p id="p54456249178"><a name="p54456249178"></a><a name="p54456249178"></a>+0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 24**  AddRelu特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table143801137164310"></a>
<table><thead align="left"><tr id="row193811637104314"><th class="cellrowborder" valign="top" width="14.24%" id="mcps1.2.6.1.1"><p id="p93811437124319"><a name="p93811437124319"></a><a name="p93811437124319"></a>src0取值/src1取值</p>
</th>
<th class="cellrowborder" valign="top" width="15.1%" id="mcps1.2.6.1.2"><p id="p838143713431"><a name="p838143713431"></a><a name="p838143713431"></a>中间阶段(src0 + src1)</p>
</th>
<th class="cellrowborder" valign="top" width="27.29%" id="mcps1.2.6.1.3"><p id="p193811137184313"><a name="p193811137184313"></a><a name="p193811137184313"></a>中间阶段（饱和模式，仅输出为half数据类型时支持）</p>
</th>
<th class="cellrowborder" valign="top" width="20.580000000000002%" id="mcps1.2.6.1.4"><p id="p1338112377438"><a name="p1338112377438"></a><a name="p1338112377438"></a>dst输出（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="22.79%" id="mcps1.2.6.1.5"><p id="p113821037174310"><a name="p113821037174310"></a><a name="p113821037174310"></a>dst输出（饱和模式，仅输出为half数据类型时支持）</p>
</th>
</tr>
</thead>
<tbody><tr id="row638233724312"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p18382237104316"><a name="p18382237104316"></a><a name="p18382237104316"></a>+0/+0</p>
<p id="p1038219376439"><a name="p1038219376439"></a><a name="p1038219376439"></a>+0/-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p63822379430"><a name="p63822379430"></a><a name="p63822379430"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p63822371435"><a name="p63822371435"></a><a name="p63822371435"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p413020531144"><a name="p413020531144"></a><a name="p413020531144"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p171301653191410"><a name="p171301653191410"></a><a name="p171301653191410"></a>+0</p>
</td>
</tr>
<tr id="row123853375431"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p5385203724310"><a name="p5385203724310"></a><a name="p5385203724310"></a>-0/-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p17385143714318"><a name="p17385143714318"></a><a name="p17385143714318"></a>-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p138583714437"><a name="p138583714437"></a><a name="p138583714437"></a>-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p7125553161419"><a name="p7125553161419"></a><a name="p7125553161419"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p7124165321413"><a name="p7124165321413"></a><a name="p7124165321413"></a>+0</p>
</td>
</tr>
<tr id="row18388163719435"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p3388173794314"><a name="p3388173794314"></a><a name="p3388173794314"></a>+inf/+inf</p>
<p id="p16388237134320"><a name="p16388237134320"></a><a name="p16388237134320"></a>+inf/MAX</p>
<p id="p1889611020273"><a name="p1889611020273"></a><a name="p1889611020273"></a>+inf/MIN</p>
<p id="p10388183715431"><a name="p10388183715431"></a><a name="p10388183715431"></a>+0/+inf</p>
<p id="p25722421509"><a name="p25722421509"></a><a name="p25722421509"></a>-0/+inf</p>
<p id="p153881437204313"><a name="p153881437204313"></a><a name="p153881437204313"></a>MAX/MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p143881637144313"><a name="p143881637144313"></a><a name="p143881637144313"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p1838913711436"><a name="p1838913711436"></a><a name="p1838913711436"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p12119135351412"><a name="p12119135351412"></a><a name="p12119135351412"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p11119453181417"><a name="p11119453181417"></a><a name="p11119453181417"></a>MAX</p>
</td>
</tr>
<tr id="row53911237134316"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p9316155522611"><a name="p9316155522611"></a><a name="p9316155522611"></a>-inf/-inf</p>
<p id="p8434807270"><a name="p8434807270"></a><a name="p8434807270"></a>-inf/MAX</p>
<p id="p7949319152719"><a name="p7949319152719"></a><a name="p7949319152719"></a>-inf/MIN</p>
<p id="p1223192332717"><a name="p1223192332717"></a><a name="p1223192332717"></a>+0/-inf</p>
<p id="p13169152611296"><a name="p13169152611296"></a><a name="p13169152611296"></a>-0/+inf</p>
<p id="p46981432202916"><a name="p46981432202916"></a><a name="p46981432202916"></a>MIN/MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p8392123794314"><a name="p8392123794314"></a><a name="p8392123794314"></a>-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p0392437124314"><a name="p0392437124314"></a><a name="p0392437124314"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p1811355331414"><a name="p1811355331414"></a><a name="p1811355331414"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p151138531148"><a name="p151138531148"></a><a name="p151138531148"></a>+0</p>
</td>
</tr>
<tr id="row5395153714432"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p7395937204317"><a name="p7395937204317"></a><a name="p7395937204317"></a>使得src0 + src1 = MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p1539533744312"><a name="p1539533744312"></a><a name="p1539533744312"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p1396183724311"><a name="p1396183724311"></a><a name="p1396183724311"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p11108115318149"><a name="p11108115318149"></a><a name="p11108115318149"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p20108253151417"><a name="p20108253151417"></a><a name="p20108253151417"></a>MAX</p>
</td>
</tr>
<tr id="row1839863711435"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p20398133712439"><a name="p20398133712439"></a><a name="p20398133712439"></a>使得src0 + src1 = MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p15398193712431"><a name="p15398193712431"></a><a name="p15398193712431"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p10399193704317"><a name="p10399193704317"></a><a name="p10399193704317"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p0103105371415"><a name="p0103105371415"></a><a name="p0103105371415"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p17103135316146"><a name="p17103135316146"></a><a name="p17103135316146"></a>+0</p>
</td>
</tr>
<tr id="row124011337124315"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p8401163712438"><a name="p8401163712438"></a><a name="p8401163712438"></a>nan/任意值</p>
<p id="p7766317193419"><a name="p7766317193419"></a><a name="p7766317193419"></a>+inf/-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p1840223712435"><a name="p1840223712435"></a><a name="p1840223712435"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p340212379430"><a name="p340212379430"></a><a name="p340212379430"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p29675316144"><a name="p29675316144"></a><a name="p29675316144"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p8961853151413"><a name="p8961853151413"></a><a name="p8961853151413"></a>+0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 25**  SubRelu特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table7783335111811"></a>
<table><thead align="left"><tr id="row77831835131814"><th class="cellrowborder" valign="top" width="14.24%" id="mcps1.2.6.1.1"><p id="p11783235171813"><a name="p11783235171813"></a><a name="p11783235171813"></a>src0取值/src1取值</p>
</th>
<th class="cellrowborder" valign="top" width="15.1%" id="mcps1.2.6.1.2"><p id="p1178383581815"><a name="p1178383581815"></a><a name="p1178383581815"></a>中间阶段(src0 - src1)</p>
</th>
<th class="cellrowborder" valign="top" width="27.29%" id="mcps1.2.6.1.3"><p id="p1478383515185"><a name="p1478383515185"></a><a name="p1478383515185"></a>中间阶段（饱和模式，仅输出为half数据类型时支持）</p>
</th>
<th class="cellrowborder" valign="top" width="20.580000000000002%" id="mcps1.2.6.1.4"><p id="p1378418357182"><a name="p1378418357182"></a><a name="p1378418357182"></a>dst输出（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="22.79%" id="mcps1.2.6.1.5"><p id="p378415353188"><a name="p378415353188"></a><a name="p378415353188"></a>dst输出（饱和模式，仅输出为half数据类型时支持）</p>
</th>
</tr>
</thead>
<tbody><tr id="row147841035171819"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p127848352184"><a name="p127848352184"></a><a name="p127848352184"></a>+0/+0</p>
<p id="p1478463571813"><a name="p1478463571813"></a><a name="p1478463571813"></a>+0/-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p87848356188"><a name="p87848356188"></a><a name="p87848356188"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p1478412353180"><a name="p1478412353180"></a><a name="p1478412353180"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p1078423511812"><a name="p1078423511812"></a><a name="p1078423511812"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p177841735101812"><a name="p177841735101812"></a><a name="p177841735101812"></a>+0</p>
</td>
</tr>
<tr id="row2784103571816"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p197847355187"><a name="p197847355187"></a><a name="p197847355187"></a>-0/+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p1778403518187"><a name="p1778403518187"></a><a name="p1778403518187"></a>-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p147843357189"><a name="p147843357189"></a><a name="p147843357189"></a>-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p4785113531812"><a name="p4785113531812"></a><a name="p4785113531812"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p15785335121817"><a name="p15785335121817"></a><a name="p15785335121817"></a>+0</p>
</td>
</tr>
<tr id="row278593581811"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p107851535111816"><a name="p107851535111816"></a><a name="p107851535111816"></a>+inf/-inf</p>
<p id="p12785133551813"><a name="p12785133551813"></a><a name="p12785133551813"></a>+inf/MAX</p>
<p id="p078583517188"><a name="p078583517188"></a><a name="p078583517188"></a>+inf/MIN</p>
<p id="p16785535131814"><a name="p16785535131814"></a><a name="p16785535131814"></a>+0/-inf</p>
<p id="p12785123510188"><a name="p12785123510188"></a><a name="p12785123510188"></a>-0/-inf</p>
<p id="p1378543519185"><a name="p1378543519185"></a><a name="p1378543519185"></a>MAX/MIN</p>
<p id="p230191482013"><a name="p230191482013"></a><a name="p230191482013"></a>MAX/-inf</p>
<p id="p4893142782012"><a name="p4893142782012"></a><a name="p4893142782012"></a>MIN/-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p1078553512189"><a name="p1078553512189"></a><a name="p1078553512189"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p87851235191817"><a name="p87851235191817"></a><a name="p87851235191817"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p5785193501814"><a name="p5785193501814"></a><a name="p5785193501814"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p378583551815"><a name="p378583551815"></a><a name="p378583551815"></a>MAX</p>
</td>
</tr>
<tr id="row178533591819"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p157851235141810"><a name="p157851235141810"></a><a name="p157851235141810"></a>-inf/-inf</p>
<p id="p1378553515188"><a name="p1378553515188"></a><a name="p1378553515188"></a>-inf/MAX</p>
<p id="p12785193511811"><a name="p12785193511811"></a><a name="p12785193511811"></a>-inf/MIN</p>
<p id="p1378563591814"><a name="p1378563591814"></a><a name="p1378563591814"></a>+0/-inf</p>
<p id="p4786835161810"><a name="p4786835161810"></a><a name="p4786835161810"></a>-0/+inf</p>
<p id="p12786133516186"><a name="p12786133516186"></a><a name="p12786133516186"></a>MIN/MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p678615352189"><a name="p678615352189"></a><a name="p678615352189"></a>-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p147862035111819"><a name="p147862035111819"></a><a name="p147862035111819"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p14786173531819"><a name="p14786173531819"></a><a name="p14786173531819"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p77861935131810"><a name="p77861935131810"></a><a name="p77861935131810"></a>+0</p>
</td>
</tr>
<tr id="row15786153571819"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p37861135141812"><a name="p37861135141812"></a><a name="p37861135141812"></a>使得src0 - src1 = MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p1678611356183"><a name="p1678611356183"></a><a name="p1678611356183"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p167861356181"><a name="p167861356181"></a><a name="p167861356181"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p978633501812"><a name="p978633501812"></a><a name="p978633501812"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p1778623581814"><a name="p1778623581814"></a><a name="p1778623581814"></a>MAX</p>
</td>
</tr>
<tr id="row1578663515188"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p4787835121813"><a name="p4787835121813"></a><a name="p4787835121813"></a>使得src0 - src1 = MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p67871435181817"><a name="p67871435181817"></a><a name="p67871435181817"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p4787183519189"><a name="p4787183519189"></a><a name="p4787183519189"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p1478773591820"><a name="p1478773591820"></a><a name="p1478773591820"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p278743561812"><a name="p278743561812"></a><a name="p278743561812"></a>+0</p>
</td>
</tr>
<tr id="row1578793518186"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p67878358181"><a name="p67878358181"></a><a name="p67878358181"></a>nan/任意值</p>
<p id="p1578753514186"><a name="p1578753514186"></a><a name="p1578753514186"></a>+inf/+inf</p>
<p id="p1139352522711"><a name="p1139352522711"></a><a name="p1139352522711"></a>-inf/-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p2078713514188"><a name="p2078713514188"></a><a name="p2078713514188"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p1078733519183"><a name="p1078733519183"></a><a name="p1078733519183"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p878723521811"><a name="p878723521811"></a><a name="p878723521811"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p1278763518183"><a name="p1278763518183"></a><a name="p1278763518183"></a>+0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 26**  AddReluCast特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table494414452367"></a>
<table><thead align="left"><tr id="row694484513617"><th class="cellrowborder" valign="top" width="18.67%" id="mcps1.2.6.1.1"><p id="p1051733854513"><a name="p1051733854513"></a><a name="p1051733854513"></a>src0数据类型/src1数据类型/dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="18.65%" id="mcps1.2.6.1.2"><p id="p89441145103612"><a name="p89441145103612"></a><a name="p89441145103612"></a>src0取值/src1取值</p>
</th>
<th class="cellrowborder" valign="top" width="18.02%" id="mcps1.2.6.1.3"><p id="p1194414515365"><a name="p1194414515365"></a><a name="p1194414515365"></a>中间阶段1(src0 + src1)（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20.39%" id="mcps1.2.6.1.4"><p id="p1694511458361"><a name="p1694511458361"></a><a name="p1694511458361"></a>中间阶段2（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="24.27%" id="mcps1.2.6.1.5"><p id="p124582541381"><a name="p124582541381"></a><a name="p124582541381"></a>dst输出（INF/NAN模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row16945204515368"><td class="cellrowborder" rowspan="7" valign="top" width="18.67%" headers="mcps1.2.6.1.1 "><p id="p9360195916497"><a name="p9360195916497"></a><a name="p9360195916497"></a>float/float/half</p>
</td>
<td class="cellrowborder" valign="top" width="18.65%" headers="mcps1.2.6.1.2 "><p id="p6945174553612"><a name="p6945174553612"></a><a name="p6945174553612"></a>+0/+0</p>
<p id="p3945045183620"><a name="p3945045183620"></a><a name="p3945045183620"></a>+0/-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="18.02%" headers="mcps1.2.6.1.3 "><p id="p1094574518361"><a name="p1094574518361"></a><a name="p1094574518361"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.39%" headers="mcps1.2.6.1.4 "><p id="p69454453363"><a name="p69454453363"></a><a name="p69454453363"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="24.27%" headers="mcps1.2.6.1.5 "><p id="p10691192114515"><a name="p10691192114515"></a><a name="p10691192114515"></a>+0</p>
</td>
</tr>
<tr id="row3945114520366"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p13946645193619"><a name="p13946645193619"></a><a name="p13946645193619"></a>-0/-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p20946174514364"><a name="p20946174514364"></a><a name="p20946174514364"></a>-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p6946134533610"><a name="p6946134533610"></a><a name="p6946134533610"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p668962114459"><a name="p668962114459"></a><a name="p668962114459"></a>+0</p>
</td>
</tr>
<tr id="row1794644573611"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p794614593617"><a name="p794614593617"></a><a name="p794614593617"></a>+inf/+inf</p>
<p id="p1694617457367"><a name="p1694617457367"></a><a name="p1694617457367"></a>+inf/MAX</p>
<p id="p179461345143616"><a name="p179461345143616"></a><a name="p179461345143616"></a>+inf/MIN</p>
<p id="p1094624514368"><a name="p1094624514368"></a><a name="p1094624514368"></a>+0/+inf</p>
<p id="p179461745123613"><a name="p179461745123613"></a><a name="p179461745123613"></a>-0/+inf</p>
<p id="p1094754513610"><a name="p1094754513610"></a><a name="p1094754513610"></a>MAX/MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p1694718452366"><a name="p1694718452366"></a><a name="p1694718452366"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p694764523617"><a name="p694764523617"></a><a name="p694764523617"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p1688102115458"><a name="p1688102115458"></a><a name="p1688102115458"></a>+inf</p>
</td>
</tr>
<tr id="row19472454364"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p169471545193613"><a name="p169471545193613"></a><a name="p169471545193613"></a>-inf/-inf</p>
<p id="p4947145173617"><a name="p4947145173617"></a><a name="p4947145173617"></a>-inf/MAX</p>
<p id="p494734512367"><a name="p494734512367"></a><a name="p494734512367"></a>-inf/MIN</p>
<p id="p7947445183610"><a name="p7947445183610"></a><a name="p7947445183610"></a>+0/-inf</p>
<p id="p139485459363"><a name="p139485459363"></a><a name="p139485459363"></a>-0/+inf</p>
<p id="p39488454363"><a name="p39488454363"></a><a name="p39488454363"></a>MIN/MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p594884533616"><a name="p594884533616"></a><a name="p594884533616"></a>-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p0948164515367"><a name="p0948164515367"></a><a name="p0948164515367"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p4459155403815"><a name="p4459155403815"></a><a name="p4459155403815"></a>+0</p>
</td>
</tr>
<tr id="row894834514365"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p994834512368"><a name="p994834512368"></a><a name="p994834512368"></a>使得 src0 + src1 = MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p16948174543616"><a name="p16948174543616"></a><a name="p16948174543616"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p14949204520361"><a name="p14949204520361"></a><a name="p14949204520361"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p6689050165419"><a name="p6689050165419"></a><a name="p6689050165419"></a>MAX</p>
</td>
</tr>
<tr id="row594918451368"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p294954514367"><a name="p294954514367"></a><a name="p294954514367"></a>使得 src0 + src1 = MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p169494456366"><a name="p169494456366"></a><a name="p169494456366"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p1294954518369"><a name="p1294954518369"></a><a name="p1294954518369"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p1145935416386"><a name="p1145935416386"></a><a name="p1145935416386"></a>+0</p>
</td>
</tr>
<tr id="row79509455368"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p695084517369"><a name="p695084517369"></a><a name="p695084517369"></a>nan/任意值</p>
<p id="p1695054593620"><a name="p1695054593620"></a><a name="p1695054593620"></a>+inf/-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p1295024533612"><a name="p1295024533612"></a><a name="p1295024533612"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p129505454360"><a name="p129505454360"></a><a name="p129505454360"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p18459205453818"><a name="p18459205453818"></a><a name="p18459205453818"></a>+0</p>
</td>
</tr>
<tr id="row19554103510506"><td class="cellrowborder" rowspan="7" valign="top" width="18.67%" headers="mcps1.2.6.1.1 "><p id="p187711318195118"><a name="p187711318195118"></a><a name="p187711318195118"></a>half/half/int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="18.65%" headers="mcps1.2.6.1.2 "><p id="p7995182413711"><a name="p7995182413711"></a><a name="p7995182413711"></a>+0/+0</p>
<p id="p8995324871"><a name="p8995324871"></a><a name="p8995324871"></a>+0/-0</p>
</td>
<td class="cellrowborder" valign="top" width="18.02%" headers="mcps1.2.6.1.3 "><p id="p774213775112"><a name="p774213775112"></a><a name="p774213775112"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="20.39%" headers="mcps1.2.6.1.4 "><p id="p1955593585015"><a name="p1955593585015"></a><a name="p1955593585015"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="24.27%" headers="mcps1.2.6.1.5 "><p id="p11555335145014"><a name="p11555335145014"></a><a name="p11555335145014"></a>0</p>
</td>
</tr>
<tr id="row1488243585011"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p3996424375"><a name="p3996424375"></a><a name="p3996424375"></a>-0/-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p07421572510"><a name="p07421572510"></a><a name="p07421572510"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16882113513508"><a name="p16882113513508"></a><a name="p16882113513508"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1288218354507"><a name="p1288218354507"></a><a name="p1288218354507"></a>0</p>
</td>
</tr>
<tr id="row61718366505"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p119961724575"><a name="p119961724575"></a><a name="p119961724575"></a>+inf/+inf</p>
<p id="p14996224874"><a name="p14996224874"></a><a name="p14996224874"></a>+inf/MAX</p>
<p id="p1996324578"><a name="p1996324578"></a><a name="p1996324578"></a>+inf/MIN</p>
<p id="p99961624976"><a name="p99961624976"></a><a name="p99961624976"></a>+0/+inf</p>
<p id="p129961624173"><a name="p129961624173"></a><a name="p129961624173"></a>-0/+inf</p>
<p id="p299682420710"><a name="p299682420710"></a><a name="p299682420710"></a>MAX/MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p174210717512"><a name="p174210717512"></a><a name="p174210717512"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9185365501"><a name="p9185365501"></a><a name="p9185365501"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1845714571302"><a name="p1845714571302"></a><a name="p1845714571302"></a>MAX</p>
</td>
</tr>
<tr id="row216063614501"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p899662419716"><a name="p899662419716"></a><a name="p899662419716"></a>-inf/-inf</p>
<p id="p49969243711"><a name="p49969243711"></a><a name="p49969243711"></a>-inf/MAX</p>
<p id="p999672410715"><a name="p999672410715"></a><a name="p999672410715"></a>-inf/MIN</p>
<p id="p1499714241873"><a name="p1499714241873"></a><a name="p1499714241873"></a>+0/-inf</p>
<p id="p6997202411714"><a name="p6997202411714"></a><a name="p6997202411714"></a>-0/+inf</p>
<p id="p7997162413711"><a name="p7997162413711"></a><a name="p7997162413711"></a>MIN/MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1274219725118"><a name="p1274219725118"></a><a name="p1274219725118"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1916015369508"><a name="p1916015369508"></a><a name="p1916015369508"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p191617363506"><a name="p191617363506"></a><a name="p191617363506"></a>0</p>
</td>
</tr>
<tr id="row123051361503"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p129976241472"><a name="p129976241472"></a><a name="p129976241472"></a>使得 src0 + src1 = MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1174227135116"><a name="p1174227135116"></a><a name="p1174227135116"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1423512381008"><a name="p1423512381008"></a><a name="p1423512381008"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p43061236135012"><a name="p43061236135012"></a><a name="p43061236135012"></a>MAX</p>
</td>
</tr>
<tr id="row6447153645012"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p189971243712"><a name="p189971243712"></a><a name="p189971243712"></a>使得 src0 + src1 = MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p187431372512"><a name="p187431372512"></a><a name="p187431372512"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p10447113665012"><a name="p10447113665012"></a><a name="p10447113665012"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p744753615016"><a name="p744753615016"></a><a name="p744753615016"></a>0</p>
</td>
</tr>
<tr id="row15577336125019"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p69971124379"><a name="p69971124379"></a><a name="p69971124379"></a>nan/任意值</p>
<p id="p9997152412710"><a name="p9997152412710"></a><a name="p9997152412710"></a>+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p107431976518"><a name="p107431976518"></a><a name="p107431976518"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1357714366505"><a name="p1357714366505"></a><a name="p1357714366505"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p20113323217"><a name="p20113323217"></a><a name="p20113323217"></a>0</p>
</td>
</tr>
<tr id="row1171473675019"><td class="cellrowborder" rowspan="3" valign="top" width="18.67%" headers="mcps1.2.6.1.1 "><p id="p1342584219516"><a name="p1342584219516"></a><a name="p1342584219516"></a>int16_t/int16_t/int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="18.65%" headers="mcps1.2.6.1.2 "><p id="p67148366508"><a name="p67148366508"></a><a name="p67148366508"></a>0/0</p>
</td>
<td class="cellrowborder" valign="top" width="18.02%" headers="mcps1.2.6.1.3 "><p id="p17714163613505"><a name="p17714163613505"></a><a name="p17714163613505"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20.39%" headers="mcps1.2.6.1.4 "><p id="p157146362501"><a name="p157146362501"></a><a name="p157146362501"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="24.27%" headers="mcps1.2.6.1.5 "><p id="p19714203625012"><a name="p19714203625012"></a><a name="p19714203625012"></a>0</p>
</td>
</tr>
<tr id="row1085583612501"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p13855536115016"><a name="p13855536115016"></a><a name="p13855536115016"></a>MAX/MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p198551336125016"><a name="p198551336125016"></a><a name="p198551336125016"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11161113911312"><a name="p11161113911312"></a><a name="p11161113911312"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p158551736165017"><a name="p158551736165017"></a><a name="p158551736165017"></a>MAX</p>
</td>
</tr>
<tr id="row79941336135019"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p799483685013"><a name="p799483685013"></a><a name="p799483685013"></a>MIN/MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p09947363509"><a name="p09947363509"></a><a name="p09947363509"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p19994193635013"><a name="p19994193635013"></a><a name="p19994193635013"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p7994193695016"><a name="p7994193695016"></a><a name="p7994193695016"></a>0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 27**  SubReluCast特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table1418126172917"></a>
<table><thead align="left"><tr id="row161916682912"><th class="cellrowborder" valign="top" width="18.67%" id="mcps1.2.6.1.1"><p id="p81906182919"><a name="p81906182919"></a><a name="p81906182919"></a>src0数据类型/src1数据类型/dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="18.65%" id="mcps1.2.6.1.2"><p id="p41917615292"><a name="p41917615292"></a><a name="p41917615292"></a>src0取值/src1取值</p>
</th>
<th class="cellrowborder" valign="top" width="18.02%" id="mcps1.2.6.1.3"><p id="p161918632915"><a name="p161918632915"></a><a name="p161918632915"></a>中间阶段1(src0 - src1)（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="20.39%" id="mcps1.2.6.1.4"><p id="p519166182910"><a name="p519166182910"></a><a name="p519166182910"></a>中间阶段2（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="24.27%" id="mcps1.2.6.1.5"><p id="p16199612297"><a name="p16199612297"></a><a name="p16199612297"></a>dst输出（INF/NAN模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row31911672911"><td class="cellrowborder" rowspan="7" valign="top" width="18.67%" headers="mcps1.2.6.1.1 "><p id="p2019663292"><a name="p2019663292"></a><a name="p2019663292"></a>float/float/half</p>
</td>
<td class="cellrowborder" valign="top" width="18.65%" headers="mcps1.2.6.1.2 "><p id="p9191361292"><a name="p9191361292"></a><a name="p9191361292"></a>+0/+0</p>
<p id="p16191661293"><a name="p16191661293"></a><a name="p16191661293"></a>+0/-0</p>
<p id="p714624142919"><a name="p714624142919"></a><a name="p714624142919"></a>-0/-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="18.02%" headers="mcps1.2.6.1.3 "><p id="p020564292"><a name="p020564292"></a><a name="p020564292"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.39%" headers="mcps1.2.6.1.4 "><p id="p5201766297"><a name="p5201766297"></a><a name="p5201766297"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="24.27%" headers="mcps1.2.6.1.5 "><p id="p1820176102912"><a name="p1820176102912"></a><a name="p1820176102912"></a>+0</p>
</td>
</tr>
<tr id="row142020652917"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p19206642918"><a name="p19206642918"></a><a name="p19206642918"></a>-0/+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p142015692915"><a name="p142015692915"></a><a name="p142015692915"></a>-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p3201617296"><a name="p3201617296"></a><a name="p3201617296"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p182066182919"><a name="p182066182919"></a><a name="p182066182919"></a>+0</p>
</td>
</tr>
<tr id="row8205610297"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15204615299"><a name="p15204615299"></a><a name="p15204615299"></a>+inf/-inf</p>
<p id="p202010614299"><a name="p202010614299"></a><a name="p202010614299"></a>+inf/MAX</p>
<p id="p162020614298"><a name="p162020614298"></a><a name="p162020614298"></a>+inf/MIN</p>
<p id="p22010632912"><a name="p22010632912"></a><a name="p22010632912"></a>+0/-inf</p>
<p id="p1420186122915"><a name="p1420186122915"></a><a name="p1420186122915"></a>-0/-inf</p>
<p id="p1201661291"><a name="p1201661291"></a><a name="p1201661291"></a>MAX/MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p7219612915"><a name="p7219612915"></a><a name="p7219612915"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p82176102913"><a name="p82176102913"></a><a name="p82176102913"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p421166162913"><a name="p421166162913"></a><a name="p421166162913"></a>+inf</p>
</td>
</tr>
<tr id="row6215612914"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p22114620294"><a name="p22114620294"></a><a name="p22114620294"></a>-inf/+0</p>
<p id="p1370518117314"><a name="p1370518117314"></a><a name="p1370518117314"></a>-inf/+0</p>
<p id="p12111692910"><a name="p12111692910"></a><a name="p12111692910"></a>-inf/MAX</p>
<p id="p5211666299"><a name="p5211666299"></a><a name="p5211666299"></a>-inf/MIN</p>
<p id="p52196182914"><a name="p52196182914"></a><a name="p52196182914"></a>+0/+inf</p>
<p id="p14215610292"><a name="p14215610292"></a><a name="p14215610292"></a>-0/+inf</p>
<p id="p162110617298"><a name="p162110617298"></a><a name="p162110617298"></a>MIN/MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p132116692918"><a name="p132116692918"></a><a name="p132116692918"></a>-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p1421156152912"><a name="p1421156152912"></a><a name="p1421156152912"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p8211369295"><a name="p8211369295"></a><a name="p8211369295"></a>+0</p>
</td>
</tr>
<tr id="row142110620293"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p32276172912"><a name="p32276172912"></a><a name="p32276172912"></a>使得src0 - src1 = MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p322468295"><a name="p322468295"></a><a name="p322468295"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p22213617298"><a name="p22213617298"></a><a name="p22213617298"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p1922116132916"><a name="p1922116132916"></a><a name="p1922116132916"></a>MAX</p>
</td>
</tr>
<tr id="row172216622918"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1224662914"><a name="p1224662914"></a><a name="p1224662914"></a>使得src0 - src1 = MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p14227610295"><a name="p14227610295"></a><a name="p14227610295"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p182226112918"><a name="p182226112918"></a><a name="p182226112918"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p1822196142912"><a name="p1822196142912"></a><a name="p1822196142912"></a>+0</p>
</td>
</tr>
<tr id="row132226102911"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p182214610297"><a name="p182214610297"></a><a name="p182214610297"></a>nan/任意值</p>
<p id="p422364291"><a name="p422364291"></a><a name="p422364291"></a>+inf/+inf</p>
<p id="p719415438315"><a name="p719415438315"></a><a name="p719415438315"></a>-inf/-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.2 "><p id="p112396142911"><a name="p112396142911"></a><a name="p112396142911"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.3 "><p id="p0235616293"><a name="p0235616293"></a><a name="p0235616293"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.6.1.4 "><p id="p12231365293"><a name="p12231365293"></a><a name="p12231365293"></a>+0</p>
</td>
</tr>
<tr id="row172314610295"><td class="cellrowborder" rowspan="7" valign="top" width="18.67%" headers="mcps1.2.6.1.1 "><p id="p162316172913"><a name="p162316172913"></a><a name="p162316172913"></a>half/half/int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="18.65%" headers="mcps1.2.6.1.2 "><p id="p122319652915"><a name="p122319652915"></a><a name="p122319652915"></a>+0/+0</p>
<p id="p10233612914"><a name="p10233612914"></a><a name="p10233612914"></a>+0/-0</p>
<p id="p1387613581317"><a name="p1387613581317"></a><a name="p1387613581317"></a>-0/-0</p>
</td>
<td class="cellrowborder" valign="top" width="18.02%" headers="mcps1.2.6.1.3 "><p id="p9234672911"><a name="p9234672911"></a><a name="p9234672911"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="20.39%" headers="mcps1.2.6.1.4 "><p id="p823196182911"><a name="p823196182911"></a><a name="p823196182911"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" width="24.27%" headers="mcps1.2.6.1.5 "><p id="p142376152911"><a name="p142376152911"></a><a name="p142376152911"></a>0</p>
</td>
</tr>
<tr id="row162316672916"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15231466293"><a name="p15231466293"></a><a name="p15231466293"></a>-0/-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p182318652918"><a name="p182318652918"></a><a name="p182318652918"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1224166172915"><a name="p1224166172915"></a><a name="p1224166172915"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p18241761299"><a name="p18241761299"></a><a name="p18241761299"></a>0</p>
</td>
</tr>
<tr id="row0244602914"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9241066296"><a name="p9241066296"></a><a name="p9241066296"></a>+inf/-inf</p>
<p id="p2024968299"><a name="p2024968299"></a><a name="p2024968299"></a>+inf/MAX</p>
<p id="p122486162918"><a name="p122486162918"></a><a name="p122486162918"></a>+inf/MIN</p>
<p id="p17272632183211"><a name="p17272632183211"></a><a name="p17272632183211"></a>+inf/0</p>
<p id="p235283510321"><a name="p235283510321"></a><a name="p235283510321"></a>+inf/-0</p>
<p id="p82413611298"><a name="p82413611298"></a><a name="p82413611298"></a>+0/-inf</p>
<p id="p5245612292"><a name="p5245612292"></a><a name="p5245612292"></a>-0/-inf</p>
<p id="p4241632914"><a name="p4241632914"></a><a name="p4241632914"></a>MAX/MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p182413619299"><a name="p182413619299"></a><a name="p182413619299"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p122446182912"><a name="p122446182912"></a><a name="p122446182912"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p52416192917"><a name="p52416192917"></a><a name="p52416192917"></a>MAX</p>
</td>
</tr>
<tr id="row12244682920"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p112417617299"><a name="p112417617299"></a><a name="p112417617299"></a>-inf/-inf</p>
<p id="p19251960296"><a name="p19251960296"></a><a name="p19251960296"></a>-inf/MAX</p>
<p id="p625186122918"><a name="p625186122918"></a><a name="p625186122918"></a>-inf/MIN</p>
<p id="p92513611290"><a name="p92513611290"></a><a name="p92513611290"></a>+0/-inf</p>
<p id="p15253617294"><a name="p15253617294"></a><a name="p15253617294"></a>-0/+inf</p>
<p id="p182576202917"><a name="p182576202917"></a><a name="p182576202917"></a>MIN/MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p122586142917"><a name="p122586142917"></a><a name="p122586142917"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p425126142913"><a name="p425126142913"></a><a name="p425126142913"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p3255692915"><a name="p3255692915"></a><a name="p3255692915"></a>0</p>
</td>
</tr>
<tr id="row12512672918"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1425563298"><a name="p1425563298"></a><a name="p1425563298"></a>使得 src0 - src1 = MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p17251663297"><a name="p17251663297"></a><a name="p17251663297"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p112520632914"><a name="p112520632914"></a><a name="p112520632914"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p12510617296"><a name="p12510617296"></a><a name="p12510617296"></a>MAX</p>
</td>
</tr>
<tr id="row182619632918"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p152617615299"><a name="p152617615299"></a><a name="p152617615299"></a>使得 src0 - src1 = MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p13267602915"><a name="p13267602915"></a><a name="p13267602915"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p5263612919"><a name="p5263612919"></a><a name="p5263612919"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p52612662912"><a name="p52612662912"></a><a name="p52612662912"></a>0</p>
</td>
</tr>
<tr id="row15264692911"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p326106142911"><a name="p326106142911"></a><a name="p326106142911"></a>nan/任意值</p>
<p id="p18265610299"><a name="p18265610299"></a><a name="p18265610299"></a>+inf/+inf</p>
<p id="p8524925334"><a name="p8524925334"></a><a name="p8524925334"></a>-inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12266612293"><a name="p12266612293"></a><a name="p12266612293"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p15261968294"><a name="p15261968294"></a><a name="p15261968294"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1826126122911"><a name="p1826126122911"></a><a name="p1826126122911"></a>0</p>
</td>
</tr>
<tr id="row1427136152913"><td class="cellrowborder" rowspan="3" valign="top" width="18.67%" headers="mcps1.2.6.1.1 "><p id="p72716642918"><a name="p72716642918"></a><a name="p72716642918"></a>int16_t/int16_t/int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="18.65%" headers="mcps1.2.6.1.2 "><p id="p72714610291"><a name="p72714610291"></a><a name="p72714610291"></a>0/0</p>
</td>
<td class="cellrowborder" valign="top" width="18.02%" headers="mcps1.2.6.1.3 "><p id="p1827663292"><a name="p1827663292"></a><a name="p1827663292"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20.39%" headers="mcps1.2.6.1.4 "><p id="p1927106192912"><a name="p1927106192912"></a><a name="p1927106192912"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="24.27%" headers="mcps1.2.6.1.5 "><p id="p16271060292"><a name="p16271060292"></a><a name="p16271060292"></a>0</p>
</td>
</tr>
<tr id="row15272066297"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1227262298"><a name="p1227262298"></a><a name="p1227262298"></a>MAX/MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3274612918"><a name="p3274612918"></a><a name="p3274612918"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p16271067297"><a name="p16271067297"></a><a name="p16271067297"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p62717617298"><a name="p62717617298"></a><a name="p62717617298"></a>MAX</p>
</td>
</tr>
<tr id="row3271862298"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p52818612294"><a name="p52818612294"></a><a name="p52818612294"></a>MIN/MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p328146122916"><a name="p328146122916"></a><a name="p328146122916"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p13285617293"><a name="p13285617293"></a><a name="p13285617293"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p18285617294"><a name="p18285617294"></a><a name="p18285617294"></a>0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 28**  MulCast特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值）

<a name="table1698216295159"></a>
<table><thead align="left"><tr id="row4983102921514"><th class="cellrowborder" valign="top" width="14.24%" id="mcps1.2.6.1.1"><p id="p498315294159"><a name="p498315294159"></a><a name="p498315294159"></a>src0取值/src1取值</p>
</th>
<th class="cellrowborder" valign="top" width="15.1%" id="mcps1.2.6.1.2"><p id="p18983172913152"><a name="p18983172913152"></a><a name="p18983172913152"></a>中间阶段(src0 * src1)</p>
</th>
<th class="cellrowborder" valign="top" width="27.29%" id="mcps1.2.6.1.3"><p id="p598312921517"><a name="p598312921517"></a><a name="p598312921517"></a>中间阶段（饱和模式，仅输出为half数据类型时支持）</p>
</th>
<th class="cellrowborder" valign="top" width="20.580000000000002%" id="mcps1.2.6.1.4"><p id="p4983329151519"><a name="p4983329151519"></a><a name="p4983329151519"></a>dst输出（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="22.79%" id="mcps1.2.6.1.5"><p id="p189846296153"><a name="p189846296153"></a><a name="p189846296153"></a>dst输出（饱和模式，仅输出为half数据类型时支持）</p>
</th>
</tr>
</thead>
<tbody><tr id="row4984172919152"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p141910402192"><a name="p141910402192"></a><a name="p141910402192"></a>+0/+0</p>
<p id="p10810167202014"><a name="p10810167202014"></a><a name="p10810167202014"></a>-0/-0</p>
<p id="p16222171082013"><a name="p16222171082013"></a><a name="p16222171082013"></a>+0/MAX</p>
<p id="p167311612015"><a name="p167311612015"></a><a name="p167311612015"></a>-0/MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p1498432915159"><a name="p1498432915159"></a><a name="p1498432915159"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p498432919152"><a name="p498432919152"></a><a name="p498432919152"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p9984172918151"><a name="p9984172918151"></a><a name="p9984172918151"></a>0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p15984329101517"><a name="p15984329101517"></a><a name="p15984329101517"></a>0</p>
</td>
</tr>
<tr id="row698442961519"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p541934014192"><a name="p541934014192"></a><a name="p541934014192"></a>+0/-0</p>
<p id="p139382819208"><a name="p139382819208"></a><a name="p139382819208"></a>+0/MIN</p>
<p id="p4317113110206"><a name="p4317113110206"></a><a name="p4317113110206"></a>-0/MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p1698514299159"><a name="p1698514299159"></a><a name="p1698514299159"></a>-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p1698522981517"><a name="p1698522981517"></a><a name="p1698522981517"></a>-0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p1498552910158"><a name="p1498552910158"></a><a name="p1498552910158"></a>0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p5985142961517"><a name="p5985142961517"></a><a name="p5985142961517"></a>0</p>
</td>
</tr>
<tr id="row498512913153"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p5418134016197"><a name="p5418134016197"></a><a name="p5418134016197"></a>MAX/MAX</p>
<p id="p27711337162014"><a name="p27711337162014"></a><a name="p27711337162014"></a>+inf/MAX</p>
<p id="p3662146172017"><a name="p3662146172017"></a><a name="p3662146172017"></a>-inf/MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p1198692911150"><a name="p1198692911150"></a><a name="p1198692911150"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p5986132919158"><a name="p5986132919158"></a><a name="p5986132919158"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p998620296151"><a name="p998620296151"></a><a name="p998620296151"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p1498616296157"><a name="p1498616296157"></a><a name="p1498616296157"></a>MAX</p>
</td>
</tr>
<tr id="row898662971515"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p16416640171913"><a name="p16416640171913"></a><a name="p16416640171913"></a>MAX/MIN</p>
<p id="p1958718072119"><a name="p1958718072119"></a><a name="p1958718072119"></a>+inf/MIN</p>
<p id="p12755105162116"><a name="p12755105162116"></a><a name="p12755105162116"></a>-inf/MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p998772951511"><a name="p998772951511"></a><a name="p998772951511"></a>-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p12987829181515"><a name="p12987829181515"></a><a name="p12987829181515"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p1734711919233"><a name="p1734711919233"></a><a name="p1734711919233"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p1998782971513"><a name="p1998782971513"></a><a name="p1998782971513"></a>MIN</p>
</td>
</tr>
<tr id="row159871129161518"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p16414174011918"><a name="p16414174011918"></a><a name="p16414174011918"></a>使得src0 * src1 = MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p9987152919154"><a name="p9987152919154"></a><a name="p9987152919154"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p1987429161518"><a name="p1987429161518"></a><a name="p1987429161518"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p15988229191519"><a name="p15988229191519"></a><a name="p15988229191519"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p17988029181511"><a name="p17988029181511"></a><a name="p17988029181511"></a>MAX</p>
</td>
</tr>
<tr id="row59883292159"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p154141140131919"><a name="p154141140131919"></a><a name="p154141140131919"></a>使得src0 * src1 = MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p12988729131510"><a name="p12988729131510"></a><a name="p12988729131510"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p1498815298155"><a name="p1498815298155"></a><a name="p1498815298155"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p7988142911516"><a name="p7988142911516"></a><a name="p7988142911516"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p1098811298150"><a name="p1098811298150"></a><a name="p1098811298150"></a>MIN</p>
</td>
</tr>
<tr id="row16989429101515"><td class="cellrowborder" valign="top" width="14.24%" headers="mcps1.2.6.1.1 "><p id="p1441344020197"><a name="p1441344020197"></a><a name="p1441344020197"></a>nan/任意数</p>
<p id="p163531911218"><a name="p163531911218"></a><a name="p163531911218"></a>+0/+inf</p>
<p id="p19192792120"><a name="p19192792120"></a><a name="p19192792120"></a>+0/-inf</p>
<p id="p77541234192112"><a name="p77541234192112"></a><a name="p77541234192112"></a>-0/+inf</p>
<p id="p04491837132110"><a name="p04491837132110"></a><a name="p04491837132110"></a>-0/-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="15.1%" headers="mcps1.2.6.1.2 "><p id="p8989929101519"><a name="p8989929101519"></a><a name="p8989929101519"></a>nan</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="27.29%" headers="mcps1.2.6.1.3 "><p id="p15989329191510"><a name="p15989329191510"></a><a name="p15989329191510"></a>+0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="20.580000000000002%" headers="mcps1.2.6.1.4 "><p id="p1698982951518"><a name="p1698982951518"></a><a name="p1698982951518"></a>0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="22.79%" headers="mcps1.2.6.1.5 "><p id="p139898299151"><a name="p139898299151"></a><a name="p139898299151"></a>0</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 29**  CastDequant特殊值/边界值输入的计算结果（MAX/MIN 对应操作数对应数据类型下的最大值和最小值，scale=1，offset=0）

<a name="table18277108174810"></a>
<table><thead align="left"><tr id="row20277108164812"><th class="cellrowborder" valign="top" width="12.370000000000001%" id="mcps1.2.5.1.1"><p id="p12355239104812"><a name="p12355239104812"></a><a name="p12355239104812"></a>src数据类型/dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="21.67%" id="mcps1.2.5.1.2"><p id="p142779874815"><a name="p142779874815"></a><a name="p142779874815"></a>src取值</p>
</th>
<th class="cellrowborder" valign="top" width="31.290000000000003%" id="mcps1.2.5.1.3"><p id="p42782854811"><a name="p42782854811"></a><a name="p42782854811"></a>dst输出（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="34.67%" id="mcps1.2.5.1.4"><p id="p22781881481"><a name="p22781881481"></a><a name="p22781881481"></a>dst输出（饱和模式，仅输出为half数据类型时支持）</p>
</th>
</tr>
</thead>
<tbody><tr id="row2027808204818"><td class="cellrowborder" rowspan="5" align="center" valign="middle" width="12.370000000000001%" headers="mcps1.2.5.1.1 "><p id="p29961421185110"><a name="p29961421185110"></a><a name="p29961421185110"></a>int32_t/half</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="21.67%" headers="mcps1.2.5.1.2 "><p id="p162781289482"><a name="p162781289482"></a><a name="p162781289482"></a>0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="31.290000000000003%" headers="mcps1.2.5.1.3 "><p id="p82798813489"><a name="p82798813489"></a><a name="p82798813489"></a>0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="34.67%" headers="mcps1.2.5.1.4 "><p id="p12798894811"><a name="p12798894811"></a><a name="p12798894811"></a>0</p>
</td>
</tr>
<tr id="row22791815486"><td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.1 "><p id="p10221138543"><a name="p10221138543"></a><a name="p10221138543"></a>65504</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.2 "><p id="p122807819482"><a name="p122807819482"></a><a name="p122807819482"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.3 "><p id="p132806864812"><a name="p132806864812"></a><a name="p132806864812"></a>MAX</p>
</td>
</tr>
<tr id="row1959710415562"><td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.1 "><p id="p11597174145619"><a name="p11597174145619"></a><a name="p11597174145619"></a>-65504</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.2 "><p id="p105979418563"><a name="p105979418563"></a><a name="p105979418563"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.3 "><p id="p55971041145616"><a name="p55971041145616"></a><a name="p55971041145616"></a>MIN</p>
</td>
</tr>
<tr id="row228012814819"><td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.1 "><p id="p76617365813"><a name="p76617365813"></a><a name="p76617365813"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.2 "><p id="p728117812486"><a name="p728117812486"></a><a name="p728117812486"></a>+inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.3 "><p id="p1128168184815"><a name="p1128168184815"></a><a name="p1128168184815"></a>MAX</p>
</td>
</tr>
<tr id="row122811386484"><td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.1 "><p id="p2028198164816"><a name="p2028198164816"></a><a name="p2028198164816"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.2 "><p id="p158772542545"><a name="p158772542545"></a><a name="p158772542545"></a>-inf</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.3 "><p id="p182813894813"><a name="p182813894813"></a><a name="p182813894813"></a>MIN</p>
</td>
</tr>
<tr id="row13579102816587"><td class="cellrowborder" rowspan="3" align="center" valign="middle" width="12.370000000000001%" headers="mcps1.2.5.1.1 "><p id="p1457992817589"><a name="p1457992817589"></a><a name="p1457992817589"></a>int16_t/int8_t</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="21.67%" headers="mcps1.2.5.1.2 "><p id="p057914281580"><a name="p057914281580"></a><a name="p057914281580"></a>0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="31.290000000000003%" headers="mcps1.2.5.1.3 "><p id="p1157992820580"><a name="p1157992820580"></a><a name="p1157992820580"></a>0</p>
</td>
<td class="cellrowborder" align="center" valign="middle" width="34.67%" headers="mcps1.2.5.1.4 "><p id="p157962865810"><a name="p157962865810"></a><a name="p157962865810"></a>/</p>
</td>
</tr>
<tr id="row57111051201"><td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.1 "><p id="p177111657018"><a name="p177111657018"></a><a name="p177111657018"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.2 "><p id="p2071112515016"><a name="p2071112515016"></a><a name="p2071112515016"></a>MAX</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.3 "><p id="p47111755014"><a name="p47111755014"></a><a name="p47111755014"></a>/</p>
</td>
</tr>
<tr id="row17751971208"><td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.1 "><p id="p14775374018"><a name="p14775374018"></a><a name="p14775374018"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.2 "><p id="p13775107109"><a name="p13775107109"></a><a name="p13775107109"></a>MIN</p>
</td>
<td class="cellrowborder" align="center" valign="middle" headers="mcps1.2.5.1.3 "><p id="p187751871504"><a name="p187751871504"></a><a name="p187751871504"></a>/</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

## 比较与选择<a name="section829519484517"></a>

GetCmpMask为设置比较掩码寄存器的数据搬运操作，不涉及特殊值/边界值。

**表 30**  Compare 特殊值/边界值输入的计算结果

<a name="table1961220798"></a>
<table><thead align="left"><tr id="row156282018918"><th class="cellrowborder" valign="top" width="12.07%" id="mcps1.2.7.1.1"><p id="p61671852499"><a name="p61671852499"></a><a name="p61671852499"></a>src数据类型/src1数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="10.100000000000001%" id="mcps1.2.7.1.2"><p id="p030154581018"><a name="p030154581018"></a><a name="p030154581018"></a>cmpMode比较模式</p>
</th>
<th class="cellrowborder" valign="top" width="13.98%" id="mcps1.2.7.1.3"><p id="p11671752099"><a name="p11671752099"></a><a name="p11671752099"></a>src0数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="36.09%" id="mcps1.2.7.1.4"><p id="p316865216920"><a name="p316865216920"></a><a name="p316865216920"></a>src1数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="15.2%" id="mcps1.2.7.1.5"><p id="p1990365431913"><a name="p1990365431913"></a><a name="p1990365431913"></a>dst输出数据（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="12.559999999999999%" id="mcps1.2.7.1.6"><p id="p1790315547196"><a name="p1790315547196"></a><a name="p1790315547196"></a>dst输出数据（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row96292010914"><td class="cellrowborder" rowspan="42" valign="top" width="12.07%" headers="mcps1.2.7.1.1 "><p id="p10621720490"><a name="p10621720490"></a><a name="p10621720490"></a>half/half</p>
</td>
<td class="cellrowborder" rowspan="7" valign="middle" width="10.100000000000001%" headers="mcps1.2.7.1.2 "><p id="p6301545171014"><a name="p6301545171014"></a><a name="p6301545171014"></a>CMPMODE::EQ</p>
</td>
<td class="cellrowborder" valign="middle" width="13.98%" headers="mcps1.2.7.1.3 "><p id="p38941045144414"><a name="p38941045144414"></a><a name="p38941045144414"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="middle" width="36.09%" headers="mcps1.2.7.1.4 "><p id="p14927482465"><a name="p14927482465"></a><a name="p14927482465"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="15.2%" headers="mcps1.2.7.1.5 "><p id="p2048119717599"><a name="p2048119717599"></a><a name="p2048119717599"></a>1/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" width="12.559999999999999%" headers="mcps1.2.7.1.6 "><p id="p8221221519"><a name="p8221221519"></a><a name="p8221221519"></a>1/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row9458121861916"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1278872331414"><a name="p1278872331414"></a><a name="p1278872331414"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p132774324144"><a name="p132774324144"></a><a name="p132774324144"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p666612511155"><a name="p666612511155"></a><a name="p666612511155"></a>0/1/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p5169194813163"><a name="p5169194813163"></a><a name="p5169194813163"></a>0/1/0/0/0/0/0</p>
</td>
</tr>
<tr id="row106491018191912"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p0788122391419"><a name="p0788122391419"></a><a name="p0788122391419"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p094193214141"><a name="p094193214141"></a><a name="p094193214141"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p76669512159"><a name="p76669512159"></a><a name="p76669512159"></a>0/0/1/1/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1816910482162"><a name="p1816910482162"></a><a name="p1816910482162"></a>0/0/1/1/0/0/0</p>
</td>
</tr>
<tr id="row5127152065413"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p97881523131417"><a name="p97881523131417"></a><a name="p97881523131417"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p857320339147"><a name="p857320339147"></a><a name="p857320339147"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p116661159157"><a name="p116661159157"></a><a name="p116661159157"></a>0/0/1/1/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p191691448141615"><a name="p191691448141615"></a><a name="p191691448141615"></a>0/0/1/1/0/0/0</p>
</td>
</tr>
<tr id="row128471818161911"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p19789923111420"><a name="p19789923111420"></a><a name="p19789923111420"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1520443413148"><a name="p1520443413148"></a><a name="p1520443413148"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p66655515155"><a name="p66655515155"></a><a name="p66655515155"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p11169174812163"><a name="p11169174812163"></a><a name="p11169174812163"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row13721971913"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17789123181417"><a name="p17789123181417"></a><a name="p17789123181417"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p8843163471415"><a name="p8843163471415"></a><a name="p8843163471415"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1666515111510"><a name="p1666515111510"></a><a name="p1666515111510"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p16169184821611"><a name="p16169184821611"></a><a name="p16169184821611"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row8252101913193"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p3789112341419"><a name="p3789112341419"></a><a name="p3789112341419"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1248053520145"><a name="p1248053520145"></a><a name="p1248053520145"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p76641756157"><a name="p76641756157"></a><a name="p76641756157"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p20170184812165"><a name="p20170184812165"></a><a name="p20170184812165"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row1090116910456"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p143014451100"><a name="p143014451100"></a><a name="p143014451100"></a>CMPMODE::GE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p204971944189"><a name="p204971944189"></a><a name="p204971944189"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1749717451812"><a name="p1749717451812"></a><a name="p1749717451812"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p93732171599"><a name="p93732171599"></a><a name="p93732171599"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p886484913246"><a name="p886484913246"></a><a name="p886484913246"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row193318944514"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p049714413186"><a name="p049714413186"></a><a name="p049714413186"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p849784101813"><a name="p849784101813"></a><a name="p849784101813"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1537311711593"><a name="p1537311711593"></a><a name="p1537311711593"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p7864154912411"><a name="p7864154912411"></a><a name="p7864154912411"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row6134294455"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1549717451818"><a name="p1549717451818"></a><a name="p1549717451818"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p164978411811"><a name="p164978411811"></a><a name="p164978411811"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p937311717592"><a name="p937311717592"></a><a name="p937311717592"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p98641749132419"><a name="p98641749132419"></a><a name="p98641749132419"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row167063289544"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p849812412185"><a name="p849812412185"></a><a name="p849812412185"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p18498548189"><a name="p18498548189"></a><a name="p18498548189"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1320764019234"><a name="p1320764019234"></a><a name="p1320764019234"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1086514492244"><a name="p1086514492244"></a><a name="p1086514492244"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row89535817452"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p249814111815"><a name="p249814111815"></a><a name="p249814111815"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p94981440184"><a name="p94981440184"></a><a name="p94981440184"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p0373117105917"><a name="p0373117105917"></a><a name="p0373117105917"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p98657494245"><a name="p98657494245"></a><a name="p98657494245"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row197795874518"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1549819413183"><a name="p1549819413183"></a><a name="p1549819413183"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p144982414183"><a name="p144982414183"></a><a name="p144982414183"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p737315179597"><a name="p737315179597"></a><a name="p737315179597"></a>1/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1086514914246"><a name="p1086514914246"></a><a name="p1086514914246"></a>1/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row17595198114510"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p4498194161813"><a name="p4498194161813"></a><a name="p4498194161813"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p184981144182"><a name="p184981144182"></a><a name="p184981144182"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p19373131715599"><a name="p19373131715599"></a><a name="p19373131715599"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p12865174982418"><a name="p12865174982418"></a><a name="p12865174982418"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row7390884455"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p1331134561012"><a name="p1331134561012"></a><a name="p1331134561012"></a>CMPMODE::GT</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p529254210187"><a name="p529254210187"></a><a name="p529254210187"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1429216423186"><a name="p1429216423186"></a><a name="p1429216423186"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p3809830145920"><a name="p3809830145920"></a><a name="p3809830145920"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p25211350112614"><a name="p25211350112614"></a><a name="p25211350112614"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row114751757144710"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p929364251819"><a name="p929364251819"></a><a name="p929364251819"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1429315426187"><a name="p1429315426187"></a><a name="p1429315426187"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1480983016595"><a name="p1480983016595"></a><a name="p1480983016595"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p19013182719"><a name="p19013182719"></a><a name="p19013182719"></a>/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row7650957114714"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p62938429186"><a name="p62938429186"></a><a name="p62938429186"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p629334231814"><a name="p629334231814"></a><a name="p629334231814"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p124941321122616"><a name="p124941321122616"></a><a name="p124941321122616"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1495110325264"><a name="p1495110325264"></a><a name="p1495110325264"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row97793810547"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p172931842131814"><a name="p172931842131814"></a><a name="p172931842131814"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1829314424188"><a name="p1829314424188"></a><a name="p1829314424188"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p17990530182618"><a name="p17990530182618"></a><a name="p17990530182618"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p14417153472610"><a name="p14417153472610"></a><a name="p14417153472610"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row083413572477"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p132934422186"><a name="p132934422186"></a><a name="p132934422186"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p2029384213184"><a name="p2029384213184"></a><a name="p2029384213184"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1780903005914"><a name="p1780903005914"></a><a name="p1780903005914"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p71554919279"><a name="p71554919279"></a><a name="p71554919279"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row31825811473"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p72941942191818"><a name="p72941942191818"></a><a name="p72941942191818"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p4294174212189"><a name="p4294174212189"></a><a name="p4294174212189"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p9809193011598"><a name="p9809193011598"></a><a name="p9809193011598"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p13433132692819"><a name="p13433132692819"></a><a name="p13433132692819"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row1119925813476"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p9294144219188"><a name="p9294144219188"></a><a name="p9294144219188"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1229484217184"><a name="p1229484217184"></a><a name="p1229484217184"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p178091230125917"><a name="p178091230125917"></a><a name="p178091230125917"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p11433172672812"><a name="p11433172672812"></a><a name="p11433172672812"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row10681965511"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p331104511101"><a name="p331104511101"></a><a name="p331104511101"></a>CMPMODE::LE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p4617184531810"><a name="p4617184531810"></a><a name="p4617184531810"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p176171445201811"><a name="p176171445201811"></a><a name="p176171445201811"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p106501342165919"><a name="p106501342165919"></a><a name="p106501342165919"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p125062019329"><a name="p125062019329"></a><a name="p125062019329"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row185362091550"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p66171145141820"><a name="p66171145141820"></a><a name="p66171145141820"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p11618144515185"><a name="p11618144515185"></a><a name="p11618144515185"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p46501425594"><a name="p46501425594"></a><a name="p46501425594"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p19506140123218"><a name="p19506140123218"></a><a name="p19506140123218"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row973813918558"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p18618134513188"><a name="p18618134513188"></a><a name="p18618134513188"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p7618545151811"><a name="p7618545151811"></a><a name="p7618545151811"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p465018421598"><a name="p465018421598"></a><a name="p465018421598"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p6506708322"><a name="p6506708322"></a><a name="p6506708322"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row11979169155511"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p161812452182"><a name="p161812452182"></a><a name="p161812452182"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p19618194591817"><a name="p19618194591817"></a><a name="p19618194591817"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p16501442125910"><a name="p16501442125910"></a><a name="p16501442125910"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p5506150153217"><a name="p5506150153217"></a><a name="p5506150153217"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row18210201095519"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p11618124518181"><a name="p11618124518181"></a><a name="p11618124518181"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p16181645181810"><a name="p16181645181810"></a><a name="p16181645181810"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1265034245913"><a name="p1265034245913"></a><a name="p1265034245913"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p7507100143219"><a name="p7507100143219"></a><a name="p7507100143219"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row644881011557"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p106183455184"><a name="p106183455184"></a><a name="p106183455184"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p261854591819"><a name="p261854591819"></a><a name="p261854591819"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p15650124210598"><a name="p15650124210598"></a><a name="p15650124210598"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1050718043210"><a name="p1050718043210"></a><a name="p1050718043210"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row17816810125518"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17619164561819"><a name="p17619164561819"></a><a name="p17619164561819"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p176191445191811"><a name="p176191445191811"></a><a name="p176191445191811"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p186501242145913"><a name="p186501242145913"></a><a name="p186501242145913"></a>1/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p45077014323"><a name="p45077014323"></a><a name="p45077014323"></a>1/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row114011258164712"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p83112452106"><a name="p83112452106"></a><a name="p83112452106"></a>CMPMODE::LT</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p76953498182"><a name="p76953498182"></a><a name="p76953498182"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p869520495188"><a name="p869520495188"></a><a name="p869520495188"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1578910191604"><a name="p1578910191604"></a><a name="p1578910191604"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p673494510355"><a name="p673494510355"></a><a name="p673494510355"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row1585658194719"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p13695049191817"><a name="p13695049191817"></a><a name="p13695049191817"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1569516493187"><a name="p1569516493187"></a><a name="p1569516493187"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p14789111911012"><a name="p14789111911012"></a><a name="p14789111911012"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p19734164503518"><a name="p19734164503518"></a><a name="p19734164503518"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row977235815479"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p169534914187"><a name="p169534914187"></a><a name="p169534914187"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p469624911818"><a name="p469624911818"></a><a name="p469624911818"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3789121912020"><a name="p3789121912020"></a><a name="p3789121912020"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2734144523517"><a name="p2734144523517"></a><a name="p2734144523517"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row467644375417"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p196961449131815"><a name="p196961449131815"></a><a name="p196961449131815"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p8696164919186"><a name="p8696164919186"></a><a name="p8696164919186"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p478915191702"><a name="p478915191702"></a><a name="p478915191702"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1773514513510"><a name="p1773514513510"></a><a name="p1773514513510"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row1294919587476"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p469613494186"><a name="p469613494186"></a><a name="p469613494186"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p10697114961819"><a name="p10697114961819"></a><a name="p10697114961819"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p37891719408"><a name="p37891719408"></a><a name="p37891719408"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1073515452355"><a name="p1073515452355"></a><a name="p1073515452355"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row1013885910477"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1169714911813"><a name="p1169714911813"></a><a name="p1169714911813"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p156971497189"><a name="p156971497189"></a><a name="p156971497189"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p17309315143514"><a name="p17309315143514"></a><a name="p17309315143514"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p773544510355"><a name="p773544510355"></a><a name="p773544510355"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row12314115910472"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p10697124911813"><a name="p10697124911813"></a><a name="p10697124911813"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p9697184981813"><a name="p9697184981813"></a><a name="p9697184981813"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p178911195014"><a name="p178911195014"></a><a name="p178911195014"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p177351745133516"><a name="p177351745133516"></a><a name="p177351745133516"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row165091759154717"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p1831164571016"><a name="p1831164571016"></a><a name="p1831164571016"></a>CMPMODE::NE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p410019526182"><a name="p410019526182"></a><a name="p410019526182"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p12100105211182"><a name="p12100105211182"></a><a name="p12100105211182"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p147409319015"><a name="p147409319015"></a><a name="p147409319015"></a>0/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p9887213113017"><a name="p9887213113017"></a><a name="p9887213113017"></a>0/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row6304612114811"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1010095213187"><a name="p1010095213187"></a><a name="p1010095213187"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p10101125220186"><a name="p10101125220186"></a><a name="p10101125220186"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p16740631301"><a name="p16740631301"></a><a name="p16740631301"></a>1/0/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p288714137307"><a name="p288714137307"></a><a name="p288714137307"></a>1/0/1/1/0/1/1</p>
</td>
</tr>
<tr id="row49908592478"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p191018522180"><a name="p191018522180"></a><a name="p191018522180"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p61011152131814"><a name="p61011152131814"></a><a name="p61011152131814"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p167403311707"><a name="p167403311707"></a><a name="p167403311707"></a>1/1/0/0/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p78875132309"><a name="p78875132309"></a><a name="p78875132309"></a>1/1/0/0/0/1/1</p>
</td>
</tr>
<tr id="row17286486541"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p151019529183"><a name="p151019529183"></a><a name="p151019529183"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p8101152181812"><a name="p8101152181812"></a><a name="p8101152181812"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p674018311808"><a name="p674018311808"></a><a name="p674018311808"></a>1/1/0/0/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p68876137306"><a name="p68876137306"></a><a name="p68876137306"></a>1/1/0/0/0/1/1</p>
</td>
</tr>
<tr id="row915438124511"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p210117528180"><a name="p210117528180"></a><a name="p210117528180"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p19101195221810"><a name="p19101195221810"></a><a name="p19101195221810"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p9376132211811"><a name="p9376132211811"></a><a name="p9376132211811"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1988717134306"><a name="p1988717134306"></a><a name="p1988717134306"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row3137197134513"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1510111525182"><a name="p1510111525182"></a><a name="p1510111525182"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p61011852101810"><a name="p61011852101810"></a><a name="p61011852101810"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p174011316014"><a name="p174011316014"></a><a name="p174011316014"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p148881213173016"><a name="p148881213173016"></a><a name="p148881213173016"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row96015581442"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p6102185218189"><a name="p6102185218189"></a><a name="p6102185218189"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p610225210186"><a name="p610225210186"></a><a name="p610225210186"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1774016314015"><a name="p1774016314015"></a><a name="p1774016314015"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p98888135308"><a name="p98888135308"></a><a name="p98888135308"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row8623203916"><td class="cellrowborder" rowspan="42" valign="top" width="12.07%" headers="mcps1.2.7.1.1 "><p id="p6621420297"><a name="p6621420297"></a><a name="p6621420297"></a>float/float</p>
</td>
<td class="cellrowborder" rowspan="7" valign="middle" width="10.100000000000001%" headers="mcps1.2.7.1.2 "><p id="p1950112274311"><a name="p1950112274311"></a><a name="p1950112274311"></a>CMPMODE::EQ</p>
</td>
<td class="cellrowborder" valign="middle" width="13.98%" headers="mcps1.2.7.1.3 "><p id="p10587101194519"><a name="p10587101194519"></a><a name="p10587101194519"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="36.09%" headers="mcps1.2.7.1.4 "><p id="p122441156111917"><a name="p122441156111917"></a><a name="p122441156111917"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="15.2%" headers="mcps1.2.7.1.5 "><p id="p072674617515"><a name="p072674617515"></a><a name="p072674617515"></a>1/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" width="12.559999999999999%" headers="mcps1.2.7.1.6 "><p id="p1697024121520"><a name="p1697024121520"></a><a name="p1697024121520"></a>1/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row55511253131914"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17587131124517"><a name="p17587131124517"></a><a name="p17587131124517"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p119121824185512"><a name="p119121824185512"></a><a name="p119121824185512"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p0726246057"><a name="p0726246057"></a><a name="p0726246057"></a>0/1/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p16970341131512"><a name="p16970341131512"></a><a name="p16970341131512"></a>0/1/0/0/0/0/0</p>
</td>
</tr>
<tr id="row687410536196"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p175870119456"><a name="p175870119456"></a><a name="p175870119456"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p185684258558"><a name="p185684258558"></a><a name="p185684258558"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p9726174616512"><a name="p9726174616512"></a><a name="p9726174616512"></a>0/0/1/1/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1297115417156"><a name="p1297115417156"></a><a name="p1297115417156"></a>0/0/1/1/0/0/0</p>
</td>
</tr>
<tr id="row8811854141920"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1058721174519"><a name="p1058721174519"></a><a name="p1058721174519"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p5190326165519"><a name="p5190326165519"></a><a name="p5190326165519"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p272617461653"><a name="p272617461653"></a><a name="p272617461653"></a>0/0/1/1/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p169711341161515"><a name="p169711341161515"></a><a name="p169711341161515"></a>0/0/1/1/0/0/0</p>
</td>
</tr>
<tr id="row15286175419191"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p358891117459"><a name="p358891117459"></a><a name="p358891117459"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p08187265552"><a name="p08187265552"></a><a name="p08187265552"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p172644610514"><a name="p172644610514"></a><a name="p172644610514"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p597144131510"><a name="p597144131510"></a><a name="p597144131510"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row84571254121919"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1588111194519"><a name="p1588111194519"></a><a name="p1588111194519"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p114491227145520"><a name="p114491227145520"></a><a name="p114491227145520"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p16726164610518"><a name="p16726164610518"></a><a name="p16726164610518"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p13971144141512"><a name="p13971144141512"></a><a name="p13971144141512"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row1263855441915"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p25883110455"><a name="p25883110455"></a><a name="p25883110455"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14741228155514"><a name="p14741228155514"></a><a name="p14741228155514"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p207263461956"><a name="p207263461956"></a><a name="p207263461956"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p14971194111510"><a name="p14971194111510"></a><a name="p14971194111510"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row1928112819115"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p09503227438"><a name="p09503227438"></a><a name="p09503227438"></a>CMPMODE::GE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p16871301563"><a name="p16871301563"></a><a name="p16871301563"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p14696152810554"><a name="p14696152810554"></a><a name="p14696152810554"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2053463517414"><a name="p2053463517414"></a><a name="p2053463517414"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p79711841191510"><a name="p79711841191510"></a><a name="p79711841191510"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row1070910811115"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p16872100175619"><a name="p16872100175619"></a><a name="p16872100175619"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1931511290559"><a name="p1931511290559"></a><a name="p1931511290559"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1353419357414"><a name="p1353419357414"></a><a name="p1353419357414"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p897244181519"><a name="p897244181519"></a><a name="p897244181519"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row9533918111"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p198720095611"><a name="p198720095611"></a><a name="p198720095611"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p11950172915559"><a name="p11950172915559"></a><a name="p11950172915559"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1653412351744"><a name="p1653412351744"></a><a name="p1653412351744"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p18972741191519"><a name="p18972741191519"></a><a name="p18972741191519"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row163306913112"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1487210205614"><a name="p1487210205614"></a><a name="p1487210205614"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p657233012550"><a name="p657233012550"></a><a name="p657233012550"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1945107165710"><a name="p1945107165710"></a><a name="p1945107165710"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p297211412157"><a name="p297211412157"></a><a name="p297211412157"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row06604914112"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p987290175618"><a name="p987290175618"></a><a name="p987290175618"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p2019414319551"><a name="p2019414319551"></a><a name="p2019414319551"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p145341235940"><a name="p145341235940"></a><a name="p145341235940"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p7972154111151"><a name="p7972154111151"></a><a name="p7972154111151"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row109981491214"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p48721302565"><a name="p48721302565"></a><a name="p48721302565"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1282963175513"><a name="p1282963175513"></a><a name="p1282963175513"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p17535735444"><a name="p17535735444"></a><a name="p17535735444"></a>1/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1972194117157"><a name="p1972194117157"></a><a name="p1972194117157"></a>1/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row345211010112"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p687217019568"><a name="p687217019568"></a><a name="p687217019568"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p446083225510"><a name="p446083225510"></a><a name="p446083225510"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p153519356416"><a name="p153519356416"></a><a name="p153519356416"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p39721141141514"><a name="p39721141141514"></a><a name="p39721141141514"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row1314515171118"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p129516224438"><a name="p129516224438"></a><a name="p129516224438"></a>CMPMODE::GT</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p13432512125917"><a name="p13432512125917"></a><a name="p13432512125917"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1485113315511"><a name="p1485113315511"></a><a name="p1485113315511"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p10511018618"><a name="p10511018618"></a><a name="p10511018618"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p10973941151512"><a name="p10973941151512"></a><a name="p10973941151512"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row23426170117"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p8432412175911"><a name="p8432412175911"></a><a name="p8432412175911"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p271917337557"><a name="p271917337557"></a><a name="p271917337557"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p05110111619"><a name="p05110111619"></a><a name="p05110111619"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p11973941181513"><a name="p11973941181513"></a><a name="p11973941181513"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row8593171714111"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p143241215918"><a name="p143241215918"></a><a name="p143241215918"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p5345934125512"><a name="p5345934125512"></a><a name="p5345934125512"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p155111110610"><a name="p155111110610"></a><a name="p155111110610"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p16973144117158"><a name="p16973144117158"></a><a name="p16973144117158"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row48383171216"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p74323122598"><a name="p74323122598"></a><a name="p74323122598"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p9966153475515"><a name="p9966153475515"></a><a name="p9966153475515"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p11511414610"><a name="p11511414610"></a><a name="p11511414610"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p897354113152"><a name="p897354113152"></a><a name="p897354113152"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row870151815116"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p84321122595"><a name="p84321122595"></a><a name="p84321122595"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p18579935155511"><a name="p18579935155511"></a><a name="p18579935155511"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p15111111614"><a name="p15111111614"></a><a name="p15111111614"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p9973154110158"><a name="p9973154110158"></a><a name="p9973154110158"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row13336318910"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p743211125593"><a name="p743211125593"></a><a name="p743211125593"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p102083368551"><a name="p102083368551"></a><a name="p102083368551"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1451812069"><a name="p1451812069"></a><a name="p1451812069"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p9974041121511"><a name="p9974041121511"></a><a name="p9974041121511"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row96212191119"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p144331712185914"><a name="p144331712185914"></a><a name="p144331712185914"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p138321136115515"><a name="p138321136115515"></a><a name="p138321136115515"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p155116114612"><a name="p155116114612"></a><a name="p155116114612"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p12974124171510"><a name="p12974124171510"></a><a name="p12974124171510"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row182318314115"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p1395113229438"><a name="p1395113229438"></a><a name="p1395113229438"></a>CMPMODE::LE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p10131153115620"><a name="p10131153115620"></a><a name="p10131153115620"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1845619371558"><a name="p1845619371558"></a><a name="p1845619371558"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p437615377612"><a name="p437615377612"></a><a name="p437615377612"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p129743416151"><a name="p129743416151"></a><a name="p129743416151"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row1428517311418"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p161314314564"><a name="p161314314564"></a><a name="p161314314564"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17891138165515"><a name="p17891138165515"></a><a name="p17891138165515"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p93765373612"><a name="p93765373612"></a><a name="p93765373612"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p199742414156"><a name="p199742414156"></a><a name="p199742414156"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row1347510311118"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p131311638561"><a name="p131311638561"></a><a name="p131311638561"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1872223813551"><a name="p1872223813551"></a><a name="p1872223813551"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p183765378612"><a name="p183765378612"></a><a name="p183765378612"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1497419419153"><a name="p1497419419153"></a><a name="p1497419419153"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row1070811314110"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p81311337562"><a name="p81311337562"></a><a name="p81311337562"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1634523995517"><a name="p1634523995517"></a><a name="p1634523995517"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p593926263"><a name="p593926263"></a><a name="p593926263"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1797454141520"><a name="p1797454141520"></a><a name="p1797454141520"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row139901311619"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1913114355615"><a name="p1913114355615"></a><a name="p1913114355615"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p7973183915556"><a name="p7973183915556"></a><a name="p7973183915556"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p637693713617"><a name="p637693713617"></a><a name="p637693713617"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p5974124116157"><a name="p5974124116157"></a><a name="p5974124116157"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row92665321211"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p15131163195618"><a name="p15131163195618"></a><a name="p15131163195618"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p12604144017552"><a name="p12604144017552"></a><a name="p12604144017552"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p63764375612"><a name="p63764375612"></a><a name="p63764375612"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1097554117155"><a name="p1097554117155"></a><a name="p1097554117155"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row115828327115"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p151314319561"><a name="p151314319561"></a><a name="p151314319561"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p92341041135510"><a name="p92341041135510"></a><a name="p92341041135510"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p337617371460"><a name="p337617371460"></a><a name="p337617371460"></a>1/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p997584120153"><a name="p997584120153"></a><a name="p997584120153"></a>1/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row1982043719115"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p1095212284315"><a name="p1095212284315"></a><a name="p1095212284315"></a>CMPMODE::LT</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p2949142566"><a name="p2949142566"></a><a name="p2949142566"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1786544114555"><a name="p1786544114555"></a><a name="p1786544114555"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1971213191476"><a name="p1971213191476"></a><a name="p1971213191476"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p2097524117156"><a name="p2097524117156"></a><a name="p2097524117156"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row13714381411"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p8949843565"><a name="p8949843565"></a><a name="p8949843565"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p649814429554"><a name="p649814429554"></a><a name="p649814429554"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p37121219170"><a name="p37121219170"></a><a name="p37121219170"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1897519413157"><a name="p1897519413157"></a><a name="p1897519413157"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row1223519380113"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p209493475612"><a name="p209493475612"></a><a name="p209493475612"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1812174313553"><a name="p1812174313553"></a><a name="p1812174313553"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p117121319279"><a name="p117121319279"></a><a name="p117121319279"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p16975134161511"><a name="p16975134161511"></a><a name="p16975134161511"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row845115381018"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p394914413568"><a name="p394914413568"></a><a name="p394914413568"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14746144385516"><a name="p14746144385516"></a><a name="p14746144385516"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1271211198714"><a name="p1271211198714"></a><a name="p1271211198714"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p119756419154"><a name="p119756419154"></a><a name="p119756419154"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row137013382011"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p19949647562"><a name="p19949647562"></a><a name="p19949647562"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p183691944205518"><a name="p183691944205518"></a><a name="p183691944205518"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p57121197717"><a name="p57121197717"></a><a name="p57121197717"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p10976154171519"><a name="p10976154171519"></a><a name="p10976154171519"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row4932143819112"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p294914185620"><a name="p294914185620"></a><a name="p294914185620"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p311045105517"><a name="p311045105517"></a><a name="p311045105517"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p2713111910712"><a name="p2713111910712"></a><a name="p2713111910712"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1097634116155"><a name="p1097634116155"></a><a name="p1097634116155"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row2018943913111"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1949344569"><a name="p1949344569"></a><a name="p1949344569"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17635345145519"><a name="p17635345145519"></a><a name="p17635345145519"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p071317191173"><a name="p071317191173"></a><a name="p071317191173"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p997611411155"><a name="p997611411155"></a><a name="p997611411155"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row42561845214"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p7952422184311"><a name="p7952422184311"></a><a name="p7952422184311"></a>CMPMODE::NE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p18091613566"><a name="p18091613566"></a><a name="p18091613566"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p52691046155517"><a name="p52691046155517"></a><a name="p52691046155517"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p286914271184"><a name="p286914271184"></a><a name="p286914271184"></a>0/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p12976164181510"><a name="p12976164181510"></a><a name="p12976164181510"></a>0/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row15454104511117"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p280917618560"><a name="p280917618560"></a><a name="p280917618560"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1890494619551"><a name="p1890494619551"></a><a name="p1890494619551"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p208698273816"><a name="p208698273816"></a><a name="p208698273816"></a>1/0/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p4976941121520"><a name="p4976941121520"></a><a name="p4976941121520"></a>1/0/1/1/0/1/1</p>
</td>
</tr>
<tr id="row12699174519116"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p28091368564"><a name="p28091368564"></a><a name="p28091368564"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1853374711558"><a name="p1853374711558"></a><a name="p1853374711558"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p18869182718810"><a name="p18869182718810"></a><a name="p18869182718810"></a>1/1/0/0/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p89760413153"><a name="p89760413153"></a><a name="p89760413153"></a>1/1/0/0/0/1/1</p>
</td>
</tr>
<tr id="row1993711451419"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p38101669562"><a name="p38101669562"></a><a name="p38101669562"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p11168144875518"><a name="p11168144875518"></a><a name="p11168144875518"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p9623203818144"><a name="p9623203818144"></a><a name="p9623203818144"></a>1/1/0/0/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1397711411153"><a name="p1397711411153"></a><a name="p1397711411153"></a>1/1/0/0/0/1/1</p>
</td>
</tr>
<tr id="row016510461716"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p781096155615"><a name="p781096155615"></a><a name="p781096155615"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p19791124805519"><a name="p19791124805519"></a><a name="p19791124805519"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1850419425144"><a name="p1850419425144"></a><a name="p1850419425144"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1519771931818"><a name="p1519771931818"></a><a name="p1519771931818"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row441119468118"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p18810156185614"><a name="p18810156185614"></a><a name="p18810156185614"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p541624965514"><a name="p541624965514"></a><a name="p541624965514"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p2869727782"><a name="p2869727782"></a><a name="p2869727782"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p7977194141513"><a name="p7977194141513"></a><a name="p7977194141513"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row136871446619"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p11810768564"><a name="p11810768564"></a><a name="p11810768564"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p164517502555"><a name="p164517502555"></a><a name="p164517502555"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3869182714811"><a name="p3869182714811"></a><a name="p3869182714811"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p69770414159"><a name="p69770414159"></a><a name="p69770414159"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row1945910533119"><td class="cellrowborder" rowspan="2" valign="top" width="12.07%" headers="mcps1.2.7.1.1 "><p id="p6818125421917"><a name="p6818125421917"></a><a name="p6818125421917"></a>int32_t/int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="middle" width="10.100000000000001%" headers="mcps1.2.7.1.2 "><p id="p1032144511106"><a name="p1032144511106"></a><a name="p1032144511106"></a>CMPMODE::EQ</p>
</td>
<td class="cellrowborder" valign="middle" width="13.98%" headers="mcps1.2.7.1.3 "><p id="p1412232292110"><a name="p1412232292110"></a><a name="p1412232292110"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="36.09%" headers="mcps1.2.7.1.4 "><p id="p6807161521017"><a name="p6807161521017"></a><a name="p6807161521017"></a>-2147483648（MIN）/2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="15.2%" headers="mcps1.2.7.1.5 "><p id="p3287481100"><a name="p3287481100"></a><a name="p3287481100"></a>1/0</p>
</td>
<td class="cellrowborder" valign="top" width="12.559999999999999%" headers="mcps1.2.7.1.6 "><p id="p1483214215489"><a name="p1483214215489"></a><a name="p1483214215489"></a>1/0</p>
</td>
</tr>
<tr id="row13818854101917"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1397818109477"><a name="p1397818109477"></a><a name="p1397818109477"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p3818175415198"><a name="p3818175415198"></a><a name="p3818175415198"></a>-2147483648（MIN）/2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p92884821015"><a name="p92884821015"></a><a name="p92884821015"></a>0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p198321721489"><a name="p198321721489"></a><a name="p198321721489"></a>0/1</p>
</td>
</tr>
</tbody>
</table>

注：

-   CMPMODE::NE模式中，在数据类型为half和float时，Compare接口计算结果存在与IEEE754理论结果不同：
    -   src0 = nan，src1 = MIN/MAX/+0/-0/nan/+inf/-inf 时，实际结果：dst输出数据（INF/NAN模式和饱和模式）均为0/0/0/0/0/0/0，IEEE754理论结果：1/1/1/1/1/1/1
    -   src0 = MIN/MAX/+0/-0/nan/+inf/-inf，src1 =nan 时，实际结果：dst输出数据（INF/NAN模式和饱和模式）均为0/0/0/0/0/0/0，IEEE754理论结果：1/1/1/1/1/1/1

-   在其余模式中，dst输出数据（INF/NAN模式）、dst输出数据（饱和模式）结果，与IEEE754理论结果一致。

**表 31**  Compares 特殊值/边界值输入的计算结果

<a name="table1126411512176"></a>
<table><thead align="left"><tr id="row526515112179"><th class="cellrowborder" valign="top" width="12.07%" id="mcps1.2.7.1.1"><p id="p4265105114175"><a name="p4265105114175"></a><a name="p4265105114175"></a>src数据类型/src1数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="10.100000000000001%" id="mcps1.2.7.1.2"><p id="p17265115120179"><a name="p17265115120179"></a><a name="p17265115120179"></a>cmpMode比较模式</p>
</th>
<th class="cellrowborder" valign="top" width="13.98%" id="mcps1.2.7.1.3"><p id="p4265951101713"><a name="p4265951101713"></a><a name="p4265951101713"></a>src0数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="36.09%" id="mcps1.2.7.1.4"><p id="p2265115161716"><a name="p2265115161716"></a><a name="p2265115161716"></a>scalar数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="15.06%" id="mcps1.2.7.1.5"><p id="p22651751121711"><a name="p22651751121711"></a><a name="p22651751121711"></a>dst输出数据（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="12.7%" id="mcps1.2.7.1.6"><p id="p182651051121718"><a name="p182651051121718"></a><a name="p182651051121718"></a>dst输出数据（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row132661351191714"><td class="cellrowborder" rowspan="42" valign="top" width="12.07%" headers="mcps1.2.7.1.1 "><p id="p426685112173"><a name="p426685112173"></a><a name="p426685112173"></a>half/half</p>
</td>
<td class="cellrowborder" rowspan="7" valign="middle" width="10.100000000000001%" headers="mcps1.2.7.1.2 "><p id="p17266451111716"><a name="p17266451111716"></a><a name="p17266451111716"></a>CMPMODE::EQ</p>
</td>
<td class="cellrowborder" valign="middle" width="13.98%" headers="mcps1.2.7.1.3 "><p id="p14266151101719"><a name="p14266151101719"></a><a name="p14266151101719"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="middle" width="36.09%" headers="mcps1.2.7.1.4 "><p id="p132661451171713"><a name="p132661451171713"></a><a name="p132661451171713"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="15.06%" headers="mcps1.2.7.1.5 "><p id="p126612515175"><a name="p126612515175"></a><a name="p126612515175"></a>1/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" width="12.7%" headers="mcps1.2.7.1.6 "><p id="p3266751151719"><a name="p3266751151719"></a><a name="p3266751151719"></a>1/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row126616511175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p162665511173"><a name="p162665511173"></a><a name="p162665511173"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p8266165117175"><a name="p8266165117175"></a><a name="p8266165117175"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p17266151201715"><a name="p17266151201715"></a><a name="p17266151201715"></a>0/1/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2266195151712"><a name="p2266195151712"></a><a name="p2266195151712"></a>0/1/0/0/0/0/0</p>
</td>
</tr>
<tr id="row1526618517174"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p426710511176"><a name="p426710511176"></a><a name="p426710511176"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1226715112176"><a name="p1226715112176"></a><a name="p1226715112176"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p2267551181716"><a name="p2267551181716"></a><a name="p2267551181716"></a>0/0/1/1/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p18267351191717"><a name="p18267351191717"></a><a name="p18267351191717"></a>0/0/1/1/0/0/0</p>
</td>
</tr>
<tr id="row9267105121714"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p9267125101711"><a name="p9267125101711"></a><a name="p9267125101711"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p142675513177"><a name="p142675513177"></a><a name="p142675513177"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p5267251111710"><a name="p5267251111710"></a><a name="p5267251111710"></a>0/0/1/1/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1267135131711"><a name="p1267135131711"></a><a name="p1267135131711"></a>0/0/1/1/0/0/0</p>
</td>
</tr>
<tr id="row7267125181720"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p7267205141716"><a name="p7267205141716"></a><a name="p7267205141716"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p82682515174"><a name="p82682515174"></a><a name="p82682515174"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p32685516172"><a name="p32685516172"></a><a name="p32685516172"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p112689518173"><a name="p112689518173"></a><a name="p112689518173"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row8268651201711"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p226820513175"><a name="p226820513175"></a><a name="p226820513175"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p42682515172"><a name="p42682515172"></a><a name="p42682515172"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1268115111717"><a name="p1268115111717"></a><a name="p1268115111717"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p13268105110179"><a name="p13268105110179"></a><a name="p13268105110179"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row426815517179"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p14268145114171"><a name="p14268145114171"></a><a name="p14268145114171"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p22681151181718"><a name="p22681151181718"></a><a name="p22681151181718"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p10268165131714"><a name="p10268165131714"></a><a name="p10268165131714"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p19269155141713"><a name="p19269155141713"></a><a name="p19269155141713"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row13269251171712"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p162691251141719"><a name="p162691251141719"></a><a name="p162691251141719"></a>CMPMODE::GE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p15269145101716"><a name="p15269145101716"></a><a name="p15269145101716"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p9269851151716"><a name="p9269851151716"></a><a name="p9269851151716"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p10269125171711"><a name="p10269125171711"></a><a name="p10269125171711"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p1526915121719"><a name="p1526915121719"></a><a name="p1526915121719"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row8269115121710"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1926945114170"><a name="p1926945114170"></a><a name="p1926945114170"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p62690518179"><a name="p62690518179"></a><a name="p62690518179"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p2026975161711"><a name="p2026975161711"></a><a name="p2026975161711"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p16269185141710"><a name="p16269185141710"></a><a name="p16269185141710"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row826945115172"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p527005118174"><a name="p527005118174"></a><a name="p527005118174"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p42702513175"><a name="p42702513175"></a><a name="p42702513175"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3270135161719"><a name="p3270135161719"></a><a name="p3270135161719"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1827045118172"><a name="p1827045118172"></a><a name="p1827045118172"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row142709512174"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p14270105114173"><a name="p14270105114173"></a><a name="p14270105114173"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p827035121719"><a name="p827035121719"></a><a name="p827035121719"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p327015113174"><a name="p327015113174"></a><a name="p327015113174"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p13270145141715"><a name="p13270145141715"></a><a name="p13270145141715"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row927025121712"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p132701651151711"><a name="p132701651151711"></a><a name="p132701651151711"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p4271251181711"><a name="p4271251181711"></a><a name="p4271251181711"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1427118517176"><a name="p1427118517176"></a><a name="p1427118517176"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p14271165111174"><a name="p14271165111174"></a><a name="p14271165111174"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row10271125115178"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p7271115113175"><a name="p7271115113175"></a><a name="p7271115113175"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p182711951141715"><a name="p182711951141715"></a><a name="p182711951141715"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1327145116176"><a name="p1327145116176"></a><a name="p1327145116176"></a>1/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p72711451141720"><a name="p72711451141720"></a><a name="p72711451141720"></a>1/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row9271451141710"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p16271051101710"><a name="p16271051101710"></a><a name="p16271051101710"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1271155131711"><a name="p1271155131711"></a><a name="p1271155131711"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p927135121710"><a name="p927135121710"></a><a name="p927135121710"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1327215511175"><a name="p1327215511175"></a><a name="p1327215511175"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row8272155131710"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p7272175191715"><a name="p7272175191715"></a><a name="p7272175191715"></a>CMPMODE::GT</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p1227215518172"><a name="p1227215518172"></a><a name="p1227215518172"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p6272195141710"><a name="p6272195141710"></a><a name="p6272195141710"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p192726516174"><a name="p192726516174"></a><a name="p192726516174"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p1127225151715"><a name="p1127225151715"></a><a name="p1127225151715"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row132727513172"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p5272115120178"><a name="p5272115120178"></a><a name="p5272115120178"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1527210518178"><a name="p1527210518178"></a><a name="p1527210518178"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p827295114179"><a name="p827295114179"></a><a name="p827295114179"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1727214513170"><a name="p1727214513170"></a><a name="p1727214513170"></a>/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row62721151131717"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p6273165101713"><a name="p6273165101713"></a><a name="p6273165101713"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p927385115179"><a name="p927385115179"></a><a name="p927385115179"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1727385121710"><a name="p1727385121710"></a><a name="p1727385121710"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p6273105111178"><a name="p6273105111178"></a><a name="p6273105111178"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row1273251131713"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1727355111710"><a name="p1727355111710"></a><a name="p1727355111710"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p427320519177"><a name="p427320519177"></a><a name="p427320519177"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1027325171719"><a name="p1027325171719"></a><a name="p1027325171719"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p132739517175"><a name="p132739517175"></a><a name="p132739517175"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row627316510174"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p2027345121716"><a name="p2027345121716"></a><a name="p2027345121716"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14273105151717"><a name="p14273105151717"></a><a name="p14273105151717"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p18274351191720"><a name="p18274351191720"></a><a name="p18274351191720"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p527465161715"><a name="p527465161715"></a><a name="p527465161715"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row11274145111711"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1827485111711"><a name="p1827485111711"></a><a name="p1827485111711"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p20274151121711"><a name="p20274151121711"></a><a name="p20274151121711"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p927417517172"><a name="p927417517172"></a><a name="p927417517172"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p17274155112179"><a name="p17274155112179"></a><a name="p17274155112179"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row14274175171714"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p5274751121717"><a name="p5274751121717"></a><a name="p5274751121717"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p112741051191716"><a name="p112741051191716"></a><a name="p112741051191716"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p927485120176"><a name="p927485120176"></a><a name="p927485120176"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p17274135112176"><a name="p17274135112176"></a><a name="p17274135112176"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row13274051111713"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p327525111171"><a name="p327525111171"></a><a name="p327525111171"></a>CMPMODE::LE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p82752519179"><a name="p82752519179"></a><a name="p82752519179"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1327514519172"><a name="p1327514519172"></a><a name="p1327514519172"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p112751251151710"><a name="p112751251151710"></a><a name="p112751251151710"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p14275651161711"><a name="p14275651161711"></a><a name="p14275651161711"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row0275165181715"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1727555117176"><a name="p1727555117176"></a><a name="p1727555117176"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p727515117177"><a name="p727515117177"></a><a name="p727515117177"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p192757510174"><a name="p192757510174"></a><a name="p192757510174"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p42751551201717"><a name="p42751551201717"></a><a name="p42751551201717"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row82754514173"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p192759519178"><a name="p192759519178"></a><a name="p192759519178"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p62761551121718"><a name="p62761551121718"></a><a name="p62761551121718"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p12276205161715"><a name="p12276205161715"></a><a name="p12276205161715"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p227685119178"><a name="p227685119178"></a><a name="p227685119178"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row2027685111717"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p162761519174"><a name="p162761519174"></a><a name="p162761519174"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p152761251181716"><a name="p152761251181716"></a><a name="p152761251181716"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p0276165119177"><a name="p0276165119177"></a><a name="p0276165119177"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p152765512174"><a name="p152765512174"></a><a name="p152765512174"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row1227616514179"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p4276651191714"><a name="p4276651191714"></a><a name="p4276651191714"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1627675121716"><a name="p1627675121716"></a><a name="p1627675121716"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p127665116173"><a name="p127665116173"></a><a name="p127665116173"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p6276155191714"><a name="p6276155191714"></a><a name="p6276155191714"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row127625116175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1827711511175"><a name="p1827711511175"></a><a name="p1827711511175"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p927775112174"><a name="p927775112174"></a><a name="p927775112174"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p42771519177"><a name="p42771519177"></a><a name="p42771519177"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p127718519173"><a name="p127718519173"></a><a name="p127718519173"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row152771951201716"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p18277351131719"><a name="p18277351131719"></a><a name="p18277351131719"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1827755118172"><a name="p1827755118172"></a><a name="p1827755118172"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1227785111719"><a name="p1227785111719"></a><a name="p1227785111719"></a>1/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2277251121718"><a name="p2277251121718"></a><a name="p2277251121718"></a>1/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row142778516175"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p1027725121717"><a name="p1027725121717"></a><a name="p1027725121717"></a>CMPMODE::LT</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p42771751151717"><a name="p42771751151717"></a><a name="p42771751151717"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1727715117179"><a name="p1727715117179"></a><a name="p1727715117179"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1027875101719"><a name="p1027875101719"></a><a name="p1027875101719"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p112784519178"><a name="p112784519178"></a><a name="p112784519178"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row102783515175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1827816512179"><a name="p1827816512179"></a><a name="p1827816512179"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p82781251131719"><a name="p82781251131719"></a><a name="p82781251131719"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p0278551161710"><a name="p0278551161710"></a><a name="p0278551161710"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p102781551171719"><a name="p102781551171719"></a><a name="p102781551171719"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row10278135161716"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p52788517176"><a name="p52788517176"></a><a name="p52788517176"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p3278145112172"><a name="p3278145112172"></a><a name="p3278145112172"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p9278751141711"><a name="p9278751141711"></a><a name="p9278751141711"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p192788512177"><a name="p192788512177"></a><a name="p192788512177"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row1027912513173"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17279135151712"><a name="p17279135151712"></a><a name="p17279135151712"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1327917510173"><a name="p1327917510173"></a><a name="p1327917510173"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3279251161711"><a name="p3279251161711"></a><a name="p3279251161711"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p122791517174"><a name="p122791517174"></a><a name="p122791517174"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row82792513175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p827915518175"><a name="p827915518175"></a><a name="p827915518175"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p8279125161719"><a name="p8279125161719"></a><a name="p8279125161719"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p92791651201714"><a name="p92791651201714"></a><a name="p92791651201714"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1227905171718"><a name="p1227905171718"></a><a name="p1227905171718"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row102797519178"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p2279165121718"><a name="p2279165121718"></a><a name="p2279165121718"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17280145101718"><a name="p17280145101718"></a><a name="p17280145101718"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p828012517175"><a name="p828012517175"></a><a name="p828012517175"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2280451191716"><a name="p2280451191716"></a><a name="p2280451191716"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row19280051101712"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p62801651171714"><a name="p62801651171714"></a><a name="p62801651171714"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p14280155117171"><a name="p14280155117171"></a><a name="p14280155117171"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p62801351191714"><a name="p62801351191714"></a><a name="p62801351191714"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p15280851131711"><a name="p15280851131711"></a><a name="p15280851131711"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row1428011511172"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p5280195101714"><a name="p5280195101714"></a><a name="p5280195101714"></a>CMPMODE::NE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p128095131718"><a name="p128095131718"></a><a name="p128095131718"></a>-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p4280135111178"><a name="p4280135111178"></a><a name="p4280135111178"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p42811651151719"><a name="p42811651151719"></a><a name="p42811651151719"></a>0/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p728185171710"><a name="p728185171710"></a><a name="p728185171710"></a>0/1/1/1/1/1/1</p>
</td>
</tr>
<tr id="row18281135112178"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1428155181720"><a name="p1428155181720"></a><a name="p1428155181720"></a>65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17281165141710"><a name="p17281165141710"></a><a name="p17281165141710"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p182810515174"><a name="p182810515174"></a><a name="p182810515174"></a>1/0/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p628114516179"><a name="p628114516179"></a><a name="p628114516179"></a>1/0/1/1/1/1/1</p>
</td>
</tr>
<tr id="row152815519170"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p112813515176"><a name="p112813515176"></a><a name="p112813515176"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p9281651101713"><a name="p9281651101713"></a><a name="p9281651101713"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p8281155120170"><a name="p8281155120170"></a><a name="p8281155120170"></a>1/1/0/0/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p7281195112176"><a name="p7281195112176"></a><a name="p7281195112176"></a>1/1/0/0/1/1/1</p>
</td>
</tr>
<tr id="row8281125191718"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p3282185118175"><a name="p3282185118175"></a><a name="p3282185118175"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p10282151121715"><a name="p10282151121715"></a><a name="p10282151121715"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p4282155151710"><a name="p4282155151710"></a><a name="p4282155151710"></a>1/1/0/0/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p162821751171710"><a name="p162821751171710"></a><a name="p162821751171710"></a>1/1/0/0/1/1/1</p>
</td>
</tr>
<tr id="row1128245114171"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p10282195115178"><a name="p10282195115178"></a><a name="p10282195115178"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p16282051151710"><a name="p16282051151710"></a><a name="p16282051151710"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p212702911813"><a name="p212702911813"></a><a name="p212702911813"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1931210309188"><a name="p1931210309188"></a><a name="p1931210309188"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row19282155171710"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1028285141711"><a name="p1028285141711"></a><a name="p1028285141711"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p17282175141710"><a name="p17282175141710"></a><a name="p17282175141710"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p14283145191712"><a name="p14283145191712"></a><a name="p14283145191712"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p7283125161711"><a name="p7283125161711"></a><a name="p7283125161711"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row8283135191712"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p13283125191719"><a name="p13283125191719"></a><a name="p13283125191719"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p10283205191719"><a name="p10283205191719"></a><a name="p10283205191719"></a>-65504.0（MIN）/65504.0（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p8283135110176"><a name="p8283135110176"></a><a name="p8283135110176"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p82831751121714"><a name="p82831751121714"></a><a name="p82831751121714"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row12283151111710"><td class="cellrowborder" rowspan="42" valign="top" width="12.07%" headers="mcps1.2.7.1.1 "><p id="p1128315113178"><a name="p1128315113178"></a><a name="p1128315113178"></a>float/float</p>
</td>
<td class="cellrowborder" rowspan="7" valign="middle" width="10.100000000000001%" headers="mcps1.2.7.1.2 "><p id="p12283175121713"><a name="p12283175121713"></a><a name="p12283175121713"></a>CMPMODE::EQ</p>
</td>
<td class="cellrowborder" valign="middle" width="13.98%" headers="mcps1.2.7.1.3 "><p id="p72841951181718"><a name="p72841951181718"></a><a name="p72841951181718"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="36.09%" headers="mcps1.2.7.1.4 "><p id="p11284185111720"><a name="p11284185111720"></a><a name="p11284185111720"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" width="15.06%" headers="mcps1.2.7.1.5 "><p id="p12284165112173"><a name="p12284165112173"></a><a name="p12284165112173"></a>1/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" width="12.7%" headers="mcps1.2.7.1.6 "><p id="p132848514177"><a name="p132848514177"></a><a name="p132848514177"></a>1/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row10284155101713"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1028418516177"><a name="p1028418516177"></a><a name="p1028418516177"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1628417517178"><a name="p1628417517178"></a><a name="p1628417517178"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p9284155113177"><a name="p9284155113177"></a><a name="p9284155113177"></a>0/1/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1628495141717"><a name="p1628495141717"></a><a name="p1628495141717"></a>0/1/0/0/0/0/0</p>
</td>
</tr>
<tr id="row182841451111713"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17284145181719"><a name="p17284145181719"></a><a name="p17284145181719"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p2028415513179"><a name="p2028415513179"></a><a name="p2028415513179"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1228545112171"><a name="p1228545112171"></a><a name="p1228545112171"></a>0/0/1/1/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1428555119179"><a name="p1428555119179"></a><a name="p1428555119179"></a>0/0/1/1/0/0/0</p>
</td>
</tr>
<tr id="row1628518511175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p11285115116175"><a name="p11285115116175"></a><a name="p11285115116175"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p18285165111175"><a name="p18285165111175"></a><a name="p18285165111175"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p72858517174"><a name="p72858517174"></a><a name="p72858517174"></a>0/0/1/1/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p17285145131720"><a name="p17285145131720"></a><a name="p17285145131720"></a>0/0/1/1/0/0/0</p>
</td>
</tr>
<tr id="row82851251121715"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1128595141710"><a name="p1128595141710"></a><a name="p1128595141710"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p72857515173"><a name="p72857515173"></a><a name="p72857515173"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p5285251121712"><a name="p5285251121712"></a><a name="p5285251121712"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1328520514174"><a name="p1328520514174"></a><a name="p1328520514174"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row19285351121719"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p928655113174"><a name="p928655113174"></a><a name="p928655113174"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p9286145120172"><a name="p9286145120172"></a><a name="p9286145120172"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p5286551111713"><a name="p5286551111713"></a><a name="p5286551111713"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p128612511173"><a name="p128612511173"></a><a name="p128612511173"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row132861851101716"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1228610517176"><a name="p1228610517176"></a><a name="p1228610517176"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1428610515172"><a name="p1428610515172"></a><a name="p1428610515172"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1028685181711"><a name="p1028685181711"></a><a name="p1028685181711"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p628635121710"><a name="p628635121710"></a><a name="p628635121710"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row8286205101711"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p92869515178"><a name="p92869515178"></a><a name="p92869515178"></a>CMPMODE::GE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p13286115112172"><a name="p13286115112172"></a><a name="p13286115112172"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p22861851131719"><a name="p22861851131719"></a><a name="p22861851131719"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p16287155181720"><a name="p16287155181720"></a><a name="p16287155181720"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p112873512178"><a name="p112873512178"></a><a name="p112873512178"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row1228755191712"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p10287195114179"><a name="p10287195114179"></a><a name="p10287195114179"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1528717516171"><a name="p1528717516171"></a><a name="p1528717516171"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p528785116176"><a name="p528785116176"></a><a name="p528785116176"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p328745191711"><a name="p328745191711"></a><a name="p328745191711"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row6287551121711"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p828785171710"><a name="p828785171710"></a><a name="p828785171710"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p5287195151710"><a name="p5287195151710"></a><a name="p5287195151710"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p228710513176"><a name="p228710513176"></a><a name="p228710513176"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p22883510172"><a name="p22883510172"></a><a name="p22883510172"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row5288175119174"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p17288175151712"><a name="p17288175151712"></a><a name="p17288175151712"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p8288155131717"><a name="p8288155131717"></a><a name="p8288155131717"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1428885101715"><a name="p1428885101715"></a><a name="p1428885101715"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2288185141717"><a name="p2288185141717"></a><a name="p2288185141717"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row9288851181719"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p14288125114179"><a name="p14288125114179"></a><a name="p14288125114179"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p3288195115173"><a name="p3288195115173"></a><a name="p3288195115173"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p182881751131719"><a name="p182881751131719"></a><a name="p182881751131719"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2288105114172"><a name="p2288105114172"></a><a name="p2288105114172"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row8289251161717"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p112891851151714"><a name="p112891851151714"></a><a name="p112891851151714"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p828915512178"><a name="p828915512178"></a><a name="p828915512178"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1528955131710"><a name="p1528955131710"></a><a name="p1528955131710"></a>1/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p328920512176"><a name="p328920512176"></a><a name="p328920512176"></a>1/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row3289115116179"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p82891951181711"><a name="p82891951181711"></a><a name="p82891951181711"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p132891051161713"><a name="p132891051161713"></a><a name="p132891051161713"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p18289135116171"><a name="p18289135116171"></a><a name="p18289135116171"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p5289155191713"><a name="p5289155191713"></a><a name="p5289155191713"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row228910513171"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p10289175116177"><a name="p10289175116177"></a><a name="p10289175116177"></a>CMPMODE::GT</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p1129045191716"><a name="p1129045191716"></a><a name="p1129045191716"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p4290115116176"><a name="p4290115116176"></a><a name="p4290115116176"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p192908510171"><a name="p192908510171"></a><a name="p192908510171"></a>0/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p429055191711"><a name="p429055191711"></a><a name="p429055191711"></a>0/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row82903519176"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p8290115161719"><a name="p8290115161719"></a><a name="p8290115161719"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p5290351111716"><a name="p5290351111716"></a><a name="p5290351111716"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p102903517179"><a name="p102903517179"></a><a name="p102903517179"></a>1/0/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p192901251151714"><a name="p192901251151714"></a><a name="p192901251151714"></a>1/0/1/1/0/0/1</p>
</td>
</tr>
<tr id="row1229005115174"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p192901951111711"><a name="p192901951111711"></a><a name="p192901951111711"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1829045111715"><a name="p1829045111715"></a><a name="p1829045111715"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p829195111177"><a name="p829195111177"></a><a name="p829195111177"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p12915516177"><a name="p12915516177"></a><a name="p12915516177"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row1291175113175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p529112515172"><a name="p529112515172"></a><a name="p529112515172"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p19291145191712"><a name="p19291145191712"></a><a name="p19291145191712"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p8291175171710"><a name="p8291175171710"></a><a name="p8291175171710"></a>1/0/0/0/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p13291185117176"><a name="p13291185117176"></a><a name="p13291185117176"></a>1/0/0/0/0/0/1</p>
</td>
</tr>
<tr id="row1429155116170"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p132911551201720"><a name="p132911551201720"></a><a name="p132911551201720"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p42911151191714"><a name="p42911151191714"></a><a name="p42911151191714"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p129125118172"><a name="p129125118172"></a><a name="p129125118172"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1029175151712"><a name="p1029175151712"></a><a name="p1029175151712"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row1291175161714"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p229275121710"><a name="p229275121710"></a><a name="p229275121710"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p122921351171719"><a name="p122921351171719"></a><a name="p122921351171719"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p112921451101710"><a name="p112921451101710"></a><a name="p112921451101710"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p829215151715"><a name="p829215151715"></a><a name="p829215151715"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row14292351151715"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p1529295191713"><a name="p1529295191713"></a><a name="p1529295191713"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p152923515175"><a name="p152923515175"></a><a name="p152923515175"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p182922518172"><a name="p182922518172"></a><a name="p182922518172"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p202923514170"><a name="p202923514170"></a><a name="p202923514170"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row3292351121714"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p142921351191717"><a name="p142921351191717"></a><a name="p142921351191717"></a>CMPMODE::LE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p13292551151714"><a name="p13292551151714"></a><a name="p13292551151714"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p72931510171"><a name="p72931510171"></a><a name="p72931510171"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1329395181711"><a name="p1329395181711"></a><a name="p1329395181711"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p1829305121713"><a name="p1829305121713"></a><a name="p1829305121713"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row17293551121711"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p122931551141719"><a name="p122931551141719"></a><a name="p122931551141719"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p19293105121711"><a name="p19293105121711"></a><a name="p19293105121711"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p52931951151713"><a name="p52931951151713"></a><a name="p52931951151713"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1329395116177"><a name="p1329395116177"></a><a name="p1329395116177"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row18293175181715"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p4293125110178"><a name="p4293125110178"></a><a name="p4293125110178"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p13293135111714"><a name="p13293135111714"></a><a name="p13293135111714"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p3293145120177"><a name="p3293145120177"></a><a name="p3293145120177"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2294125191719"><a name="p2294125191719"></a><a name="p2294125191719"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row13294115111175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p6294145114176"><a name="p6294145114176"></a><a name="p6294145114176"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1429412511179"><a name="p1429412511179"></a><a name="p1429412511179"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1129415516178"><a name="p1129415516178"></a><a name="p1129415516178"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p1629485111715"><a name="p1629485111715"></a><a name="p1629485111715"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row102949517173"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p029415112177"><a name="p029415112177"></a><a name="p029415112177"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p429415115179"><a name="p429415115179"></a><a name="p429415115179"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p12941151121716"><a name="p12941151121716"></a><a name="p12941151121716"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p172941351121720"><a name="p172941351121720"></a><a name="p172941351121720"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row12951451141716"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p52955519173"><a name="p52955519173"></a><a name="p52955519173"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p9295751151717"><a name="p9295751151717"></a><a name="p9295751151717"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p129505151715"><a name="p129505151715"></a><a name="p129505151715"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p12295251161712"><a name="p12295251161712"></a><a name="p12295251161712"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row19295165161713"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p229595121713"><a name="p229595121713"></a><a name="p229595121713"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p82951451141711"><a name="p82951451141711"></a><a name="p82951451141711"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p14295115116176"><a name="p14295115116176"></a><a name="p14295115116176"></a>1/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p14295105113174"><a name="p14295105113174"></a><a name="p14295105113174"></a>1/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row8295155118179"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p162961351161719"><a name="p162961351161719"></a><a name="p162961351161719"></a>CMPMODE::LT</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p8296115117173"><a name="p8296115117173"></a><a name="p8296115117173"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p0296165118174"><a name="p0296165118174"></a><a name="p0296165118174"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p829612517175"><a name="p829612517175"></a><a name="p829612517175"></a>0/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p329635112171"><a name="p329635112171"></a><a name="p329635112171"></a>0/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row1229665161715"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p529610518174"><a name="p529610518174"></a><a name="p529610518174"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p3296195114175"><a name="p3296195114175"></a><a name="p3296195114175"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1029695101716"><a name="p1029695101716"></a><a name="p1029695101716"></a>0/0/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p18296651201711"><a name="p18296651201711"></a><a name="p18296651201711"></a>0/0/0/0/0/1/0</p>
</td>
</tr>
<tr id="row529695112173"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p32974516177"><a name="p32974516177"></a><a name="p32974516177"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p6297115110178"><a name="p6297115110178"></a><a name="p6297115110178"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p18297145120177"><a name="p18297145120177"></a><a name="p18297145120177"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p12971851131710"><a name="p12971851131710"></a><a name="p12971851131710"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row02979517173"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p929795131712"><a name="p929795131712"></a><a name="p929795131712"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p229795111175"><a name="p229795111175"></a><a name="p229795111175"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p329718513173"><a name="p329718513173"></a><a name="p329718513173"></a>0/1/0/0/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p13297551141718"><a name="p13297551141718"></a><a name="p13297551141718"></a>0/1/0/0/0/1/0</p>
</td>
</tr>
<tr id="row172971551171714"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p132971151121710"><a name="p132971151121710"></a><a name="p132971151121710"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p5298151171714"><a name="p5298151171714"></a><a name="p5298151171714"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p7298251181714"><a name="p7298251181714"></a><a name="p7298251181714"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p172981251121720"><a name="p172981251121720"></a><a name="p172981251121720"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row8298251181714"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p329855118172"><a name="p329855118172"></a><a name="p329855118172"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p1629816514172"><a name="p1629816514172"></a><a name="p1629816514172"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p20298105111172"><a name="p20298105111172"></a><a name="p20298105111172"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p2029825161717"><a name="p2029825161717"></a><a name="p2029825161717"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row18298185161719"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p8298195181716"><a name="p8298195181716"></a><a name="p8298195181716"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p112981451201715"><a name="p112981451201715"></a><a name="p112981451201715"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1729895119173"><a name="p1729895119173"></a><a name="p1729895119173"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p11298195116176"><a name="p11298195116176"></a><a name="p11298195116176"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row202991051111710"><td class="cellrowborder" rowspan="7" valign="middle" headers="mcps1.2.7.1.1 "><p id="p529965110177"><a name="p529965110177"></a><a name="p529965110177"></a>CMPMODE::NE</p>
</td>
<td class="cellrowborder" valign="middle" headers="mcps1.2.7.1.2 "><p id="p2299165151718"><a name="p2299165151718"></a><a name="p2299165151718"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p729905151718"><a name="p729905151718"></a><a name="p729905151718"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p12299205101716"><a name="p12299205101716"></a><a name="p12299205101716"></a>0/1/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.5 "><p id="p198750117314"><a name="p198750117314"></a><a name="p198750117314"></a>0/1/1/1/0/1/1</p>
</td>
</tr>
<tr id="row82991518172"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p229915114177"><a name="p229915114177"></a><a name="p229915114177"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p16299165171714"><a name="p16299165171714"></a><a name="p16299165171714"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p13299251151712"><a name="p13299251151712"></a><a name="p13299251151712"></a>1/0/1/1/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p3875101133113"><a name="p3875101133113"></a><a name="p3875101133113"></a>1/0/1/1/0/1/1</p>
</td>
</tr>
<tr id="row72993515172"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p43001551121713"><a name="p43001551121713"></a><a name="p43001551121713"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p16300125113175"><a name="p16300125113175"></a><a name="p16300125113175"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p330025131718"><a name="p330025131718"></a><a name="p330025131718"></a>1/1/0/0/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p987541123115"><a name="p987541123115"></a><a name="p987541123115"></a>1/1/0/0/0/1/1</p>
</td>
</tr>
<tr id="row73001951111715"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p330085141710"><a name="p330085141710"></a><a name="p330085141710"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p13006517173"><a name="p13006517173"></a><a name="p13006517173"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1530025116172"><a name="p1530025116172"></a><a name="p1530025116172"></a>1/1/0/0/0/1/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p14875191133112"><a name="p14875191133112"></a><a name="p14875191133112"></a>1/1/0/0/0/1/1</p>
</td>
</tr>
<tr id="row2300175119179"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p53006513179"><a name="p53006513179"></a><a name="p53006513179"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p730116512171"><a name="p730116512171"></a><a name="p730116512171"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p62363371810"><a name="p62363371810"></a><a name="p62363371810"></a>0/0/0/0/0/0/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p178754115312"><a name="p178754115312"></a><a name="p178754115312"></a>0/0/0/0/0/0/0</p>
</td>
</tr>
<tr id="row18301145113177"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p173011951181713"><a name="p173011951181713"></a><a name="p173011951181713"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p133011151191720"><a name="p133011151191720"></a><a name="p133011151191720"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p1730145111716"><a name="p1730145111716"></a><a name="p1730145111716"></a>1/1/1/1/0/0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p18875191117314"><a name="p18875191117314"></a><a name="p18875191117314"></a>1/1/1/1/0/0/1</p>
</td>
</tr>
<tr id="row163013510175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p73016517170"><a name="p73016517170"></a><a name="p73016517170"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p330195114178"><a name="p330195114178"></a><a name="p330195114178"></a>-3.4028235e+38（MIN）/3.4028235e+38（MAX）/+0/-0/nan/+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p123011851161712"><a name="p123011851161712"></a><a name="p123011851161712"></a>1/1/1/1/0/1/0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p387611117316"><a name="p387611117316"></a><a name="p387611117316"></a>1/1/1/1/0/1/0</p>
</td>
</tr>
<tr id="row13021051161715"><td class="cellrowborder" rowspan="2" valign="top" width="12.07%" headers="mcps1.2.7.1.1 "><p id="p930275121712"><a name="p930275121712"></a><a name="p930275121712"></a>int32_t/int32_t</p>
</td>
<td class="cellrowborder" rowspan="2" valign="middle" width="10.100000000000001%" headers="mcps1.2.7.1.2 "><p id="p18302115111176"><a name="p18302115111176"></a><a name="p18302115111176"></a>CMPMODE::EQ</p>
</td>
<td class="cellrowborder" valign="middle" width="13.98%" headers="mcps1.2.7.1.3 "><p id="p1830217511176"><a name="p1830217511176"></a><a name="p1830217511176"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="36.09%" headers="mcps1.2.7.1.4 "><p id="p930265114171"><a name="p930265114171"></a><a name="p930265114171"></a>-2147483648（MIN）/2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="15.06%" headers="mcps1.2.7.1.5 "><p id="p33021551121711"><a name="p33021551121711"></a><a name="p33021551121711"></a>1/0</p>
</td>
<td class="cellrowborder" valign="top" width="12.7%" headers="mcps1.2.7.1.6 "><p id="p11302351181715"><a name="p11302351181715"></a><a name="p11302351181715"></a>1/0</p>
</td>
</tr>
<tr id="row1730285112175"><td class="cellrowborder" valign="top" headers="mcps1.2.7.1.1 "><p id="p13028510177"><a name="p13028510177"></a><a name="p13028510177"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.2 "><p id="p330255141715"><a name="p330255141715"></a><a name="p330255141715"></a>-2147483648（MIN）/2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.3 "><p id="p103021951131711"><a name="p103021951131711"></a><a name="p103021951131711"></a>0/1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.7.1.4 "><p id="p63035518171"><a name="p63035518171"></a><a name="p63035518171"></a>0/1</p>
</td>
</tr>
</tbody>
</table>

注：

-   CMPMODE::NE模式中，在数据类型为half和float时，Compares接口计算结果存在与IEEE754理论结果不同：
    -   src0 = nan，scalar = MIN/MAX/+0/-0/nan/+inf/-inf 时，实际结果：dst输出数据（INF/NAN模式和饱和模式）均为0/0/0/0/0/0/0，IEEE754理论结果：1/1/1/1/1/1/1
    -   src0 = MIN/MAX/+0/-0/nan/+inf/-inf，scalar =nan 时，实际结果：dst输出数据（INF/NAN模式和饱和模式）均为0/0/0/0/0/0/0，IEEE754理论结果：1/1/1/1/1/1/1

-   在其余模式中，dst输出数据（INF/NAN模式）、dst输出数据（饱和模式）结果，与IEEE754理论结果一致。

## 类型转换<a name="section3558104315715"></a>

**表 32**  Cast特殊值/边界值输入的计算结果

<a name="table1225292414418"></a>
<table><thead align="left"><tr id="row7262142417414"><th class="cellrowborder" valign="top" width="20.4%" id="mcps1.2.5.1.1"><p id="p32621624949"><a name="p32621624949"></a><a name="p32621624949"></a>src数据类型/dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="21.09%" id="mcps1.2.5.1.2"><p id="p10321152311416"><a name="p10321152311416"></a><a name="p10321152311416"></a>src输入数据</p>
</th>
<th class="cellrowborder" valign="top" width="20.990000000000002%" id="mcps1.2.5.1.3"><p id="p1626212247413"><a name="p1626212247413"></a><a name="p1626212247413"></a>dst输出数据（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="37.519999999999996%" id="mcps1.2.5.1.4"><p id="p692217313520"><a name="p692217313520"></a><a name="p692217313520"></a>dst输出数据（饱和模式，仅输出数据类型为half、bfloat16有效）</p>
</th>
</tr>
</thead>
<tbody><tr id="row122621724447"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p726220241648"><a name="p726220241648"></a><a name="p726220241648"></a>int4b_t/half</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p192621724145"><a name="p192621724145"></a><a name="p192621724145"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p1126212241449"><a name="p1126212241449"></a><a name="p1126212241449"></a>-8</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p171710113114"><a name="p171710113114"></a><a name="p171710113114"></a>-8</p>
</td>
</tr>
<tr id="row993618121410"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p19937118101417"><a name="p19937118101417"></a><a name="p19937118101417"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2093719815148"><a name="p2093719815148"></a><a name="p2093719815148"></a>7</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p3171117111"><a name="p3171117111"></a><a name="p3171117111"></a>7</p>
</td>
</tr>
<tr id="row13262124241"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p1926219241546"><a name="p1926219241546"></a><a name="p1926219241546"></a>uint8_t/half</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p92621924445"><a name="p92621924445"></a><a name="p92621924445"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p1426212240410"><a name="p1426212240410"></a><a name="p1426212240410"></a>0</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p5184101115"><a name="p5184101115"></a><a name="p5184101115"></a>0</p>
</td>
</tr>
<tr id="row22955581719"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1264661719174"><a name="p1264661719174"></a><a name="p1264661719174"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1629518520171"><a name="p1629518520171"></a><a name="p1629518520171"></a>255</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p218111141113"><a name="p218111141113"></a><a name="p218111141113"></a>255</p>
</td>
</tr>
<tr id="row105165912516"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p13513591856"><a name="p13513591856"></a><a name="p13513591856"></a>int8_t/half</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p176359951"><a name="p176359951"></a><a name="p176359951"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p3610591058"><a name="p3610591058"></a><a name="p3610591058"></a>127</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p18188118110"><a name="p18188118110"></a><a name="p18188118110"></a>127</p>
</td>
</tr>
<tr id="row11447952111712"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1144755210179"><a name="p1144755210179"></a><a name="p1144755210179"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1644712529179"><a name="p1644712529179"></a><a name="p1644712529179"></a>-128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p151841171113"><a name="p151841171113"></a><a name="p151841171113"></a>-128</p>
</td>
</tr>
<tr id="row66059101613"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p1260510102064"><a name="p1260510102064"></a><a name="p1260510102064"></a>half/int4b_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p6605161019618"><a name="p6605161019618"></a><a name="p6605161019618"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p86051110461"><a name="p86051110461"></a><a name="p86051110461"></a>7</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p0182101120"><a name="p0182101120"></a><a name="p0182101120"></a>7</p>
</td>
</tr>
<tr id="row16378172717187"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p203788279189"><a name="p203788279189"></a><a name="p203788279189"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1837816272182"><a name="p1837816272182"></a><a name="p1837816272182"></a>-8</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1918810117"><a name="p1918810117"></a><a name="p1918810117"></a>-8</p>
</td>
</tr>
<tr id="row181167141916"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1911675193"><a name="p1911675193"></a><a name="p1911675193"></a>+0/-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18137121910"><a name="p18137121910"></a><a name="p18137121910"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p4195131111"><a name="p4195131111"></a><a name="p4195131111"></a>0</p>
</td>
</tr>
<tr id="row1789792310196"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p128971923101919"><a name="p128971923101919"></a><a name="p128971923101919"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p38971023181919"><a name="p38971023181919"></a><a name="p38971023181919"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p131917112115"><a name="p131917112115"></a><a name="p131917112115"></a>0</p>
</td>
</tr>
<tr id="row33996313194"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p11399131171916"><a name="p11399131171916"></a><a name="p11399131171916"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11399331151918"><a name="p11399331151918"></a><a name="p11399331151918"></a>7</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1819317115"><a name="p1819317115"></a><a name="p1819317115"></a>7</p>
</td>
</tr>
<tr id="row19635113313196"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3635143315190"><a name="p3635143315190"></a><a name="p3635143315190"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p26350339193"><a name="p26350339193"></a><a name="p26350339193"></a>-8</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18191114113"><a name="p18191114113"></a><a name="p18191114113"></a>-8</p>
</td>
</tr>
<tr id="row15480172513618"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p548016251661"><a name="p548016251661"></a><a name="p548016251661"></a>half/int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p153541448122013"><a name="p153541448122013"></a><a name="p153541448122013"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p948014251667"><a name="p948014251667"></a><a name="p948014251667"></a>127</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p819121151118"><a name="p819121151118"></a><a name="p819121151118"></a>127</p>
</td>
</tr>
<tr id="row2407833122019"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p135414812019"><a name="p135414812019"></a><a name="p135414812019"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p540716335203"><a name="p540716335203"></a><a name="p540716335203"></a>-128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p10206117113"><a name="p10206117113"></a><a name="p10206117113"></a>-128</p>
</td>
</tr>
<tr id="row125463336201"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p113551248182013"><a name="p113551248182013"></a><a name="p113551248182013"></a>+0/-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1554603322012"><a name="p1554603322012"></a><a name="p1554603322012"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p6201121111"><a name="p6201121111"></a><a name="p6201121111"></a>0</p>
</td>
</tr>
<tr id="row2071963372018"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p33551648132013"><a name="p33551648132013"></a><a name="p33551648132013"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p157191533162011"><a name="p157191533162011"></a><a name="p157191533162011"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9206131117"><a name="p9206131117"></a><a name="p9206131117"></a>0</p>
</td>
</tr>
<tr id="row1589903352012"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8355114819205"><a name="p8355114819205"></a><a name="p8355114819205"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1489923317203"><a name="p1489923317203"></a><a name="p1489923317203"></a>127</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p172091131110"><a name="p172091131110"></a><a name="p172091131110"></a>127</p>
</td>
</tr>
<tr id="row1449143415205"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p0355648192017"><a name="p0355648192017"></a><a name="p0355648192017"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1501934152014"><a name="p1501934152014"></a><a name="p1501934152014"></a>-128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19217110112"><a name="p19217110112"></a><a name="p19217110112"></a>-128</p>
</td>
</tr>
<tr id="row116551443562"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p46559435615"><a name="p46559435615"></a><a name="p46559435615"></a>half/uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p1885613752111"><a name="p1885613752111"></a><a name="p1885613752111"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p86555431562"><a name="p86555431562"></a><a name="p86555431562"></a>255</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p7215118112"><a name="p7215118112"></a><a name="p7215118112"></a>255</p>
</td>
</tr>
<tr id="row175284254216"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p198565371218"><a name="p198565371218"></a><a name="p198565371218"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19528525142111"><a name="p19528525142111"></a><a name="p19528525142111"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p82111110110"><a name="p82111110110"></a><a name="p82111110110"></a>0</p>
</td>
</tr>
<tr id="row20663102513219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13856103711218"><a name="p13856103711218"></a><a name="p13856103711218"></a>+0/-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p266342514214"><a name="p266342514214"></a><a name="p266342514214"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1921131101111"><a name="p1921131101111"></a><a name="p1921131101111"></a>0</p>
</td>
</tr>
<tr id="row1980812256217"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p385623713217"><a name="p385623713217"></a><a name="p385623713217"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p198080256211"><a name="p198080256211"></a><a name="p198080256211"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p421611111"><a name="p421611111"></a><a name="p421611111"></a>0</p>
</td>
</tr>
<tr id="row13945192515218"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13856153710214"><a name="p13856153710214"></a><a name="p13856153710214"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1388062682214"><a name="p1388062682214"></a><a name="p1388062682214"></a>255</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2223116116"><a name="p2223116116"></a><a name="p2223116116"></a>255</p>
</td>
</tr>
<tr id="row1213632652115"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1685663714218"><a name="p1685663714218"></a><a name="p1685663714218"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8136162615216"><a name="p8136162615216"></a><a name="p8136162615216"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p13222116111"><a name="p13222116111"></a><a name="p13222116111"></a>0</p>
</td>
</tr>
<tr id="row155531424718"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p855311219717"><a name="p855311219717"></a><a name="p855311219717"></a>half/int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p13886141392612"><a name="p13886141392612"></a><a name="p13886141392612"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p1255313219713"><a name="p1255313219713"></a><a name="p1255313219713"></a>65504</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p422151171114"><a name="p422151171114"></a><a name="p422151171114"></a>65504</p>
</td>
</tr>
<tr id="row1039115457228"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4886513142610"><a name="p4886513142610"></a><a name="p4886513142610"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p73928459223"><a name="p73928459223"></a><a name="p73928459223"></a>-65504</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p4224116118"><a name="p4224116118"></a><a name="p4224116118"></a>-65504</p>
</td>
</tr>
<tr id="row65431745112214"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1588771318261"><a name="p1588771318261"></a><a name="p1588771318261"></a>+0/-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p75436453224"><a name="p75436453224"></a><a name="p75436453224"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p17223112112"><a name="p17223112112"></a><a name="p17223112112"></a>0</p>
</td>
</tr>
<tr id="row669564512222"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p18887813162610"><a name="p18887813162610"></a><a name="p18887813162610"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p126961645202213"><a name="p126961645202213"></a><a name="p126961645202213"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7237118115"><a name="p7237118115"></a><a name="p7237118115"></a>0</p>
</td>
</tr>
<tr id="row684184514226"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p988771313267"><a name="p988771313267"></a><a name="p988771313267"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p384116459225"><a name="p384116459225"></a><a name="p384116459225"></a>65504</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p62310171110"><a name="p62310171110"></a><a name="p62310171110"></a>65504</p>
</td>
</tr>
<tr id="row1581146112215"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p188731319269"><a name="p188731319269"></a><a name="p188731319269"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1296469224"><a name="p1296469224"></a><a name="p1296469224"></a>-65504</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p142317118112"><a name="p142317118112"></a><a name="p142317118112"></a>-65504</p>
</td>
</tr>
<tr id="row1675418146715"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p875414144718"><a name="p875414144718"></a><a name="p875414144718"></a>half/int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p106881972711"><a name="p106881972711"></a><a name="p106881972711"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p10755171419718"><a name="p10755171419718"></a><a name="p10755171419718"></a>65504</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p423131151116"><a name="p423131151116"></a><a name="p423131151116"></a>65504</p>
</td>
</tr>
<tr id="row1749501316271"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p968111919271"><a name="p968111919271"></a><a name="p968111919271"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p14496713182716"><a name="p14496713182716"></a><a name="p14496713182716"></a>-65504</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5239119119"><a name="p5239119119"></a><a name="p5239119119"></a>-65504</p>
</td>
</tr>
<tr id="row464312135271"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p269151922711"><a name="p269151922711"></a><a name="p269151922711"></a>+0/-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p6643201362714"><a name="p6643201362714"></a><a name="p6643201362714"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p52431161115"><a name="p52431161115"></a><a name="p52431161115"></a>0</p>
</td>
</tr>
<tr id="row108011113172717"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p116919191274"><a name="p116919191274"></a><a name="p116919191274"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p880118136273"><a name="p880118136273"></a><a name="p880118136273"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1524614115"><a name="p1524614115"></a><a name="p1524614115"></a>0</p>
</td>
</tr>
<tr id="row1894311319271"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16951914277"><a name="p16951914277"></a><a name="p16951914277"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1094491332713"><a name="p1094491332713"></a><a name="p1094491332713"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p4243118116"><a name="p4243118116"></a><a name="p4243118116"></a>2147483647</p>
</td>
</tr>
<tr id="row2882014172715"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p5691119202713"><a name="p5691119202713"></a><a name="p5691119202713"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1888614142715"><a name="p1888614142715"></a><a name="p1888614142715"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1524111121120"><a name="p1524111121120"></a><a name="p1524111121120"></a>-2147483648</p>
</td>
</tr>
<tr id="row09246291677"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p992410291479"><a name="p992410291479"></a><a name="p992410291479"></a>half/float</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p8251037153312"><a name="p8251037153312"></a><a name="p8251037153312"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p1464811436349"><a name="p1464811436349"></a><a name="p1464811436349"></a>65504</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p1024151141118"><a name="p1024151141118"></a><a name="p1024151141118"></a>65504</p>
</td>
</tr>
<tr id="row116931733215"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p15251133714333"><a name="p15251133714333"></a><a name="p15251133714333"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p167481646143411"><a name="p167481646143411"></a><a name="p167481646143411"></a>-65504</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2250151116"><a name="p2250151116"></a><a name="p2250151116"></a>-65504</p>
</td>
</tr>
<tr id="row1030417171328"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p125193793319"><a name="p125193793319"></a><a name="p125193793319"></a>+0/-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p930410172324"><a name="p930410172324"></a><a name="p930410172324"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p025151171119"><a name="p025151171119"></a><a name="p025151171119"></a>0</p>
</td>
</tr>
<tr id="row18433111714327"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13251143733314"><a name="p13251143733314"></a><a name="p13251143733314"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4434161710323"><a name="p4434161710323"></a><a name="p4434161710323"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1258113111"><a name="p1258113111"></a><a name="p1258113111"></a>nan</p>
</td>
</tr>
<tr id="row5582917163212"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p325243793319"><a name="p325243793319"></a><a name="p325243793319"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p14582017113211"><a name="p14582017113211"></a><a name="p14582017113211"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1525815118"><a name="p1525815118"></a><a name="p1525815118"></a>+inf</p>
</td>
</tr>
<tr id="row7734121793214"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p125223713337"><a name="p125223713337"></a><a name="p125223713337"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11735141793211"><a name="p11735141793211"></a><a name="p11735141793211"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1525121161112"><a name="p1525121161112"></a><a name="p1525121161112"></a>-inf</p>
</td>
</tr>
<tr id="row1983418354713"><td class="cellrowborder" rowspan="3" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p58344353717"><a name="p58344353717"></a><a name="p58344353717"></a>int16_t/half</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p883413351171"><a name="p883413351171"></a><a name="p883413351171"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p9834183510711"><a name="p9834183510711"></a><a name="p9834183510711"></a>32768.00</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p52521111114"><a name="p52521111114"></a><a name="p52521111114"></a>32768.00</p>
</td>
</tr>
<tr id="row2360917163617"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1636051753613"><a name="p1636051753613"></a><a name="p1636051753613"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p136013178360"><a name="p136013178360"></a><a name="p136013178360"></a>-32768.00</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p20261218116"><a name="p20261218116"></a><a name="p20261218116"></a>-32768.00</p>
</td>
</tr>
<tr id="row115011117183617"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p650151753619"><a name="p650151753619"></a><a name="p650151753619"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p850151714367"><a name="p850151714367"></a><a name="p850151714367"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p142616141114"><a name="p142616141114"></a><a name="p142616141114"></a>+0</p>
</td>
</tr>
<tr id="row20401046675"><td class="cellrowborder" rowspan="3" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p114021546775"><a name="p114021546775"></a><a name="p114021546775"></a>int16_t/float</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p54681955143713"><a name="p54681955143713"></a><a name="p54681955143713"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p194682551377"><a name="p194682551377"></a><a name="p194682551377"></a>32768.00</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p326211113"><a name="p326211113"></a><a name="p326211113"></a>32768.00</p>
</td>
</tr>
<tr id="row8280164717377"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p14469195514375"><a name="p14469195514375"></a><a name="p14469195514375"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p164691055143715"><a name="p164691055143715"></a><a name="p164691055143715"></a>-32768.00</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p182617191115"><a name="p182617191115"></a><a name="p182617191115"></a>-32768.00</p>
</td>
</tr>
<tr id="row1424184716378"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1846912556376"><a name="p1846912556376"></a><a name="p1846912556376"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p34691255173718"><a name="p34691255173718"></a><a name="p34691255173718"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1026101111115"><a name="p1026101111115"></a><a name="p1026101111115"></a>+0</p>
</td>
</tr>
<tr id="row18108617814"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p8109111989"><a name="p8109111989"></a><a name="p8109111989"></a>bfloat16_t/int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p2888122013918"><a name="p2888122013918"></a><a name="p2888122013918"></a>3.3895314e+38 (MAX)</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p310971884"><a name="p310971884"></a><a name="p310971884"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p12271111115"><a name="p12271111115"></a><a name="p12271111115"></a>2147483647</p>
</td>
</tr>
<tr id="row689431183820"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17109011188"><a name="p17109011188"></a><a name="p17109011188"></a>-3.3895314e+38 (MIN)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18954115385"><a name="p18954115385"></a><a name="p18954115385"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p427151171116"><a name="p427151171116"></a><a name="p427151171116"></a>-2147483648</p>
</td>
</tr>
<tr id="row235162143817"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p7358216382"><a name="p7358216382"></a><a name="p7358216382"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p23516293816"><a name="p23516293816"></a><a name="p23516293816"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p22714113118"><a name="p22714113118"></a><a name="p22714113118"></a>0</p>
</td>
</tr>
<tr id="row152251218388"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p177923394017"><a name="p177923394017"></a><a name="p177923394017"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p5226142153814"><a name="p5226142153814"></a><a name="p5226142153814"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p192761101116"><a name="p192761101116"></a><a name="p192761101116"></a>2147483647</p>
</td>
</tr>
<tr id="row18380727386"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p438111253814"><a name="p438111253814"></a><a name="p438111253814"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p638116243812"><a name="p638116243812"></a><a name="p638116243812"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p122714120114"><a name="p122714120114"></a><a name="p122714120114"></a>-2147483648</p>
</td>
</tr>
<tr id="row527784116418"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p7277341104113"><a name="p7277341104113"></a><a name="p7277341104113"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1027864116419"><a name="p1027864116419"></a><a name="p1027864116419"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p828613113"><a name="p828613113"></a><a name="p828613113"></a>0</p>
</td>
</tr>
<tr id="row88839191882"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p17883919288"><a name="p17883919288"></a><a name="p17883919288"></a>bfloat16_t/float</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p88846191582"><a name="p88846191582"></a><a name="p88846191582"></a>3.3895314e+38 (MAX)</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p98843191689"><a name="p98843191689"></a><a name="p98843191689"></a>3.3895314e+38</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p132861141118"><a name="p132861141118"></a><a name="p132861141118"></a>3.3895314e+38</p>
</td>
</tr>
<tr id="row09851933144013"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p59851033164012"><a name="p59851033164012"></a><a name="p59851033164012"></a>-3.3895314e+38 (MIN)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p5985143311400"><a name="p5985143311400"></a><a name="p5985143311400"></a>-3.3895314e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9281112111"><a name="p9281112111"></a><a name="p9281112111"></a>-3.3895314e+38</p>
</td>
</tr>
<tr id="row14119153420406"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p121209344401"><a name="p121209344401"></a><a name="p121209344401"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p0120173494010"><a name="p0120173494010"></a><a name="p0120173494010"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11281910118"><a name="p11281910118"></a><a name="p11281910118"></a>0</p>
</td>
</tr>
<tr id="row182591834194010"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p725910342405"><a name="p725910342405"></a><a name="p725910342405"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9259123410402"><a name="p9259123410402"></a><a name="p9259123410402"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p6286115113"><a name="p6286115113"></a><a name="p6286115113"></a>+inf</p>
</td>
</tr>
<tr id="row1840716344406"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p6407634124013"><a name="p6407634124013"></a><a name="p6407634124013"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8407143416405"><a name="p8407143416405"></a><a name="p8407143416405"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1929201151113"><a name="p1929201151113"></a><a name="p1929201151113"></a>-inf</p>
</td>
</tr>
<tr id="row974616109423"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p20746710114215"><a name="p20746710114215"></a><a name="p20746710114215"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p187461110184213"><a name="p187461110184213"></a><a name="p187461110184213"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p6294110119"><a name="p6294110119"></a><a name="p6294110119"></a>nan</p>
</td>
</tr>
<tr id="row69557311289"><td class="cellrowborder" rowspan="3" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p695513117818"><a name="p695513117818"></a><a name="p695513117818"></a>int32_t/int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p16955203118813"><a name="p16955203118813"></a><a name="p16955203118813"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p18955103111814"><a name="p18955103111814"></a><a name="p18955103111814"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p92913111112"><a name="p92913111112"></a><a name="p92913111112"></a>MAX</p>
</td>
</tr>
<tr id="row18252172413426"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p202524247423"><a name="p202524247423"></a><a name="p202524247423"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p62521424184211"><a name="p62521424184211"></a><a name="p62521424184211"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9299181111"><a name="p9299181111"></a><a name="p9299181111"></a>MIN</p>
</td>
</tr>
<tr id="row6394824134210"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p143951424104214"><a name="p143951424104214"></a><a name="p143951424104214"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p73951524184217"><a name="p73951524184217"></a><a name="p73951524184217"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p132921141110"><a name="p132921141110"></a><a name="p132921141110"></a>0</p>
</td>
</tr>
<tr id="row15427133817820"><td class="cellrowborder" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p542716381881"><a name="p542716381881"></a><a name="p542716381881"></a>int32_t/half（参考CastDequant）</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p3427133813810"><a name="p3427133813810"></a><a name="p3427133813810"></a>/</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p12427193817815"><a name="p12427193817815"></a><a name="p12427193817815"></a>/</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p73020120117"><a name="p73020120117"></a><a name="p73020120117"></a>/</p>
</td>
</tr>
<tr id="row18302125710817"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p53021257480"><a name="p53021257480"></a><a name="p53021257480"></a>int32_t/float</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p1230214571789"><a name="p1230214571789"></a><a name="p1230214571789"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p43021457288"><a name="p43021457288"></a><a name="p43021457288"></a>2147483520.00</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p8301014117"><a name="p8301014117"></a><a name="p8301014117"></a>2147483520.00</p>
</td>
</tr>
<tr id="row12245165954415"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1124525954415"><a name="p1124525954415"></a><a name="p1124525954415"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1124535920441"><a name="p1124535920441"></a><a name="p1124535920441"></a>-2147483648.00</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p83071141120"><a name="p83071141120"></a><a name="p83071141120"></a>-2147483648.00</p>
</td>
</tr>
<tr id="row92409519914"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p02414511916"><a name="p02414511916"></a><a name="p02414511916"></a>int32_t/int64_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p6984756104615"><a name="p6984756104615"></a><a name="p6984756104615"></a>2147483647（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p62417515910"><a name="p62417515910"></a><a name="p62417515910"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p19302018119"><a name="p19302018119"></a><a name="p19302018119"></a>2147483647</p>
</td>
</tr>
<tr id="row7275165574614"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p0984145618465"><a name="p0984145618465"></a><a name="p0984145618465"></a>-2147483648（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p22757559465"><a name="p22757559465"></a><a name="p22757559465"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p43061171114"><a name="p43061171114"></a><a name="p43061171114"></a>-2147483648</p>
</td>
</tr>
<tr id="row895613291492"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p2956172918914"><a name="p2956172918914"></a><a name="p2956172918914"></a>float/int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p149567299918"><a name="p149567299918"></a><a name="p149567299918"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p69561291194"><a name="p69561291194"></a><a name="p69561291194"></a>32767</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p173151151112"><a name="p173151151112"></a><a name="p173151151112"></a>32767</p>
</td>
</tr>
<tr id="row1852829144717"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p352913944715"><a name="p352913944715"></a><a name="p352913944715"></a>-3.4028235e+38（MIN)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p352917920471"><a name="p352917920471"></a><a name="p352917920471"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p103171171116"><a name="p103171171116"></a><a name="p103171171116"></a>-32768</p>
</td>
</tr>
<tr id="row02371619184910"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1276324114920"><a name="p1276324114920"></a><a name="p1276324114920"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p182378194494"><a name="p182378194494"></a><a name="p182378194494"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p831919112"><a name="p831919112"></a><a name="p831919112"></a>0</p>
</td>
</tr>
<tr id="row1039012193497"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p72761724144916"><a name="p72761724144916"></a><a name="p72761724144916"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1857117353491"><a name="p1857117353491"></a><a name="p1857117353491"></a>32767</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1631314110"><a name="p1631314110"></a><a name="p1631314110"></a>32767</p>
</td>
</tr>
<tr id="row1530101917496"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1027682420498"><a name="p1027682420498"></a><a name="p1027682420498"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17908173774916"><a name="p17908173774916"></a><a name="p17908173774916"></a>-32768</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p93171181113"><a name="p93171181113"></a><a name="p93171181113"></a>-32768</p>
</td>
</tr>
<tr id="row6666181924912"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17277182424919"><a name="p17277182424919"></a><a name="p17277182424919"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p566610194499"><a name="p566610194499"></a><a name="p566610194499"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9325117110"><a name="p9325117110"></a><a name="p9325117110"></a>0</p>
</td>
</tr>
<tr id="row39411431093"><td class="cellrowborder" rowspan="7" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p1394114317913"><a name="p1394114317913"></a><a name="p1394114317913"></a>float/bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p2942043795"><a name="p2942043795"></a><a name="p2942043795"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p89414311915"><a name="p89414311915"></a><a name="p89414311915"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p53215114114"><a name="p53215114114"></a><a name="p53215114114"></a>MAX</p>
</td>
</tr>
<tr id="row1270617487"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p9705116480"><a name="p9705116480"></a><a name="p9705116480"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p137081124811"><a name="p137081124811"></a><a name="p137081124811"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18329120110"><a name="p18329120110"></a><a name="p18329120110"></a>MIN</p>
</td>
</tr>
<tr id="row1621491124810"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8214121154816"><a name="p8214121154816"></a><a name="p8214121154816"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p32143184810"><a name="p32143184810"></a><a name="p32143184810"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9320141115"><a name="p9320141115"></a><a name="p9320141115"></a>+0</p>
</td>
</tr>
<tr id="row26691257438"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p366916518435"><a name="p366916518435"></a><a name="p366916518435"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7669857437"><a name="p7669857437"></a><a name="p7669857437"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p366910544313"><a name="p366910544313"></a><a name="p366910544313"></a>-0</p>
</td>
</tr>
<tr id="row269164716488"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p126964710482"><a name="p126964710482"></a><a name="p126964710482"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p6691547134818"><a name="p6691547134818"></a><a name="p6691547134818"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p173313171117"><a name="p173313171117"></a><a name="p173313171117"></a>MAX</p>
</td>
</tr>
<tr id="row14639953174817"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p196401853184819"><a name="p196401853184819"></a><a name="p196401853184819"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p46401536481"><a name="p46401536481"></a><a name="p46401536481"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p193316151110"><a name="p193316151110"></a><a name="p193316151110"></a>MIN</p>
</td>
</tr>
<tr id="row7768551498"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p476965134911"><a name="p476965134911"></a><a name="p476965134911"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1476919554911"><a name="p1476919554911"></a><a name="p1476919554911"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1033201161119"><a name="p1033201161119"></a><a name="p1033201161119"></a>0</p>
</td>
</tr>
<tr id="row84788591399"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p16478659398"><a name="p16478659398"></a><a name="p16478659398"></a>float/half</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p14467112417508"><a name="p14467112417508"></a><a name="p14467112417508"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p143303351509"><a name="p143303351509"></a><a name="p143303351509"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p153391191119"><a name="p153391191119"></a><a name="p153391191119"></a>MAX</p>
</td>
</tr>
<tr id="row7731312105011"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p24671024185018"><a name="p24671024185018"></a><a name="p24671024185018"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19731612135012"><a name="p19731612135012"></a><a name="p19731612135012"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p133361171113"><a name="p133361171113"></a><a name="p133361171113"></a>MIN</p>
</td>
</tr>
<tr id="row686218121508"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p446702455016"><a name="p446702455016"></a><a name="p446702455016"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p086201225012"><a name="p086201225012"></a><a name="p086201225012"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p834191181113"><a name="p834191181113"></a><a name="p834191181113"></a>0</p>
</td>
</tr>
<tr id="row20999212125014"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1246712417503"><a name="p1246712417503"></a><a name="p1246712417503"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4061317503"><a name="p4061317503"></a><a name="p4061317503"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p534171171118"><a name="p534171171118"></a><a name="p534171171118"></a>MAX</p>
</td>
</tr>
<tr id="row13142513125018"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1546802410506"><a name="p1546802410506"></a><a name="p1546802410506"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1714214133501"><a name="p1714214133501"></a><a name="p1714214133501"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18341610112"><a name="p18341610112"></a><a name="p18341610112"></a>MIN</p>
</td>
</tr>
<tr id="row828121318508"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p134689247501"><a name="p134689247501"></a><a name="p134689247501"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1228221318505"><a name="p1228221318505"></a><a name="p1228221318505"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1534615116"><a name="p1534615116"></a><a name="p1534615116"></a>0</p>
</td>
</tr>
<tr id="row149921612100"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p9992146111012"><a name="p9992146111012"></a><a name="p9992146111012"></a>float/int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p1843185417523"><a name="p1843185417523"></a><a name="p1843185417523"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p1896714616538"><a name="p1896714616538"></a><a name="p1896714616538"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p93417120117"><a name="p93417120117"></a><a name="p93417120117"></a>2147483647</p>
</td>
</tr>
<tr id="row1845145165118"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p9844195485219"><a name="p9844195485219"></a><a name="p9844195485219"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p142401349105313"><a name="p142401349105313"></a><a name="p142401349105313"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p83517161112"><a name="p83517161112"></a><a name="p83517161112"></a>-2147483648</p>
</td>
</tr>
<tr id="row29911256516"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4844125485218"><a name="p4844125485218"></a><a name="p4844125485218"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p119919535116"><a name="p119919535116"></a><a name="p119919535116"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p0351219112"><a name="p0351219112"></a><a name="p0351219112"></a>0</p>
</td>
</tr>
<tr id="row21231461516"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p78441154155211"><a name="p78441154155211"></a><a name="p78441154155211"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p23323455315"><a name="p23323455315"></a><a name="p23323455315"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p73514171112"><a name="p73514171112"></a><a name="p73514171112"></a>2147483647</p>
</td>
</tr>
<tr id="row1288018985119"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p48446548525"><a name="p48446548525"></a><a name="p48446548525"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1288099155118"><a name="p1288099155118"></a><a name="p1288099155118"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p183914115111"><a name="p183914115111"></a><a name="p183914115111"></a>-2147483648</p>
</td>
</tr>
<tr id="row298918995118"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p58441254165219"><a name="p58441254165219"></a><a name="p58441254165219"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p129896919517"><a name="p129896919517"></a><a name="p129896919517"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p143961101120"><a name="p143961101120"></a><a name="p143961101120"></a>0</p>
</td>
</tr>
<tr id="row1431161818101"><td class="cellrowborder" rowspan="7" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p13113183105"><a name="p13113183105"></a><a name="p13113183105"></a>float/float</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p3311111810103"><a name="p3311111810103"></a><a name="p3311111810103"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p16311518151013"><a name="p16311518151013"></a><a name="p16311518151013"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p1939214119"><a name="p1939214119"></a><a name="p1939214119"></a>MAX</p>
</td>
</tr>
<tr id="row195235537539"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1652365316537"><a name="p1652365316537"></a><a name="p1652365316537"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1452315534538"><a name="p1452315534538"></a><a name="p1452315534538"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p13951141111"><a name="p13951141111"></a><a name="p13951141111"></a>MIN</p>
</td>
</tr>
<tr id="row10655185311537"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2655135318533"><a name="p2655135318533"></a><a name="p2655135318533"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p165525305316"><a name="p165525305316"></a><a name="p165525305316"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p140151131110"><a name="p140151131110"></a><a name="p140151131110"></a>+0</p>
</td>
</tr>
<tr id="row35901357111616"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3591457111610"><a name="p3591457111610"></a><a name="p3591457111610"></a>-0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8591057111612"><a name="p8591057111612"></a><a name="p8591057111612"></a>+0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p175911957131610"><a name="p175911957131610"></a><a name="p175911957131610"></a>+0</p>
</td>
</tr>
<tr id="row1809115317534"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p148101453105316"><a name="p148101453105316"></a><a name="p148101453105316"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p3810115315538"><a name="p3810115315538"></a><a name="p3810115315538"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p44017115117"><a name="p44017115117"></a><a name="p44017115117"></a>+inf</p>
</td>
</tr>
<tr id="row11959135395317"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p15959155315320"><a name="p15959155315320"></a><a name="p15959155315320"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p99599533538"><a name="p99599533538"></a><a name="p99599533538"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p74071141114"><a name="p74071141114"></a><a name="p74071141114"></a>-inf</p>
</td>
</tr>
<tr id="row1010095419536"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2010045435312"><a name="p2010045435312"></a><a name="p2010045435312"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1110075420537"><a name="p1110075420537"></a><a name="p1110075420537"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p140141181110"><a name="p140141181110"></a><a name="p140141181110"></a>nan</p>
</td>
</tr>
<tr id="row1119652919106"><td class="cellrowborder" rowspan="6" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p619682981017"><a name="p619682981017"></a><a name="p619682981017"></a>float/int64_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p51964297104"><a name="p51964297104"></a><a name="p51964297104"></a>3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p4197102914100"><a name="p4197102914100"></a><a name="p4197102914100"></a>9.22337E+18</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p174015116111"><a name="p174015116111"></a><a name="p174015116111"></a>9.22337E+18</p>
</td>
</tr>
<tr id="row15415194620553"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p144151746115518"><a name="p144151746115518"></a><a name="p144151746115518"></a>-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p94157463552"><a name="p94157463552"></a><a name="p94157463552"></a>-9.22337E+18</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p6410161115"><a name="p6410161115"></a><a name="p6410161115"></a>-9.22337E+18</p>
</td>
</tr>
<tr id="row195708462557"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4570146155511"><a name="p4570146155511"></a><a name="p4570146155511"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p557016469555"><a name="p557016469555"></a><a name="p557016469555"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2411315112"><a name="p2411315112"></a><a name="p2411315112"></a>0</p>
</td>
</tr>
<tr id="row197021746205517"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p0702144615515"><a name="p0702144615515"></a><a name="p0702144615515"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4702184614559"><a name="p4702184614559"></a><a name="p4702184614559"></a>9.22337E+18</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5411913113"><a name="p5411913113"></a><a name="p5411913113"></a>9.22337E+18</p>
</td>
</tr>
<tr id="row28383462557"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p683864619553"><a name="p683864619553"></a><a name="p683864619553"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p749612511567"><a name="p749612511567"></a><a name="p749612511567"></a>-9.22337E+18</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1341181151116"><a name="p1341181151116"></a><a name="p1341181151116"></a>-9.22337E+18</p>
</td>
</tr>
<tr id="row19538201195910"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16538171195914"><a name="p16538171195914"></a><a name="p16538171195914"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1553818118597"><a name="p1553818118597"></a><a name="p1553818118597"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p204131161120"><a name="p204131161120"></a><a name="p204131161120"></a>0</p>
</td>
</tr>
<tr id="row10217133961016"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p321773911015"><a name="p321773911015"></a><a name="p321773911015"></a>int64_t/int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p2217839111011"><a name="p2217839111011"></a><a name="p2217839111011"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p1321783914103"><a name="p1321783914103"></a><a name="p1321783914103"></a>2147483647</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p742131191111"><a name="p742131191111"></a><a name="p742131191111"></a>2147483647</p>
</td>
</tr>
<tr id="row267317578561"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p6673195785613"><a name="p6673195785613"></a><a name="p6673195785613"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1673757165616"><a name="p1673757165616"></a><a name="p1673757165616"></a>-2147483648</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9422111119"><a name="p9422111119"></a><a name="p9422111119"></a>-2147483648</p>
</td>
</tr>
<tr id="row162003483100"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p142001548171016"><a name="p142001548171016"></a><a name="p142001548171016"></a>int64_t/float</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p10126193565915"><a name="p10126193565915"></a><a name="p10126193565915"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p82002048101014"><a name="p82002048101014"></a><a name="p82002048101014"></a>9223372036854775808.00（MAX）</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p16421219118"><a name="p16421219118"></a><a name="p16421219118"></a>9223372036854775808.00（MAX）</p>
</td>
</tr>
<tr id="row11702728135917"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p31264356595"><a name="p31264356595"></a><a name="p31264356595"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11702112875918"><a name="p11702112875918"></a><a name="p11702112875918"></a>-9223372036854775808.00（MIN）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p542919113"><a name="p542919113"></a><a name="p542919113"></a>-9223372036854775808.00（MIN）</p>
</td>
</tr>
<tr id="row58541503113"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p885415051114"><a name="p885415051114"></a><a name="p885415051114"></a>int16_t/int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p178543061110"><a name="p178543061110"></a><a name="p178543061110"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p16854100161110"><a name="p16854100161110"></a><a name="p16854100161110"></a>127</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p154218113111"><a name="p154218113111"></a><a name="p154218113111"></a>127</p>
</td>
</tr>
<tr id="row691625319013"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p39164531208"><a name="p39164531208"></a><a name="p39164531208"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8391143610113"><a name="p8391143610113"></a><a name="p8391143610113"></a>-128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7434121118"><a name="p7434121118"></a><a name="p7434121118"></a>-128</p>
</td>
</tr>
<tr id="row14308342119"><td class="cellrowborder" rowspan="2" valign="top" width="20.4%" headers="mcps1.2.5.1.1 "><p id="p203013349114"><a name="p203013349114"></a><a name="p203013349114"></a>int16_t/uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="21.09%" headers="mcps1.2.5.1.2 "><p id="p103043418113"><a name="p103043418113"></a><a name="p103043418113"></a>MAX</p>
</td>
<td class="cellrowborder" valign="top" width="20.990000000000002%" headers="mcps1.2.5.1.3 "><p id="p4301334161117"><a name="p4301334161117"></a><a name="p4301334161117"></a>255</p>
</td>
<td class="cellrowborder" valign="top" width="37.519999999999996%" headers="mcps1.2.5.1.4 "><p id="p204351191112"><a name="p204351191112"></a><a name="p204351191112"></a>255</p>
</td>
</tr>
<tr id="row117361717116"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p41736172114"><a name="p41736172114"></a><a name="p41736172114"></a>MIN</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19173817319"><a name="p19173817319"></a><a name="p19173817319"></a>0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7432114114"><a name="p7432114114"></a><a name="p7432114114"></a>0</p>
</td>
</tr>
</tbody>
</table>

float-\>float转换，-0会被转为+0，与IEEE不一致

## 归约计算<a name="section156745111398"></a>

**表 33**  ReduceDataBlock<MAX\>特殊值/边界值输入的计算结果

<a name="table2883162019243"></a>
<table><thead align="left"><tr id="row188319208245"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p1588316202244"><a name="p1588316202244"></a><a name="p1588316202244"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p088312201240"><a name="p088312201240"></a><a name="p088312201240"></a>src（1个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p1688372022415"><a name="p1688372022415"></a><a name="p1688372022415"></a>dst的元素取值</p>
<p id="p1988362012242"><a name="p1988362012242"></a><a name="p1988362012242"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p108831120142411"><a name="p108831120142411"></a><a name="p108831120142411"></a>dst的元素取值</p>
<p id="p088462013243"><a name="p088462013243"></a><a name="p088462013243"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row1388452020246"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p118841120142416"><a name="p118841120142416"></a><a name="p118841120142416"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p138848206241"><a name="p138848206241"></a><a name="p138848206241"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p1388413208241"><a name="p1388413208241"></a><a name="p1388413208241"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p28842200243"><a name="p28842200243"></a><a name="p28842200243"></a>-65504.0</p>
</td>
</tr>
<tr id="row1288472092415"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3884102042415"><a name="p3884102042415"></a><a name="p3884102042415"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11884120102416"><a name="p11884120102416"></a><a name="p11884120102416"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p158843204246"><a name="p158843204246"></a><a name="p158843204246"></a>65504.0</p>
</td>
</tr>
<tr id="row6884620122420"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1588432012417"><a name="p1588432012417"></a><a name="p1588432012417"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p888432002413"><a name="p888432002413"></a><a name="p888432002413"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p208841520172414"><a name="p208841520172414"></a><a name="p208841520172414"></a>0.0</p>
</td>
</tr>
<tr id="row0884320162417"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p14884152032417"><a name="p14884152032417"></a><a name="p14884152032417"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7607131314134"><a name="p7607131314134"></a><a name="p7607131314134"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1359581310131"><a name="p1359581310131"></a><a name="p1359581310131"></a>-0.0</p>
</td>
</tr>
<tr id="row15885620102416"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3885132052410"><a name="p3885132052410"></a><a name="p3885132052410"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p388514203247"><a name="p388514203247"></a><a name="p388514203247"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14885122020243"><a name="p14885122020243"></a><a name="p14885122020243"></a>0.0</p>
</td>
</tr>
<tr id="row388572072412"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1488512206248"><a name="p1488512206248"></a><a name="p1488512206248"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1988522013241"><a name="p1988522013241"></a><a name="p1988522013241"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18851120192414"><a name="p18851120192414"></a><a name="p18851120192414"></a>65504.0</p>
</td>
</tr>
<tr id="row08851120102419"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p988552010243"><a name="p988552010243"></a><a name="p988552010243"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15885142011242"><a name="p15885142011242"></a><a name="p15885142011242"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p12885192019241"><a name="p12885192019241"></a><a name="p12885192019241"></a>-65504.0</p>
</td>
</tr>
<tr id="row122631517101820"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p23701021101919"><a name="p23701021101919"></a><a name="p23701021101919"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15250101915209"><a name="p15250101915209"></a><a name="p15250101915209"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7891132292011"><a name="p7891132292011"></a><a name="p7891132292011"></a>0.0</p>
</td>
</tr>
<tr id="row17886720122414"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p437011218199"><a name="p437011218199"></a><a name="p437011218199"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17250111915206"><a name="p17250111915206"></a><a name="p17250111915206"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p88911722152019"><a name="p88911722152019"></a><a name="p88911722152019"></a>65504.0</p>
</td>
</tr>
<tr id="row4886132092419"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p437012117196"><a name="p437012117196"></a><a name="p437012117196"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p13250719162011"><a name="p13250719162011"></a><a name="p13250719162011"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1589132213209"><a name="p1589132213209"></a><a name="p1589132213209"></a>0.0</p>
</td>
</tr>
<tr id="row15887162092415"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p10370192121915"><a name="p10370192121915"></a><a name="p10370192121915"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p02501719112016"><a name="p02501719112016"></a><a name="p02501719112016"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18911822132013"><a name="p18911822132013"></a><a name="p18911822132013"></a>0.0</p>
</td>
</tr>
<tr id="row15887152062411"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p203707214194"><a name="p203707214194"></a><a name="p203707214194"></a>15个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p122501219142011"><a name="p122501219142011"></a><a name="p122501219142011"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1089122202016"><a name="p1089122202016"></a><a name="p1089122202016"></a>65504.0</p>
</td>
</tr>
<tr id="row108875205246"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p15370102110194"><a name="p15370102110194"></a><a name="p15370102110194"></a>15个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p925081972018"><a name="p925081972018"></a><a name="p925081972018"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p12891202222013"><a name="p12891202222013"></a><a name="p12891202222013"></a>1.0</p>
</td>
</tr>
<tr id="row28882020152413"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8370132121910"><a name="p8370132121910"></a><a name="p8370132121910"></a>15个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p225041920208"><a name="p225041920208"></a><a name="p225041920208"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p389113227209"><a name="p389113227209"></a><a name="p389113227209"></a>0.0</p>
</td>
</tr>
<tr id="row788812200247"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p18888172019249"><a name="p18888172019249"></a><a name="p18888172019249"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p588813203242"><a name="p588813203242"></a><a name="p588813203242"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p198888206243"><a name="p198888206243"></a><a name="p198888206243"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p7888162014247"><a name="p7888162014247"></a><a name="p7888162014247"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row6243104331818"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p488922052412"><a name="p488922052412"></a><a name="p488922052412"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1388922019247"><a name="p1388922019247"></a><a name="p1388922019247"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1888915208245"><a name="p1888915208245"></a><a name="p1888915208245"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row122438437181"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p15889152011244"><a name="p15889152011244"></a><a name="p15889152011244"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1188914200241"><a name="p1188914200241"></a><a name="p1188914200241"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p13889320152412"><a name="p13889320152412"></a><a name="p13889320152412"></a>0.0</p>
</td>
</tr>
<tr id="row12243194321819"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13889112042416"><a name="p13889112042416"></a><a name="p13889112042416"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p5965854161718"><a name="p5965854161718"></a><a name="p5965854161718"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p095410543176"><a name="p095410543176"></a><a name="p095410543176"></a>-0.0</p>
</td>
</tr>
<tr id="row11243443191819"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16890320102410"><a name="p16890320102410"></a><a name="p16890320102410"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p6890182016247"><a name="p6890182016247"></a><a name="p6890182016247"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1089012012414"><a name="p1089012012414"></a><a name="p1089012012414"></a>nan</p>
</td>
</tr>
<tr id="row22439436181"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2890620142413"><a name="p2890620142413"></a><a name="p2890620142413"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p178906200249"><a name="p178906200249"></a><a name="p178906200249"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1289022018240"><a name="p1289022018240"></a><a name="p1289022018240"></a>inf</p>
</td>
</tr>
<tr id="row1324334331813"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2890162017242"><a name="p2890162017242"></a><a name="p2890162017242"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p08911920122419"><a name="p08911920122419"></a><a name="p08911920122419"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p389162022420"><a name="p389162022420"></a><a name="p389162022420"></a>-inf</p>
</td>
</tr>
<tr id="row753275519186"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p43381346173316"><a name="p43381346173316"></a><a name="p43381346173316"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18221510142112"><a name="p18221510142112"></a><a name="p18221510142112"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8161269216"><a name="p8161269216"></a><a name="p8161269216"></a>nan</p>
</td>
</tr>
<tr id="row553219555181"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4338124663317"><a name="p4338124663317"></a><a name="p4338124663317"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15221010112119"><a name="p15221010112119"></a><a name="p15221010112119"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p101646162110"><a name="p101646162110"></a><a name="p101646162110"></a>inf</p>
</td>
</tr>
<tr id="row85321755151810"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p19338746143315"><a name="p19338746143315"></a><a name="p19338746143315"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p13221710162117"><a name="p13221710162117"></a><a name="p13221710162117"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14169682116"><a name="p14169682116"></a><a name="p14169682116"></a>nan</p>
</td>
</tr>
<tr id="row453265512181"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2033874663320"><a name="p2033874663320"></a><a name="p2033874663320"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p22221082116"><a name="p22221082116"></a><a name="p22221082116"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p191610612213"><a name="p191610612213"></a><a name="p191610612213"></a>nan</p>
</td>
</tr>
<tr id="row1532125541819"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p21011538162011"><a name="p21011538162011"></a><a name="p21011538162011"></a>7个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p222151013211"><a name="p222151013211"></a><a name="p222151013211"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p13168613213"><a name="p13168613213"></a><a name="p13168613213"></a>inf</p>
</td>
</tr>
<tr id="row6532355151813"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17101938132016"><a name="p17101938132016"></a><a name="p17101938132016"></a>7个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16222108212"><a name="p16222108212"></a><a name="p16222108212"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p3168612214"><a name="p3168612214"></a><a name="p3168612214"></a>1.0</p>
</td>
</tr>
<tr id="row1444111440201"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p141011938182010"><a name="p141011938182010"></a><a name="p141011938182010"></a>7个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1722710132117"><a name="p1722710132117"></a><a name="p1722710132117"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p131686142114"><a name="p131686142114"></a><a name="p131686142114"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 34**  ReduceDataBlock<MIN\>特殊值/边界值输入的计算结果

<a name="table6602183015217"></a>
<table><thead align="left"><tr id="row06029301212"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p660212305212"><a name="p660212305212"></a><a name="p660212305212"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p1360218307213"><a name="p1360218307213"></a><a name="p1360218307213"></a>src（1个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p1760214300216"><a name="p1760214300216"></a><a name="p1760214300216"></a>dst的元素取值</p>
<p id="p260216302216"><a name="p260216302216"></a><a name="p260216302216"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p660210305214"><a name="p660210305214"></a><a name="p660210305214"></a>dst的元素取值</p>
<p id="p1602153032112"><a name="p1602153032112"></a><a name="p1602153032112"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row960293032111"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p2602183062113"><a name="p2602183062113"></a><a name="p2602183062113"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p2602153032117"><a name="p2602153032117"></a><a name="p2602153032117"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p1355105719234"><a name="p1355105719234"></a><a name="p1355105719234"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p17432205112411"><a name="p17432205112411"></a><a name="p17432205112411"></a>-65504.0</p>
</td>
</tr>
<tr id="row1160211303219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p5602123082115"><a name="p5602123082115"></a><a name="p5602123082115"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p14551457102312"><a name="p14551457102312"></a><a name="p14551457102312"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p643213522411"><a name="p643213522411"></a><a name="p643213522411"></a>65504.0</p>
</td>
</tr>
<tr id="row5602123016216"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p12602430202120"><a name="p12602430202120"></a><a name="p12602430202120"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15551105718234"><a name="p15551105718234"></a><a name="p15551105718234"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p84329517243"><a name="p84329517243"></a><a name="p84329517243"></a>0.0</p>
</td>
</tr>
<tr id="row499239142414"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p155861138244"><a name="p155861138244"></a><a name="p155861138244"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p189924910240"><a name="p189924910240"></a><a name="p189924910240"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5992691245"><a name="p5992691245"></a><a name="p5992691245"></a>-0.0</p>
</td>
</tr>
<tr id="row46027308211"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p10602103032110"><a name="p10602103032110"></a><a name="p10602103032110"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p135511057162310"><a name="p135511057162310"></a><a name="p135511057162310"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1643216511244"><a name="p1643216511244"></a><a name="p1643216511244"></a>0.0</p>
</td>
</tr>
<tr id="row760213010218"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p146020304218"><a name="p146020304218"></a><a name="p146020304218"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p255145716233"><a name="p255145716233"></a><a name="p255145716233"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p124321652244"><a name="p124321652244"></a><a name="p124321652244"></a>65504.0</p>
</td>
</tr>
<tr id="row176021730162112"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8602123017213"><a name="p8602123017213"></a><a name="p8602123017213"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p10551195710237"><a name="p10551195710237"></a><a name="p10551195710237"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1443210592417"><a name="p1443210592417"></a><a name="p1443210592417"></a>-65504.0</p>
</td>
</tr>
<tr id="row360323092116"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p43421456153316"><a name="p43421456153316"></a><a name="p43421456153316"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1955115714230"><a name="p1955115714230"></a><a name="p1955115714230"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14432352247"><a name="p14432352247"></a><a name="p14432352247"></a>0.0</p>
</td>
</tr>
<tr id="row1460393017211"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1134218564339"><a name="p1134218564339"></a><a name="p1134218564339"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p115511757112320"><a name="p115511757112320"></a><a name="p115511757112320"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p144324520240"><a name="p144324520240"></a><a name="p144324520240"></a>-65504</p>
</td>
</tr>
<tr id="row3603730202113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1734245617338"><a name="p1734245617338"></a><a name="p1734245617338"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p155165711233"><a name="p155165711233"></a><a name="p155165711233"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p104320512415"><a name="p104320512415"></a><a name="p104320512415"></a>0.0</p>
</td>
</tr>
<tr id="row460343012113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1634212562336"><a name="p1634212562336"></a><a name="p1634212562336"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7551105711233"><a name="p7551105711233"></a><a name="p7551105711233"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1143217519246"><a name="p1143217519246"></a><a name="p1143217519246"></a>0.0</p>
</td>
</tr>
<tr id="row12603153010211"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p11603193012117"><a name="p11603193012117"></a><a name="p11603193012117"></a>15个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p455125742315"><a name="p455125742315"></a><a name="p455125742315"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p114326516248"><a name="p114326516248"></a><a name="p114326516248"></a>1.0</p>
</td>
</tr>
<tr id="row5603630162113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p26034305210"><a name="p26034305210"></a><a name="p26034305210"></a>15个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1551145762310"><a name="p1551145762310"></a><a name="p1551145762310"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p443220520240"><a name="p443220520240"></a><a name="p443220520240"></a>-65504.0</p>
</td>
</tr>
<tr id="row1860343012118"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p5603133022113"><a name="p5603133022113"></a><a name="p5603133022113"></a>15个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p85511857172313"><a name="p85511857172313"></a><a name="p85511857172313"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p54321522412"><a name="p54321522412"></a><a name="p54321522412"></a>0.0</p>
</td>
</tr>
<tr id="row106031630192112"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p460317307212"><a name="p460317307212"></a><a name="p460317307212"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p862638103213"><a name="p862638103213"></a><a name="p862638103213"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p11828798258"><a name="p11828798258"></a><a name="p11828798258"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p16835821253"><a name="p16835821253"></a><a name="p16835821253"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row13603183017219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p862618843210"><a name="p862618843210"></a><a name="p862618843210"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p85022812715"><a name="p85022812715"></a><a name="p85022812715"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p850928474"><a name="p850928474"></a><a name="p850928474"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row166032306214"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1662628113216"><a name="p1662628113216"></a><a name="p1662628113216"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p382899192519"><a name="p382899192519"></a><a name="p382899192519"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5835122132510"><a name="p5835122132510"></a><a name="p5835122132510"></a>0.0</p>
</td>
</tr>
<tr id="row1060463010213"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p962616823219"><a name="p962616823219"></a><a name="p962616823219"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2604203017214"><a name="p2604203017214"></a><a name="p2604203017214"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18604630162115"><a name="p18604630162115"></a><a name="p18604630162115"></a>-0.0</p>
</td>
</tr>
<tr id="row11604123010213"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1762616815323"><a name="p1762616815323"></a><a name="p1762616815323"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17340640122511"><a name="p17340640122511"></a><a name="p17340640122511"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p26691632112510"><a name="p26691632112510"></a><a name="p26691632112510"></a>nan</p>
</td>
</tr>
<tr id="row166041309215"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p19626688329"><a name="p19626688329"></a><a name="p19626688329"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1934094082510"><a name="p1934094082510"></a><a name="p1934094082510"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p566914326251"><a name="p566914326251"></a><a name="p566914326251"></a>inf</p>
</td>
</tr>
<tr id="row260453016212"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p96261081329"><a name="p96261081329"></a><a name="p96261081329"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1134044052515"><a name="p1134044052515"></a><a name="p1134044052515"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1566919321258"><a name="p1566919321258"></a><a name="p1566919321258"></a>-inf</p>
</td>
</tr>
<tr id="row1660414309211"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p367131043415"><a name="p367131043415"></a><a name="p367131043415"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p534013407253"><a name="p534013407253"></a><a name="p534013407253"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p766983219252"><a name="p766983219252"></a><a name="p766983219252"></a>nan</p>
</td>
</tr>
<tr id="row16604163017217"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p967910133412"><a name="p967910133412"></a><a name="p967910133412"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p434014407251"><a name="p434014407251"></a><a name="p434014407251"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p15669732152512"><a name="p15669732152512"></a><a name="p15669732152512"></a>-inf</p>
</td>
</tr>
<tr id="row1960416307218"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16751033416"><a name="p16751033416"></a><a name="p16751033416"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1234064019250"><a name="p1234064019250"></a><a name="p1234064019250"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p06691232162517"><a name="p06691232162517"></a><a name="p06691232162517"></a>nan</p>
</td>
</tr>
<tr id="row960419304216"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p86771012349"><a name="p86771012349"></a><a name="p86771012349"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1340440102511"><a name="p1340440102511"></a><a name="p1340440102511"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p186691232162518"><a name="p186691232162518"></a><a name="p186691232162518"></a>nan</p>
</td>
</tr>
<tr id="row17604930102117"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1562615813217"><a name="p1562615813217"></a><a name="p1562615813217"></a>7个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1934124010253"><a name="p1934124010253"></a><a name="p1934124010253"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16669123216251"><a name="p16669123216251"></a><a name="p16669123216251"></a>1.0</p>
</td>
</tr>
<tr id="row26041330112119"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p56269873213"><a name="p56269873213"></a><a name="p56269873213"></a>7个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p20341184012256"><a name="p20341184012256"></a><a name="p20341184012256"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p766913292519"><a name="p766913292519"></a><a name="p766913292519"></a>-inf</p>
</td>
</tr>
<tr id="row260415309211"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p462610818324"><a name="p462610818324"></a><a name="p462610818324"></a>7个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p12341114042511"><a name="p12341114042511"></a><a name="p12341114042511"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5669193216257"><a name="p5669193216257"></a><a name="p5669193216257"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 35**  ReduceDataBlock<SUM\>特殊值/边界值输入的计算结果

<a name="table47791758122218"></a>
<table><thead align="left"><tr id="row147798588224"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p1877955819229"><a name="p1877955819229"></a><a name="p1877955819229"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p57795586225"><a name="p57795586225"></a><a name="p57795586225"></a>src（1个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p107796583229"><a name="p107796583229"></a><a name="p107796583229"></a>dst的元素取值</p>
<p id="p8779558102218"><a name="p8779558102218"></a><a name="p8779558102218"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p1377965818228"><a name="p1377965818228"></a><a name="p1377965818228"></a>dst的元素取值</p>
<p id="p577985802218"><a name="p577985802218"></a><a name="p577985802218"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row1077917589224"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p4779458102214"><a name="p4779458102214"></a><a name="p4779458102214"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p147791458162210"><a name="p147791458162210"></a><a name="p147791458162210"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p106541311263"><a name="p106541311263"></a><a name="p106541311263"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p710285962512"><a name="p710285962512"></a><a name="p710285962512"></a>-65504.0</p>
</td>
</tr>
<tr id="row47796584221"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p377925802219"><a name="p377925802219"></a><a name="p377925802219"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p136555312611"><a name="p136555312611"></a><a name="p136555312611"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p131020595257"><a name="p131020595257"></a><a name="p131020595257"></a>65504.0</p>
</td>
</tr>
<tr id="row277985812216"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3779558152215"><a name="p3779558152215"></a><a name="p3779558152215"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p46551132262"><a name="p46551132262"></a><a name="p46551132262"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8102559112511"><a name="p8102559112511"></a><a name="p8102559112511"></a>0.0</p>
</td>
</tr>
<tr id="row13779458142213"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p177794589223"><a name="p177794589223"></a><a name="p177794589223"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p977915815222"><a name="p977915815222"></a><a name="p977915815222"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p377935812225"><a name="p377935812225"></a><a name="p377935812225"></a>-0.0</p>
</td>
</tr>
<tr id="row1877995813227"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p277915586225"><a name="p277915586225"></a><a name="p277915586225"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p59222233262"><a name="p59222233262"></a><a name="p59222233262"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p83461817192610"><a name="p83461817192610"></a><a name="p83461817192610"></a>0.0</p>
</td>
</tr>
<tr id="row207793580226"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p477914582221"><a name="p477914582221"></a><a name="p477914582221"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p692272352612"><a name="p692272352612"></a><a name="p692272352612"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p4346101732612"><a name="p4346101732612"></a><a name="p4346101732612"></a>65504.0</p>
</td>
</tr>
<tr id="row9779158152213"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p677985862212"><a name="p677985862212"></a><a name="p677985862212"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p13922182313261"><a name="p13922182313261"></a><a name="p13922182313261"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1934691713265"><a name="p1934691713265"></a><a name="p1934691713265"></a>-65504.0</p>
</td>
</tr>
<tr id="row1777935818229"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p04836138343"><a name="p04836138343"></a><a name="p04836138343"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19922102392620"><a name="p19922102392620"></a><a name="p19922102392620"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p20346917192618"><a name="p20346917192618"></a><a name="p20346917192618"></a>0.0</p>
</td>
</tr>
<tr id="row1677918584226"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p948310135347"><a name="p948310135347"></a><a name="p948310135347"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9922192352612"><a name="p9922192352612"></a><a name="p9922192352612"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19346141715260"><a name="p19346141715260"></a><a name="p19346141715260"></a>0.0</p>
</td>
</tr>
<tr id="row10780175882215"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13483121393417"><a name="p13483121393417"></a><a name="p13483121393417"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1922122313263"><a name="p1922122313263"></a><a name="p1922122313263"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p163461717172613"><a name="p163461717172613"></a><a name="p163461717172613"></a>0.0</p>
</td>
</tr>
<tr id="row147802058132219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16483151312342"><a name="p16483151312342"></a><a name="p16483151312342"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p192210233261"><a name="p192210233261"></a><a name="p192210233261"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1034611178260"><a name="p1034611178260"></a><a name="p1034611178260"></a>0.0</p>
</td>
</tr>
<tr id="row4780358112214"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p107801558112212"><a name="p107801558112212"></a><a name="p107801558112212"></a>15个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19922202318260"><a name="p19922202318260"></a><a name="p19922202318260"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p15346201712260"><a name="p15346201712260"></a><a name="p15346201712260"></a>65344.0</p>
</td>
</tr>
<tr id="row7780105819226"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1178011582228"><a name="p1178011582228"></a><a name="p1178011582228"></a>15个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15922123162610"><a name="p15922123162610"></a><a name="p15922123162610"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7346817192612"><a name="p7346817192612"></a><a name="p7346817192612"></a>-65344.0</p>
</td>
</tr>
<tr id="row17780858122213"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1878085813228"><a name="p1878085813228"></a><a name="p1878085813228"></a>15个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p392272312269"><a name="p392272312269"></a><a name="p392272312269"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p734614177267"><a name="p734614177267"></a><a name="p734614177267"></a>14.0</p>
</td>
</tr>
<tr id="row1780115822218"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p97801358202218"><a name="p97801358202218"></a><a name="p97801358202218"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p442614163217"><a name="p442614163217"></a><a name="p442614163217"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p39511029162711"><a name="p39511029162711"></a><a name="p39511029162711"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p650592410278"><a name="p650592410278"></a><a name="p650592410278"></a>-inf</p>
</td>
</tr>
<tr id="row1578095832219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p124218145327"><a name="p124218145327"></a><a name="p124218145327"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p209511929182718"><a name="p209511929182718"></a><a name="p209511929182718"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p95051724152719"><a name="p95051724152719"></a><a name="p95051724152719"></a>inf</p>
</td>
</tr>
<tr id="row4780358132217"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p64211144325"><a name="p64211144325"></a><a name="p64211144325"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15951202982714"><a name="p15951202982714"></a><a name="p15951202982714"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19505162442719"><a name="p19505162442719"></a><a name="p19505162442719"></a>0.0</p>
</td>
</tr>
<tr id="row11780165816222"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p64221413328"><a name="p64221413328"></a><a name="p64221413328"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p77806587223"><a name="p77806587223"></a><a name="p77806587223"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1578085811222"><a name="p1578085811222"></a><a name="p1578085811222"></a>-0.0</p>
</td>
</tr>
<tr id="row8780165810225"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1942141463220"><a name="p1942141463220"></a><a name="p1942141463220"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p169104410272"><a name="p169104410272"></a><a name="p169104410272"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p921014398271"><a name="p921014398271"></a><a name="p921014398271"></a>nan</p>
</td>
</tr>
<tr id="row778010586227"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1742181412329"><a name="p1742181412329"></a><a name="p1742181412329"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p591445276"><a name="p591445276"></a><a name="p591445276"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p32106391272"><a name="p32106391272"></a><a name="p32106391272"></a>inf</p>
</td>
</tr>
<tr id="row1378025892211"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1742314143214"><a name="p1742314143214"></a><a name="p1742314143214"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19344192719"><a name="p19344192719"></a><a name="p19344192719"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16210153932711"><a name="p16210153932711"></a><a name="p16210153932711"></a>-inf</p>
</td>
</tr>
<tr id="row1678025820225"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p5142142417341"><a name="p5142142417341"></a><a name="p5142142417341"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p691244182715"><a name="p691244182715"></a><a name="p691244182715"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p82100390272"><a name="p82100390272"></a><a name="p82100390272"></a>nan</p>
</td>
</tr>
<tr id="row1178016581223"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1714213248343"><a name="p1714213248343"></a><a name="p1714213248343"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p59104412275"><a name="p59104412275"></a><a name="p59104412275"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p122101239152714"><a name="p122101239152714"></a><a name="p122101239152714"></a>nan</p>
</td>
</tr>
<tr id="row18780105815225"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p10142724133418"><a name="p10142724133418"></a><a name="p10142724133418"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p492446276"><a name="p492446276"></a><a name="p492446276"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p321013932715"><a name="p321013932715"></a><a name="p321013932715"></a>nan</p>
</td>
</tr>
<tr id="row10780158162219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1014272411341"><a name="p1014272411341"></a><a name="p1014272411341"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p179844162710"><a name="p179844162710"></a><a name="p179844162710"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p112106395272"><a name="p112106395272"></a><a name="p112106395272"></a>nan</p>
</td>
</tr>
<tr id="row7780185842212"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2429147325"><a name="p2429147325"></a><a name="p2429147325"></a>7个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p29194419274"><a name="p29194419274"></a><a name="p29194419274"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p12210153902712"><a name="p12210153902712"></a><a name="p12210153902712"></a>inf</p>
</td>
</tr>
<tr id="row2078119580221"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p642614153212"><a name="p642614153212"></a><a name="p642614153212"></a>7个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p09184482712"><a name="p09184482712"></a><a name="p09184482712"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p321013917276"><a name="p321013917276"></a><a name="p321013917276"></a>-inf</p>
</td>
</tr>
<tr id="row207811758112217"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p242161411325"><a name="p242161411325"></a><a name="p242161411325"></a>7个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1397448275"><a name="p1397448275"></a><a name="p1397448275"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1521053918277"><a name="p1521053918277"></a><a name="p1521053918277"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 36**  ReduceRepeat<MAX\>特殊值/边界值输入的计算结果

<a name="table818992302819"></a>
<table><thead align="left"><tr id="row18189423182818"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p0189122392817"><a name="p0189122392817"></a><a name="p0189122392817"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p618916234285"><a name="p618916234285"></a><a name="p618916234285"></a>src（8个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p0189162311286"><a name="p0189162311286"></a><a name="p0189162311286"></a>dst的元素取值</p>
<p id="p17189142313288"><a name="p17189142313288"></a><a name="p17189142313288"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p191891823112816"><a name="p191891823112816"></a><a name="p191891823112816"></a>dst的元素取值</p>
<p id="p131891423112814"><a name="p131891423112814"></a><a name="p131891423112814"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row18190102314286"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p719042352819"><a name="p719042352819"></a><a name="p719042352819"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p1190152317288"><a name="p1190152317288"></a><a name="p1190152317288"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p41901923122815"><a name="p41901923122815"></a><a name="p41901923122815"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p141901223202811"><a name="p141901223202811"></a><a name="p141901223202811"></a>-65504.0</p>
</td>
</tr>
<tr id="row1119092392812"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p171907235283"><a name="p171907235283"></a><a name="p171907235283"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p519062316284"><a name="p519062316284"></a><a name="p519062316284"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p519020230289"><a name="p519020230289"></a><a name="p519020230289"></a>65504.0</p>
</td>
</tr>
<tr id="row519052313287"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p10190112342817"><a name="p10190112342817"></a><a name="p10190112342817"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8190132317281"><a name="p8190132317281"></a><a name="p8190132317281"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p419042318286"><a name="p419042318286"></a><a name="p419042318286"></a>0.0</p>
</td>
</tr>
<tr id="row2019072322814"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p919042352815"><a name="p919042352815"></a><a name="p919042352815"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p5190102316286"><a name="p5190102316286"></a><a name="p5190102316286"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p719022382814"><a name="p719022382814"></a><a name="p719022382814"></a>-0.0</p>
</td>
</tr>
<tr id="row31909230287"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p49898592306"><a name="p49898592306"></a><a name="p49898592306"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8338639124014"><a name="p8338639124014"></a><a name="p8338639124014"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p118581733144012"><a name="p118581733144012"></a><a name="p118581733144012"></a>0.0</p>
</td>
</tr>
<tr id="row119042317283"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1598985916307"><a name="p1598985916307"></a><a name="p1598985916307"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p13338039104010"><a name="p13338039104010"></a><a name="p13338039104010"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p12858133315407"><a name="p12858133315407"></a><a name="p12858133315407"></a>65504.0</p>
</td>
</tr>
<tr id="row3190122332813"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p189891159133020"><a name="p189891159133020"></a><a name="p189891159133020"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1338143917406"><a name="p1338143917406"></a><a name="p1338143917406"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1685843316409"><a name="p1685843316409"></a><a name="p1685843316409"></a>-65504.0</p>
</td>
</tr>
<tr id="row111907236286"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1477474093410"><a name="p1477474093410"></a><a name="p1477474093410"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p63391439204011"><a name="p63391439204011"></a><a name="p63391439204011"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1885815334403"><a name="p1885815334403"></a><a name="p1885815334403"></a>0.0</p>
</td>
</tr>
<tr id="row21906235280"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p977424010343"><a name="p977424010343"></a><a name="p977424010343"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11339939144013"><a name="p11339939144013"></a><a name="p11339939144013"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p58581733104011"><a name="p58581733104011"></a><a name="p58581733104011"></a>65504.0</p>
</td>
</tr>
<tr id="row15190323122818"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17747407345"><a name="p17747407345"></a><a name="p17747407345"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18339173919404"><a name="p18339173919404"></a><a name="p18339173919404"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1185883314019"><a name="p1185883314019"></a><a name="p1185883314019"></a>0.0</p>
</td>
</tr>
<tr id="row1419012310284"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p147741840193418"><a name="p147741840193418"></a><a name="p147741840193418"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p153391339114019"><a name="p153391339114019"></a><a name="p153391339114019"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p68587335403"><a name="p68587335403"></a><a name="p68587335403"></a>0.0</p>
</td>
</tr>
<tr id="row9190132352814"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p159890597301"><a name="p159890597301"></a><a name="p159890597301"></a>127个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7339113910409"><a name="p7339113910409"></a><a name="p7339113910409"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p198581933134016"><a name="p198581933134016"></a><a name="p198581933134016"></a>65504.0</p>
</td>
</tr>
<tr id="row6190142302816"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4989559193018"><a name="p4989559193018"></a><a name="p4989559193018"></a>127个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1533993912401"><a name="p1533993912401"></a><a name="p1533993912401"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p185883320407"><a name="p185883320407"></a><a name="p185883320407"></a>1.0</p>
</td>
</tr>
<tr id="row219012317288"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p498916598306"><a name="p498916598306"></a><a name="p498916598306"></a>127个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16339103910405"><a name="p16339103910405"></a><a name="p16339103910405"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p785893315404"><a name="p785893315404"></a><a name="p785893315404"></a>0.0</p>
</td>
</tr>
<tr id="row2190182315282"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p219042310281"><a name="p219042310281"></a><a name="p219042310281"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p5190152322810"><a name="p5190152322810"></a><a name="p5190152322810"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p10190102320283"><a name="p10190102320283"></a><a name="p10190102320283"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p1019052315281"><a name="p1019052315281"></a><a name="p1019052315281"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row1219014238280"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13190423182817"><a name="p13190423182817"></a><a name="p13190423182817"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p14190102312289"><a name="p14190102312289"></a><a name="p14190102312289"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1190102312286"><a name="p1190102312286"></a><a name="p1190102312286"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row319082318288"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16191112352819"><a name="p16191112352819"></a><a name="p16191112352819"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p819152352811"><a name="p819152352811"></a><a name="p819152352811"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1019112310284"><a name="p1019112310284"></a><a name="p1019112310284"></a>0.0</p>
</td>
</tr>
<tr id="row191911723172810"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p101911623192813"><a name="p101911623192813"></a><a name="p101911623192813"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9191122302816"><a name="p9191122302816"></a><a name="p9191122302816"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p119115236289"><a name="p119115236289"></a><a name="p119115236289"></a>-0.0</p>
</td>
</tr>
<tr id="row19191162352814"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1278191313314"><a name="p1278191313314"></a><a name="p1278191313314"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4844171324111"><a name="p4844171324111"></a><a name="p4844171324111"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p106601221124114"><a name="p106601221124114"></a><a name="p106601221124114"></a>nan</p>
</td>
</tr>
<tr id="row11915237283"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p37820136318"><a name="p37820136318"></a><a name="p37820136318"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1484419137414"><a name="p1484419137414"></a><a name="p1484419137414"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p146601921184114"><a name="p146601921184114"></a><a name="p146601921184114"></a>inf</p>
</td>
</tr>
<tr id="row3191122311286"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4782013163116"><a name="p4782013163116"></a><a name="p4782013163116"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p14844813194113"><a name="p14844813194113"></a><a name="p14844813194113"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p066072117413"><a name="p066072117413"></a><a name="p066072117413"></a>-inf</p>
</td>
</tr>
<tr id="row019172311289"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p770124553415"><a name="p770124553415"></a><a name="p770124553415"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1684461318412"><a name="p1684461318412"></a><a name="p1684461318412"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p66601221174118"><a name="p66601221174118"></a><a name="p66601221174118"></a>nan</p>
</td>
</tr>
<tr id="row619112318283"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2701184593411"><a name="p2701184593411"></a><a name="p2701184593411"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1984461394111"><a name="p1984461394111"></a><a name="p1984461394111"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1660521184114"><a name="p1660521184114"></a><a name="p1660521184114"></a>inf</p>
</td>
</tr>
<tr id="row11911723162817"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p470164593415"><a name="p470164593415"></a><a name="p470164593415"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p128441813164119"><a name="p128441813164119"></a><a name="p128441813164119"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11660121164116"><a name="p11660121164116"></a><a name="p11660121164116"></a>nan</p>
</td>
</tr>
<tr id="row11191152392811"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1170115454343"><a name="p1170115454343"></a><a name="p1170115454343"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p084418135414"><a name="p084418135414"></a><a name="p084418135414"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16660152154118"><a name="p16660152154118"></a><a name="p16660152154118"></a>nan</p>
</td>
</tr>
<tr id="row6191102318289"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4785135316"><a name="p4785135316"></a><a name="p4785135316"></a>63个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1784451354119"><a name="p1784451354119"></a><a name="p1784451354119"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1660162124110"><a name="p1660162124110"></a><a name="p1660162124110"></a>inf</p>
</td>
</tr>
<tr id="row31911323192816"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1378151393112"><a name="p1378151393112"></a><a name="p1378151393112"></a>63个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p12844111334110"><a name="p12844111334110"></a><a name="p12844111334110"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p86601521164118"><a name="p86601521164118"></a><a name="p86601521164118"></a>1</p>
</td>
</tr>
<tr id="row7191192315282"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p878191311318"><a name="p878191311318"></a><a name="p878191311318"></a>63个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1184411311417"><a name="p1184411311417"></a><a name="p1184411311417"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14660112154118"><a name="p14660112154118"></a><a name="p14660112154118"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 37**  ReduceRepeat<MIN\>特殊值/边界值输入的计算结果

<a name="table11721236104119"></a>
<table><thead align="left"><tr id="row17172436194113"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p16172113618413"><a name="p16172113618413"></a><a name="p16172113618413"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p3172143619413"><a name="p3172143619413"></a><a name="p3172143619413"></a>src（8个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p141721436134116"><a name="p141721436134116"></a><a name="p141721436134116"></a>dst的元素取值</p>
<p id="p1617213361418"><a name="p1617213361418"></a><a name="p1617213361418"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p18172936144114"><a name="p18172936144114"></a><a name="p18172936144114"></a>dst的元素取值</p>
<p id="p15172173624113"><a name="p15172173624113"></a><a name="p15172173624113"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row1517213613415"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p3172103634119"><a name="p3172103634119"></a><a name="p3172103634119"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p1817243644119"><a name="p1817243644119"></a><a name="p1817243644119"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p111731365415"><a name="p111731365415"></a><a name="p111731365415"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p1117313604118"><a name="p1117313604118"></a><a name="p1117313604118"></a>-65504.0</p>
</td>
</tr>
<tr id="row5173736154119"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p017323604120"><a name="p017323604120"></a><a name="p017323604120"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19173133611413"><a name="p19173133611413"></a><a name="p19173133611413"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9173153634110"><a name="p9173153634110"></a><a name="p9173153634110"></a>65504.0</p>
</td>
</tr>
<tr id="row81731036124112"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1617318363419"><a name="p1617318363419"></a><a name="p1617318363419"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17173133634118"><a name="p17173133634118"></a><a name="p17173133634118"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p81732036194110"><a name="p81732036194110"></a><a name="p81732036194110"></a>0.0</p>
</td>
</tr>
<tr id="row1717393618413"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8173153654117"><a name="p8173153654117"></a><a name="p8173153654117"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p31731336204120"><a name="p31731336204120"></a><a name="p31731336204120"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p517333614119"><a name="p517333614119"></a><a name="p517333614119"></a>-0.0</p>
</td>
</tr>
<tr id="row15173123644111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p21730361414"><a name="p21730361414"></a><a name="p21730361414"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8958630104711"><a name="p8958630104711"></a><a name="p8958630104711"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p978383874710"><a name="p978383874710"></a><a name="p978383874710"></a>0</p>
</td>
</tr>
<tr id="row181738367411"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p12173173604118"><a name="p12173173604118"></a><a name="p12173173604118"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p159581130154717"><a name="p159581130154717"></a><a name="p159581130154717"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p378373816473"><a name="p378373816473"></a><a name="p378373816473"></a>65504</p>
</td>
</tr>
<tr id="row1717373624115"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p11732365412"><a name="p11732365412"></a><a name="p11732365412"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p295823084716"><a name="p295823084716"></a><a name="p295823084716"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p177839382476"><a name="p177839382476"></a><a name="p177839382476"></a>-65504</p>
</td>
</tr>
<tr id="row21731136154118"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1817314362419"><a name="p1817314362419"></a><a name="p1817314362419"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p495816303474"><a name="p495816303474"></a><a name="p495816303474"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14783143844720"><a name="p14783143844720"></a><a name="p14783143844720"></a>0</p>
</td>
</tr>
<tr id="row61736368416"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p101733361419"><a name="p101733361419"></a><a name="p101733361419"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1995819302476"><a name="p1995819302476"></a><a name="p1995819302476"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9783133834715"><a name="p9783133834715"></a><a name="p9783133834715"></a>-65504</p>
</td>
</tr>
<tr id="row2173123612410"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3173736204119"><a name="p3173736204119"></a><a name="p3173736204119"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9958430144710"><a name="p9958430144710"></a><a name="p9958430144710"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p378311382478"><a name="p378311382478"></a><a name="p378311382478"></a>0</p>
</td>
</tr>
<tr id="row41731736114110"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p161738369411"><a name="p161738369411"></a><a name="p161738369411"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p119581430164720"><a name="p119581430164720"></a><a name="p119581430164720"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p87834389477"><a name="p87834389477"></a><a name="p87834389477"></a>0</p>
</td>
</tr>
<tr id="row517363617411"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p14173133664119"><a name="p14173133664119"></a><a name="p14173133664119"></a>127个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p159581430134712"><a name="p159581430134712"></a><a name="p159581430134712"></a>1</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p10783193813472"><a name="p10783193813472"></a><a name="p10783193813472"></a>1</p>
</td>
</tr>
<tr id="row017403618418"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p6174936104115"><a name="p6174936104115"></a><a name="p6174936104115"></a>127个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p169586309479"><a name="p169586309479"></a><a name="p169586309479"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8783103834718"><a name="p8783103834718"></a><a name="p8783103834718"></a>-65504</p>
</td>
</tr>
<tr id="row51746367413"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p161741836194117"><a name="p161741836194117"></a><a name="p161741836194117"></a>127个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p695853014718"><a name="p695853014718"></a><a name="p695853014718"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18783143834718"><a name="p18783143834718"></a><a name="p18783143834718"></a>0</p>
</td>
</tr>
<tr id="row12174173604110"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p617453610412"><a name="p617453610412"></a><a name="p617453610412"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p4174143612416"><a name="p4174143612416"></a><a name="p4174143612416"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p8174113674114"><a name="p8174113674114"></a><a name="p8174113674114"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p817453619416"><a name="p817453619416"></a><a name="p817453619416"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row13174736154113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2174153612412"><a name="p2174153612412"></a><a name="p2174153612412"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p0174336204114"><a name="p0174336204114"></a><a name="p0174336204114"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p121741436134110"><a name="p121741436134110"></a><a name="p121741436134110"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row51741936104120"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p171743367412"><a name="p171743367412"></a><a name="p171743367412"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p0174636144113"><a name="p0174636144113"></a><a name="p0174636144113"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p717463620413"><a name="p717463620413"></a><a name="p717463620413"></a>0.0</p>
</td>
</tr>
<tr id="row17174736174113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3174203654115"><a name="p3174203654115"></a><a name="p3174203654115"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18174183674120"><a name="p18174183674120"></a><a name="p18174183674120"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p6174193614118"><a name="p6174193614118"></a><a name="p6174193614118"></a>-0.0</p>
</td>
</tr>
<tr id="row5174236174114"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p12174103664119"><a name="p12174103664119"></a><a name="p12174103664119"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9455314717"><a name="p9455314717"></a><a name="p9455314717"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16767115714476"><a name="p16767115714476"></a><a name="p16767115714476"></a>nan</p>
</td>
</tr>
<tr id="row917433611418"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p517463620412"><a name="p517463620412"></a><a name="p517463620412"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8425374714"><a name="p8425374714"></a><a name="p8425374714"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11767195719472"><a name="p11767195719472"></a><a name="p11767195719472"></a>inf</p>
</td>
</tr>
<tr id="row1417463684119"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13174103614119"><a name="p13174103614119"></a><a name="p13174103614119"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p164115374714"><a name="p164115374714"></a><a name="p164115374714"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2076765794714"><a name="p2076765794714"></a><a name="p2076765794714"></a>-inf</p>
</td>
</tr>
<tr id="row13174203619413"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1317423664115"><a name="p1317423664115"></a><a name="p1317423664115"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p24155311475"><a name="p24155311475"></a><a name="p24155311475"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1776745784717"><a name="p1776745784717"></a><a name="p1776745784717"></a>nan</p>
</td>
</tr>
<tr id="row3175736104117"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8175193614118"><a name="p8175193614118"></a><a name="p8175193614118"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p124165319470"><a name="p124165319470"></a><a name="p124165319470"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7767205713478"><a name="p7767205713478"></a><a name="p7767205713478"></a>-inf</p>
</td>
</tr>
<tr id="row17175103664119"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8175163619412"><a name="p8175163619412"></a><a name="p8175163619412"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17425313474"><a name="p17425313474"></a><a name="p17425313474"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p137671457154714"><a name="p137671457154714"></a><a name="p137671457154714"></a>nan</p>
</td>
</tr>
<tr id="row141751366418"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p617573674114"><a name="p617573674114"></a><a name="p617573674114"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p74353144710"><a name="p74353144710"></a><a name="p74353144710"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9767357144713"><a name="p9767357144713"></a><a name="p9767357144713"></a>nan</p>
</td>
</tr>
<tr id="row71757360412"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p417511361414"><a name="p417511361414"></a><a name="p417511361414"></a>63个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p104145334714"><a name="p104145334714"></a><a name="p104145334714"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p137671357144720"><a name="p137671357144720"></a><a name="p137671357144720"></a>1.0</p>
</td>
</tr>
<tr id="row01751336164111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1417573694116"><a name="p1417573694116"></a><a name="p1417573694116"></a>63个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1342053104711"><a name="p1342053104711"></a><a name="p1342053104711"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19767145744712"><a name="p19767145744712"></a><a name="p19767145744712"></a>-inf</p>
</td>
</tr>
<tr id="row317514364413"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1117523616414"><a name="p1117523616414"></a><a name="p1117523616414"></a>63个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1242053204713"><a name="p1242053204713"></a><a name="p1242053204713"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18767175774716"><a name="p18767175774716"></a><a name="p18767175774716"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 38**  ReduceRepeat<SUM\>特殊值/边界值输入的计算结果

<a name="table9281103820417"></a>
<table><thead align="left"><tr id="row1928153810417"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p82811738114112"><a name="p82811738114112"></a><a name="p82811738114112"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p32811438184120"><a name="p32811438184120"></a><a name="p32811438184120"></a>src（8个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p52811838174116"><a name="p52811838174116"></a><a name="p52811838174116"></a>dst的元素取值</p>
<p id="p122811038194116"><a name="p122811038194116"></a><a name="p122811038194116"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p5281153844116"><a name="p5281153844116"></a><a name="p5281153844116"></a>dst的元素取值</p>
<p id="p1028183814117"><a name="p1028183814117"></a><a name="p1028183814117"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row1428112382410"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p92813383415"><a name="p92813383415"></a><a name="p92813383415"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p9281143813419"><a name="p9281143813419"></a><a name="p9281143813419"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p1587182313480"><a name="p1587182313480"></a><a name="p1587182313480"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p72811938164119"><a name="p72811938164119"></a><a name="p72811938164119"></a>-65504.0</p>
</td>
</tr>
<tr id="row8281238184113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p202811938174119"><a name="p202811938174119"></a><a name="p202811938174119"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p175878233486"><a name="p175878233486"></a><a name="p175878233486"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p028123818415"><a name="p028123818415"></a><a name="p028123818415"></a>65504.0</p>
</td>
</tr>
<tr id="row172811938114115"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p728110382419"><a name="p728110382419"></a><a name="p728110382419"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17587152314489"><a name="p17587152314489"></a><a name="p17587152314489"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p12281173844113"><a name="p12281173844113"></a><a name="p12281173844113"></a>0.0</p>
</td>
</tr>
<tr id="row11281938124117"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p14281123834117"><a name="p14281123834117"></a><a name="p14281123834117"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11281153894114"><a name="p11281153894114"></a><a name="p11281153894114"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p928143819410"><a name="p928143819410"></a><a name="p928143819410"></a>-0.0</p>
</td>
</tr>
<tr id="row12281163811412"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p9281163818415"><a name="p9281163818415"></a><a name="p9281163818415"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p48411831134820"><a name="p48411831134820"></a><a name="p48411831134820"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p128926372488"><a name="p128926372488"></a><a name="p128926372488"></a>0.0</p>
</td>
</tr>
<tr id="row11282338204114"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16282738144112"><a name="p16282738144112"></a><a name="p16282738144112"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4841131194812"><a name="p4841131194812"></a><a name="p4841131194812"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1189233710480"><a name="p1189233710480"></a><a name="p1189233710480"></a>65504.0</p>
</td>
</tr>
<tr id="row6282438154114"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p328253864118"><a name="p328253864118"></a><a name="p328253864118"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18411431154814"><a name="p18411431154814"></a><a name="p18411431154814"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p58921337104811"><a name="p58921337104811"></a><a name="p58921337104811"></a>-65504.0</p>
</td>
</tr>
<tr id="row1528293884115"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p628253816414"><a name="p628253816414"></a><a name="p628253816414"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p68411731204816"><a name="p68411731204816"></a><a name="p68411731204816"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p489283718487"><a name="p489283718487"></a><a name="p489283718487"></a>0.0</p>
</td>
</tr>
<tr id="row1128243815411"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p19282203844119"><a name="p19282203844119"></a><a name="p19282203844119"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p684103194820"><a name="p684103194820"></a><a name="p684103194820"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p17892133717483"><a name="p17892133717483"></a><a name="p17892133717483"></a>0.0</p>
</td>
</tr>
<tr id="row13282173814114"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p428219388412"><a name="p428219388412"></a><a name="p428219388412"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p08419313485"><a name="p08419313485"></a><a name="p08419313485"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p118927375488"><a name="p118927375488"></a><a name="p118927375488"></a>0.0</p>
</td>
</tr>
<tr id="row32824387414"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p028210382419"><a name="p028210382419"></a><a name="p028210382419"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p284133110486"><a name="p284133110486"></a><a name="p284133110486"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p889233714813"><a name="p889233714813"></a><a name="p889233714813"></a>0.0</p>
</td>
</tr>
<tr id="row128216388418"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p12282438144119"><a name="p12282438144119"></a><a name="p12282438144119"></a>127个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p984153194810"><a name="p984153194810"></a><a name="p984153194810"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8892143715484"><a name="p8892143715484"></a><a name="p8892143715484"></a>65504.0</p>
</td>
</tr>
<tr id="row14282938124119"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p228214381418"><a name="p228214381418"></a><a name="p228214381418"></a>127个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1384193114481"><a name="p1384193114481"></a><a name="p1384193114481"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p38921737174817"><a name="p38921737174817"></a><a name="p38921737174817"></a>-65376.0</p>
</td>
</tr>
<tr id="row1628233874120"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p42826380410"><a name="p42826380410"></a><a name="p42826380410"></a>127个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p19841183194811"><a name="p19841183194811"></a><a name="p19841183194811"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1289218372486"><a name="p1289218372486"></a><a name="p1289218372486"></a>126.0</p>
</td>
</tr>
<tr id="row428217381417"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p728215388410"><a name="p728215388410"></a><a name="p728215388410"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p62820388417"><a name="p62820388417"></a><a name="p62820388417"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p1724719304495"><a name="p1724719304495"></a><a name="p1724719304495"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p6247103013491"><a name="p6247103013491"></a><a name="p6247103013491"></a>-inf</p>
</td>
</tr>
<tr id="row8282203815419"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p172831738124120"><a name="p172831738124120"></a><a name="p172831738124120"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4247123094912"><a name="p4247123094912"></a><a name="p4247123094912"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8247830144911"><a name="p8247830144911"></a><a name="p8247830144911"></a>inf</p>
</td>
</tr>
<tr id="row028323834118"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1328311384415"><a name="p1328311384415"></a><a name="p1328311384415"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p42474309494"><a name="p42474309494"></a><a name="p42474309494"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p624753094910"><a name="p624753094910"></a><a name="p624753094910"></a>0.0</p>
</td>
</tr>
<tr id="row928393864111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p12831438204118"><a name="p12831438204118"></a><a name="p12831438204118"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p102831238114114"><a name="p102831238114114"></a><a name="p102831238114114"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p15283238164111"><a name="p15283238164111"></a><a name="p15283238164111"></a>-0.0</p>
</td>
</tr>
<tr id="row1028393884111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p12831738124113"><a name="p12831738124113"></a><a name="p12831738124113"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1213934819499"><a name="p1213934819499"></a><a name="p1213934819499"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p531035316494"><a name="p531035316494"></a><a name="p531035316494"></a>nan</p>
</td>
</tr>
<tr id="row228313817415"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p172838389419"><a name="p172838389419"></a><a name="p172838389419"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11391748154914"><a name="p11391748154914"></a><a name="p11391748154914"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p143105539495"><a name="p143105539495"></a><a name="p143105539495"></a>inf</p>
</td>
</tr>
<tr id="row1528343824118"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p11283113813411"><a name="p11283113813411"></a><a name="p11283113813411"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1513954854912"><a name="p1513954854912"></a><a name="p1513954854912"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p143101753144911"><a name="p143101753144911"></a><a name="p143101753144911"></a>-inf</p>
</td>
</tr>
<tr id="row7283123864111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1828373804112"><a name="p1828373804112"></a><a name="p1828373804112"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2013964819493"><a name="p2013964819493"></a><a name="p2013964819493"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2031135384917"><a name="p2031135384917"></a><a name="p2031135384917"></a>nan</p>
</td>
</tr>
<tr id="row1928319384418"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p18283038104120"><a name="p18283038104120"></a><a name="p18283038104120"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7140174816491"><a name="p7140174816491"></a><a name="p7140174816491"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p631115364913"><a name="p631115364913"></a><a name="p631115364913"></a>nan</p>
</td>
</tr>
<tr id="row22835384411"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p202837381413"><a name="p202837381413"></a><a name="p202837381413"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1314014810499"><a name="p1314014810499"></a><a name="p1314014810499"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1231125344910"><a name="p1231125344910"></a><a name="p1231125344910"></a>nan</p>
</td>
</tr>
<tr id="row16283163804111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p528316386415"><a name="p528316386415"></a><a name="p528316386415"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8140174820499"><a name="p8140174820499"></a><a name="p8140174820499"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1311115374912"><a name="p1311115374912"></a><a name="p1311115374912"></a>nan</p>
</td>
</tr>
<tr id="row162836382416"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1283113816415"><a name="p1283113816415"></a><a name="p1283113816415"></a>63个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1914084874915"><a name="p1914084874915"></a><a name="p1914084874915"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p12311105354910"><a name="p12311105354910"></a><a name="p12311105354910"></a>inf</p>
</td>
</tr>
<tr id="row18284123820416"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1284138204113"><a name="p1284138204113"></a><a name="p1284138204113"></a>63个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p81407481495"><a name="p81407481495"></a><a name="p81407481495"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1531145318498"><a name="p1531145318498"></a><a name="p1531145318498"></a>-inf</p>
</td>
</tr>
<tr id="row928443819415"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p528410387411"><a name="p528410387411"></a><a name="p528410387411"></a>63个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p71405484495"><a name="p71405484495"></a><a name="p71405484495"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p3311145318494"><a name="p3311145318494"></a><a name="p3311145318494"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 39**  ReduceMax特殊值/边界值输入的计算结果

<a name="table491948105210"></a>
<table><thead align="left"><tr id="row139202885217"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p992015865219"><a name="p992015865219"></a><a name="p992015865219"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p59201687526"><a name="p59201687526"></a><a name="p59201687526"></a>src（8个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p1092048195215"><a name="p1092048195215"></a><a name="p1092048195215"></a>dst的元素取值</p>
<p id="p119201288525"><a name="p119201288525"></a><a name="p119201288525"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p13920183521"><a name="p13920183521"></a><a name="p13920183521"></a>dst的元素取值</p>
<p id="p7920687521"><a name="p7920687521"></a><a name="p7920687521"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row149209825210"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p7920988526"><a name="p7920988526"></a><a name="p7920988526"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p592098165219"><a name="p592098165219"></a><a name="p592098165219"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p292013818528"><a name="p292013818528"></a><a name="p292013818528"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p1692014825214"><a name="p1692014825214"></a><a name="p1692014825214"></a>-65504.0</p>
</td>
</tr>
<tr id="row39203812528"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1392019865214"><a name="p1392019865214"></a><a name="p1392019865214"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1292011825213"><a name="p1292011825213"></a><a name="p1292011825213"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p139201180528"><a name="p139201180528"></a><a name="p139201180528"></a>65504.0</p>
</td>
</tr>
<tr id="row12920148155219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1392019814524"><a name="p1392019814524"></a><a name="p1392019814524"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p992017885214"><a name="p992017885214"></a><a name="p992017885214"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p109209855212"><a name="p109209855212"></a><a name="p109209855212"></a>0.0</p>
</td>
</tr>
<tr id="row1192018885218"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p11920138165212"><a name="p11920138165212"></a><a name="p11920138165212"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9920188125219"><a name="p9920188125219"></a><a name="p9920188125219"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14920885526"><a name="p14920885526"></a><a name="p14920885526"></a>-0.0</p>
</td>
</tr>
<tr id="row189204818522"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p7920128145210"><a name="p7920128145210"></a><a name="p7920128145210"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p999120101007"><a name="p999120101007"></a><a name="p999120101007"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2049035803"><a name="p2049035803"></a><a name="p2049035803"></a>0.0</p>
</td>
</tr>
<tr id="row492028145217"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p179201988529"><a name="p179201988529"></a><a name="p179201988529"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p199912102009"><a name="p199912102009"></a><a name="p199912102009"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p54903513013"><a name="p54903513013"></a><a name="p54903513013"></a>65504.0</p>
</td>
</tr>
<tr id="row19920168175217"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2920388521"><a name="p2920388521"></a><a name="p2920388521"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p189915101406"><a name="p189915101406"></a><a name="p189915101406"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8490854011"><a name="p8490854011"></a><a name="p8490854011"></a>-65504.0</p>
</td>
</tr>
<tr id="row199208819528"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p549012346530"><a name="p549012346530"></a><a name="p549012346530"></a>inf，-inf，nan加253个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p69911103010"><a name="p69911103010"></a><a name="p69911103010"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11491753012"><a name="p11491753012"></a><a name="p11491753012"></a>1.0</p>
</td>
</tr>
<tr id="row13921178155217"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1490203413534"><a name="p1490203413534"></a><a name="p1490203413534"></a>inf，-inf加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7991161019015"><a name="p7991161019015"></a><a name="p7991161019015"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11491359015"><a name="p11491359015"></a><a name="p11491359015"></a>65504.0</p>
</td>
</tr>
<tr id="row19218817524"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3490103411535"><a name="p3490103411535"></a><a name="p3490103411535"></a>inf，nan加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p189911910904"><a name="p189911910904"></a><a name="p189911910904"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p18491751305"><a name="p18491751305"></a><a name="p18491751305"></a>1.0</p>
</td>
</tr>
<tr id="row59211888527"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p449063495319"><a name="p449063495319"></a><a name="p449063495319"></a>-inf，nan加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1699114101107"><a name="p1699114101107"></a><a name="p1699114101107"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p849120514013"><a name="p849120514013"></a><a name="p849120514013"></a>1.0</p>
</td>
</tr>
<tr id="row792120820525"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p949033415320"><a name="p949033415320"></a><a name="p949033415320"></a>255个1.0加一个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p69919107010"><a name="p69919107010"></a><a name="p69919107010"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9491751013"><a name="p9491751013"></a><a name="p9491751013"></a>65504.0</p>
</td>
</tr>
<tr id="row5921148135211"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p0490134185315"><a name="p0490134185315"></a><a name="p0490134185315"></a>255个1.0加一个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p39917105013"><a name="p39917105013"></a><a name="p39917105013"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19491115608"><a name="p19491115608"></a><a name="p19491115608"></a>1.0</p>
</td>
</tr>
<tr id="row14921158195215"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1949003495317"><a name="p1949003495317"></a><a name="p1949003495317"></a>255个1.0加一个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1299112101019"><a name="p1299112101019"></a><a name="p1299112101019"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p34918513013"><a name="p34918513013"></a><a name="p34918513013"></a>1.0</p>
</td>
</tr>
<tr id="row4921085525"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p1792138205217"><a name="p1792138205217"></a><a name="p1792138205217"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p199210814522"><a name="p199210814522"></a><a name="p199210814522"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p179213865214"><a name="p179213865214"></a><a name="p179213865214"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p1892110805219"><a name="p1892110805219"></a><a name="p1892110805219"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row3921188195217"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p29215810522"><a name="p29215810522"></a><a name="p29215810522"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1992115810520"><a name="p1992115810520"></a><a name="p1992115810520"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p49218810524"><a name="p49218810524"></a><a name="p49218810524"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row1992114825220"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p5921186527"><a name="p5921186527"></a><a name="p5921186527"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1092115835217"><a name="p1092115835217"></a><a name="p1092115835217"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1292110812527"><a name="p1292110812527"></a><a name="p1292110812527"></a>0.0</p>
</td>
</tr>
<tr id="row29211581527"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1192118825212"><a name="p1192118825212"></a><a name="p1192118825212"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1692128105211"><a name="p1692128105211"></a><a name="p1692128105211"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p292178135215"><a name="p292178135215"></a><a name="p292178135215"></a>-0.0</p>
</td>
</tr>
<tr id="row59217845212"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p129221875215"><a name="p129221875215"></a><a name="p129221875215"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p58532377013"><a name="p58532377013"></a><a name="p58532377013"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1614810431609"><a name="p1614810431609"></a><a name="p1614810431609"></a>nan</p>
</td>
</tr>
<tr id="row1592218885219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16922289526"><a name="p16922289526"></a><a name="p16922289526"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1085317371602"><a name="p1085317371602"></a><a name="p1085317371602"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p314819431014"><a name="p314819431014"></a><a name="p314819431014"></a>inf</p>
</td>
</tr>
<tr id="row892258115219"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p79221386528"><a name="p79221386528"></a><a name="p79221386528"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p68539372008"><a name="p68539372008"></a><a name="p68539372008"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1414810437019"><a name="p1414810437019"></a><a name="p1414810437019"></a>-inf</p>
</td>
</tr>
<tr id="row89221185525"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p109225813521"><a name="p109225813521"></a><a name="p109225813521"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1085318376014"><a name="p1085318376014"></a><a name="p1085318376014"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p31484431202"><a name="p31484431202"></a><a name="p31484431202"></a>nan</p>
</td>
</tr>
<tr id="row19221486522"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p49223817525"><a name="p49223817525"></a><a name="p49223817525"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p785319373019"><a name="p785319373019"></a><a name="p785319373019"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p914894313019"><a name="p914894313019"></a><a name="p914894313019"></a>inf</p>
</td>
</tr>
<tr id="row1492216875216"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p149221884522"><a name="p149221884522"></a><a name="p149221884522"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2085315371304"><a name="p2085315371304"></a><a name="p2085315371304"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19149144317012"><a name="p19149144317012"></a><a name="p19149144317012"></a>nan</p>
</td>
</tr>
<tr id="row1292219812526"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1492216835218"><a name="p1492216835218"></a><a name="p1492216835218"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p285314371305"><a name="p285314371305"></a><a name="p285314371305"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16149174312013"><a name="p16149174312013"></a><a name="p16149174312013"></a>nan</p>
</td>
</tr>
<tr id="row59229835213"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p109227885214"><a name="p109227885214"></a><a name="p109227885214"></a>63个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p78531371501"><a name="p78531371501"></a><a name="p78531371501"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p181495431004"><a name="p181495431004"></a><a name="p181495431004"></a>inf</p>
</td>
</tr>
<tr id="row59221788521"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p492215805215"><a name="p492215805215"></a><a name="p492215805215"></a>63个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p685313710016"><a name="p685313710016"></a><a name="p685313710016"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p131497431505"><a name="p131497431505"></a><a name="p131497431505"></a>1.0</p>
</td>
</tr>
<tr id="row109221689529"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p59228814526"><a name="p59228814526"></a><a name="p59228814526"></a>63个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7853183710015"><a name="p7853183710015"></a><a name="p7853183710015"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2149643400"><a name="p2149643400"></a><a name="p2149643400"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 40**  ReduceMin特殊值/边界值输入的计算结果

<a name="table144621256001"></a>
<table><thead align="left"><tr id="row946265612012"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p184629561100"><a name="p184629561100"></a><a name="p184629561100"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p7462115617014"><a name="p7462115617014"></a><a name="p7462115617014"></a>src（8个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p24621561808"><a name="p24621561808"></a><a name="p24621561808"></a>dst的元素取值</p>
<p id="p124626561501"><a name="p124626561501"></a><a name="p124626561501"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p1746212562020"><a name="p1746212562020"></a><a name="p1746212562020"></a>dst的元素取值</p>
<p id="p04621056100"><a name="p04621056100"></a><a name="p04621056100"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row3462155612019"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p1846217561404"><a name="p1846217561404"></a><a name="p1846217561404"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p14627562018"><a name="p14627562018"></a><a name="p14627562018"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p346265617016"><a name="p346265617016"></a><a name="p346265617016"></a>-65504.0</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p1246265614020"><a name="p1246265614020"></a><a name="p1246265614020"></a>-65504.0</p>
</td>
</tr>
<tr id="row164621561017"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p194628568011"><a name="p194628568011"></a><a name="p194628568011"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p246285619016"><a name="p246285619016"></a><a name="p246285619016"></a>65504.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p146210561204"><a name="p146210561204"></a><a name="p146210561204"></a>65504.0</p>
</td>
</tr>
<tr id="row144627567013"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3462195617010"><a name="p3462195617010"></a><a name="p3462195617010"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p174621561304"><a name="p174621561304"></a><a name="p174621561304"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1746210568013"><a name="p1746210568013"></a><a name="p1746210568013"></a>0.0</p>
</td>
</tr>
<tr id="row184621156409"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p144628564018"><a name="p144628564018"></a><a name="p144628564018"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8587144818113"><a name="p8587144818113"></a><a name="p8587144818113"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p897854915111"><a name="p897854915111"></a><a name="p897854915111"></a>-0.0</p>
</td>
</tr>
<tr id="row164628561502"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2463105617019"><a name="p2463105617019"></a><a name="p2463105617019"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1155713111121"><a name="p1155713111121"></a><a name="p1155713111121"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1137912029"><a name="p1137912029"></a><a name="p1137912029"></a>0.0</p>
</td>
</tr>
<tr id="row64637561907"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p164632561102"><a name="p164632561102"></a><a name="p164632561102"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p655720115210"><a name="p655720115210"></a><a name="p655720115210"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1737611322"><a name="p1737611322"></a><a name="p1737611322"></a>65504.0</p>
</td>
</tr>
<tr id="row11463195612015"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p154635561307"><a name="p154635561307"></a><a name="p154635561307"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p455720111323"><a name="p455720111323"></a><a name="p455720111323"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p337911428"><a name="p337911428"></a><a name="p337911428"></a>-65504.0</p>
</td>
</tr>
<tr id="row24632561309"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p20463105611015"><a name="p20463105611015"></a><a name="p20463105611015"></a>inf，-inf，nan加253个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p955717119216"><a name="p955717119216"></a><a name="p955717119216"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p6371011320"><a name="p6371011320"></a><a name="p6371011320"></a>0.0</p>
</td>
</tr>
<tr id="row446318561701"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p846335610016"><a name="p846335610016"></a><a name="p846335610016"></a>inf，-inf加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p455713111421"><a name="p455713111421"></a><a name="p455713111421"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p163718111214"><a name="p163718111214"></a><a name="p163718111214"></a>-65504.0</p>
</td>
</tr>
<tr id="row194635569012"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p34637561602"><a name="p34637561602"></a><a name="p34637561602"></a>inf，nan加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p155578111627"><a name="p155578111627"></a><a name="p155578111627"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1637612029"><a name="p1637612029"></a><a name="p1637612029"></a>0.0</p>
</td>
</tr>
<tr id="row17463195614018"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p746310561901"><a name="p746310561901"></a><a name="p746310561901"></a>-inf，nan加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1055720111820"><a name="p1055720111820"></a><a name="p1055720111820"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1437111922"><a name="p1437111922"></a><a name="p1437111922"></a>0.0</p>
</td>
</tr>
<tr id="row1446305613010"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p184633565014"><a name="p184633565014"></a><a name="p184633565014"></a>255个1.0加一个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16557151116215"><a name="p16557151116215"></a><a name="p16557151116215"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1837111226"><a name="p1837111226"></a><a name="p1837111226"></a>1.0</p>
</td>
</tr>
<tr id="row134632561204"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p11463185616015"><a name="p11463185616015"></a><a name="p11463185616015"></a>255个1.0加一个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1855718111329"><a name="p1855718111329"></a><a name="p1855718111329"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p133731624"><a name="p133731624"></a><a name="p133731624"></a>-65504.0</p>
</td>
</tr>
<tr id="row1046417561503"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1446415613020"><a name="p1446415613020"></a><a name="p1446415613020"></a>255个1.0加一个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1355711119213"><a name="p1355711119213"></a><a name="p1355711119213"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1637711421"><a name="p1637711421"></a><a name="p1637711421"></a>0.0</p>
</td>
</tr>
<tr id="row154649569015"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p19464155619015"><a name="p19464155619015"></a><a name="p19464155619015"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p104644561702"><a name="p104644561702"></a><a name="p104644561702"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p154644561802"><a name="p154644561802"></a><a name="p154644561802"></a>-3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p546455613013"><a name="p546455613013"></a><a name="p546455613013"></a>-3.4028235e+38</p>
</td>
</tr>
<tr id="row104647561804"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p194641561108"><a name="p194641561108"></a><a name="p194641561108"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p946465615013"><a name="p946465615013"></a><a name="p946465615013"></a>3.4028235e+38</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p44643564014"><a name="p44643564014"></a><a name="p44643564014"></a>3.4028235e+38</p>
</td>
</tr>
<tr id="row154649568010"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1546419569013"><a name="p1546419569013"></a><a name="p1546419569013"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p54641956402"><a name="p54641956402"></a><a name="p54641956402"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1946418561808"><a name="p1946418561808"></a><a name="p1946418561808"></a>0.0</p>
</td>
</tr>
<tr id="row6464145614012"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p114641756707"><a name="p114641756707"></a><a name="p114641756707"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p45481524120"><a name="p45481524120"></a><a name="p45481524120"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p179871153313"><a name="p179871153313"></a><a name="p179871153313"></a>-0.0</p>
</td>
</tr>
<tr id="row746414564010"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1946417563013"><a name="p1946417563013"></a><a name="p1946417563013"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p92615505319"><a name="p92615505319"></a><a name="p92615505319"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p13262104416316"><a name="p13262104416316"></a><a name="p13262104416316"></a>nan</p>
</td>
</tr>
<tr id="row246411567018"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1046413561208"><a name="p1046413561208"></a><a name="p1046413561208"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p10261750735"><a name="p10261750735"></a><a name="p10261750735"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p152624445314"><a name="p152624445314"></a><a name="p152624445314"></a>inf</p>
</td>
</tr>
<tr id="row1546495615012"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1464135610014"><a name="p1464135610014"></a><a name="p1464135610014"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11261508316"><a name="p11261508316"></a><a name="p11261508316"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2262144410315"><a name="p2262144410315"></a><a name="p2262144410315"></a>-inf</p>
</td>
</tr>
<tr id="row2046516569018"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p7465165611014"><a name="p7465165611014"></a><a name="p7465165611014"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p3267501132"><a name="p3267501132"></a><a name="p3267501132"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1126212441735"><a name="p1126212441735"></a><a name="p1126212441735"></a>nan</p>
</td>
</tr>
<tr id="row154651956105"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p446520563015"><a name="p446520563015"></a><a name="p446520563015"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p152611501730"><a name="p152611501730"></a><a name="p152611501730"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5262174419320"><a name="p5262174419320"></a><a name="p5262174419320"></a>-inf</p>
</td>
</tr>
<tr id="row1465125616012"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p24651956709"><a name="p24651956709"></a><a name="p24651956709"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p122611500318"><a name="p122611500318"></a><a name="p122611500318"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p326204413311"><a name="p326204413311"></a><a name="p326204413311"></a>nan</p>
</td>
</tr>
<tr id="row846511561006"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1946512561019"><a name="p1946512561019"></a><a name="p1946512561019"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p192695011317"><a name="p192695011317"></a><a name="p192695011317"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2026264419314"><a name="p2026264419314"></a><a name="p2026264419314"></a>nan</p>
</td>
</tr>
<tr id="row9465195619014"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p74652561012"><a name="p74652561012"></a><a name="p74652561012"></a>63个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p102675010315"><a name="p102675010315"></a><a name="p102675010315"></a>1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p526224412310"><a name="p526224412310"></a><a name="p526224412310"></a>1.0</p>
</td>
</tr>
<tr id="row174651956907"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p14651056604"><a name="p14651056604"></a><a name="p14651056604"></a>63个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4276501137"><a name="p4276501137"></a><a name="p4276501137"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p926217441037"><a name="p926217441037"></a><a name="p926217441037"></a>-inf</p>
</td>
</tr>
<tr id="row10465456202"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p134658567017"><a name="p134658567017"></a><a name="p134658567017"></a>63个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p172711509320"><a name="p172711509320"></a><a name="p172711509320"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2026216445314"><a name="p2026216445314"></a><a name="p2026216445314"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

**表 41**  ReduceSum特殊值/边界值输入的计算结果

<a name="table16600131416"></a>
<table><thead align="left"><tr id="row16001831512"><th class="cellrowborder" valign="top" width="22.85%" id="mcps1.2.5.1.1"><p id="p15600138111"><a name="p15600138111"></a><a name="p15600138111"></a>src/dst的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="19.75%" id="mcps1.2.5.1.2"><p id="p5600131118"><a name="p5600131118"></a><a name="p5600131118"></a>src（8个DataBlock）的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.3"><p id="p260010315111"><a name="p260010315111"></a><a name="p260010315111"></a>dst的元素取值</p>
<p id="p360019310116"><a name="p360019310116"></a><a name="p360019310116"></a>（INF/NAN模式）</p>
</th>
<th class="cellrowborder" valign="top" width="28.7%" id="mcps1.2.5.1.4"><p id="p8600931514"><a name="p8600931514"></a><a name="p8600931514"></a>dst的元素取值</p>
<p id="p960043118"><a name="p960043118"></a><a name="p960043118"></a>（饱和模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row14600634114"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p176001536113"><a name="p176001536113"></a><a name="p176001536113"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p760083816"><a name="p760083816"></a><a name="p760083816"></a>全为-65504.0（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p9945292419"><a name="p9945292419"></a><a name="p9945292419"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p59031416411"><a name="p59031416411"></a><a name="p59031416411"></a>-65504.0</p>
</td>
</tr>
<tr id="row1160003817"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1460073513"><a name="p1460073513"></a><a name="p1460073513"></a>全为65504.0（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p294518917419"><a name="p294518917419"></a><a name="p294518917419"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7902141414"><a name="p7902141414"></a><a name="p7902141414"></a>65504.0</p>
</td>
</tr>
<tr id="row13601931417"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p166019311111"><a name="p166019311111"></a><a name="p166019311111"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p99451193415"><a name="p99451193415"></a><a name="p99451193415"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p139013147416"><a name="p139013147416"></a><a name="p139013147416"></a>0.0</p>
</td>
</tr>
<tr id="row760113311115"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1601638110"><a name="p1601638110"></a><a name="p1601638110"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p45231569119"><a name="p45231569119"></a><a name="p45231569119"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p197915581518"><a name="p197915581518"></a><a name="p197915581518"></a>-0.0</p>
</td>
</tr>
<tr id="row1660193417"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4601103313"><a name="p4601103313"></a><a name="p4601103313"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p126823251649"><a name="p126823251649"></a><a name="p126823251649"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5220133118419"><a name="p5220133118419"></a><a name="p5220133118419"></a>0.0</p>
</td>
</tr>
<tr id="row18601631916"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p156011234114"><a name="p156011234114"></a><a name="p156011234114"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1268211259411"><a name="p1268211259411"></a><a name="p1268211259411"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p132213317413"><a name="p132213317413"></a><a name="p132213317413"></a>65504.0</p>
</td>
</tr>
<tr id="row12601631715"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p196011830116"><a name="p196011830116"></a><a name="p196011830116"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p20682425744"><a name="p20682425744"></a><a name="p20682425744"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1622117312041"><a name="p1622117312041"></a><a name="p1622117312041"></a>-65504.0</p>
</td>
</tr>
<tr id="row1760115311113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1460114313113"><a name="p1460114313113"></a><a name="p1460114313113"></a>inf，-inf，nan加253个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p168218251943"><a name="p168218251943"></a><a name="p168218251943"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p522118311419"><a name="p522118311419"></a><a name="p522118311419"></a>252.0</p>
</td>
</tr>
<tr id="row1860113118"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1960143311"><a name="p1960143311"></a><a name="p1960143311"></a>inf，-inf加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1568282510417"><a name="p1568282510417"></a><a name="p1568282510417"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1322153118419"><a name="p1322153118419"></a><a name="p1322153118419"></a>254.0</p>
</td>
</tr>
<tr id="row14601231819"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p186011431317"><a name="p186011431317"></a><a name="p186011431317"></a>inf，nan加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11682725441"><a name="p11682725441"></a><a name="p11682725441"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1722113317415"><a name="p1722113317415"></a><a name="p1722113317415"></a>254.0</p>
</td>
</tr>
<tr id="row7602236111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17602838110"><a name="p17602838110"></a><a name="p17602838110"></a>-inf，nan加254个1.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p126826251745"><a name="p126826251745"></a><a name="p126826251745"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p02219311747"><a name="p02219311747"></a><a name="p02219311747"></a>254.0</p>
</td>
</tr>
<tr id="row1660220316111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8602731310"><a name="p8602731310"></a><a name="p8602731310"></a>255个1.0加一个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1668232517416"><a name="p1668232517416"></a><a name="p1668232517416"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1122113116413"><a name="p1122113116413"></a><a name="p1122113116413"></a>65504.0</p>
</td>
</tr>
<tr id="row9602833111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1602831715"><a name="p1602831715"></a><a name="p1602831715"></a>255个1.0加一个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1768210251847"><a name="p1768210251847"></a><a name="p1768210251847"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11221193110418"><a name="p11221193110418"></a><a name="p11221193110418"></a>-65248.0</p>
</td>
</tr>
<tr id="row76021231110"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p14602439118"><a name="p14602439118"></a><a name="p14602439118"></a>255个1.0加一个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p068212251246"><a name="p068212251246"></a><a name="p068212251246"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p122123115420"><a name="p122123115420"></a><a name="p122123115420"></a>254.0</p>
</td>
</tr>
<tr id="row86021311118"><td class="cellrowborder" rowspan="14" valign="top" width="22.85%" headers="mcps1.2.5.1.1 "><p id="p206021131617"><a name="p206021131617"></a><a name="p206021131617"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="19.75%" headers="mcps1.2.5.1.2 "><p id="p1260243912"><a name="p1260243912"></a><a name="p1260243912"></a>全为-3.4028235e+38（MIN）</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.3 "><p id="p12311412516"><a name="p12311412516"></a><a name="p12311412516"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" width="28.7%" headers="mcps1.2.5.1.4 "><p id="p20311711950"><a name="p20311711950"></a><a name="p20311711950"></a>-inf</p>
</td>
</tr>
<tr id="row13602163917"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p56027310111"><a name="p56027310111"></a><a name="p56027310111"></a>全为3.4028235e+38（MAX）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p53111017513"><a name="p53111017513"></a><a name="p53111017513"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p163111111457"><a name="p163111111457"></a><a name="p163111111457"></a>inf</p>
</td>
</tr>
<tr id="row060233717"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p66021131213"><a name="p66021131213"></a><a name="p66021131213"></a>全为0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p23111011157"><a name="p23111011157"></a><a name="p23111011157"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7311111259"><a name="p7311111259"></a><a name="p7311111259"></a>0.0</p>
</td>
</tr>
<tr id="row106021739117"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13602531113"><a name="p13602531113"></a><a name="p13602531113"></a>全为-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16615123010212"><a name="p16615123010212"></a><a name="p16615123010212"></a>-0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p883063417210"><a name="p883063417210"></a><a name="p883063417210"></a>-0.0</p>
</td>
</tr>
<tr id="row360333314"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p196031532116"><a name="p196031532116"></a><a name="p196031532116"></a>全为nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p43442222512"><a name="p43442222512"></a><a name="p43442222512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p692615912515"><a name="p692615912515"></a><a name="p692615912515"></a>nan</p>
</td>
</tr>
<tr id="row136039314113"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17603434110"><a name="p17603434110"></a><a name="p17603434110"></a>全为inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p63440228515"><a name="p63440228515"></a><a name="p63440228515"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p992629655"><a name="p992629655"></a><a name="p992629655"></a>inf</p>
</td>
</tr>
<tr id="row76034318111"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p10603193513"><a name="p10603193513"></a><a name="p10603193513"></a>全为-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p63446221357"><a name="p63446221357"></a><a name="p63446221357"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1492617918519"><a name="p1492617918519"></a><a name="p1492617918519"></a>-inf</p>
</td>
</tr>
<tr id="row7603173813"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p560363516"><a name="p560363516"></a><a name="p560363516"></a>inf，-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1234414229511"><a name="p1234414229511"></a><a name="p1234414229511"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1092616918515"><a name="p1092616918515"></a><a name="p1092616918515"></a>nan</p>
</td>
</tr>
<tr id="row6603634115"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p06036312113"><a name="p06036312113"></a><a name="p06036312113"></a>inf，-inf 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1934432218512"><a name="p1934432218512"></a><a name="p1934432218512"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p79261790520"><a name="p79261790520"></a><a name="p79261790520"></a>nan</p>
</td>
</tr>
<tr id="row1260320318119"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p86031631817"><a name="p86031631817"></a><a name="p86031631817"></a>inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p33442221855"><a name="p33442221855"></a><a name="p33442221855"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p7926199252"><a name="p7926199252"></a><a name="p7926199252"></a>nan</p>
</td>
</tr>
<tr id="row17603536115"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p46035318111"><a name="p46035318111"></a><a name="p46035318111"></a>-inf，nan 交替</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1344222356"><a name="p1344222356"></a><a name="p1344222356"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9927159556"><a name="p9927159556"></a><a name="p9927159556"></a>nan</p>
</td>
</tr>
<tr id="row196037312116"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p7603137114"><a name="p7603137114"></a><a name="p7603137114"></a>63个1.0加1个inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16344142219518"><a name="p16344142219518"></a><a name="p16344142219518"></a>inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1927891859"><a name="p1927891859"></a><a name="p1927891859"></a>inf</p>
</td>
</tr>
<tr id="row156044317117"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p46048320119"><a name="p46048320119"></a><a name="p46048320119"></a>63个1.0加1个-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2034420221755"><a name="p2034420221755"></a><a name="p2034420221755"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p69271691554"><a name="p69271691554"></a><a name="p69271691554"></a>-inf</p>
</td>
</tr>
<tr id="row14604113411"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p56043316111"><a name="p56043316111"></a><a name="p56043316111"></a>63个1.0加1个nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p73441422150"><a name="p73441422150"></a><a name="p73441422150"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8927149852"><a name="p8927149852"></a><a name="p8927149852"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：dst的元素取值（INF/NAN模式）列结果，与IEEE754理论结果一致。

## 数据排布转换<a name="section33343814910"></a>

Transpose和TransDataTo5HD属于搬运类接口，不涉及特殊值/边界值。

## 数据填充<a name="section10713185619915"></a>

Brcb和Duplicate属于填充类接口，不涉及特殊值/边界值。

CreateVecIndex特殊值/边界值输入的计算结果和Adds接口一致。

## 排序组合<a name="section1342519199109"></a>

Sort32、MrgSort、GetMrgSortResult不涉及特殊值/边界值。

## 离散与聚合<a name="section14495123581018"></a>

Gather和Gatherb属于搬运类接口，不涉及特殊值/边界值。

## 掩码操作<a name="section0460049181018"></a>

SetMaskCount、SetMaskNorm、SetVectorMask和ResetMask为掩码配置接口，不涉及特殊值/边界值。

