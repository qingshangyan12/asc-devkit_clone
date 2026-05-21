# Git 冲突解决方案

## 冲突概况

当前正在进行 rebase 操作（dev1 分支 rebase 到 97c74793），有 8 个冲突文件。

**冲突原因**：
- HEAD (dev1): 添加了新的样例目录条目
- Incoming (docs分支): 按字母顺序重新排列目录结构，并添加"支持的产品"列

## 解决策略

保留 HEAD 新增 + 应用 docs 分支的排序和产品列。

---

## 文件 1: examples/01_simd_cpp_api/00_introduction/03_fusion_operation/README.md

**冲突行**: 9-21

**替换为**:
```markdown
| 目录名称 | 功能描述 | 支持的产品 |
| ------------------------------------------------------------ | ---------------------------------------------------- | --- |
| [gather_adds_simt_simd_hybrid](./gather_adds_simt_simd_hybrid) | 基于SIMT和SIMD实现Gather和Adds融合计算 | Ascend 950PR/Ascend 950DT |
| [matmul_leakyrelu_basic_api](./matmul_leakyrelu_basic_api) | 基于基础API实现Matmul矩阵乘与LeakyRelu激活函数计算融合 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [matmul_leakyrelu_high_level_api](./matmul_leakyrelu_high_level_api) | 基于高阶API实现Matmul矩阵乘与LeakyRelu激活函数计算融合 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [matrix_transpose](./matrix_transpose/) | SIMD与SIMT混合编程下使用SIMT实现矩阵转置 | Ascend 950PR/Ascend 950DT |
```

---

## 文件 2: examples/01_simd_cpp_api/00_introduction/04_vector_reg/README.md

**冲突行**: 9-16

**替换为**:
```markdown
|  目录名称                                                   |  功能描述                                               | 支持的产品 |
| ------------------------------------------------------------ | ----------------------------------------------------  | --- |
| [add](./add) | 基于Reg编程接口的Add样例  | Ascend 950PR/Ascend 950DT |
```

---

## 文件 3: examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/README.md

**冲突行**: 9-41

**替换为**:
```markdown
| 目录名称 | 功能描述 | 支持的产品 |
| --------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------- | --- |
| [auxscalar_reg](./auxscalar_reg) | 本样例演示使用AuxScalar方式从UB读取多个标量数据参与计算 | Ascend 950PR/Ascend 950DT |
| [broadcast_ub2l0c](./broadcast_ub2l0c) | 本样例基于BroadCastVecToMM实现数据广播搬运，将位于UB（Unified Buffer）上数据进行广播并搬运到CO1（L0C Buffer） | Atlas 推理系列产品AI Core |
| [copy_ub2ub](./copy_ub2ub) | 本样例基于Copy实现数据搬运，适用于VECIN，VECOUT之间的数据搬运，支持mask连续模式和counter模式。 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [data_copy_gm2ub_nddma](./data_copy_gm2ub_nddma) | 本样例介绍如何使用多维数据搬运接口实现GM（Global Memory）到UB（Unified Buffer）通路的数据搬运，通过自由配置搬入的维度信息以及对应的Stride，可以用于Padding、Transpose、BroadCast、Slice等多种数据变换操作 | Ascend 950PR/Ascend 950DT |
| [data_copy_gm2ub_slice](./data_copy_gm2ub_slice) | 本样例基于DataCopy实现数据切片搬运，提取多维Tensor数据的子集进行GM（Global Memory）与UB（Unified Buffer）通路之间的搬运 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [data_copy_l0c2gm](./data_copy_l0c2gm) | 本样例在卷积场景下，基于DataCopy实现数据随路量化激活搬运 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [data_copy_pad_gm2ub_ub2gm](./data_copy_pad_gm2ub_ub2gm) | 本样例基于DataCopyPad实现数据非32字节对齐数据的搬运，并进行数据填充 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [data_copy_ub2l1](./data_copy_ub2l1) | 本样例在Mmad矩阵乘场景下，基于DataCopy实现UB（Unified Buffer）到L1（L1 Buffer）的数据搬运 | Ascend 950PR/Ascend 950DT |
| [gather_ld_reg](./gather_ld_reg) | 本样例演示使用Gather接口实现离散数据搬入，包含高维Gather（源为LocalTensor）和Reg::GatherB（按DataBlock收集）两种场景 | Ascend 950PR/Ascend 950DT |
| [ld_st_reg_align](./ld_st_reg_align) | 本样例基于Reg编程接口实现UB(Unified Buffer)对RegTensor(Reg矢量计算基本单元)的连续和非连续的对齐数据搬运操作 | Ascend 950PR/Ascend 950DT |
| [ld_st_reg_mask](./ld_st_reg_mask) | 本样例基于Reg编程接口实现UB(Unified Buffer)对MaskReg(掩码寄存器)的搬入搬出，以及使用mask进行掩码搬出的操作 | Ascend 950PR/Ascend 950DT |
| [ld_st_reg_unalign](./ld_st_reg_unalign) | 本样例基于Reg编程接口实现UB(Unified Buffer)对RegTensor(Reg矢量计算基本单元)的非对齐数据搬运操作 | Ascend 950PR/Ascend 950DT |
| [move_reg](./move_reg) | 本样例演示调用MaskGenWithRegTensor接口实现将掩码从RegTensor(Reg矢量计算基本单元)搬入MaskReg(掩码寄存器)的操作 | Ascend 950PR/Ascend 950DT |
| [scatter_st_reg](./scatter_st_reg) | 本样例演示使用Reg::Scatter接口实现离散数据搬出（按元素分散到UB） | Ascend 950PR/Ascend 950DT |
```

