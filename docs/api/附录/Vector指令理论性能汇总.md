# Vector指令理论性能汇总<a name="ZH-CN_TOPIC_0000002557396789"></a>

**表 1**  基础算术类指令理论性能汇总

<a name="table291476204714"></a>
<table><thead align="left"><tr id="row10914166144717"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p19412193471"><a name="p19412193471"></a><a name="p19412193471"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p541215913474"><a name="p541215913474"></a><a name="p541215913474"></a>输入/输出数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p16412699470"><a name="p16412699470"></a><a name="p16412699470"></a>硬件并行度（单位：element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row19914196184717"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p94417184712"><a name="p94417184712"></a><a name="p94417184712"></a>Add</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p16418175474"><a name="p16418175474"></a><a name="p16418175474"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p2421716472"><a name="p2421716472"></a><a name="p2421716472"></a>128</p>
</td>
</tr>
<tr id="row7914860472"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p17410178475"><a name="p17410178475"></a><a name="p17410178475"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p124131774715"><a name="p124131774715"></a><a name="p124131774715"></a>128</p>
</td>
</tr>
<tr id="row191466104710"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p15411764717"><a name="p15411764717"></a><a name="p15411764717"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1948174471"><a name="p1948174471"></a><a name="p1948174471"></a>64</p>
</td>
</tr>
<tr id="row169151614474"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1649174478"><a name="p1649174478"></a><a name="p1649174478"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p842175471"><a name="p842175471"></a><a name="p842175471"></a>64</p>
</td>
</tr>
<tr id="row882316192476"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p20465182794711"><a name="p20465182794711"></a><a name="p20465182794711"></a>Exp</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p74661327184710"><a name="p74661327184710"></a><a name="p74661327184710"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p646602784716"><a name="p646602784716"></a><a name="p646602784716"></a>32</p>
</td>
</tr>
<tr id="row3607520144717"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p74669277475"><a name="p74669277475"></a><a name="p74669277475"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1146662714472"><a name="p1146662714472"></a><a name="p1146662714472"></a>32</p>
</td>
</tr>
<tr id="row9439142394718"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p84661027154713"><a name="p84661027154713"></a><a name="p84661027154713"></a>Ln</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p94661027114711"><a name="p94661027114711"></a><a name="p94661027114711"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p24661277479"><a name="p24661277479"></a><a name="p24661277479"></a>32</p>
</td>
</tr>
<tr id="row1097182484716"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1646682712476"><a name="p1646682712476"></a><a name="p1646682712476"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p3466192794718"><a name="p3466192794718"></a><a name="p3466192794718"></a>32</p>
</td>
</tr>
<tr id="row114744013481"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p6389447487"><a name="p6389447487"></a><a name="p6389447487"></a>Abs</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p138154412482"><a name="p138154412482"></a><a name="p138154412482"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p18382448487"><a name="p18382448487"></a><a name="p18382448487"></a>128</p>
</td>
</tr>
<tr id="row14752340144819"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1438194444817"><a name="p1438194444817"></a><a name="p1438194444817"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p938104414813"><a name="p938104414813"></a><a name="p938104414813"></a>64</p>
</td>
</tr>
<tr id="row9674164619482"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1255011508487"><a name="p1255011508487"></a><a name="p1255011508487"></a>Reciprocal</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p105502050114814"><a name="p105502050114814"></a><a name="p105502050114814"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p15501550104811"><a name="p15501550104811"></a><a name="p15501550104811"></a>128</p>
</td>
</tr>
<tr id="row23289471489"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1555015094819"><a name="p1555015094819"></a><a name="p1555015094819"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p7550205016484"><a name="p7550205016484"></a><a name="p7550205016484"></a>64</p>
</td>
</tr>
<tr id="row119271553134818"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p69921858164811"><a name="p69921858164811"></a><a name="p69921858164811"></a>Sqrt</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1992758184811"><a name="p1992758184811"></a><a name="p1992758184811"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p09928584489"><a name="p09928584489"></a><a name="p09928584489"></a>32</p>
</td>
</tr>
<tr id="row1753035484818"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p18992145818488"><a name="p18992145818488"></a><a name="p18992145818488"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1992185894812"><a name="p1992185894812"></a><a name="p1992185894812"></a>32</p>
</td>
</tr>
<tr id="row11233355104816"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1699225818485"><a name="p1699225818485"></a><a name="p1699225818485"></a>Rsqrt</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p09924587484"><a name="p09924587484"></a><a name="p09924587484"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p18992125813484"><a name="p18992125813484"></a><a name="p18992125813484"></a>128</p>
</td>
</tr>
<tr id="row582212552488"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p14992158124818"><a name="p14992158124818"></a><a name="p14992158124818"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1499218581486"><a name="p1499218581486"></a><a name="p1499218581486"></a>64</p>
</td>
</tr>
<tr id="row1756282194920"><td class="cellrowborder" rowspan="3" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p145545913498"><a name="p145545913498"></a><a name="p145545913498"></a>Relu</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p755510917491"><a name="p755510917491"></a><a name="p755510917491"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p455516934916"><a name="p455516934916"></a><a name="p455516934916"></a>128</p>
</td>
</tr>
<tr id="row151781633496"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p7555490492"><a name="p7555490492"></a><a name="p7555490492"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p9555169104917"><a name="p9555169104917"></a><a name="p9555169104917"></a>64</p>
</td>
</tr>
<tr id="row976019311497"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1655510910496"><a name="p1655510910496"></a><a name="p1655510910496"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p855509174916"><a name="p855509174916"></a><a name="p855509174916"></a>64</p>
</td>
</tr>
<tr id="row12388647496"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p2055516944913"><a name="p2055516944913"></a><a name="p2055516944913"></a>Sub</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p155599194918"><a name="p155599194918"></a><a name="p155599194918"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p35557944910"><a name="p35557944910"></a><a name="p35557944910"></a>128</p>
</td>
</tr>
<tr id="row8115584911"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p7556159154912"><a name="p7556159154912"></a><a name="p7556159154912"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p755617934916"><a name="p755617934916"></a><a name="p755617934916"></a>128</p>
</td>
</tr>
<tr id="row1605115144911"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1255669114919"><a name="p1255669114919"></a><a name="p1255669114919"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p165568917499"><a name="p165568917499"></a><a name="p165568917499"></a>64</p>
</td>
</tr>
<tr id="row152276644918"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p855618915496"><a name="p855618915496"></a><a name="p855618915496"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p65564964919"><a name="p65564964919"></a><a name="p65564964919"></a>64</p>
</td>
</tr>
<tr id="row437571794915"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1318782619496"><a name="p1318782619496"></a><a name="p1318782619496"></a>Mul</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p5187172613497"><a name="p5187172613497"></a><a name="p5187172613497"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p2187142610499"><a name="p2187142610499"></a><a name="p2187142610499"></a>128</p>
</td>
</tr>
<tr id="row75521894912"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1918882619497"><a name="p1918882619497"></a><a name="p1918882619497"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p8188202611497"><a name="p8188202611497"></a><a name="p8188202611497"></a>128</p>
</td>
</tr>
<tr id="row13620111813499"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p4188112620499"><a name="p4188112620499"></a><a name="p4188112620499"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1718862624910"><a name="p1718862624910"></a><a name="p1718862624910"></a>64</p>
</td>
</tr>
<tr id="row3220141913491"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1018822674918"><a name="p1018822674918"></a><a name="p1018822674918"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p111881026124912"><a name="p111881026124912"></a><a name="p111881026124912"></a>64</p>
</td>
</tr>
<tr id="row16824021114919"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1418815265493"><a name="p1418815265493"></a><a name="p1418815265493"></a>Div</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1418852634917"><a name="p1418852634917"></a><a name="p1418852634917"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p818822664919"><a name="p818822664919"></a><a name="p818822664919"></a>32</p>
</td>
</tr>
<tr id="row1558642218492"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p318872617495"><a name="p318872617495"></a><a name="p318872617495"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p121888261494"><a name="p121888261494"></a><a name="p121888261494"></a>32</p>
</td>
</tr>
<tr id="row464673614497"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p15621042124913"><a name="p15621042124913"></a><a name="p15621042124913"></a>Max</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p115621742114917"><a name="p115621742114917"></a><a name="p115621742114917"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p17562114234918"><a name="p17562114234918"></a><a name="p17562114234918"></a>128</p>
</td>
</tr>
<tr id="row112352379496"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1562114204911"><a name="p1562114204911"></a><a name="p1562114204911"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p11562194264913"><a name="p11562194264913"></a><a name="p11562194264913"></a>128</p>
</td>
</tr>
<tr id="row882143784914"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1556214234910"><a name="p1556214234910"></a><a name="p1556214234910"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p115624424493"><a name="p115624424493"></a><a name="p115624424493"></a>64</p>
</td>
</tr>
<tr id="row8417133815494"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p16562124210496"><a name="p16562124210496"></a><a name="p16562124210496"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p16562184217494"><a name="p16562184217494"></a><a name="p16562184217494"></a>64</p>
</td>
</tr>
<tr id="row432293984910"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p6563134224916"><a name="p6563134224916"></a><a name="p6563134224916"></a>Min</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p13563114213491"><a name="p13563114213491"></a><a name="p13563114213491"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p456354244918"><a name="p456354244918"></a><a name="p456354244918"></a>128</p>
</td>
</tr>
<tr id="row5976039134912"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p17563142164916"><a name="p17563142164916"></a><a name="p17563142164916"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p17563134211496"><a name="p17563134211496"></a><a name="p17563134211496"></a>128</p>
</td>
</tr>
<tr id="row85996404490"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p115639422492"><a name="p115639422492"></a><a name="p115639422492"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1156311424498"><a name="p1156311424498"></a><a name="p1156311424498"></a>64</p>
</td>
</tr>
<tr id="row121314115496"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p14563164217492"><a name="p14563164217492"></a><a name="p14563164217492"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1956374219491"><a name="p1956374219491"></a><a name="p1956374219491"></a>64</p>
</td>
</tr>
<tr id="row54612552493"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p2625615185011"><a name="p2625615185011"></a><a name="p2625615185011"></a>Adds</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p262591514508"><a name="p262591514508"></a><a name="p262591514508"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p206251215175020"><a name="p206251215175020"></a><a name="p206251215175020"></a>128</p>
</td>
</tr>
<tr id="row1815465644917"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p662531515015"><a name="p662531515015"></a><a name="p662531515015"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p156253156506"><a name="p156253156506"></a><a name="p156253156506"></a>128</p>
</td>
</tr>
<tr id="row1274775618495"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p2062501515509"><a name="p2062501515509"></a><a name="p2062501515509"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p15625215185011"><a name="p15625215185011"></a><a name="p15625215185011"></a>64</p>
</td>
</tr>
<tr id="row736375714918"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p20625415145017"><a name="p20625415145017"></a><a name="p20625415145017"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p3625715185016"><a name="p3625715185016"></a><a name="p3625715185016"></a>64</p>
</td>
</tr>
<tr id="row111551589491"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p4625315195011"><a name="p4625315195011"></a><a name="p4625315195011"></a>Muls</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p3626615125017"><a name="p3626615125017"></a><a name="p3626615125017"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p13626315175012"><a name="p13626315175012"></a><a name="p13626315175012"></a>128</p>
</td>
</tr>
<tr id="row9827185818490"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p86261415105010"><a name="p86261415105010"></a><a name="p86261415105010"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p2626171585012"><a name="p2626171585012"></a><a name="p2626171585012"></a>128</p>
</td>
</tr>
<tr id="row653285974914"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1562601525019"><a name="p1562601525019"></a><a name="p1562601525019"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p262651525014"><a name="p262651525014"></a><a name="p262651525014"></a>64</p>
</td>
</tr>
<tr id="row815416019509"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p16626615135011"><a name="p16626615135011"></a><a name="p16626615135011"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p116261115115019"><a name="p116261115115019"></a><a name="p116261115115019"></a>64</p>
</td>
</tr>
<tr id="row147913119505"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p56261515185014"><a name="p56261515185014"></a><a name="p56261515185014"></a>Maxs</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p66267157503"><a name="p66267157503"></a><a name="p66267157503"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p14626131514503"><a name="p14626131514503"></a><a name="p14626131514503"></a>128</p>
</td>
</tr>
<tr id="row3115112195015"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p062681535016"><a name="p062681535016"></a><a name="p062681535016"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p13626915105016"><a name="p13626915105016"></a><a name="p13626915105016"></a>128</p>
</td>
</tr>
<tr id="row168756213503"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p126271615115012"><a name="p126271615115012"></a><a name="p126271615115012"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p126277158502"><a name="p126277158502"></a><a name="p126277158502"></a>64</p>
</td>
</tr>
<tr id="row205466312501"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1762714152504"><a name="p1762714152504"></a><a name="p1762714152504"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p962751525010"><a name="p962751525010"></a><a name="p962751525010"></a>64</p>
</td>
</tr>
<tr id="row209011549506"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1562751517507"><a name="p1562751517507"></a><a name="p1562751517507"></a>Mins</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1362719153501"><a name="p1362719153501"></a><a name="p1362719153501"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p562741513501"><a name="p562741513501"></a><a name="p562741513501"></a>128</p>
</td>
</tr>
<tr id="row56954525013"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p186272159504"><a name="p186272159504"></a><a name="p186272159504"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p14627115185011"><a name="p14627115185011"></a><a name="p14627115185011"></a>128</p>
</td>
</tr>
<tr id="row173141615504"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1162751515011"><a name="p1162751515011"></a><a name="p1162751515011"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p10627181515500"><a name="p10627181515500"></a><a name="p10627181515500"></a>64</p>
</td>
</tr>
<tr id="row99951965504"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p15628101545016"><a name="p15628101545016"></a><a name="p15628101545016"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p362891516508"><a name="p362891516508"></a><a name="p362891516508"></a>64</p>
</td>
</tr>
<tr id="row1752612814505"><td class="cellrowborder" rowspan="4" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p166289153507"><a name="p166289153507"></a><a name="p166289153507"></a>LeakyRelu</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p462851514507"><a name="p462851514507"></a><a name="p462851514507"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p3628415185016"><a name="p3628415185016"></a><a name="p3628415185016"></a>128</p>
</td>
</tr>
<tr id="row22671695509"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p166281315185011"><a name="p166281315185011"></a><a name="p166281315185011"></a>int16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1262818152505"><a name="p1262818152505"></a><a name="p1262818152505"></a>128</p>
</td>
</tr>
<tr id="row387319915503"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p76281015145015"><a name="p76281015145015"></a><a name="p76281015145015"></a>int32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p862811156501"><a name="p862811156501"></a><a name="p862811156501"></a>64</p>
</td>
</tr>
<tr id="row14771310195016"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p20628515125018"><a name="p20628515125018"></a><a name="p20628515125018"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p206287151509"><a name="p206287151509"></a><a name="p206287151509"></a>64</p>
</td>
</tr>
</tbody>
</table>

