# UnitFlag<a name="ZH-CN_TOPIC_0000002538071264"></a>

**特性说明：**

unitFlag的核心功能体现为：Mmad 和 FixPipe接口 引入了单元标志（unit-flag）机制，通过以内存块为粒度实现精细化的数据同步，从而有效降低同步延迟，提升系统整体性能。当UnitFlag开关打开后， 对于L0C中的每个内存块（512B），提供一个单元标志位，用于指示该块是否可读或可写。

Mmad和Fixpipe接口设置unitFlag值为2/3后，系统会启动单元标志位。

当mmadParams.unitFlag == 2（0b10）, 使能unitFlag功能，在硬件执行完指令后，不改变单元标志位；

-   对于写操作\(Mmad接口\)， 如果单元标志位0，则硬件直接写入L0C；否则，如果单元标志位为1， 则写操作会等待直到单元标志变为0；执行完成后将单元标志位保持为0；
-   对于读操作\(Fixpipe接口\)，如果单元标志位1，则硬件直接读取L0C;  否则,  如果单元标志位为0，则读操作为等待直到单元标志变为1；执行完成后将单元标志位保持为1；

当mmadParams.unitFlag == 3（0b11）, 使能unitFlag功能，在硬件执行完指令后，改变单元标志位；

-   对于写操作\(Mmad接口\)， 如果单元标志位0，则硬件直接写入L0C；否则，如果单元标志位为1， 则写操作会等待直到单元标志变为0；执行完成后将单元标志位设置成1；
-   对于读操作\(Fixpipe接口\)，如果单元标志位1，则硬件直接读取L0C;  否则,  如果单元标志位为0，则读操作为等待直到单元标志变为1；执行完成后将单元标志位设置成0；

根据上述特性，如果用户在进行A矩阵\[128, 1024\], B矩阵为\[1024, 128\]的矩阵乘计算时，需要沿着K轴进行迭代循环， 假设每次迭代K长度为128，则需要迭代8次，此时8次Mmad对应1次Fixpipe操作；

-   前7次Mmad的unitFlag都设置成2， 写入后将单元标记位始终为0， 保证后续Mmad可以写入L0C；
-   最后1次Mmad设置成3，写入后将单元标志位设置成1， 保证Fixpipe可以读取L0C；
-   Fixpipe的unitFlag设置为3，读取后将单元标记位设置为0，保证后续Mmad接口可以顺利写入L0C数据；

如果用户需要单次Mmad的结果分多次搬出时, 譬如Mmad计算结果的L0C为M\(128\) x N\(256\), 沿N轴分两次搬出；这样一次Mmad会对应两次Fixpipe；

-   Mmad的时候需要设置unitFlag = 3，保证Fixpipe可以读取L0C数据；
-   每一次Fixpipe的unitFlag都设置为3，读取后将单元标记位设置为0，保证后续其他Mmad接口在复用这块L0C地址时可以顺利写入数据；

