# API流水类型汇总<a name="ZH-CN_TOPIC_0000002533354434"></a>

**表 1**  Memory矢量计算API流水类型汇总

<a name="table132070121817"></a>
<table><thead align="left"><tr id="row16205051813"><th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.1"><p id="p1318745274714"><a name="p1318745274714"></a><a name="p1318745274714"></a>API类型</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.2"><p id="p620130151817"><a name="p620130151817"></a><a name="p620130151817"></a>API接口名词</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="p1125313691813"><a name="p1125313691813"></a><a name="p1125313691813"></a>执行核（AIC/AIV）</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="p152536641819"><a name="p152536641819"></a><a name="p152536641819"></a>流水</p>
</th>
</tr>
</thead>
<tbody><tr id="row565114371362"><td class="cellrowborder" rowspan="17" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p21961829195915"><a name="p21961829195915"></a><a name="p21961829195915"></a>基础算术</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p3651163711363"><a name="p3651163711363"></a><a name="p3651163711363"></a>Exp</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p171781578373"><a name="p171781578373"></a><a name="p171781578373"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p3178185715373"><a name="p3178185715373"></a><a name="p3178185715373"></a>PIPE_V</p>
</td>
</tr>
<tr id="row699314820360"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p29931486362"><a name="p29931486362"></a><a name="p29931486362"></a>Ln</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p109056576377"><a name="p109056576377"></a><a name="p109056576377"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p890595713711"><a name="p890595713711"></a><a name="p890595713711"></a>PIPE_V</p>
</td>
</tr>
<tr id="row843915223617"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p143914527369"><a name="p143914527369"></a><a name="p143914527369"></a>Abs</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1578185812373"><a name="p1578185812373"></a><a name="p1578185812373"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1257835817372"><a name="p1257835817372"></a><a name="p1257835817372"></a>PIPE_V</p>
</td>
</tr>
<tr id="row443995213362"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p18440205253618"><a name="p18440205253618"></a><a name="p18440205253618"></a>Reciprocal</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p699675883819"><a name="p699675883819"></a><a name="p699675883819"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2099665823814"><a name="p2099665823814"></a><a name="p2099665823814"></a>PIPE_V</p>
</td>
</tr>
<tr id="row123491657153617"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1834925713361"><a name="p1834925713361"></a><a name="p1834925713361"></a>Sqrt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1341082813918"><a name="p1341082813918"></a><a name="p1341082813918"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p13410112823910"><a name="p13410112823910"></a><a name="p13410112823910"></a>PIPE_V</p>
</td>
</tr>
<tr id="row1434975743616"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p234911573363"><a name="p234911573363"></a><a name="p234911573363"></a>Rsqrt</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p123492134017"><a name="p123492134017"></a><a name="p123492134017"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p12345216407"><a name="p12345216407"></a><a name="p12345216407"></a>PIPE_V</p>
</td>
</tr>
<tr id="row14349135719362"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p9349157143614"><a name="p9349157143614"></a><a name="p9349157143614"></a>Relu</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p54992351406"><a name="p54992351406"></a><a name="p54992351406"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p4499193594015"><a name="p4499193594015"></a><a name="p4499193594015"></a>PIPE_V</p>
</td>
</tr>
<tr id="row143506578364"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p83509577369"><a name="p83509577369"></a><a name="p83509577369"></a>Sub</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p5798171417414"><a name="p5798171417414"></a><a name="p5798171417414"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p579815142414"><a name="p579815142414"></a><a name="p579815142414"></a>PIPE_V</p>
</td>
</tr>
<tr id="row2042132133716"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1342216214370"><a name="p1342216214370"></a><a name="p1342216214370"></a>Mul</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p961244264111"><a name="p961244264111"></a><a name="p961244264111"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8612174218413"><a name="p8612174218413"></a><a name="p8612174218413"></a>PIPE_V</p>
</td>
</tr>
<tr id="row1142232133715"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p64226293719"><a name="p64226293719"></a><a name="p64226293719"></a>Div</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1621612134214"><a name="p1621612134214"></a><a name="p1621612134214"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p562141218426"><a name="p562141218426"></a><a name="p562141218426"></a>PIPE_V</p>
</td>
</tr>
<tr id="row07311579375"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p137329717378"><a name="p137329717378"></a><a name="p137329717378"></a>Max</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1787913764218"><a name="p1787913764218"></a><a name="p1787913764218"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p16879203720427"><a name="p16879203720427"></a><a name="p16879203720427"></a>PIPE_V</p>
</td>
</tr>
<tr id="row97327716373"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p107327720375"><a name="p107327720375"></a><a name="p107327720375"></a>Min</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p992588114313"><a name="p992588114313"></a><a name="p992588114313"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p592518854314"><a name="p592518854314"></a><a name="p592518854314"></a>PIPE_V</p>
</td>
</tr>
<tr id="row15732147113711"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p373217113716"><a name="p373217113716"></a><a name="p373217113716"></a>Adds</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1981124518447"><a name="p1981124518447"></a><a name="p1981124518447"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1181345184417"><a name="p1181345184417"></a><a name="p1181345184417"></a>PIPE_V</p>
</td>
</tr>
<tr id="row187321470373"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4732276372"><a name="p4732276372"></a><a name="p4732276372"></a>Muls</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p148119944513"><a name="p148119944513"></a><a name="p148119944513"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1481114911459"><a name="p1481114911459"></a><a name="p1481114911459"></a>PIPE_V</p>
</td>
</tr>
<tr id="row1873214712377"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p47336713718"><a name="p47336713718"></a><a name="p47336713718"></a>Maxs</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p7790436124514"><a name="p7790436124514"></a><a name="p7790436124514"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19790436164519"><a name="p19790436164519"></a><a name="p19790436164519"></a>PIPE_V</p>
</td>
</tr>
<tr id="row5733147173711"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p7733147103716"><a name="p7733147103716"></a><a name="p7733147103716"></a>Mins</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p222312310468"><a name="p222312310468"></a><a name="p222312310468"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p9223183124611"><a name="p9223183124611"></a><a name="p9223183124611"></a>PIPE_V</p>
</td>
</tr>
<tr id="row173310713713"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p27332793713"><a name="p27332793713"></a><a name="p27332793713"></a>LeakyRelu</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p545263114467"><a name="p545263114467"></a><a name="p545263114467"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p345213116462"><a name="p345213116462"></a><a name="p345213116462"></a>PIPE_V</p>
</td>
</tr>
<tr id="row177334763719"><td class="cellrowborder" rowspan="5" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p101885523479"><a name="p101885523479"></a><a name="p101885523479"></a>逻辑计算</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p167331477374"><a name="p167331477374"></a><a name="p167331477374"></a>Not</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1020421165114"><a name="p1020421165114"></a><a name="p1020421165114"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p52082117519"><a name="p52082117519"></a><a name="p52082117519"></a>PIPE_V</p>
</td>
</tr>
<tr id="row27333715373"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1273417743717"><a name="p1273417743717"></a><a name="p1273417743717"></a>And</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p69764715512"><a name="p69764715512"></a><a name="p69764715512"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p197154712511"><a name="p197154712511"></a><a name="p197154712511"></a>PIPE_V</p>
</td>
</tr>
<tr id="row873419712376"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p107348716373"><a name="p107348716373"></a><a name="p107348716373"></a>Or</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p161373165539"><a name="p161373165539"></a><a name="p161373165539"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8137111619536"><a name="p8137111619536"></a><a name="p8137111619536"></a>PIPE_V</p>
</td>
</tr>
<tr id="row13422152133710"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p94221528374"><a name="p94221528374"></a><a name="p94221528374"></a>ShiftLeft</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p14538124165319"><a name="p14538124165319"></a><a name="p14538124165319"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p65398416534"><a name="p65398416534"></a><a name="p65398416534"></a>PIPE_V</p>
</td>
</tr>
<tr id="row1642213213373"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p3422172153719"><a name="p3422172153719"></a><a name="p3422172153719"></a>ShiftRight</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p6462106135415"><a name="p6462106135415"></a><a name="p6462106135415"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p946256125417"><a name="p946256125417"></a><a name="p946256125417"></a>PIPE_V</p>
</td>
</tr>
<tr id="row342320215373"><td class="cellrowborder" rowspan="11" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p19769275014"><a name="p19769275014"></a><a name="p19769275014"></a>复合计算</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p34233216377"><a name="p34233216377"></a><a name="p34233216377"></a>Axpy</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p75661143185511"><a name="p75661143185511"></a><a name="p75661143185511"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p15662431551"><a name="p15662431551"></a><a name="p15662431551"></a>PIPE_V</p>
</td>
</tr>
<tr id="row17423725372"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p184231326379"><a name="p184231326379"></a><a name="p184231326379"></a>MulAddDst</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p189956165569"><a name="p189956165569"></a><a name="p189956165569"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p899581613562"><a name="p899581613562"></a><a name="p899581613562"></a>PIPE_V</p>
</td>
</tr>
<tr id="row74238220375"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1171218243567"><a name="p1171218243567"></a><a name="p1171218243567"></a>MulCast</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9191242155619"><a name="p9191242155619"></a><a name="p9191242155619"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p8191184295617"><a name="p8191184295617"></a><a name="p8191184295617"></a>PIPE_V</p>
</td>
</tr>
<tr id="row14423626374"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4423172163712"><a name="p4423172163712"></a><a name="p4423172163712"></a>AddRelu</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8263137115718"><a name="p8263137115718"></a><a name="p8263137115718"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p2026367135710"><a name="p2026367135710"></a><a name="p2026367135710"></a>PIPE_V</p>
</td>
</tr>
<tr id="row22551118145718"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p72561818115720"><a name="p72561818115720"></a><a name="p72561818115720"></a>AddReluCast</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p10401184545810"><a name="p10401184545810"></a><a name="p10401184545810"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1140184516586"><a name="p1140184516586"></a><a name="p1140184516586"></a>PIPE_V</p>
</td>
</tr>
<tr id="row625691815572"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1225620180576"><a name="p1225620180576"></a><a name="p1225620180576"></a>SubRelu</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p12556410135917"><a name="p12556410135917"></a><a name="p12556410135917"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p125571310105912"><a name="p125571310105912"></a><a name="p125571310105912"></a>PIPE_V</p>
</td>
</tr>
<tr id="row2256418165719"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p8706121535916"><a name="p8706121535916"></a><a name="p8706121535916"></a>SubReluCast</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p994312355599"><a name="p994312355599"></a><a name="p994312355599"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1794363519595"><a name="p1794363519595"></a><a name="p1794363519595"></a>PIPE_V</p>
</td>
</tr>
<tr id="row152569187573"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p139311758145919"><a name="p139311758145919"></a><a name="p139311758145919"></a>FusedMulAdd</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2989616907"><a name="p2989616907"></a><a name="p2989616907"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p4989916803"><a name="p4989916803"></a><a name="p4989916803"></a>PIPE_V</p>
</td>
</tr>
<tr id="row16256418175719"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p2256121835714"><a name="p2256121835714"></a><a name="p2256121835714"></a>MulAddRelu</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p4338164212010"><a name="p4338164212010"></a><a name="p4338164212010"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19338104218012"><a name="p19338104218012"></a><a name="p19338104218012"></a>PIPE_V</p>
</td>
</tr>
<tr id="row1225614188577"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p191215505019"><a name="p191215505019"></a><a name="p191215505019"></a>CastDequant</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p9591320110"><a name="p9591320110"></a><a name="p9591320110"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p759820015"><a name="p759820015"></a><a name="p759820015"></a>PIPE_V</p>
</td>
</tr>
<tr id="row925741865717"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p52578187570"><a name="p52578187570"></a><a name="p52578187570"></a>AddDeqRelu</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p07086382013"><a name="p07086382013"></a><a name="p07086382013"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1570812387111"><a name="p1570812387111"></a><a name="p1570812387111"></a>PIPE_V</p>
</td>
</tr>
<tr id="row8257131825713"><td class="cellrowborder" rowspan="4" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p16188105219476"><a name="p16188105219476"></a><a name="p16188105219476"></a>比较与选择</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p8257518195715"><a name="p8257518195715"></a><a name="p8257518195715"></a>Compare</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p02571318165713"><a name="p02571318165713"></a><a name="p02571318165713"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1925719185579"><a name="p1925719185579"></a><a name="p1925719185579"></a><span>PIPE_V</span></p>
</td>
</tr>
<tr id="row112571718175715"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p172571718165715"><a name="p172571718165715"></a><a name="p172571718165715"></a>Compare（结果存入寄存器）</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p3243326125812"><a name="p3243326125812"></a><a name="p3243326125812"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p62433262583"><a name="p62433262583"></a><a name="p62433262583"></a><span>PIPE_V</span></p>
</td>
</tr>
<tr id="row425711855717"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p102571218145716"><a name="p102571218145716"></a><a name="p102571218145716"></a>Compares</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p13402461218"><a name="p13402461218"></a><a name="p13402461218"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p174011461824"><a name="p174011461824"></a><a name="p174011461824"></a>PIPE_V</p>
</td>
</tr>
<tr id="row825861817579"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1599951410172"><a name="p1599951410172"></a><a name="p1599951410172"></a>GetCmpMask(ISASI)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p10668145012586"><a name="p10668145012586"></a><a name="p10668145012586"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p17668750105817"><a name="p17668750105817"></a><a name="p17668750105817"></a><span>PIPE_V</span></p>
</td>
</tr>
<tr id="row1370010560315"><td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p7188552124714"><a name="p7188552124714"></a><a name="p7188552124714"></a>类型转换</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p19700356939"><a name="p19700356939"></a><a name="p19700356939"></a>Cast</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p18321212519"><a name="p18321212519"></a><a name="p18321212519"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p583220211655"><a name="p583220211655"></a><a name="p583220211655"></a>PIPE_V</p>
</td>
</tr>
<tr id="row19700256333"><td class="cellrowborder" rowspan="8" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p121889526475"><a name="p121889526475"></a><a name="p121889526475"></a>归约计算</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p570010561636"><a name="p570010561636"></a><a name="p570010561636"></a>ReduceDataBlock</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1531511916612"><a name="p1531511916612"></a><a name="p1531511916612"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p63152191268"><a name="p63152191268"></a><a name="p63152191268"></a>PIPE_V</p>
</td>
</tr>
<tr id="row0700155618317"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1370014563310"><a name="p1370014563310"></a><a name="p1370014563310"></a>ReduceRepeat</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p142534511586"><a name="p142534511586"></a><a name="p142534511586"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p10253145110814"><a name="p10253145110814"></a><a name="p10253145110814"></a>PIPE_V</p>
</td>
</tr>
<tr id="row1570118567310"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p157011056235"><a name="p157011056235"></a><a name="p157011056235"></a>ReducePairElem</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p18661924396"><a name="p18661924396"></a><a name="p18661924396"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1486632103911"><a name="p1486632103911"></a><a name="p1486632103911"></a>PIPE_V</p>
</td>
</tr>
<tr id="row970117567311"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p070125618318"><a name="p070125618318"></a><a name="p070125618318"></a>ReduceMax</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p165581031113913"><a name="p165581031113913"></a><a name="p165581031113913"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p11558193111394"><a name="p11558193111394"></a><a name="p11558193111394"></a>PIPE_V</p>
</td>
</tr>
<tr id="row11284283918"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1472114923916"><a name="p1472114923916"></a><a name="p1472114923916"></a>ReduceMin</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p16807154407"><a name="p16807154407"></a><a name="p16807154407"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p98078554019"><a name="p98078554019"></a><a name="p98078554019"></a>PIPE_V</p>
</td>
</tr>
<tr id="row12324283912"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p4583152814013"><a name="p4583152814013"></a><a name="p4583152814013"></a>ReduceSum</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1253294794010"><a name="p1253294794010"></a><a name="p1253294794010"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p165329473404"><a name="p165329473404"></a><a name="p165329473404"></a>PIPE_V</p>
</td>
</tr>
<tr id="row83144263910"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p34117573414"><a name="p34117573414"></a><a name="p34117573414"></a>GetReduceRepeatSumSpr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p153982024144717"><a name="p153982024144717"></a><a name="p153982024144717"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p731042113911"><a name="p731042113911"></a><a name="p731042113911"></a>PIPE_S</p>
</td>
</tr>
<tr id="row103642173911"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1349105235110"><a name="p1349105235110"></a><a name="p1349105235110"></a>GetReduceRepeatMaxMinSpr</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p8508425144717"><a name="p8508425144717"></a><a name="p8508425144717"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p13424216394"><a name="p13424216394"></a><a name="p13424216394"></a>PIPE_S</p>
</td>
</tr>
<tr id="row2041842173916"><td class="cellrowborder" rowspan="2" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p518875218479"><a name="p518875218479"></a><a name="p518875218479"></a>数据排布格式转换</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p2041342193911"><a name="p2041342193911"></a><a name="p2041342193911"></a>Transpose</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p1971061912230"><a name="p1971061912230"></a><a name="p1971061912230"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p271018196232"><a name="p271018196232"></a><a name="p271018196232"></a>PIPE_V</p>
</td>
</tr>
<tr id="row144184253914"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p741042113914"><a name="p741042113914"></a><a name="p741042113914"></a>TransDataTo5HD</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p97105194234"><a name="p97105194234"></a><a name="p97105194234"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p19710151912232"><a name="p19710151912232"></a><a name="p19710151912232"></a>PIPE_V</p>
</td>
</tr>
<tr id="row8424217392"><td class="cellrowborder" rowspan="3" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p131887521474"><a name="p131887521474"></a><a name="p131887521474"></a>数据填充</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p164164223915"><a name="p164164223915"></a><a name="p164164223915"></a>Brcb</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p165921520182311"><a name="p165921520182311"></a><a name="p165921520182311"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p759282002319"><a name="p759282002319"></a><a name="p759282002319"></a>PIPE_V</p>
</td>
</tr>
<tr id="row157873313543"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1678773165418"><a name="p1678773165418"></a><a name="p1678773165418"></a>Duplicate</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1091042162317"><a name="p1091042162317"></a><a name="p1091042162317"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p149101621112310"><a name="p149101621112310"></a><a name="p149101621112310"></a>PIPE_V</p>
</td>
</tr>
<tr id="row177872313541"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p77872311547"><a name="p77872311547"></a><a name="p77872311547"></a>CreateVecIndex</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p491062115234"><a name="p491062115234"></a><a name="p491062115234"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p169101215237"><a name="p169101215237"></a><a name="p169101215237"></a>PIPE_V</p>
</td>
</tr>
<tr id="row1278713110545"><td class="cellrowborder" rowspan="3" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1318875215476"><a name="p1318875215476"></a><a name="p1318875215476"></a>排序组合</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p197881931135418"><a name="p197881931135418"></a><a name="p197881931135418"></a>Sort32</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p10226133117551"><a name="p10226133117551"></a><a name="p10226133117551"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p1222618311556"><a name="p1222618311556"></a><a name="p1222618311556"></a>PIPE_V</p>
</td>
</tr>
<tr id="row13788163118544"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p9788731155417"><a name="p9788731155417"></a><a name="p9788731155417"></a>MrgSort</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p109121758175510"><a name="p109121758175510"></a><a name="p109121758175510"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1912185855518"><a name="p1912185855518"></a><a name="p1912185855518"></a>PIPE_V</p>
</td>
</tr>
<tr id="row1778843195412"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1260719563"><a name="p1260719563"></a><a name="p1260719563"></a>GetMrgSortResult</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p710014280477"><a name="p710014280477"></a><a name="p710014280477"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p5788131175414"><a name="p5788131175414"></a><a name="p5788131175414"></a>PIPE_S</p>
</td>
</tr>
<tr id="row1745875045615"><td class="cellrowborder" rowspan="2" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1318815284711"><a name="p1318815284711"></a><a name="p1318815284711"></a>离散与聚合</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p1945835015560"><a name="p1945835015560"></a><a name="p1945835015560"></a>Gather</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p45021723122314"><a name="p45021723122314"></a><a name="p45021723122314"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p4502823192312"><a name="p4502823192312"></a><a name="p4502823192312"></a>PIPE_V</p>
</td>
</tr>
<tr id="row13458125065610"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p245895055615"><a name="p245895055615"></a><a name="p245895055615"></a>Gatherb</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p15502823192310"><a name="p15502823192310"></a><a name="p15502823192310"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p35021023112316"><a name="p35021023112316"></a><a name="p35021023112316"></a>PIPE_V</p>
</td>
</tr>
<tr id="row74591350175619"><td class="cellrowborder" rowspan="4" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1818875294720"><a name="p1818875294720"></a><a name="p1818875294720"></a>掩码操作</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p6606162312576"><a name="p6606162312576"></a><a name="p6606162312576"></a>SetMaskCount</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p99918191609"><a name="p99918191609"></a><a name="p99918191609"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p182376212116"><a name="p182376212116"></a><a name="p182376212116"></a>PIPE_S</p>
</td>
</tr>
<tr id="row14459105025610"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p16459550125611"><a name="p16459550125611"></a><a name="p16459550125611"></a>SetMaskNorm</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p2010417191005"><a name="p2010417191005"></a><a name="p2010417191005"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1024818211117"><a name="p1024818211117"></a><a name="p1024818211117"></a>PIPE_S</p>
</td>
</tr>
<tr id="row9459135015569"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1045995018568"><a name="p1045995018568"></a><a name="p1045995018568"></a>SetVectorMask</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p910815191502"><a name="p910815191502"></a><a name="p910815191502"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p12601128111"><a name="p12601128111"></a><a name="p12601128111"></a>PIPE_S</p>
</td>
</tr>
<tr id="row7459135065618"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p1459115075615"><a name="p1459115075615"></a><a name="p1459115075615"></a>ResetMask</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1811417191003"><a name="p1811417191003"></a><a name="p1811417191003"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p3266132151113"><a name="p3266132151113"></a><a name="p3266132151113"></a>PIPE_S</p>
</td>
</tr>
<tr id="row165752169017"><td class="cellrowborder" rowspan="5" valign="top" width="25%" headers="mcps1.2.5.1.1 "><p id="p1318815224715"><a name="p1318815224715"></a><a name="p1318815224715"></a>数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.2 "><p id="p157513161403"><a name="p157513161403"></a><a name="p157513161403"></a>DataCopy(GM-&gt;UB)</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="p7342185114282"><a name="p7342185114282"></a><a name="p7342185114282"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="p134265152811"><a name="p134265152811"></a><a name="p134265152811"></a>PIPE_MTE2</p>
</td>
</tr>
<tr id="row57189271303"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p471814279015"><a name="p471814279015"></a><a name="p471814279015"></a>DataCopy(UB-&gt;GM)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p555915332813"><a name="p555915332813"></a><a name="p555915332813"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p1155916531287"><a name="p1155916531287"></a><a name="p1155916531287"></a>PIPE_MTE3</p>
</td>
</tr>
<tr id="row1371916271019"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p771917271206"><a name="p771917271206"></a><a name="p771917271206"></a>DataCopyPad(GM-&gt;UB)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p343845642820"><a name="p343845642820"></a><a name="p343845642820"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p144381156122810"><a name="p144381156122810"></a><a name="p144381156122810"></a>PIPE_MTE2</p>
</td>
</tr>
<tr id="row63137221203"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p18732151320215"><a name="p18732151320215"></a><a name="p18732151320215"></a>DataCopyPad(UB-&gt;GM)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p122881957162818"><a name="p122881957162818"></a><a name="p122881957162818"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p428813575286"><a name="p428813575286"></a><a name="p428813575286"></a>PIPE_MTE3</p>
</td>
</tr>
<tr id="row123131322803"><td class="cellrowborder" valign="top" headers="mcps1.2.5.1.1 "><p id="p20313152212019"><a name="p20313152212019"></a><a name="p20313152212019"></a>DataCopy(UB-&gt;UB)</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.2 "><p id="p1219827172917"><a name="p1219827172917"></a><a name="p1219827172917"></a>AIV</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.5.1.3 "><p id="p17198167102914"><a name="p17198167102914"></a><a name="p17198167102914"></a>PIPE_V</p>
</td>
</tr>
</tbody>
</table>