**表 2**  逻辑计算类指令理论性能汇总

<a name="table116881509216"></a>
<table><thead align="left"><tr id="row12688195012111"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p19150954202111"><a name="p19150954202111"></a><a name="p19150954202111"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p12150454122118"><a name="p12150454122118"></a><a name="p12150454122118"></a>src数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p31509547211"><a name="p31509547211"></a><a name="p31509547211"></a>硬件并行度（单位：element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row12689165012114"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p8150354192113"><a name="p8150354192113"></a><a name="p8150354192113"></a>Not</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p515014540217"><a name="p515014540217"></a><a name="p515014540217"></a>int16_t/uint16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p4150254202116"><a name="p4150254202116"></a><a name="p4150254202116"></a>128</p>
</td>
</tr>
<tr id="row468945017212"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1815175402116"><a name="p1815175402116"></a><a name="p1815175402116"></a>And</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p2015112540215"><a name="p2015112540215"></a><a name="p2015112540215"></a>int16_t/uint16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p5151185402116"><a name="p5151185402116"></a><a name="p5151185402116"></a>128</p>
</td>
</tr>
<tr id="row1168910505214"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p15151155418213"><a name="p15151155418213"></a><a name="p15151155418213"></a>Or</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p21511554142110"><a name="p21511554142110"></a><a name="p21511554142110"></a>int16_t/uint16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p14151854162112"><a name="p14151854162112"></a><a name="p14151854162112"></a>128</p>
</td>
</tr>
<tr id="row26891506216"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p6151175415217"><a name="p6151175415217"></a><a name="p6151175415217"></a>ShiftLeft</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p11151754192112"><a name="p11151754192112"></a><a name="p11151754192112"></a>int16_t/uint16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p6151105415216"><a name="p6151105415216"></a><a name="p6151105415216"></a>128</p>
</td>
</tr>
<tr id="row468985017217"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p6151155417215"><a name="p6151155417215"></a><a name="p6151155417215"></a>int32_t/uint32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p7151554122118"><a name="p7151554122118"></a><a name="p7151554122118"></a>64</p>
</td>
</tr>
<tr id="row16689155010211"><td class="cellrowborder" rowspan="2" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p14151105462114"><a name="p14151105462114"></a><a name="p14151105462114"></a>ShiftRight</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1715165432118"><a name="p1715165432118"></a><a name="p1715165432118"></a>int16_t/uint16_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p11511654122116"><a name="p11511654122116"></a><a name="p11511654122116"></a>128</p>
</td>
</tr>
<tr id="row136891650102112"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p15151185412120"><a name="p15151185412120"></a><a name="p15151185412120"></a>int32_t/uint32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p18151105422117"><a name="p18151105422117"></a><a name="p18151105422117"></a>64</p>
</td>
</tr>
</tbody>
</table>

