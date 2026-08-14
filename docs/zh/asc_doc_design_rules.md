# Ascend C资料设计规范

## 1 概述

### 1.1 目标

本规范旨在统一Ascend C资料的设计要求，提升资料质量，帮助开发者高效完成从了解、上手到熟练使用Ascend C进行算子开发的全过程。

### 1.2 资料体系架构

Ascend C资料体系由**五份**核心文档组成，通过交叉链接形成可导航的资料网络。

资料体系架构图如下所示：

<img src="figures/asc_docs_architecture.png" alt="资料体系架构图"  width="850px" height="580px">


**核心导航逻辑**：每个文档既是链接的**发起方**（首次提到其他文档负责的内容时，链接过去），也是链接的**接收方**（别的文档提到本文档负责的内容时，链接回来）。

**五份文档定位与导航原则**：

| 文档 | 负责说清什么 | 别的文档从这里获取什么 | 本文档链接出去的方向 |
|------|------------|-------------------|-----------------|
| **入门教程** | Ascend C概述、环境准备、快速上手实践（HelloWorld、首个算子） | 其他文档可链接到入门教程作为零基础入口 | 编程概念深入→链接到编程指南；首次提到某个API→链接到API参考手册 |
| **编程指南** | 编程模型、编程范式、编译运行、硬件架构、高级编程的核心概念 | 其他文档遇到编程概念时**链接回**编程指南获取权威解释 | 首次提到某个API→链接到API参考手册；首次提到优化/实践→链接到算子实践参考；提到架构版本差异→链接到跨代迁移指南 |
| **API参考手册** | 每个接口的参数定义、使用约束、代码示例、API之间的关联关系 | 编程指南和算子实践参考提到某个API时**链接到**API参考手册获取接口详情 | 前置概念→链接回编程指南；API有跨版本差异→链接到跨代迁移指南 |
| **算子实践参考** | 算子怎么写、性能怎么优化、怎么调试、典型案例 | 编程指南点到为止的实践和优化内容**链接到**算子实践参考展开 | 首次用到某个API→链接到API参考手册；涉及编程概念→链接回编程指南；优化方案因架构版本不同→链接到跨代迁移指南 |
| **跨代迁移兼容性指南** | API兼容策略、架构版本间有哪些变更、具体迁移步骤 | 任何文档涉及版本差异、API废弃/新增时**链接到**迁移指南获取迁移路径 | 概念定义→链接回编程指南；迁移后的新API→链接到API参考手册 |

**补充说明**：
- 样例库（`asc-devkit/examples/`）**不属于**Ascend C资料体系，但五份文档中的代码示例均可链接到样例库
- 技术附录（术语表、原理、语法限制）归入编程指南，作为共享知识基础设施
- 入门教程独立成册，目录位于 `docs/zh/guide/getting_started/`，包含概述、环境准备、快速入门（SIMD/SIMT）

### 1.3 设计要求

本规范从三个维度定义Ascend C资料的设计要求：

| 维度 | 核心问题 | 目标状态 |
|------|---------|---------|
| **可获取性（Discoverability）** | 开发者能否在3步内找到所需信息？ | 从"搜索→猜测→试错"变为"导航→定位→理解" |
| **可读性（Readability）** | 找到后能否无歧义地理解？ | 从"反复推理+交叉验证"变为"一次读完即理解" |
| **完备性（Completeness）** | 理解后信息是否足够完成开发任务？ | 从"文档只给一半，另一半靠踩坑"变为"按文档操作可直接完成" |

此外，跨版本迁移场景（涉及跨代迁移兼容性指南和版本约束标注规范章节）同时涉及上述三个维度：开发者要能找到迁移信息（可获取性）、看懂迁移步骤（可读性）、迁移后功能不退化（完备性）。

### 1.4 条款组织方式

本规范中的条款按照以下结构组织：

- **条款编号**：以"DOC-"为前缀，按章节分类编号，如DOC-DISC-00、DOC-READ-00、DOC-COMP-00等。
- **条款标题**：简明描述条款要求。
- **【类型】**：条款的类型，分为"原则"和"规范"。“原则”指根本性、方向性的指导准则；“规范”指具体、可操作的行为要求。
- **【描述】**：条款的具体要求说明。
- **【正例】**：符合要求的写作示例。
- **【反例】**：不符合要求的写作示例。

---

## 2 可获取性（Discoverability）

### DOC-DISC-00 三层导航体系

【级别】原则

【描述】文档提供三层导航，覆盖不同使用场景：

| 导航层 | 形式 | 适用场景 | 设计要求 |
|--------|------|---------|---------|
| **全局层** | 目录树 | 知道自己要找什么 | 目录≤5级深度 |
| **决策层** | 决策树/对比表/选择指引 | 知道需求但不知道选哪个 | 每个多选岔路口必须有决策树或对比表 |
| **关联层** | 链接/具体介绍 | 找到A后需要了解关联的B | 五份文档互链形成可导航网络 |

**具体要求**：

- 全局层是文档体系的骨架，需要采用目录树展示模块及其下属分类、子模块和具体文档，确保用户可从模块入口逐级定位目标内容；目录层级不超过5级，同级条目按统一维度组织，名称应准确反映文档主题。
- 决策层是在用户面临“多选一”岔路口时，提供结构化比较与推荐方案的导航工具，需要提供决策树、对比表或选择指引；内容应覆盖影响选择的关键差异、适用场景和推荐条件，必要时给出默认推荐，确保用户无需逐页阅读即可完成选择。
- 关联层负责建立文档间的引用网络，需要对文档中提及的关联概念进行具体介绍，若该概念会被多个文档提及，则需将其抽离为独立文档，并通过链接建立引用关系。

【正例】

**全局层**：

如下文档内容逐级组织目录树，同级条目采用统一的分类维度，层级清晰且名称能够准确反映文档主题，便于用户从模块入口快速定位目标接口：

``` text
-   [SIMD API](simd_api/simd_api.md)
    -   [基础API](simd_api/basic_api/basic_api.md)
        -   [Add](simd_api/basic_api/Add.md)
        -   [Sub](simd_api/basic_api/Sub.md)
    -   [C API](simd_api/c_api/c_api.md)
        -   [asc_add](simd_api/c_api/asc_add.md)
        -   [asc_sub](simd_api/c_api/asc_sub.md)
-   [SIMT API](simt_api/simt_api.md)
    -   [acosf](simt_api/acosf.md)
    -   [asinf](simt_api/asinf.md)
```

**决策层**：

如下文档内容中，提供了对比表并给出了推荐使用的场景，令用户能够无需逐页阅读即可完成选择：

