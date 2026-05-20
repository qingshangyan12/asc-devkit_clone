# 矩阵计算的搬入总体说明

矩阵计算的搬入是AscendC编程框架中用于数据搬运的核心接口系列，主要实现Global Memory到L1、L1到L0A/L0B之间的数据高效传输。该接口系列提供了多种数据搬运模式，能够满足矩阵计算中左矩阵、右矩阵、MX缩放系数以及辅助数据的搬运需求，通常与Mmad接口配合使用。

矩阵计算的搬入接口系列支持多种数据搬运场景，包括**二维格式数据搬运、矩阵转置搬运、ND/DN/NZ/ZN布局转换、MX Scale数据搬运以及BiasTable Buffer/Fixpipe Buffer/UB辅助数据搬运**等。通过灵活配置不同的张量布局、存储位置和搬运原子对象，开发者可以精确控制数据的搬运路径、起始位置、搬运长度等关键信息，充分利用硬件的数据搬运能力，最大化算子的整体执行性能。

矩阵计算的搬入接口支持多种数据通路，每种通路对应特定的存储层级和访问特性。

针对Ascend 950PR/Ascend 950DT：

**表 1** **数据通路和存储层级**

<a name="tensor_api_matrix_data_in_overview_table_data_path"></a>
<table><thead align="left"><tr><th class="cellrowborder" valign="top" width="16.748325167483248%" id="mcps1.1.6.1.1"><p>源位置</p>
</th>
<th class="cellrowborder" valign="top" width="13.65863413658634%" id="mcps1.1.6.1.2"><p>源地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="11.868813118688129%" id="mcps1.1.6.1.3"><p>目的位置</p>
</th>
<th class="cellrowborder" valign="top" width="17.90820917908209%" id="mcps1.1.6.1.4"><p>目的地址对齐要求</p>
</th>
<th class="cellrowborder" valign="top" width="39.816018398160175%" id="mcps1.1.6.1.5"><p>典型应用场景</p>
</th>
</tr>
</thead>
<tbody><tr><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.1.6.1.1 "><p>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.1.6.1.2 "><p>1字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.1.6.1.3 "><p>L1(A1)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.1.6.1.4 "><p>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.1.6.1.5 "><p>从全局内存加载左矩阵数据到L1</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.1.6.1.1 "><p>Global Memory</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.1.6.1.2 "><p>1字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.1.6.1.3 "><p>L1(B1)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.1.6.1.4 "><p>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.1.6.1.5 "><p>从全局内存加载右矩阵数据到L1</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.1.6.1.1 "><p>L1(A1)</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.1.6.1.2 "><p>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.1.6.1.3 "><p>L0A(A2)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.1.6.1.4 "><p>512字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.1.6.1.5 "><p>从L1加载左矩阵数据到L0A</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="16.748325167483248%" headers="mcps1.1.6.1.1 "><p>L1(B1)</p>
</td>
<td class="cellrowborder" valign="top" width="13.65863413658634%" headers="mcps1.1.6.1.2 "><p>32字节</p>
</td>
<td class="cellrowborder" valign="top" width="11.868813118688129%" headers="mcps1.1.6.1.3 "><p>L0B(B2)</p>
</td>
<td class="cellrowborder" valign="top" width="17.90820917908209%" headers="mcps1.1.6.1.4 "><p>512字节</p>
</td>
<td class="cellrowborder" valign="top" width="39.816018398160175%" headers="mcps1.1.6.1.5 "><p>从L1加载右矩阵数据到L0B</p>
</td>
</tr>
</tbody>
</table>

**表 2** **接口分类与使用场景**

<a name="tensor_api_matrix_data_in_overview_table_interface_category"></a>
<table><thead align="left"><tr><th class="cellrowborder" valign="top" width="24.98%" id="mcps1.2.5.1.1"><p>接口名称</p>
</th>
<th class="cellrowborder" valign="top" width="25.019999999999996%" id="mcps1.2.5.1.2"><p>主要功能</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.3"><p>数据通路</p>
</th>
<th class="cellrowborder" valign="top" width="25%" id="mcps1.2.5.1.4"><p>典型应用场景</p>
</th>
</tr>
</thead>
<tbody><tr><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p><a href="gm_to_l1_data_movement.md">CopyGM2L1</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p>Global Memory到L1的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p>GM-&gt;L1</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p>左矩阵/右矩阵搬入L1，支持ND、DN、NZ、ZN以及MX Scale相关布局搬运</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p><a href="l1_to_l0a_data_movement.md">CopyL12L0A</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p>L1到L0A的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p>L1-&gt;L0A</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p>左矩阵搬入L0A，支持普通搬运和转置搬运</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p><a href="l1_to_l0b_data_movement.md">CopyL12L0B</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p>L1到L0B的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p>L1-&gt;L0B</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p>右矩阵搬入L0B，支持普通搬运和转置搬运</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p><a href="l1_to_l0scalea_data_movement.md">CopyL12L0ScaleA</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p>L1到L0ScaleA的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p>L1-&gt;L0ScaleA</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p>MX矩阵计算中左矩阵缩放系数搬入</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="24.98%" headers="mcps1.2.5.1.1 "><p><a href="l1_to_l0scaleb_data_movement.md">CopyL12L0ScaleB</a></p>
</td>
<td class="cellrowborder" valign="top" width="25.019999999999996%" headers="mcps1.2.5.1.2 "><p>L1到L0ScaleB的数据搬运</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.3 "><p>L1-&gt;L0ScaleB</p>
</td>
<td class="cellrowborder" valign="top" width="25%" headers="mcps1.2.5.1.4 "><p>MX矩阵计算中右矩阵缩放系数搬入</p>
</td>
</tr>
</tbody>
</table>