**表 2**  CUBE API流水类型汇总

<a name="table12311219174812"></a>
<table><thead align="left"><tr id="row73111190489"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p453227115016"><a name="p453227115016"></a><a name="p453227115016"></a>API接口名词</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p1153127145016"><a name="p1153127145016"></a><a name="p1153127145016"></a>执行核（AIC/AIV）</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p453727185016"><a name="p453727185016"></a><a name="p453727185016"></a>流水</p>
</th>
</tr>
</thead>
<tbody><tr id="row16311171913480"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p36681446124819"><a name="p36681446124819"></a><a name="p36681446124819"></a>DataCopy(GM-&gt;L1)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p9669144619480"><a name="p9669144619480"></a><a name="p9669144619480"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p0669194611488"><a name="p0669194611488"></a><a name="p0669194611488"></a>PIPE_MTE2</p>
</td>
</tr>
<tr id="row531121910482"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p198731554104819"><a name="p198731554104819"></a><a name="p198731554104819"></a>Load2D(GM-&gt;L1)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p89921722203"><a name="p89921722203"></a><a name="p89921722203"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1899214223015"><a name="p1899214223015"></a><a name="p1899214223015"></a>PIPE_MTE2</p>
</td>
</tr>
<tr id="row10312219164817"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1031220192485"><a name="p1031220192485"></a><a name="p1031220192485"></a>Load2DV2(GM-&gt;L1)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1393518571705"><a name="p1393518571705"></a><a name="p1393518571705"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p11935757804"><a name="p11935757804"></a><a name="p11935757804"></a>PIPE_MTE2</p>
</td>
</tr>
<tr id="row331211954810"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p93126199484"><a name="p93126199484"></a><a name="p93126199484"></a>Load2D(GM-&gt;L0)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1833211251219"><a name="p1833211251219"></a><a name="p1833211251219"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p6332125821"><a name="p6332125821"></a><a name="p6332125821"></a>PIPE_MTE2</p>
</td>
</tr>
<tr id="row10312161913487"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1031213199489"><a name="p1031213199489"></a><a name="p1031213199489"></a>Load2D(L1-&gt;L0)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p0863152512218"><a name="p0863152512218"></a><a name="p0863152512218"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p2086372517213"><a name="p2086372517213"></a><a name="p2086372517213"></a>PIPE_MTE1</p>
</td>
</tr>
<tr id="row13124193483"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p19312171915484"><a name="p19312171915484"></a><a name="p19312171915484"></a>Load3D(L1-&gt;L0)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1681215316313"><a name="p1681215316313"></a><a name="p1681215316313"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p88125531137"><a name="p88125531137"></a><a name="p88125531137"></a>PIPE_MTE1</p>
</td>
</tr>
<tr id="row168581710131711"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p108581010191719"><a name="p108581010191719"></a><a name="p108581010191719"></a>LoadDataWithTranspose(L1-&gt;L0)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p552553210174"><a name="p552553210174"></a><a name="p552553210174"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p152513321170"><a name="p152513321170"></a><a name="p152513321170"></a>PIPE_MTE1</p>
</td>
</tr>
<tr id="row78581310131711"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p785821091717"><a name="p785821091717"></a><a name="p785821091717"></a>LoadDataWithSparse(L1-&gt;L0)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p104201925101812"><a name="p104201925101812"></a><a name="p104201925101812"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p194209254187"><a name="p194209254187"></a><a name="p194209254187"></a>PIPE_MTE1</p>
</td>
</tr>
<tr id="row198587101172"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p6858161011170"><a name="p6858161011170"></a><a name="p6858161011170"></a>Bias Table Buffer(L1-&gt;BT)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p17974181513209"><a name="p17974181513209"></a><a name="p17974181513209"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p2974121512200"><a name="p2974121512200"></a><a name="p2974121512200"></a>PIPE_MTE1</p>
</td>
</tr>
<tr id="row158586101178"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p2085811010178"><a name="p2085811010178"></a><a name="p2085811010178"></a>Fixpipe Buffer(L1-&gt;FB)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p13806992219"><a name="p13806992219"></a><a name="p13806992219"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p580209142220"><a name="p580209142220"></a><a name="p580209142220"></a>PIPE_MTE1</p>
</td>
</tr>
<tr id="row14859610171711"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p7859101019176"><a name="p7859101019176"></a><a name="p7859101019176"></a>Fill(L1)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p94950170290"><a name="p94950170290"></a><a name="p94950170290"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p949515174299"><a name="p949515174299"></a><a name="p949515174299"></a>PIPE_MTE2</p>
</td>
</tr>
<tr id="row1585914102172"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p5859101017175"><a name="p5859101017175"></a><a name="p5859101017175"></a>Fill(L0)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p5495101717291"><a name="p5495101717291"></a><a name="p5495101717291"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1949520178293"><a name="p1949520178293"></a><a name="p1949520178293"></a>PIPE_MTE1</p>
</td>
</tr>
<tr id="row72821125122916"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p18282112572919"><a name="p18282112572919"></a><a name="p18282112572919"></a>SetFmatrix</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1928272516294"><a name="p1928272516294"></a><a name="p1928272516294"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p528212515299"><a name="p528212515299"></a><a name="p528212515299"></a>PIPE_S</p>
</td>
</tr>
<tr id="row16282162520297"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p728220252291"><a name="p728220252291"></a><a name="p728220252291"></a>SetLoadDataBoundary</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p567513743316"><a name="p567513743316"></a><a name="p567513743316"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p196751713311"><a name="p196751713311"></a><a name="p196751713311"></a>PIPE_S</p>
</td>
</tr>
<tr id="row11282172513298"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p32821025182916"><a name="p32821025182916"></a><a name="p32821025182916"></a>SetLoadDataRepeat</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p88951728142416"><a name="p88951728142416"></a><a name="p88951728142416"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1789562822418"><a name="p1789562822418"></a><a name="p1789562822418"></a>PIPE_S</p>
</td>
</tr>
<tr id="row32821325112911"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1128232532910"><a name="p1128232532910"></a><a name="p1128232532910"></a>SetLoadDataPaddingValue</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1955916474241"><a name="p1955916474241"></a><a name="p1955916474241"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p13559147162418"><a name="p13559147162418"></a><a name="p13559147162418"></a>PIPE_S</p>
</td>
</tr>
<tr id="row20282112519298"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p2282112522914"><a name="p2282112522914"></a><a name="p2282112522914"></a>Mmad</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1794325619250"><a name="p1794325619250"></a><a name="p1794325619250"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1294385620253"><a name="p1294385620253"></a><a name="p1294385620253"></a>PIPE_M</p>
</td>
</tr>
<tr id="row62831725152919"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1628317257292"><a name="p1628317257292"></a><a name="p1628317257292"></a>MmadWithSparse</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p191315211271"><a name="p191315211271"></a><a name="p191315211271"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p51314214275"><a name="p51314214275"></a><a name="p51314214275"></a>PIPE_M</p>
</td>
</tr>
<tr id="row128317255299"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p162831525122919"><a name="p162831525122919"></a><a name="p162831525122919"></a>SetMMColumnMajor/SetMMRowMajor</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p146607912811"><a name="p146607912811"></a><a name="p146607912811"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p16601898285"><a name="p16601898285"></a><a name="p16601898285"></a>PIPE_S</p>
</td>
</tr>
<tr id="row52831325162914"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p2028382562911"><a name="p2028382562911"></a><a name="p2028382562911"></a>SetHF32Mode</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p139231236292"><a name="p139231236292"></a><a name="p139231236292"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p9923223122915"><a name="p9923223122915"></a><a name="p9923223122915"></a>PIPE_S</p>
</td>
</tr>
<tr id="row142831525152913"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p8283112513294"><a name="p8283112513294"></a><a name="p8283112513294"></a>SetHF32TransMode</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p14659644306"><a name="p14659644306"></a><a name="p14659644306"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p76596415308"><a name="p76596415308"></a><a name="p76596415308"></a>PIPE_S</p>
</td>
</tr>
<tr id="row1328319255291"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p3283625122918"><a name="p3283625122918"></a><a name="p3283625122918"></a>DataCopy(L0C-&gt;GM)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p159751536113112"><a name="p159751536113112"></a><a name="p159751536113112"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p119754361314"><a name="p119754361314"></a><a name="p119754361314"></a>PIPE_FIX</p>
</td>
</tr>
<tr id="row0283182552910"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1128319252291"><a name="p1128319252291"></a><a name="p1128319252291"></a>DataCopy(L0C-&gt;L1)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p198761593613"><a name="p198761593613"></a><a name="p198761593613"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p10987141593612"><a name="p10987141593612"></a><a name="p10987141593612"></a>PIPE_FIX</p>
</td>
</tr>
<tr id="row16632383379"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1466613489375"><a name="p1466613489375"></a><a name="p1466613489375"></a>DataCopy(L1-&gt;GM)</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p211341713388"><a name="p211341713388"></a><a name="p211341713388"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p17113181753816"><a name="p17113181753816"></a><a name="p17113181753816"></a>PIPE_MTE3</p>
</td>
</tr>
<tr id="row9631038143714"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1163163817378"><a name="p1163163817378"></a><a name="p1163163817378"></a>SetFixPipeConfig</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p5103122164010"><a name="p5103122164010"></a><a name="p5103122164010"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p31031521114013"><a name="p31031521114013"></a><a name="p31031521114013"></a>PIPE_S</p>
</td>
</tr>
<tr id="row126363819374"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p116317381377"><a name="p116317381377"></a><a name="p116317381377"></a>SetFixpipePreQuantFlag</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p158934439407"><a name="p158934439407"></a><a name="p158934439407"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1089317434400"><a name="p1089317434400"></a><a name="p1089317434400"></a>PIPE_S</p>
</td>
</tr>
<tr id="row1664113833713"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p564133811379"><a name="p564133811379"></a><a name="p564133811379"></a>SetFixpipeNz2ndFlag</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p1644512314411"><a name="p1644512314411"></a><a name="p1644512314411"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1044503124114"><a name="p1044503124114"></a><a name="p1044503124114"></a>PIPE_S</p>
</td>
</tr>
<tr id="row864238193716"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1264123819373"><a name="p1264123819373"></a><a name="p1264123819373"></a>SetLreluALPHA</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 "><p id="p77911939114116"><a name="p77911939114116"></a><a name="p77911939114116"></a>AIC</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1479113398417"><a name="p1479113398417"></a><a name="p1479113398417"></a>PIPE_S</p>
</td>
</tr>
</tbody>
</table>