| API层级 | 语言 | 特点 | 主要用途 | 推荐使用场景 |
|----------|------|------|----------|----------|
| **TPipe/TQue框架编程API** | **C++** | 基于**Tensor**编程，通过**TPipe/TQue**自动管理内存搬运与同步，屏蔽底层细节。 | 使用框架自动编排数据搬运和计算，提升编程易用性与开发效率。 | 偏好**C++Tensor编程&自动管理同步/内存&高编程易用性** |
| **基础API** | **C++** | 基于**Tensor**编程，提供**C++完备编程能力**，通过`LocalMemoryAllocator`等分配Tensor，由开发者自主管理同步。|自主管理同步与内存布局，适配C\+\+ Tensor开发习惯，开放全部底层硬件能力，支撑精细化调优与极致性能实现。| 偏好**C++Tensor编程&自主管理同步/内存** |
| **语言扩展层SIMD API** | **C** | 基于**指针**编程，提供**C完备编程能力**，通过声明静态数组管理本地内存，由开发者自主管理同步。 | 自主管理同步与内存，适配C语言开发习惯，开放全部底层硬件能力，支撑精细化调优与极致性能实现。| 偏好**指针编程** |

- **关联层**：

如下文档内容为Counter模式的介绍，提及了SetVectorMask，并提供了SetVectorMask的超链接：

``` text
设置Mask模式为Counter模式。该模式下，不需要开发者去感知迭代次数、处理非对齐的尾块等操作，可直接传入计算数据量，实际迭代次数由Vector计算单元自动推断。本接口推荐配合API中isSetMask模板参数使用，当isSetMask为false时，支持用户调用本接口手动管理Counter模式，并通过[SetVectorMask](api/SIMD-API/基础API/Memory矢量计算/掩码操作/SetVectorMask.md)设置Counter模式下参与计算的元素个数。
```

【反例】

**全局层**：

如下目录树中，`GEMV`为6级目录，不符合要求：

``` text
-   [SIMD API](simd_api/simd_api.md)
    -   [基础API](simd_api/basic_api/basic_api.md)
        -   [矩阵计算](SIMD-API/basic_api/cube_compute/cube_compute.md)
            -   [Mmad计算](SIMD-API/basic_api/cube_compute/mmad_compute/mmad_compute.md)
                -   [关键特性说明](SIMD-API/basic_api/cube_compute/mmad_compute/features/features.md)
                    -   [GEMV](SIMD-API/basic_api/cube_compute/mmad_compute/features/GEMV.md)
```   

**决策层**：

如下对比表中，未给出推荐条件，用户无法快速选择需要的模型：

| 编程模型 | 支持范围 | 芯片支持 |
|----------|----------|----------|
| **SIMD(主)** | 向量、矩阵、融合计算 | 昇腾全系列 |
| **SIMT(辅)** | 仅向量计算 | 仅限Ascend 950PR/Ascend 950DT |
| **SIMD+SIMT混合** | 向量、矩阵、融合计算 | 仅限Ascend 950PR/Ascend 950DT |

**关联层**：

如下文档内容中提及UnaryRepeatParams、repeatStride、dataBlockStride等概念，但未进行解释或添加链接：

``` text
控制操作数地址步长的参数。UnaryRepeatParams类型，包含操作数相邻迭代间相同DataBlock的地址步长，操作数同一迭代内不同DataBlock的地址步长等参数。相邻迭代间的地址步长参数说明请参考repeatStride；同一迭代内DataBlock的地址步长参数说明请参考dataBlockStride。
```

### DOC-DISC-01 五文档链接联动

【级别】原则

【描述】五份核心文档通过链接形成可导航网络，**链接方向遵循"谁提到别人负责的内容，谁就加链接"原则**：

**链接关系（9条链接规则）**：

| 链接规则 | 发起方 | 链接到 | 触发时机 | 说明 |
|---------|--------|--------|---------|------|
| L0 | 入门教程 | 编程指南 | 入门教程点到编程概念需深入时 | 入门教程只给快速上手，概念深入链接到编程指南 |
| L1 | 编程指南 | API参考手册 | 首次引入新API名称 | 概念讲解中API名首次出现处加链接 |
| L2 | 编程指南 | 算子实践参考 | 首次引入实践/优化话题 | 编程指南点到为止，链接到实践参考展开 |
| L3 | 编程指南 | 跨代迁移指南 | 提及架构版本差异/废弃API时 | 详见跨代迁移兼容性指南 |
| L4 | 算子实践参考 | API参考手册 | 实践案例中首次使用API | 算子样例代码中API首次出现处链接到接口详情 |
| L5 | 算子实践参考 | 编程指南 | 首次引入编程概念 | 实践中涉及编程模型概念时链接到权威解释 |
| L6 | 算子实践参考 | 跨代迁移指南 | 提及跨架构优化差异时 | 性能优化方案因架构版本不同时链接 |
| L7 | API参考手册 | 编程指南 | 首次引入编程概念 | "前置知识"段链接到概念介绍章节 |
| L8 | API参考手册 | 跨代迁移指南 | 标注API版本差异/废弃信息时 | API页标注版本范围并链接 |

**链接规则**：
- 同一概念/API**首次出现**时加链接，后续重复出现不重复链接
- API参考页面的概念引用就近行内链接（非集中在页面底部）
- **注意方向**：算子实践参考→API参考手册（查接口详情），API参考手册不需要反向链接到算子实践参考
- 所有资料涉及的完整代码样例引用到[样例库](../../examples)

【正例】

**L0**：在如下入门教程中提到了核函数这一概念，概念详细内容链接到编程指南中。

```text
3. **启动NPU计算任务**：调用Device侧预先编写的[核函数](../../programming_guide/programming_model/ai_core_simd_programming/kernel_function.md)，NPU开始并行计算。
```

**L1**：在如下编程指南中首次出现了API CrossCoreSetFlag和CrossCoreWaitFlag，添加对应的超链接。

```text
算子按计算特征可划分为三类：Cube算子（矩阵计算）、Vector算子（矢量计算）和CV融合算子（矩阵与矢量混合计算）。算子类型决定了其核间同步方式与group配置模式的选择。针对不同算子场景，C++ Tensor编程通过[CrossCoreSetFlag](../../../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md)和[CrossCoreWaitFlag](../../../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md)两个接口组合实现核间同步，以满足多样化的算子开发需求。
```

**L2**：在如下编程指南中提及VF融合优化和VF循环优化，链接到算子实践参考中的对应文档。