**表 3**  复合计算类指令理论性能汇总

<a name="table1089823515222"></a>
<table><thead align="left"><tr id="row58992035152211"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.1"><p id="p192302479233"><a name="p192302479233"></a><a name="p192302479233"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.2"><p id="p12230164713232"><a name="p12230164713232"></a><a name="p12230164713232"></a>src0数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.3"><p id="p7230154718239"><a name="p7230154718239"></a><a name="p7230154718239"></a>scalar/src1数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p202316479231"><a name="p202316479231"></a><a name="p202316479231"></a>dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p423116471237"><a name="p423116471237"></a><a name="p423116471237"></a>硬件并行度（单位element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row189911350222"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p152311147122314"><a name="p152311147122314"></a><a name="p152311147122314"></a>Axpy</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p162311547152311"><a name="p162311547152311"></a><a name="p162311547152311"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p723124715233"><a name="p723124715233"></a><a name="p723124715233"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p523144792314"><a name="p523144792314"></a><a name="p523144792314"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1823144772318"><a name="p1823144772318"></a><a name="p1823144772318"></a>128</p>
</td>
</tr>
<tr id="row188991435182215"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p32315472230"><a name="p32315472230"></a><a name="p32315472230"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1231184711235"><a name="p1231184711235"></a><a name="p1231184711235"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p623164719230"><a name="p623164719230"></a><a name="p623164719230"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p723114762318"><a name="p723114762318"></a><a name="p723114762318"></a>64</p>
</td>
</tr>
<tr id="row1489963522217"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17231204710233"><a name="p17231204710233"></a><a name="p17231204710233"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p102327475235"><a name="p102327475235"></a><a name="p102327475235"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p42321047112319"><a name="p42321047112319"></a><a name="p42321047112319"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p523254782319"><a name="p523254782319"></a><a name="p523254782319"></a>64</p>
</td>
</tr>
<tr id="row209001435102214"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p202321947152317"><a name="p202321947152317"></a><a name="p202321947152317"></a>MulAddDst</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p12321247192318"><a name="p12321247192318"></a><a name="p12321247192318"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p142326472231"><a name="p142326472231"></a><a name="p142326472231"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p152323477233"><a name="p152323477233"></a><a name="p152323477233"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p8232174702312"><a name="p8232174702312"></a><a name="p8232174702312"></a>64</p>
</td>
</tr>
<tr id="row153281659152216"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1323218471233"><a name="p1323218471233"></a><a name="p1323218471233"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10232947112315"><a name="p10232947112315"></a><a name="p10232947112315"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p202321747122315"><a name="p202321747122315"></a><a name="p202321747122315"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p32321347142313"><a name="p32321347142313"></a><a name="p32321347142313"></a>32</p>
</td>
</tr>
<tr id="row14934110172315"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p16232194718231"><a name="p16232194718231"></a><a name="p16232194718231"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p20233647112315"><a name="p20233647112315"></a><a name="p20233647112315"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p182336470234"><a name="p182336470234"></a><a name="p182336470234"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p72331474237"><a name="p72331474237"></a><a name="p72331474237"></a>32</p>
</td>
</tr>
<tr id="row1566118122320"><td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p823312476237"><a name="p823312476237"></a><a name="p823312476237"></a>MulCast</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p152334475239"><a name="p152334475239"></a><a name="p152334475239"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p923313476235"><a name="p923313476235"></a><a name="p923313476235"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p182331477238"><a name="p182331477238"></a><a name="p182331477238"></a>uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p16233047192312"><a name="p16233047192312"></a><a name="p16233047192312"></a>128</p>
</td>
</tr>
<tr id="row1832662202317"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1623314476235"><a name="p1623314476235"></a><a name="p1623314476235"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p523313479237"><a name="p523313479237"></a><a name="p523313479237"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4233147152310"><a name="p4233147152310"></a><a name="p4233147152310"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p723314752313"><a name="p723314752313"></a><a name="p723314752313"></a>128</p>
</td>
</tr>
<tr id="row137521136236"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p623364712231"><a name="p623364712231"></a><a name="p623364712231"></a>AddRelu</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1423444712317"><a name="p1423444712317"></a><a name="p1423444712317"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p1823414711232"><a name="p1823414711232"></a><a name="p1823414711232"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p323444716235"><a name="p323444716235"></a><a name="p323444716235"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p62344479236"><a name="p62344479236"></a><a name="p62344479236"></a>128</p>
</td>
</tr>
<tr id="row1239318415239"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2023418479238"><a name="p2023418479238"></a><a name="p2023418479238"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p20234144752314"><a name="p20234144752314"></a><a name="p20234144752314"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1423434722319"><a name="p1423434722319"></a><a name="p1423434722319"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p152341147102319"><a name="p152341147102319"></a><a name="p152341147102319"></a>128</p>
</td>
</tr>
<tr id="row197821952233"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p323464718234"><a name="p323464718234"></a><a name="p323464718234"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1123444722319"><a name="p1123444722319"></a><a name="p1123444722319"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p14234114712237"><a name="p14234114712237"></a><a name="p14234114712237"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1023484702315"><a name="p1023484702315"></a><a name="p1023484702315"></a>64</p>
</td>
</tr>
<tr id="row184421663233"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p023524732310"><a name="p023524732310"></a><a name="p023524732310"></a>AddReluCast</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p192351047142318"><a name="p192351047142318"></a><a name="p192351047142318"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p223554732313"><a name="p223554732313"></a><a name="p223554732313"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p723524720239"><a name="p723524720239"></a><a name="p723524720239"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p10235047132319"><a name="p10235047132319"></a><a name="p10235047132319"></a>64</p>
</td>
</tr>
<tr id="row020317716232"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p123534742313"><a name="p123534742313"></a><a name="p123534742313"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p3235144732315"><a name="p3235144732315"></a><a name="p3235144732315"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1323517473234"><a name="p1323517473234"></a><a name="p1323517473234"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1423512471237"><a name="p1423512471237"></a><a name="p1423512471237"></a>128</p>
</td>
</tr>
<tr id="row1796347182317"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15235047152315"><a name="p15235047152315"></a><a name="p15235047152315"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p323554742318"><a name="p323554742318"></a><a name="p323554742318"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p2236194715236"><a name="p2236194715236"></a><a name="p2236194715236"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p13236174717236"><a name="p13236174717236"></a><a name="p13236174717236"></a>128</p>
</td>
</tr>
<tr id="row15231119152317"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p132361947192318"><a name="p132361947192318"></a><a name="p132361947192318"></a>SubRelu</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p42369472232"><a name="p42369472232"></a><a name="p42369472232"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p723613477233"><a name="p723613477233"></a><a name="p723613477233"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p4236647112318"><a name="p4236647112318"></a><a name="p4236647112318"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p152361847122319"><a name="p152361847122319"></a><a name="p152361847122319"></a>128</p>
</td>
</tr>
<tr id="row298510992316"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8236184712230"><a name="p8236184712230"></a><a name="p8236184712230"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p9236447122313"><a name="p9236447122313"></a><a name="p9236447122313"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p4237184720234"><a name="p4237184720234"></a><a name="p4237184720234"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1023724732318"><a name="p1023724732318"></a><a name="p1023724732318"></a>128</p>
</td>
</tr>
<tr id="row1171521062317"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p72371847172317"><a name="p72371847172317"></a><a name="p72371847172317"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p223774782319"><a name="p223774782319"></a><a name="p223774782319"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p823774762314"><a name="p823774762314"></a><a name="p823774762314"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p7237247162312"><a name="p7237247162312"></a><a name="p7237247162312"></a>64</p>
</td>
</tr>
<tr id="row14469411192312"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p8237144752311"><a name="p8237144752311"></a><a name="p8237144752311"></a>SubReluCast</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p1723724719236"><a name="p1723724719236"></a><a name="p1723724719236"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p182371847132318"><a name="p182371847132318"></a><a name="p182371847132318"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p18237647122314"><a name="p18237647122314"></a><a name="p18237647122314"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1123717478235"><a name="p1123717478235"></a><a name="p1123717478235"></a>64</p>
</td>
</tr>
<tr id="row267213124233"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p142386472236"><a name="p142386472236"></a><a name="p142386472236"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p7238154752317"><a name="p7238154752317"></a><a name="p7238154752317"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p123815471237"><a name="p123815471237"></a><a name="p123815471237"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p142380472234"><a name="p142380472234"></a><a name="p142380472234"></a>128</p>
</td>
</tr>
<tr id="row1737341310235"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p15238247162319"><a name="p15238247162319"></a><a name="p15238247162319"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1923816474236"><a name="p1923816474236"></a><a name="p1923816474236"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7238047122315"><a name="p7238047122315"></a><a name="p7238047122315"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p723874742317"><a name="p723874742317"></a><a name="p723874742317"></a>128</p>
</td>
</tr>
<tr id="row753718144237"><td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p192389475233"><a name="p192389475233"></a><a name="p192389475233"></a>FusedMulAdd</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p623964715235"><a name="p623964715235"></a><a name="p623964715235"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p0239114792318"><a name="p0239114792318"></a><a name="p0239114792318"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p202391147112316"><a name="p202391147112316"></a><a name="p202391147112316"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p02391047182311"><a name="p02391047182311"></a><a name="p02391047182311"></a>128</p>
</td>
</tr>
<tr id="row628310153237"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p423924715231"><a name="p423924715231"></a><a name="p423924715231"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1023974711232"><a name="p1023974711232"></a><a name="p1023974711232"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p9239847142318"><a name="p9239847142318"></a><a name="p9239847142318"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p9239154732320"><a name="p9239154732320"></a><a name="p9239154732320"></a>64</p>
</td>
</tr>
<tr id="row17601161632320"><td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p172391447172312"><a name="p172391447172312"></a><a name="p172391447172312"></a>MulAddRelu</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p02391247202316"><a name="p02391247202316"></a><a name="p02391247202316"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p42391547182319"><a name="p42391547182319"></a><a name="p42391547182319"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p14240647202315"><a name="p14240647202315"></a><a name="p14240647202315"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p13240104782313"><a name="p13240104782313"></a><a name="p13240104782313"></a>128</p>
</td>
</tr>
<tr id="row43624172239"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p8240154742312"><a name="p8240154742312"></a><a name="p8240154742312"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p824024732316"><a name="p824024732316"></a><a name="p824024732316"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1524044782313"><a name="p1524044782313"></a><a name="p1524044782313"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p6240047172314"><a name="p6240047172314"></a><a name="p6240047172314"></a>64</p>
</td>
</tr>
<tr id="row1529713187231"><td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p20240047132319"><a name="p20240047132319"></a><a name="p20240047132319"></a>CastDequant</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p32403471231"><a name="p32403471231"></a><a name="p32403471231"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p22401447172311"><a name="p22401447172311"></a><a name="p22401447172311"></a>NA</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p10240104722312"><a name="p10240104722312"></a><a name="p10240104722312"></a>int8_t/uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p1724115474233"><a name="p1724115474233"></a><a name="p1724115474233"></a>128</p>
</td>
</tr>
<tr id="row18652195238"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1724184792315"><a name="p1724184792315"></a><a name="p1724184792315"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7241347102317"><a name="p7241347102317"></a><a name="p7241347102317"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p16241184772312"><a name="p16241184772312"></a><a name="p16241184772312"></a>64</p>
</td>
</tr>
<tr id="row188345192233"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p1124154722319"><a name="p1124154722319"></a><a name="p1124154722319"></a>AddDeqRelu</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 ">&nbsp;&nbsp;</td>
</tr>
</tbody>
</table>

