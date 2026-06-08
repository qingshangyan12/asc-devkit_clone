# Cube指令理论性能汇总<a name="ZH-CN_TOPIC_0000002607728945"></a>

**表 1**  矩阵计算搬入至L0的指令理论性能说明

<a name="table19306524182917"></a>
<table><thead align="left"><tr id="row163069240290"><th class="cellrowborder" valign="top" width="41.48%" id="mcps1.2.4.1.1"><p id="p430652419296"><a name="p430652419296"></a><a name="p430652419296"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="27.51%" id="mcps1.2.4.1.2"><p id="p030682417299"><a name="p030682417299"></a><a name="p030682417299"></a>M/N</p>
</th>
<th class="cellrowborder" valign="top" width="31.009999999999998%" id="mcps1.2.4.1.3"><p id="p330612462913"><a name="p330612462913"></a><a name="p330612462913"></a>K</p>
</th>
</tr>
</thead>
<tbody><tr id="row3306172413294"><td class="cellrowborder" rowspan="2" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p23064249291"><a name="p23064249291"></a><a name="p23064249291"></a>Load2D(L1-&gt;L0A)</p>
<p id="p16306112462913"><a name="p16306112462913"></a><a name="p16306112462913"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p4306132417296"><a name="p4306132417296"></a><a name="p4306132417296"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p7306224132920"><a name="p7306224132920"></a><a name="p7306224132920"></a>16</p>
</td>
</tr>
<tr id="row13306102422918"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p230692414299"><a name="p230692414299"></a><a name="p230692414299"></a>32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p7306162422919"><a name="p7306162422919"></a><a name="p7306162422919"></a>32</p>
</td>
</tr>
<tr id="row173061424172918"><td class="cellrowborder" rowspan="2" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p9306324142918"><a name="p9306324142918"></a><a name="p9306324142918"></a>Load2D(L1-&gt;L0A)</p>
<p id="p19306122412918"><a name="p19306122412918"></a><a name="p19306122412918"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p630612418291"><a name="p630612418291"></a><a name="p630612418291"></a>64</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p1830616248292"><a name="p1830616248292"></a><a name="p1830616248292"></a>64</p>
</td>
</tr>
<tr id="row5307142482913"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p113071424152912"><a name="p113071424152912"></a><a name="p113071424152912"></a>128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p12307724172919"><a name="p12307724172919"></a><a name="p12307724172919"></a>128</p>
</td>
</tr>
<tr id="row930792417296"><td class="cellrowborder" rowspan="2" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p6307132402918"><a name="p6307132402918"></a><a name="p6307132402918"></a>Load2D(L1-&gt;L0B)</p>
<p id="p12307142462910"><a name="p12307142462910"></a><a name="p12307142462910"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p73071824172912"><a name="p73071824172912"></a><a name="p73071824172912"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p11307112419293"><a name="p11307112419293"></a><a name="p11307112419293"></a>16</p>
</td>
</tr>
<tr id="row14307524172917"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p15307132462918"><a name="p15307132462918"></a><a name="p15307132462918"></a>32</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p230719245292"><a name="p230719245292"></a><a name="p230719245292"></a>32</p>
</td>
</tr>
<tr id="row6307162452918"><td class="cellrowborder" rowspan="2" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p030711245291"><a name="p030711245291"></a><a name="p030711245291"></a>Load2D(L1-&gt;L0B)</p>
<p id="p1930772417292"><a name="p1930772417292"></a><a name="p1930772417292"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p5307202472916"><a name="p5307202472916"></a><a name="p5307202472916"></a>64</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p2307112472913"><a name="p2307112472913"></a><a name="p2307112472913"></a>64</p>
</td>
</tr>
<tr id="row1630792442916"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p63070247297"><a name="p63070247297"></a><a name="p63070247297"></a>128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p19307192432914"><a name="p19307192432914"></a><a name="p19307192432914"></a>128</p>
</td>
</tr>
<tr id="row203071224112914"><td class="cellrowborder" rowspan="3" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p130712248293"><a name="p130712248293"></a><a name="p130712248293"></a>Load3D(L1-&gt;L0A)</p>
<p id="p15307924102914"><a name="p15307924102914"></a><a name="p15307924102914"></a></p>
<p id="p15307524152915"><a name="p15307524152915"></a><a name="p15307524152915"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p730732415294"><a name="p730732415294"></a><a name="p730732415294"></a>32</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p630742492919"><a name="p630742492919"></a><a name="p630742492919"></a>32</p>
</td>
</tr>
<tr id="row19307102417290"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p103087248292"><a name="p103087248292"></a><a name="p103087248292"></a>64</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p103081924182915"><a name="p103081924182915"></a><a name="p103081924182915"></a>64</p>
</td>
</tr>
<tr id="row1630822422913"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p16308172492915"><a name="p16308172492915"></a><a name="p16308172492915"></a>128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p15308224112917"><a name="p15308224112917"></a><a name="p15308224112917"></a>128</p>
</td>
</tr>
<tr id="row330802462919"><td class="cellrowborder" rowspan="3" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p15308132410294"><a name="p15308132410294"></a><a name="p15308132410294"></a>Load3D(L1-&gt;L0B)</p>
<p id="p19308192411299"><a name="p19308192411299"></a><a name="p19308192411299"></a></p>
<p id="p2308142432910"><a name="p2308142432910"></a><a name="p2308142432910"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p130892472915"><a name="p130892472915"></a><a name="p130892472915"></a>32</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p130842432911"><a name="p130842432911"></a><a name="p130842432911"></a>32</p>
</td>
</tr>
<tr id="row7308142412299"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1130842413295"><a name="p1130842413295"></a><a name="p1130842413295"></a>64</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p23081724102919"><a name="p23081724102919"></a><a name="p23081724102919"></a>64</p>
</td>
</tr>
<tr id="row173081324112920"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p14308122462914"><a name="p14308122462914"></a><a name="p14308122462914"></a>128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1630822422913"><a name="p1630822422913"></a><a name="p1630822422913"></a>128</p>
</td>
</tr>
<tr id="row9308122492920"><td class="cellrowborder" rowspan="2" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p030802452911"><a name="p030802452911"></a><a name="p030802452911"></a>LoadDataWithTranspose(L1-&gt;L0A)</p>
<p id="p9308182411295"><a name="p9308182411295"></a><a name="p9308182411295"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p1530832422915"><a name="p1530832422915"></a><a name="p1530832422915"></a>64</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p1330811246297"><a name="p1330811246297"></a><a name="p1330811246297"></a>64</p>
</td>
</tr>
<tr id="row1730812462913"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p113089248293"><a name="p113089248293"></a><a name="p113089248293"></a>128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p1130852492915"><a name="p1130852492915"></a><a name="p1130852492915"></a>128</p>
</td>
</tr>
<tr id="row16308182417292"><td class="cellrowborder" rowspan="2" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p43082243297"><a name="p43082243297"></a><a name="p43082243297"></a>LoadDataWithTranspose(L1-&gt;L0B)</p>
<p id="p6308182462918"><a name="p6308182462918"></a><a name="p6308182462918"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p1130819242292"><a name="p1130819242292"></a><a name="p1130819242292"></a>64</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p23081224152913"><a name="p23081224152913"></a><a name="p23081224152913"></a>64</p>
</td>
</tr>
<tr id="row1530812248299"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p113081324102910"><a name="p113081324102910"></a><a name="p113081324102910"></a>128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p18308624132913"><a name="p18308624132913"></a><a name="p18308624132913"></a>128</p>
</td>
</tr>
<tr id="row33081624122913"><td class="cellrowborder" rowspan="2" valign="top" width="41.48%" headers="mcps1.2.4.1.1 "><p id="p930842482913"><a name="p930842482913"></a><a name="p930842482913"></a>LoadDataWithSparse(L1-&gt;L0B)</p>
<p id="p19308024112918"><a name="p19308024112918"></a><a name="p19308024112918"></a></p>
</td>
<td class="cellrowborder" valign="top" width="27.51%" headers="mcps1.2.4.1.2 "><p id="p1230814249293"><a name="p1230814249293"></a><a name="p1230814249293"></a>64</p>
</td>
<td class="cellrowborder" valign="top" width="31.009999999999998%" headers="mcps1.2.4.1.3 "><p id="p9308142442913"><a name="p9308142442913"></a><a name="p9308142442913"></a>64</p>
</td>
</tr>
<tr id="row1130811244295"><td class="cellrowborder" valign="top" headers="mcps1.2.4.1.1 "><p id="p1530802418299"><a name="p1530802418299"></a><a name="p1530802418299"></a>128</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.4.1.2 "><p id="p830822416297"><a name="p830822416297"></a><a name="p830822416297"></a>128</p>
</td>
</tr>
</tbody>
</table>