```text
**提示**：VF融合并非范围越大越好。融合粒度过大可能导致寄存器溢出，反而使性能下降。更多优化手段可以参考[VF融合优化](../../../../operator_practice/simd_operator_optimization/vector_compute/vf_optimization/vf_fusion_optimization.md)和[VF循环优化](../../../../operator_practice/simd_operator_optimization/vector_compute/vf_optimization/vf_loop_optimization.md)。
```

**L3**：在如下编程指南中提及提及架构版本差异，链接到跨代迁移兼容性指南的对应文档。

```text
与NPU架构版本2201相比，NPU架构版本3510的主要差异如下：

**表2**  Membase和Regbase差异

| 计算方式 | 数据暂存位置 | 特点 | 适用场景 |
| --- | --- | --- | --- |
| Membase | Local Memory（UB） | 每步计算结果写回UB | NPU架构版本2201 |
| Regbase | 寄存器（VF Reg） | 中间结果可暂存寄存器，减少UB读写 | NPU架构版本3510 |

>[!NOTE]说明
>- 详细架构变更请参考[2201到3510架构变更](../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_arch_changes.md)。
```

**L4**：在如下算子实践参考文档中首次使用SetGlobalBuffer，链接到该接口的文档。

```text
本样例中的分配方案是：数据整体长度TOTAL\_LENGTH为1 \* 2048，使用GlobalTensor类的[SetGlobalBuffer](../../../../api/SIMD-API/basic_api/data_structures/GlobalTensor/SetGlobalBuffer.md)接口设定该核上Global Memory的起始地址以及长度。
```

**L5**：在如下算子实践参考文档中首次提及矢量编程范式这一编程概念，链接到该概念的权威解释。

```text
基于Ascend C方式实现基础矢量算子核函数的流程如下所示。
-   算子分析：分析算子的数学表达式、输入、输出以及计算逻辑的实现，明确需要调用的Ascend C接口。
-   核函数定义：定义Ascend C算子入口函数。
-   根据[矢量编程范式](../../../programming_guide/programming_model/ai_core_simd_programming/tpipe_tque_programming/tpipe_tque_paradigm.md#section116515238815)实现算子类：完成核函数的内部实现，包括3个基本任务：CopyIn，Compute，CopyOut。
```

**L6**：在如下算子实践参考文档中，性能优化方案因架构版本不同而存在差异，需要链接到跨代迁移指南呈现架构差异。

```text
可以看出bank冲突的场景与Unified Buffer的规格密切相关，规格的变化通常会导致bank冲突场景的变化。不同架构版本的具体差异请参考[2201到3510架构变更](../../../../cross_gen_migration_guide/3510_arch_migration/2201_to_3510_arch_changes.md)。

-   由于NPU架构版本3510的bank group上有两组读口和写口，因此两次读操作访问同一个bank group的不同bank时，不会引起冲突。
-   假设读指令操作的地址为0x0000（bank0），写指令操作的地址为0x10000，在NPU架构版本2201中，地址0x10000（bank16）不会发生读写冲突，而在NPU架构版本3510中，这个地址0x10000（bank0）会引发读写冲突。
```

**L7**：在如下API参考手册文档中首次引入C语言编程这一概念，链接到对应编程概念介绍章节。

```text
C API是Ascend C三层梯度化编程接口中的**语言扩展层SIMD API**，定位为最底层的C语言接口，基于指针编程，提供完备的C语言编程能力。C API可直接映射NPU硬件指令，开发者自主管理内存搬运与同步（与TPipe/TQue自动管理内存、同步不同），开放全部底层硬件能力。适配C语言开发习惯，适合对性能和可控性要求较高的算子开发场景，是追求极致性能、充分释放NPU硬件潜能的核心路径。

详细内容请参考[C语言编程概述](../../../guide/programming_guide/programming_model/ai_core_simd_programming/c_pointer_programming/c_programming_overview.md)。
```

**L8**：在如下API参考手册文档中，Exp在3510架构有对应的函数原型，在产品支持情况中标注该差异并链接到跨代迁移指南文档中的对应章节。

```text
# Exp<a name="ZH-CN_TOPIC_0000001530181537"></a>

## 产品支持情况<a name="section1550532418810"></a>

### 不传入config的原型

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90：支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：支持
<!-- end id9 -->

### 传入config的原型

<!-- npu="950" id10 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id10 -->
<!-- npu="A3" id11 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id11 -->
<!-- npu="910b" id12 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id12 -->
<!-- npu="310b" id13 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id13 -->
<!-- npu="310p" id14 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id14 -->
<!-- npu="310p" id15 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id15 -->
<!-- npu="910" id16 -->
- Atlas 训练系列产品：不支持
<!-- end id16 -->
<!-- npu="x90" id17 -->
- Kirin X90：不支持
<!-- end id17 -->
<!-- npu="9030" id18 -->
- Kirin 9030：不支持
<!-- end id18 -->

>[!NOTE]说明
>- [NPU架构版本3510](../../../../../guide/programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)默认不支持Subnormal功能，因此该接口提供了传入config的函数原型。详细信息请参考[基础API迁移](../../../../..//guide/cross_gen_migration_guide/3510_arch_migration/2201_to_3510_guide/basic_api_migration.md)。
```

### DOC-DISC-02 术语统一入口

【级别】规范

【描述】建立**统一术语对照表**（独立附录文件），所有文档共享：

| 对照内容 | 形式 | 位置 |
|---------|------|------|
| 术语对照表 | 逐条明确各术语的标准定义；若涉及相近或关联概念，一并阐述其内在联系与关键差异，以辅助读者准确理解和区分 | 独立附录文件，概述章链接引用 |

**具体要求**：
- 任何文档中出现的代号/缩写，首次出现时括号注释全称并链接到术语对照表
- 术语表中术语若涉及相近或关联概念，需要一并阐释其内在联系和关键差异，辅助读者准确理解和区分
- 后续对术语的引用需保持一致性，避免出现一种概念多种表达

【正例】建立统一术语对照表，文档中首次出现时括号注释全称并链接到对照表：

| 术语/缩略语 | 含义 |
|---|---|
| Global Memory/GM | 设备端的主内存，AI Core的外部存储，用于存储大规模数据，但需要优化访问模式以提升性能。 |
| Local Memory | AI Core的内部存储，包括L1 Buffer、L0A Buffer、L0B Buffer、L0C Buffer、UB等存储单元。 |
| DMA | Direct Memory Access，直接内存访问单元。<br>负责数据搬运，包括Global Memory和Local Memory之间的数据搬运以及不同层级Local Memory之间的数据搬运，包含搬运单元MTE2、MTE3等。|