**表 4**  比较与选择类指令理论性能汇总

<a name="table205131952256"></a>
<table><thead align="left"><tr id="row251425162517"><th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.1"><p id="p2433899260"><a name="p2433899260"></a><a name="p2433899260"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.2"><p id="p343314919263"><a name="p343314919263"></a><a name="p343314919263"></a>src0数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.3"><p id="p164338912620"><a name="p164338912620"></a><a name="p164338912620"></a>scalar/src1数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.4"><p id="p1143317918263"><a name="p1143317918263"></a><a name="p1143317918263"></a>dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20%" id="mcps1.2.6.1.5"><p id="p043315917260"><a name="p043315917260"></a><a name="p043315917260"></a>硬件并行度（单位element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row25149516253"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p7790202512266"><a name="p7790202512266"></a><a name="p7790202512266"></a>Compare</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p779012255262"><a name="p779012255262"></a><a name="p779012255262"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p1679012253260"><a name="p1679012253260"></a><a name="p1679012253260"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p979014257263"><a name="p979014257263"></a><a name="p979014257263"></a>uint8_t、int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p20563164385618"><a name="p20563164385618"></a><a name="p20563164385618"></a>128</p>
</td>
</tr>
<tr id="row812833818552"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1586818481564"><a name="p1586818481564"></a><a name="p1586818481564"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p10613183765617"><a name="p10613183765617"></a><a name="p10613183765617"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p683953274315"><a name="p683953274315"></a><a name="p683953274315"></a>uint8_t、int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p17563124325616"><a name="p17563124325616"></a><a name="p17563124325616"></a>64</p>
</td>
</tr>
<tr id="row1320019223564"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p3868144885611"><a name="p3868144885611"></a><a name="p3868144885611"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1679011383542"><a name="p1679011383542"></a><a name="p1679011383542"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1983913219438"><a name="p1983913219438"></a><a name="p1983913219438"></a>uint8_t、int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p879073865412"><a name="p879073865412"></a><a name="p879073865412"></a>64</p>
</td>
</tr>
<tr id="row1251411552515"><td class="cellrowborder" rowspan="2" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p479062519261"><a name="p479062519261"></a><a name="p479062519261"></a>Compare（结果存入寄存器）</p>
<p id="p14475112645715"><a name="p14475112645715"></a><a name="p14475112645715"></a></p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p182141736105717"><a name="p182141736105717"></a><a name="p182141736105717"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p33171633115715"><a name="p33171633115715"></a><a name="p33171633115715"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p16411737133418"><a name="p16411737133418"></a><a name="p16411737133418"></a>uint8_t、int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p18317533165713"><a name="p18317533165713"></a><a name="p18317533165713"></a>128</p>
</td>
</tr>
<tr id="row20475172635719"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p12214153614576"><a name="p12214153614576"></a><a name="p12214153614576"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p4317153395718"><a name="p4317153395718"></a><a name="p4317153395718"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1797664110506"><a name="p1797664110506"></a><a name="p1797664110506"></a>uint8_t、int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1431733325712"><a name="p1431733325712"></a><a name="p1431733325712"></a>64</p>
</td>
</tr>
<tr id="row1558391972610"><td class="cellrowborder" rowspan="3" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p19791112518263"><a name="p19791112518263"></a><a name="p19791112518263"></a>Compares</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p979112251262"><a name="p979112251262"></a><a name="p979112251262"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p3791172592613"><a name="p3791172592613"></a><a name="p3791172592613"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p15791625132615"><a name="p15791625132615"></a><a name="p15791625132615"></a>uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p137911525152616"><a name="p137911525152616"></a><a name="p137911525152616"></a>128</p>
</td>
</tr>
<tr id="row336820208261"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p17919259265"><a name="p17919259265"></a><a name="p17919259265"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1792152552618"><a name="p1792152552618"></a><a name="p1792152552618"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p379218251265"><a name="p379218251265"></a><a name="p379218251265"></a>uint8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p279282510267"><a name="p279282510267"></a><a name="p279282510267"></a>64</p>
</td>
</tr>
<tr id="row6541921162616"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p379262512613"><a name="p379262512613"></a><a name="p379262512613"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p179232510268"><a name="p179232510268"></a><a name="p179232510268"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p147921825102611"><a name="p147921825102611"></a><a name="p147921825102611"></a>uint8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p187922025142613"><a name="p187922025142613"></a><a name="p187922025142613"></a>64</p>
</td>
</tr>
<tr id="row263112315263"><td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.1 "><p id="p0795125102619"><a name="p0795125102619"></a><a name="p0795125102619"></a>GetCmpMask(ISASI)</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.2 "><p id="p87955252262"><a name="p87955252262"></a><a name="p87955252262"></a>/</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.3 "><p id="p77951425182611"><a name="p77951425182611"></a><a name="p77951425182611"></a>/</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.4 "><p id="p279592520261"><a name="p279592520261"></a><a name="p279592520261"></a>/</p>
</td>
<td class="cellrowborder" valign="top" width="20%" headers="mcps1.2.6.1.5 "><p id="p77958256263"><a name="p77958256263"></a><a name="p77958256263"></a>1</p>
</td>
</tr>
</tbody>
</table>

