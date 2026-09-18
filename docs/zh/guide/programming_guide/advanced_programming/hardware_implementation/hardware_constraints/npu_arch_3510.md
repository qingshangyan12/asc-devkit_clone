# NPU架构版本3510<a name="ZH-CN_TOPIC_0000002477825679"></a>

本节介绍硬件约束以及解决方案建议。对应的产品型号为：

<!-- npu="950" id1 -->
-   Ascend 950PR/Ascend 950DT 系列产品
<!-- end id1 -->

**表1**  硬件约束以及解决方案建议

<a name="table5984016192020"></a>
<table><thead align="left"><tr id="row49841016132016"><th class="cellrowborder" valign="top" width="13.608639136086392%" id="mcps1.2.4.1.1"><p id="p1498411692019"><a name="p1498411692019"></a><a name="p1498411692019"></a>分类</p>
</th>
<th class="cellrowborder" valign="top" width="33.406659334066596%" id="mcps1.2.4.1.2"><p id="p16984816162020"><a name="p16984816162020"></a><a name="p16984816162020"></a>硬件约束描述</p>
</th>
<th class="cellrowborder" valign="top" width="52.98470152984702%" id="mcps1.2.4.1.3"><p id="p681910154819"><a name="p681910154819"></a><a name="p681910154819"></a>解决方案建议</p>
</th>
</tr>
</thead>
<tbody><tr id="row1098410169205"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p73939428117"><a name="p73939428117"></a><a name="p73939428117"></a>内存访问对齐约束</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p339313421117"><a name="p339313421117"></a><a name="p339313421117"></a>各存储单元的最小访问粒度/地址对齐要求：</p>
<p id="p14393742911"><a name="p14393742911"></a><a name="p14393742911"></a>Unified Buffer：32Byte对齐。</p>
<p id="p1339384218120"><a name="p1339384218120"></a><a name="p1339384218120"></a>L1 Buffer：32Byte对齐。</p>
<p id="p03939425117"><a name="p03939425117"></a><a name="p03939425117"></a>L0A Buffer/L0B Buffer：512Byte对齐。</p>
<p id="p139316421915"><a name="p139316421915"></a><a name="p139316421915"></a>L0C Buffer：64/1024Byte对齐。</p>
<p id="p1739319428115"><a name="p1739319428115"></a><a name="p1739319428115"></a>BiasTable Buffer：64Byte对齐。</p>
<p id="p13932421317"><a name="p13932421317"></a><a name="p13932421317"></a>Fixpipe Buffer：64/128Byte对齐。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p9477161418"><a name="p9477161418"></a><a name="p9477161418"></a>进行数据搬运时，需要感知对齐约束。</p>
<p id="p8477161419"><a name="p8477161419"></a><a name="p8477161419"></a>针对UB，对于非对齐场景，可使用非对齐搬入LoadUnAlign和非对齐搬出StoreUnAlign指令进行处理，可支持1Byte对齐访问。其中首地址非对齐连续访问时性能与对齐场景基本一致，首地址非对齐非连续访问时，计算bound场景下不受影响，UB访存bound会受影响。</p>
<p id="p7477161420"><a name="p7477161420"></a><a name="p7477161420"></a>针对L0C Buffer，Mmad计算时以1024B对齐，矩阵计算搬出时以64B对齐。</p>
<p id="p6477161421"><a name="p6477161421"></a><a name="p6477161421"></a>针对Fixpipe Buffer，relu场景下以64B对齐，quant场景以128B对齐，具体可参考<a href="../../../../../api/SIMD-API/tensor_api/matrix_compute/cube_store_key_features/relu_pre.md">随路ReLU</a>、<a href="../../../../../api/SIMD-API/tensor_api/matrix_compute/cube_store_key_features/quant_pre.md">随路量化</a>API章节。</p>
</td>
</tr>
<tr id="row698415161207"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p153938429118"><a name="p153938429118"></a><a name="p153938429118"></a>API通用约束</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p1396164216117"><a name="p1396164216117"></a><a name="p1396164216117"></a>使用Ascend C API时，源操作数和目的操作数的地址重叠通用约束</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p143966421118"><a name="p143966421118"></a><a name="p143966421118"></a>使用基础API的Tensor高维切分计算接口时，为了节省地址空间，开发者可以定义一个Tensor，供源操作数与目的操作数同时使用（即地址重叠）。使用时需要注意以下约束：</p>
<a name="ul24282818270"></a><a name="ul24282818270"></a><ul id="ul24282818270"><li>单次迭代内：源操作数与目的操作数必须100%完全重叠，不支持部分重叠。</li><li>多次迭代间：不支持前序迭代的目的操作数与后序迭代的源操作数重叠。例如，第N次迭代的目的操作数是第N+1次的源操作数。在这种情况下，第N次迭代可能会改写覆盖源操作数的数值，导致无法得到预期结果。特别地，对于部分双目计算类的API（Add、Sub、Mul、Max、Min、AddRelu、SubRelu），当数据类型为half、int32_t、float时，支持前序迭代的目的操作数与后序迭代的源操作数重叠：仅针对目的操作数和第二个源操作数重叠的情况，且src1RepStride或者dstRepStride必须为0。</li></ul>
</td>
</tr>
<tr id="row798416160210"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p5393154210116"><a name="p5393154210116"></a><a name="p5393154210116"></a>内存访问（GM）</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p153934427115"><a name="p153934427115"></a><a name="p153934427115"></a>单次搬运数据长度16KB以上时，可发挥带宽的最佳性能。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p1839334215113"><a name="p1839334215113"></a><a name="p1839334215113"></a>根据实测经验，单次搬运数据长度16KB以上时，通常能较好地发挥出带宽的最佳性能。因此对于单次搬运，应考虑尽可能的搬运较大的数据块（不同芯片不一样）。</p>
<p id="p14280187181120"><a name="p14280187181120"></a><a name="p14280187181120"></a>具体解决方案可参考<a href="../../../../operator_practice/simd_operator_optimization/memory_access/transfer_larger_data_blocks.md">尽量一次搬运较大的数据块</a>章节。</p>
</td>
</tr>
<tr id="row189843164203"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p1339364210116"><a name="p1339364210116"></a><a name="p1339364210116"></a>内存访问（GM）</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439317429120"><a name="p439317429120"></a><a name="p439317429120"></a>数据搬运会被拆成128B/256B/512B不同长度进行搬运，非对齐会向上取整。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p133931242414"><a name="p133931242414"></a><a name="p133931242414"></a>Tiling尽量让搬运的内轴128B、256B、512B对齐。</p>
<p id="p4131102271515"><a name="p4131102271515"></a><a name="p4131102271515"></a>具体解决方案可参考<a href="../../../../operator_practice/simd_operator_optimization/memory_access/gm_address_512b_alignment.md">GM地址尽量512B对齐</a>章节。</p>
</td>
</tr>
<tr id="row10984111617207"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p153938429119"><a name="p153938429119"></a><a name="p153938429119"></a>内存访问（UB）</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p1393442616"><a name="p1393442616"></a><a name="p1393442616"></a>UB bank访问冲突（Vector计算访问/搬运访问）。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p123932421312"><a name="p123932421312"></a><a name="p123932421312"></a>UB需要按照芯片要求，在软件实现时错开处理的地址，从而解决bank冲突。具体解决方案可参考<a href="../../../../operator_practice/simd_operator_optimization/memory_access/avoid_ub_bank_conflict/avoid_bank_conflict_npu_arch_3510.md">避免bank冲突（NPU架构版本3510）</a>章节。</p>
</td>
</tr>
<tr id="row13619162955919"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p039544211116"><a name="p039544211116"></a><a name="p039544211116"></a>Vector计算</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439412428114"><a name="p439412428114"></a><a name="p439412428114"></a>Ln、Exp、Sqrt、Rsqrt、Reciprocal、Div等接口不支持Subnormal计算</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p173946428118"><a name="p173946428118"></a><a name="p173946428118"></a>Ln、Exp、Sqrt、Rsqrt、Reciprocal、Div接口在3510上若需要支持Subnormal计算，需要配置对应的FTZ_FALSE算法，参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md#subnormal-migration">兼容性迁移指南</a>。</p>
</td>
</tr>
<tr id="row7725134123914"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p1639310422015"><a name="p1639310422015"></a><a name="p1639310422015"></a>Cube数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p339311421713"><a name="p339311421713"></a><a name="p339311421713"></a>删除L1 Buffer到GM的数据通路</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p113937420112"><a name="p113937420112"></a><a name="p113937420112"></a>1. 对于纯Cube侧算子，可以走 L1 Buffer -&gt; L0A Buffer/L0B Buffer -&gt; L0C Buffer -&gt; GM这一组数据通路。</p>
<p id="p18393742615"><a name="p18393742615"></a><a name="p18393742615"></a>2. 对于Cube+Vector的融合算子，可走 L1 Buffer -&gt; UB -&gt; GM。</p>
<p id="p28393742616"><a name="p28393742616"></a><a name="p28393742616"></a>参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md">兼容性迁移指南</a></p>
</td>
</tr>
<tr id="row10203343193115"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p1139319422116"><a name="p1139319422116"></a><a name="p1139319422116"></a>Cube数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p139319421514"><a name="p139319421514"></a><a name="p139319421514"></a>删除GM到L0A数据通路</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p23931842017"><a name="p23931842017"></a><a name="p23931842017"></a>将原本GM-&gt;L0A Buffer的LoadData 2D数据搬运拆分为两部分：</p>
<p id="p33931842018"><a name="p33931842018"></a><a name="p33931842018"></a>1. GM -&gt; L1 Buffer（使用LoadData 2D矩阵搬运）</p>
<p id="p43931842019"><a name="p43931842019"></a><a name="p43931842019"></a>2. L1 Buffer -&gt; L0A Buffer（使用LoadData 2D矩阵搬运）</p>
<p id="p53931842020"><a name="p53931842020"></a><a name="p53931842020"></a>参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md">兼容性迁移指南</a></p>
</td>
</tr>
<tr id="row194173017535"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p1839310421317"><a name="p1839310421317"></a><a name="p1839310421317"></a>Cube数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p1139313428114"><a name="p1139313428114"></a><a name="p1139313428114"></a>删除GM到L0B数据通路</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p639415421614"><a name="p639415421614"></a><a name="p639415421614"></a>将原本GM-&gt;L0B Buffer的LoadData 2D数据搬运拆分为两部分：</p>
<p id="p739415421615"><a name="p739415421615"></a><a name="p739415421615"></a>1. GM -&gt; L1 Buffer（使用LoadData 2D矩阵搬运）</p>
<p id="p839415421616"><a name="p839415421616"></a><a name="p839415421616"></a>2. L1 Buffer -&gt; L0B Buffer（使用LoadData 2D矩阵搬运）</p>
<p id="p939415421617"><a name="p939415421617"></a><a name="p939415421617"></a>参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md">兼容性迁移指南</a></p>
</td>
</tr>
<tr id="row724416349532"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p4394342219"><a name="p4394342219"></a><a name="p4394342219"></a>Cube数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p11394134215111"><a name="p11394134215111"></a><a name="p11394134215111"></a>在使用LoadData（卷积数据搬运）时，不再支持SetLoadDataBoundary接口。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p939416422114"><a name="p939416422114"></a><a name="p939416422114"></a>1. 当A2/A3上SetLoadDataBoundary的值为0时，在3510直接移除调用即可。</p>
<p id="p1039416422115"><a name="p1039416422115"></a><a name="p1039416422115"></a>2. 当A2/A3上SetLoadDataBoundary的值为非0时，把一次搬运拆成多次 LoadData，显式更新源/目的偏移实现绕回。详情参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md#set-load-data-boundary-migration">手动拆分LoadData并实现地址绕回</a>。</p>
</td>
</tr>
<tr id="row6730204410539"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p153942421415"><a name="p153942421415"></a><a name="p153942421415"></a>Cube数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p73942421016"><a name="p73942421016"></a><a name="p73942421016"></a>Fill接口不支持L0A/L0B的初始化</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p1139411421119"><a name="p1139411421119"></a><a name="p1139411421119"></a>1. 调用Fill接口，完成L1 Buffer上的初始化</p>
<p id="p1239411421120"><a name="p1239411421120"></a><a name="p1239411421120"></a>2. 用LoadData 2D矩阵将数据搬运搬到 L0A/L0B；</p>
<p id="p1339411421121"><a name="p1339411421121"></a><a name="p1339411421121"></a>参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md">兼容性迁移指南</a></p>
</td>
</tr>
<tr id="row56187293596"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p113943421111"><a name="p113943421111"></a><a name="p113943421111"></a>Cube数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439413421617"><a name="p439413421617"></a><a name="p439413421617"></a>Cube数据搬入不支持int4b_t数据类型</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p439454212112"><a name="p439454212112"></a><a name="p439454212112"></a>1. 可使用fp4x2_e2m1_t、fp4x2_e1m2_t进行搬运，具体可参考：<a href="../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_MX.md">LoadData（2D矩阵搬运MX）</a></p>
<p id="p539454212113"><a name="p539454212113"></a><a name="p539454212113"></a>2. 在 Vector上把int4b_t数据类型转化为int8_t数据类型。</p>
<p id="p639454212114"><a name="p639454212114"></a><a name="p639454212114"></a>3. 利用UB-&gt;L1数据通路，将数据搬运到L1 Buffer。</p>
<p id="p739454212115"><a name="p739454212115"></a><a name="p739454212115"></a>参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md">兼容性迁移指南</a></p>
</td>
</tr>
<tr id="row16184293595"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p939494215116"><a name="p939494215116"></a><a name="p939494215116"></a>Cube数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p12394194219113"><a name="p12394194219113"></a><a name="p12394194219113"></a>LoadDataWithTranspose接口不支持L1-&gt;L0A通路</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p1639410425118"><a name="p1639410425118"></a><a name="p1639410425118"></a>替换为其它搬运接口，如<a href="../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_V2.md">LoadData（2D矩阵搬运V2）</a>矩阵搬运</p>
</td>
</tr>
<tr id="row1361852935919"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p339410421115"><a name="p339410421115"></a><a name="p339410421115"></a>Cube数据搬运/Cube计算</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p14651181022321"><a name="p14651181022321"></a><a name="p14651181022321"></a>L0A矩阵分形从Zz变为Nz</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p73946421416"><a name="p73946421416"></a><a name="p73946421416"></a>分形的转换涉及到以下两种典型场景的兼容性适配：</p>
<p id="p83946421417"><a name="p83946421417"></a><a name="p83946421417"></a>1. L0A不复用。对一整块A矩阵进行一次mmad计算。</p>
<p id="p93946421418"><a name="p93946421418"></a><a name="p93946421418"></a>2. L0A复用。切分M轴，A矩阵分为两块子矩阵，进行两次mmad计算。</p>
<p id="p103946421419"><a name="p103946421419"></a><a name="p103946421419"></a>对于场景1，需要修改L1&gt;L0A搬运过程的逻辑，由Nz-&gt;Zz分形转换变为Nz-&gt;Nz。具体可参考样例实现。</p>
<p id="p113946421420"><a name="p113946421420"></a><a name="p113946421420"></a>对于场景2，在Atlas A2/A3 系列产品上，由于Zz分形本身在M轴上连续，所以L1-&gt;L0A搬运逻辑和场景1相同，而对于Ascend 950PR/Ascend 950DT 系列产品，切分M轴会导致矩阵不连续，需要拆分为两个Nz矩阵。具体可参考样例实现。</p>
<p id="p123946421421"><a name="p123946421421"></a><a name="p123946421421"></a>详情可参考样例：<a href="../../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/pattern_transformation/README.md">pattern_transformation</a></p>
</td>
</tr>
<tr id="row661982915598"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p0394442217"><a name="p0394442217"></a><a name="p0394442217"></a>Cube数据搬运/Cube计算</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p339412428114"><a name="p339412428114"></a><a name="p339412428114"></a>不支持4:2结构化稀疏矩阵计算，以下指令不支持：LoadDataWithSparse、MmadWithSparse</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p173946428119"><a name="p173946428119"></a><a name="p173946428119"></a>移除该加载路径，保留/恢复稠密 B 矩阵布局，可使用普通 LoadData 和 Mmad。参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md">兼容性迁移指南</a></p>
</td>
</tr>
<tr id="row461911299593"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p14394142120"><a name="p14394142120"></a><a name="p14394142120"></a>Cube计算</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p93942421518"><a name="p93942421518"></a><a name="p93942421518"></a>Cube计算不支持int4b_t数据类型</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p1139412421412"><a name="p1139412421412"></a><a name="p1139412421412"></a>1. 在 Vector上把int4b_t数据类型转化为int8_t数据类型。</p>
<p id="p1239412421413"><a name="p1239412421413"></a><a name="p1239412421413"></a>2. 利用UB-&gt;L1数据通路，将数据搬运到L1 Buffer。</p>
<p id="p1339412421414"><a name="p1339412421414"></a><a name="p1339412421414"></a>参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md">兼容性迁移指南</a></p>
</td>
</tr>
<tr id="row196181129135912"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p193948425112"><a name="p193948425112"></a><a name="p193948425112"></a>Cube计算</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p163941542120"><a name="p163941542120"></a><a name="p163941542120"></a>Cube计算场景float算力为half算力的1/16。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p73946421420"><a name="p73946421420"></a><a name="p73946421420"></a>Cube计算场景float算力为half算力的1/16，建议使用<a href="../../../../../api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute_key_features/HF32.md">HF32</a>特性，算力为half算力的1/2，更多理论算力规格可参考<a href="../../../../../api/appendix/cube_instruction_theoretical_perf_summary.md">Cube指令理论性能汇总</a>章节。</p>
</td>
</tr>
<tr id="row96190298592"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p1395242515"><a name="p1395242515"></a><a name="p1395242515"></a>指令队列深度</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p12395184212117"><a name="p12395184212117"></a><a name="p12395184212117"></a>Cube/Vector计算单元和搬运单元的指令队列存在深度规格，对应的指令队列达到最大深度时会阻塞该Cube/Vector核上其他全部指令下发，可能引起流水断流。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p11395342315"><a name="p11395342315"></a><a name="p11395342315"></a>队列深度表示同一指令队列中可缓存的待执行指令数量。当其中任一待执行指令（Vector指令队列、Cube指令队列、MTE1/MTE2/MTE3指令队列等）数量达到队列深度时，对应的Scalar单元如果再次向该指令队列发射指令会被阻塞，导致Scalar单元无法继续顺序发射后续指令，可通过优化指令数量等方式进行性能优化，更多说明请参考<a href="../basic_architecture.md#典型的指令流">典型的指令流</a>，具体的规格请参考<a href="../architecture_spec/npu_arch_3510.md#指令队列深度">3510指令队列深度</a>。</p>
</td>
</tr>
<tr id="row13620172910598"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p139512427120"><a name="p139512427120"></a><a name="p139512427120"></a>流水同步（核内）</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439594219112"><a name="p439594219112"></a><a name="p439594219112"></a>set/wait同步不匹配，状态会残留，影响后续算子。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p10395542820"><a name="p10395542820"></a><a name="p10395542820"></a>通过<a href="../../../debug_and_tuning/functional_debug/cpu_twin_debug.md">孪生调试</a>/<a href="../../../debug_and_tuning/functional_debug/npu_board_debug.md">mssanitizer</a>工具，提前识别此类问题。</p>
</td>
</tr>
<tr id="row13620172910599"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p139512427121"><a name="p139512427121"></a><a name="p139512427121"></a>流水同步（核内）</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439594219113"><a name="p439594219113"></a><a name="p439594219113"></a>相同流水、相同eventID下，连续使用SetFlag会引发未定义行为，例如：</p>
<p id="p539594219114"><a name="p539594219114"></a><a name="p539594219114"></a>AscendC::SetFlag&lt;AscendC::HardEvent::M_MTE1&gt;(0);</p>
<p id="p639594219115"><a name="p639594219115"></a><a name="p639594219115"></a>AscendC::SetFlag&lt;AscendC::HardEvent::M_MTE1&gt;(0);</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p10395542821"><a name="p10395542821"></a><a name="p10395542821"></a>通过<a href="../../../debug_and_tuning/functional_debug/cpu_twin_debug.md">孪生调试</a>/<a href="../../../debug_and_tuning/functional_debug/npu_board_debug.md">mssanitizer</a>工具，提前识别此类问题。</p>
</td>
</tr>
<tr id="row13620172910600"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p139512427122"><a name="p139512427122"></a><a name="p139512427122"></a>流水同步（核间）</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439594219116"><a name="p439594219116"></a><a name="p439594219116"></a>使用核间同步接口的核函数必须用mix函数限定符。<a href="../../../language_extension/simd_builtin_keywords.md#函数执行空间限定符">mix关键字</a></p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p10395542822"><a name="p10395542822"></a><a name="p10395542822"></a>通过<a href="../../../debug_and_tuning/functional_debug/cpu_twin_debug.md">孪生调试</a>/<a href="../../../debug_and_tuning/functional_debug/npu_board_debug.md">mssanitizer</a>工具，提前识别此类问题。</p>
</td>
</tr>
<tr id="row13620172910601"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p139512427123"><a name="p139512427123"></a><a name="p139512427123"></a>流水同步（核间）</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439594219117"><a name="p439594219117"></a><a name="p439594219117"></a><a href="../../../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md">CrossCoreSetFlag</a>和<a href="../../../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md">CrossCoreWaitFlag</a>模式为0、1、2时，模板参数pipe不支持设置为PIPE_ALL，PIPE_S。模式4不支持PIPE_ALL。详情参考<a href="../../../../cross_gen_migration_guide/instructions_for_new_features/3510_new_features.md#section_crosscore">新增核间同步控制模式4</a>。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p10395542823"><a name="p10395542823"></a><a name="p10395542823"></a>通过<a href="../../../debug_and_tuning/functional_debug/cpu_twin_debug.md">孪生调试</a>/<a href="../../../debug_and_tuning/functional_debug/npu_board_debug.md">mssanitizer</a>工具，提前识别此类问题。</p>
</td>
</tr>
<tr id="row13620172910602"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p139512427124"><a name="p139512427124"></a><a name="p139512427124"></a>流水同步（核间）</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439594219118"><a name="p439594219118"></a><a name="p439594219118"></a>CrossCoreSetFlag计数器存在限制，超出15次需要做一次反向同步，否则会出现卡死。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p10395542824"><a name="p10395542824"></a><a name="p10395542824"></a>通过<a href="../../../debug_and_tuning/functional_debug/cpu_twin_debug.md">孪生调试</a>/<a href="../../../debug_and_tuning/functional_debug/npu_board_debug.md">mssanitizer</a>工具，超出场景提前报错。</p>
</td>
</tr>
<tr id="row13620172910603"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p139512427125"><a name="p139512427125"></a><a name="p139512427125"></a>Scalar</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439594219119"><a name="p439594219119"></a><a name="p439594219119"></a>标量写GM时，数据会被缓存在DCache。硬件不保证DCache和GM一致性，需要用户保证。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p10395542825"><a name="p10395542825"></a><a name="p10395542825"></a>可使用<a href="../../../../../api/SIMD-API/basic_api/cache_control/DataCacheCleanAndInvalid.md">DataCacheCleanAndInvalid</a>来保证一致性，若预期不通过DCache读写数据，可通过<a href="../../../../../api/SIMD-API/basic_api/scalar_compute/ReadGmBypassDCache_ISASI.md">ReadGmBypassDCache</a>/<a href="../../../../../api/SIMD-API/basic_api/scalar_compute/WriteGmBypassDCache_ISASI.md">WriteGmBypassDCache</a>接口进行调用。详情参考<a href="../../memory_model/cache_coherence.md">缓存一致性</a>。</p>
</td>
</tr>
<tr id="row10203343193116"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p139319421515"><a name="p139319421515"></a><a name="p139319421515"></a>ICache</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p23931842018"><a name="p23931842018"></a><a name="p23931842018"></a>AIC ICache 硬件规格限制32KB，AIV ICache 硬件规格限制16KB。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p33931842019"><a name="p33931842019"></a><a name="p33931842019"></a>ICache位置在Scalar取指缓存，ICache大小在AIC和AIV侧存在差异，缓存一致性内容可参考<a href="../../memory_model/cache_coherence.md">缓存一致性</a>章节。拆分Tiling_key或使用模板函数来减少代码段。详情见<a href="../../../advanced_programming/aclnn_operator_development/design_and_implementation/multi_branch_strategy.md">多分支策略</a>。</p>
</td>
</tr>
<tr id="row194173017536"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p1839310421318"><a name="p1839310421318"></a><a name="p1839310421318"></a>ICache</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p1139313428115"><a name="p1139313428115"></a><a name="p1139313428115"></a>不同核从同一个GM上访问指令数据并加载到ICache时，并行访问读取性能会下降。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p639415421618"><a name="p639415421618"></a><a name="p639415421618"></a>小shape场景下可采取减少启动核数等优化手段，减少多核访问同一个GM上的数据。</p>
</td>
</tr>
<tr id="row724416349533"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p4394342220"><a name="p4394342220"></a><a name="p4394342220"></a>DCache</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p11394134215112"><a name="p11394134215112"></a><a name="p11394134215112"></a>SIMD DCache硬件规格限制32KB，SIMT DCache硬件规格限制128KB。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p939416422116"><a name="p939416422116"></a><a name="p939416422116"></a>DCache位置在Scalar数据缓存，缓存一致性内容可参考<a href="../../memory_model/cache_coherence.md">缓存一致性</a>章节。</p>
</td>
</tr>
<tr id="row6730204410540"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p153942421416"><a name="p153942421416"></a><a name="p153942421416"></a>AIPP</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p73942421017"><a name="p73942421017"></a><a name="p73942421017"></a>删除 AIPP 硬件指令并采用软仿，功能可迁移但性能可能下降。对应指令如下：SetAippFunctions、LoadImageToLocal</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p1139411421122"><a name="p1139411421122"></a><a name="p1139411421122"></a>对涉及AIPP硬件指令的算子重新进行性能评估。参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md">兼容性迁移指南</a></p>
</td>
</tr>
<tr id="row56187293597"><td class="cellrowborder" valign="top" width="13.608639136086392%" headers="mcps1.2.4.1.1 "><p id="p113943421112"><a name="p113943421112"></a><a name="p113943421112"></a>调测</p>
</td>
<td class="cellrowborder" valign="top" width="33.406659334066596%" headers="mcps1.2.4.1.2 "><p id="p439413421618"><a name="p439413421618"></a><a name="p439413421618"></a>删除CheckLocalMemoryIA接口相关寄存器，不支持该调测 API。</p>
</td>
<td class="cellrowborder" valign="top" width="52.98470152984702%" headers="mcps1.2.4.1.3 "><p id="p439454212116"><a name="p439454212116"></a><a name="p439454212116"></a>调测类接口，需要移除功能依赖。该接口为A2/A3上的ISASI接口，950不保证兼容性。参考<a href="../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md#check-local-memory-ia-migration">删除CheckLocalMemoryIA调用</a>。</p>
</td>
</tr>
</tbody>
</table>
