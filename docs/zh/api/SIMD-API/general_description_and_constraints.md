# 通用说明和约束<a name="ZH-CN_TOPIC_0000001714258422"></a>

本文介绍[基础API](basic_api/basic_api.md)、[高阶API](adv_api/adv_api.md)、[C API](c_api/c_api.md)和[Tensor API](tensor_api/tensor_api.md)共同适用的说明和约束。

## API使用说明

SIMD API包括C API、Tensor API、基础API和高阶API。不同类型API的编程方式存在差异，使用方法请参考[AI Core SIMD编程](../../guide/programming_guide/programming_model/ai_core_simd_programming/ai_core_simd_programming.md)中的以下内容：

| API类型 | 编程方式 | 参考文档 |
| --- | --- | --- |
| C API | 基于C语言指针编程，由开发者管理本地内存和同步。 | [C语言编程概述](../../guide/programming_guide/programming_model/ai_core_simd_programming/c_pointer_programming/c_programming_overview.md) |
| Tensor API | 基于携带Layout信息的扩展Tensor编程。 | [C++ Tensor编程概述](../../guide/programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/cpp_tensor_programming_overview.md) |
| 基础API | 基于C++基础Tensor编程，可由开发者管理本地内存和同步，也可结合TPipe和TQue管理资源与流水同步。 | [C++ Tensor编程概述](../../guide/programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/cpp_tensor_programming_overview.md)、[TPipe和TQue编程范式](../../guide/programming_guide/programming_model/ai_core_simd_programming/tpipe_tque_programming/tpipe_tque_paradigm.md) |
| 高阶API | 基于C++ Tensor编程，通常结合TPipe和TQue编程范式使用。 | [TPipe和TQue编程范式](../../guide/programming_guide/programming_model/ai_core_simd_programming/tpipe_tque_programming/tpipe_tque_paradigm.md) |

## 通用约束

以下约束适用于C API、Tensor API、基础API和高阶API中的Device侧接口。Host侧接口的约束以具体API文档中的说明为准。

