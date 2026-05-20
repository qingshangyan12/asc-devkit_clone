# 矩阵计算的搬入总体说明

矩阵计算的搬入是AscendC编程框架中用于数据搬运的核心接口系列，主要实现Global Memory到L0A/L0B之间的数据高效传输。该接口系列提供了多种数据搬运模式，能够满足矩阵计算中左右矩阵等数据的搬运需求，通常与Mmad接口配合使用。

矩阵计算的搬入接口系列支持多种数据搬运场景，包括但不限于**二维格式数据搬运、三维格式转换、压缩数据解压、矩阵转置搬运以及稀疏化数据处理**等。通过灵活配置不同的参数结构体，开发者可以精确控制数据的搬运路径、起始位置、搬运长度等关键信息，充分利用硬件的数据搬运能力，最大化算子的整体执行性能。

矩阵计算的搬入接口支持多种数据通路，每种通路对应特定的存储层级和访问特性。

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品  ：

**表 1** **数据通路和存储层级**

<a name="zh-cn_topic_0000002543771563_table3523123518108"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002543771563_row3542113515107"><th class="cellrowborder" valign="top" width="16.748325167483248%" id="mcps1.2.6.1.1"><p id="zh-cn_topic_0000002543771563_p554243511105"><a name="zh-cn_topic_0000002543771563_p554243511105"></a><a name="zh-cn_topic_0000002543771563_p554243511105"></a>源位置</p>
</th>
<th class="cellrowborder" valign="top" width="13.65863413658634%" id="mcps1.2.6.1.2"><p id="zh-cn_topic_0000002543771563_p5291111573011"><a name="zh-cn_topic_0000002543771563_p5291111573011"></a><a name="zh-cn_topic_0000002543771563_p5291111573011"></a>源地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="11.868813118688129%" id="mcps1.2.6.1.3"><p id="zh-cn_topic_0000002543771563_p9542143515103"><a name="zh-cn_topic_0000002543771563_p9542143515103"></a><a name="zh-cn_topic_0000002543771563_p9542143515103"></a>目的位置</p>
</th>
<th class="cellrowborder" valign="top" width="17.90820917908209%" id="mcps1.2.6.1.4"><p id="zh-cn_topic_0000002543771563_p15420351106"><a name="zh-cn_topic_0000002543771563_p15420351106"></a><a name="zh-cn_topic_0000002543771563_p15420351106"></a>目的地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="39.816018398160175%" id="mcps1.2.6.1.5"><p id="zh-cn_topic_0000002543771563_p4542183531015"><a name="zh-cn_topic_0000002543771563_p4542183531015"></a><a name="zh-cn_topic_0000002543771563_p4542183531015"></a>典型应用场景</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002543771563_row754213581014"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p1254217355103"><a name="zh-cn_topic_0000002543771563_p1254217355103"></a><a name="zh-cn_topic_0000002543771563_p1254217355103"></a>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p1229151515306"><a name="zh-cn_topic_0000002543771563_p1229151515306"></a><a name="zh-cn_topic_0000002543771563_p1229151515306"></a>1字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p15542103510107"><a name="zh-cn_topic_0000002543771563_p15542103510107"></a><a name="zh-cn_topic_0000002543771563_p15542103510107"></a>L1(A1)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p3542135121017"><a name="zh-cn_topic_0000002543771563_p3542135121017"></a><a name="zh-cn_topic_0000002543771563_p3542135121017"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p1454223521019"><a name="zh-cn_topic_0000002543771563_p1454223521019"></a><a name="zh-cn_topic_0000002543771563_p1454223521019"></a>从全局内存加载左矩阵数据到L1</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row13542143510105"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p1954283518102"><a name="zh-cn_topic_0000002543771563_p1954283518102"></a><a name="zh-cn_topic_0000002543771563_p1954283518102"></a>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p12481174583018"><a name="zh-cn_topic_0000002543771563_p12481174583018"></a><a name="zh-cn_topic_0000002543771563_p12481174583018"></a>1字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p95426356101"><a name="zh-cn_topic_0000002543771563_p95426356101"></a><a name="zh-cn_topic_0000002543771563_p95426356101"></a>L1(B1)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p1754253515103"><a name="zh-cn_topic_0000002543771563_p1754253515103"></a><a name="zh-cn_topic_0000002543771563_p1754253515103"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p754243511018"><a name="zh-cn_topic_0000002543771563_p754243511018"></a><a name="zh-cn_topic_0000002543771563_p754243511018"></a>从全局内存加载右矩阵数据到L1</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row195423357103"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p354243561020"><a name="zh-cn_topic_0000002543771563_p354243561020"></a><a name="zh-cn_topic_0000002543771563_p354243561020"></a>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p621204643011"><a name="zh-cn_topic_0000002543771563_p621204643011"></a><a name="zh-cn_topic_0000002543771563_p621204643011"></a>1字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p0542143512102"><a name="zh-cn_topic_0000002543771563_p0542143512102"></a><a name="zh-cn_topic_0000002543771563_p0542143512102"></a>L0A(A2)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p20542143581015"><a name="zh-cn_topic_0000002543771563_p20542143581015"></a><a name="zh-cn_topic_0000002543771563_p20542143581015"></a>512字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p145421235121016"><a name="zh-cn_topic_0000002543771563_p145421235121016"></a><a name="zh-cn_topic_0000002543771563_p145421235121016"></a>从全局内存加载左矩阵数据到L0A</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row154223581017"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p75421835181019"><a name="zh-cn_topic_0000002543771563_p75421835181019"></a><a name="zh-cn_topic_0000002543771563_p75421835181019"></a>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p1458884663012"><a name="zh-cn_topic_0000002543771563_p1458884663012"></a><a name="zh-cn_topic_0000002543771563_p1458884663012"></a>1字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p145421735101015"><a name="zh-cn_topic_0000002543771563_p145421735101015"></a><a name="zh-cn_topic_0000002543771563_p145421735101015"></a>L0B(B2)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p14542113521020"><a name="zh-cn_topic_0000002543771563_p14542113521020"></a><a name="zh-cn_topic_0000002543771563_p14542113521020"></a>512字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p45421335151013"><a name="zh-cn_topic_0000002543771563_p45421335151013"></a><a name="zh-cn_topic_0000002543771563_p45421335151013"></a>从全局内存加载右矩阵数据到L0B</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row1454213357104"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p185428353101"><a name="zh-cn_topic_0000002543771563_p185428353101"></a><a name="zh-cn_topic_0000002543771563_p185428353101"></a>L1(A1)</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p152911815183020"><a name="zh-cn_topic_0000002543771563_p152911815183020"></a><a name="zh-cn_topic_0000002543771563_p152911815183020"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p2543153513109"><a name="zh-cn_topic_0000002543771563_p2543153513109"></a><a name="zh-cn_topic_0000002543771563_p2543153513109"></a>L0A(A2)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p1054333561012"><a name="zh-cn_topic_0000002543771563_p1054333561012"></a><a name="zh-cn_topic_0000002543771563_p1054333561012"></a>512字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p915173761117"><a name="zh-cn_topic_0000002543771563_p915173761117"></a><a name="zh-cn_topic_0000002543771563_p915173761117"></a>从L1加载左矩阵数据到L0A</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row3543173513103"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p11543435131018"><a name="zh-cn_topic_0000002543771563_p11543435131018"></a><a name="zh-cn_topic_0000002543771563_p11543435131018"></a>L1(B1)</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p3291111516302"><a name="zh-cn_topic_0000002543771563_p3291111516302"></a><a name="zh-cn_topic_0000002543771563_p3291111516302"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p054323513106"><a name="zh-cn_topic_0000002543771563_p054323513106"></a><a name="zh-cn_topic_0000002543771563_p054323513106"></a>L0B(B2)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p8543193517105"><a name="zh-cn_topic_0000002543771563_p8543193517105"></a><a name="zh-cn_topic_0000002543771563_p8543193517105"></a>512字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p135431735181015"><a name="zh-cn_topic_0000002543771563_p135431735181015"></a><a name="zh-cn_topic_0000002543771563_p135431735181015"></a>从L1加载右矩阵数据到L0B</p>
</td>
</tr>
</tbody>
</table>