---

## 文件 4: examples/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/README.md

**冲突行**: 9-47

**替换为**:
```markdown
| 目录名称 | 功能描述 | 支持的产品 |
| -------------------------------- | --------------------------------------------------------------------------------------------- | --- |
| [abs](./abs) | 本样例基于Reg编程接口实现Abs运算，Relu/Exp/Sqrt/Ln/Log/Log2/Log10/Neg接口皆可参考该样例 | Ascend 950PR/Ascend 950DT |
| [arange](./arange) | 本样例基于Reg编程接口实现Arange运算，以标量值为起始生成递增/递减索引序列 | Ascend 950PR/Ascend 950DT |
| [cast](./cast) | 本样例基于Reg编程接口实现Cast运算，用于RegTensor数据类型转换（位宽大转小/小转大） | Ascend 950PR/Ascend 950DT |
| [compare](./compare) | 本样例基于Reg编程接口实现Compare运算，用于比较两个向量或向量与标量的大小关系 | Ascend 950PR/Ascend 950DT |
| [data_relayout](./data_relayout) | 本样例基于Reg编程接口演示Interleave和Pack接口实现数据重排（Interleave交织 + Pack低位提取） | Ascend 950PR/Ascend 950DT |
| [duplicate](./duplicate) | 本样例基于Reg编程接口实现Duplicate运算（标量填充），将标量值填充到向量的每个位置 | Ascend 950PR/Ascend 950DT |
| [gather](./gather) | 本样例基于Reg编程接口演示Gather接口（源操作数为寄存器）按索引收集元素，使用LoadAlign/StoreAlign的postUpdate模式 | Ascend 950PR/Ascend 950DT |
| [histogram](./histogram) | 本样例基于Reg编程接口实现Histogram运算，对输入数据进行直方图统计计算 | Ascend 950PR/Ascend 950DT |
| [mergemode](./mergemode) | 本样例基于Reg编程接口综合演示MaskMergeMode::MERGING + postUpdate + UpdateMask + ReduceMax | Ascend 950PR/Ascend 950DT |
| [mul](./mul) | 本样例基于Reg编程接口实现Mul运算，Add/Sub/Div/Max/Min/Prelu接口皆可参考该样例 | Ascend 950PR/Ascend 950DT |
| [muls](./muls) | 本样例基于Reg编程接口实现Muls运算，Adds/Maxs/Mins/LeakyRelu接口皆可参考该样例 | Ascend 950PR/Ascend 950DT |
| [reduce](./reduce) | 本样例基于Reg编程接口实现Reduce运算（SUM模式），Reduce接口支持SUM/MAX/MIN归约模式 | Ascend 950PR/Ascend 950DT |
| [reduce_block](./reduce_block) | 本样例基于Reg编程接口实现ReduceDataBlock运算（SUM模式），对每个DataBlock(32B)内元素归约 | Ascend 950PR/Ascend 950DT |
| [reduce_pair](./reduce_pair) | 本样例基于Reg编程接口实现PairReduceElem运算（SUM模式），对相邻奇偶元素对进行归约求和 | Ascend 950PR/Ascend 950DT |
| [select](./select) | 本样例基于Reg编程接口实现Select运算，根据掩码在两个向量之间选取元素 | Ascend 950PR/Ascend 950DT |
| [squeeze](./squeeze) | 本样例基于Reg编程接口实现Squeeze运算，将向量压缩为标量 | Ascend 950PR/Ascend 950DT |
| [truncate](./truncate) | 本样例基于Reg编程接口实现Truncate运算，将浮点数截断到整数位 | Ascend 950PR/Ascend 950DT |
```

---

## 文件 5: examples/01_simd_cpp_api/03_libraries/01_activation/softmaxgrad/README.md

**冲突行**: 72-77

**替换为**（保留更规范的格式）:
```markdown
  请根据当前环境上 CANN 开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

```

---

## 文件 6: examples/01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/README.md

**冲突行**: 8-20