**表 3**  同步指令流水类型汇总

<a name="table1054353913238"></a>
<table><thead align="left"><tr id="row3544163913239"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p17814935142618"><a name="p17814935142618"></a><a name="p17814935142618"></a>API接口名词</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p14814143512264"><a name="p14814143512264"></a><a name="p14814143512264"></a>执行核（AIC/AIV）</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p178146358265"><a name="p178146358265"></a><a name="p178146358265"></a>流水</p>
</th>
</tr>
</thead>
<tbody><tr id="row15544143912316"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p13150104172610"><a name="p13150104172610"></a><a name="p13150104172610"></a>SetFlag/WaitFlag</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p254415392234"><a name="p254415392234"></a><a name="p254415392234"></a>PIPE_S</p>
</td>
</tr>
<tr id="row155441539132320"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p155443397232"><a name="p155443397232"></a><a name="p155443397232"></a>PipeBarrier</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1254463932317"><a name="p1254463932317"></a><a name="p1254463932317"></a>PIPE_S</p>
</td>
</tr>
<tr id="row19544143922320"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p654413916232"><a name="p654413916232"></a><a name="p654413916232"></a>DataSyncBarrier</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1990121814292"><a name="p1990121814292"></a><a name="p1990121814292"></a>PIPE_S</p>
</td>
</tr>
<tr id="row1054443917233"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p854411392238"><a name="p854411392238"></a><a name="p854411392238"></a>CrossCoreSetFlag</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1954483912317"><a name="p1954483912317"></a><a name="p1954483912317"></a>PIPE_S</p>
</td>
</tr>
<tr id="row11544939132319"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p5544143913237"><a name="p5544143913237"></a><a name="p5544143913237"></a>CrossCoreWaitFlag</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p254414396231"><a name="p254414396231"></a><a name="p254414396231"></a>PIPE_S</p>
</td>
</tr>
</tbody>
</table>