针对Ascend 950PR/Ascend 950DT  ：

**表 2** **数据通路和存储层级**

<a name="zh-cn_topic_0000002543771563_table3316655163516"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002543771563_row1031645583514"><th class="cellrowborder" valign="top" width="16.748325167483248%" id="mcps1.2.6.1.1"><p id="zh-cn_topic_0000002543771563_p5316195515358"><a name="zh-cn_topic_0000002543771563_p5316195515358"></a><a name="zh-cn_topic_0000002543771563_p5316195515358"></a>源位置</p>
</th>
<th class="cellrowborder" valign="top" width="13.65863413658634%" id="mcps1.2.6.1.2"><p id="zh-cn_topic_0000002543771563_p113169556357"><a name="zh-cn_topic_0000002543771563_p113169556357"></a><a name="zh-cn_topic_0000002543771563_p113169556357"></a>源地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="11.868813118688129%" id="mcps1.2.6.1.3"><p id="zh-cn_topic_0000002543771563_p1331611558359"><a name="zh-cn_topic_0000002543771563_p1331611558359"></a><a name="zh-cn_topic_0000002543771563_p1331611558359"></a>目的位置</p>
</th>
<th class="cellrowborder" valign="top" width="17.90820917908209%" id="mcps1.2.6.1.4"><p id="zh-cn_topic_0000002543771563_p3316455143513"><a name="zh-cn_topic_0000002543771563_p3316455143513"></a><a name="zh-cn_topic_0000002543771563_p3316455143513"></a>目的地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="39.816018398160175%" id="mcps1.2.6.1.5"><p id="zh-cn_topic_0000002543771563_p531613556359"><a name="zh-cn_topic_0000002543771563_p531613556359"></a><a name="zh-cn_topic_0000002543771563_p531613556359"></a>典型应用场景</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002543771563_row1316155514353"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p1231625515353"><a name="zh-cn_topic_0000002543771563_p1231625515353"></a><a name="zh-cn_topic_0000002543771563_p1231625515353"></a>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p15316955113510"><a name="zh-cn_topic_0000002543771563_p15316955113510"></a><a name="zh-cn_topic_0000002543771563_p15316955113510"></a>1字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p13316355193514"><a name="zh-cn_topic_0000002543771563_p13316355193514"></a><a name="zh-cn_topic_0000002543771563_p13316355193514"></a>L1(A1)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p131645563519"><a name="zh-cn_topic_0000002543771563_p131645563519"></a><a name="zh-cn_topic_0000002543771563_p131645563519"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p17316125593510"><a name="zh-cn_topic_0000002543771563_p17316125593510"></a><a name="zh-cn_topic_0000002543771563_p17316125593510"></a>从全局内存加载左矩阵数据到L1</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row8316125519355"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p1331610553352"><a name="zh-cn_topic_0000002543771563_p1331610553352"></a><a name="zh-cn_topic_0000002543771563_p1331610553352"></a>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p14316185523511"><a name="zh-cn_topic_0000002543771563_p14316185523511"></a><a name="zh-cn_topic_0000002543771563_p14316185523511"></a>1字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p15316165573511"><a name="zh-cn_topic_0000002543771563_p15316165573511"></a><a name="zh-cn_topic_0000002543771563_p15316165573511"></a>L1(B1)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p3316115510356"><a name="zh-cn_topic_0000002543771563_p3316115510356"></a><a name="zh-cn_topic_0000002543771563_p3316115510356"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p113168554356"><a name="zh-cn_topic_0000002543771563_p113168554356"></a><a name="zh-cn_topic_0000002543771563_p113168554356"></a>从全局内存加载右矩阵数据到L1</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row131745513512"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p4317255153514"><a name="zh-cn_topic_0000002543771563_p4317255153514"></a><a name="zh-cn_topic_0000002543771563_p4317255153514"></a>L1(A1)</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p7317185593518"><a name="zh-cn_topic_0000002543771563_p7317185593518"></a><a name="zh-cn_topic_0000002543771563_p7317185593518"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p1131719554359"><a name="zh-cn_topic_0000002543771563_p1131719554359"></a><a name="zh-cn_topic_0000002543771563_p1131719554359"></a>L0A(A2)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p113171755183513"><a name="zh-cn_topic_0000002543771563_p113171755183513"></a><a name="zh-cn_topic_0000002543771563_p113171755183513"></a>512字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p153171755183511"><a name="zh-cn_topic_0000002543771563_p153171755183511"></a><a name="zh-cn_topic_0000002543771563_p153171755183511"></a>从L1加载左矩阵数据到L0A</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row13317255173513"><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.2.6.1.1 "><p id="zh-cn_topic_0000002543771563_p331765523516"><a name="zh-cn_topic_0000002543771563_p331765523516"></a><a name="zh-cn_topic_0000002543771563_p331765523516"></a>L1(B1)</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.2.6.1.2 "><p id="zh-cn_topic_0000002543771563_p93171555143516"><a name="zh-cn_topic_0000002543771563_p93171555143516"></a><a name="zh-cn_topic_0000002543771563_p93171555143516"></a>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.2.6.1.3 "><p id="zh-cn_topic_0000002543771563_p1831765516355"><a name="zh-cn_topic_0000002543771563_p1831765516355"></a><a name="zh-cn_topic_0000002543771563_p1831765516355"></a>L0B(B2)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.2.6.1.4 "><p id="zh-cn_topic_0000002543771563_p431755513355"><a name="zh-cn_topic_0000002543771563_p431755513355"></a><a name="zh-cn_topic_0000002543771563_p431755513355"></a>512字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.2.6.1.5 "><p id="zh-cn_topic_0000002543771563_p631765563511"><a name="zh-cn_topic_0000002543771563_p631765563511"></a><a name="zh-cn_topic_0000002543771563_p631765563511"></a>从L1加载右矩阵数据到L0B</p>
</td>
</tr>
</tbody>
</table>