```text
[**DMA（Direct Memory Access）搬运单元**](../../../technical_appendix/concepts_and_terms/glossary.md)负责数据搬运，包括Global Memory和Local Memory之间的数据搬入、搬出，以及不同层级Local Memory之间的数据流转。
```

### DOC-DISC-03 文件职责聚焦

【级别】原则

【描述】每个文件有明确的单一职责，概述文件定位为**导航页**：

| 文件类型 | 职责 | 禁止 |
|---------|------|------|
| 概述/总览文件 | 导航：主题列表+1-2句概述+子章节链接 | 展开技术细节、大量代码 |
| 概念介绍文件 | 解释：概念定义+原理+约束+关系 | 重复其他文件已有内容（改为引用） |
| 操作指南文件 | 步骤：代码片段+操作步骤+注意事项 | 概念定义（链接到概念文件） |
| API参考页 | 参考：原型+参数+约束+示例+关联API | 编程模型解释（链接到编程指南） |
| 迁移指导文件 | 步骤：变更清单+验证步骤 | 编程模型解释（链接到编程指南）、重复已有迁移内容（改为引用） |

**具体要求**：
- 如果一个文件超过3个不相关主题，拆分为多个子文件
- 同一内容的详细描述只出现一次（权威版本），其余位置用"详见X章节"引用
- 硬件架构描述统一到一份文件，不允许SIMD版/SIMT版各描述一半

【正例】线程架构文档作为概念介绍文件，只包含线程相关主题，文件职责明确，它的结构如下：

```text
# 线程架构

## 线程层次结构

## Warp执行机制

## 配置最大线程数
```

【反例】根据如下概念介绍文件的结构可以看出，它包含了超过3个不相关主题：

```text
# AI Core SIMT编程模型

## 抽象硬件架构

## 线程架构

## 内存层级

## 核函数

## 同步机制
```

## 3 可读性（Readability）

### DOC-READ-00 按抽象层级使用术语

【级别】原则

【描述】文档按章节的抽象层级使用对应术语，**首次跨界时必须关联**：

| 章节层级 | 使用术语 | 首次跨界处理 |
|---------|---------|-------------|
| 高层（抽象硬件架构、概述） | 抽象名：Global Memory、Local Memory、计算单元 | 不突兀混入UB、L0A等底层名 |
| 低层（编程模型、数据搬运） | 硬件名：UB、L1 Buffer、L0A/B/C | **首次出现时**括号关联：`UB（即抽象架构中Local Memory的一部分）` |
| API层（API参考手册） | API名：DataCopy、LocalTensor | — |

【正例】

**抽象硬件架构**：
```text
## 存储单元和搬运单元

存储单元可以从使用位置上分为Local Memory和Global Memory。

-   **Local Memory**：AI Core片内存储，用于暂存从Global Memory搬入的数据分片，并保存计算输出和中间结果。这些数据可供Vector、Cube等计算单元高效访问，继续参与片内计算，或通过搬运单元搬出到Global Memory。Local Memory包含面向不同编程方式和计算单元的片内资源，如[图2](#fig_simd_reg_memory_hierarchy)所示。
-   **Global Memory**：Device侧全局存储，是Local Memory中数据搬入和搬出的主要来源或目的位置。
```

**编程模型**：
```text
# Memory矢量计算编程

本文介绍基于统一缓存（Unified Buffer，下文简称UB，为抽象架构中Local Memory的一部分）的Ascend C矢量计算开发方法，该计算能力简称Memory矢量计算编程，是Ascend C中最基础、应用最广泛的矢量编程方式。
```

【反例】抽象硬件架构文档直接写"数据通过UB搬入VEC运算"，跳过抽象层。

### DOC-READ-01 易混概念显式区分

【级别】原则

【描述】名称相近、层级易混淆的概念必须用对比表或关系图**显式区分**：

**必须区分的概念组**：

| 概念组 | 区分维度 | 区分形式 |
|--------|---------|---------|
| SPMD vs SIMD vs SIMT | SPMD=编程模型、SIMD=指令执行模式、SIMT=线程执行模式 | 层级关系图 |
| 四步法(Tiling→搬→算→搬) vs TPipe四步(Alloc→EnQue→...) | 编程流程vs流水管理范式 | 对比表 |
| DMA vs MTE vs DataCopy | 三层名同一件事 | 术语映射表 |
| MemBase(基础API) vs RegBase(VF融合API) | 计算位置不同(UB vs寄存器)、Load/Store次数不同 | 对比表+场景推荐 |
| Block vs CTA | Ascend C编程单元vs CUDA等价概念 | 竞品映射表 |
| `LocalTensor` vs `GlobalTensor` vs `TBuf` | 计算用/外部用/临时用缓冲区 | 对照表+场景推荐 |
| `__ubuf__` vs `__cbuf__` vs `__gm__` | UB空间/L1空间/GM空间地址限定符 | 对照表 |
| `asc_`前缀vs `Ascend C::`前缀vs `cce::`前缀 | C API / C++ API / Intrinsics | 命名规则表 |

**具体要求**：
- 在**首次出现混淆可能的位置**就给出区分，不延后
- 禁止用一个概念的代码示例暗示等价于另一概念

【正例】通过以下对照表区分`__ubuf__`、`__cbuf__`、`__gm__`等地址空间限定符：

| 地址空间限定符 | AI Core物理存储空间 |
|----------------|---------------------|
| \_\_gm\_\_ | 设备侧内存GM |
| \_\_ubuf\_\_ | Vector Unified Buffer |
| \_\_ca\_\_ | Cube L0A Buffer |
| \_\_cb\_\_ | Cube L0B Buffer |
| \_\_cc\_\_ | Cube L0C Buffer |
| \_\_cbuf\_\_ | Cube L1 Buffer |
| \_\_fbuf\_\_ | Fixpipe Buffer |
| \_\_ssbuf\_\_ | SSBuffer |


### DOC-READ-02 约束醒目且集中说明

【级别】规范

【描述】所有硬件级/平台级约束在概念**首次定义处**醒目标注，不依赖后续章节或运行时报错暴露：

**约束信息要素**：
- 约束类型：地址对齐/数据类型限制/元素数量范围/格式限制/只读语义/时序要求
- 约束值：具体数值或范围（如"32字节对齐"、"half类型≥128元素"）
- 违反后果：编译报错/运行时越界/结果错误/性能下降

**格式要求**：独立段落，集中列出（不分散在各参数说明小字中）

