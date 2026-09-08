# 总体说明<a name="ZH-CN_TOPIC_0000002569070899"></a>

矩阵计算的搬入通过C API将GM或UB中的输入送入L1 Buffer，再准备L0A/L0B矩阵及Bias、MX、量化和激活系数。矩阵搬运、格式转换和辅助寄存器配置分别由对应接口完成，通常与`asc_mmad`、`asc_mmad_mx`或`asc_mmad_sparse`配合使用。

本文按数据通路分类。2201与3510的实现和单位不同，选择重载时同时核对架构、地址空间、数据类型和分形布局。

**表1** 数据通路和存储层级<a name="zh-cn_topic_0000002543771563_table3523123518108"></a>

| 源位置 | 源地址对齐 | 目的位置 | 目的地址对齐 | 架构及说明 |
| --- | --- | --- | --- | --- |
| GM | 普通字节搬运1B；分形/格式转换按所选重载要求 | L1（`__cbuf__`） | 32B，分形搬运还需满足接口要求 | 2201、3510；普通拷贝与ND2NZ不是同一接口 |
| GM | 按GM→L0分形搬运接口要求 | L0A/L0B（`__ca__`/`__cb__`） | 512B | 仅2201 |
| L1 | 32B | L0A/L0B | 512B | 2201、3510；2D、转置或3D展开 |
| L1 | 32B | BiasTable | 64B | `asc_copy_l12bt` |
| L1 | 32B | Fixpipe Buffer | 量化区128B；激活区按对应通路要求 | `asc_copy_l12fb`；系数地址索引单位另见搬出配置 |
| L1 | 32B | L0A_MX/L0B_MX | 32B | 仅3510，搬运E8M0系数 |
| UB | 32B | L1 | 32B | 当前C API的`asc_copy_ub2l1`为3510的AIV侧接口 |

<!-- npu="950" id1 -->
> [!NOTE]说明
> Ascend 950PR/Ascend 950DT因硬件变更，删除GM到L0A Buffer、L0B Buffer的数据通路。产生的影响是原GM到L0A Buffer和L0B Buffer的数据搬运需要拆分为两步，即从GM到L1 Buffer的数据搬运和从L1 Buffer到L0A Buffer、L0B Buffer的数据搬运。
<!-- end id1 -->

## GM->L1 Buffer的接口分类与使用场景<a name="zh-cn_topic_0000002543771563_section_gm2l1"></a>

**表2** 通用GM->L1接口

| 接口类别 | 主要功能 | 支持通路 | 典型应用场景 |
| --- | --- | --- | --- |
| [asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1.md)，连续/高维切分重载 | 拷贝已排布数据，保持内容和格式 | GM→L1 | 完整矩阵、非连续矩阵分片 |
| [asc_copy_gm2l1_nd2nz](../asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz.md) | 搬运并将ND转为NZ | GM→L1 | 线性矩阵输入 |
| [asc_copy_gm2l1](../asc_copy_gm2l1/asc_copy_gm2l1.md)，2D分形重载 | 按分形搬运 | GM→L1 | 离线分形数据 |

**表3** 3510扩展GM->L1接口

| 接口类别 | 主要功能 | 支持通路 | 典型应用场景 |
| --- | --- | --- | --- |
| [asc_copy_gm2l1_dn2nz](../asc_copy_gm2l1_dn2nz.md) | DN→NZ | GM→L1 | 列主序矩阵输入 |
| [asc_copy_gm2l1_align](../asc_copy_gm2l1_align.md) | 非对齐搬运及左右Padding | GM→L1 | 字节长度、边界不满足普通块拷贝要求 |
| [asc_copy_gm2l1（2D）](../asc_copy_gm2l1/asc_copy_gm2l1_2d_arch_3510.md) | 2DV2分形搬运，可配L2 Cache策略 | GM→L1 | 多方向分形块输入 |
| [asc_copy_gm2l1（高维切分）](../asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md) | 支持`asc_channel_pad_mode`通道填充/压缩 | GM→L1 | 按32B通道扩展有效区或去除高位字节 |

## UB->L1 Buffer的接口分类与使用场景<a name="zh-cn_topic_0000002543771563_section_ub2l1"></a>

**表4** UB->L1接口

| 接口类别 | 主要功能 | 支持通路 | 典型应用场景 |
| --- | --- | --- | --- |
| [asc_copy_ub2l1](../../vector_datamove/asc_copy_ub2l1.md)连续重载 | 连续数据搬运 | UB→L1 | 3510矢量结果送入矩阵计算 |
| 同接口高维切分重载 | 按32B块配置长度和间隔 | UB→L1 | 3510非连续矢量结果搬运 |

