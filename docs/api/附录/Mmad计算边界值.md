# Mmad计算边界值<a name="ZH-CN_TOPIC_0000002607849175"></a>

本节汇总介绍了Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品  Mmad计算接口在边界值输入下的输出结果：

**表 1**  Mmad特殊值/边界值输入的计算结果说明

<a name="table7822641411"></a>
<table><thead align="left"><tr id="row1991926171413"><th class="cellrowborder" valign="top" width="19.220000000000002%" id="mcps1.2.6.1.1"><p id="p16972651418"><a name="p16972651418"></a><a name="p16972651418"></a>C矩阵的数据类型</p>
</th>
<th class="cellrowborder" valign="top" width="15.52%" id="mcps1.2.6.1.2"><p id="p11367203817575"><a name="p11367203817575"></a><a name="p11367203817575"></a>Mmad子阶段</p>
</th>
<th class="cellrowborder" valign="top" width="19.35%" id="mcps1.2.6.1.3"><p id="p29112617146"><a name="p29112617146"></a><a name="p29112617146"></a>A矩阵的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="22.79%" id="mcps1.2.6.1.4"><p id="p1791526111411"><a name="p1791526111411"></a><a name="p1791526111411"></a>B矩阵的元素取值</p>
</th>
<th class="cellrowborder" valign="top" width="23.119999999999997%" id="mcps1.2.6.1.5"><p id="p4982641416"><a name="p4982641416"></a><a name="p4982641416"></a>dst的元素取值</p>
<p id="p10920268145"><a name="p10920268145"></a><a name="p10920268145"></a>（INF/NAN模式）</p>
</th>
</tr>
</thead>
<tbody><tr id="row4962612147"><td class="cellrowborder" rowspan="19" valign="top" width="19.220000000000002%" headers="mcps1.2.6.1.1 "><p id="p29726191419"><a name="p29726191419"></a><a name="p29726191419"></a>float</p>
<p id="p89192616146"><a name="p89192616146"></a><a name="p89192616146"></a></p>
<p id="p1318532111310"><a name="p1318532111310"></a><a name="p1318532111310"></a></p>
<p id="p41261635161318"><a name="p41261635161318"></a><a name="p41261635161318"></a></p>
</td>
<td class="cellrowborder" rowspan="13" valign="top" width="15.52%" headers="mcps1.2.6.1.2 "><p id="p141743581981"><a name="p141743581981"></a><a name="p141743581981"></a>乘法阶段</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" width="19.35%" headers="mcps1.2.6.1.3 "><p id="p619815166211"><a name="p619815166211"></a><a name="p619815166211"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" width="22.79%" headers="mcps1.2.6.1.4 "><p id="p98581514125812"><a name="p98581514125812"></a><a name="p98581514125812"></a>norm(&gt; 0.0) / +inf</p>
</td>
<td class="cellrowborder" valign="top" width="23.119999999999997%" headers="mcps1.2.6.1.5 "><p id="p9498194501414"><a name="p9498194501414"></a><a name="p9498194501414"></a>+inf</p>
</td>
</tr>
<tr id="row19942631412"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p128573143583"><a name="p128573143583"></a><a name="p128573143583"></a>norm(&lt; 0.0) / -inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p114980459143"><a name="p114980459143"></a><a name="p114980459143"></a>-inf</p>
</td>
</tr>
<tr id="row7902691419"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p143690341934"><a name="p143690341934"></a><a name="p143690341934"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p194983457145"><a name="p194983457145"></a><a name="p194983457145"></a>norm(&gt; 0.0) / +inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p0498184591416"><a name="p0498184591416"></a><a name="p0498184591416"></a>-inf</p>
</td>
</tr>
<tr id="row791626141411"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1749834561410"><a name="p1749834561410"></a><a name="p1749834561410"></a>norm(&lt; 0.0) / -inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p14988451145"><a name="p14988451145"></a><a name="p14988451145"></a>+inf</p>
</td>
</tr>
<tr id="row99142651416"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p147507321708"><a name="p147507321708"></a><a name="p147507321708"></a>+inf/-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p134981452144"><a name="p134981452144"></a><a name="p134981452144"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p8498114511144"><a name="p8498114511144"></a><a name="p8498114511144"></a>nan</p>
</td>
</tr>
<tr id="row99926141411"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p139217471744"><a name="p139217471744"></a><a name="p139217471744"></a>norm(&gt; 0.0)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p44988456147"><a name="p44988456147"></a><a name="p44988456147"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p249816456148"><a name="p249816456148"></a><a name="p249816456148"></a>+inf</p>
</td>
</tr>
<tr id="row310192619141"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p137561156181418"><a name="p137561156181418"></a><a name="p137561156181418"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p975605611142"><a name="p975605611142"></a><a name="p975605611142"></a>-inf</p>
</td>
</tr>
<tr id="row1110182681415"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p189711311953"><a name="p189711311953"></a><a name="p189711311953"></a>norm(&lt; 0.0)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p17568569149"><a name="p17568569149"></a><a name="p17568569149"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p77561656131419"><a name="p77561656131419"></a><a name="p77561656131419"></a>-inf</p>
</td>
</tr>
<tr id="row01042611146"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p19756656151415"><a name="p19756656151415"></a><a name="p19756656151415"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p169091423205817"><a name="p169091423205817"></a><a name="p169091423205817"></a>+inf</p>
</td>
</tr>
<tr id="row1310826161418"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1310926191416"><a name="p1310926191416"></a><a name="p1310926191416"></a>0.0</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p745752681519"><a name="p745752681519"></a><a name="p745752681519"></a>+inf/ -inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1245782619159"><a name="p1245782619159"></a><a name="p1245782619159"></a>nan</p>
</td>
</tr>
<tr id="row61062619144"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p878214384018"><a name="p878214384018"></a><a name="p878214384018"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1245752651511"><a name="p1245752651511"></a><a name="p1245752651511"></a>任意输入</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p20457126201520"><a name="p20457126201520"></a><a name="p20457126201520"></a>nan</p>
</td>
</tr>
<tr id="row10111426141412"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p107823381307"><a name="p107823381307"></a><a name="p107823381307"></a>任意输入</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p15457326151519"><a name="p15457326151519"></a><a name="p15457326151519"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p1745712615151"><a name="p1745712615151"></a><a name="p1745712615151"></a>nan</p>
</td>
</tr>
<tr id="row111112616146"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p41152619147"><a name="p41152619147"></a><a name="p41152619147"></a>任意输入</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p12240538161510"><a name="p12240538161510"></a><a name="p12240538161510"></a>任意输入</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p524083871511"><a name="p524083871511"></a><a name="p524083871511"></a>结果溢出情况下，正向溢出为INF，负向溢出为-INF</p>
</td>
</tr>
<tr id="row1384973416151"><td class="cellrowborder" rowspan="6" valign="top" headers="mcps1.2.6.1.1 "><p id="p20474193814145"><a name="p20474193814145"></a><a name="p20474193814145"></a>加法阶段</p>
</td>
<td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.2 "><p id="p1337632991013"><a name="p1337632991013"></a><a name="p1337632991013"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p11240938101511"><a name="p11240938101511"></a><a name="p11240938101511"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.4 "><p id="p14240338121516"><a name="p14240338121516"></a><a name="p14240338121516"></a>nan</p>
</td>
</tr>
<tr id="row61102681417"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9175949141017"><a name="p9175949141017"></a><a name="p9175949141017"></a>+inf/norm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1224020383155"><a name="p1224020383155"></a><a name="p1224020383155"></a>+inf</p>
</td>
</tr>
<tr id="row811102621411"><td class="cellrowborder" rowspan="2" valign="top" headers="mcps1.2.6.1.1 "><p id="p6867104912113"><a name="p6867104912113"></a><a name="p6867104912113"></a>-inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5523152111518"><a name="p5523152111518"></a><a name="p5523152111518"></a>+inf</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p25231752191516"><a name="p25231752191516"></a><a name="p25231752191516"></a>nan</p>
</td>
</tr>
<tr id="row121119268142"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p9523155231513"><a name="p9523155231513"></a><a name="p9523155231513"></a>-inf/norm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p5523155212150"><a name="p5523155212150"></a><a name="p5523155212150"></a>-inf</p>
</td>
</tr>
<tr id="row161815320130"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p1819732121310"><a name="p1819732121310"></a><a name="p1819732121310"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p1819153218135"><a name="p1819153218135"></a><a name="p1819153218135"></a>任意输入</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p121911322134"><a name="p121911322134"></a><a name="p121911322134"></a>nan</p>
</td>
</tr>
<tr id="row141251435141318"><td class="cellrowborder" valign="top" headers="mcps1.2.6.1.1 "><p id="p151260358131"><a name="p151260358131"></a><a name="p151260358131"></a>任意输入</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.2 "><p id="p312603531319"><a name="p312603531319"></a><a name="p312603531319"></a>nan</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.6.1.3 "><p id="p61265358136"><a name="p61265358136"></a><a name="p61265358136"></a>nan</p>
</td>
</tr>
</tbody>
</table>

注：

（1）INF/NAN模式（非饱和模式）下，乘法阶段：inf\*非零值输出inf，inf\*0输出nan，输入包含nan时输出为nan。加法阶段：正负inf相加时输出为nan。

（2）饱和模式下，+inf按照数据类型取对应max，-inf按照数据类型取对应min值参与计算，nan值按照0参与计算。