**表 5**  类型转换类指令理论性能汇总

<a name="table1712055602918"></a>
<table><thead align="left"><tr id="row31212560291"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p191691001308"><a name="p191691001308"></a><a name="p191691001308"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p131697017301"><a name="p131697017301"></a><a name="p131697017301"></a>src数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p11692015302"><a name="p11692015302"></a><a name="p11692015302"></a>dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p1783419893017"><a name="p1783419893017"></a><a name="p1783419893017"></a>理论并行度（element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row812155682916"><td class="cellrowborder" rowspan="14" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1143145518304"><a name="p1143145518304"></a><a name="p1143145518304"></a>Cast</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p314395517309"><a name="p314395517309"></a><a name="p314395517309"></a>int4b_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p2796844113012"><a name="p2796844113012"></a><a name="p2796844113012"></a>half</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1796344133013"><a name="p1796344133013"></a><a name="p1796344133013"></a>128</p>
</td>
</tr>
<tr id="row141211562299"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13143115511305"><a name="p13143115511305"></a><a name="p13143115511305"></a>uint8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p17796174453013"><a name="p17796174453013"></a><a name="p17796174453013"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p157961244203014"><a name="p157961244203014"></a><a name="p157961244203014"></a>128</p>
</td>
</tr>
<tr id="row1152142193014"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p11434554304"><a name="p11434554304"></a><a name="p11434554304"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p779654420305"><a name="p779654420305"></a><a name="p779654420305"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p27966445300"><a name="p27966445300"></a><a name="p27966445300"></a>128</p>
</td>
</tr>
<tr id="row696042118306"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.5.1.1 "><p id="p3144185519306"><a name="p3144185519306"></a><a name="p3144185519306"></a>int16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2796844103012"><a name="p2796844103012"></a><a name="p2796844103012"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p47967442304"><a name="p47967442304"></a><a name="p47967442304"></a>128</p>
</td>
</tr>
<tr id="row1658432213309"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p13796144433017"><a name="p13796144433017"></a><a name="p13796144433017"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p279694453015"><a name="p279694453015"></a><a name="p279694453015"></a>64</p>
</td>
</tr>
<tr id="row521820238309"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.5.1.1 "><p id="p714495517309"><a name="p714495517309"></a><a name="p714495517309"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18797644133017"><a name="p18797644133017"></a><a name="p18797644133017"></a>int4b_t/int8_t/uint8_t/int16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16797134417306"><a name="p16797134417306"></a><a name="p16797134417306"></a>128</p>
</td>
</tr>
<tr id="row288011233309"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p17797344153011"><a name="p17797344153011"></a><a name="p17797344153011"></a>int32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15797644153013"><a name="p15797644153013"></a><a name="p15797644153013"></a>64</p>
</td>
</tr>
<tr id="row10516324123012"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4144175553012"><a name="p4144175553012"></a><a name="p4144175553012"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p177977441307"><a name="p177977441307"></a><a name="p177977441307"></a>int32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1379714443019"><a name="p1379714443019"></a><a name="p1379714443019"></a>64</p>
</td>
</tr>
<tr id="row44471225103019"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.5.1.1 "><p id="p21441557303"><a name="p21441557303"></a><a name="p21441557303"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p67971644203010"><a name="p67971644203010"></a><a name="p67971644203010"></a>int16_t/half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p13797344193018"><a name="p13797344193018"></a><a name="p13797344193018"></a>64</p>
</td>
</tr>
<tr id="row151265266301"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p57971144113015"><a name="p57971144113015"></a><a name="p57971144113015"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1279744414305"><a name="p1279744414305"></a><a name="p1279744414305"></a>64</p>
</td>
</tr>
<tr id="row148844260304"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p979714445309"><a name="p979714445309"></a><a name="p979714445309"></a>int64_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p11797134420309"><a name="p11797134420309"></a><a name="p11797134420309"></a>32</p>
</td>
</tr>
<tr id="row1966762719303"><td class="cellrowborder" rowspan="3" valign="top" headers="mcps1.2.5.1.1 "><p id="p41441655123017"><a name="p41441655123017"></a><a name="p41441655123017"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p157971544123012"><a name="p157971544123012"></a><a name="p157971544123012"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p079813449301"><a name="p079813449301"></a><a name="p079813449301"></a>64</p>
</td>
</tr>
<tr id="row641682815304"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16798184417301"><a name="p16798184417301"></a><a name="p16798184417301"></a>int16_t/bfloat16_t/int32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7798844103015"><a name="p7798844103015"></a><a name="p7798844103015"></a>64</p>
</td>
</tr>
<tr id="row13108102903015"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p117987445308"><a name="p117987445308"></a><a name="p117987445308"></a>int64_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1779814411303"><a name="p1779814411303"></a><a name="p1779814411303"></a>32</p>
</td>
</tr>
</tbody>
</table>

**表 6**  归约计算类指令理论性能汇总

<a name="table76761043412"></a>
<table><thead align="left"><tr id="row967310113416"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p18418154481315"><a name="p18418154481315"></a><a name="p18418154481315"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p20463657101314"><a name="p20463657101314"></a><a name="p20463657101314"></a>src数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p1830166144"><a name="p1830166144"></a><a name="p1830166144"></a>dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p995717415159"><a name="p995717415159"></a><a name="p995717415159"></a>理论并行度（element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row393610386143"><td class="cellrowborder" rowspan="2" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p7239748121410"><a name="p7239748121410"></a><a name="p7239748121410"></a>ReduceDataBlock</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1540451411511"><a name="p1540451411511"></a><a name="p1540451411511"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p64040144157"><a name="p64040144157"></a><a name="p64040144157"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1696512568144"><a name="p1696512568144"></a><a name="p1696512568144"></a>128/7</p>
</td>
</tr>
<tr id="row496493616145"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p440441417157"><a name="p440441417157"></a><a name="p440441417157"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p6404111415154"><a name="p6404111415154"></a><a name="p6404111415154"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p149650569141"><a name="p149650569141"></a><a name="p149650569141"></a>64</p>
</td>
</tr>
<tr id="row1467191083419"><td class="cellrowborder" rowspan="2" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p818192615140"><a name="p818192615140"></a><a name="p818192615140"></a>ReduceRepeat</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p7121193461317"><a name="p7121193461317"></a><a name="p7121193461317"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1883012621418"><a name="p1883012621418"></a><a name="p1883012621418"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p17189142135215"><a name="p17189142135215"></a><a name="p17189142135215"></a>128/7</p>
</td>
</tr>
<tr id="row76771019349"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2189824528"><a name="p2189824528"></a><a name="p2189824528"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1898011901410"><a name="p1898011901410"></a><a name="p1898011901410"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1818917217526"><a name="p1818917217526"></a><a name="p1818917217526"></a>64/7</p>
</td>
</tr>
<tr id="row73251141201415"><td class="cellrowborder" rowspan="2" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1432511411148"><a name="p1432511411148"></a><a name="p1432511411148"></a>ReducePairElem</p>
<p id="p495219429144"><a name="p495219429144"></a><a name="p495219429144"></a></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p5784161531513"><a name="p5784161531513"></a><a name="p5784161531513"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p678410154158"><a name="p678410154158"></a><a name="p678410154158"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p206089172164"><a name="p206089172164"></a><a name="p206089172164"></a>128</p>
</td>
</tr>
<tr id="row17952142121415"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1784515171513"><a name="p1784515171513"></a><a name="p1784515171513"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15784215151511"><a name="p15784215151511"></a><a name="p15784215151511"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p26081017171618"><a name="p26081017171618"></a><a name="p26081017171618"></a>64</p>
</td>
</tr>
<tr id="row6733142411614"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1873314240164"><a name="p1873314240164"></a><a name="p1873314240164"></a>ReduceMax/ReduceMin/ReduceSum</p>
</td>
<td class="cellrowborder" colspan="3" valign="top" headers="mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="p1049444718171"><a name="p1049444718171"></a><a name="p1049444718171"></a>软仿指令，不提供理论并行度</p>
</td>
</tr>
</tbody>
</table>