**放置位置**：
- API参考页：独立的"约束与限制"段落
- 编程指南：概念引入时同步给出约束，而非后续补充

【正例】

**DataCopy（GM与UB连续数据搬运） API参考页中包含独立的约束说明章节**：

```text
## 约束说明

- 位于Global Memory的地址必须按照对应数据类型所占字节数对齐，位于Unified Buffer的地址必须32字节对齐。
- 调用连续搬运接口时，count \* sizeof\(T\)需要32字节对齐，若未对齐，则搬运量会向下取整到32字节对齐。
- 如果需要执行多个DataCopy指令，且DataCopy的目的地址存在重叠，需要通过调用[PipeBarrier(ISASI)](../../sync_control/intra_core_sync/PipeBarrier_ISASI.md)来插入同步指令，保证多个DataCopy指令的串行化，防止出现异常数据。如下图左侧示意图，执行两个DataCopy指令，搬运的目的Global Memory地址存在重叠，两条搬运指令之间需要通过调用`PipeBarrier<PIPE_MTE3>()`添加MTE3搬出流水的同步；如下图右侧示意图所示，搬运的目的地址Unified Buffer存在重叠，两条搬运指令之间需要调用`PipeBarrier<PIPE_MTE2>()`添加MTE2搬入流水的同步。

    ![](../../../../figures/datacopy_address_overlap_sync_diagram.png)

<!-- npu="910b,A3" id11 -->
- 针对如下产品型号：
    - Atlas A2 训练系列产品/Atlas A2 推理系列产品
    - Atlas A3 训练系列产品/Atlas A3 推理系列产品

    在跨卡通信算子开发场景，DataCopy类接口支持跨卡数据搬运，仅支持HCCS物理链路，不支持其他通路；开发者开发过程中，需要关注涉及卡间通信的物理通路，可通过`npu-smi info -t topo`命令查询HCCS物理链路。
<!-- end id11 -->
```
**编程指南中的Memory数据计算章节引入repeat_time概念时同步给出约束**：

```text
#### 迭代控制

矢量计算单元单次迭代从UB读取8个连续的DataBlock（每个32字节），运算结果写入目的UB的8个对应DataBlock。

若设置repeat_time（迭代次数）为2，单元将执行两轮迭代，总处理数据量为2 × 8 × 32字节 = 512字节；若数据类型为half（2字节 / 元素），则对应处理256个元素。

> 📌 硬件约束：repeat_time取值范围为1~255，该约束对[NPU架构版本2201](../../../language_extension/simd_builtin_keywords.md)、[NPU架构版本3510](../../../language_extension/simd_builtin_keywords.md)全系列产品生效。
```

### DOC-READ-03 参数语义图解化

【级别】规范 

【描述】复杂参数（涉及内存布局、维度映射、stride计算）用图解代替纯文字：

| 参数类型 | 图解形式 |
|---------|---------|
| stride/offset类 | 内存布局对照图（标注每步偏移） |
| 维度映射类 | 维度变换映射图（如[K,N]→[N,K]转置） |
| 格式类(ND/NZ) | 两种格式的内存排布对比图 |
| mask类 | 元素对应位掩码图 |
| 数据流类 | 输入→处理→输出流图 |

**判断标准**：一个参数用纯文字需要3句以上才能解释清楚→必须配图。图解与文字描述必须一致。

【正例】通过图解帮助理解dataBlockStride设置为1和设置大于1的场景：

```text
#### 地址间隔配置

矢量计算单元还支持带地址间隔的向量计算能力，可通过dataBlockStride和repeatStride两个参数精准配置：
- dataBlockStride：单次迭代内不同DataBlock间的地址步长，取值不得超过UB的空间大小限制；
- repeatStride：相邻迭代间相同DataBlock的地址步长（下文详细说明）。

连续计算，`dataBlockStride`设置为1，对同一迭代内的8个DataBlock数据连续进行处理。
非连续计算，`dataBlockStride`值大于1（如取2），同一迭代内不同DataBlock之间在读取数据时出现一个DataBlock的间隔，如下图所示。

![dataBlockStride示例](../../../../figures/db_stride.png)
```

【反例】复杂参数使用纯文字解释，且超过三句。

### DOC-READ-04 代码片段精简聚焦

【级别】规范 

【描述】每种编程范式/关键流程提供精简代码片段+完整样例链接：

| 要求 | 说明 |
|------|------|
| 精简聚焦 | 只展示当前讲解相关代码，用 `// ... 其他初始化` 省略无关部分 |
| 可理解 | 每个片段配2-3行文字说明"这段代码做了什么" |
| 可追踪 | 片段末尾链接到样例库完整示例 |
| 关键字注释 | Ascend C特有关键字（`__aicore__`、`__ubuf__`、`__simd_vf__`等）必须行内注释 |
| 参数覆盖 | 覆盖常用参数组合，不仅展示单一用法 |

【正例】如下Memory矢量计算同步控制章节为矢量加法计算流程提供了精简代码片段以及完整样例：

````text
AI Core内部执行单元（如MTE2搬运单元、Vector计算单元等）采用异步并行方式运行，当不同单元读写同一存储资源时，易产生数据依赖问题。因此，Memory矢量计算需通过流水同步接口协调执行顺序，确保计算流程正确。Memory矢量计算流程较Cube矩阵计算更为简洁，主要包含三步：数据搬入（Global Memory → UB）、计算（UB）、数据搬出（UB → Global Memory）。三个步骤分别对应PIPE_MTE2、PIPE_V、PIPE_MTE3流水线，需通过核内同步接口协调执行顺序，确保各步骤按序完成。

```cpp
// Kernel implementation, decorated with __global__ to mark kernel entry point
__global__ __vector__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    uint8_t mutex_id = 1;
    AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
    AscendC::LocalTensor<float> xLocal = ubAllocator.Alloc<float, 48>();
    AscendC::LocalTensor<float> yLocal = ubAllocator.Alloc<float, 48>();
    AscendC::LocalTensor<float> zLocal = ubAllocator.Alloc<float, 48>();
    
    AscendC::GlobalTensor<float> xGlobal, yGlobal, zGlobal;
    xGlobal.SetGlobalBuffer((__gm__ float*)x);
    yGlobal.SetGlobalBuffer((__gm__ float*)y);
    zGlobal.SetGlobalBuffer((__gm__ float*)z);
    
    // ...
    // 1. Step 1: Data transfer in, execution pipeline is PIPE_MTE2
    AscendC::Mutex::Lock<PIPE_MTE2>(mutex_id);
    AscendC::DataCopy(xLocal, xGlobal, 48);
    AscendC::DataCopy(yLocal, yGlobal, 48);
    AscendC::Mutex::Unlock<PIPE_MTE2>(mutex_id);

    // 2. Step 2: Compute, execution pipeline is PIPE_V
    AscendC::Mutex::Lock<PIPE_V>(mutex_id);
    AscendC::Add(zLocal, xLocal, yLocal, 48);
    AscendC::Mutex::Unlock<PIPE_V>(mutex_id);
    
    // 3. Step 3: Data transfer out, execution pipeline is PIPE_MTE3
    AscendC::Mutex::Lock<PIPE_MTE3>(mutex_id);
    AscendC::DataCopy(zGlobal, zLocal, 48);
    AscendC::Mutex::Unlock<PIPE_MTE3>(mutex_id);
}
```