针对Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品  ：

**表 3** **接口分类与使用场景**

<a name="zh-cn_topic_0000002543771563_table4531205110133"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002543771563_row45451951101311"><th class="cellrowborder" valign="top" width="24.98%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002543771563_p8545145181318"><a name="zh-cn_topic_0000002543771563_p8545145181318"></a><a name="zh-cn_topic_0000002543771563_p8545145181318"></a>接口类别</p>
</th>
<th class="cellrowborder" valign="top" width="25.019999999999996%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002543771563_p554525131315"><a name="zh-cn_topic_0000002543771563_p554525131315"></a><a name="zh-cn_topic_0000002543771563_p554525131315"></a>主要功能</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002543771563_p1954575191311"><a name="zh-cn_topic_0000002543771563_p1954575191311"></a><a name="zh-cn_topic_0000002543771563_p1954575191311"></a>支持通路</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002543771563_p4545251191316"><a name="zh-cn_topic_0000002543771563_p4545251191316"></a><a name="zh-cn_topic_0000002543771563_p4545251191316"></a>典型应用场景</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002543771563_row1635265117401"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p73521851154011"><a name="zh-cn_topic_0000002543771563_p73521851154011"></a><a name="zh-cn_topic_0000002543771563_p73521851154011"></a><a href="GM--L1随路转换-ND2NZ搬运(DataCopy)-57.md#ZH-CN_TOPIC_0000002569070913">GM-&gt;L1随路转换-ND2NZ搬运(DataCopy)</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p435245154018"><a name="zh-cn_topic_0000002543771563_p435245154018"></a><a name="zh-cn_topic_0000002543771563_p435245154018"></a>二维数据高效传输，同时进行随路ND到NZ格式的转换</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p1035295118405"><a name="zh-cn_topic_0000002543771563_p1035295118405"></a><a name="zh-cn_topic_0000002543771563_p1035295118405"></a>GM-&gt;L1</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p183521851124010"><a name="zh-cn_topic_0000002543771563_p183521851124010"></a><a name="zh-cn_topic_0000002543771563_p183521851124010"></a>随路进行ND到NZ转换的矩阵数据搬运</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row105453518135"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p1954517514134"><a name="zh-cn_topic_0000002543771563_p1954517514134"></a><a name="zh-cn_topic_0000002543771563_p1954517514134"></a><a href="Load2D.md#ZH-CN_TOPIC_0000002569070923">Load2D</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p12545351121319"><a name="zh-cn_topic_0000002543771563_p12545351121319"></a><a name="zh-cn_topic_0000002543771563_p12545351121319"></a>二维数据高效传输</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p15454518137"><a name="zh-cn_topic_0000002543771563_p15454518137"></a><a name="zh-cn_topic_0000002543771563_p15454518137"></a>GM-&gt;L1,GM-&gt;L0A/L0B, L1-&gt;L0A/L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p175455514136"><a name="zh-cn_topic_0000002543771563_p175455514136"></a><a name="zh-cn_topic_0000002543771563_p175455514136"></a>普通矩阵数据搬运、分形矩阵处理</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row9545175131316"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p1154514514132"><a name="zh-cn_topic_0000002543771563_p1154514514132"></a><a name="zh-cn_topic_0000002543771563_p1154514514132"></a><a href="Load3D.md#ZH-CN_TOPIC_0000002538071224">Load3D</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p2545155111133"><a name="zh-cn_topic_0000002543771563_p2545155111133"></a><a name="zh-cn_topic_0000002543771563_p2545155111133"></a>Image to Column转换及数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p195450516138"><a name="zh-cn_topic_0000002543771563_p195450516138"></a><a name="zh-cn_topic_0000002543771563_p195450516138"></a>L1-&gt;L0A/L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p1545105119133"><a name="zh-cn_topic_0000002543771563_p1545105119133"></a><a name="zh-cn_topic_0000002543771563_p1545105119133"></a>卷积运算、特征图展开</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row954505110137"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p1654514512137"><a name="zh-cn_topic_0000002543771563_p1654514512137"></a><a name="zh-cn_topic_0000002543771563_p1654514512137"></a><a href="LoadDataWithTranspose.md#ZH-CN_TOPIC_0000002538231154">LoadDataWithTranspose</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p954511515139"><a name="zh-cn_topic_0000002543771563_p954511515139"></a><a name="zh-cn_topic_0000002543771563_p954511515139"></a>带转置的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p5545185119131"><a name="zh-cn_topic_0000002543771563_p5545185119131"></a><a name="zh-cn_topic_0000002543771563_p5545185119131"></a>L1-&gt;L0A/L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p2545155111314"><a name="zh-cn_topic_0000002543771563_p2545155111314"></a><a name="zh-cn_topic_0000002543771563_p2545155111314"></a>普通矩阵计算中转置数据</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row19545851161317"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p20545151141313"><a name="zh-cn_topic_0000002543771563_p20545151141313"></a><a name="zh-cn_topic_0000002543771563_p20545151141313"></a><a href="LoadDataWithSparse.md#ZH-CN_TOPIC_0000002568950935">LoadDataWithSparse</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p4545451101320"><a name="zh-cn_topic_0000002543771563_p4545451101320"></a><a name="zh-cn_topic_0000002543771563_p4545451101320"></a>稀疏化数据处理</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p10545175110133"><a name="zh-cn_topic_0000002543771563_p10545175110133"></a><a name="zh-cn_topic_0000002543771563_p10545175110133"></a>L1-&gt;L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p1954510518133"><a name="zh-cn_topic_0000002543771563_p1954510518133"></a><a name="zh-cn_topic_0000002543771563_p1954510518133"></a>稀疏矩阵乘法、权重剪枝场景</p>
</td>
</tr>
</tbody>
</table>