该接口在AIV侧执行，需要与AIC消费者建立跨核同步。本仓库C API未提供与Basic API的UB→L1随路ND2NZ、非对齐Padding完全对应的公开接口；需要在UB预先处理布局和补齐，或使用已支持的数据通路。硬件具备某通路不等于当前公开C API已封装其全部模式。

## L1 Buffer->L0A/B Buffer的接口分类与使用场景<a name="zh-cn_topic_0000002543771563_section_l12l0"></a>

**表5** 数据与辅助Buffer搬运

| 接口类别 | 主要功能 | 支持通路 | 典型应用场景 |
| --- | --- | --- | --- |
| [asc_copy_l12l0a](../asc_copy_l12l0a/asc_copy_l12l0a.md)、[asc_copy_l12l0b](../asc_copy_l12l0b/asc_copy_l12l0b.md)的2D重载 | 按512B分形搬运 | L1→L0A/L0B | 普通矩阵输入 |
| 同接口的3D重载 | NC1HWC0输入在线img2col | L1→L0A/L0B | 卷积展开 |
| [asc_copy_l12l0a_trans](../asc_copy_l12l0a_trans.md)、[asc_copy_l12l0b_trans](../asc_copy_l12l0b_trans/asc_copy_l12l0b_trans.md) | 拼接分形后转置搬运 | L1→L0A/L0B；3510仅L0B | 特定位宽的块转置 |
| [asc_copy_l12bt](../asc_copy_l12bt/asc_copy_l12bt.md) | Bias搬入 | L1→BiasTable | 累加初值 |
| [asc_copy_l12fb](../asc_copy_l12fb/asc_copy_l12fb.md) | 随路系数搬入 | L1→Fixpipe Buffer | 量化/激活 |

**表6** 2201稀疏搬运

| 接口 | 功能 | 通路 |
| --- | --- | --- |
| [asc_copy_l12l0b_sparse](../asc_copy_l12l0b_sparse.md) | 压缩B数据和索引分别就位 | L1→L0B及IDX Buffer |

**表7** 3510二维和MX扩展

| 接口 | 功能 | 使用方式 |
| --- | --- | --- |
| `asc_copy_l12l0a` / `asc_copy_l12l0b`的2D重载 | 2DV2二维分形搬运，支持FP4/FP8/HiFloat8及整数位宽 | 设置`m_step`、`k_step`和源/目标步长 |
| `asc_copy_l12l0a_transpose` / `asc_copy_l12l0b_transpose` | 同一2DV2指令的转置变体 | 与普通2D重载参数布局对应；不是`_trans`指令 |
| [asc_copy_l12l0a_mx](../asc_copy_l12l0a_mx.md) / [asc_copy_l12l0b_mx](../asc_copy_l12l0b_mx.md) | 只搬运32B分形的E8M0系数 | 与A/B数据搬运分别调用，匹配映射地址 |

## 辅助配置接口说明<a name="zh-cn_topic_0000002543771563_section_aux"></a>

**表8** 辅助配置接口

| 接口 | 功能 | 架构/注意事项 |
| --- | --- | --- |
| [asc_fill_l1](../asc_fill_l1.md)、[asc_fill_l0a](../asc_fill_l0a.md)、[asc_fill_l0b](../asc_fill_l0b.md) | 初始化Buffer | 2201支持三个通路；3510支持L1，L0填充不能跨架构套用 |
| [asc_set_l13d_fmatrix](../asc_set_l13d_fmatrix.md)、[asc_set_l13d_fmatrix_b](../asc_set_l13d_fmatrix_b.md) | FeatureMap尺寸和Padding边界 | 根据A/B侧与`f_matrix_ctrl`选择 |
| [asc_set_l13d_size](../asc_set_l13d_size.md) | L1边界大小 | 2201；按对应3D接口要求 |
| [asc_set_l13d_rpt](../asc_set_l13d_rpt.md) | 3D重复和步长 | 3510优先用`asc_l13d_repeat_direction`枚举重载；B侧重复配置使用`asc_set_l13d_rpt_b` |
| [asc_set_l12l0a_3d_padding](../asc_set_l12l0a_3d_padding.md)、[asc_set_l12l0b_3d_padding](../asc_set_l12l0b_3d_padding.md) | 带类型的Padding值配置 | 3510，注意两侧寄存器及共享配置 |
| [asc_set_l13d_padding](../asc_set_l13d_padding.md) | Padding配置 | 2201按接口位编码使用 |
| [asc_load_image_to_cbuf](../asc_load_image_to_cbuf.md) | 图像搬入L1 | 2201；按该接口支持范围配置 |
| [asc_copy_l12l0c](../asc_copy_l12l0c.md) | L1→L0C初值搬运 | 2201，初始化路径的补充 |