结合上文介绍的Memory矢量数据搬运与计算能力，开发者可基于C++ Tensor编程接口实现完整的Memory矢量计算算子。具体开发流程与代码示例可参考[cpp_api_add样例](../../../../../../../examples/01_simd_cpp_api/00_introduction/01_add/add/README.md)，该样例完整展示了矢量加法算子的开发、编译与验证流程。
````

【反例】代码片段没有文字说明，片段末尾未链接到样例库完整示例：

````text
```cpp
AscendC::Mutex::Lock<PIPE_MTE2>(mutex_id);
AscendC::DataCopy(xLocal, xGlobal, 48);
AscendC::DataCopy(yLocal, yGlobal, 48);
AscendC::Mutex::Unlock<PIPE_MTE2>(mutex_id);
```
````

### DOC-READ-05 前置知识显式铺垫

【级别】原则

【描述】每个概念/API首次出现时提供前置知识铺垫，确保**不逆序阅读**：

- 若章节涉及前置知识，该章节的开头需要放置相关的概念说明
- 编程指南章节按学习路径排列：基础概念→编程模型→编程范式→性能优化（不按功能模块平铺）
- 概念B依赖概念A时，A先出现或在B处有明确引用链接

【正例】如下算子功能设计章节涉及到了aclnn工程化算子开发的前置知识，在开头放置了相关说明：

```text
# 算子功能设计

## 前置理解：aclnn工程化算子开发方式由哪几部分组成

一个aclnn自定义算子工程通常需要完成三类代码。

第一类代码：**算子原型定义**，声明算子的接口信息，包括输入、输出和属性，以及支持的dtype、format。

第二类代码：**Host侧Tiling实现**，在Kernel执行前准备运行参数。Kernel在AI Core上运行前，需要知道本次输入一共有多少数据、如何切核并启动多少个Block、每个Block内部如何继续切分、是否需要额外workspace，以及是否需要设置调度模式等launch配置。

第三类代码：**Kernel侧算子实现**，在AI Core上执行的实际计算。Kernel侧根据Tiling传入的参数，从GM搬运数据到UB，在UB上完成计算，再把结果写回GM。

本文的功能设计，就是在写这三类代码前，先确定每一类代码需要表达什么。
```

【反例】算子功能设计章节没有提供前置概念的说明：

```text
# 算子功能设计

## 围绕算子原型做功能设计

## 围绕Host侧Tiling做功能设计

## 围绕Kernel侧实现做功能设计

## 针对不同场景扩展Kernel实现
```

---

## 4 完备性（Completeness）

### DOC-COMP-00 约束信息零遗漏

【级别】原则

【描述】所有隐式约束在文档中显式记录，不依赖运行时assert或编译错误暴露：

| 约束类别 | 覆盖要求 | 常见遗漏 |
|---------|---------|---------|
| 数据类型限制 | 每个API列出支持的完整数据类型列表 | half类型最小元素数、bf16支持缺失 |
| 地址对齐 | 明确对齐字节数和对齐方向 | 32B对齐仅写在assert中 |
| 元素数量范围 | 明确最小值、最大值、对齐粒度 | "8元素"约束未文档化 |
| 格式限制 | ND/NZ/FRACTAL等格式支持情况 | 格式切换的偏移计算差异 |
| 使用模式限制 | 直调/工程模式/调试模式的差异 | PipeBarrier仅特定模式可用 |
| 多核/多实例限制 | kernel内只读、核间同步要求 | 配置参数只读语义未说明 |
| API组合限制 | 互斥API/必需搭配API | 文档推荐的组合实际不支持 |

**操作要求**：约束信息从规格说明书和assert代码中提取，集中展示在首次定义处。

【正例】如下DataCopy（GM与UB连续数据搬运）API文档内容中列出了全部支持的数据类型以及完整的约束信息：

```text
## 数据类型

源操作数和目的操作数支持的数据类型保持一致，Global Memory -> Unified Buffer和Unified Buffer -> Global Memory两个数据通路对同一产品支持的数据类型相同，具体如下：

<!-- npu="950" id2 -->
- Ascend 950PR/Ascend 950DT，支持的数据类型为：b8、b16、b32、b64。
<!-- end id2 -->

<!-- npu="A3" id3 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double。
<!-- end id3 -->

<!-- npu="910b" id4 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double。
<!-- end id4 -->

<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float、int64_t、uint64_t、double。
<!-- end id5 -->

<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、int32_t、uint32_t、float、int64_t、uint64_t、double。
<!-- end id6 -->

<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、int32_t、uint32_t、float、int64_t、uint64_t、double。
<!-- end id7 -->

<!-- npu="910" id8 -->
- Atlas 训练系列产品，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、int32_t、uint32_t、float、int64_t、uint64_t、double。
<!-- end id8 -->

<!-- npu="x90" id9 -->
- Kirin X90，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、int32_t、uint32_t、float、int64_t、uint64_t、double。
<!-- end id9 -->

<!-- npu="9030" id10 -->
- Kirin 9030，支持的数据类型为：int8_t、uint8_t、int16_t、uint16_t、half、int32_t、uint32_t、float、int64_t、uint64_t、double。
<!-- end id10 -->

## 返回值说明

无

## 约束说明

- 位于Global Memory的地址必须按照对应数据类型所占字节数对齐，位于Unified Buffer的地址必须32字节对齐。
- 调用连续搬运接口时，count \* sizeof\(T\)需要32字节对齐，若未对齐，则搬运量会向下取整到32字节对齐。
- 如果需要执行多个DataCopy指令，且DataCopy的目的地址存在重叠，需要通过调用[PipeBarrier(ISASI)](../../sync_control/intra_core_sync/PipeBarrier_ISASI.md)来插入同步指令，保证多个DataCopy指令的串行化，防止出现异常数据。如下图左侧示意图，执行两个DataCopy指令，搬运的目的Global Memory地址存在重叠，两条搬运指令之间需要通过调用`PipeBarrier<PIPE_MTE3>()`添加MTE3搬出流水的同步；如下图右侧示意图所示，搬运的目的地址Unified Buffer存在重叠，两条搬运指令之间需要调用`PipeBarrier<PIPE_MTE2>()`添加MTE2搬入流水的同步。

    ![](../../../../figures/datacopy_address_overlap_sync_diagram.png)

<!-- npu="910b,A3" id11 -->
- 针对如下产品型号：
    - Atlas A2 训练系列产品/Atlas A2 推理系列产品
    - Atlas A3 训练系列产品/Atlas A3 推理系列产品

    在跨卡通信算子开发场景，DataCopy类接口支持跨卡数据搬运，仅支持HCCS物理链路，不支持其他通路；开发者开发过程中，需要关注涉及卡间通信的物理通路，可通过`npu-smi info -t topo`命令查询HCCS物理链路。
<!-- end id11 -->
```