**表 4**  原子操作流水类型汇总

<a name="table25871912193814"></a>
<table><thead align="left"><tr id="row158711125385"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p19933141919385"><a name="p19933141919385"></a><a name="p19933141919385"></a>API接口名词</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p17933161920388"><a name="p17933161920388"></a><a name="p17933161920388"></a>执行核（AIC/AIV）</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p193321914389"><a name="p193321914389"></a><a name="p193321914389"></a>流水</p>
</th>
</tr>
</thead>
<tbody><tr id="row15587141216387"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p2587191212388"><a name="p2587191212388"></a><a name="p2587191212388"></a>SetAtomicAdd</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p17587112203812"><a name="p17587112203812"></a><a name="p17587112203812"></a>PIPE_S</p>
</td>
</tr>
<tr id="row758710125387"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p115887126383"><a name="p115887126383"></a><a name="p115887126383"></a>SetAtomicMax</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p1658891203817"><a name="p1658891203817"></a><a name="p1658891203817"></a>PIPE_S</p>
</td>
</tr>
<tr id="row15882122385"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p55883124383"><a name="p55883124383"></a><a name="p55883124383"></a>SetAtomicMin</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p6588201253815"><a name="p6588201253815"></a><a name="p6588201253815"></a>PIPE_S</p>
</td>
</tr>
<tr id="row65882012193814"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p14588131214389"><a name="p14588131214389"></a><a name="p14588131214389"></a>SetAtomicType</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p258821211386"><a name="p258821211386"></a><a name="p258821211386"></a>PIPE_S</p>
</td>
</tr>
<tr id="row12588111223812"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p13588151211382"><a name="p13588151211382"></a><a name="p13588151211382"></a>DisableDmaAtomic</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p958817126383"><a name="p958817126383"></a><a name="p958817126383"></a>PIPE_S</p>
</td>
</tr>
</tbody>
</table>

