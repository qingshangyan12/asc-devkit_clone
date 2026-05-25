# Bank冲突优化<a name="ZH-CN_TOPIC_0000002543422600"></a>

避免bank冲突的方法有两种：**优化计算逻辑**和**优化地址分配**。

-   **优化计算逻辑**

    对一个shape为\(8, 16, 16\)的输入做\(1, 0, 2\)的transpose操作，输出shape为\(16, 8, 16\)。通过将计算逻辑由“跳读，连续写”修改为“连续读，跳写”可避免冲突问题。实现方案对比如下：

    <a name="zh-cn_topic_0000002525448372_table12921549195512"></a>
    <table><thead align="left"><tr id="zh-cn_topic_0000002525448372_row1229364945511"><th class="cellrowborder" valign="top" width="6.813978389954251%" id="mcps1.1.4.1.1"><p id="zh-cn_topic_0000002525448372_p2081249145715"><a name="zh-cn_topic_0000002525448372_p2081249145715"></a><a name="zh-cn_topic_0000002525448372_p2081249145715"></a>实现方案</p>
    </th>
    <th class="cellrowborder" valign="top" width="42.6652389759564%" id="mcps1.1.4.1.2"><p id="zh-cn_topic_0000002525448372_p2029374985519"><a name="zh-cn_topic_0000002525448372_p2029374985519"></a><a name="zh-cn_topic_0000002525448372_p2029374985519"></a>原始实现</p>
    </th>
    <th class="cellrowborder" valign="top" width="50.520782634089365%" id="mcps1.1.4.1.3"><p id="zh-cn_topic_0000002525448372_p152931149115516"><a name="zh-cn_topic_0000002525448372_p152931149115516"></a><a name="zh-cn_topic_0000002525448372_p152931149115516"></a>优化实现</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="zh-cn_topic_0000002525448372_row1629374995517"><td class="cellrowborder" valign="top" width="6.813978389954251%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002525448372_p108124995710"><a name="zh-cn_topic_0000002525448372_p108124995710"></a><a name="zh-cn_topic_0000002525448372_p108124995710"></a>实现方法</p>
    </td>
    <td class="cellrowborder" valign="top" width="42.6652389759564%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002525448372_p55722115019"><a name="zh-cn_topic_0000002525448372_p55722115019"></a><a name="zh-cn_topic_0000002525448372_p55722115019"></a>跳读，连续写</p>
    <p id="zh-cn_topic_0000002525448372_p144638315712"><a name="zh-cn_topic_0000002525448372_p144638315712"></a><a name="zh-cn_topic_0000002525448372_p144638315712"></a>同一Repeat内输入的8个DataBlock都在同一个bank group而发生读读冲突。</p>
    </td>
    <td class="cellrowborder" valign="top" width="50.520782634089365%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002525448372_p2293249135516"><a name="zh-cn_topic_0000002525448372_p2293249135516"></a><a name="zh-cn_topic_0000002525448372_p2293249135516"></a>连续读，跳写</p>
    <p id="zh-cn_topic_0000002525448372_p6641340414"><a name="zh-cn_topic_0000002525448372_p6641340414"></a><a name="zh-cn_topic_0000002525448372_p6641340414"></a>同一个Repeat内输入的8个DataBlock不在同一个bank group内，避免了读读冲突。</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002525448372_row14922142214585"><td class="cellrowborder" valign="top" width="6.813978389954251%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002525448372_p6922182210587"><a name="zh-cn_topic_0000002525448372_p6922182210587"></a><a name="zh-cn_topic_0000002525448372_p6922182210587"></a>示意图</p>
    </td>
    <td class="cellrowborder" valign="top" width="42.6652389759564%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002525448372_p10922222205810"><a name="zh-cn_topic_0000002525448372_p10922222205810"></a><a name="zh-cn_topic_0000002525448372_p10922222205810"></a><a name="zh-cn_topic_0000002525448372_image1757423545813"></a><a name="zh-cn_topic_0000002525448372_image1757423545813"></a><span><img class="eddx" id="zh-cn_topic_0000002525448372_image1757423545813" src="figures/矩阵编程逻辑位置示意图.png" width="422.94" height="376.36672500000003"></span></p>
    </td>
    <td class="cellrowborder" valign="top" width="50.520782634089365%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002525448372_p1922622115813"><a name="zh-cn_topic_0000002525448372_p1922622115813"></a><a name="zh-cn_topic_0000002525448372_p1922622115813"></a><a name="zh-cn_topic_0000002525448372_image6621154316580"></a><a name="zh-cn_topic_0000002525448372_image6621154316580"></a><span><img class="eddx" id="zh-cn_topic_0000002525448372_image6621154316580" src="figures/矩阵编程逻辑位置示意图-0.png" width="489.77250000000004" height="363.174721"></span></p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002525448372_row3293124918559"><td class="cellrowborder" valign="top" width="6.813978389954251%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002525448372_p12812993573"><a name="zh-cn_topic_0000002525448372_p12812993573"></a><a name="zh-cn_topic_0000002525448372_p12812993573"></a>示例代码</p>
    </td>
    <td class="cellrowborder" valign="top" width="42.6652389759564%" headers="mcps1.1.4.1.2 "><a name="zh-cn_topic_0000002525448372_screen924835613570"></a><a name="zh-cn_topic_0000002525448372_screen924835613570"></a><pre class="screen" codetype="Cpp" id="zh-cn_topic_0000002525448372_screen924835613570">uint64_t mask = 128;
    UnaryRepeatParams params;
    params.dstBlkStride  = 1;
    params.srcBlkStride = 16;
    for(uint32_t i=0; i&lt;16; i++)   {
        AscendC::Adds(dstLocal[i * 128], srcLocal[i * 16], 0, mask, 1, params);
    }</pre>
    </td>
    <td class="cellrowborder" valign="top" width="50.520782634089365%" headers="mcps1.1.4.1.3 "><a name="zh-cn_topic_0000002525448372_screen271414925813"></a><a name="zh-cn_topic_0000002525448372_screen271414925813"></a><pre class="screen" codetype="Cpp" id="zh-cn_topic_0000002525448372_screen271414925813">uint64_t mask = 128;
    UnaryRepeatParams params;
    params.dstBlkStride  = 8;
    params.srcBlkStride = 1;
    for(uint32_t i=0; i&lt;8; i++)   {
        AscendC::Adds(dstLocal[i * 16], srcLocal[i * 256], 0, mask, 2, params);
    }</pre>
    </td>
    </tr>
    </tbody>
    </table>

