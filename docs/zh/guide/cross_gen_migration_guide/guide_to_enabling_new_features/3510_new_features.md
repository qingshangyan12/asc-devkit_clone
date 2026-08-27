# 3510新特性使用说明

本文介绍[NPU架构版本3510](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)新增特性的功能、适用场景、使用约束及使能方法，为开发者将算子从其他架构迁移至3510架构提供参考。有关3510架构的整体变更，请参见[2201到3510架构变更](../3510_arch_migration/2201_to_3510_arch_changes.md)。

## 特性总览

**表 1**  3510新特性总览<a name="table_3510_new_feature_overview"></a>

| 特性 | 特性说明 | 相关基础API接口 | 相关C API接口 |
|------|---------|-----------------|----------------|
| [新增UB->L1搬运数据通路](#section_ub2l1) | 支持将数据从UB直接搬运至L1 Buffer，无需经由GM中转，从而缩短数据搬运链路并提升性能。使用该通路时，需配置`ENABLE_CV_COMM_VIA_SSBUF=true` | [DataCopy（UBToL1）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_continuous.md)、[DataCopyPad（UBToL1）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopyPad_UBToL1.md) | [asc_copy_ub2l1](../../../api/SIMD-API/c_api/vector_data_move/asc_copy_ub2l1.md) |
| [新增L1->UB搬运数据通路](#section_l12ub) | 支持Mix场景下将数据从L1 Buffer直接搬运至指定Vector核的UB（按subBlockId写入），无需经过GM中转 | [DataCopyL1ToUB](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopyL1ToUB.md) | [asc_copy_l12ub](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l12ub.md) |
| [新增L0C->UB搬运数据通路](#section_fixpipe_l0c2ub) | 支持将矩阵计算结果从L0C Buffer直接搬运到UB（Fixpipe实现，L0C->UB不支持DataCopy）；L0C->L1 Buffer/L0C->GM支持Fixpipe和DataCopy两种方式 | [Fixpipe（L0CToUB）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md)、[DataCopy（L0CToL1）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopy_L0CToL1.md)、[DataCopy（L0CToGM）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopy_L0CToGM.md) | [asc_copy_l0c2ub](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l0c2ub.md)、[asc_copy_l0c2l1（arch3510）](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md)、[asc_copy_l0c2gm（arch3510）](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md) |
| [新增ND-DMA多维数据搬运](#section_nddma) | 扩展DataCopy的数据搬运能力，支持配置搬入数据的维度信息和Stride，适用于转置、Broadcast、Slice、Padding等场景 | [DataCopy（GMToUB NDDMA）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMToUB_NDDMA.md)、[NdDmaDci](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/NdDmaDci.md) | [asc_ndim_copy_gm2ub](../../../api/SIMD-API/c_api/vector_data_move/asc_ndim_copy_gm2ub.md) |
| [DataCopy/DataCopyPad新增loop模式](#section_loop_mode) | 支持L1 Buffer与GM之间、GM与UB之间通路的loop模式搬运，配合DataCopyPad可实现五维数据搬运 | [SetLoopModePara](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/SetLoopModePara.md)、[ResetLoopModePara](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/ResetLoopModePara.md) | [asc_set_gm2l1_loop_size等](../../../api/SIMD-API/c_api/cube_data_move/asc_set_gm2l1_loop_size.md)、[asc_set_ub2gm_loop1_stride等](../../../api/SIMD-API/c_api/vector_data_move/asc_set_ub2gm_loop1_stride.md) |
| [LoadData新增MX矩阵搬运](#section_mx) | LoadData新增支持MX场景数据搬运，将矩阵数据与其量化系数矩阵同步搬入L0A/L0B及L0A_MX/L0B_MX | [LoadData（2D MX搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_MX.md)、[MmadMx](../../../api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/MmadMx.md) | [asc_copy_l12l0a_mx](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l12l0a_mx.md)、[asc_copy_l12l0b_mx](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l12l0b_mx.md)、[asc_mmad_mx](../../../api/SIMD-API/c_api/cube_compute/asc_mmad_mx.md) |
| [LoadDataWithTranspose新增分形转置能力](#section_transpose) | 新增DN分形和`LoadData2dTransposeParamsV2`参数，支持在搬运过程中完成分形转置；L1 Buffer->L0A Buffer通路不再支持transpose | [LoadDataWithTranspose](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadDataWithTranspose.md) | [asc_copy_l12l0b_trans（arch3510）](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md) |
| [Fixpipe新增NZ2DN/NZ2ND随路转换](#section_nz2dn) | Fixpipe搬出过程支持NZ2DN、NZ2ND随路格式转换 | [Fixpipe（L0CToGM/L1/UB）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md) | [asc_set_l0c2gm_nz2nd](../../../api/SIMD-API/c_api/cube_compute/asc_set_l0c2gm_nz2nd.md) |
| [矩阵计算基础API扩展](#section_cube_api_ext) | 新增LoadData 2D V2/BitMode、LoadDataWithStride、MmadMx、MmadBitMode等矩阵计算基础API，扩展矩阵搬运与计算能力 | 对应基础API（详见[矩阵计算基础API扩展](#section_cube_api_ext)） | [asc_copy_gm2l1（arch3510）](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md)等 |
| [矢量计算基础API扩展](#section_vec_api_ext) | 新增flexible_scalar标量变体、ISASI接口、交织/逻辑计算等矢量计算基础API，扩展矢量编程能力 | 对应基础API（详见[矢量计算基础API扩展](#section_vec_api_ext)） | [asc_add/asc_mul等寄存器算术C API](../../../api/SIMD-API/c_api/reg/arithmetic_compute/asc_add.md)等 |
| [原子操作接口](#section_atomic_api) | 新增GM地址原子操作接口（原子加/交换/比较交换/最大/最小） | [AtomicAdd/AtomicMax等](../../../api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md) | [asc_atomic_add/asc_atomic_cas等](../../../api/SIMD-API/c_api/scalar_compute/asc_atomic_add.md) |
| [系统变量与工具接口](#section_sys_api) | 新增特殊寄存器访问（GetSpr/ClearSpr等）、UB大小/矢长查询、数值极限、性能打点等系统变量与工具接口 | 对应基础API（详见[系统变量与工具接口](#section_sys_api)） | [asc_get_vf_len等C系统变量接口](../../../api/SIMD-API/c_api/sys_var/asc_get_vf_len.md) |
| [新增DCI数据缓存失效能力](#section_dci) | 支持在算子尾部使用DCI（DataCacheInvalid）指令替代默认的DCCI指令，跳过将DCache脏数据回写至GM的Clean过程，从而降低算子尾部开销 | / | [asc_dci](../../../api/SIMD-API/c_api/cache_ctrl/asc_dci.md) |
| [新增TSCM资源管理接口](#section_tscm) | 新增TSCM存储位置（TPosition::TSCM）及队列管理接口，用于矩阵计算中临时数据交换 | [TSCM::AllocTensor/EnQue等](../../../api/SIMD-API/basic_api/resource_management/TSCM/AllocTensor.md) | / |
| [新增SSBuffer核内存储单元](#section_ssbuf) | 新增SSBuffer核内存储单元，支持AIC核与AIV核间的直接通信（Scalar访问），无需经过GM（需配置`ENABLE_CV_COMM_VIA_SSBUF=true`） | [GetSsbufBaseAddr](../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSsbufBaseAddr.md) | / |
| [新增Mutex核内同步能力](#section_mutex) | 新增Mutex能力，用于核内异步流水指令之间的同步控制，功能类似于传统CPU中的锁机制 | [Mutex::Lock/Unlock](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)、[AllocMutexID](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/AllocMutexID_ISASI.md) | [asc_lock](../../../api/SIMD-API/c_api/sync/asc_lock.md)、[asc_unlock](../../../api/SIMD-API/c_api/sync/asc_unlock.md) |
| [新增核间同步控制模式](#section_crosscore) | CrossCoreSetFlag/CrossCoreWaitFlag新增模式4，AIV0与AIV1可单独触发AIC等待 | [CrossCoreSetFlag/CrossCoreWaitFlag（ISASI）](../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md) | [asc_sync_inter_arrive/wait](../../../api/SIMD-API/c_api/sync/asc_sync_inter_arrive.md) |
| [新增Regbase矢量计算方式](#section_regbase) | AIV核采用Regbase架构，支持直接操作Vector寄存器，中间结果可暂存寄存器，减少UB读写 | [Reg::RegTensor/MaskReg等Reg基础API](../../../api/SIMD-API/basic_api/reg_vector_compute/overview.md) | / |
| [Matmul高阶API新增数据类型支持](#section_matmul_dtype) | Matmul类高阶API新增支持fp8/hifloat8/float/bfloat16_t/half/int8_t等数据类型矩阵乘及MX矩阵乘场景 | [Matmul高阶API](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/Matmul_usage.md)、[SetTensorScaleA/B](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorScaleA.md) | [asc_mmad等Cube计算C API](../../../api/SIMD-API/c_api/c_api.md) |
| [高阶API扩展能力总览](#section_adv_api_ext) | 数学计算、归约、排序、张量变换、量化等类高阶API扩展支持新的数据类型、算法配置与排布转换场景，并新增位运算/量化/随机数等950独占高阶API | 对应高阶API（详见[高阶API扩展能力总览](#section_adv_api_ext)） | / |
| [HCCL通信新增能力](#section_hccl) | HCCL新增SetReduceType和AlltoAllvWrite接口，并支持HCCL_SERVER_TYPE_CCU服务端类型 | [SetReduceType](../../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Tiling/SetReduceType.md)、[AlltoAllvWrite](../../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AlltoAllvWrite.md) | / |

> [!NOTE]
> 除上表中所列特性以外，3510还新增以下内容：
>
> - **新增SIMT编程模型**（[详细说明](#section_simt)）：该模型为独立编程模型，配套使用[SIMT-API接口集](../../../api/SIMT-API/SIMT-API.md)，涵盖同步与内存栅栏、原子操作、Warp函数、数学函数、访存函数、地址空间函数和协作组等功能，与上表中的基础API和高阶API（C++ Tensor方式）分属不同的编程体系，因此未纳入上表。该模型仅支持Ascend 950PR/Ascend 950DT，编译时需配置`--enable-simt`。
> - **新增Tensor API编程接口**：该接口是基于`tensor_api/tensor.h`的试验接口，采用“内存指针+布局（Layout）”绑定的编程范式，由Layout自动推导搬运和计算参数，与上表中的基础API和高阶API（C++ Tensor方式）不属同一编程体系，因此未纳入上表。接口清单请参见[数据结构（TensorAPI）](../../../api/SIMD-API/basic_api/data_structure_TensorAPI/data_structure_TensorAPI.md)和[矩阵计算（TensorAPI，试验特性）](../../../api/SIMD-API/basic_api/cube_compute_TensorAPI/cube_compute_TensorAPI.md)。

## 特性说明

### 新增UB->L1搬运数据通路<a name="section_ub2l1"></a>

#### 特性应用场景说明

- 在矩阵计算（Cube计算）相关算子中，例如Matmul算子以及Vector与Cube融合算子，Vector核（AIV）侧计算或搬运产生的矩阵数据存放于UB（TPosition为VECIN、VECCALC或VECOUT），并作为Cube核（AIC）侧矩阵乘加计算的输入。典型数据流为：首先将数据从GM搬运至UB，再从UB搬运至L1 Buffer，最后通过LoadData将数据从L1 Buffer搬运至L0A/L0B Buffer并执行Mmad矩阵乘加计算。
- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等较早架构上，UB与L1 Buffer之间没有直接的硬件搬运通路。UB->L1 Buffer数据搬运需经由GM中转（UB->GM->L1 Buffer），或借助Matmul高阶API的workspace GM空间实现，搬运链路较长且会产生额外开销。
- 3510架构新增UB->L1 Buffer硬件数据通路。使能该通路后，数据可从UB直接搬运至L1 Buffer，无需经由GM中转，从而降低搬运开销。

#### 功能说明

本特性涉及UB->L1 Buffer数据搬运基础API，接口定义在`basic_api/kernel_operator_data_copy_intf.h`中。相关API及其功能如下：

**表 2**  UB->L1 Buffer数据搬运API说明<a name="table_ub2l1_api"></a>

| API | 功能说明 | 函数原型 |
|-----|---------|---------|
| [DataCopy（UBToL1连续数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_continuous.md) | 将矩阵从UB连续搬运至L1 Buffer，数据搬运时格式和内容保持不变 | `DataCopy(dst, src, count)` |
| [DataCopy（UBToL1高维切分数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_highdim_split.md) | 将矩阵从UB搬运至L1 Buffer，支持连续搬运和非连续搬运 | `DataCopy(dst, src, DataCopyParams)` |
| [DataCopy（UBToL1随路转换-ND2NZ搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_ND2NZ.md) | 将矩阵从UB搬运至L1 Buffer，搬运过程中完成ND->NZ格式转换 | `DataCopy(dst, src, Nd2NzParams)` |
| [DataCopyPad（UBToL1非对齐数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopyPad_UBToL1.md) | 将矩阵从UB搬运至L1 Buffer，支持非对齐数据搬运及按需填充 | `DataCopyPad(dst, src, DataCopyExtParams, Nd2NzParams)` |

- 数据通路选择：

  - 使能编译选项[`ENABLE_CV_COMM_VIA_SSBUF`](#section_enable_option)后，使用SSBuffer进行通信，数据通过UB->L1 Buffer之间的硬件通道直接搬运（推荐），该方式无需经过GM中转，性能较高。
  - 未使能该编译选项时，数据需经由GM搬运至L1 Buffer。在此场景下，UB->L1 Buffer搬运接口采用软件仿真实现，并基于Matmul高阶API将其workspace GM空间作为中转空间。使用接口前，需通过`REGISTER_MATMUL`注册Matmul高阶API。

- 数据约束：

  - 源操作数位于UB（TPosition为VECIN/VECCALC/VECOUT），目的操作数位于L1 Buffer（矩阵计算场景下对应TPosition为A1/B1等）。
  - 源地址和目的地址均要求32字节对齐；连续搬运时`count * sizeof(T)`需要32字节对齐，若未对齐，搬运量会向下取整到32字节对齐。
  - 支持的数据类型请参考对应API文档的"数据类型"章节。

- 同步约束：

  - UB->L1 Buffer搬运（PIPE_MTE3）前，需要先保证UB数据就绪。若UB数据来源于GM->UB搬运（PIPE_MTE2），需要先通过SetFlag/WaitFlag（如MTE2_MTE3事件）完成搬运同步。
  - UB->L1 Buffer搬运在Vector核（AIV）侧发起，搬运完成后需要与Cube核（AIC）进行核间同步（如CrossCoreSetFlag/CrossCoreWaitFlag），待L1 Buffer数据就绪后再发起LoadData读取及Mmad计算。
  - 多笔搬运的目的地址在L1 Buffer上存在重叠时，搬运指令之间需要通过PipeBarrier插入同步，保证搬运指令串行化，防止出现异常数据。

#### 使能说明

使能该特性需要配置如下编译选项：

**表 3**  编译选项说明<a name="section_enable_option"></a>

| 编译选项 | 可选值 | 说明 |
|---------|-------|------|
| `ENABLE_CV_COMM_VIA_SSBUF` | `true` | 控制是否使用SSBuffer和UB->L1 Buffer硬件通路。涉及CV通信（AIC与AIV之间的通信）或相关数据搬运API时，需根据实际场景配置该选项。使能后，可提升相关API的性能或启用扩展功能。该选项默认关闭，仅支持Ascend 950PR/Ascend 950DT。 |

编译时配置方式如下：

- 使用bisheng命令行编译：

  ```shell
  bisheng <source_file>.asc -o <output_file> --npu-arch=dav-3510 -DENABLE_CV_COMM_VIA_SSBUF=true
  ```

- 使用CMake编译工程：

  ```cmake
  ascendc_compile_definitions(demo PRIVATE
      -DENABLE_CV_COMM_VIA_SSBUF=true
  )
  ```

> [!NOTE]
> - 为保证兼容性，从其他硬件平台迁移至3510平台的算子默认不使能该编译选项。如需使用UB->L1 Buffer硬件通路，需显式配置`ENABLE_CV_COMM_VIA_SSBUF=true`。更多信息请参见[内置编译宏开关](../../programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md#内置编译宏开关)。
> - 开关关闭（未使能硬件通路）时，使用UB->L1 Buffer搬运接口前需要先使用`REGISTER_MATMUL`注册Matmul高阶API。

使能硬件通路后，在AIV侧执行UB->L1 Buffer搬运的代码片段示例如下，完整示例请参考[典型样例说明](#section_ub2l1_sample)。

```cpp
if ASCEND_IS_AIV {
    // 将矩阵A从GM搬运到UB
    AscendC::DataCopy(ubA1Local, aGM, aSize);
    AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);
    // 将矩阵A从UB直接搬运到L1 Buffer（UB->L1硬件通路）
    AscendC::DataCopy(a1Local, ubA1Local, aSize);
    // 搬运完成后通知AIC核读取L1 Buffer数据
    AscendC::CrossCoreSetFlag<0x2, PIPE_MTE3>(0x8);
}
```

#### 典型样例说明<a name="section_ub2l1_sample"></a>

- 样例位置：[data_copy_ub2l1样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_ub2l1)。样例功能：在Mmad矩阵乘场景下，基于DataCopy实现UB->L1数据搬运，通过编译选项`SCENARIO_NUM`切换连续搬运（场景1）和随路ND2NZ搬运（场景2）两种场景，样例完整数据流为GM->UB->L1->L0A/L0B->Mmad->L0C->GM。样例编译工程已通过`ascendc_compile_definitions`配置`-DENABLE_CV_COMM_VIA_SSBUF=true`。
- 样例编译运行方式：

  ```shell
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..
  make -j
  python3 ../scripts/gen_data.py --scenarioNum=1
  ./demo
  ```

### 新增L1->UB搬运数据通路<a name="section_l12ub"></a>

#### 特性应用场景说明

- 在Vector与Cube融合（Mix）算子场景下（AIC:AIV=1:2），Cube核（AIC）侧的数据或计算结果存放于L1 Buffer，而Vector核（AIV）侧的后续计算需在UB中读取这些数据。相应地，Vector核的计算结果也可能需要写回L1 Buffer，以参与下一轮Mmad计算。
- 3510架构新增L1 Buffer->UB数据搬运能力。`DataCopyL1ToUB`接口仅支持Ascend 950PR/Ascend 950DT，可在Mix场景下由Cube核发起搬运，并根据`subBlockId`将数据从L1 Buffer直接搬运至指定Vector核的UB。

#### 功能说明

本特性涉及[DataCopyL1ToUB（L1到UB数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopyL1ToUB.md)接口。该接口定义在`basic_api/kernel_operator_data_copy_intf.h`中。具体功能如下：

- 支持将数据从L1 Buffer连续搬运至UB，同时支持非连续搬运（按`DataCopyParams`参数配置的repeat模式）。
- 函数原型：

  ```cpp
  // 连续搬运
  template <typename T, uint8_t subBlockId = 0>
  __aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)

  // 同时支持非连续搬运和连续搬运
  template <typename T, uint8_t subBlockId = 0>
  __aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
  ```

- 使用要点与约束：

  - 该接口仅适用于Mix算子场景，且仅支持Cube核数与Vector核数比例为1:2的配置；在该场景下，subBlockId仅支持取值0或1。
  - 如果需要执行多个DataCopyL1ToUB指令，且指令的目的地址存在重叠，需要通过调用[PipeBarrier（ISASI）](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md)插入同步指令，保证多个指令的串行化，防止出现异常数据。
  - 搬运完成后，若Vector核需要读取UB数据，需要先完成与Cube核的核间同步（如CrossCoreSetFlag/CrossCoreWaitFlag）。

### 新增L0C->UB搬运数据通路<a name="section_fixpipe_l0c2ub"></a>

#### 特性应用场景说明

- 在矩阵计算相关算子（如Matmul算子、Vector与Cube融合算子）中，Cube核（AIC）侧Mmad矩阵乘加计算的结果存放于L0C Buffer（TPosition为CO1等），后续若需要在Vector核（AIV）侧进行归一化、激活等向量计算，需要先将L0C Buffer中的数据搬运到UB。
- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等较早架构上，L0C Buffer与UB之间没有直接的硬件搬运通路。L0C->UB数据搬运需经由GM中转（L0C->GM->UB），搬运链路较长且开销较大。
- 3510架构新增L0C Buffer->UB单向数据通路。使能该通路后，数据可从L0C Buffer直接搬运至UB，无需经由GM中转，从而降低搬运开销。

#### 功能说明

本特性涉及[Fixpipe（L0C到UB数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md)接口。具体功能如下：

- 在Ascend 950PR/Ascend 950DT上，L0C Buffer的搬出（L0C->UB、L0C->L1 Buffer、L0C->GM）支持Fixpipe和DataCopy两种接口实现：

  - [Fixpipe（L0C到UB数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md)：使用携带`FixpipeParamsArch3510`参数的Fixpipe接口实现L0C->UB搬出，是3510架构新增L0C->UB通路的推荐实现方式。
  - [DataCopy（L0C到L1 Buffer数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopy_L0CToL1.md)、[DataCopy（L0C到GM数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopy_L0CToGM.md)：使用`DataCopyCO12DstParams`参数的DataCopy接口实现L0C->L1 Buffer/L0C->GM搬出；L0C->UB通路在3510架构上不支持DataCopy方式，请使用Fixpipe实现。

- 使用携带`FixpipeParamsArch3510`参数的Fixpipe接口，将矩阵从L0C Buffer搬运至UB，搬运过程中支持随路格式转换（[NZ2NZ](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/NZ2NZ.md)、[NZ2ND](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/NZ2ND.md)、[NZ2DN](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/NZ2DN.md)）、随路量化、随路ReLU、随路通道合并等操作。
- 函数原型：

  ```cpp
  template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
  __aicore__ inline void Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src,
                                 const FixpipeParamsArch3510<config.format>& intriParams)
  ```

- 使用要点：

  - 使用`FixpipeParamsArch3510`参数时，需要显式构造设置`isToUB = true`的`FixpipeConfig`配置，例如`constexpr AscendC::FixpipeConfig CFG_ROW_MAJOR_UB = {AscendC::CO2Layout::ROW_MAJOR, true};`，不能直接使用`CFG_ROW_MAJOR`等配置。
  - 支持Ascend 950PR/Ascend 950DT新增的`dualDstCtrl`双目标模式（1个Cube+2个Vector场景），M×N矩阵可被拆分为两半并同时写入两个Vector核各自的UB（支持按M维度或N维度拆分），详情请参考[L0C Buffer到UB双目标模式](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/L0C_to_UB_dual_target_mode.md)。

- 随路功能扩展：

  - 搬运过程中支持随路量化、随路ReLU（Normal ReLU/ClipReLU）、随路通道拆分/合并（F32 Channel Split/Int8 Channel Merge）等操作，相关配置接口与参数说明请参考[随路量化](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/accompanying_quantization.md)、[随路ReLU](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/accompanying_relu.md)、[随路量化与随路ReLU场景组合](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/accompanying_quantization_and_relu_scenario_combination.md)。
  - Ascend 950PR/Ascend 950DT新增支持fp8/hifloat8/bf16等随路量化模式与cast能力，例如float量化成fp8_e4m3fn_t（QF322FP8_PRE）、float量化成hifloat8_t（QF322HIF8_PRE）、int32_t量化成bfloat16_t（QS322BF16_PRE）、float量化成half/bfloat16_t/float等，覆盖L0C结果的低比特搬出场景。

- 数据约束：

  - 源操作数位于L0C Buffer（NZ分形格式，64字节对齐），目的操作数位于UB（32字节对齐）。
  - `nSize`不开启channelSplit时须为16的倍数；`mSize=0`或`nSize=0`时视为NOP（空操作）。
  - 支持的数据类型请参考API文档的"数据类型"章节。

- 同步约束：

  - Fixpipe（PIPE_FIX）执行前，需要先保证L0C Buffer数据就绪，即Mmad计算（PIPE_M）与Fixpipe之间需要完成同步（如M_MTE3事件或PipeBarrier）。
  - L0C->UB搬运完成后，若Vector核需要读取UB数据，需要先完成与Cube核的核间同步（如CrossCoreSetFlag/CrossCoreWaitFlag）。

#### 典型样例说明

样例位置：[fixpipe_l0c2ub样例](../../../../../examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/fixpipe_l0c2ub)。样例功能：基于Fixpipe实现L0C->UB数据搬运，完整数据流为GM->L1->L0A/L0B->Mmad->L0C->UB。

### 新增ND-DMA多维数据搬运<a name="section_nddma"></a>

#### 特性应用场景说明

- 在Vector算子中，将数据从GM搬运至UB是常见的数据搬运场景。当数据搬运涉及多维矩阵、不规则Stride，以及Padding、Transpose、Broadcast或Slice等复杂数据变换时，使用基础DataCopy接口通常需要拆分为多条搬运指令，并结合Vector计算实现，编码复杂度较高且性能受限。
- 3510架构新增ND-DMA硬件指令。与基础数据搬运接口相比，该指令支持灵活配置搬入数据的维度信息和Stride，可通过一条指令完成上述复杂场景的数据搬运。

#### 功能说明

本特性涉及[DataCopy（GMToUB多维数据搬运NDDMA）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMToUB_NDDMA.md)接口。具体功能如下：

- 支持维度`dim∈[1, 5]`的多维数据搬运，支持逐维配置Stride，支持左右Padding（常数填充或最近值填充）、Transpose、Broadcast、Slice等场景。
- 函数原型：

  ```cpp
  template <typename T, uint8_t dim, const NdDmaConfig& config = kDefaultNdDmaConfig>
  __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
                                  const NdDmaParams<T, dim>& params)
  ```

- 使用要点：

  - 该数据通路仅支持GM->UB（GM->VECIN）方向。
  - 单条指令访问的地址范围宽度不得超过40位（1TB）。每层循环的`dstStride`按升序排列时，不同循环的地址空间不得交织或重叠。
  - NDDMA Cache容量为32KB。多个核读取同一GM地址时，搬运前必须调用[NdDmaDci](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/NdDmaDci.md)接口刷新NDDMA Cache，否则可能读取到错误数据。
  - 数据类型为b64（int64_t/uint64_t/double/complex64）时，不能使能最近值填充，constantValue必须为0。

#### 典型样例说明

样例位置：[data_copy_gm2ub_nddma样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2ub_nddma)。样例功能：基于ND-DMA指令实现GM->UB多维数据搬运，模拟多维Shape下移动窗口（Slice）的数据搬运，包含输入Stride和不连续源数据搬运的配置方式。

### DataCopy/DataCopyPad新增loop模式<a name="section_loop_mode"></a>

#### 特性应用场景说明

- 在Vector算子中，当需要将GM中的多段数据交替搬运至UB的不同地址，或沿相反方向搬运时，通常需要循环执行多条DataCopy指令，导致指令数量和流水开销增加。
- 3510架构新增loop模式数据搬运能力。单条DataCopy或DataCopyPad指令即可完成循环搬运，从而减少指令下发开销并提升搬运效率。

#### 功能说明

本特性涉及[SetLoopModePara（loop模式参数设置）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/SetLoopModePara.md)接口。具体功能如下：

- 通过`SetLoopModePara`接口使能loop模式并配置相关参数，支持L1 Buffer与GM之间以及GM与UB之间的loop模式搬运。与[DataCopyPad](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)配合使用时，还可实现五维数据搬运。
- 函数原型：

  ```cpp
  __aicore__ inline void SetLoopModePara(const LoopModeParams& loopParams, const DataCopyMVType type)
  ```

  - `LoopModeParams`：loop模式参数，包含loop1Size、loop2Size、loop1SrcStride、loop1DstStride、loop2SrcStride、loop2DstStride。
  - `DataCopyMVType`：搬运数据通路类型，支持`UB_TO_OUT`（UB->GM方向）和`OUT_TO_UB`（GM->UB方向）。

- 使用要点：

  - 源地址和目的地址均要求32字节对齐。
  - 完成一次DataCopy或DataCopyPad搬运后，必须通过[ResetLoopModePara](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/ResetLoopModePara.md)接口复位loop模式参数，否则可能影响后续数据搬运。
  - 目的数据不能重叠，不同迭代间的目的数据可交织。

### LoadData新增MX矩阵搬运<a name="section_mx"></a>

#### 特性应用场景说明

- MicroScaling（MX）是一种低比特量化数据格式，例如fp4x2_e1m2_t、fp4x2_e2m1_t、fp8_e4m3fn_t和fp8_e5m2_t。矩阵数据与各数据块（Block）对应的量化系数（Scale）矩阵共同存储，因此矩阵计算时需同步搬运矩阵数据和量化系数矩阵。
- 3510架构新增适用于MicroScaling场景的数据搬运能力，可同步搬运矩阵数据与量化系数矩阵，为MX低比特格式的矩阵计算提供支持。

#### 功能说明

本特性涉及[LoadData（L1到L0搬运MX）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_MX.md)接口。具体功能如下：

- LoadData新增的MX搬运能力用于搬运矩阵计算所需的左、右矩阵数据及其量化系数矩阵。矩阵数据以512B分形搬运至L0A/L0B Buffer，量化系数矩阵以32B分形搬运至L0A_MX/L0B_MX Buffer。
- 函数原型：

  ```cpp
  template <typename T, typename U>
  __aicore__ inline void LoadData(const LocalTensor<U>& dst, const LocalTensor<T>& src,
                                  const LocalTensor<fp8_e8m0_t>& srcMx,
                                  const LoadData2DParamsV2& loadDataParams,
                                  const LoadData2DMxParams& loadMxDataParams)
  ```

- 使用要点：

  - T/U支持fp4x2_e2m1_t、fp4x2_e1m2_t、fp8_e4m3fn_t、fp8_e5m2_t数据类型；量化系数矩阵的数据类型固定为fp8_e8m0_t，分形为16×2。
  - L0A_MX/L0B_MX Buffer大小为4KB，其地址由接口根据L0A/L0B Buffer地址自动推导（L0A/L0B地址除以16）。
  - 支持的数据通路为L1 Buffer->L0A Buffer（左矩阵及系数）、L1 Buffer->L0B Buffer（右矩阵及系数）。

#### 典型样例说明

样例位置：[load_data_2dmx_l12l0样例](../../../../../examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/load_data_2dmx_l12l0)。样例功能：在Mmad矩阵乘场景下，基于新增LoadData MX搬运能力实现L1->L0A/L0B及量化系数的数据搬运，并进行MX格式的矩阵乘加计算。Mx矩阵计算的完整流程（GM->L1->L0A/L0AMX/L0B/L0BMX->MmadMx->L0C->GM）请参考[Mx矩阵计算流程](../../../api/SIMD-API/basic_api/cube_compute_ISASI/overview/cube_compute_flow.md#ZH-CN_TOPIC_0000002568950892)。

#### 实践指导

- Matmul高阶API的MX（MxMatmul）场景实践请参考[MxMatmul场景说明](../../operator_practice/simd_operator_impl/matrix_advanced_api/feature_scenarios/mx_matmul_scenario.md)（该章节列出的量化场景仅在Ascend 950PR/Ascend 950DT上支持）：A/B矩阵及量化系数矩阵scaleA/scaleB可设置TPosition::GM、VECOUT或TSCM（置TSCM时Format仅支持CubeFormat::NZ），A/B矩阵数据类型支持fp4x2_e1m2_t、fp4x2_e2m1_t、fp8_e4m3fn_t、fp8_e5m2_t。

### LoadDataWithTranspose新增分形转置能力<a name="section_transpose"></a>

#### 特性应用场景说明

- 矩阵乘加计算（A*B=C）中，右矩阵B（L1 Buffer中为NZ分形）按列读取时，需要先将分形数据转置（Transpose）后再搬入L0B Buffer；部分算子还存在分形拼接后整体转置的需求。
- 3510架构新增DN分形，并扩展LoadDataWithTranspose接口能力，支持在搬运过程中完成分形转置。同时，L1 Buffer->L0A Buffer通路不再支持transpose，对应场景需改用`LoadData`接口并配置`ifTranspose`实现。

#### 功能说明

本特性涉及[LoadDataWithTranspose（L1到L0转置数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadDataWithTranspose.md)接口。具体功能如下：

- 3510架构新增以下能力和变更：新增DN分形；L1 Buffer->L0A Buffer通路不再支持transpose，对应场景需改用携带`LoadData2DParamsV2`参数的`LoadData`接口，并设置`ifTranspose=true`；新增`LoadData2dTransposeParamsV2`参数类型，以支持更精细的搬运粒度，其中`srcStride`的单位为单个512B分形，新增的`srcFracGap`表示单次迭代内源分形的间隔。上述能力和变更仅适用于Ascend 950PR/Ascend 950DT。
- 函数原型：

  ```cpp
  template <typename T>
  __aicore__ inline void LoadDataWithTranspose(const LocalTensor<T>& dst, const LocalTensor<T>& src,
                                               const LoadData2dTransposeParamsV2& loadDataParams)
  ```

- 数据约束：

  - 目的地址（L0A/L0B Buffer）512字节对齐，源地址（L1 Buffer）32字节对齐。
  - repeatTimes取值范围为[1, 255]；repeatTimes=0时视为NOP（空操作）。
  - 支持的数据类型请参考API文档的"数据类型"章节（3510架构不支持int4b_t）。

### Fixpipe新增NZ2DN/NZ2ND随路转换<a name="section_nz2dn"></a>

#### 特性应用场景说明

- L0C Buffer中Mmad计算结果为NZ分形格式，部分算子输出需要DN（列主序）或ND（行主序）格式（例如Mix算子将L0C结果以DN格式输出至UB）。此前格式转换需要通过GM中转或Vector核计算完成，链路长、开销大。
- 3510架构的Fixpipe接口新增NZ2DN和NZ2ND随路转换能力，可在L0C->GM/L1/UB搬出过程中完成格式转换，避免额外的数据搬运。

#### 功能说明

本特性涉及[Fixpipe（L0C到GM/L1/UB数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)接口及随路转换特性。具体功能如下：

- 使用携带`FixpipeParamsArch3510`参数的Fixpipe接口，通过`FixpipeConfig`配置随路转换模式：

  - 配置`CO2Layout::ROW_MAJOR`（即`CFG_ROW_MAJOR`）时，实现NZ2ND随路转换，通过`Nz2NdParams`配置转换参数。
  - 配置`CO2Layout::COLUMN_MAJOR`（即`CFG_COLUMN_MAJOR`，3510架构新增）时，实现NZ2DN随路转换，通过`Nz2DnParams`配置转换参数（dnNum、srcNzMatrixStride、dstDnMatrixStride、srcNzC0Stride等）。
  - 转换原理：每个源NZ矩阵（每16列一组）对应一个目的DN矩阵（每16行一组），列元素与行元素一一对应转换。
  - 卷积场景应用：针对Ascend 950PR/Ascend 950DT，支持Fixpipe指令通过NZ2DN能力将L0C Buffer上的卷积计算结果转为NCHW格式输出（配合卷积的输出格式转换流程），详细说明请参考[卷积格式转换](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/convolution_format_conversion.md)。

- 特性约束：

  - 开启Channel Split/Channel Merge（NZ2NZ）功能时不可用；L0C->UB开启双目标模式时不可用。
  - 目的操作数地址之间不能重叠；NZ2DN转换时，`mSize * sizeof(T)`须为32的倍数；srcNzC0Stride不为1时，不能同时开启unitFlag。
  - 详细配置及约束请参考[NZ2DN](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/NZ2DN.md)、[NZ2ND](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_store_key_features/NZ2ND.md)特性说明。

### 矩阵计算基础API扩展<a name="section_cube_api_ext"></a>

#### 特性应用场景说明

除前述数据通路与随路转换能力外，3510架构还新增多项矩阵计算（Cube计算）基础API及其参数类型，用于扩展矩阵数据搬入、矩阵乘加计算和辅助配置能力。上述接口均仅支持Ascend 950PR/Ascend 950DT，具体支持情况以各API文档的“产品支持情况”章节为准。

#### 功能说明

**表 4**  矩阵计算基础API扩展说明<a name="table_cube_api_ext"></a>

| API | 功能说明 |
|-----|---------|
| [LoadData（2D矩阵搬运V2）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_V2.md) | 以512B分形为单位完成L1 Buffer->L0A Buffer/L0B Buffer的2D格式数据搬运，参数类型为`LoadData2DParamsV2` |
| [LoadData（GM到L1搬运2DV2）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_GMToL1_2DV2.md) | 以512B分形为单位将2D格式数据从GM搬运至L1 Buffer，参数类型为`LoadData2DParamsV2` |
| [LoadData（2D矩阵搬运BitMode）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_BitMode.md) | 以512B分形为单位完成L1 Buffer->L0A Buffer/L0B Buffer搬运，参数以联合体结构`Load2DBitModeParam`传入 |
| [LoadData（3D矩阵搬运BitMode）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_3D_BitMode.md) | 卷积（NC1HWC0格式）场景的L1 Buffer->L0A Buffer/L0B Buffer搬运，参数以`Load3DBitModeParam`传入 |
| [LoadDataWithStride](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadDataWithStride.md) | 卷积场景的L1 Buffer->L0A Buffer/L0B Buffer搬运，支持配置输出矩阵K轴方向偏移量dstStride（需配合[SetLoadDataRepeatWithStride](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_load_aux_config/SetLoadDataRepeatWithStride.md)使用） |
| [DataCopy（GMToL1随路转换-DN2NZ搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_GMToL1_DN2NZ.md) | 将矩阵从GM搬运至L1 Buffer，搬运过程中完成DN->NZ格式转换 |
| [DataCopyPad（GMToL1非对齐数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopyPad_GMToL1.md) | 将矩阵从GM搬运至L1 Buffer，支持非对齐数据搬运及按需填充 |
| [MmadMx（带量化系数的矩阵乘）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/MmadMx.md) | MicroScaling矩阵乘，左/右矩阵均有对应的量化系数矩阵，计算公式为C=(scaleA⊗A)*(scaleB⊗B)+Bias |
| [MmadBitMode（BitMode矩阵乘）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/MmadBitMode.md) | BitMode场景（低位宽数据类型）的矩阵乘加计算，与LoadData 2D/3D BitMode配合使用 |
| [SetFmatrixBitMode（BitMode参数设置）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_load_aux_config/SetFmatrixBitMode.md) | BitMode场景下配置Fmatrix值，与LoadData 2D/3D BitMode配合使用 |
| [GEMV（矩阵向量乘）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute_key_features/GEMV.md) | Mmad接口在M=1时自动启用GEMV功能（1×K行向量与K×N矩阵乘法）；950可通过`disableGemv`参数关闭该功能 |
| [K方向对齐约束](../../../api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute_key_features/k_direction_alignment_constraint.md) | float类型Mmad的K方向对齐特性；950上L0A Buffer为NZ排布，A矩阵转置非对齐float场景下K方向8对齐，无需开启此特性 |

- 使用要点：

  - BitMode场景（如int4b_t等低位宽数据类型）下，请使用LoadData 2D/3D BitMode接口进行数据搬运，并使用MmadBitMode计算接口，配套[SetFmatrixBitMode](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_load_aux_config/SetFmatrixBitMode.md)、[SetLoadDataRepeatWithStride](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_load_aux_config/SetLoadDataRepeatWithStride.md)等辅助配置接口，具体用法请参考各API文档。
  - MmadMx场景下，量化系数矩阵scaleA/scaleB可通过[LoadData MX搬运](#section_mx)或Tensor API的[Copy_L1ToL0ScaleA](../../../api/SIMD-API/basic_api/cube_compute_TensorAPI/cube_compute_load/Copy_L1ToL0ScaleA.md)/[Copy_L1ToL0ScaleB](../../../api/SIMD-API/basic_api/cube_compute_TensorAPI/cube_compute_load/Copy_L1ToL0ScaleB.md)接口搬入L0A_MX/L0B_MX Buffer。

#### 典型样例说明

- [mmad_mx样例](../../../../../examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_mx)：演示MmadMx矩阵乘加计算（A/B矩阵为fp4x2_e1m2_t、fp4x2_e2m1_t、fp8_e4m3fn_t、fp8_e5m2_t，scaleA/scaleB为fp8_e8m0_t），数据流为GM->L1->L0A/L0AMX/L0B/L0BMX->Mmad->L0C->GM。
- 更多矩阵计算相关样例请参考[examples/03_matrix_compute目录](../../../../../examples/01_simd_cpp_api/03_basic_api/03_matrix_compute)。

### 矢量计算基础API扩展<a name="section_vec_api_ext"></a>

#### 特性应用场景说明

3510架构的AIV核采用Regbase架构，矢量计算基础API相应新增多项接口和参数变体，包括以LocalTensor单点元素作为标量的`flexible_scalar`变体、按位或按边界处理的ISASI接口、交织与解交织、按张量指定移位量的逻辑计算，以及UB内部搬运等。上述接口均仅支持Ascend 950PR/Ascend 950DT，具体支持情况以各API文档的“产品支持情况”章节为准。

#### 功能说明

**表 5**  矢量计算基础API扩展说明<a name="table_vec_api_ext"></a>

| API | 功能说明 |
|-----|---------|
| [Adds（flexible_scalar标量加法）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Adds_flexible_scalar.md) | 矢量内每个元素与标量做加法，标量输入支持配置LocalTensor单点元素，支持标量在前/在后两种场景 |
| [Muls（flexible_scalar标量乘法）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Muls_flexible_scalar.md) | 矢量内每个元素与标量做乘法，标量输入支持配置LocalTensor单点元素 |
| [Subs（flexible_scalar标量减法）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Subs_flexible_scalar.md) | 矢量内每个元素与标量做减法，标量输入支持配置LocalTensor单点元素 |
| [Divs（flexible_scalar标量除法）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Divs_flexible_scalar.md) | 矢量内每个元素与标量做除法，标量输入支持配置LocalTensor单点元素 |
| [Maxs（flexible_scalar标量最大值）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Maxs_flexible_scalar.md) | 矢量内每个元素与标量取最大值，标量输入支持配置LocalTensor单点元素 |
| [Mins（flexible_scalar标量最小值）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Mins_flexible_scalar.md) | 矢量内每个元素与标量取最小值，标量输入支持配置LocalTensor单点元素 |
| [Compares（flexible_scalar标量比较）](../../../api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Compares_flexible_scalar.md) | 矢量内每个元素与标量比较，标量输入支持配置LocalTensor单点元素，支持标量在前/在后两种场景 |
| [Select（flexible_scalar条件选择）](../../../api/SIMD-API/basic_api/memory_vector_compute/compare_and_select/Select_flexible_scalar.md) | 根据selMask的比特位从两个源操作数中选取元素写入目的操作数 |
| [Neg（ISASI取负）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Neg_ISASI.md) | 对源操作数取相反数 |
| [Prelu（ISASI带参数ReLU）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Prelu_ISASI.md) | 带参数的ReLU计算 |
| [Mull（ISASI乘加）](../../../api/SIMD-API/basic_api/memory_vector_compute/basic_arithmetic/Mull_ISASI.md) | 32位元素相乘得到64位乘积，拆分为低32位与高32位分别写入两个目的操作数 |
| [AbsSub（ISASI绝对值差）](../../../api/SIMD-API/basic_api/memory_vector_compute/composite_compute/AbsSub_ISASI.md) | 计算两个源操作数差的绝对值 |
| [ExpSub（ISASI指数差）](../../../api/SIMD-API/basic_api/memory_vector_compute/composite_compute/ExpSub_ISASI.md) | 计算`exp(src0)-exp(src1)` |
| [MulsCast（ISASI乘后转换）](../../../api/SIMD-API/basic_api/memory_vector_compute/composite_compute/MulsCast_ISASI.md) | 矢量与标量相乘后再进行数据类型转换 |
| [Truncate（ISASI截断）](../../../api/SIMD-API/basic_api/memory_vector_compute/type_conversion/Truncate_ISASI.md) | 对源操作数执行向下取整（截断）操作 |
| [Interleave（交织）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_rearrange_ISASI/Interleave.md) | 将两个源操作数中的元素交织存入结果操作数 |
| [DeInterleave（解交织）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_rearrange_ISASI/DeInterleave.md) | 将源操作数中的元素按交织方式拆分到两个结果操作数 |
| [ShiftLeft（移位量输入左移）](../../../api/SIMD-API/basic_api/memory_vector_compute/logical_compute/ShiftLeft_shift_amount_tensor.md) | 对源操作数中的每个元素按移位量左移，移位量由张量输入指定 |
| [ShiftRight（移位量输入右移）](../../../api/SIMD-API/basic_api/memory_vector_compute/logical_compute/ShiftRight_shift_amount_tensor.md) | 对源操作数中的每个元素按移位量右移，移位量由张量输入指定 |
| [Ands（标量按位与）](../../../api/SIMD-API/basic_api/memory_vector_compute/logical_compute/Ands.md) | 矢量内每个元素和标量做按位与操作，标量输入支持配置LocalTensor单点元素 |
| [Ors（标量按位或）](../../../api/SIMD-API/basic_api/memory_vector_compute/logical_compute/Ors.md) | 矢量内每个元素和标量做按位或操作，标量输入支持配置LocalTensor单点元素 |
| [Copy（UBToUB连续搬运）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/Copy_UBToUB_continuous.md) | UB内部的连续数据搬运，数据搬运时格式和内容保持不变 |
| [DataCopyPad Compact模式](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md) | 非对齐数据搬运的紧凑模式（PaddingMode::Compact，仅950支持），将多块非32字节对齐数据合并为连续数据块、仅在末尾补齐至32字节对齐，减少无效数据搬运量、节省带宽（实践请参考[减少无效数据搬运](../../operator_practice/simd_operator_optimization/memory_access/reduce_invalid_data_transfer.md)） |

- 使用要点：

  - `flexible_scalar`系列接口的标量参数支持LocalTensor单点元素，可在矢量计算中直接使用UB中的单点标量值，避免在Host与Device之间搬运标量。
  - 3510架构在Memory矢量计算的高维切分接口中引入**stride参数**，表示前一数据块起始地址与后一数据块起始地址之间的间隔，用于替代2201架构的gap参数。gap参数表示前一数据块结束地址与后一数据块起始地址之间的间隔。从其他架构迁移时，需注意两者的语义差异。详情请参见[Memory矢量计算编程](../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/memory_vector_computation.md)。
  - 上述接口的详细原型、数据类型支持范围及约束请参考对应API文档。

### 原子操作接口<a name="section_atomic_api"></a>

#### 特性应用场景说明

在多线程或多核并发访问GM中同一数据的场景下，例如直方图统计和全局计数器累加，需要对GM地址中的数据执行原子读改写操作，以避免并发写入引发数据竞争。3510架构新增GM地址原子操作基础接口。接口定义在`basic_api/kernel_operator_atomic_intf.h`中，均仅支持Ascend 950PR/Ascend 950DT。

#### 功能说明

**表 6**  原子操作接口说明<a name="table_atomic_api"></a>

| API | 功能说明 |
|-----|---------|
| [AtomicAdd（原子加）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md) | 将GM地址上的旧值与输入标量求和并写回，返回修改前的值 |
| [AtomicMax（原子最大值）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicMax.md) | 将GM地址上的旧值与输入标量取最大值并写回，返回修改前的值 |
| [AtomicMin（原子最小值）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicMin.md) | 将GM地址上的旧值与输入标量取最小值并写回，返回修改前的值 |
| [AtomicExch（原子交换）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicExch.md) | 将GM地址上的旧值与输入标量交换，返回修改前的值 |
| [AtomicCas（原子比较交换）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicCas.md) | 将GM地址上的旧值与期望值比较，相等则写入新值，返回修改前的值 |

#### 典型样例说明

样例位置：[scalar_atomic_operations样例](../../../../../examples/01_simd_cpp_api/03_basic_api/06_atomic/scalar_atomic_operations)。样例功能：基于AtomicAdd和AtomicCas接口，演示在GM地址上进行标量原子加和原子比较交换的实现流程，支持多核并行更新单个内存地址，仅支持Ascend 950PR/Ascend 950DT。

### 系统变量与工具接口<a name="section_sys_api"></a>

#### 特性应用场景说明

3510架构新增系统变量访问、特殊寄存器访问、数值极限、标量计算和性能打点等工具类基础接口，用于算子编程中的资源查询与辅助计算。上述接口均仅支持Ascend 950PR/Ascend 950DT。

#### 功能说明

**表 7**  系统变量与工具接口说明<a name="table_sys_api"></a>

| API | 功能说明 |
|-----|---------|
| [GetUBSizeInBytes（UB大小查询）](../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetUBSizeInBytes.md) | 获取UB空间大小（字节），用于计算循环次数等参数 |
| [GetRuntimeUBSize（运行时UB大小查询）](../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetRuntimeUBSize.md) | 获取运行时UB空间大小（字节），支持非默认UB大小配置场景 |
| [GetVecLen（寄存器位宽查询）](../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetVecLen.md) | 获取RegTensor位宽VL（Vector Length）的大小，用于Reg矢量计算的循环控制 |
| [GetSpr（特殊寄存器读取）](../../../api/SIMD-API/basic_api/special_register_access/GetSpr.md) | 获取指定特殊寄存器的值 |
| [ClearSpr（特殊寄存器清零）](../../../api/SIMD-API/basic_api/special_register_access/ClearSpr.md) | 对指定的特殊寄存器进行清零 |
| [ResetCtrlSpr（控制寄存器重置）](../../../api/SIMD-API/basic_api/special_register_access/ResetCtrlSpr_ISASI.md) | 对CTRL寄存器的特定比特位做重置 |
| [NumericLimits::Max（最大值）](../../../api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/Max.md) | 获取数值类型的最大值 |
| [NumericLimits::Min（最小值）](../../../api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/Min.md) | 获取数值类型的最小值 |
| [NumericLimits::Lowest（最低值）](../../../api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/Lowest.md) | 获取数值类型的最低值（最小的有限值） |
| [NumericLimits::Infinity（正无穷）](../../../api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/Infinity.md) | 获取数值类型的正无穷大 |
| [NumericLimits::NegativeInfinity（负无穷）](../../../api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/NegativeInfinity.md) | 获取数值类型的负无穷大 |
| [NumericLimits::QuietNaN（静默NaN）](../../../api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/QuietNaN.md) | 获取数值类型的静默NaN |
| [NumericLimits::SignalingNaN（信号NaN）](../../../api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/SignalingNaN.md) | 获取数值类型的信号NaN |
| [NumericLimits::DeNormMin（最小非规格化数）](../../../api/SIMD-API/basic_api/tool_interface/NumericLimits_utils/DeNormMin.md) | 获取数值类型的最小非规格化数 |
| [CeilDivision（向上取整除法）](../../../api/Utils-API/cpp_stdlib/math_functions/CeilDivision.md) | 计算两个整数相除后向上取整的结果 |
| [Nop（延迟执行）](../../../api/SIMD-API/basic_api/scalar_compute/Nop.md) | 按输入延迟个数延迟若干个cycle |
| [MarkStamp（性能打点）](../../../api/SIMD-API/basic_api/debug_interface/performance_stats/MarkStamp.md) | 在算子执行过程中标记特定位置，便于通过流水图分析代码执行路径与性能热点 |
| [asc_mark_stamp（性能打点C接口）](../../../api/Utils-API/tuning_interface/asc_mark_stamp.md) | 与MarkStamp功能一致的C语言接口，同时适用于C语言和C++语言编程 |
| [__trap（中断执行）](../../../api/Utils-API/tuning_interface/__trap.md) | 在SIMT代码中调用此接口会中断算子的运行，适用于核函数（Kernel）侧异常场景的调试 |
| [ceil_div（向上取整除法）](../../../api/Utils-API/cpp_stdlib/math_functions/ceil_div.md) | 计算两个整数相除后向上取整的结果（头文件路径为`utils/std/cmath.h`） |
| [sqrt（平方根）](../../../api/Utils-API/cpp_stdlib/math_functions/sqrt.md) | 计算输入数据的平方根（头文件路径为`utils/std/cmath.h`），仅Ascend 950PR/Ascend 950DT支持 |
| [abs（绝对值）](../../../api/Utils-API/cpp_stdlib/math_functions/abs.md) | 获取输入数据的绝对值（头文件路径为`utils/std/cmath.h`），仅Ascend 950PR/Ascend 950DT支持 |
| [asc_dump（数据打印，950独占重载）](../../../api/Utils-API/tuning_interface/asc_dump.md) | BiasTable Buffer/Fixpipe Buffer数据打印、寄存器数据打印（asc_dump_reg/asc_dump，simd_vf中使用），仅950支持的重载版本 |
| [clock（时钟周期，SIMT版）](../../../api/Utils-API/tuning_interface/clock.md) | SIMT中使用的clock接口，返回程序开始到调用时的时钟周期数，仅950支持 |
| [printf（格式化打印，VF/SIMT版）](../../../api/Utils-API/tuning_interface/printf.md) | simd_vf中的printf接口（寄存器编程）和SIMT中的printf接口，仅950支持 |

### 新增DCI数据缓存失效能力<a name="section_dci"></a>

#### 特性应用场景说明

- 算子执行结束时，需将DCache置为失效，防止后续算子读取其中的无效数据。默认情况下，编译框架会在算子尾部插入DCCI（DataCacheCleanAndInvalid）指令，在使DCache失效的同时将脏数据写回GM，因此会产生额外的Clean开销。
- 3510架构（Ascend 950PR/Ascend 950DT）新增DCI（DataCacheInvalid）数据缓存失效能力。DCI指令可直接使整个数据缓存失效，而不回写脏缓存行。在算子尾部使用DCI指令替代DCCI指令，可跳过将DCache脏数据同步至GM的Clean过程，从而降低算子尾部开销。

#### 功能说明

- 使能方式（均仅支持Ascend 950PR/Ascend 950DT）：

  - 编译选项方式：编译时配置[`--cce-no-dcache-flush`](../../operator_practice/simd_operator_optimization/overhead_optimization/dci_compile_option.md)，编译框架将在算子尾部插入DCI指令，替代默认的DCCI指令。推荐使用该方式使能DCI能力。
  - 接口方式：使用[asc_dci](../../../api/SIMD-API/c_api/cache_ctrl/asc_dci.md)接口执行数据缓存失效，调用前需先通过[asc_sync_data_barrier](../../../api/SIMD-API/c_api/sync/asc_sync_data_barrier.md)插入DSB_ALL指令，等待所有内存访问指令执行结束。

- 适用场景：

  - 算子通过`__gm__`指针改写GM内存或调用`GlobalTensor.SetValue`时，已正确调用[DataCacheCleanAndInvalid](../../../api/SIMD-API/basic_api/cache_control/DataCacheCleanAndInvalid.md)接口，将数据从DCache回写至GM并保证Cache一致性，因而不依赖编译框架自动插入DCCI指令。
  - 算子不包含通过`__gm__`指针改写GM内存或调用`GlobalTensor.SetValue`的代码。

- 详细说明请参考[设置DCI编译选项来减少算子尾开销](../../operator_practice/simd_operator_optimization/overhead_optimization/dci_compile_option.md)。

### 新增TSCM资源管理接口<a name="section_tscm"></a>

#### 特性应用场景说明

- TSCM（Temp Swap Cache Memory）是3510架构新增的存储位置（TPosition::TSCM），用于在Matmul等矩阵计算中将临时数据交换至额外空间。Vector核与Cube核通过队列（Queue）实现任务间通信与同步。当数据通路的目的地址为TSCM Position时，需使用TSCM结构管理队列操作和相关资源。
- 3510架构新增TSCM队列的内存分配、内存释放、入队和出队管理接口。接口定义在`basic_api/kernel_tpipe.h`中，均仅支持Ascend 950PR/Ascend 950DT。TSCM、TQueBind和TQue属于同类结构。

#### 功能说明

**表 8**  TSCM资源管理接口说明<a name="table_tscm_api"></a>

| API | 功能说明 |
|-----|---------|
| [AllocTensor（TSCM分配）](../../../api/SIMD-API/basic_api/resource_management/TSCM/AllocTensor.md) | 从TSCM中分配tensor。tensor占用的空间大小为调用InitBuffer时设置的单块内存长度。分配后的tensor内容不保证全为0，可能包含随机值 |
| [FreeTensor（TSCM释放）](../../../api/SIMD-API/basic_api/resource_management/TSCM/FreeTensor.md) | 释放从TSCM中分配的tensor |
| [EnQue（TSCM入队）](../../../api/SIMD-API/basic_api/resource_management/TSCM/EnQue.md) | 将tensor加入队列，用于流水线调度 |
| [DeQue（TSCM出队）](../../../api/SIMD-API/basic_api/resource_management/TSCM/DeQue.md) | 将tensor从队列中取出，用于后续处理 |

#### 使用说明

- TSCM的使用方式与TQue/TQueBind一致：首先通过`InitBuffer`初始化队列缓冲区，然后通过`AllocTensor`和`FreeTensor`分配、释放tensor，并通过`EnQue`和`DeQue`完成入队、出队操作。建议将队列深度设置为1，编译器已针对该场景进行专项优化。
- 详细说明请参考[TSCM简介](../../../api/SIMD-API/basic_api/resource_management/TSCM/TSCM_intro.md)及[TPosition说明](../../../api/SIMD-API/basic_api/aux_data_structures/TPosition.md)。

### 新增SSBuffer核内存储单元<a name="section_ssbuf"></a>

#### 特性应用场景说明

- 在Vector与Cube融合（Mix）算子中，AIC核与AIV核之间需要传递控制信息和标量数据。在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等较早架构上，AIC与AIV通过GM（全局内存）通信，通信延迟较高且占用GM带宽。
- 3510架构新增SSBuffer核内存储单元，支持AI Core内的AIC核与AIV核通过Scalar直接访问SSBuffer进行核间通信。该方式无需访问GM，可降低通信延迟并减少GM带宽占用。
- SSBuffer适用于同一任务内AIC核与AIV核之间的小数据量通信，例如传递控制信息、地址或少量标量数据。大块数据应通过相应的数据通路搬运，不应将SSBuffer用作通用数据缓存。

#### 功能说明

- 存储规格：[SSBuffer](../../programming_guide/advanced_programming/hardware_implementation/architecture_spec/npu_arch_3510.md)是3510架构AI Core内新增的核间通信存储单元，容量为3KB。SSBuffer使用独立于L1 Buffer和UB的地址空间，对应的地址空间限定符为`__ssbuf__`。详情请参见[SIMD BuiltIn关键字](../../programming_guide/language_extension/simd_builtin_keywords.md)。
- 使用范围：

  - SSBuffer仅支持Ascend 950PR/Ascend 950DT；在分离模式的AIC:AIV为1:2的混合算子中，AIC核与对应AIV核可通过SSBuffer交换数据，核函数类型和AIC/AIV比例必须满足核间同步接口的约束。

- 访问方式：

  - 非MIX模式下，AIC、AIV0、AIV1各自独立占用1KB空间；MIX模式下（AIC:AIV=1:2）共享整个3KB空间。
  - 通过[GetSsbufBaseAddr](../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSsbufBaseAddr.md)接口获取SSBuffer基地址（返回`__ssbuf__`指针），基于基地址偏移进行读写访问。
  - SSBuffer仅支持通过32字节对齐的读写指令访问，通信数据块的起始地址和访问粒度均须满足32字节对齐要求。不得对SSBuffer执行小于32字节的成员读写；访问地址超出SSBuffer范围时会触发异常。

- 使用约束：

  - SSBuffer的初始内容不确定，读取结果不保证全为0。每次通信均应由生产者完整写入数据，消费者不得依赖SSBuffer中的历史内容。
  - 数据布局必须在通信双方（AIC与AIV）保持一致，且数据不得越过SSBuffer末端地址。
  - AIC和AIV启动不同任务时不能访问SSBuffer；当前任务未开启SSBuffer能力时不能访问。

#### 通信流程

SSBuffer仅提供共享存储能力，不保证数据已就绪。生产者和消费者必须配合核间同步接口建立正确的数据访问顺序。推荐流程如下：

1. 规划固定的32字节对齐消息布局，并为每个通信方向分配互不重叠的区域。
2. 生产者将完整消息写入SSBuffer（在所有前置流水完成之后）。
3. 生产者在所有前置流水完成后调用`CrossCoreSetFlag`发布消息。
4. 消费者调用配对的`CrossCoreWaitFlag`，等待对应标记完成后读取SSBuffer中的消息。
5. 下一轮复用同一消息区域前，使用新的同步轮次确保上一轮读写均已完成。

AIC与全部AIV之间同步使用模式2；在AIC:AIV为1:2场景需要与单个AIV通信时，可使用模式4。`CrossCoreSetFlag`与`CrossCoreWaitFlag`必须成对使用，且flagId不得与当前算子中Matmul高阶API或SyncAll等接口占用的标记冲突，具体说明请参考[新增核间同步控制模式](#section_crosscore)。

#### 使能说明

SSBuffer能力与[UB->L1搬运数据通路](#section_ub2l1)共用编译选项[`ENABLE_CV_COMM_VIA_SSBUF`](#section_enable_option)，配置`-DENABLE_CV_COMM_VIA_SSBUF=true`后使能。使用Matmul高阶API的SetTensorScaleA等接口（内部实现使用SSBuffer）时，也需要使能该编译选项。

#### 使用示例

以下代码示例演示SSBuffer核间通信的完整用法（AIC:AIV为1:2分离模式，AIV0向AIC单向传递32字节消息）。使用SSBuffer前需使能[`ENABLE_CV_COMM_VIA_SSBUF`](#section_enable_option)编译选项，且`CrossCoreSetFlag`/`CrossCoreWaitFlag`的flagId需避开当前算子中Matmul高阶API或SyncAll等接口占用的flagId。

- 获取基地址并定义消息：

  ```cpp
  // 获取SSBuffer基地址，返回__ssbuf__指针；NPU域中该特殊基址可能显示为0或nil，不应作为普通空指针解引用
  __ssbuf__ void* ssbuf = AscendC::GetSsbufBaseAddr();

  // 定义32字节对齐的通信消息
  struct alignas(32) SsbufferMessage {
      uint64_t value[4];
  };

  // 基于基地址偏移定位消息区域，起始偏移须32字节对齐且不越过SSBuffer末端
  __ssbuf__ uint8_t* ssbuffer = reinterpret_cast<__ssbuf__ uint8_t*>(ssbuf);
  __ssbuf__ SsbufferMessage* message =
      reinterpret_cast<__ssbuf__ SsbufferMessage*>(ssbuffer + 256);
  ```

- AIV核（生产者）写入消息并发布就绪标记：

  ```cpp
  if ASCEND_IS_AIV {
      // 1:2场景中仅AIV0生产本示例的消息，AIV1仍须参与模式2同步
      if (AscendC::GetSubBlockIdx() == 0) {
          // 生产者将完整32字节消息写入SSBuffer
          message->value[0] = 0x1122334455667788ULL;
          message->value[1] = 0x8877665544332211ULL;
          message->value[2] = 0x0123456789abcdefULL;
          message->value[3] = 0xfedcba9876543210ULL;
      }
      // SSBuffer访问位于Scalar流水：先建立Scalar到Vector的核内顺序，
      // 再以模式2发布AIV到AIC的就绪标记
      AscendC::SetFlag<AscendC::HardEvent::S_V>(0);
      AscendC::WaitFlag<AscendC::HardEvent::S_V>(0);
      AscendC::CrossCoreSetFlag<2, PIPE_V>(AIV_TO_AIC_FLAG);
  }
  ```

- AIC核（消费者）等待标记完成后读取消息：

  ```cpp
  if ASCEND_IS_AIC {
      // 模式2等待同一逻辑AI Core上的两个AIV均发布完成
      AscendC::CrossCoreWaitFlag<2, PIPE_M>(AIV_TO_AIC_FLAG);
      // 先建立Cube流水到Scalar读取的顺序，再读取SSBuffer中的消息
      AscendC::SetFlag<AscendC::HardEvent::M_S>(0);
      AscendC::WaitFlag<AscendC::HardEvent::M_S>(0);
      // 消费者按通信双方约定的布局使用message中的消息数据
      output[0] = message->value[0];
      output[1] = message->value[1];
      output[2] = message->value[2];
      output[3] = message->value[3];
  }
  ```

### 新增Mutex核内同步能力<a name="section_mutex"></a>

#### 特性应用场景说明

- 在Vector算子中，异步流水指令之间通常通过SetFlag/WaitFlag事件进行同步，例如GM->UB搬运（PIPE_MTE2）、向量计算（PIPE_V）和UB->GM搬运（PIPE_MTE3）。该方式需要显式配对事件，编码较为复杂，且受事件资源数量限制。
- 3510架构新增Mutex能力，通过锁定和释放指定流水建立流水间的同步依赖。与SetFlag/WaitFlag相比，Mutex与其他流水解耦，可简化反向同步，并提供更多可用的同步信号量。

#### 功能说明

本特性涉及[Mutex（核内同步）](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)接口。具体功能如下：

- Mutex用于核内异步流水指令之间的同步控制，其功能类似于传统CPU中的锁机制：通过[Lock](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Lock.md)锁定指定流水，阻塞后续指定流水指令队列；通过[Unlock](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Unlock.md)释放指定流水，继续执行后续指令。同一MutexID下可配对不同流水（如MTE2与V的互斥同步）。
- 函数原型：

  ```cpp
  class Mutex {
      template <pipe_t pipe>
      static __aicore__ inline void Lock(MutexID id);
      template <pipe_t pipe>
      static __aicore__ inline void Unlock(MutexID id);
  };
  ```

- 使用要点：

  - TPipe-TQue编程范式下，通过[AllocMutexID](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/AllocMutexID_ISASI.md)/[ReleaseMutexID](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/ReleaseMutexID_ISASI.md)接口申请/释放MutexID；静态Tensor编程范式下自行管理MutexID（建议使用0-27，28-31为系统内部规划预留）。
  - 使用流程为：申请MutexID -> 调用`Mutex::Lock<pipe>(id)` -> 下发该流水的指令（片元） -> 调用`Mutex::Unlock<pipe>(id)` -> 释放MutexID。
  - Lock/Unlock对同一MutexID必须严格成对使用且pipe必须相同，否则硬件行为不可预测；相同ID相同流水的两组Lock/Unlock第二次不会阻塞流水。

#### 典型样例说明

样例位置：[mutex样例](../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/mutex)。样例功能：使用Mutex实现MTE2、V流水与MTE3流水之间的同步，在无SetFlag/WaitFlag的情况下正确完成数据搬运与计算。

### 新增核间同步控制模式<a name="section_crosscore"></a>

#### 特性应用场景说明

- 在Vector与Cube融合（Mix）算子中，AIV核（Vector）与AIC核（Cube）之间存在数据依赖，需要通过核间同步接口（CrossCoreSetFlag/CrossCoreWaitFlag）保证数据就绪后再发起后续计算。
- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等较早架构上，当AIC核等待同步时，AIV0与AIV1必须同时上报，即采用模式2实现AIC与所有AIV同步，因此同步粒度较粗。
- 3510架构新增核间同步控制模式4，用于AIC与单个AIV同步。AIV0与AIV1可分别触发AIC等待，从而细化同步粒度、减少冗余等待并提高执行效率。

#### 功能说明

本特性涉及[CrossCoreSetFlag（核间同步设置标志）](../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md)和[CrossCoreWaitFlag（核间同步等待标志）](../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md)接口。具体功能如下：

- 同步模式说明：

  - 模式0：AI Core核间全同步。
  - 模式1：AI Core内AIV之间同步。
  - 模式2：AIC与所有AIV同步。
  - 模式4：AIC与单个AIV同步（3510架构新增，仅Ascend 950PR/Ascend 950DT支持）。AIV0发起的flagId 0-15对应AIC核WaitFlag的flagId 0-15；AIV1发起的flagId 0-15对应AIC核WaitFlag的flagId 16-31；AIC发起的flagId 0-15/16-31分别对应AIV0/AIV1的WaitFlag的flagId 0-15。

- 函数原型：

  ```cpp
  template <uint8_t modeId, pipe_t pipe>
  __aicore__ inline void CrossCoreSetFlag(uint16_t flagId);
  template <uint8_t modeId, pipe_t pipe>
  __aicore__ inline void CrossCoreWaitFlag(uint16_t flagId);
  ```

- 使用要点：

  - CrossCoreSetFlag/CrossCoreWaitFlag必须成对使用，且需在不同的核上（SetFlag在数据就绪侧核上调用，WaitFlag在等待侧核上调用）。
  - 模式4仅支持`__mix__(1, 2)`核型配置（KERNEL_TYPE_MIX_AIC_1_2）的算子场景。
  - 模式0/1/2支持PIPE_V/PIPE_M/PIPE_MTE1/PIPE_MTE2/PIPE_MTE3/PIPE_FIX，模式4额外支持PIPE_S；均不支持PIPE_ALL。一个核连续发出的多次SetFlag的执行顺序不保证。
  - 模式4下flagId占用量与Matmul高阶API相关，需注意避让（Matmul高阶API占用flagId [0, 2N-1]，N为Mix核数）。

#### 典型样例说明

样例位置：[cross_core_set_wait_flag样例](../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/cross_core_set_wait_flag)。样例功能：在Mix算子（AIC:AIV=1:2）场景下，使用模式4核间同步，AIV0与AIV1分别单独触发AIC核等待，完成UB->L1搬运与Mmad计算的流水协作。

### 新增Regbase矢量计算方式<a name="section_regbase"></a>

#### 特性应用场景说明

- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等较早架构上，AIV核采用Membase架构。矢量计算的结果必须写回Unified Buffer（UB）后，才能供后续指令使用。中间结果需要反复读写UB，因而产生额外的搬运开销。
- 3510架构的AIV核采用Regbase架构，支持直接操作Vector寄存器（Register）。中间结果可暂存于寄存器中，从而减少UB读写并提高执行效率。

#### 功能说明

- 两种矢量计算方式的差异如下：

  **表 9**  Membase与Regbase架构差异<a name="table_membase_regbase"></a>

  | 计算方式 | 数据暂存位置 | 特点 | 适用场景 |
  |---------|------------|------|---------|
  | Membase | Local Memory（UB） | 每步计算结果写回UB | NPU架构版本2201 |
  | Regbase | 寄存器（VF Reg） | 中间结果可暂存寄存器，减少UB读写 | NPU架构版本3510 |

- 编程方式说明（详细请参考[Reg矢量计算](../../../api/SIMD-API/basic_api/reg_vector_compute/overview.md)相关文档）：

  - C++ Tensor方式：使用`Reg::RegTensor<T>`定义位宽为VL的寄存器Tensor，使用`Reg::MaskReg`定义掩码寄存器，并通过`asc_vf_call`调用`__simd_vf__`函数。典型处理流程为LoadAlign（UB->寄存器）-> Reg计算（例如Add）-> StoreAlign（寄存器->UB）。循环计算时，使用`UpdateMask`更新掩码，并配合Hardware Loop完成处理。
  - 3510架构的Reg资源数量有限，开发时需合理分配。资源规格包括256B×32的矢量数据寄存器、32B×8的掩码寄存器、4B×8的地址寄存器，以及32B×4的非对齐搬入/搬出寄存器。详情请参见[NPU架构规格](../../programming_guide/advanced_programming/hardware_implementation/architecture_spec/npu_arch_3510.md)。
  - 3510架构Reg矢量计算接口支持**指令双发**能力（1个Cycle可执行两条指令，仅针对Ascend 950PR/Ascend 950DT生效），各接口的单双发情况请参考[Reg矢量计算接口指令单双发汇总](../../../api/appendix/reg_vector_compute_interface_instruction_single_dual_issue_summary.md)；编程时合理利用双发能力（如寄存器搬入LoadAlign、Move等）可进一步提升性能。
  - **VF循环优化**：VF（Vector Function）函数中的循环在满足[Hardware Loop编码规范](../../operator_practice/simd_operator_optimization/vector_compute/vf_optimization/vf_loop_optimization.md)时可被编译器优化为Hardware Loop（硬件级向量循环），提升执行性能；同时3510架构引入[地址寄存器（AddrReg）](../../../api/SIMD-API/basic_api/reg_vector_compute/register_data_types/AddrReg.md)优化地址偏移量计算，满足特定地址寄存器生成模式时可消除Scalar计算消耗，VF循环优化指导请参考[VF循环优化](../../operator_practice/simd_operator_optimization/vector_compute/vf_optimization/vf_loop_optimization.md)（950专属实践）。
  - **配套参考汇总**：3510架构Reg矢量计算配套提供以下950独占的参考汇总文档：[Reg矢量计算接口精度标准汇总](../../../api/appendix/reg_vector_compute_interface_precision_standard_summary.md)（各接口的硬件指令精度标准与软仿精度标准）、[Reg矢量计算接口软仿情况汇总](../../../api/appendix/reg_vector_compute_interface_soft_simulation_summary.md)（各接口软仿支持的数据类型）、[C-API Reg矢量计算接口理论性能汇总](../../../api/appendix/c_api_reg_vector_compute_theoretical_perf_summary.md)（寄存器C API各接口的单指令理论性能）。

Reg矢量计算相关接口均仅支持Ascend 950PR/Ascend 950DT产品，完整接口清单请参考[Reg矢量计算](../../../api/SIMD-API/basic_api/reg_vector_compute/overview.md)文档。

#### 典型样例说明

Reg矢量计算相关样例位于`examples/01_simd_cpp_api/03_basic_api/00_data_movement`目录下，均仅支持Ascend 950PR/Ascend 950DT产品，常用样例说明如下：

- [reg_load_store_align样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_store_align)：基于Reg编程接口实现UB对RegTensor的连续/非连续对齐数据搬运（LoadAlign/StoreAlign接口，支持POST_MODE_UPDATE、DATA_BLOCK_COPY等模式），通过编译参数`SCENARIO_NUM`选择6种搬运场景。
- [reg_load_store_mask样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_store_mask)：实现UB对MaskReg（掩码寄存器）的搬入搬出，以及使用掩码进行掩码搬出。
- [reg_load_store_unalign样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_store_unalign)：实现UB对RegTensor的非对齐数据搬运。
- [reg_load_gather样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/reg_load_gather)：实现将离散数据从UB搬入RegTensor。
- [reg_store_scatter样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/reg_store_scatter)：实现按元素将数据从寄存器分散写回UB。
- [reg_move样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/reg_move)：实现从RegTensor搬运数据到MaskReg（MaskGenWithRegTensor接口）。
- [reg_auxscalar样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/reg_auxscalar)：实现AuxScalar方式从UB读取多个标量数据，结合Adds进行向量与标量加法计算。

Reg计算（Add/Mul/Reduce等）相关样例位于`examples/01_simd_cpp_api/03_basic_api/02_reg_vector_compute`目录下，例如add、mul、reduce、reduce_pair等，请参考[02_reg_vector_compute目录](../../../../../examples/01_simd_cpp_api/03_basic_api/02_reg_vector_compute)。

Reg矢量计算高性能算子样例位于`examples/01_simd_cpp_api/05_best_practices/02_reg_compute`目录，包括[gelu_high_performance样例](../../../../../examples/01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_high_performance)（基于Reg矢量计算的GELU激活函数高性能示例）、[gelu_eltwise_high_performance样例](../../../../../examples/01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_eltwise_high_performance)和[softmax_high_performance样例](../../../../../examples/01_simd_cpp_api/05_best_practices/02_reg_compute/softmax_high_performance)（基于Reg矢量计算的Softmax高性能示例）。

### 新增SIMT编程模型<a name="section_simt"></a>

#### 特性应用场景说明

- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等较早架构上，AI Core仅支持SIMD（单指令多数据）编程模型，对复杂控制流和离散数据访问等不规则场景的支持存在一定限制。
- 3510架构新增SIMT（单指令多线程）编程模型，并采用以SIMD为主、SIMT为辅的编程范式。该模型利用部分向量计算能力提供SIMT功能，为复杂控制流和离散访存等不规则场景提供线程级并行能力，同时支持SIMD与SIMT混合编程。

#### 功能说明

- SIMT编程模型以线程为基本执行单元，单条指令可同时驱动多个独立线程，每个线程处理一个数据元素，并可根据代码分支（例如`if-else`）独立执行。当前仅Ascend 950PR/Ascend 950DT支持该模型。详情请参见[编程模型](../../programming_guide/programming_model/programming_model_overview.md)。
- SIMD与SIMT混合编程：以SIMD核函数（Kernel）作为Device侧入口，在核函数或`__aicore__`函数中通过`asc_vf_call`启动`__simt_vf__`标记的SIMT VF子任务，SIMT VF内可使用`__simt_callee__`子函数、threadIdx/blockDim等线程层级内置变量，详细说明请参考[SIMD与SIMT混合编程](../../programming_guide/language_extension/simd_simt_hybrid_builtin_keywords.md)。

- SIMT接口清单：SIMT编程配套的API接口（[SIMT-API文档目录](../../../api/SIMT-API/SIMT-API.md)）均仅支持Ascend 950PR/Ascend 950DT产品，主要包括：

  - [同步与内存栅栏](../../../api/SIMT-API/sync_and_memory_fence/sync_and_memory_fence.md)：`asc_threadfence`、`asc_threadfence_block`、`asc_syncthreads`等线程同步与内存栅栏接口。
  - [原子操作](../../../api/SIMT-API/atomic_operations/atomic_operations.md)：`asc_atomic_add`、`asc_atomic_sub`、`asc_atomic_max/min`、`asc_atomic_cas`、`asc_atomic_exch`、`asc_atomic_inc/dec`等GM/UB原子操作。
  - [Warp函数](../../../api/SIMT-API/Warp_functions/Warp_functions.md)：Warp内lane id获取、Warp广播（shfl）、投票（vote）、归约（reduce）等函数。
  - [数学函数](../../../api/SIMT-API/math_functions/math_functions.md)：支持half类型、bfloat16_t类型、float类型、fp8类型、整数类型的数学函数（指数、对数、三角函数、幂函数、取整、数据类型转换等，共约600个接口）。
  - [访存函数](../../../api/SIMT-API/memory_access_functions/memory_access_functions.md)：开启Cache Hints的Load/Store函数（`asc_ldca`、`asc_ldcg`、`asc_stcg`、`asc_stwt`、`asc_dcci`等）。
  - [地址空间谓词函数](../../../api/SIMT-API/address_space_predicate_functions/address_space_predicate_functions.md)：`__isGlobal`、`__isLocal`、`__isUbuf`等地址空间判断。
  - [地址空间转换函数](../../../api/SIMT-API/address_space_conversion_functions/address_space_conversion_functions.md)：`__cvta_*`系列地址空间转换接口。
  - [协作组](../../../api/SIMT-API/cooperative_groups/cooperative_groups.md)：`thread_block`、`thread_block_tile`、`coalesced_group`、`grid_group`、`tiled_partition`等协作组接口。

  各接口的详细函数原型、参数与约束请参考[SIMT-API文档目录](../../../api/SIMT-API/SIMT-API.md)中对应文档。

#### 使能说明

SIMT编程在编译时需要增加SIMT编译选项，配置方式如下：

- 使用bisheng命令行编译（独立/动态库/静态库编译均需配置）：

  ```shell
  bisheng <source_file>.asc -o <output_file> --npu-arch=dav-3510 --enable-simt
  ```

- 使用CMake编译工程：

  ```cmake
  set(CMAKE_ASC_ENABLE_SIMT ON)
  ```

> [!NOTE]
> - 设置编译选项`--enable-simt`时，若源文件包含SIMD API的头文件会导致编译失败，SIMT编程与SIMD API不可混用。更多说明请参考[AI Core SIMT编译](../../programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md)。
> - SIMD与SIMT混合编程场景无需额外设置该编译选项，以SIMD方式编译即可。

#### 典型样例说明

样例位置：[hello_world_simt样例](../../../../../examples/03_simt_api/00_introduction/00_quickstart/hello_world_simt)。样例功能：演示SIMT编程的基本流程；更多SIMT样例请参考[examples/03_simt_api目录](../../../../../examples/03_simt_api)及[examples/05_simd_simt_hybrid目录](../../../../../examples/05_simd_simt_hybrid)。

#### 实践指导

- SIMT编程与SIMD/SIMT混合编程的算子实现及性能优化指导，包括[SIMT算子实现](../../operator_practice/simt_operator_impl/simt_operator_impl.md)、[SIMD与SIMT混合编程算子实现](../../operator_practice/simd_simt_hybrid_operator_impl/simd_simt_hybrid_operator_impl.md)和[SIMD与SIMT混合编程算子优化](../../operator_practice/simd_simt_hybrid_optimization/simd_simt_hybrid_optimization.md)，涵盖3510架构下的SIMT线程配置、UB内存访问、分支判断和快速整数除法等实践场景。
- [SIMT算子优化](../../operator_practice/simt_operator_optimization/simt_operator_optimization.md)提供了950专属的SIMT性能优化指导，包括[原子指令优化](../../operator_practice/simt_operator_optimization/instruction_optimization/atomic_instruction_optimization.md)、[通过类型对齐提升搬运效率](../../operator_practice/simt_operator_optimization/memory_access/type_alignment_transfer.md)（950支持1B/2B/4B/8B四种位宽访存指令，通过`alignas`类型对齐到更大位宽可提升GM搬运效率）、[SIMT UB Bank冲突避免](../../operator_practice/simt_operator_optimization/memory_access/avoid_ub_bank_conflict_simt.md)等。
- SIMD与SIMT混合编程相关样例位于[examples/05_simd_simt_hybrid目录](../../../../../examples/05_simd_simt_hybrid)，包括[simd_simt_gather_and_adds样例](../../../../../examples/05_simd_simt_hybrid/00_introduction/simd_simt_gather_and_adds)（入门样例）、[simd_simt_high_performance样例](../../../../../examples/05_simd_simt_hybrid/02_best_practices/simd_simt_high_performance)（混合编程高性能示例）和[simd_simt_grid_dim_config样例](../../../../../examples/05_simd_simt_hybrid/02_best_practices/simd_simt_grid_dim_config)（SIMT线程网格配置）等。

### Matmul高阶API新增数据类型支持<a name="section_matmul_dtype"></a>

#### 特性应用场景说明

- 3510架构扩展了矩阵计算能力。Matmul类高阶API新增支持fp8、hifloat8、float、bfloat16_t、half和int8_t等数据类型的矩阵乘加计算。与2201架构支持的half和int8_t等类型相比，新增类型可覆盖低比特量化和高精度浮点计算等更多业务场景。
- 3510架构的Cube计算单元不再支持2201架构支持的int4b_t数据类型。使用该类型的数据时，需先通过Cast将其转换为int8_t，再执行矩阵计算。详情请参见[矩阵计算兼容性说明](../3510_arch_migration/2201_to_3510_guide/basic_api_migration.md#section17560113713122)，兼容性样例请参见[matmul_s4样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/matmul_s4)。

#### 功能说明

Matmul类高阶API新增支持的数据类型组合如下，其中计算关系为A矩阵×B矩阵，并可选用Bias矩阵，计算结果写入C矩阵。

**表 10**  Matmul高阶API新增数据类型说明<a name="table_matmul_dtype"></a>

| A矩阵 | B矩阵 | Bias矩阵 | C矩阵 | 说明 |
|-------|-------|---------|-------|------|
| fp8_e4m3fn_t、fp8_e5m2_t | fp8_e4m3fn_t、fp8_e5m2_t | float、half、bfloat16_t | fp8_e4m3fn_t、half、bfloat16_t、float | 3510架构新增 |
| hifloat8_t | hifloat8_t | float、half、bfloat16_t | hifloat8_t、half、bfloat16_t、float | 3510架构新增 |
| float | float | bfloat16_t | float、half、bfloat16_t | 3510架构新增 |
| bfloat16_t | bfloat16_t | bfloat16_t | float、half、bfloat16_t | 3510架构新增 |
| half | half | bfloat16_t | float、half、bfloat16_t | 3510架构新增 |
| int8_t | int8_t | int32_t | bfloat16_t | 3510架构新增 |

- 使用要点：

  - 上述新增类型组合的详细参数配置，包括tiling和workspace等，请参见[Matmul高阶API文档](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/Matmul_usage.md)及各数据类型组合对应的样例。
  - Matmul高阶API在3510架构还支持MX（MicroScaling）矩阵乘场景：通过[SetTensorScaleA](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorScaleA.md)、[SetTensorScaleB](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorScaleB.md)接口（Ascend 950PR/Ascend 950DT新增，内部实现使用SSBuffer，需使能[`ENABLE_CV_COMM_VIA_SSBUF`](#section_enable_option)）设置左右矩阵的量化系数矩阵，配合[LoadData MX搬运](#section_mx)实现MX格式的小数低比特矩阵乘。样例请参考[mmad_mx样例](../../../../../examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_mx)。
  - 有关3510架构新增的fp8_e4m3fn_t、fp8_e5m2_t、hifloat8_t、fp4x2_e1m2_t和fp4x2_e2m1_t等内置数据类型，请参见[数据类型介绍](../../../api/SIMD-API/basic_api/data_structures/builtin_data_types.md)。量化、反量化和伪量化类高阶API（AscendQuant、AscendDequant和AscendAntiQuant）也扩展支持上述类型，详情请参见[高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md)。
  - **Matmul输出量化/反量化场景**：3510架构新增支持的Matmul输出随路量化/反量化数据类型组合包括：int8_t×int8_t→bfloat16_t、fp8_e4m3fn_t/fp8_e5m2_t→fp8_e4m3fn_t/half/bfloat16_t/float、hifloat8_t→hifloat8_t/half/bfloat16_t/float（均仅Ascend 950PR/Ascend 950DT支持）；通过[SetDequantType](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Tiling/SetDequantType.md)（Tiling侧）、[SetQuantScalar](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetQuantScalar.md)/[SetQuantVector](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetQuantVector.md)（核函数侧）配置量化/反量化模式，详细说明请参考[矩阵乘输出的量化/反量化](../../operator_practice/simd_operator_impl/matrix_advanced_api/feature_scenarios/matmul_output_quant_dequant.md)。
  - **Mix融合算子Scenario 2（L0C直接写UB）**：在CV融合算子的深度性能优化场景中，3510架构支持Scenario 2方式，仅适用于Ascend 950PR/Ascend 950DT。AIC侧通过携带`FixpipeParamsArch3510`参数的Fixpipe接口，将L0C计算结果直接写入UB；AIV侧无需从GM读取数据，可直接在UB中执行GELU或LeakyReLU等计算并将结果写回GM，从而避免GM中转产生的读写开销。该场景是[L0C->UB搬运数据通路](#section_fixpipe_l0c2ub)在融合算子中的典型应用。详情请参见[matmul_leakyrelu_fusion_operator极限优化](../../operator_practice/typical_operator_cases/simd_operator_impl/matmul_leakyrelu_fusion_operator/extreme_optimization.md)。

### 高阶API扩展能力总览<a name="section_adv_api_ext"></a>

#### 特性应用场景说明

- 除Matmul类高阶API外，3510架构还扩展了数学计算、数据归一化、量化、归约、排序、索引计算、数据过滤和张量变换等高阶API，新增数据类型、算法配置和数据排布转换场景。
- 上述能力基于3510架构的硬件能力扩展，例如新增fp8、hifloat8等数据类型，并采用AIV核Regbase架构。开发者可根据业务场景选用相应能力。有关兼容性的详细说明，请参见[高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md)。

#### 功能说明

3510架构高阶API的扩展能力如下：

**表 11**  高阶API扩展能力总览<a name="table_adv_api_ext"></a>

| API类别 | 涉及API | 扩展能力 |
|---------|--------|---------|
| 数学计算 | Tanh、Asin、Sin、Acos、Cos、Log、Atan、Fmod | 扩展支持算法配置（通过模板参数配置API使用的算法，提供高精度、高性能的算法选择） |
| 数学计算 | Power | 扩展支持uint8_t、int8_t、uint16_t、int16_t、uint32_t、bfloat16_t数据类型 |
| 数学计算 | Sign | 扩展支持int64_t数据类型 |
| 数据归一化 | Normalize、WelfordUpdate | 扩展支持bfloat16_t数据类型 |
| 数据归一化 | LayerNorm | 扩展支持求方差 |
| 数据归一化 | WelfordFinalize | 扩展支持算法配置（指定计算方差时是否使用修正系数） |
| 量化操作 | AscendQuant、AscendDequant、AscendAntiQuant | 扩展支持PRE_TOKEN、PRE_GROUP量化方式；扩展支持fp8_e5m2_t、fp8_e4m3fn_t、hifloat8_t、fp4x2_e1m2_t、fp4x2_e2m1_t等数据类型的量化和反量化 |
| 归约操作 | ReduceSum | 扩展支持int32_t、uint32_t、int64_t、uint64_t数据类型 |
| 归约操作 | ReduceMax、ReduceMin | 扩展支持int8_t、uint8_t、int16_t、uint16_t、bfloat16_t、int32_t、uint32_t、int64_t、uint64_t数据类型 |
| 排序操作 | TopK | 使用RADIX_SELECT算法时，扩展支持uint8_t、int8_t、uint16_t、int16_t、uint32_t、int32_t、bfloat16_t、uint64_t、int64_t数据类型 |
| 排序操作 | Sort | 扩展支持算法配置（指定排序算法以及降序升序排序） |
| 索引计算 | Arange | 扩展支持int64_t数据类型 |
| 数据过滤 | DropOut | 扩展支持bfloat16_t数据类型 |
| 张量变换 | Transpose | 新增支持数据排布转换场景：二维转置或三维的后两位转置、三维中的第一维和第二维互换、三维中的第一维和第三维互换、使用交织指令对二维ND2NZ转置 |
| 张量变换 | BroadCast | 扩展支持动态Shape；扩展支持int16_t、uint16_t、bfloat16_t、int32_t、uint32_t数据类型 |
| 张量变换 | Fill | 扩展支持uint8_t、int8_t、bfloat16_t、uint64_t、int64_t数据类型 |
| 数学计算 | [BitwiseAnd](../../../api/SIMD-API/adv_api/math_compute/BitwiseAnd_interface/BitwiseAnd.md)、[BitwiseNot](../../../api/SIMD-API/adv_api/math_compute/BitwiseNot_interface/BitwiseNot.md)、[BitwiseOr](../../../api/SIMD-API/adv_api/math_compute/BitwiseOr_interface/BitwiseOr.md)、[BitwiseXor](../../../api/SIMD-API/adv_api/math_compute/BitwiseXor_interface/BitwiseXor.md)、[LogicalAnd](../../../api/SIMD-API/adv_api/math_compute/LogicalAnd_interface/LogicalAnd.md)、[LogicalAnds](../../../api/SIMD-API/adv_api/math_compute/LogicalAnds_interface/LogicalAnds.md)、[LogicalNot](../../../api/SIMD-API/adv_api/math_compute/LogicalNot_interface/LogicalNot.md)、[LogicalOr](../../../api/SIMD-API/adv_api/math_compute/LogicalOr_interface/LogicalOr.md)、[LogicalOrs](../../../api/SIMD-API/adv_api/math_compute/LogicalOrs_interface/LogicalOrs.md)、[LogicalXor](../../../api/SIMD-API/adv_api/math_compute/LogicalXor_interface/LogicalXor.md) | 位运算/逻辑运算接口，仅Ascend 950PR/950DT支持 |
| 数学计算 | [Fma](../../../api/SIMD-API/adv_api/math_compute/Fma_interface/Fma.md)、[Clamp](../../../api/SIMD-API/adv_api/math_compute/Clamp_interface/Clamp.md)、[Hypot](../../../api/SIMD-API/adv_api/math_compute/Hypot_interface/Hypot.md)、[Rint](../../../api/SIMD-API/adv_api/math_compute/Rint_interface/Rint.md)、[SinCos](../../../api/SIMD-API/adv_api/math_compute/SinCos_interface/SinCos.md)、[Where](../../../api/SIMD-API/adv_api/math_compute/Where_interface/Where.md)、[IsFinite](../../../api/SIMD-API/adv_api/math_compute/IsFinite_interface/IsFinite.md)、[IsInf](../../../api/SIMD-API/adv_api/math_compute/IsInf_interface/IsInf.md)、[IsNan](../../../api/SIMD-API/adv_api/math_compute/IsNan_interface/IsNan.md) | 融合乘加/限幅/数学函数/判断类接口，仅Ascend 950PR/950DT支持 |
| 量化操作 | [Quantize](../../../api/SIMD-API/adv_api/quantization/Quantize.md)、[Dequantize](../../../api/SIMD-API/adv_api/quantization/Dequantize.md)、[AntiQuantize](../../../api/SIMD-API/adv_api/quantization/AntiQuantize.md) | 量化/反量化/伪量化接口，仅Ascend 950PR/950DT支持 |
| 随机函数 | [PhiloxRandom](../../../api/SIMD-API/adv_api/random_functions/PhiloxRandom.md) | 基于Philox算法的随机数生成接口，仅Ascend 950PR/950DT支持 |

#### 使用说明

- 上述扩展能力的具体API原型、模板参数配置及使用方式，请参考对应API文档及[高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717)。
- 高阶API相关样例位于`examples/01_simd_cpp_api/04_advanced_api`目录，各API对应样例请参考该目录说明。

### HCCL通信新增能力<a name="section_hccl"></a>

#### 特性应用场景说明

在多卡通信算子中，HCCL高阶API基于窗口（Window）机制实现多卡间的数据收发。3510架构新增CCU（Compute Communication Unit）服务端类型`HCCL_SERVER_TYPE_CCU`，并新增通信配置和数据下发接口，用于在多卡通信场景中配置归约类型，以及自定义窗口内的数据收发。

#### 功能说明

- [SetReduceType](../../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Tiling/SetReduceType.md)（Ascend 950PR/Ascend 950DT新增）：Tiling侧配置接口，用于设置Reduce操作类型，仅对包含归约操作的通信任务生效。例如，当ReduceScatter任务的输入数据类型为int8_t、hifloat8_t、fp8_e5m2_t或fp8_e4m3fn_t时，输出数据类型必须为half、bfloat16_t或float。
- [AlltoAllvWrite](../../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AlltoAllvWrite.md)（Ascend 950PR/Ascend 950DT新增）：Kernel侧通信数据写入接口。通信域内的每张卡均可自定义向其他卡发送的数据量（`sendSizes`）及数据在内存中的偏移（`sendOffsets`），还可配置对端窗口的写入偏移（`remoteWinOffset`）和本卡接收的数据量（`localDataSize`）。接口返回任务标识`handleId`。
- 使用要点：

  - AlltoAllvWrite调用前需要先调用InitV2和SetCcTilingV2接口完成初始化；CCU服务端默认仅支持FullMesh算法，单次最大通信数据量不超过256M。
  - 一个通信域内所有Prepare和InterHcclGroupSync的调用总次数不超过63次。
  - 未指定下发核时，AlltoAllvWrite只能在AIC或AIV之一上调用；AI CPU服务端暂不支持上述能力。

### 其他增强说明

除上述特性外，3510架构还提供以下增强能力与兼容性样例。详情请参见[2201到3510架构变更](../3510_arch_migration/2201_to_3510_arch_changes.md)。

#### 架构规格

- **AI Core核数增加**：3510架构的AI Core核数较2201架构有所增加，可支持更大的片上并行规模。
- **UB容量提升**：3510架构UB容量提升（总容量256KB，16个bank组成8个bank group，每个bank group包含2个bank，每个bank 16KB），若发生UB冲突，可参考[避免UB的bank冲突](../../operator_practice/simd_operator_optimization/memory_access/avoid_ub_bank_conflict/overview.md)解决；相关样例请参考[bank_conflict_3510样例](../../../../../examples/01_simd_cpp_api/05_best_practices/04_memory_access/bank_conflict_3510)（UB Bank冲突场景验证与分析（架构3510））。

#### 数据类型与数据结构

- **数据类型扩展**：3510架构扩展了支持的数据类型（如fp8_e4m3fn_t/fp8_e5m2_t、hifloat8_t、fp4x2_e1m2_t/fp4x2_e2m1_t等），具体可参考[数据类型介绍](../../../api/SIMD-API/basic_api/data_structures/builtin_data_types.md)。其中bfloat16_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、fp8_e8m0_t、fp4x2_e2m1_t、fp4x2_e1m2_t、int4x2_t等数据类型在Host端支持以C++模板形式定义与声明（较A2/A3仅支持bfloat16_t范围更广），编译约束请参考[约束说明](../../programming_guide/compilation_and_execution/operator_compilation/constraints.md)。
- **新增复数数据类型**：新增[complex32/complex64](../../../api/SIMD-API/basic_api/aux_data_structures/complex32-complex64.md)数据类型。complex32表示实部和虚部均为half类型且总位宽为32位的复数；complex64表示实部和虚部均为float类型且总位宽为64位的复数。上述类型仅支持Ascend 950PR/Ascend 950DT。
- **GlobalTensor类型重解释**：新增[ReinterpretCast（GlobalTensor类型重解释）](../../../api/SIMD-API/basic_api/data_structures/GlobalTensor/ReinterpretCast.md)接口，可将当前GlobalTensor重新解释为用户指定的类型。该接口仅支持Ascend 950PR/Ascend 950DT。

#### 缓存管理

- **新增Cache类型**：3510架构新增支持SIMT DCache与NDDMA Cache（2201架构仅支持L2 Cache、DCache和ICache），其中SIMT DCache用于SIMT线程访问Global Memory时的数据缓存，NDDMA Cache用于ND-DMA指令（[NDDMA多维搬运](#section_nddma)）的数据缓存，多核间数据一致性说明请参考[Cache一致性](../../programming_guide/advanced_programming/memory_model/cache_coherence.md)。完整Cache类型与功能说明请参考[系统缓存概述](../../../api/SIMD-API/basic_api/cache_control/system_cache_overview.md)。
- **Cache控制接口增强**：3510架构下的[DataCacheCleanAndInvalid](../../../api/SIMD-API/basic_api/cache_control/DataCacheCleanAndInvalid.md)接口新增支持`CACHELINE_ATOMIC`参数，用于在原子操作过程中保证DCache与GM的一致性。该参数仅支持Ascend 950PR/Ascend 950DT，A3、A2等其他产品暂不支持。

#### 编译选项与空间优化

- **UB空间使用与编译选项**：UB总容量为256KB，其中部分空间可能用于系统预留或划分为Data Cache。3510架构下Ascend C接口内部使用**2KB**预留UB空间（2201架构为8KB），用于指令兼容或存储中间数据。以下编译选项生效，可调整可用的UB空间大小：
  - `--cce-disable-asc-reserved-ubuf`：禁用Ascend C接口使用预留UB空间，开启后依赖预留UB空间的接口不可用（3510架构下使用预留UB空间的API范围请参考[使用预留UB空间的API列表](../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/static_tensor_programming.md#使用预留ub空间的api范围)中的表6）。
  - `--cce-disable-vf-stack-reserved-ubuf`：禁用SIMD VF栈预留的UB空间（3510架构生效，开启后可释放6KB预留空间作为普通UB使用，但编译器无法再使用该空间缓存寄存器溢出数据，需保证寄存器不溢出）。
  - SIMD与SIMT混合编程场景下，开启上述两个编译选项后，用户最多可使用的实际UB空间大小为`248KB + 6KB + 2KB - 32KB = 224KB`（默认可用UB空间为248KB，248KB = 256KB - 6KB（VF栈预留） - 2KB（Ascend C预留）；开启上述两个编译选项后释放6KB和2KB预留空间，混合编程场景还需从UB中划分32KB至128KB作为SIMT线程访问GM的Data Cache）。
- **性能优化编译宏**：`NO_OVERLAP_IN_MULTI_REPEAT`（950架构专属）可在没有地址重叠的情况下移除不必要的内存同步指令，提升基础API高维切分计算场景的性能，详情请参考[内置编译宏开关](../../programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md#内置编译宏开关)。
- **DCI编译选项（性能优化，950独占）**：`--cce-no-dcache-flush`编译选项可在算子尾部插入DCI（DataCacheInvalid）指令替代默认的DCCI（DataCacheCleanAndInvalid）指令使DCache失效，减少数据从DCache同步回GM（Clean）的过程，降低算子尾开销，详细说明请参考[新增DCI数据缓存失效能力](#section_dci)。

#### SIMT相关增强

- **SIMT超大容量寄存器**：3510架构为SIMT应用程序提供总容量为128KB的超大容量寄存器（每个Thread可用的寄存器数和Thread数量相关）。SIMT访问GM需经过SIMT DCache中转，该DCache直接占用UB存储空间，最大支持128KB，对外访存以128B为粒度。详细说明请参考[NPU架构规格](../../programming_guide/advanced_programming/hardware_implementation/architecture_spec/npu_arch_3510.md)。
- **SIMT协作组调度约束**：Ascend 950PR/Ascend 950DT的SIMT架构**不支持独立线程调度**，一个Warp内的多个[协作组](../../programming_guide/advanced_programming/simt_cooperative_groups.md)之间应避免存在相互等待或数据依赖，否则可能导致卡死。

#### 融合编译特性

- **SuperKernel二进制融合特性（试验）**：3510架构初次支持SuperKernel特性（算子的二进制融合技术，将多个子核函数融合为一个超级核函数，减少Task调度开销、实现算子间流水并行），当前在Ascend 950PR/Ascend 950DT为试验特性（后续版本可能调整，不保证兼容性），支持通过PyTorch图模式（npugraph_ex/GE图模式）开启，详细说明请参考[SuperKernel原理](../../programming_guide/advanced_programming/super_kernel/principles.md)。

#### 兼容性样例

- **兼容性样例**：涉及2201到3510架构变更（如L1 Buffer到GM通路删除、SetLoadDataBoundary删除、int4b_t不支持、L0A Buffer分形变化、Fill接口不支持直接初始化L0A/L0B等）的兼容性适配样例，请参考[兼容性样例说明](../3510_arch_migration/2201_to_3510_arch_changes.md)中"影响"列对应的[06_compatibility_guide样例目录](../../../../../examples/01_simd_cpp_api/06_compatibility_guide)，包括[data_copy_l1togm样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/data_copy_l1togm)、[set_loaddata_boundary样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/set_loaddata_boundary)、[matmul_s4样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/matmul_s4)、[pattern_transformation样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/pattern_transformation)、[fill样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/fill)。