当开启 \`unitFlag\` 后，\`Mmad\` 和 \`Fixpipe\` 会对同一块分形的 \`L0C\` 进行读写操作，因此 \`Mmad\` 计算和 \`Fixpipe\` 保持一致的读写顺序，有助于获得更优的性能表现。

在调用 \`Mmad\` 接口时，需要通过 \`**[SetMMColumnMajor/SetMMRowMajor](SetMMColumnMajor-SetMMRowMajor.md#ZH-CN_TOPIC_0000002538231190)**\` 接口设置 \`Mmad\` 的计算方向。当 \`Fixpipe\` 使能了 \`NZ2ND\` 或 \`ChannelMerge\` 等 layout 变换时，需将 \`Mmad\` 的计算方向设置为 N 方向优先，即调用 \`SetMMRowMajor\(\)\`；反之，若未使用这些特性，则应将计算方向设置为M 方向优先，即调用 \`SetMMColumnMajor\(\)\`。

**图 1**  Mmad和Fixpipe同时沿M方向写/读<a name="zh-cn_topic_0000002512241020_fig94721733376"></a>  
![](figures/Mmad和Fixpipe同时沿M方向写-读.png "Mmad和Fixpipe同时沿M方向写-读")

**特性约束：**

-   Mmad 和 Fixpipe 接口均提供了 \`unitFlag\` 参数来控制该功能的启用，需确保两者同步开启，才能正常生效。
-   当希望控制同一块L0C内存空间能持续只被多条Mmad或多条Fixpipe指令操作时，需将对应的前n-1条指令的unitFlag值设置为2，维持被操作内存空间的持续占用状态，最后一条指令设置为3，解除被占用状态。
-   当启用 \`unitFlag\` 功能后，建议Mmad 的计算数据量与 Fixpipe 搬出的数据量保持一致。若 Mmad 计算了大块数据（M × N = 128 × 128），但 Fixpipe 只搬出了其中一部分数据（M × N = 64 × 64），则可能会导致执行异常，可以通过SetFixPipeConfig\(\)接口重置L0C的状态，详细操作方式见下方示例。

**沿K轴迭代循环使用示例片段:**

```
// 调用kRound次Mmad
for (auto kIndex = 0; kIndex = kRound; ++kIndex) {
    if (kIndex != kRound - 1) {
        // 前kRound-1次迭代设置为2，保证Mmad在K迭代循环中可以一直写入L0C
        mmadParams.unitFlag = 2;
    } else {
        // 最后一次迭代设置为3，将单元标志位设成1, 保证Fixpipe可以读L0C
        mmadParams.unitFlag = 3;
    }
    AscendC::Mmad(c, a, b, mmadParams);
    if ((m / ALIGN_NUM) * (n / ALIGN_NUM) < LIMIT_MNSIZE) {
        AscendC::PipeBarrier<PIPE_M>();
    }
}
// Fixpipe一次搬出
// Fixpipe的unitFlag设置为3，读取后将单元标记位设置为0，保证后续Mmad接口可以顺利写入L0C数据
fixpipeParams.unitFlag = 3;
AscendC::Fixpipe(cGM, c, fixpipeParams);
```

开启与不开启unitFlag功能的完整示例：[UnitFlag示例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_unitflag)

**表 1**  性能示例说明（以输入数据类型half为例，无业务实测表示忽略指令的前后序操作，只考虑单指令性能，包含带宽延迟等开销）

<a name="zh-cn_topic_0000002512241020_table771513523122"></a>
<table><thead align="left"><tr id="zh-cn_topic_0000002512241020_row13716452101216"><th class="cellrowborder" valign="top" width="7.820782078207822%" id="mcps1.2.9.1.1"><p id="zh-cn_topic_0000002512241020_p87161528121"><a name="zh-cn_topic_0000002512241020_p87161528121"></a><a name="zh-cn_topic_0000002512241020_p87161528121"></a>M</p>
</th>
<th class="cellrowborder" valign="top" width="7.940794079407941%" id="mcps1.2.9.1.2"><p id="zh-cn_topic_0000002512241020_p2716195201216"><a name="zh-cn_topic_0000002512241020_p2716195201216"></a><a name="zh-cn_topic_0000002512241020_p2716195201216"></a>N</p>
</th>
<th class="cellrowborder" valign="top" width="7.510751075107512%" id="mcps1.2.9.1.3"><p id="zh-cn_topic_0000002512241020_p471619528122"><a name="zh-cn_topic_0000002512241020_p471619528122"></a><a name="zh-cn_topic_0000002512241020_p471619528122"></a>K</p>
</th>
<th class="cellrowborder" valign="top" width="12.651265126512653%" id="mcps1.2.9.1.4"><p id="zh-cn_topic_0000002512241020_p127161452171212"><a name="zh-cn_topic_0000002512241020_p127161452171212"></a><a name="zh-cn_topic_0000002512241020_p127161452171212"></a>UnitFlag</p>
</th>
<th class="cellrowborder" valign="top" width="16.821682168216824%" id="mcps1.2.9.1.5"><p id="zh-cn_topic_0000002512241020_p10716145211217"><a name="zh-cn_topic_0000002512241020_p10716145211217"></a><a name="zh-cn_topic_0000002512241020_p10716145211217"></a>LOOP_COUNT</p>
</th>
<th class="cellrowborder" valign="top" width="17.681768176817688%" id="mcps1.2.9.1.6"><p id="zh-cn_topic_0000002512241020_p197619051519"><a name="zh-cn_topic_0000002512241020_p197619051519"></a><a name="zh-cn_topic_0000002512241020_p197619051519"></a>Mmad理论值(cycle)</p>
</th>
<th class="cellrowborder" valign="top" width="17.47174717471747%" id="mcps1.2.9.1.7"><p id="zh-cn_topic_0000002512241020_p27161752131211"><a name="zh-cn_topic_0000002512241020_p27161752131211"></a><a name="zh-cn_topic_0000002512241020_p27161752131211"></a>Mmad无业务实测值（cycle）</p>
</th>
<th class="cellrowborder" valign="top" width="12.101210121012102%" id="mcps1.2.9.1.8"><p id="zh-cn_topic_0000002512241020_p65661419155412"><a name="zh-cn_topic_0000002512241020_p65661419155412"></a><a name="zh-cn_topic_0000002512241020_p65661419155412"></a>Fixpipe实测值(cycle)</p>
</th>
</tr>
</thead>
<tbody><tr id="zh-cn_topic_0000002512241020_row107161152171219"><td class="cellrowborder" valign="top" width="7.820782078207822%" headers="mcps1.2.9.1.1 "><p id="zh-cn_topic_0000002512241020_p1771695210126"><a name="zh-cn_topic_0000002512241020_p1771695210126"></a><a name="zh-cn_topic_0000002512241020_p1771695210126"></a>128</p>
</td>
<td class="cellrowborder" valign="top" width="7.940794079407941%" headers="mcps1.2.9.1.2 "><p id="zh-cn_topic_0000002512241020_p57166526129"><a name="zh-cn_topic_0000002512241020_p57166526129"></a><a name="zh-cn_topic_0000002512241020_p57166526129"></a>256</p>
</td>
<td class="cellrowborder" valign="top" width="7.510751075107512%" headers="mcps1.2.9.1.3 "><p id="zh-cn_topic_0000002512241020_p371611526124"><a name="zh-cn_topic_0000002512241020_p371611526124"></a><a name="zh-cn_topic_0000002512241020_p371611526124"></a>512</p>
</td>
<td class="cellrowborder" valign="top" width="12.651265126512653%" headers="mcps1.2.9.1.4 "><p id="zh-cn_topic_0000002512241020_p17716552141213"><a name="zh-cn_topic_0000002512241020_p17716552141213"></a><a name="zh-cn_topic_0000002512241020_p17716552141213"></a>Enable</p>
</td>
<td class="cellrowborder" valign="top" width="16.821682168216824%" headers="mcps1.2.9.1.5 "><p id="zh-cn_topic_0000002512241020_p1371665291218"><a name="zh-cn_topic_0000002512241020_p1371665291218"></a><a name="zh-cn_topic_0000002512241020_p1371665291218"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="17.681768176817688%" headers="mcps1.2.9.1.6 "><p id="zh-cn_topic_0000002512241020_p763714253585"><a name="zh-cn_topic_0000002512241020_p763714253585"></a><a name="zh-cn_topic_0000002512241020_p763714253585"></a>4156</p>
</td>
<td class="cellrowborder" valign="top" width="17.47174717471747%" headers="mcps1.2.9.1.7 "><p id="zh-cn_topic_0000002512241020_p871645241210"><a name="zh-cn_topic_0000002512241020_p871645241210"></a><a name="zh-cn_topic_0000002512241020_p871645241210"></a>4209</p>
</td>
<td class="cellrowborder" valign="top" width="12.101210121012102%" headers="mcps1.2.9.1.8 "><p id="zh-cn_topic_0000002512241020_p17567819165411"><a name="zh-cn_topic_0000002512241020_p17567819165411"></a><a name="zh-cn_topic_0000002512241020_p17567819165411"></a>待测试</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002512241020_row4716052101211"><td class="cellrowborder" valign="top" width="7.820782078207822%" headers="mcps1.2.9.1.1 "><p id="zh-cn_topic_0000002512241020_p12716145211211"><a name="zh-cn_topic_0000002512241020_p12716145211211"></a><a name="zh-cn_topic_0000002512241020_p12716145211211"></a>128</p>
</td>
<td class="cellrowborder" valign="top" width="7.940794079407941%" headers="mcps1.2.9.1.2 "><p id="zh-cn_topic_0000002512241020_p1716105261213"><a name="zh-cn_topic_0000002512241020_p1716105261213"></a><a name="zh-cn_topic_0000002512241020_p1716105261213"></a>256</p>
</td>
<td class="cellrowborder" valign="top" width="7.510751075107512%" headers="mcps1.2.9.1.3 "><p id="zh-cn_topic_0000002512241020_p5719152110166"><a name="zh-cn_topic_0000002512241020_p5719152110166"></a><a name="zh-cn_topic_0000002512241020_p5719152110166"></a>512</p>
</td>
<td class="cellrowborder" valign="top" width="12.651265126512653%" headers="mcps1.2.9.1.4 "><p id="zh-cn_topic_0000002512241020_p11717252151216"><a name="zh-cn_topic_0000002512241020_p11717252151216"></a><a name="zh-cn_topic_0000002512241020_p11717252151216"></a>Disable</p>
</td>
<td class="cellrowborder" valign="top" width="16.821682168216824%" headers="mcps1.2.9.1.5 "><p id="zh-cn_topic_0000002512241020_p1171720524127"><a name="zh-cn_topic_0000002512241020_p1171720524127"></a><a name="zh-cn_topic_0000002512241020_p1171720524127"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="17.681768176817688%" headers="mcps1.2.9.1.6 "><p id="zh-cn_topic_0000002512241020_p36364250585"><a name="zh-cn_topic_0000002512241020_p36364250585"></a><a name="zh-cn_topic_0000002512241020_p36364250585"></a>4156</p>
</td>
<td class="cellrowborder" valign="top" width="17.47174717471747%" headers="mcps1.2.9.1.7 "><p id="zh-cn_topic_0000002512241020_p18717115211128"><a name="zh-cn_topic_0000002512241020_p18717115211128"></a><a name="zh-cn_topic_0000002512241020_p18717115211128"></a>4225</p>
</td>
<td class="cellrowborder" valign="top" width="12.101210121012102%" headers="mcps1.2.9.1.8 "><p id="zh-cn_topic_0000002512241020_p1756791918546"><a name="zh-cn_topic_0000002512241020_p1756791918546"></a><a name="zh-cn_topic_0000002512241020_p1756791918546"></a>待测试</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002512241020_row3717175216129"><td class="cellrowborder" valign="top" width="7.820782078207822%" headers="mcps1.2.9.1.1 "><p id="zh-cn_topic_0000002512241020_p17171452131210"><a name="zh-cn_topic_0000002512241020_p17171452131210"></a><a name="zh-cn_topic_0000002512241020_p17171452131210"></a>128</p>
</td>
<td class="cellrowborder" valign="top" width="7.940794079407941%" headers="mcps1.2.9.1.2 "><p id="zh-cn_topic_0000002512241020_p7717205291219"><a name="zh-cn_topic_0000002512241020_p7717205291219"></a><a name="zh-cn_topic_0000002512241020_p7717205291219"></a>128</p>
</td>
<td class="cellrowborder" valign="top" width="7.510751075107512%" headers="mcps1.2.9.1.3 "><p id="zh-cn_topic_0000002512241020_p153816220161"><a name="zh-cn_topic_0000002512241020_p153816220161"></a><a name="zh-cn_topic_0000002512241020_p153816220161"></a>512</p>
</td>
<td class="cellrowborder" valign="top" width="12.651265126512653%" headers="mcps1.2.9.1.4 "><p id="zh-cn_topic_0000002512241020_p1171775291216"><a name="zh-cn_topic_0000002512241020_p1171775291216"></a><a name="zh-cn_topic_0000002512241020_p1171775291216"></a>Enable</p>
</td>
<td class="cellrowborder" valign="top" width="16.821682168216824%" headers="mcps1.2.9.1.5 "><p id="zh-cn_topic_0000002512241020_p771710528122"><a name="zh-cn_topic_0000002512241020_p771710528122"></a><a name="zh-cn_topic_0000002512241020_p771710528122"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="17.681768176817688%" headers="mcps1.2.9.1.6 "><p id="zh-cn_topic_0000002512241020_p3636132515583"><a name="zh-cn_topic_0000002512241020_p3636132515583"></a><a name="zh-cn_topic_0000002512241020_p3636132515583"></a>2108</p>
</td>
<td class="cellrowborder" valign="top" width="17.47174717471747%" headers="mcps1.2.9.1.7 "><p id="zh-cn_topic_0000002512241020_p6717552131215"><a name="zh-cn_topic_0000002512241020_p6717552131215"></a><a name="zh-cn_topic_0000002512241020_p6717552131215"></a>2172</p>
</td>
<td class="cellrowborder" valign="top" width="12.101210121012102%" headers="mcps1.2.9.1.8 "><p id="zh-cn_topic_0000002512241020_p35679195548"><a name="zh-cn_topic_0000002512241020_p35679195548"></a><a name="zh-cn_topic_0000002512241020_p35679195548"></a>待测试</p>
</td>
</tr>
<tr id="zh-cn_topic_0000002512241020_row11717552141217"><td class="cellrowborder" valign="top" width="7.820782078207822%" headers="mcps1.2.9.1.1 "><p id="zh-cn_topic_0000002512241020_p9717165251212"><a name="zh-cn_topic_0000002512241020_p9717165251212"></a><a name="zh-cn_topic_0000002512241020_p9717165251212"></a>128</p>
</td>
<td class="cellrowborder" valign="top" width="7.940794079407941%" headers="mcps1.2.9.1.2 "><p id="zh-cn_topic_0000002512241020_p471715241210"><a name="zh-cn_topic_0000002512241020_p471715241210"></a><a name="zh-cn_topic_0000002512241020_p471715241210"></a>128</p>
</td>
<td class="cellrowborder" valign="top" width="7.510751075107512%" headers="mcps1.2.9.1.3 "><p id="zh-cn_topic_0000002512241020_p10169122341618"><a name="zh-cn_topic_0000002512241020_p10169122341618"></a><a name="zh-cn_topic_0000002512241020_p10169122341618"></a>512</p>
</td>
<td class="cellrowborder" valign="top" width="12.651265126512653%" headers="mcps1.2.9.1.4 "><p id="zh-cn_topic_0000002512241020_p5717165261214"><a name="zh-cn_topic_0000002512241020_p5717165261214"></a><a name="zh-cn_topic_0000002512241020_p5717165261214"></a>Disable</p>
</td>
<td class="cellrowborder" valign="top" width="16.821682168216824%" headers="mcps1.2.9.1.5 "><p id="zh-cn_topic_0000002512241020_p871713525124"><a name="zh-cn_topic_0000002512241020_p871713525124"></a><a name="zh-cn_topic_0000002512241020_p871713525124"></a>8</p>
</td>
<td class="cellrowborder" valign="top" width="17.681768176817688%" headers="mcps1.2.9.1.6 "><p id="zh-cn_topic_0000002512241020_p116365259582"><a name="zh-cn_topic_0000002512241020_p116365259582"></a><a name="zh-cn_topic_0000002512241020_p116365259582"></a>2108</p>
</td>
<td class="cellrowborder" valign="top" width="17.47174717471747%" headers="mcps1.2.9.1.7 "><p id="zh-cn_topic_0000002512241020_p18717115271213"><a name="zh-cn_topic_0000002512241020_p18717115271213"></a><a name="zh-cn_topic_0000002512241020_p18717115271213"></a>2172</p>
</td>
<td class="cellrowborder" valign="top" width="12.101210121012102%" headers="mcps1.2.9.1.8 "><p id="zh-cn_topic_0000002512241020_p14567131919547"><a name="zh-cn_topic_0000002512241020_p14567131919547"></a><a name="zh-cn_topic_0000002512241020_p14567131919547"></a>待测试</p>
</td>
</tr>
</tbody>
</table>