-   **优化地址分配**

    实现连续4096个float元素的加法z = x + y，通过在内存分配时适当扩大内存，保证在一个Repeat内，x和y不会同时出现在同一个bank group内，x/y和z不会同时出现同一个bank内。完整样例可参考[避免bank冲突优化样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/04_best_practices/01_bank_conflict/bank_conflict/README.md)。

    实现方案对比如下：

    <a name="zh-cn_topic_0000002525448372_table839512544411"></a>
    <table><thead align="left"><tr id="zh-cn_topic_0000002525448372_row73965541942"><th class="cellrowborder" valign="top" width="6.813978389954251%" id="mcps1.1.4.1.1"><p id="zh-cn_topic_0000002525448372_p63963545416"><a name="zh-cn_topic_0000002525448372_p63963545416"></a><a name="zh-cn_topic_0000002525448372_p63963545416"></a>实现方案</p>
    </th>
    <th class="cellrowborder" valign="top" width="42.01304390148935%" id="mcps1.1.4.1.2"><p id="zh-cn_topic_0000002525448372_p539610544415"><a name="zh-cn_topic_0000002525448372_p539610544415"></a><a name="zh-cn_topic_0000002525448372_p539610544415"></a>原始实现</p>
    </th>
    <th class="cellrowborder" valign="top" width="51.17297770855641%" id="mcps1.1.4.1.3"><p id="zh-cn_topic_0000002525448372_p6396185413418"><a name="zh-cn_topic_0000002525448372_p6396185413418"></a><a name="zh-cn_topic_0000002525448372_p6396185413418"></a>优化实现</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="zh-cn_topic_0000002525448372_row143961754148"><td class="cellrowborder" valign="top" width="6.813978389954251%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002525448372_p43962544416"><a name="zh-cn_topic_0000002525448372_p43962544416"></a><a name="zh-cn_topic_0000002525448372_p43962544416"></a>实现方法</p>
    </td>
    <td class="cellrowborder" valign="top" width="42.01304390148935%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002525448372_p33964541745"><a name="zh-cn_topic_0000002525448372_p33964541745"></a><a name="zh-cn_topic_0000002525448372_p33964541745"></a>不做地址优化，直接使用InitBuffer分配内存，各个Tensor的地址分别为：</p>
    <p id="zh-cn_topic_0000002525448372_p17357131764"><a name="zh-cn_topic_0000002525448372_p17357131764"></a><a name="zh-cn_topic_0000002525448372_p17357131764"></a>x：起始地址0x0，tensor长度为4096 * sizeof(float)字节</p>
    <p id="zh-cn_topic_0000002525448372_p15372720618"><a name="zh-cn_topic_0000002525448372_p15372720618"></a><a name="zh-cn_topic_0000002525448372_p15372720618"></a>y：起始地址0x4000，tensor长度为4096 * sizeof(float)字节</p>
    <p id="zh-cn_topic_0000002525448372_p138635471667"><a name="zh-cn_topic_0000002525448372_p138635471667"></a><a name="zh-cn_topic_0000002525448372_p138635471667"></a>z：起始地址0x8000，tensor长度为4096 * sizeof(float)字节</p>
    <p id="zh-cn_topic_0000002525448372_p1927691223819"><a name="zh-cn_topic_0000002525448372_p1927691223819"></a><a name="zh-cn_topic_0000002525448372_p1927691223819"></a>在一个Repeat内，x和y同时读同一个bank group，x/y和z同时读写同一个bank。</p>
    </td>
    <td class="cellrowborder" valign="top" width="51.17297770855641%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002525448372_p330914381671"><a name="zh-cn_topic_0000002525448372_p330914381671"></a><a name="zh-cn_topic_0000002525448372_p330914381671"></a>优化地址，使用InitBuffer分配内存时适当扩大内存申请，各个Tensor的地址分别为：</p>
    <p id="zh-cn_topic_0000002525448372_p326294904420"><a name="zh-cn_topic_0000002525448372_p326294904420"></a><a name="zh-cn_topic_0000002525448372_p326294904420"></a>x：起始地址0x0，tensor长度为(4096 * sizeof(float) + 256)字节</p>
    <p id="zh-cn_topic_0000002525448372_p153096389720"><a name="zh-cn_topic_0000002525448372_p153096389720"></a><a name="zh-cn_topic_0000002525448372_p153096389720"></a>y：起始地址0x4100，tensor长度为(64 * 1024 - (4096 * sizeof(float) + 256))字节</p>
    <p id="zh-cn_topic_0000002525448372_p1130923814713"><a name="zh-cn_topic_0000002525448372_p1130923814713"></a><a name="zh-cn_topic_0000002525448372_p1130923814713"></a>z：起始地址0x10000，tensor长度为4096 * sizeof(float) 字节</p>
    <p id="zh-cn_topic_0000002525448372_p9897551154415"><a name="zh-cn_topic_0000002525448372_p9897551154415"></a><a name="zh-cn_topic_0000002525448372_p9897551154415"></a>x多申请256字节，避免一个Repeat内x y同时读同一个bank group；y多申请空间，确保z不会和x/y落入同一个bank</p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002525448372_row83963547410"><td class="cellrowborder" valign="top" width="6.813978389954251%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002525448372_p103961544412"><a name="zh-cn_topic_0000002525448372_p103961544412"></a><a name="zh-cn_topic_0000002525448372_p103961544412"></a>示意图</p>
    </td>
    <td class="cellrowborder" valign="top" width="42.01304390148935%" headers="mcps1.1.4.1.2 "><p id="zh-cn_topic_0000002525448372_p575310529216"><a name="zh-cn_topic_0000002525448372_p575310529216"></a><a name="zh-cn_topic_0000002525448372_p575310529216"></a><a name="zh-cn_topic_0000002525448372_image5993165819214"></a><a name="zh-cn_topic_0000002525448372_image5993165819214"></a><span><img class="eddx" id="zh-cn_topic_0000002525448372_image5993165819214" src="figures/矩阵编程逻辑位置示意图-1.png" width="357.105" height="231.19556250000002"></span></p>
    </td>
    <td class="cellrowborder" valign="top" width="51.17297770855641%" headers="mcps1.1.4.1.3 "><p id="zh-cn_topic_0000002525448372_p20186156039"><a name="zh-cn_topic_0000002525448372_p20186156039"></a><a name="zh-cn_topic_0000002525448372_p20186156039"></a><a name="zh-cn_topic_0000002525448372_image35231563314"></a><a name="zh-cn_topic_0000002525448372_image35231563314"></a><span><img class="eddx" id="zh-cn_topic_0000002525448372_image35231563314" src="figures/矩阵编程逻辑位置示意图-2.png" width="388.02750000000003" height="231.42000000000002"></span></p>
    </td>
    </tr>
    <tr id="zh-cn_topic_0000002525448372_row1539620548413"><td class="cellrowborder" valign="top" width="6.813978389954251%" headers="mcps1.1.4.1.1 "><p id="zh-cn_topic_0000002525448372_p43961554645"><a name="zh-cn_topic_0000002525448372_p43961554645"></a><a name="zh-cn_topic_0000002525448372_p43961554645"></a>示例代码</p>
    </td>
    <td class="cellrowborder" valign="top" width="42.01304390148935%" headers="mcps1.1.4.1.2 "><a name="zh-cn_topic_0000002525448372_screen35066415507"></a><a name="zh-cn_topic_0000002525448372_screen35066415507"></a><pre class="screen" codetype="Cpp" id="zh-cn_topic_0000002525448372_screen35066415507">pipe.InitBuffer(inQueueX, 1, 4096 * sizeof(float));
    pipe.InitBuffer(inQueueY, 1, 4096 * sizeof(float));
    pipe.InitBuffer(outQueueZ, 1, 4096 * sizeof(float));</pre>
    </td>
    <td class="cellrowborder" valign="top" width="51.17297770855641%" headers="mcps1.1.4.1.3 "><a name="zh-cn_topic_0000002525448372_screen51721010145115"></a><a name="zh-cn_topic_0000002525448372_screen51721010145115"></a><pre class="screen" codetype="Cpp" id="zh-cn_topic_0000002525448372_screen51721010145115">pipe.InitBuffer(inQueueX, 1, 4096 * sizeof(float) + 256); // 多申请256字节
    pipe.InitBuffer(inQueueY, 1, 64 * 1024 - (4096 * sizeof(float) + 256)); //多申请空间，确保z不会和x/y落入同一个bank， 64 * 1024是16个bank的空间，4096 * sizeof(float) + 256是x所占的空间
    pipe.InitBuffer(outQueueZ, 1, 4096 * sizeof(float));</pre>
    </td>
    </tr>
    </tbody>
    </table>