**替换为**:
```markdown
|  目录名称  |  功能描述  | 支持的产品 |
| -------------------------------------------------- | ---------------------------------------------------- | --- |
| [gelu_eltwise_high_performance](./gelu_eltwise_high_performance) | 本样例以Gelu+Element-wise计算为例，介绍RegBase的向量性能调优方法，样例展示并行度调整、Loop拆分和循环展开之后的性能收益情况。 | Ascend 950PR/Ascend 950DT |
| [gelu_high_performance](./gelu_high_performance) | 本样例以Gelu计算为例，介绍RegBase的向量性能调优方法，样例展示使能VF融合和循环展开之后的性能收益情况。 | Ascend 950PR/Ascend 950DT |
| [optimize_vf_continious_align](./optimize_vf_continious_align) | 本样例演示了SIMD场景下，基于连续非对齐搬运接口LoadUnAlign/StoreUnAlign进行搬运优化的算子实现。 | Ascend 950PR/Ascend 950DT |
| [optimize_vf_dual_instr](./optimize_vf_dual_instr) | 本样例演示了SIMD场景下，基于Reg编程接口下VF指令双发优化的样例，通过合理拆分VF循环，适当搬出中间结果到UB，减少数据依赖。 | Ascend 950PR/Ascend 950DT |
| [optimize_vf_fusion](./optimize_vf_fusion) | 本样例演示了SIMD场景下，基于Reg编程接口，通过VF融合优化算子代码实现。 | Ascend 950PR/Ascend 950DT |
| [optimize_vf_loop](./optimize_vf_loop) | 通过循环内成员变量访问优化、循环内指令分布优化、循环内地址管理优化等手段优化VF循环。 | Ascend 950PR/Ascend 950DT |
```

---

## 文件 7: examples/01_simd_cpp_api/04_best_practices/04_memory_access_practices/README.md

**冲突行**: 6-17

**替换为**:
```markdown
|  目录名称                                                   |  功能描述                                              | 支持的产品 |
| ------------------------------------------------------------ | ----------------------------------------------------  | --- |
| [bank_conflict](./bank_conflict) | 基于Ascend C的bank冲突性能优化样例。 | |
| [gm_address_conflict](./gm_address_conflict) | 本样例介绍了同地址冲突的影响以及两种解决方法，并提供核函数直调方法。 | Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [optimize_datacopy](./optimize_datacopy) | 基于搬运类API使用的优化样例，通过<<<>>>直调的实现方式，介绍了减少无效数据搬运、减少搬运指令数量等方法。 | |
```

---

## 文件 8: examples/01_simd_cpp_api/05_compatibility_guide/README.md

**冲突行**: 10-28

**替换为**:
```markdown
|  目录名称  |  功能描述  | 支持的产品 |
| -------------------------------------------------- | ---------------------------------------------------- | --- |
| [data_copy_l1togm](./data_copy_l1togm) | 本样例演示L1数据搬运到GM的端到端流程。 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [fill](./fill) | 本样例展示如何使用Fill接口对L0A Buffer和L0B Buffer进行初始化。 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [matmul_s4](./matmul_s4) | 本样例演示 int4 矩阵乘计算的端到端实现。 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [matmul_s4_910B](./matmul_s4_910B) | 本样例直接使用Matmul高阶API进行矩阵计算。 | Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [matmul_s4_950](./matmul_s4_950) | 新架构下Cube计算单元删除int4b_t数据类型。用户可以在算子侧通过MIX模式在Vector Core进行int4b_t到int8_t的Cast转换，再通过UB搬运到L1后进行Mmad计算。 | Ascend 950PR/Ascend 950DT |
| [pattern_transformation](./pattern_transformation) | 本样例基于基础的mmad样例，演示了 L1 Buffer -> L0A Buffer 通路的分形转换逻辑。 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [scatter](./scatter) | 本样例演示数据离散功能，可根据输入张量和目的地址偏移张量，将输入张量分散到结果张量中。 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
| [set_loaddata_boundary](./set_loaddata_boundary) | 本样例实现L1 Buffer边界值的设置。 | Ascend 950PR/Ascend 950DT、Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品 |
```

---

## 后续步骤

1. 手动编辑上述 8 个文件，将冲突标记区域替换为对应内容
2. 检查每个文件的修改是否符合预期
3. 执行以下命令完成 rebase：

```bash
git add examples/01_simd_cpp_api/00_introduction/03_fusion_operation/README.md
git add examples/01_simd_cpp_api/00_introduction/04_vector_reg/README.md
git add examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/README.md
git add examples/01_simd_cpp_api/02_features/03_basic_api/03_reg_vector_compute/README.md
git add examples/01_simd_cpp_api/03_libraries/01_activation/softmaxgrad/README.md
git add examples/01_simd_cpp_api/04_best_practices/02_reg_vector_compute_practices/README.md
git add examples/01_simd_cpp_api/04_best_practices/04_memory_access_practices/README.md
git add examples/01_simd_cpp_api/05_compatibility_guide/README.md
git rebase --continue
```