**表 7**  数据排布转换类指令理论性能汇总

<a name="table2514311153610"></a>
<table><thead align="left"><tr id="row17514101133611"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p919983764619"><a name="p919983764619"></a><a name="p919983764619"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p21991237194613"><a name="p21991237194613"></a><a name="p21991237194613"></a>src数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p191992370468"><a name="p191992370468"></a><a name="p191992370468"></a>dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p19199203794610"><a name="p19199203794610"></a><a name="p19199203794610"></a>理论并行度（element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row7385951185110"><td class="cellrowborder" rowspan="3" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p17385155125112"><a name="p17385155125112"></a><a name="p17385155125112"></a></p>
<p id="p05151411153613"><a name="p05151411153613"></a><a name="p05151411153613"></a>Transpose</p>
<p id="p1781529184719"><a name="p1781529184719"></a><a name="p1781529184719"></a></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p33851651105110"><a name="p33851651105110"></a><a name="p33851651105110"></a>int8_t/uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p23851951155110"><a name="p23851951155110"></a><a name="p23851951155110"></a>int8_t/uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p33851551175114"><a name="p33851551175114"></a><a name="p33851551175114"></a>256</p>
</td>
</tr>
<tr id="row19515011113615"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1918962165219"><a name="p1918962165219"></a><a name="p1918962165219"></a>int16_t/uint16_t/half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p197621547155113"><a name="p197621547155113"></a><a name="p197621547155113"></a>int16_t/uint16_t/half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p135156119366"><a name="p135156119366"></a><a name="p135156119366"></a>128</p>
</td>
</tr>
<tr id="row47811329184720"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p778172920473"><a name="p778172920473"></a><a name="p778172920473"></a>int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15781162994711"><a name="p15781162994711"></a><a name="p15781162994711"></a>int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1778117293473"><a name="p1778117293473"></a><a name="p1778117293473"></a>64</p>
</td>
</tr>
<tr id="row1551521110363"><td class="cellrowborder" rowspan="3" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1751571118362"><a name="p1751571118362"></a><a name="p1751571118362"></a>TransDataTo5HD</p>
<p id="p11521220105417"><a name="p11521220105417"></a><a name="p11521220105417"></a></p>
<p id="p28374177548"><a name="p28374177548"></a><a name="p28374177548"></a></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p7165192713548"><a name="p7165192713548"></a><a name="p7165192713548"></a>int8_t/uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1216582735414"><a name="p1216582735414"></a><a name="p1216582735414"></a>int8_t/uint8_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p21661627195413"><a name="p21661627195413"></a><a name="p21661627195413"></a>256</p>
</td>
</tr>
<tr id="row8521320185416"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4166102715542"><a name="p4166102715542"></a><a name="p4166102715542"></a>int16_t/uint16_t/half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p416622713542"><a name="p416622713542"></a><a name="p416622713542"></a>int16_t/uint16_t/half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p14166152711545"><a name="p14166152711545"></a><a name="p14166152711545"></a>128</p>
</td>
</tr>
<tr id="row18836417145419"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p6166182716545"><a name="p6166182716545"></a><a name="p6166182716545"></a>int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4166192712541"><a name="p4166192712541"></a><a name="p4166192712541"></a>int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p216622715549"><a name="p216622715549"></a><a name="p216622715549"></a>64</p>
</td>
</tr>
</tbody>
</table>

**表 8**  数据填充类指令理论性能汇总

<a name="table18675133833617"></a>
<table><thead align="left"><tr id="row4675338173612"><th class="cellrowborder" valign="top" width="23.12%" id="mcps1.2.6.1.1"><p id="p225173910546"><a name="p225173910546"></a><a name="p225173910546"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="18.130000000000003%" id="mcps1.2.6.1.2"><p id="p2251139135411"><a name="p2251139135411"></a><a name="p2251139135411"></a>src数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="17.07%" id="mcps1.2.6.1.3"><p id="p13254394548"><a name="p13254394548"></a><a name="p13254394548"></a>dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="18.560000000000002%" id="mcps1.2.6.1.4"><p id="p49379135717"><a name="p49379135717"></a><a name="p49379135717"></a>理论输入并行度（element / cycle）</p>
</th>
<th class="cellrowborder" valign="top" width="23.12%" id="mcps1.2.6.1.5"><p id="p5251239195415"><a name="p5251239195415"></a><a name="p5251239195415"></a>理论输出并行度（element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row667533818366"><td class="cellrowborder" rowspan="2" valign="top" width="23.12%" headers="mcps1.2.6.1.1 "><p id="p10182748125314"><a name="p10182748125314"></a><a name="p10182748125314"></a>Brcb</p>
<p id="p1727883318571"><a name="p1727883318571"></a><a name="p1727883318571"></a></p>
</td>
<td class="cellrowborder" valign="top" width="18.130000000000003%" headers="mcps1.2.6.1.2 "><p id="p12675173833614"><a name="p12675173833614"></a><a name="p12675173833614"></a>int16_t/uint16_t/half/bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="17.07%" headers="mcps1.2.6.1.3 "><p id="p15675173812361"><a name="p15675173812361"></a><a name="p15675173812361"></a>int16_t/uint16_t/half/bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="18.560000000000002%" headers="mcps1.2.6.1.4 "><p id="p1323575510567"><a name="p1323575510567"></a><a name="p1323575510567"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="23.12%" headers="mcps1.2.6.1.5 "><p id="p1675138113614"><a name="p1675138113614"></a><a name="p1675138113614"></a>128</p>
</td>
</tr>
<tr id="row192781433105719"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1427918330570"><a name="p1427918330570"></a><a name="p1427918330570"></a>int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p627953312571"><a name="p627953312571"></a><a name="p627953312571"></a>int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p12279123310575"><a name="p12279123310575"></a><a name="p12279123310575"></a>8</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p192791833175715"><a name="p192791833175715"></a><a name="p192791833175715"></a>64</p>
</td>
</tr>
<tr id="row767517383367"><td class="cellrowborder" rowspan="2" valign="top" width="23.12%" headers="mcps1.2.6.1.1 "><p id="p1300145118532"><a name="p1300145118532"></a><a name="p1300145118532"></a>Duplicate</p>
<p id="p20638173917584"><a name="p20638173917584"></a><a name="p20638173917584"></a></p>
</td>
<td class="cellrowborder" valign="top" width="18.130000000000003%" headers="mcps1.2.6.1.2 "><p id="p82692468587"><a name="p82692468587"></a><a name="p82692468587"></a>int16_t/uint16_t/half/bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="17.07%" headers="mcps1.2.6.1.3 "><p id="p926934685819"><a name="p926934685819"></a><a name="p926934685819"></a>int16_t/uint16_t/half/bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" width="18.560000000000002%" headers="mcps1.2.6.1.4 "><p id="p17235165513567"><a name="p17235165513567"></a><a name="p17235165513567"></a>-</p>
</td>
<td class="cellrowborder" valign="top" width="23.12%" headers="mcps1.2.6.1.5 "><p id="p20676938143610"><a name="p20676938143610"></a><a name="p20676938143610"></a>128</p>
</td>
</tr>
<tr id="row6638139135816"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p62690467581"><a name="p62690467581"></a><a name="p62690467581"></a>int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p226914625819"><a name="p226914625819"></a><a name="p226914625819"></a>int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p0638439165813"><a name="p0638439165813"></a><a name="p0638439165813"></a>-</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p1463813396580"><a name="p1463813396580"></a><a name="p1463813396580"></a>64</p>
</td>
</tr>
<tr id="row1425164215314"><td class="cellrowborder" rowspan="2" valign="top" width="23.12%" headers="mcps1.2.6.1.1 "><p id="p105059544537"><a name="p105059544537"></a><a name="p105059544537"></a>CreateVecIndex</p>
<p id="p141991836195910"><a name="p141991836195910"></a><a name="p141991836195910"></a></p>
</td>
<td class="cellrowborder" valign="top" width="18.130000000000003%" headers="mcps1.2.6.1.2 "><p id="p1942594285310"><a name="p1942594285310"></a><a name="p1942594285310"></a>int16_t/half</p>
</td>
<td class="cellrowborder" valign="top" width="17.07%" headers="mcps1.2.6.1.3 "><p id="p090061316018"><a name="p090061316018"></a><a name="p090061316018"></a>int16_t/half</p>
</td>
<td class="cellrowborder" valign="top" width="18.560000000000002%" headers="mcps1.2.6.1.4 "><p id="p172351055145616"><a name="p172351055145616"></a><a name="p172351055145616"></a>-</p>
</td>
<td class="cellrowborder" valign="top" width="23.12%" headers="mcps1.2.6.1.5 "><p id="p74256424538"><a name="p74256424538"></a><a name="p74256424538"></a>128</p>
</td>
</tr>
<tr id="row8198836125919"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p619993635913"><a name="p619993635913"></a><a name="p619993635913"></a>int32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p33971128303"><a name="p33971128303"></a><a name="p33971128303"></a>int32_t/float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20199113617599"><a name="p20199113617599"></a><a name="p20199113617599"></a>-</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p101992036125915"><a name="p101992036125915"></a><a name="p101992036125915"></a>64</p>
</td>
</tr>
</tbody>
</table>