针对Ascend 950PR/Ascend 950DT  ：

**表 4** **接口分类与使用场景**

<a name="zh-cn_topic_0000002543771563_table18501247192710"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002543771563_row1685034716277"><th class="cellrowborder" valign="top" width="24.98%" id="mcps1.2.5.1.1"><p id="zh-cn_topic_0000002543771563_p3850247122718"><a name="zh-cn_topic_0000002543771563_p3850247122718"></a><a name="zh-cn_topic_0000002543771563_p3850247122718"></a>接口类别</p>
</th>
<th class="cellrowborder" valign="top" width="25.019999999999996%" id="mcps1.2.5.1.2"><p id="zh-cn_topic_0000002543771563_p18850144719277"><a name="zh-cn_topic_0000002543771563_p18850144719277"></a><a name="zh-cn_topic_0000002543771563_p18850144719277"></a>主要功能</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p id="zh-cn_topic_0000002543771563_p12850154722718"><a name="zh-cn_topic_0000002543771563_p12850154722718"></a><a name="zh-cn_topic_0000002543771563_p12850154722718"></a>支持通路</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p id="zh-cn_topic_0000002543771563_p6850154719274"><a name="zh-cn_topic_0000002543771563_p6850154719274"></a><a name="zh-cn_topic_0000002543771563_p6850154719274"></a>典型应用场景</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002543771563_row15850347162716"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p198512475279"><a name="zh-cn_topic_0000002543771563_p198512475279"></a><a name="zh-cn_topic_0000002543771563_p198512475279"></a><a href="GM--L1随路转换-ND2NZ搬运(DataCopy)-57.md#ZH-CN_TOPIC_0000002569070913">GM-&gt;L1随路转换-ND2NZ搬运(DataCopy)</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p158514479273"><a name="zh-cn_topic_0000002543771563_p158514479273"></a><a name="zh-cn_topic_0000002543771563_p158514479273"></a>二维数据高效传输，同时进行随路ND到NZ格式的转换</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p585194716279"><a name="zh-cn_topic_0000002543771563_p585194716279"></a><a name="zh-cn_topic_0000002543771563_p585194716279"></a>GM-&gt;L1</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p1285134715278"><a name="zh-cn_topic_0000002543771563_p1285134715278"></a><a name="zh-cn_topic_0000002543771563_p1285134715278"></a>随路进行ND到NZ转换的矩阵数据搬运</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row8851104722715"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p78516479271"><a name="zh-cn_topic_0000002543771563_p78516479271"></a><a name="zh-cn_topic_0000002543771563_p78516479271"></a><a href="Load2D.md#ZH-CN_TOPIC_0000002569070923">Load2D</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p685112471279"><a name="zh-cn_topic_0000002543771563_p685112471279"></a><a name="zh-cn_topic_0000002543771563_p685112471279"></a>二维数据高效传输</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p4851114711274"><a name="zh-cn_topic_0000002543771563_p4851114711274"></a><a name="zh-cn_topic_0000002543771563_p4851114711274"></a>GM-&gt;L1, L1-&gt;L0A/L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p485118479271"><a name="zh-cn_topic_0000002543771563_p485118479271"></a><a name="zh-cn_topic_0000002543771563_p485118479271"></a>普通矩阵数据搬运、分形矩阵处理</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row9851247102712"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p98519473273"><a name="zh-cn_topic_0000002543771563_p98519473273"></a><a name="zh-cn_topic_0000002543771563_p98519473273"></a><a href="Load2DV2.md#ZH-CN_TOPIC_0000002563687926">Load2DV2</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p98514470270"><a name="zh-cn_topic_0000002543771563_p98514470270"></a><a name="zh-cn_topic_0000002543771563_p98514470270"></a>二维数据高效传输</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p9851247152710"><a name="zh-cn_topic_0000002543771563_p9851247152710"></a><a name="zh-cn_topic_0000002543771563_p9851247152710"></a>GM-&gt;L1,L1-&gt;L0A/L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p178511847112712"><a name="zh-cn_topic_0000002543771563_p178511847112712"></a><a name="zh-cn_topic_0000002543771563_p178511847112712"></a>普通矩阵数据搬运、分形矩阵处理</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row58511470274"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p128511247142712"><a name="zh-cn_topic_0000002543771563_p128511247142712"></a><a name="zh-cn_topic_0000002543771563_p128511247142712"></a><a href="Load3D.md#ZH-CN_TOPIC_0000002538071224">Load3D</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p485124714276"><a name="zh-cn_topic_0000002543771563_p485124714276"></a><a name="zh-cn_topic_0000002543771563_p485124714276"></a>Image to Column转换及数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p1185164762714"><a name="zh-cn_topic_0000002543771563_p1185164762714"></a><a name="zh-cn_topic_0000002543771563_p1185164762714"></a>L1-&gt;L0A/L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p5851114710277"><a name="zh-cn_topic_0000002543771563_p5851114710277"></a><a name="zh-cn_topic_0000002543771563_p5851114710277"></a>卷积运算、特征图展开</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row185194712713"><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p id="zh-cn_topic_0000002543771563_p685144752714"><a name="zh-cn_topic_0000002543771563_p685144752714"></a><a name="zh-cn_topic_0000002543771563_p685144752714"></a><a href="LoadDataWithTranspose.md#ZH-CN_TOPIC_0000002538231154">LoadDataWithTranspose</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p id="zh-cn_topic_0000002543771563_p1385164742712"><a name="zh-cn_topic_0000002543771563_p1385164742712"></a><a name="zh-cn_topic_0000002543771563_p1385164742712"></a>带转置的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p id="zh-cn_topic_0000002543771563_p685112475278"><a name="zh-cn_topic_0000002543771563_p685112475278"></a><a name="zh-cn_topic_0000002543771563_p685112475278"></a>L1-&gt;L0A/L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p id="zh-cn_topic_0000002543771563_p1585112475272"><a name="zh-cn_topic_0000002543771563_p1585112475272"></a><a name="zh-cn_topic_0000002543771563_p1585112475272"></a>普通矩阵计算中转置数据</p>
</td>
</tr>
</tbody>
</table>