**表 3** **辅助数据搬运接口说明**

<a name="tensor_api_matrix_data_in_overview_table_auxiliary_interface"></a>
<table><thead align="left"><tr><th class="cellrowborder" valign="top" width="19.383838383838384%" id="mcps1.3.4.1.1"><p>接口名称</p>
</th>
<th class="cellrowborder" valign="top" width="47.28282828282829%" id="mcps1.3.4.1.2"><p>功能说明</p>
</th>
<th class="cellrowborder" valign="top" width="33.333333333333336%" id="mcps1.3.4.1.3"><p>配合使用的场景</p>
</th>
</tr>
</thead>
<tbody><tr><td class="cellrowborder" valign="top" width="19.383838383838384%" headers="mcps1.3.4.1.1 "><p><a href="l1_to_bias_table_buffer_data_movement.md">CopyL12BT</a></p>
</td>
<td class="cellrowborder" valign="top" width="47.28282828282829%" headers="mcps1.3.4.1.2 "><p>将L1中的偏置数据搬运到BiasTable Buffer</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.3.4.1.3 "><p>矩阵计算或搬出阶段需要偏置表数据的场景</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="19.383838383838384%" headers="mcps1.3.4.1.1 "><p><a href="l1_to_fixpipe_buffer_data_movement.md">CopyL12FB</a></p>
</td>
<td class="cellrowborder" valign="top" width="47.28282828282829%" headers="mcps1.3.4.1.2 "><p>将L1中的辅助控制数据搬运到Fixpipe Buffer</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.3.4.1.3 "><p>Fixpipe相关搬出配置需要辅助数据的场景</p>
</td>
</tr>
<tr><td class="cellrowborder" valign="top" width="19.383838383838384%" headers="mcps1.3.4.1.1 "><p><a href="l1_to_ub_data_movement.md">CopyL12UB</a></p>
</td>
<td class="cellrowborder" valign="top" width="47.28282828282829%" headers="mcps1.3.4.1.2 "><p>将L1中的数据搬运到UB</p>
</td>
<td class="cellrowborder" valign="top" width="33.333333333333336%" headers="mcps1.3.4.1.3 "><p>L1与UB之间需要中间数据交换的场景</p>


</td>
</tr>
</tbody>
</table>

**表 4** **Ascend 950PR/Ascend 950DT产品接口支持情况**

<a name="tensor_api_matrix_data_in_overview_table_product_support"></a>
<table><thead align="left"><tr><th class="cellrowborder" valign="top" width="19.07%" id="mcps1.4.8.1.1"><p>产品系列</p>
</th>
<th class="cellrowborder" valign="top" width="15.07%" id="mcps1.4.8.1.2"><p>CopyGM2L1</p>
</th>
<th class="cellrowborder" valign="top" width="13.459999999999999%" id="mcps1.4.8.1.3"><p>CopyL12L0A</p>
</th>
<th class="cellrowborder" valign="top" width="13.459999999999999%" id="mcps1.4.8.1.4"><p>CopyL12L0B</p>
</th>
<th class="cellrowborder" valign="top" width="19.47%" id="mcps1.4.8.1.5"><p>CopyL12L0ScaleA</p>
</th>
<th class="cellrowborder" valign="top" width="19.47%" id="mcps1.4.8.1.6"><p>CopyL12L0ScaleB</p>
</th>
</tr>
</thead>
<tbody><tr><td class="cellrowborder" valign="top" width="19.07%" headers="mcps1.4.8.1.1 "><p>Ascend 950PR/Ascend 950DT</p>
</td>
<td class="cellrowborder" valign="top" width="15.07%" headers="mcps1.4.8.1.2 "><p>√</p>
</td>
<td class="cellrowborder" valign="top" width="13.459999999999999%" headers="mcps1.4.8.1.3 "><p>√</p>
</td>
<td class="cellrowborder" valign="top" width="13.459999999999999%" headers="mcps1.4.8.1.4 "><p>√</p>
</td>
<td class="cellrowborder" valign="top" width="19.47%" headers="mcps1.4.8.1.5 "><p>√</p>
</td>
<td class="cellrowborder" valign="top" width="19.47%" headers="mcps1.4.8.1.6 "><p>√</p>
</td>
</tr>
</tbody>
</table>