**表 9**  排序组合类指令理论性能汇总

<a name="table1361419514367"></a>
<table><thead align="left"><tr id="row20614115133616"><th class="cellrowborder" valign="top" width="16.5%" id="mcps1.2.6.1.1"><p id="p0614451173612"><a name="p0614451173612"></a><a name="p0614451173612"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="20.979999999999997%" id="mcps1.2.6.1.2"><p id="p1161518511364"><a name="p1161518511364"></a><a name="p1161518511364"></a>src0数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="20.169999999999998%" id="mcps1.2.6.1.3"><p id="p46151651143612"><a name="p46151651143612"></a><a name="p46151651143612"></a>src1数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="16.6%" id="mcps1.2.6.1.4"><p id="p9615851143614"><a name="p9615851143614"></a><a name="p9615851143614"></a>dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25.75%" id="mcps1.2.6.1.5"><p id="p15881656121914"><a name="p15881656121914"></a><a name="p15881656121914"></a>理论并行度（element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row4615751153614"><td class="cellrowborder" rowspan="2" valign="top" width="16.5%" headers="mcps1.2.6.1.1 "><p id="p6678145815311"><a name="p6678145815311"></a><a name="p6678145815311"></a>Sort32</p>
<p id="p56694722018"><a name="p56694722018"></a><a name="p56694722018"></a></p>
</td>
<td class="cellrowborder" valign="top" width="20.979999999999997%" headers="mcps1.2.6.1.2 "><p id="p206151351143615"><a name="p206151351143615"></a><a name="p206151351143615"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="20.169999999999998%" headers="mcps1.2.6.1.3 "><p id="p26150512365"><a name="p26150512365"></a><a name="p26150512365"></a>uint32_t</p>
</td>
<td class="cellrowborder" valign="top" width="16.6%" headers="mcps1.2.6.1.4 "><p id="p116154518364"><a name="p116154518364"></a><a name="p116154518364"></a>half（fp16）</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.2.6.1.5 "><p id="p15588105611918"><a name="p15588105611918"></a><a name="p15588105611918"></a>64</p>
</td>
</tr>
<tr id="row1266104710200"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p2673474209"><a name="p2673474209"></a><a name="p2673474209"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p093631612210"><a name="p093631612210"></a><a name="p093631612210"></a>uint32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p7443992119"><a name="p7443992119"></a><a name="p7443992119"></a>float（fp32）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p186711479204"><a name="p186711479204"></a><a name="p186711479204"></a>32</p>
</td>
</tr>
<tr id="row136154518360"><td class="cellrowborder" valign="top" width="16.5%" headers="mcps1.2.6.1.1 "><p id="p168422012541"><a name="p168422012541"></a><a name="p168422012541"></a>MrgSort（待补充）</p>
</td>
<td class="cellrowborder" valign="top" width="20.979999999999997%" headers="mcps1.2.6.1.2 "><p id="p1961555123614"><a name="p1961555123614"></a><a name="p1961555123614"></a>-</p>
</td>
<td class="cellrowborder" valign="top" width="20.169999999999998%" headers="mcps1.2.6.1.3 "><p id="p11615145111361"><a name="p11615145111361"></a><a name="p11615145111361"></a>-</p>
</td>
<td class="cellrowborder" valign="top" width="16.6%" headers="mcps1.2.6.1.4 "><p id="p116151751163610"><a name="p116151751163610"></a><a name="p116151751163610"></a>-</p>
</td>
<td class="cellrowborder" valign="top" width="25.75%" headers="mcps1.2.6.1.5 "><p id="p1258835617195"><a name="p1258835617195"></a><a name="p1258835617195"></a>-</p>
</td>
</tr>
</tbody>
</table>

**表 10**  离散与聚合类指令理论性能汇总

<a name="table111588914379"></a>
<table><thead align="left"><tr id="row201581495376"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p18841732815"><a name="p18841732815"></a><a name="p18841732815"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p1788493213119"><a name="p1788493213119"></a><a name="p1788493213119"></a>src数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p788417321813"><a name="p788417321813"></a><a name="p788417321813"></a>dst数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p788403218112"><a name="p788403218112"></a><a name="p788403218112"></a>理论并行度（element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row201585912374"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p10158129113711"><a name="p10158129113711"></a><a name="p10158129113711"></a>Gather</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1715816943712"><a name="p1715816943712"></a><a name="p1715816943712"></a>int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p10277112414218"><a name="p10277112414218"></a><a name="p10277112414218"></a>int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p61584910378"><a name="p61584910378"></a><a name="p61584910378"></a>5</p>
</td>
</tr>
<tr id="row3158189113712"><td class="cellrowborder" rowspan="2" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p11583915375"><a name="p11583915375"></a><a name="p11583915375"></a>Gatherb</p>
<p id="p104141059129"><a name="p104141059129"></a><a name="p104141059129"></a></p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1315829103714"><a name="p1315829103714"></a><a name="p1315829103714"></a>uint16_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1694612105310"><a name="p1694612105310"></a><a name="p1694612105310"></a>uint16_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1115816963712"><a name="p1115816963712"></a><a name="p1115816963712"></a>42</p>
</td>
</tr>
<tr id="row144142591426"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p164144591928"><a name="p164144591928"></a><a name="p164144591928"></a>uint32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p894610100311"><a name="p894610100311"></a><a name="p894610100311"></a>uint32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p10414759327"><a name="p10414759327"></a><a name="p10414759327"></a>21</p>
</td>
</tr>
</tbody>
</table>

**表 11**  掩码操作类指令理论性能汇总

<a name="table12905102516375"></a>
<table><thead align="left"><tr id="row18905192503714"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p159058256371"><a name="p159058256371"></a><a name="p159058256371"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p1905142523714"><a name="p1905142523714"></a><a name="p1905142523714"></a>输入数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p1390502510375"><a name="p1390502510375"></a><a name="p1390502510375"></a>理论并行度（element / cycle）</p>
</th>
</tr>
</thead>
<tbody><tr id="row13905182533716"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p790542518377"><a name="p790542518377"></a><a name="p790542518377"></a>SetMaskCount</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p10905142513716"><a name="p10905142513716"></a><a name="p10905142513716"></a>/</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p4906825173717"><a name="p4906825173717"></a><a name="p4906825173717"></a>1</p>
</td>
</tr>
<tr id="row99061025163718"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p79061825103712"><a name="p79061825103712"></a><a name="p79061825103712"></a>SetMaskNorm</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p4906142543712"><a name="p4906142543712"></a><a name="p4906142543712"></a>/</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1990612512372"><a name="p1990612512372"></a><a name="p1990612512372"></a>1</p>
</td>
</tr>
<tr id="row144201347124916"><td class="cellrowborder" rowspan="2" valign="middle" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p7420154754911"><a name="p7420154754911"></a><a name="p7420154754911"></a>SetVectorMask</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p14420194710499"><a name="p14420194710499"></a><a name="p14420194710499"></a>uint64_t</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p04207472499"><a name="p04207472499"></a><a name="p04207472499"></a>1</p>
</td>
</tr>
<tr id="row13159748124914"><td class="cellrowborder" valign="middle" headers="mcps1.2.4.1.1 "><p id="p1015974884911"><a name="p1015974884911"></a><a name="p1015974884911"></a>int32_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1159204811496"><a name="p1159204811496"></a><a name="p1159204811496"></a>1</p>
</td>
</tr>
<tr id="row193491110165311"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p834910100538"><a name="p834910100538"></a><a name="p834910100538"></a>ResetMask</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p034912101531"><a name="p034912101531"></a><a name="p034912101531"></a>/</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p134941012533"><a name="p134941012533"></a><a name="p134941012533"></a>1</p>
</td>
</tr>
</tbody>
</table>