【反例】API文档中未完整列出支持的数据类型及约束条件，且部分已列出的信息存在描述不准确或与实际不符的情况。

### DOC-COMP-01 示例分层覆盖

【级别】原则

【描述】每个API/编程范式提供示例：

**示例层级定义**：

| 层级 | 定位 | 篇幅 | 要求 |
|------|------|------|------|
| **Minimal** | 最小可运行示例 | <30行核心代码 | 核函数的代码片段和完整样例链接 |
| **Standard** | 典型用法示例 | 50-150行 | 包含核函数定义和调用的代码片段 |

**各文档的具体要求**：
- API参考手册：每个API至少1个典型用法示例（Standard），并在示例中覆盖常用参数组合
- 编程指南：每个编程范式至少1个Minimal示例+样例库完整示例链接
- 算子实践参考：每个算子实践案例至少1个Minimal示例+样例库完整示例链接

【正例】

**Minimal**：

````text
```c
__global__ __vector__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    uint8_t mutex_id = 1;
    // ...
    // 1. Step 1: Data transfer in, execution pipeline is PIPE_MTE2
    asc_lock(PIPE_MTE2, mutex_id);
    asc_copy_gm2ub_align(x_local, x_gm, 1, 48 * sizeof(half), 0, 0, false, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 0, 0);
    asc_copy_gm2ub_align(y_local, y_gm, 1, 48 * sizeof(half), 0, 0, false, asc_load_l2_cache_mode::NORMAL_FIRST_VICTIM, 0, 0);
    asc_unlock(PIPE_MTE2, mutex_id);

    // 2. Step 2: Compute, execution pipeline is PIPE_V
    asc_lock(PIPE_V, mutex_id);
    asc_add(z, x, y, 4096);
    asc_unlock(PIPE_V, mutex_id);
    
    // 3. Step 3: Data transfer out, execution pipeline is PIPE_MTE3
    asc_lock(PIPE_MTE3, mutex_id);
    asc_copy_ub2gm_align(z_gm, z_local, 1, 48 * sizeof(int8_t), asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, 0, 0);
    asc_unlock(PIPE_MTE3, mutex_id);
}
```
完整工程示例可参考：[c_api_add样例](../../../../../../../examples/02_simd_c_api/00_introduction/01_add/c_api_delicacy_async_add)。
````

**Standard**：

````text
```cpp
template <uint32_t blockLength>
__vector__ __global__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    // ...
}

int32_t main(int32_t argc, char* argv[])
{
    // ...
    add_custom<blockLength><<<numBlocks, 0, stream>>>(xDevice, yDevice, zDevice);
    // ...

}
```
````

### DOC-COMP-03 API关联与组合使用说明

【级别】原则

【描述】每个API页面说明同一版本内API之间的组合使用关系和禁忌。若API废弃需要链接到实现此功能的新API页面。

**要求**：
- 互斥组合用独立警告框标注（如"BatchMatmul与SetQuantVector组合不支持"）
- 功能相近API族提供选择决策树或对照表
- 废弃API需要链接到实现此功能的新API页面
- 若API需与其他接口协同工作，应明确列出相关API

【正例】

**互斥组合用独立警告框标注**：

```text
>[!NOTE]说明
>- Batch Matmul不支持量化/反量化模式，即不支持[SetQuantScalar](../../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetQuantScalar.md)、[SetQuantVector](../../../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetQuantVector.md)接口。
```

**功能相近API族提供选择决策树或对照表**：

```text
Compare、Compares称之为比较接口，用于逐元素比较Tensor或Scalar的大小，并根据比较结果生成掩码。推荐使用场景如下：

**表1**  比较接口推荐使用场景

| API | 核心功能 | Mask类型 | 推荐使用场景 |
| --- | --- | --- | --- |
| [Compare](Compare.md) | 逐元素比较两个tensor大小，如果比较后的结果为真，则输出结果的对应比特位为1，否则为0。支持多种比较模式（等于、小于、大于...）。<br>不支持Vector计算的Mask设置。结果存入UB。 | 未使用 | 逐元素比较，结果写入内存。 |
| [Compare（结果存入寄存器）](Compare_store_to_register.md) | 逐元素比较两个tensor大小，如果比较后的结果为真，则输出结果的对应比特位为1，否则为0。支持多种比较模式。<br>支持Vector计算的Mask设置。结果暂存CmpMask寄存器。详情参考[表2 CmpMask与VectorMask](#cmpMask-vectorMask)。 | src传参mask用于Vector计算；<br>输出结果为CmpMask（比较掩码）。 | 逐元素比较，结果写寄存器。 |
| [Compares](Compares.md) | 逐元素比较一个tensor中的元素和另一个scalar的大小，如果比较后的结果为真，则输出结果的对应比特位为1，否则为0。<br>支持多种比较模式。 | 未使用 | 张量和标量逐元素比较，结果写入内存。 |
```

**废弃API链接到实现此功能的新API页面**:

```text
## 功能说明

**该接口已废弃，请使用[asc_set_atomic_add_int32](../simd_atomic/asc_set_atomic_add.md)替代。**
```

**API需与其他接口协同工作，明确列出相关API**:
```
WaitPreBlock和NotifyNextBlock必须与[InitDetermineComputeWorkspace](InitDetermineComputeWorkspace.md#)接口配合使用，这三个接口组合使用能够确保多个AIV核严格按照blockIdx的升序执行，适用于要求确定性计算的场景，确定性计算的具体含义请参考[确定性计算](../../atomic_operations/key_features.md)。
```