**表 5** **辅助配置接口说明**

<a name="zh-cn_topic_0000002543771563_table107953337145"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002543771563_row480613381418"><th class="cellrowborder" valign="top" width="19.383838383838384%" id="mcps1.2.4.1.1"><p id="zh-cn_topic_0000002543771563_p6806113331415"><a name="zh-cn_topic_0000002543771563_p6806113331415"></a><a name="zh-cn_topic_0000002543771563_p6806113331415"></a>接口名称</p>
</th>
<th class="cellrowborder" valign="top" width="47.28282828282829%" id="mcps1.2.4.1.2"><p id="zh-cn_topic_0000002543771563_p138061033151411"><a name="zh-cn_topic_0000002543771563_p138061033151411"></a><a name="zh-cn_topic_0000002543771563_p138061033151411"></a>功能说明</p>
</th>
<th class="cellrowborder" valign="top" width="33.333333333333336%" id="mcps1.2.4.1.3"><p id="zh-cn_topic_0000002543771563_p580613311142"><a name="zh-cn_topic_0000002543771563_p580613311142"></a><a name="zh-cn_topic_0000002543771563_p580613311142"></a>配合使用的接口</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002543771563_row1064183214713"><td class="cellrowborder" valign="top" width="19.383838383838384%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002543771563_p9652324717"><a name="zh-cn_topic_0000002543771563_p9652324717"></a><a name="zh-cn_topic_0000002543771563_p9652324717"></a><a href="Fill.md#ZH-CN_TOPIC_0000002538231174">Fill</a></p>
</td>
<td class="cellrowborder" valign="top" width="47.28282828282829%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002543771563_p15651532771"><a name="zh-cn_topic_0000002543771563_p15651532771"></a><a name="zh-cn_topic_0000002543771563_p15651532771"></a>初始化设置L1/L0A/L0B的值</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002543771563_p56515321715"><a name="zh-cn_topic_0000002543771563_p56515321715"></a><a name="zh-cn_topic_0000002543771563_p56515321715"></a>/</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row208061133201416"><td class="cellrowborder" valign="top" width="19.383838383838384%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002543771563_p6806143310140"><a name="zh-cn_topic_0000002543771563_p6806143310140"></a><a name="zh-cn_topic_0000002543771563_p6806143310140"></a><a href="SetFmatrix.md#ZH-CN_TOPIC_0000002538071250">SetFmatrix</a></p>
</td>
<td class="cellrowborder" valign="top" width="47.28282828282829%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002543771563_p380613319144"><a name="zh-cn_topic_0000002543771563_p380613319144"></a><a name="zh-cn_topic_0000002543771563_p380613319144"></a>设置FeatureMap属性描述</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002543771563_p167931848154710"><a name="zh-cn_topic_0000002543771563_p167931848154710"></a><a name="zh-cn_topic_0000002543771563_p167931848154710"></a>Load3D</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row17806433111416"><td class="cellrowborder" valign="top" width="19.383838383838384%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002543771563_p98061733161416"><a name="zh-cn_topic_0000002543771563_p98061733161416"></a><a name="zh-cn_topic_0000002543771563_p98061733161416"></a><a href="SetLoadDataBoundary.md#ZH-CN_TOPIC_0000002569070957">SetLoadDataBoundary</a></p>
</td>
<td class="cellrowborder" valign="top" width="47.28282828282829%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002543771563_p1680619334144"><a name="zh-cn_topic_0000002543771563_p1680619334144"></a><a name="zh-cn_topic_0000002543771563_p1680619334144"></a>设置L1边界值</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002543771563_p380618339142"><a name="zh-cn_topic_0000002543771563_p380618339142"></a><a name="zh-cn_topic_0000002543771563_p380618339142"></a>Load3D</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row1480610333143"><td class="cellrowborder" valign="top" width="19.383838383838384%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002543771563_p1680612334142"><a name="zh-cn_topic_0000002543771563_p1680612334142"></a><a name="zh-cn_topic_0000002543771563_p1680612334142"></a><a href="SetLoadDataRepeat.md#ZH-CN_TOPIC_0000002568950953">SetLoadDataRepeat</a></p>
</td>
<td class="cellrowborder" valign="top" width="47.28282828282829%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002543771563_p1780613334149"><a name="zh-cn_topic_0000002543771563_p1780613334149"></a><a name="zh-cn_topic_0000002543771563_p1780613334149"></a>设置Repeat参数实现多迭代搬运</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002543771563_p117585412475"><a name="zh-cn_topic_0000002543771563_p117585412475"></a><a name="zh-cn_topic_0000002543771563_p117585412475"></a>Load3D</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002543771563_row180683317148"><td class="cellrowborder" valign="top" width="19.383838383838384%" headers="mcps1.2.4.1.1 "><p id="zh-cn_topic_0000002543771563_p18806333131415"><a name="zh-cn_topic_0000002543771563_p18806333131415"></a><a name="zh-cn_topic_0000002543771563_p18806333131415"></a><a href="SetLoadDataPaddingValue.md#ZH-CN_TOPIC_0000002538231176">SetLoadDataPaddingValue</a></p>
</td>
<td class="cellrowborder" valign="top" width="47.28282828282829%" headers="mcps1.2.4.1.2 "><p id="zh-cn_topic_0000002543771563_p180693317148"><a name="zh-cn_topic_0000002543771563_p180693317148"></a><a name="zh-cn_topic_0000002543771563_p180693317148"></a>设置Padding填充值</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.2.4.1.3 "><p id="zh-cn_topic_0000002543771563_p58522564471"><a name="zh-cn_topic_0000002543771563_p58522564471"></a><a name="zh-cn_topic_0000002543771563_p58522564471"></a>Load3D</p>
</td>
</tr>
</tbody>
</table>