**表 12**  标量计算类指令理论性能汇总

<a name="table12906133883716"></a>
<table><thead align="left"><tr id="row14906238133718"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p7296914711"><a name="p7296914711"></a><a name="p7296914711"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p1523505613184"><a name="p1523505613184"></a><a name="p1523505613184"></a>输入数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p1323513567184"><a name="p1323513567184"></a><a name="p1323513567184"></a>输出数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p5235165661816"><a name="p5235165661816"></a><a name="p5235165661816"></a>理论并行度（cycle / instruction）</p>
</th>
</tr>
</thead>
<tbody><tr id="row89071238103720"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p152961214411"><a name="p152961214411"></a><a name="p152961214411"></a>GetBitCount</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p490743816370"><a name="p490743816370"></a><a name="p490743816370"></a>uint64_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p20907438133717"><a name="p20907438133717"></a><a name="p20907438133717"></a>int64_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1190753818378"><a name="p1190753818378"></a><a name="p1190753818378"></a>1</p>
</td>
</tr>
<tr id="row0907838133718"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1929631418113"><a name="p1929631418113"></a><a name="p1929631418113"></a>CountLeadingZero</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1549811122311"><a name="p1549811122311"></a><a name="p1549811122311"></a>uint64_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p17498161182312"><a name="p17498161182312"></a><a name="p17498161182312"></a>int64_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p59076388376"><a name="p59076388376"></a><a name="p59076388376"></a>1</p>
</td>
</tr>
<tr id="row178351671519"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p32966141211"><a name="p32966141211"></a><a name="p32966141211"></a>GetSFFValue</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p3577113219249"><a name="p3577113219249"></a><a name="p3577113219249"></a>uint64_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p19577123272416"><a name="p19577123272416"></a><a name="p19577123272416"></a>int64_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p2836877118"><a name="p2836877118"></a><a name="p2836877118"></a>1</p>
</td>
</tr>
<tr id="row16569581710"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p629616143117"><a name="p629616143117"></a><a name="p629616143117"></a>CountBitsCntSameAsSignBit</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1438816400240"><a name="p1438816400240"></a><a name="p1438816400240"></a>int64_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p3388440192410"><a name="p3388440192410"></a><a name="p3388440192410"></a>int64_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p65694814118"><a name="p65694814118"></a><a name="p65694814118"></a>1</p>
</td>
</tr>
<tr id="row13271591617"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p20297314719"><a name="p20297314719"></a><a name="p20297314719"></a>Cast（float转half、int32_t）</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1271291512"><a name="p1271291512"></a><a name="p1271291512"></a>float</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p122711193113"><a name="p122711193113"></a><a name="p122711193113"></a>half / int32_t</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p6271098111"><a name="p6271098111"></a><a name="p6271098111"></a>1</p>
</td>
</tr>
<tr id="row1291320919116"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p182971814817"><a name="p182971814817"></a><a name="p182971814817"></a>Cast（float转bfloat16_t）</p>
</td>
<td class="cellrowborder" colspan="3" valign="top" headers="mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="p287115292715"><a name="p287115292715"></a><a name="p287115292715"></a>软仿指令，不提供理论并行度</p>
</td>
</tr>
<tr id="row1145512107116"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p42971814517"><a name="p42971814517"></a><a name="p42971814517"></a>Cast（bfloat16_t转float）</p>
</td>
<td class="cellrowborder" colspan="3" valign="top" headers="mcps1.2.5.1.2 mcps1.2.5.1.3 mcps1.2.5.1.4 "><p id="p146021552202714"><a name="p146021552202714"></a><a name="p146021552202714"></a>软仿指令，不提供理论并行度</p>
</td>
</tr>
</tbody>
</table>

**表 13**  系统变量访问类指令理论性能汇总

<a name="table1843362415383"></a>
<table><thead align="left"><tr id="row124349249386"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="p3246648602"><a name="p3246648602"></a><a name="p3246648602"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="p155717583188"><a name="p155717583188"></a><a name="p155717583188"></a>理论并行度（cycle / instruction）</p>
</th>
</tr>
</thead>
<tbody><tr id="row1434112413381"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p179815434010"><a name="p179815434010"></a><a name="p179815434010"></a>GetBlockNum</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p19434162415389"><a name="p19434162415389"></a><a name="p19434162415389"></a>1</p>
</td>
</tr>
<tr id="row11434324133814"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1679816431607"><a name="p1679816431607"></a><a name="p1679816431607"></a>GetBlockIdx（AIC）</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p34341624133810"><a name="p34341624133810"></a><a name="p34341624133810"></a>1</p>
</td>
</tr>
<tr id="row167854149310"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p1963201883111"><a name="p1963201883111"></a><a name="p1963201883111"></a>GetBlockIdx（AIV）</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p4785181414313"><a name="p4785181414313"></a><a name="p4785181414313"></a>软仿指令，不提供理论并行度</p>
</td>
</tr>
<tr id="row182797177016"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p3798114310012"><a name="p3798114310012"></a><a name="p3798114310012"></a>GetTaskRatio</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1328061710012"><a name="p1328061710012"></a><a name="p1328061710012"></a>1</p>
</td>
</tr>
<tr id="row2471813017"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p3798154311019"><a name="p3798154311019"></a><a name="p3798154311019"></a>GetSubBlockNum</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1254181007"><a name="p1254181007"></a><a name="p1254181007"></a>1</p>
</td>
</tr>
<tr id="row1356013184016"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p679915431503"><a name="p679915431503"></a><a name="p679915431503"></a>GetSubBlockIdx</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p9560121818014"><a name="p9560121818014"></a><a name="p9560121818014"></a>1</p>
</td>
</tr>
<tr id="row187595381602"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p10799114313011"><a name="p10799114313011"></a><a name="p10799114313011"></a>GetDataBlockSizeInBytes</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p1759183810014"><a name="p1759183810014"></a><a name="p1759183810014"></a>静态编译，无消耗，与直接使用立即数行为一致。</p>
</td>
</tr>
<tr id="row73948393020"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p179919431404"><a name="p179919431404"></a><a name="p179919431404"></a>GetSystemCycle</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p23941439708"><a name="p23941439708"></a><a name="p23941439708"></a>1</p>
</td>
</tr>
<tr id="row20256401003"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p11799164313010"><a name="p11799164313010"></a><a name="p11799164313010"></a>GetProgramCounter</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p139002919335"><a name="p139002919335"></a><a name="p139002919335"></a>软仿指令，不提供理论并行度</p>
</td>
</tr>
<tr id="row105851408016"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p479915431105"><a name="p479915431105"></a><a name="p479915431105"></a>GetArchVersion</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p21081952203315"><a name="p21081952203315"></a><a name="p21081952203315"></a>软仿指令，不提供理论并行度</p>
</td>
</tr>
</tbody>
</table>

**表 14**  工具接口类指令理论性能汇总

<a name="table14541161453912"></a>
<table><thead align="left"><tr id="row105411414123912"><th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.1"><p id="p10541131418393"><a name="p10541131418393"></a><a name="p10541131418393"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="50%" id="mcps1.2.3.1.2"><p id="p643892910351"><a name="p643892910351"></a><a name="p643892910351"></a>理论并行度（cycle / instruction）</p>
</th>
</tr>
</thead>
<tbody><tr id="row125416140399"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p75411214133917"><a name="p75411214133917"></a><a name="p75411214133917"></a>InitSocState</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p16367539173619"><a name="p16367539173619"></a><a name="p16367539173619"></a>软仿指令，不提供理论并行度</p>
</td>
</tr>
<tr id="row115419143394"><td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.1 "><p id="p15541201483920"><a name="p15541201483920"></a><a name="p15541201483920"></a>Async</p>
</td>
<td class="cellrowborder" valign="top" width="50%" headers="mcps1.2.3.1.2 "><p id="p17542814133914"><a name="p17542814133914"></a><a name="p17542814133914"></a>静态编译，无消耗</p>
</td>
</tr>
</tbody>
</table>

缓存控制、同步控制和原子操作指令，不涉及理论性能。