**表 2**  Mmad理论性能计算公式中并行度和k0的取值

<a name="table1877123815211"></a>
<table><thead align="left"><tr id="row167713386219"><th class="cellrowborder" valign="top" width="12.27245449089818%" id="mcps1.2.8.1.1"><p id="p68754616122"><a name="p68754616122"></a><a name="p68754616122"></a>接口</p>
</th>
<th class="cellrowborder" valign="top" width="10.662132426485297%" id="mcps1.2.8.1.2"><p id="p128291316489"><a name="p128291316489"></a><a name="p128291316489"></a>左矩阵A</p>
</th>
<th class="cellrowborder" valign="top" width="15.413082616523303%" id="mcps1.2.8.1.3"><p id="p1049712393"><a name="p1049712393"></a><a name="p1049712393"></a>右矩阵B</p>
</th>
<th class="cellrowborder" valign="top" width="15.413082616523303%" id="mcps1.2.8.1.4"><p id="p477153815217"><a name="p477153815217"></a><a name="p477153815217"></a><a name="image197705574318"></a><a name="image197705574318"></a><span><img class="mathml" id="image197705574318" src="figures/zh-cn_formulaimage_0000002577173240.png"></span></p>
</th>
<th class="cellrowborder" valign="top" width="15.413082616523303%" id="mcps1.2.8.1.5"><p id="p2771113810216"><a name="p2771113810216"></a><a name="p2771113810216"></a><a name="image19625174249"></a><a name="image19625174249"></a><span><img class="mathml" id="image19625174249" src="figures/zh-cn_formulaimage_0000002577333218.png"></span></p>
</th>
<th class="cellrowborder" valign="top" width="15.413082616523303%" id="mcps1.2.8.1.6"><p id="p177718382028"><a name="p177718382028"></a><a name="p177718382028"></a><a name="image6413147411"></a><a name="image6413147411"></a><span><img class="mathml" id="image6413147411" src="figures/zh-cn_formulaimage_0000002577173242.png"></span></p>
</th>
<th class="cellrowborder" valign="top" width="15.413082616523303%" id="mcps1.2.8.1.7"><p id="p877113382028"><a name="p877113382028"></a><a name="p877113382028"></a><a name="image26131925249"></a><a name="image26131925249"></a><span><img class="mathml" id="image26131925249" src="figures/zh-cn_formulaimage_0000002577333220.png"></span></p>
</th>
</tr>
</thead>
<tbody><tr id="row1377133817217"><td class="cellrowborder" rowspan="5" valign="top" width="12.27245449089818%" headers="mcps1.2.8.1.1 "><p id="p78734621212"><a name="p78734621212"></a><a name="p78734621212"></a>Mmad</p>
<p id="p78784681216"><a name="p78784681216"></a><a name="p78784681216"></a></p>
<p id="p78715465128"><a name="p78715465128"></a><a name="p78715465128"></a></p>
<p id="p9872462120"><a name="p9872462120"></a><a name="p9872462120"></a></p>
<p id="p78784681219"><a name="p78784681219"></a><a name="p78784681219"></a></p>
</td>
<td class="cellrowborder" valign="top" width="10.662132426485297%" headers="mcps1.2.8.1.2 "><p id="p1382913169810"><a name="p1382913169810"></a><a name="p1382913169810"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.3 "><p id="p48287161382"><a name="p48287161382"></a><a name="p48287161382"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.4 "><p id="p377118381225"><a name="p377118381225"></a><a name="p377118381225"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.5 "><p id="p1877173819214"><a name="p1877173819214"></a><a name="p1877173819214"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.6 "><p id="p127711381724"><a name="p127711381724"></a><a name="p127711381724"></a>32</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.7 "><p id="p197717381021"><a name="p197717381021"></a><a name="p197717381021"></a>32</p>
</td>
</tr>
<tr id="row1577115381129"><td class="cellrowborder" valign="top" headers="mcps1.2.8.1.1 "><p id="p123781234319"><a name="p123781234319"></a><a name="p123781234319"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.2 "><p id="p4821213121118"><a name="p4821213121118"></a><a name="p4821213121118"></a>half</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.3 "><p id="p1377217381223"><a name="p1377217381223"></a><a name="p1377217381223"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.4 "><p id="p67725384215"><a name="p67725384215"></a><a name="p67725384215"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.5 "><p id="p15772183810211"><a name="p15772183810211"></a><a name="p15772183810211"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.6 "><p id="p2077203811212"><a name="p2077203811212"></a><a name="p2077203811212"></a>16</p>
</td>
</tr>
<tr id="row577253811214"><td class="cellrowborder" valign="top" headers="mcps1.2.8.1.1 "><p id="p123781338312"><a name="p123781338312"></a><a name="p123781338312"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.2 "><p id="p20858149131117"><a name="p20858149131117"></a><a name="p20858149131117"></a>float</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.3 "><p id="p77720381029"><a name="p77720381029"></a><a name="p77720381029"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.4 "><p id="p7772738321"><a name="p7772738321"></a><a name="p7772738321"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.5 "><p id="p47720381021"><a name="p47720381021"></a><a name="p47720381021"></a>4</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.6 "><p id="p1772838422"><a name="p1772838422"></a><a name="p1772838422"></a>8</p>
</td>
</tr>
<tr id="row137721638724"><td class="cellrowborder" valign="top" headers="mcps1.2.8.1.1 "><p id="p38264164812"><a name="p38264164812"></a><a name="p38264164812"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.2 "><p id="p128261616587"><a name="p128261616587"></a><a name="p128261616587"></a>bfloat16_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.3 "><p id="p87722038322"><a name="p87722038322"></a><a name="p87722038322"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.4 "><p id="p127722381528"><a name="p127722381528"></a><a name="p127722381528"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.5 "><p id="p877214381924"><a name="p877214381924"></a><a name="p877214381924"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.6 "><p id="p127727383217"><a name="p127727383217"></a><a name="p127727383217"></a>16</p>
</td>
</tr>
<tr id="row1877212386216"><td class="cellrowborder" valign="top" headers="mcps1.2.8.1.1 "><p id="p1937803193120"><a name="p1937803193120"></a><a name="p1937803193120"></a>int4b_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.2 "><p id="p61841603113"><a name="p61841603113"></a><a name="p61841603113"></a>int4b_t</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.3 "><p id="p127721538326"><a name="p127721538326"></a><a name="p127721538326"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.4 "><p id="p4772163815214"><a name="p4772163815214"></a><a name="p4772163815214"></a>16</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.5 "><p id="p14772123812211"><a name="p14772123812211"></a><a name="p14772123812211"></a>64</p>
</td>
<td class="cellrowborder" valign="top" headers="mcps1.2.8.1.6 "><p id="p11772838322"><a name="p11772838322"></a><a name="p11772838322"></a>64</p>
</td>
</tr>
<tr id="row1432719081417"><td class="cellrowborder" valign="top" width="12.27245449089818%" headers="mcps1.2.8.1.1 "><p id="p932714015141"><a name="p932714015141"></a><a name="p932714015141"></a>MmadWithSparse</p>
</td>
<td class="cellrowborder" valign="top" width="10.662132426485297%" headers="mcps1.2.8.1.2 "><p id="p12280114015158"><a name="p12280114015158"></a><a name="p12280114015158"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.3 "><p id="p19280154041517"><a name="p19280154041517"></a><a name="p19280154041517"></a>int8_t</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.4 "><p id="p1428034031516"><a name="p1428034031516"></a><a name="p1428034031516"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.5 "><p id="p12280140181520"><a name="p12280140181520"></a><a name="p12280140181520"></a>16</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.6 "><p id="p4280840131517"><a name="p4280840131517"></a><a name="p4280840131517"></a>32</p>
</td>
<td class="cellrowborder" valign="top" width="15.413082616523303%" headers="mcps1.2.8.1.7 "><p id="p528018407152"><a name="p528018407152"></a><a name="p528018407152"></a>32</p>
</td>
</tr>
</tbody>
</table>