**表 5**  工具接口流水类型汇总

<a name="table1147331317457"></a>
<table><thead align="left"><tr id="row747316130455"><th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.1"><p id="p2360117184520"><a name="p2360117184520"></a><a name="p2360117184520"></a>API接口名词</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.2"><p id="p936019173455"><a name="p936019173455"></a><a name="p936019173455"></a>执行核（AIC/AIV）</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="p83601317184519"><a name="p83601317184519"></a><a name="p83601317184519"></a>流水</p>
</th>
</tr>
</thead>
<tbody><tr id="row947341315456"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p8473101318455"><a name="p8473101318455"></a><a name="p8473101318455"></a>GetBlockNum</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p144731136458"><a name="p144731136458"></a><a name="p144731136458"></a>PIPE_S</p>
</td>
</tr>
<tr id="row5473413184518"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p347351310451"><a name="p347351310451"></a><a name="p347351310451"></a>GetBlockIdx</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p7473141314512"><a name="p7473141314512"></a><a name="p7473141314512"></a>PIPE_S</p>
</td>
</tr>
<tr id="row1447317134458"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p114731013194514"><a name="p114731013194514"></a><a name="p114731013194514"></a>GetTaskRatio</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p24911230154615"><a name="p24911230154615"></a><a name="p24911230154615"></a>PIPE_S</p>
</td>
</tr>
<tr id="row14741313194511"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p1547491313453"><a name="p1547491313453"></a><a name="p1547491313453"></a>GetSubBlockNum</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p370845217463"><a name="p370845217463"></a><a name="p370845217463"></a>PIPE_S</p>
</td>
</tr>
<tr id="row134741813194518"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p647441311458"><a name="p647441311458"></a><a name="p647441311458"></a>GetSubBlockIdx</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p947517137458"><a name="p947517137458"></a><a name="p947517137458"></a>PIPE_S</p>
</td>
</tr>
<tr id="row347521320452"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p124751413104518"><a name="p124751413104518"></a><a name="p124751413104518"></a>GetDataBlockSizeInBytes</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p870210326477"><a name="p870210326477"></a><a name="p870210326477"></a>PIPE_S</p>
</td>
</tr>
<tr id="row1947510137459"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p6376387471"><a name="p6376387471"></a><a name="p6376387471"></a>GetSystemCycle</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p5475191318452"><a name="p5475191318452"></a><a name="p5475191318452"></a>PIPE_S</p>
</td>
</tr>
<tr id="row9971106154815"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p2097115634816"><a name="p2097115634816"></a><a name="p2097115634816"></a>GetProgramCounter</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p17971146104819"><a name="p17971146104819"></a><a name="p17971146104819"></a>PIPE_S</p>
</td>
</tr>
<tr id="row64681911194814"><td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.1 "><p id="p746819111482"><a name="p746819111482"></a><a name="p746819111482"></a>GetArchVersion</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.2 ">&nbsp;&nbsp;</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="p946851114483"><a name="p946851114483"></a><a name="p946851114483"></a>PIPE_S</p>
</td>
</tr>
</tbody>
</table>