- AI Core采用分级存储架构。执行计算前，需将数据从Global Memory搬运至对应计算单元可访问的片上存储；计算完成后，再根据业务需要将结果搬运至Global Memory。
- 开发者需保证内存访问不超出已分配的存储空间。操作数的起始地址需满足所在存储单元的地址对齐要求，具体请参见[通用地址对齐约束](#section796754519912)。
- AI Core中的数据搬运单元、Vector计算单元和Cube计算单元可异步并行执行。存在数据依赖或内存复用时，需通过相应编程方式提供的同步机制保证读写时序正确。
- 不同产品、数据类型、接口和功能场景可能存在额外约束。调用API前，需确认具体API文档中的产品支持情况和约束说明；若具体API文档与本节的通用约束存在差异，以具体API文档为准。

### 所需头文件和库文件<a name="section1299152225418"></a>

按所用API类别包含公共头文件。若具体API另有头文件或链接要求，应遵循该接口的说明。头文件中未在API资料中公开的接口，不作为对外接口使用。

**表1**  各类API的头文件和链接说明

| API类别 | 常用入口头文件 | 说明 |
| --- | --- | --- |
| 基础API | `${INSTALL_DIR}/asc/include/kernel_operator.h` | 接口头文件位于`${INSTALL_DIR}/asc/include/basic_api`目录<br>&bull; 基础API不涉及依赖的库问。 |
| 高阶API | `${INSTALL_DIR}/asc/include/kernel_operator.h` | 接口头文件位于`${INSTALL_DIR}/asc/include/adv_api`和`${INSTALL_DIR}/asc/include/tiling`目录。<br>使用配套Host Tiling接口时，需要链接`libtiling_api.a`。 |
| C API | `${INSTALL_DIR}/asc/include/c_api/asc_simd.h` | 除试验接口外，可通过该入口包含接口。<br>最细粒度公共头文件见[C API列表](c_api/c_api.md)。 |
| Tensor API | `${INSTALL_DIR}/asc/include/tensor_api/tensor.h` | 统一入口及与C API的配合方式见[Tensor API概述](tensor_api/tensor_api.md)。 |

> [!NOTE]
> `${INSTALL_DIR}`请替换为CANN软件安装后文件存储路径。以root用户安装为例，安装后文件默认存储路径为：`/usr/local/Ascend/cann`。

### 数据存储位置的描述方式

本节分别说明四类API对数据存储位置的描述方式：

- 基础API和部分高阶API的`GlobalTensor`、`LocalTensor`使用逻辑位置（[TPosition](basic_api/aux_data_structures/TPosition.md)）表达存储位置，从而隐藏硬件架构差异。
- Tensor API不使用`TPosition`，而是通过`location`和`PtrPattern`描述物理存储位置。
- C API使用带地址空间限定符的指针类型描述存储位置。
- 不同描述方式不能直接互换，具体接口仍以自身文档为准。

#### 基础API和高阶API：TPosition

`TPosition`是基础API和部分高阶API使用的逻辑存储位置类型。常见取值及其物理映射见表2；同一逻辑位置在不同产品上的物理映射可能不同，以芯片过滤内容为准。

**表2**  TPosition与物理内存映射关系

<a name="table07372185712"></a>

| TPosition | 物理存储位置 |
| --- | --- |
| GM | Global Memory |
| VECIN | UB |
| VECCALC | UB |
| VECOUT | UB |
| A1 | L1 Buffer |
| A2 | L0A Buffer |
| B1 | L1 Buffer |
| B2 | L0B Buffer |
| C1 | <!-- npu="950" id1 -->Ascend 950PR&950DT系列产品，L1 Buffer。<br><!-- end id1 --><!-- npu="910" id2 -->Atlas训练系列产品，Unified Buffer（UB）。<br><!-- end id2 --><!-- npu="310p" id3 -->Atlas推理系列产品AI Core，UB。<br><!-- end id3 --><!-- npu="910b" id4 -->Atlas A2系列产品，L1 Buffer。<br><!-- end id4 --><!-- npu="A3" id5 -->Atlas A3系列产品，L1 Buffer。<br><!-- end id5 --><!-- npu="310b" id6 -->Atlas 200I/500 A2推理产品，UB。<!-- end id6 --> |
| C2 | <!-- npu="950" id7 -->Ascend 950PR&950DT系列产品，BiasTable Buffer。<br><!-- end id7 --><!-- npu="910" id8 -->Atlas训练系列产品，L0C Buffer。<br><!-- end id8 --><!-- npu="310p" id9 -->Atlas推理系列产品AI Core，L0C Buffer。<br><!-- end id9 --><!-- npu="910b" id10 -->Atlas A2系列产品，BiasTable Buffer。<br><!-- end id10 --><!-- npu="A3" id11 -->Atlas A3系列产品，BiasTable Buffer。<br><!-- end id11 --><!-- npu="310b" id12 -->Atlas 200I/500 A2推理产品，BiasTable Buffer。<!-- end id12 --> |
| CO1 | L0C Buffer |
| CO2 | <!-- npu="950" id13 -->Ascend 950PR&950DT系列产品，Global Memory。<br><!-- end id13 --><!-- npu="910" id14 -->Atlas训练系列产品，UB。<br><!-- end id14 --><!-- npu="310p" id15 -->Atlas推理系列产品AI Core，UB。<br><!-- end id15 --><!-- npu="910b" id16 -->Atlas A2系列产品，Global Memory。<br><!-- end id16 --><!-- npu="A3" id17 -->Atlas A3系列产品，Global Memory。<br><!-- end id17 --><!-- npu="310b" id18 -->Atlas 200I/500 A2推理产品，Global Memory。<!-- end id18 --> |
| TSCM | L1 Buffer |
| SPM | <!-- npu="950" id19 -->Ascend 950PR&950DT系列产品，Global Memory。<br><!-- end id19 --><!-- npu="910" id20 -->Atlas训练系列产品，L1 Buffer。<br><!-- end id20 --><!-- npu="310p" id21 -->Atlas推理系列产品AI Core，L1 Buffer。<br><!-- end id21 --><!-- npu="910b" id22 -->Atlas A2系列产品，Global Memory。<br><!-- end id22 --><!-- npu="A3" id23 -->Atlas A3系列产品，Global Memory。<!-- end id23 --> |
| C2PIPE2GM | <!-- npu="950" id24 -->Ascend 950PR&950DT系列产品，Fixpipe Buffer。<br><!-- end id24 --><!-- npu="910b" id25 -->Atlas A2系列产品，Fixpipe Buffer。<br><!-- end id25 --><!-- npu="A3" id26 -->Atlas A3系列产品，Fixpipe Buffer。<!-- end id26 --> |

#### Tensor API：location和PtrPattern

Tensor API通过`location`枚举和`PtrPattern`标记Tensor或指针的物理存储位置，不使用`TPosition`。常见位置如下表所示；实际支持范围和布局组合以对应Tensor API接口文档为准。

**表3**  Tensor API物理存储位置

| location | 物理存储位置 | 说明 |
| --- | --- | --- |
| `location::gm` | Global Memory | 设备端的主内存，AI Core的外部存储，用于存储大规模数据，但需要优化访问模式以提升性能。 |
| `location::ub` | Unified Buffer（UB） | AI Core内部存储单元，主要用于矢量计算，与逻辑内存AscendC::TPosition::VECIN、AscendC::TPosition::VECOUT、AscendC::TPosition::VECCALC相对应。 |
| `location::l1` | L1 Buffer | AI Core内部物理存储单元，空间相对较大，通常用于缓存矩阵计算的输入数据。矩阵计算的输入一般需要从GM搬运到L1 Buffer，然后分别搬运到L0A Buffer和L0B Buffer。L1 Buffer与逻辑内存AscendC::TPosition::A1、AscendC::TPosition::B1相对应。 |
| `location::l0a` | L0A Buffer | AI Core内部物理存储单元，通常用于存储矩阵计算的左矩阵，与逻辑内存AscendC::TPosition::A2相对应。 |
| `location::l0b` | L0B Buffer | AI Core内部物理存储单元，通常用于存储矩阵计算的右矩阵，与逻辑内存AscendC::TPosition::B2相对应。 |
| `location::l0c` | L0C Buffer | AI Core内部物理存储单元，通常用于存储矩阵计算的结果，与逻辑内存AscendC::TPosition::CO1相对应。 |
| `location::bias` | BiasTable Buffer | 偏置存储，AI Core内部物理存储单元，通常用于存储矩阵计算所需的Bias（偏置）数据，与逻辑内存AscendC::TPosition::C2相对应。 |
| `location::fixbuf` | Fixpipe Buffer | AI Core内部物理存储单元，通常用于存储Fixpipe搬运过程中所需的量化参数等数据，与逻辑内存AscendC::TPosition::C2PIPE2GM相对应。 |

Tensor API中的`location`表示物理位置类别，`PtrPattern`用于将该位置类型与指针或迭代器绑定。创建Tensor或内存指针时，位置标记必须与实际存储位置一致。

#### C API：地址空间限定符

C API通常直接使用带地址空间限定符的指针表达操作数所在的存储位置。限定符不等同于`TPosition`，也不等同于Tensor API的`location`类型。

**表4**  C API常用地址空间限定符

| 地址空间限定符 | 物理存储位置 | 说明 |
| --- | --- | --- |
| `__gm__` | Global Memory | 设备端的主内存，AI Core的外部存储，用于存储大规模数据，但需要优化访问模式以提升性能。 |
| `__ubuf__` | Unified Buffer（UB） | AI Core内部存储单元，主要用于矢量计算，与逻辑内存AscendC::TPosition::VECIN、AscendC::TPosition::VECOUT、AscendC::TPosition::VECCALC相对应。 |
| `__cbuf__` | L1 Buffer | AI Core内部物理存储单元，空间相对较大，通常用于缓存矩阵计算的输入数据。矩阵计算的输入一般需要从GM搬运到L1 Buffer，然后分别搬运到L0A Buffer和L0B Buffer。L1 Buffer与逻辑内存AscendC::TPosition::A1、AscendC::TPosition::B1相对应。 |
| `__ca__` | L0A Buffer | AI Core内部物理存储单元，通常用于存储矩阵计算的左矩阵，与逻辑内存AscendC::TPosition::A2相对应。 |
| `__cb__` | L0B Buffer | AI Core内部物理存储单元，通常用于存储矩阵计算的右矩阵，与逻辑内存AscendC::TPosition::B2相对应。 |
| `__cc__` | L0C Buffer | AI Core内部物理存储单元，通常用于存储矩阵计算的结果，与逻辑内存AscendC::TPosition::CO1相对应。 |
| `__fbuf__` | Fixpipe Buffer | AI Core内部物理存储单元，通常用于存储Fixpipe搬运过程中所需的量化参数等数据，与逻辑内存AscendC::TPosition::C2PIPE2GM相对应。 |

### 通用地址对齐约束<a name="section796754519912"></a>

AI Core上的存储单元用于存储矢量计算、矩阵计算的源操作数和目的操作数。下表给出常见存储单元的架构基线对齐要求和已知架构或场景差异。**架构基线不等同于所有接口的最终约束；如果具体API或对应架构文档已明确说明操作数起始地址对齐要求，则以具体API或架构文档中的说明为准。**

**表5**  不同存储单元的对齐要求

| 存储单元 | 架构基线对齐要求 | 架构或场景差异 |
| --- | --- | --- |
| Global Memory | 1Byte对齐。 | 例如[asc_copy_gm2ub](c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md)的GM源地址需要1Byte对齐，但数据类型仍可能要求2Byte或4Byte对齐；[asc_ndim_copy_gm2ub](c_api/vector_datamove/asc_ndim_copy_gm2ub.md)按数据类型对GM源地址提出对齐要求。 |
| UB | 32Byte对齐。 | 普通搬运接口如[asc_copy_gm2ub](c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md)要求UB目的地址32Byte对齐；NDDMA接口[asc_ndim_copy_gm2ub](c_api/vector_datamove/asc_ndim_copy_gm2ub.md)和Reg非对齐搬出接口[asc_storeunalign_post](c_api/reg_compute/store/asc_storeunalign_post.md)的地址对齐要求不同，具体以接口说明为准。 |
| L1 Buffer | 32Byte对齐。 | [asc_copy_gm2l1_align](c_api/cube_datamove/cube_compute_load/asc_copy_gm2l1_align.md)的L1目的地址需要32Byte对齐；矩阵搬运接口还需满足数据布局或分形约束。 |
| L0A Buffer/L0B Buffer | 512Byte对齐。 | [asc_copy_l12l0a_2d_arch_3510](c_api/cube_datamove/cube_compute_load/asc_copy_l12l0a_2d_arch_3510.md)和[asc_copy_l12l0b_2d_arch_3510](c_api/cube_datamove/cube_compute_load/asc_copy_l12l0b_2d_arch_3510.md)分别对L0A、L0B目的地址提出512Byte对齐要求，具体接口还需满足对应分形格式约束。 |
| L0C Buffer | 64Byte对齐。 | 在NPU架构版本3510中，[asc_mmad](c_api/cube_compute/mmad_compute/asc_mmad.md)计算场景使用1024Byte对齐；[asc_copy_l0c2gm_arch_3510](c_api/cube_datamove/cube_compute_store/asc_copy_l0c2gm_arch_3510.md)矩阵计算搬出场景使用64Byte对齐。 |
| BiasTable Buffer | 64Byte对齐。 | 具体搬运接口如[asc_copy_l12bt_arch_3510](c_api/cube_datamove/cube_compute_load/asc_copy_l12bt_arch_3510.md)和[asc_copy_l12bt_arch_2201](c_api/cube_datamove/cube_compute_load/asc_copy_l12bt_arch_2201.md)的参数约束为准。 |
| Fixpipe Buffer | 按参数空间或使用场景确定。 | 在NPU架构版本3510中，[asc_copy_l12fb_arch_3510](c_api/cube_datamove/cube_compute_load/asc_copy_l12fb_arch_3510.md)的随路Vector量化参数空间要求128Byte对齐，随路ReLU参数空间要求64Byte对齐；NPU架构版本2201参见[asc_copy_l12fb_arch_2201](c_api/cube_datamove/cube_compute_load/asc_copy_l12fb_arch_2201.md)。 |

对于NPU架构版本3510，Fixpipe Buffer的量化参数空间和ReLU参数空间使用相互独立的地址空间，不能将两个空间的对齐要求混用。详细说明请参见[数据搬运总体约束](c_api/data_move_guide/overview/overall_constraints.md)和[L1到Fixpipe Buffer数据搬运](c_api/data_move_guide/l1_fixpipe_data_move.md)。

<a id="通用地址重叠约束"></a>

### 基础API地址重叠约束<a name="section668772811100"></a>

使用基础API的Tensor高维切分计算接口时，为了节省地址空间，开发者可以定义一个Tensor，供源操作数与目的操作数同时使用（即地址重叠）。使用时需要注意以下约束：

-   单次迭代内：源操作数与目的操作数必须100%完全重叠，不支持部分重叠。
-   多次迭代间：不支持前序迭代的目的操作数与后序迭代的源操作数重叠。例如，第N次迭代的目的操作数是第N+1次的源操作数（如下图所示）。在这种情况下，第N次迭代可能会改写覆盖源操作数的数值，导致无法得到预期结果。特别地，对于部分双目计算类的API（Add、Sub、Mul、Max、Min、AddRelu、SubRelu），当数据类型为half、int32\_t、float时，支持前序迭代的目的操作数与后序迭代的源操作数重叠：仅针对目的操作数和第二个源操作数重叠的情况，且src1RepStride或者dstRepStride必须为0。
-   多次迭代间：不支持前序迭代的目的操作数与后序迭代的目的操作数重叠。例如，第一轮循环写入的DataBlock编号为0、2、4、6、8、10、12、14，第二轮循环写入的编号为8、10、12、14、16、18、20、22，即上一轮迭代会踩踏前一轮迭代的后4个DataBlock，导致无法得到预期的结果。

**图1**  地址重叠示例（不支持）<a name="fig12654416181216"></a>  
![](../figures/address_overlap_unsupported.png "地址重叠示例（不支持）")

> [!NOTE]说明 
>本节所述基础API地址重叠约束适用于一般情况，API参考中如有额外特殊说明的，则以具体API中的说明为准。
>API中没有描述地址重叠约束的，视为不支持Tensor高维切分计算的地址重叠，地址重叠时计算结果可能不满足预期。