**表 6** **Atlas A2 训练系列产品/Atlas A2 推理系列产品  和  Atlas A3 训练系列产品/Atlas A3 推理系列产品  产品接口支持情况**

<a name="zh-cn_topic_0000002543771563_table1836012121514"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002543771563_row13790291510"><th class="cellrowborder" valign="top" width="18.48184818481848%" id="mcps1.2.7.1.1"><p id="zh-cn_topic_0000002543771563_p16379428159"><a name="zh-cn_topic_0000002543771563_p16379428159"></a><a name="zh-cn_topic_0000002543771563_p16379428159"></a>产品系列</p>
</th>
<th class="cellrowborder" valign="top" width="16.011601160116008%" id="mcps1.2.7.1.2"><p id="zh-cn_topic_0000002543771563_p1448516266210"><a name="zh-cn_topic_0000002543771563_p1448516266210"></a><a name="zh-cn_topic_0000002543771563_p1448516266210"></a>DataCopy(GM-&gt;L1)</p>
</th>
<th class="cellrowborder" valign="top" width="10.011001100110011%" id="mcps1.2.7.1.3"><p id="zh-cn_topic_0000002543771563_p17379152171511"><a name="zh-cn_topic_0000002543771563_p17379152171511"></a><a name="zh-cn_topic_0000002543771563_p17379152171511"></a>Load2D</p>
</th>
<th class="cellrowborder" valign="top" width="9.700970097009701%" id="mcps1.2.7.1.4"><p id="zh-cn_topic_0000002543771563_p337922171517"><a name="zh-cn_topic_0000002543771563_p337922171517"></a><a name="zh-cn_topic_0000002543771563_p337922171517"></a>Load3D</p>
</th>
<th class="cellrowborder" valign="top" width="27.29272927292729%" id="mcps1.2.7.1.5"><p id="zh-cn_topic_0000002543771563_p14379924153"><a name="zh-cn_topic_0000002543771563_p14379924153"></a><a name="zh-cn_topic_0000002543771563_p14379924153"></a>LoadDataWithTranspose</p>
</th>
<th class="cellrowborder" valign="top" width="18.5018501850185%" id="mcps1.2.7.1.6"><p id="zh-cn_topic_0000002543771563_p9379172151515"><a name="zh-cn_topic_0000002543771563_p9379172151515"></a><a name="zh-cn_topic_0000002543771563_p9379172151515"></a>LoadDataWithSparse</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002543771563_row20380223153"><td class="cellrowborder" valign="top" width="18.48184818481848%" headers="mcps1.2.7.1.1 "><p id="zh-cn_topic_0000002543771563_p183803218153"><a name="zh-cn_topic_0000002543771563_p183803218153"></a><a name="zh-cn_topic_0000002543771563_p183803218153"></a>Atlas A2/A3系列</p>
</td>
<td class="cellrowborder" valign="top" width="16.011601160116008%" headers="mcps1.2.7.1.2 "><p id="zh-cn_topic_0000002543771563_p148582662112"><a name="zh-cn_topic_0000002543771563_p148582662112"></a><a name="zh-cn_topic_0000002543771563_p148582662112"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="10.011001100110011%" headers="mcps1.2.7.1.3 "><p id="zh-cn_topic_0000002543771563_p53801626151"><a name="zh-cn_topic_0000002543771563_p53801626151"></a><a name="zh-cn_topic_0000002543771563_p53801626151"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="9.700970097009701%" headers="mcps1.2.7.1.4 "><p id="zh-cn_topic_0000002543771563_p2038015210152"><a name="zh-cn_topic_0000002543771563_p2038015210152"></a><a name="zh-cn_topic_0000002543771563_p2038015210152"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="27.29272927292729%" headers="mcps1.2.7.1.5 "><p id="zh-cn_topic_0000002543771563_p838018211155"><a name="zh-cn_topic_0000002543771563_p838018211155"></a><a name="zh-cn_topic_0000002543771563_p838018211155"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="18.5018501850185%" headers="mcps1.2.7.1.6 "><p id="zh-cn_topic_0000002543771563_p1380172161516"><a name="zh-cn_topic_0000002543771563_p1380172161516"></a><a name="zh-cn_topic_0000002543771563_p1380172161516"></a>√</p>
</td>
</tr>
</tbody>
</table>