### DOC-COMP-04 跨代迁移覆盖完备

【级别】原则

【描述】跨代迁移兼容性指南必须覆盖完整的迁移路径，确保开发者在架构升级时不因文档缺失而受阻：

**必须覆盖的迁移要素**：

| 迁移要素 | 覆盖要求 |
|---------|---------|
| 架构变更清单 | 列出两个架构版本间所有影响编程的差异项 |
| API兼容策略 | 对各类API兼容性产生的影响进行说明，并提供兼容性适配方案 |
| 迁移后验证步骤 | 迁移完成后如何验证功能正确性和性能基线 |
| 编译选项变更 | 不同架构版本的编译参数差异 |

**操作要求**：
- 架构变更描述中涉及的编程概念变更（如新增执行模式、内存模型差异）必须链接回编程指南的对应概念章节
- 迁移实践中的优化技巧应链接到算子实践参考的对应优化章节】

【正例】

**架构变更清单**:

```text
具体来说，3510架构的主要变更如下各表所示。

- 搬运单元

    **表1**  搬运单元变更
    | 3510变更 | 产生的影响 | 影响的API接口 |
    |----------|------------|---------------|
    | 删除L1 Buffer到GM的数据通路。 | 现有接口不支持从L1 Buffer直接搬运数据到GM。开发者需要在L1 Buffer分配一块空间存放单位矩阵，利用MMAD矩阵乘法计算输出到L0C Buffer，从L0C Buffer通过[Fixpipe（L0C到GM数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)将数据搬运到GM。 | DataCopy/DumpTensor |
    | 删除GM到L0A Buffer、L0B Buffer的数据通路。 | 原GM到L0A Buffer和L0B Buffer的数据搬运需要拆分为两步，即从GM到L1 Buffer的数据搬运和从L1 Buffer到L0A Buffer、L0B Buffer的数据搬运。 | LoadData |
    ...
```

**API兼容策略**：

````text
-   **3510架构版本删除L0A Buffer/L0B Buffer初始化的相关硬件指令。**

    **说明**：Fill接口将特定存储位置的LocalTensor初始化为某一具体数值，不支持直接初始化L0A Buffer、L0B Buffer。

    **兼容方案**：先通过Fill接口初始化L1 Buffer，再通过LoadData接口将L1 Buffer上的数据搬运到L0A Buffer、L0B Buffer。具体代码可参考[Fill兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/fill)。

    以GM-\>L1 Buffer-\>L0A Buffer的数据通路为例：

    1.  初始化L1 Buffer。

        ```cpp
        __aicore__ inline void InitConstA1(AscendC::LocalTensor<T>& a1Local)
        {
            AscendC::Fill(a1Local, {1, static_cast<uint16_t>(M * K * sizeof(T) / 32), 0, 1});
        }
        ```

    2.  调用LoadData接口将L1 Buffer上的数据搬运到L0A Buffer。

        ```cpp
        __aicore__ inline void Load2DA1ToA2(AscendC::LocalTensor<T>& a1Local, AscendC::LocalTensor<T>& a2Local)
        {
            AscendC::LoadData2DParamsV2 loadDataParams;
            ...
            AscendC::LoadData(a2Local, a1Local, loadDataParams);
        }
        ```
````

**编译选项变更**：

````text
- 异构编译场景，开发者使用命令行或者编写Cmake文件进行编译的情况，需要手动修改NPU架构版本号或者AI处理器型号。以修改NPU架构版本号为例，更改编译命令行或编译工程CMakeLists.txt文件中的--npu-arch配置，示例如下：

    ```
    ...
    
    target_compile_options(demo PRIVATE
        // 将dav-xxxx更换为对应NPU架构版本号
        $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-xxxx>
    )
    ```
````

### DOC-COMP-05 版本约束标注规范

【级别】规范

【描述】五份文档中涉及版本差异的内容必须统一标注格式，确保开发者一眼识别适用范围：

**标注格式**：`[适用版本：xxx]`+`> **注意**：xxx`

| 标注场景 | 格式 | 示例 |
|---------|------|------|
| 约束因版本不同 | `> **版本差异**：2201要求32B对齐；3510要求512B对齐` | `src_stride → 版本差异见迁移指南` |
| 性能优化方案因版本不同 | `[适用版本：仅2201]`/`[适用版本：仅3510]` | `IBShare → [适用版本：仅2201]` |

**操作要求**：
- 编程指南中涉及版本差异的硬件参数/约束，在参数值旁用`[适用版本：xxx]`标注
- 算子实践参考中版本特定的优化方案，在标题或开头用`[适用版本：仅xxx]`标注
- 跨代迁移指南中每个条目使用统一格式，不自行发明新格式

【正例】

**约束因版本不同**：

```text
### 关键分形格式详解

矩阵分形格式用于定义多维张量在内存中的排布规则。受硬件读取逻辑影响，不同产品型号对矩阵乘法 C = A × B 的格式要求存在差异：

> **版本差异**：2201要求左矩阵A使用Zz格式，右矩阵B使用Zn格式，结果矩阵C使用Nz格式；3510要求左矩阵A使用Nz格式，右矩阵B使用Zn格式，结果矩阵C使用Nz格式。
```

**性能优化方案因版本不同**：

```text
# 避免bank冲突（NPU架构版本2201）

[适用版本：仅2201]
```

【反例】五份文档中涉及版本差异的内容未统一格式。

---

## 5 三维度交叉设计

部分设计要求横跨多个维度，需同时满足：

| 设计要求 | 可获取性 | 可读性 | 完备性 | 具体操作 |
|---------|---------|--------|--------|---------|
| **术语对照表** | 统一入口 | 按层用词 | — | 建立独立附录文件 |
| **决策树/对比表** | 三层导航 | 易混区分 | API关联 | 每个多选岔路口提供 |
| **代码示例** | 链接样例库 | 精简聚焦 | 分层覆盖 | API页至少1个Standard示例；指南和实践提供Minimal示例及完整样例链接 |
| **约束框** | — | 前置醒目 | 零遗漏 | 约束信息在首次定义处 |
| **参数图解** | — | 图解化 | 约束可视化 | stride/layout/mask类等复杂参数必须配图 |
| **版本约束标注** | 一眼识别版本范围 | 版本差异不混淆 | 迁移路径零遗漏 | 统一格式标注，五份文档一致 |
| **跨代迁移映射** | — | 迁移步骤可理解 | 变更清单零遗漏 | 列出架构变化、API兼容策略、验证步骤和编译选项 |