**表 7** **Ascend 950PR/Ascend 950DT产品接口支持情况**

<a name="zh-cn_topic_0000002543771563_table873163514294"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002543771563_row2731135162910"><th class="cellrowborder" valign="top" width="19.07%" id="mcps1.2.8.1.1"><p id="zh-cn_topic_0000002543771563_p6731735112913"><a name="zh-cn_topic_0000002543771563_p6731735112913"></a><a name="zh-cn_topic_0000002543771563_p6731735112913"></a>产品系列</p>
</th>
<th class="cellrowborder" valign="top" width="15.07%" id="mcps1.2.8.1.2"><p id="zh-cn_topic_0000002543771563_p1373133582916"><a name="zh-cn_topic_0000002543771563_p1373133582916"></a><a name="zh-cn_topic_0000002543771563_p1373133582916"></a>DataCopy(GM-&gt;L1)</p>
</th>
<th class="cellrowborder" valign="top" width="8.459999999999999%" id="mcps1.2.8.1.3"><p id="zh-cn_topic_0000002543771563_p127311735112915"><a name="zh-cn_topic_0000002543771563_p127311735112915"></a><a name="zh-cn_topic_0000002543771563_p127311735112915"></a>Load2D</p>
</th>
<th class="cellrowborder" valign="top" width="9.19%" id="mcps1.2.8.1.4"><p id="zh-cn_topic_0000002543771563_p1731133562911"><a name="zh-cn_topic_0000002543771563_p1731133562911"></a><a name="zh-cn_topic_0000002543771563_p1731133562911"></a>Load2DV2</p>
</th>
<th class="cellrowborder" valign="top" width="9.16%" id="mcps1.2.8.1.5"><p id="zh-cn_topic_0000002543771563_p169486251284"><a name="zh-cn_topic_0000002543771563_p169486251284"></a><a name="zh-cn_topic_0000002543771563_p169486251284"></a>Load2DMX</p>
</th>
<th class="cellrowborder" valign="top" width="8.68%" id="mcps1.2.8.1.6"><p id="zh-cn_topic_0000002543771563_p873111350297"><a name="zh-cn_topic_0000002543771563_p873111350297"></a><a name="zh-cn_topic_0000002543771563_p873111350297"></a>Load3D</p>
</th>
<th class="cellrowborder" valign="top" width="30.37%" id="mcps1.2.8.1.7"><p id="zh-cn_topic_0000002543771563_p673113355293"><a name="zh-cn_topic_0000002543771563_p673113355293"></a><a name="zh-cn_topic_0000002543771563_p673113355293"></a>LoadDataWithTranspose</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002543771563_row13732123502919"><td class="cellrowborder" valign="top" width="19.07%" headers="mcps1.2.8.1.1 "><p id="zh-cn_topic_0000002543771563_p384292083111"><a name="zh-cn_topic_0000002543771563_p384292083111"></a><a name="zh-cn_topic_0000002543771563_p384292083111"></a>Ascend 950系列</p>
</td>
<td class="cellrowborder" valign="top" width="15.07%" headers="mcps1.2.8.1.2 "><p id="zh-cn_topic_0000002543771563_p1573223510290"><a name="zh-cn_topic_0000002543771563_p1573223510290"></a><a name="zh-cn_topic_0000002543771563_p1573223510290"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="8.459999999999999%" headers="mcps1.2.8.1.3 "><p id="zh-cn_topic_0000002543771563_p3732173515293"><a name="zh-cn_topic_0000002543771563_p3732173515293"></a><a name="zh-cn_topic_0000002543771563_p3732173515293"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="9.19%" headers="mcps1.2.8.1.4 "><p id="zh-cn_topic_0000002543771563_p157321535112915"><a name="zh-cn_topic_0000002543771563_p157321535112915"></a><a name="zh-cn_topic_0000002543771563_p157321535112915"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="9.16%" headers="mcps1.2.8.1.5 "><p id="zh-cn_topic_0000002543771563_p13540053142816"><a name="zh-cn_topic_0000002543771563_p13540053142816"></a><a name="zh-cn_topic_0000002543771563_p13540053142816"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="8.68%" headers="mcps1.2.8.1.6 "><p id="zh-cn_topic_0000002543771563_p2732173542919"><a name="zh-cn_topic_0000002543771563_p2732173542919"></a><a name="zh-cn_topic_0000002543771563_p2732173542919"></a>√</p>
</td>
<td class="cellrowborder" valign="top" width="30.37%" headers="mcps1.2.8.1.7 "><p id="zh-cn_topic_0000002543771563_p157321435172920"><a name="zh-cn_topic_0000002543771563_p157321435172920"></a><a name="zh-cn_topic_0000002543771563_p157321435172920"></a>√</p>
</td>
</tr>
</tbody>
</table>


