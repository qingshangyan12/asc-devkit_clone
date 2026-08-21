# 3510新特性使用说明

本文介绍[NPU架构版本3510](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)新增特性的能力说明与使能方法，帮助从其它架构迁移到3510架构的开发者使能并使用硬件新增的数据通路等能力。3510架构的整体变更情况请参考[2201到3510架构变更](../3510_arch_migration/2201_to_3510_arch_changes.md)。

## 特性总览

**表1** 3510新特性总览<a name="table_3510_new_feature_overview"></a>

| 特性 | 特性说明 | 相关基础API接口 | 相关C API接口 |
|------|---------|-----------------|----------------|
| [新增UB->L1搬运数据通路](#section_ub2l1) | 支持将数据从UB直接搬运到L1 Buffer，无需经过GM中转，缩短数据搬运链路、提升性能（需配置`ENABLE_CV_COMM_VIA_SSBUF=true`） | [DataCopy（UBToL1）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_continuous.md)、[DataCopyPad（UBToL1）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopyPad_UBToL1.md) | [asc_copy_ub2l1](../../../api/SIMD-API/c_api/vector_data_move/asc_copy_ub2l1.md) |
| [新增L1->UB搬运数据通路](#section_l12ub) | 支持Mix场景下将数据从L1 Buffer直接搬运至指定Vector核的UB（按subBlockId写入），无需经过GM中转 | [DataCopyL1ToUB](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopyL1ToUB.md) | [asc_copy_l12ub](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l12ub.md) |
| [新增L0C->UB搬运数据通路](#section_fixpipe_l0c2ub) | 支持将矩阵计算结果从L0C Buffer直接搬运到UB（Fixpipe实现，L0C->UB不支持DataCopy）；L0C->L1 Buffer/L0C->GM支持Fixpipe和DataCopy两种方式 | [Fixpipe（L0CToUB）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md)、[DataCopy（L0CToL1）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopy_L0CToL1.md)、[DataCopy（L0CToGM）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopy_L0CToGM.md) | [asc_copy_l0c2ub](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l0c2ub.md)、[asc_copy_l0c2l1（arch3510）](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md)、[asc_copy_l0c2gm（arch3510）](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md) |
| [新增ND-DMA指令](#section_nddma) | 扩展DataCopy能力，可自由配置搬入数据的维度信息及Stride，支持转置、Broadcast、Slice、Padding等场景 | [DataCopy（GMToUB NDDMA）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMToUB_NDDMA.md)、[NdDmaDci](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/NdDmaDci.md) | [asc_ndim_copy_gm2ub](../../../api/SIMD-API/c_api/vector_data_move/asc_ndim_copy_gm2ub.md) |
| [DataCopy/DataCopyPad支持loop模式](#section_loop_mode) | 支持L1 Buffer与GM之间、GM与UB之间通路的loop模式搬运，配合DataCopyPad可实现五维数据搬运 | [SetLoopModePara](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/SetLoopModePara.md)、[ResetLoopModePara](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/ResetLoopModePara.md) | [asc_set_gm2l1_loop_size等](../../../api/SIMD-API/c_api/cube_data_move/asc_set_gm2l1_loop_size.md)、[asc_set_ub2gm_loop1_stride等](../../../api/SIMD-API/c_api/vector_data_move/asc_set_ub2gm_loop1_stride.md) |
| [LoadData支持MicroScaling场景搬运](#section_mx) | LoadData新增支持MX场景数据搬运，将矩阵数据与其量化系数矩阵同步搬入L0A/L0B及L0A_MX/L0B_MX | [LoadData（2D MX搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_MX.md)、[MmadMx](../../../api/SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/MmadMx.md) | [asc_copy_l12l0a_mx](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l12l0a_mx.md)、[asc_copy_l12l0b_mx](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l12l0b_mx.md)、[asc_mmad_mx](../../../api/SIMD-API/c_api/cube_compute/asc_mmad_mx.md) |
| [LoadDataWithTranspose新增分形转置能力](#section_transpose) | 新增DN分型及`LoadData2dTransposeParamsV2`参数，支持在搬运过程中完成分形转置；L1 Buffer->L0A Buffer不再支持transpose | [LoadDataWithTranspose](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadDataWithTranspose.md) | [asc_copy_l12l0b_trans（arch3510）](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.md) |
| [Fixpipe新增NZ2DN/NZ2ND随路转换](#section_nz2dn) | Fixpipe搬出过程支持NZ2DN、NZ2ND随路格式转换 | [Fixpipe（L0CToGM/L1/UB）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md) | [asc_set_l0c2gm_nz2nd](../../../api/SIMD-API/c_api/cube_compute/asc_set_l0c2gm_nz2nd.md) |
| [新增SSBuffer核内存储单元](#section_ssbuf) | 新增SSBuffer核内存储单元，支持AIC核与AIV核间的直接通信（Scalar访问），无需经过GM（需配置`ENABLE_CV_COMM_VIA_SSBUF=true`） | [GetSsbufBaseAddr](../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSsbufBaseAddr.md) | / |
| [新增Mutex核内同步能力](#section_mutex) | 新增Mutex能力，用于核内异步流水指令之间的同步控制，功能类似于传统CPU中的锁机制 | [Mutex::Lock/Unlock](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)、[AllocMutexID](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/AllocMutexID_ISASI.md) | [asc_lock](../../../api/SIMD-API/c_api/sync/asc_lock.md)、[asc_unlock](../../../api/SIMD-API/c_api/sync/asc_unlock.md) |
| [新增核间同步控制模式](#section_crosscore) | CrossCoreSetFlag/CrossCoreWaitFlag新增模式4，AIV0与AIV1可单独触发AIC等待 | [CrossCoreSetFlag/CrossCoreWaitFlag（ISASI）](../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md) | [asc_sync_inter_arrive/wait](../../../api/SIMD-API/c_api/sync/asc_sync_inter_arrive.md) |
| [新增Regbase矢量计算方式](#section_regbase) | AIV核采用Regbase架构，支持直接操作Vector寄存器，中间结果可暂存寄存器，减少UB读写 | [Reg::RegTensor/MaskReg等Reg基础API](../../../api/SIMD-API/basic_api/reg_vector_compute/overview.md) | [asc_预置寄存器C API全系列](../../../api/SIMD-API/c_api/c_api.md) |
| [Matmul高阶API新增数据类型支持](#section_matmul_dtype) | Matmul类高阶API新增支持fp8/hifloat8/float/bfloat16_t/half/int8_t等数据类型矩阵乘及MX矩阵乘场景 | [Matmul高阶API](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/Matmul_usage.md)、[SetTensorScaleA/B](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorScaleA.md) | [asc_mmad等Cube计算C API](../../../api/SIMD-API/c_api/c_api.md) |
| [高阶API扩展能力](#section_adv_api_ext) | 数学计算、归约、排序、张量变换、量化等类高阶API扩展支持新的数据类型、算法配置与排布转换场景，并新增位运算/量化/随机数等950独占高阶API | 对应高阶API（详见[高阶API扩展能力总览](#section_adv_api_ext)） | / |
| [Hccl通信新增能力](#section_hccl) | Hccl新增SetReduceType、AlltoAllvWrite接口，并支持HCCL_SERVER_TYPE_CCU服务端类型 | [SetReduceType](../../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Tiling/SetReduceType.md)、[AlltoAllvWrite](../../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AlltoAllvWrite.md) | / |
| [新增矩阵计算基础API扩展](#section_cube_api_ext) | 新增LoadData 2D V2/BitMode、LoadDataWithStride、MmadMx、MmadBitMode等矩阵计算基础API，扩展矩阵搬运与计算能力 | 对应基础API（详见[矩阵计算基础API扩展](#section_cube_api_ext)） | [asc_copy_gm2l1（arch3510）](../../../api/SIMD-API/c_api/cube_data_move/asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.md)等 |
| [新增矢量计算基础API扩展](#section_vec_api_ext) | 新增flexible_scalar标量变体、ISASI接口、交织/逻辑计算等矢量计算基础API，扩展矢量编程能力 | 对应基础API（详见[矢量计算基础API扩展](#section_vec_api_ext)） | [asc_add/asc_mul等寄存器算术C API](../../../api/SIMD-API/c_api/reg/arithmetic_compute/asc_add.md)等 |
| [新增原子操作接口](#section_atomic_api) | 新增GM地址原子操作接口（原子加/交换/比较交换/最大/最小） | [AtomicAdd/AtomicMax等](../../../api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md) | [asc_atomic_add/asc_atomic_cas等](../../../api/SIMD-API/c_api/scalar_compute/asc_atomic_add.md) |
| [新增系统变量与工具接口](#section_sys_api) | 新增特殊寄存器访问（GetSpr/ClearSpr等）、UB大小/矢长查询、数值极限、性能打点等系统变量与工具接口 | 对应基础API（详见[系统变量与工具接口](#section_sys_api)） | [asc_get_vf_len等C系统变量接口](../../../api/SIMD-API/c_api/sys_var/asc_get_vf_len.md) |
| [新增TSCM资源管理接口](#section_tscm) | 新增TSCM存储位置（TPosition::TSCM）及队列管理接口，用于矩阵计算中临时数据交换 | [TSCM::AllocTensor/EnQue等](../../../api/SIMD-API/basic_api/resource_management/TSCM/AllocTensor.md) | / |

> [!NOTE]
> 上表中未单独列出的三类特性说明如下：
>
> - **新增SIMT编程模型**（[详细说明](#section_simt)）：为独立的编程模型，配套使用[SIMT-API接口集](../../../api/SIMT-API/SIMT-API.md)（同步与内存栅栏、原子操作、Warp函数、数学函数、访存函数、地址空间函数、协作组等，均仅支持Ascend 950PR/Ascend 950DT），与上表所列基础API/高阶API（C++ Tensor方式）和寄存器C API属于不同编程体系，编译时需配置`--enable-simt`，因此不纳入上表。
> - **新增Tensor API编程接口**：为基于`tensor_api/tensor.h`的试验接口（数据结构类Tensor/MakeTensor/MakeLayout等，矩阵计算类Copy/Mmad等），采用"内存指针+布局（Layout）"绑定的编程范式，由Layout自动推导搬运与计算参数，无对应的寄存器C API版本，因此不纳入上表，具体接口清单请参考[数据结构（TensorAPI）](../../../api/SIMD-API/basic_api/data_structure_TensorAPI/data_structure_TensorAPI.md)与[矩阵计算（TensorAPI，试验特性）](../../../api/SIMD-API/basic_api/cube_compute_TensorAPI/cube_compute_TensorAPI.md)。
> - **新增寄存器C API**：为`asc_`前缀的寄存器C编程接口（头文件`asc_simd.h`，覆盖矢量计算、数据搬运、标量计算、原子操作、同步、系统变量访问等，均仅支持Ascend 950PR/Ascend 950DT），与上表所列基础API/高阶API（C++ Tensor方式）属于不同的编程体系（与[Regbase矢量计算方式](#section_regbase)中的"C指针方式"对应），因此不纳入上表，具体接口清单请参考[C API文档](../../../api/SIMD-API/c_api/c_api.md)。

## 特性说明

### 新增UB->L1搬运数据通路<a name="section_ub2l1"></a>

#### 特性应用场景说明

- 在矩阵计算（Cube计算）相关算子（如Matmul算子、Vector与Cube融合算子）中，Vector核（AIV）侧计算或搬运产生的矩阵数据存放于UB（TPosition为VECIN/VECCALC/VECOUT），需要作为Cube核（AIC）侧矩阵乘加计算的输入矩阵使用。该类算子的典型数据流为：数据先从GM搬运到UB，再由UB搬运到L1 Buffer，之后通过LoadData从L1 Buffer搬入L0A/L0B Buffer并执行Mmad矩阵乘加计算。
- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等更早架构上，UB与L1 Buffer之间没有直接的硬件搬运通路，UB->L1 Buffer的数据搬运需要经过GM中转（UB->GM->L1 Buffer），或借助Matmul高阶API的workspace GM空间实现，中转链路较长，引入额外开销。
- 3510架构新增UB->L1 Buffer硬件数据通路，使能后数据可从UB直达L1 Buffer，免去GM中转环节，降低搬运开销。

#### 功能说明

本特性涉及的API为UB->L1 Buffer的数据搬运基础API，头文件路径为：`basic_api/kernel_operator_data_copy_intf.h`，核函数内一般通过`kernel_operator.h`间接包含使用。相关API及功能说明如下：

**表2** UB->L1 Buffer数据搬运API说明<a name="table_ub2l1_api"></a>

| API | 功能说明 | 函数原型 |
|-----|---------|---------|
| [DataCopy（UBToL1连续数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_continuous.md) | 将矩阵从UB连续搬运至L1 Buffer，数据搬运时格式和内容保持不变 | `DataCopy(dst, src, count)` |
| [DataCopy（UBToL1高维切分数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_highdim_split.md) | 将矩阵从UB搬运至L1 Buffer，支持连续搬运和非连续搬运 | `DataCopy(dst, src, DataCopyParams)` |
| [DataCopy（UBToL1随路转换-ND2NZ搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopy_UBToL1_ND2NZ.md) | 将矩阵从UB搬运至L1 Buffer，搬运过程中完成ND->NZ格式转换 | `DataCopy(dst, src, Nd2NzParams)` |
| [DataCopyPad（UBToL1非对齐数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/DataCopyPad_UBToL1.md) | 将矩阵从UB搬运至L1 Buffer，支持非对齐数据搬运及按需填充 | `DataCopyPad(dst, src, DataCopyExtParams, Nd2NzParams)` |

- 数据通路选择：

  - 使能编译选项[`ENABLE_CV_COMM_VIA_SSBUF`](#section_enable_option)后，使用SSBuffer进行通信，数据通过UB->L1 Buffer之间的硬件通道直接搬运（推荐），该方式无需经过GM中转，性能较高。
  - 未使能编译选项时，数据搬运到L1 Buffer需要经过GM。该场景下UB->L1 Buffer搬运接口为软件仿真实现，在Matmul高阶API的基础上利用其workspace GM空间作为数据中转空间，使用前需要先使用`REGISTER_MATMUL`注册高阶API。

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

**表3** 编译选项说明<a name="section_enable_option"></a>

| 编译选项 | 可选值 | 说明 |
|---------|-------|------|
| `ENABLE_CV_COMM_VIA_SSBUF` | `true` | 控制是否使用SSBuffer以及UB到L1 Buffer的硬通道，涉及CV通信（AIC和AIV）或使用数据搬运API时需关注该选项，开启后可以提高相关API的性能或扩展更多功能。默认开关关闭，仅在Ascend 950PR/Ascend 950DT支持该选项。 |

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
> - 从其它硬件平台移植到3510平台的算子，该编译选项默认关闭以保持兼容性；需要在本平台使用UB->L1 Buffer硬件通路时，请显式配置`ENABLE_CV_COMM_VIA_SSBUF=true`。更多说明请参考[内置编译宏开关](../../programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md#内置编译宏开关)。
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

- 在Vector与Cube融合（Mix）算子场景下（AIC:AIV=1:2），Cube核（AIC）侧的数据或计算结果存放于L1 Buffer，Vector核（AIV）侧后续计算需要数据在UB中；反之，Vector核计算完成的数据也可能需要回到L1 Buffer参与下一轮Mmad计算。
- 3510架构新增L1 Buffer到UB的数据搬运能力（`DataCopyL1ToUB`接口，均仅支持Ascend 950PR/Ascend 950DT产品），支持Mix场景下由Cube核发起、按subBlockId将数据从L1 Buffer直接搬运至指定Vector核的UB。

#### 功能说明

本特性涉及的API为[DataCopyL1ToUB（L1到UB数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/DataCopyL1ToUB.md)，头文件路径为`basic_api/kernel_operator_data_copy_intf.h`，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

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
- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等更早架构上，L0C Buffer与UB之间没有直接的硬件搬运通路，L0C->UB的数据搬运需经GM中转（L0C->GM->UB），链路较长且开销明显。
- 3510架构新增L0C Buffer到UB的单向数据通路，使能后数据可从L0C Buffer直达UB，省去GM中转步骤，减小搬运开销。

#### 功能说明

本特性涉及的API为[Fixpipe（L0C到UB数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToUB.md)，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

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

- 在Vector算子中，从GM向UB搬运数据是最常见的数据搬运场景。当搬运的数据涉及多维矩阵、步长（Stride）不规则、需要填充（Padding）、转置（Transpose）、广播（Broadcast）、切片（Slice）等复杂数据变换时，使用基础DataCopy接口需要拆分多条搬运指令并通过Vector计算配合实现，编码复杂度较高且性能受限。
- 3510架构新增ND-DMA硬件指令，相比基础数据搬运接口，可更加自由地配置搬入数据的维度信息及Stride，一条指令即可完成上述复杂场景的数据搬运。

#### 功能说明

本特性涉及的API为[DataCopy（GMToUB多维数据搬运NDDMA）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMToUB_NDDMA.md)，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

- 支持维度`dim∈[1, 5]`的多维数据搬运，支持逐维配置Stride，支持左右Padding（常数填充或最近值填充）、Transpose、Broadcast、Slice等场景。
- 函数原型：

  ```cpp
  template <typename T, uint8_t dim, const NdDmaConfig& config = kDefaultNdDmaConfig>
  __aicore__ inline void DataCopy(const LocalTensor<T>& dst, const GlobalTensor<T>& src,
                                  const NdDmaParams<T, dim>& params)
  ```

- 使用要点：

  - 该数据通路仅支持GM->UB（GM->VECIN）方向。
  - 一条指令所获取数据地址范围宽度不超过40位（1TB）；每层循环dstStride为升序时，不同循环地址空间不能交织或重叠。
  - NDDMA Cache仅32KB，多核读同一GM地址时，搬运前必须调用[NdDmaDci](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/NdDmaDci.md)接口刷新NDDMA Cache，否则可能读到错误数据。
  - 数据类型为b64（int64_t/uint64_t/double/complex64）时，不能使能最近值填充，constantValue必须为0。

#### 典型样例说明

样例位置：[data_copy_gm2ub_nddma样例](../../../../../examples/01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2ub_nddma)。样例功能：基于ND-DMA指令实现GM->UB多维数据搬运，模拟多维Shape下移动窗口（Slice）的数据搬运，包含输入Stride和不连续源数据搬运的配置方式。

### DataCopy/DataCopyPad新增loop模式<a name="section_loop_mode"></a>

#### 特性应用场景说明

- 在Vector算子中，当需要将GM中的多段数据交替搬运到UB的不同地址（或反向搬运）时，通常需要循环执行多次DataCopy搬运指令，指令条数多、流水开销大。
- 3510架构新增loop模式数据搬运能力，一条DataCopy/DataCopyPad指令即可完成loop循环搬运，减少指令下发开销，提升搬运效率。

#### 功能说明

本特性涉及的API为[SetLoopModePara（loop模式参数设置）](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/SetLoopModePara.md)，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

- 通过`SetLoopModePara`接口使能loop模式并设置loop模式参数，支持L1 Buffer与GM之间、GM与UB之间通路的loop模式搬运，与[DataCopyPad](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopyPad_GMToUB.md)搭配使用时还可实现五维数据搬运等场景。
- 函数原型：

  ```cpp
  __aicore__ inline void SetLoopModePara(const LoopModeParams& loopParams, const DataCopyMVType type)
  ```

  - `LoopModeParams`：loop模式参数，包含loop1Size、loop2Size、loop1SrcStride、loop1DstStride、loop2SrcStride、loop2DstStride。
  - `DataCopyMVType`：搬运数据通路类型，支持`UB_TO_OUT`（UB->GM方向）和`OUT_TO_UB`（GM->UB方向）。

- 使用要点：

  - 源地址和目的地址均要求32字节对齐。
  - loop模式使用后（一次DataCopy/DataCopyPad搬运结束后），必须通过[ResetLoopModePara](../../../api/SIMD-API/basic_api/memory_vector_compute/data_move_aux_config/ResetLoopModePara.md)接口复位loop模式参数，否则影响后续数据搬运。
  - 目的数据不能重叠，不同迭代间的目的数据可交织。

### LoadData新增MX矩阵搬运<a name="section_mx"></a>

#### 特性应用场景说明

- MicroScaling（MX）是一种低比特量化数据格式（如fp4x2_e1m2_t、fp4x2_e2m1_t、fp8_e4m3fn_t、fp8_e5m2_t），矩阵数据与每块（Block）对应的量化系数（Scale）矩阵一同存储，矩阵计算时需要同时搬运矩阵数据与量化系数矩阵。
- 3510架构新增支持MicroScaling场景的数据搬运能力，实现矩阵数据与量化系数矩阵的同步搬运，支撑MX低比特格式下的高效矩阵计算。

#### 功能说明

本特性涉及的API为[LoadData（L1到L0搬运MX）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadData_2D_MX.md)，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

- LoadData新增的MX搬运能力负责完成矩阵计算所需的左右矩阵数据和对应的左右量化系数矩阵数据的搬运：矩阵数据以512B分形搬运至L0A/L0B Buffer，量化系数矩阵以32B分形搬运至L0A_MX/L0B_MX Buffer。
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
- 3510架构新增DN分型，并扩展了LoadDataWithTranspose接口能力，支持在搬运过程中完成分形转置；同时L1 Buffer->L0A Buffer不再支持transpose，改用`LoadData`接口的`ifTranspose`配置实现。

#### 功能说明

本特性涉及的API为[LoadDataWithTranspose（L1到L0转置数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_load/LoadDataWithTranspose.md)，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

- 3510架构新增较多的能力：新增DN分型；L1 Buffer->L0A Buffer通路不再支持transpose（对应场景请改用携带`LoadData2DParamsV2`的`LoadData`接口并设置`ifTranspose=true`）；新增`LoadData2dTransposeParamsV2`参数类型，具有更精细的搬运粒度（srcStride单位为单个512B分形，新增srcFracGap：迭代内源分形间隔），仅在Ascend 950PR/Ascend 950DT支持。
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
- 3510架构Fixpipe接口新增NZ2DN、NZ2ND随路转换能力，在L0C->GM/L1/UB的搬出过程中随路完成格式转换，避免额外的数据搬移。

#### 功能说明

本特性涉及的API为[Fixpipe（L0C到GM/L1/UB数据搬运）](../../../api/SIMD-API/basic_api/cube_compute_ISASI/cube_compute_store/Fixpipe_L0CToGM.md)及随路转换特性说明，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

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

除前述新增数据通路与随路转换能力外，3510架构还新增了若干矩阵计算（Cube计算）侧的基础API接口及其对应参数类型，用于扩展矩阵数据的搬入、矩阵乘加计算与辅助配置能力。上述接口均仅支持Ascend 950PR/Ascend 950DT产品（具体以各API文档"产品支持情况"为准）。

#### 功能说明

**表4** 矩阵计算基础API扩展说明<a name="table_cube_api_ext"></a>

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

3510架构AIV核采用Regbase架构后，矢量计算基础API扩展了若干新接口与新参数变体：支持标量为LocalTensor单点元素的flexible_scalar变体、按位/按边界的ISASI接口、交织/解交织、按张量移位的逻辑计算、UB内部搬运等。上述接口均仅支持Ascend 950PR/Ascend 950DT产品（具体以各API文档"产品支持情况"为准）。

#### 功能说明

**表5** 矢量计算基础API扩展说明<a name="table_vec_api_ext"></a>

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

  - flexible_scalar系列接口的标量参数支持LocalTensor单点元素，可在矢量计算中直接使用UB中的单点标量值，避免标量在Host/Device间搬运。
  - 3510架构在Memory矢量计算的高维切分接口中引入**stride参数**（前一块数据头到后一块数据头的间隔），替代2201架构的gap参数（前一个数据块结束地址与后一块起始地址的差值），从其它架构移植时需注意参数语义差异，详见[Memory矢量计算编程](../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/memory_vector_computation.md)。
  - 上述接口的详细原型、数据类型支持范围及约束请参考对应API文档。

### 原子操作接口<a name="section_atomic_api"></a>

#### 特性应用场景说明

在多线程/多核协同访问GM上同一数据的场景（如直方图统计、全局计数器累加等）中，需要对GM地址上的数据进行原子读改写操作，避免多核并发写导致的数据竞争。3510架构新增GM地址原子操作基础接口（头文件路径为`basic_api/kernel_operator_atomic_intf.h`，均仅支持Ascend 950PR/Ascend 950DT）。

#### 功能说明

**表8** 原子操作接口说明<a name="table_atomic_api"></a>

| API | 功能说明 |
|-----|---------|
| [AtomicAdd（原子加）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md) | 将GM地址上的旧值与输入标量求和并写回，返回修改前的值 |
| [AtomicMax（原子最大值）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicMax.md) | 将GM地址上的旧值与输入标量取最大值并写回，返回修改前的值 |
| [AtomicMin（原子最小值）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicMin.md) | 将GM地址上的旧值与输入标量取最小值并写回，返回修改前的值 |
| [AtomicExch（原子交换）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicExch.md) | 将GM地址上的旧值与输入标量交换，返回修改前的值 |
| [AtomicCas（原子比较交换）](../../../api/SIMD-API/basic_api/atomic_operations/AtomicCas.md) | 将GM地址上的旧值与期望值比较，相等则写入新值，返回修改前的值 |

### 系统变量与工具接口<a name="section_sys_api"></a>

#### 特性应用场景说明

3510架构新增系统变量访问、特殊寄存器访问、数值极限、标量计算与性能打点等工具类基础接口（均仅支持Ascend 950PR/Ascend 950DT），用于算子编程中的资源查询与辅助计算。

#### 功能说明

**表9** 系统变量与工具接口说明<a name="table_sys_api"></a>

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
| [CeilDivision（向上取整除法）](../../../api/SIMD-API/basic_api/scalar_compute/CeilDivision.md) | 计算两个整数相除后向上取整的结果 |
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

### 新增TSCM资源管理接口<a name="section_tscm"></a>

#### 特性应用场景说明

- TSCM（Temp Swap Cache Memory）是3510架构新增的存储位置（TPosition::TSCM），用于在Matmul等矩阵计算中临时将数据交换到额外空间。Vector核与Cube核之间通过队列（Queue）完成任务间通信和同步，当数据通路的目的地址为TSCM Position时，需要使用TSCM结构管理执行队列相关的操作与资源。
- 3510架构新增了TSCM队列的分配、释放与入队/出队管理接口（头文件路径为`basic_api/kernel_tpipe.h`，均仅支持Ascend 950PR/Ascend 950DT），TSCM与TQueBind、TQue属于相同类型结构。

#### 功能说明

**表55** TSCM资源管理接口说明<a name="table_tscm_api"></a>

| API | 功能说明 |
|-----|---------|
| [AllocTensor（TSCM分配）](../../../api/SIMD-API/basic_api/resource_management/TSCM/AllocTensor.md) | 从TSCM中分配tensor，tensor所占大小为InitBuffer时设置的每块内存长度（分配的tensor内容并非全0，可能为随机值） |
| [FreeTensor（TSCM释放）](../../../api/SIMD-API/basic_api/resource_management/TSCM/FreeTensor.md) | 释放从TSCM中分配的tensor |
| [EnQue（TSCM入队）](../../../api/SIMD-API/basic_api/resource_management/TSCM/EnQue.md) | 将tensor push到队列，用于流水线调度 |
| [DeQue（TSCM出队）](../../../api/SIMD-API/basic_api/resource_management/TSCM/DeQue.md) | 将tensor从队列中取出，用于后续处理 |

#### 使用说明

- 使用方式与TQue/TQueBind一致：先通过`InitBuffer`初始化队列缓冲区，再通过`AllocTensor`/`FreeTensor`分配/释放tensor，通过`EnQue`/`DeQue`完成入队/出队，队列深度建议设置为1（编译器对该场景做了特殊优化）。
- 详细说明请参考[TSCM简介](../../../api/SIMD-API/basic_api/resource_management/TSCM/TSCM_intro.md)及[TPosition说明](../../../api/SIMD-API/basic_api/aux_data_structures/TPosition.md)。

### 新增SSBuffer核内存储单元<a name="section_ssbuf"></a>

#### 特性应用场景说明

- 在Vector与Cube融合（Mix）算子中，AIC核与AIV核之间存在大量控制信息、标量数据的通信需求。在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等更早架构上，AIC与AIV之间通过GM（全局内存）进行通信，通信延迟高、占用GM带宽。
- 3510架构新增SSBuffer核内存储单元，支持AI Core内AIC核与AIV核通过Scalar直接访问的核间通信，无需经过GM，降低通信延迟、减轻GM带宽压力。

#### 功能说明

- 存储规格：[SSBuffer](../../programming_guide/advanced_programming/hardware_implementation/architecture_spec/npu_arch_3510.md)是3510架构AI Core内新增的核间通信存储单元，大小为3KB，是独立于L1 Buffer、UB的地址空间，对应地址空间限定符为`__ssbuf__`，详细说明请参考[SIMD BuiltIn关键字](../../programming_guide/language_extension/simd_builtin_keywords.md)。
- 访问方式：

  - 非MIX模式下，AIC、AIV0、AIV1各自独立占用1KB空间；MIX模式下（AIC:AIV=1:2）共享整个3KB空间。
  - 通过[GetSsbufBaseAddr](../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSsbufBaseAddr.md)接口获取SSBuffer基地址（返回`__ssbuf__`指针），基于基地址偏移进行读写访问。
  - 仅支持32字节对齐的读写指令访问，访问超过SSBuffer末端地址时会触发异常。

- 使用约束：

  - SSBuffer初始存在脏数据，读取内容不保证全0。
  - AIC和AIV启动不同任务时不能访问SSBuffer；当前任务未开启SSBuffer能力时不能访问。

#### 使能说明

SSBuffer能力与[UB->L1搬运数据通路](#section_ub2l1)共用编译选项[`ENABLE_CV_COMM_VIA_SSBUF`](#section_enable_option)，配置`-DENABLE_CV_COMM_VIA_SSBUF=true`后使能。使用Matmul高阶API的SetTensorScaleA等接口（内部实现使用SSBuffer）时，也需要使能该编译选项。

### 新增Mutex核内同步能力<a name="section_mutex"></a>

#### 特性应用场景说明

- 在Vector算子中，异步流水指令（如GM->UB搬运PIPE_MTE2、向量计算PIPE_V、UB->GM搬运PIPE_MTE3）之间通常通过SetFlag/WaitFlag事件同步，需要为其显式配对事件，编码繁琐、事件资源受限。
- 3510架构新增Mutex能力，通过锁定指定流水再释放流水来完成流水间的同步依赖，与SetFlag/WaitFlag相比内聚性更强、与其它流水线解耦，可简化反向同步，且可使用更多的同步信号量。

#### 功能说明

本特性涉及的API为[Mutex（核内同步）](../../../api/SIMD-API/basic_api/sync_control/intra_core_sync/Mutex_ISASI.md)，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

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
  - 具体使用流程为：申请MutexID -> `Mutex::Lock<pipe>(id)` -> 发起该流水上的指令（片元） -> `Mutex::Unlock<pipe>(id)` -> 释放MutexID。
  - Lock/Unlock对同一MutexID必须严格成对使用且pipe必须相同，否则硬件行为不可预测；相同ID相同流水的两组Lock/Unlock第二次不会阻塞流水。

#### 典型样例说明

样例位置：[mutex样例](../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/mutex)。样例功能：使用Mutex实现MTE2、V流水与MTE3流水之间的同步，在无SetFlag/WaitFlag的情况下正确完成数据搬运与计算。

### 新增核间同步控制模式<a name="section_crosscore"></a>

#### 特性应用场景说明

- 在Vector与Cube融合（Mix）算子中，AIV核（Vector）与AIC核（Cube）之间存在数据依赖，需要通过核间同步接口（CrossCoreSetFlag/CrossCoreWaitFlag）保证数据就绪后再发起后续计算。
- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等更早架构上，AIC核等待同步时，AIV0与AIV1必须同时上报（模式2：AIC与所有AIV同步），同步粒度较粗。
- 3510架构新增核间同步控制模式（模式4：AIC与单个AIV同步），AIV0与AIV1可单独触发AIC等待，同步粒度更细，降低冗余等待，改善执行效率。

#### 功能说明

本特性涉及的API为[CrossCoreSetFlag（核间同步设置标志）](../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreSetFlag_ISASI.md)和[CrossCoreWaitFlag（核间同步等待标志）](../../../api/SIMD-API/basic_api/sync_control/inter_core_sync/CrossCoreWaitFlag_ISASI.md)，核函数内通过`kernel_operator.h`间接包含使用。功能说明如下：

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

- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等更早架构上，AIV核采用Membase架构，矢量计算结果必须写回Unified Buffer（UB）后才能被后续指令使用，中间结果反复读写UB，增加了额外的搬运开销。
- 3510架构AIV核采用Regbase架构，支持直接对芯片的Vector寄存器Register进行操作，中间结果可暂存寄存器，减少UB读写，兼顾灵活性与执行效率。

#### 功能说明

- 两种矢量计算方式的差异如下：

  **表52** Membase与Regbase架构差异<a name="table_membase_regbase"></a>

  | 计算方式 | 数据暂存位置 | 特点 | 适用场景 |
  |---------|------------|------|---------|
  | Membase | Local Memory（UB） | 每步计算结果写回UB | NPU架构版本2201 |
  | Regbase | 寄存器（VF Reg） | 中间结果可暂存寄存器，减少UB读写 | NPU架构版本3510 |

- 编程方式说明（详细请参考[Reg矢量计算](../../../api/SIMD-API/basic_api/reg_vector_compute/overview.md)相关文档）：

  - C++ Tensor方式：使用`Reg::RegTensor<T>`定义基于寄存器的Tensor（位宽为VL），使用`Reg::MaskReg`定义掩码寄存器，通过`asc_vf_call`调用`__simd_vf__`函数，典型流程为LoadAlign（UB->寄存器）-> Reg计算（如Add）-> StoreAlign（寄存器->UB），使用`UpdateMask`更新掩码并配合HardwareLoop实现循环计算。
  - C指针方式（寄存器C API）：通过`__ubuf__`等地址空间限定符定义寄存器变量，使用`asc_load/asc_store`、`asc_add`等寄存器操作接口，直接以寄存器为操作数进行矢量计算。
  - 3510架构Reg资源规格（寄存器数量有限，编程需注意资源分配）：矢量数据寄存器256B×32、掩码寄存器32B×8、地址寄存器4B×8、非对齐搬入/搬出寄存器32B×4，详细说明请参考[NPU架构规格](../../programming_guide/advanced_programming/hardware_implementation/architecture_spec/npu_arch_3510.md)。
  - 3510架构Reg矢量计算接口支持**指令双发**能力（1个Cycle可执行两条指令，仅针对Ascend 950PR/Ascend 950DT生效），各接口的单双发情况请参考[Reg矢量计算接口指令单双发汇总](../../../api/appendix/reg_vector_compute_interface_instruction_single_dual_issue_summary.md)；编程时合理利用双发能力（如寄存器搬入LoadAlign、Move等）可进一步提升性能。
  - **VF循环优化**：VF（Vector Function）函数中的循环在满足[Hardware Loop编码规范](../../operator_practice/simd_operator_optimization/vector_compute/vf_optimization/vf_loop_optimization.md)时可被编译器优化为Hardware Loop（硬件级向量循环），提升执行性能；同时3510架构引入[地址寄存器（AddrReg）](../../../api/SIMD-API/basic_api/reg_vector_compute/register_data_types/AddrReg.md)优化地址偏移量计算，满足特定地址寄存器生成模式时可消除Scalar计算消耗，VF循环优化指导请参考[VF循环优化](../../operator_practice/simd_operator_optimization/vector_compute/vf_optimization/vf_loop_optimization.md)（950专属实践）。
  - **配套参考汇总**：3510架构Reg矢量计算配套提供以下950独占的参考汇总文档：[Reg矢量计算接口精度标准汇总](../../../api/appendix/reg_vector_compute_interface_precision_standard_summary.md)（各接口的硬件指令精度标准与软仿精度标准）、[Reg矢量计算接口软仿情况汇总](../../../api/appendix/reg_vector_compute_interface_soft_simulation_summary.md)（各接口软仿支持的数据类型）、[C-API Reg矢量计算接口理论性能汇总](../../../api/appendix/c_api_reg_vector_compute_theoretical_perf_summary.md)（寄存器C API各接口的单指令理论性能）。

Reg矢量计算相关接口清单如下（均仅支持Ascend 950PR/Ascend 950DT产品）：

**表33** Reg矢量计算-寄存器数据类型接口

| 接口 | 功能说明 |
|-----|---------|
| [AddrReg](../../../api/SIMD-API/basic_api/reg_vector_compute/register_data_types/AddrReg.md) | AddrReg是用于Reg矢量计算接口管理地址偏移量的专用寄存器。 |
| [MaskReg](../../../api/SIMD-API/basic_api/reg_vector_compute/register_data_types/MaskReg.md) | MaskReg寄存器用于指示在计算过程中哪些元素参与计算，宽度为<a href="./RegTensor.md">RegTensor</a>的八分之一（VL/8）。 |
| [RegTensor](../../../api/SIMD-API/basic_api/reg_vector_compute/register_data_types/RegTensor.md) | RegTensor寄存器是矢量计算基本单元，RegTensor位宽为VL（Vector Length），长度为256B。 |
| [UnalignRegForLoad-UnalignRegForStore](../../../api/SIMD-API/basic_api/reg_vector_compute/register_data_types/UnalignRegForLoad-UnalignRegForStore.md) | 为提升对不规则内存地址的处理能力，Reg矢量计算支持在数据搬运过程中对非32字节对齐的地址进行访问，适用于从Unified Buffer（UB）向RegTensor非对齐搬运，或从RegTensor向UB非对齐搬运的场景。 |

**表34** Reg矢量计算-寄存器搬入接口

| 接口 | 功能说明 |
|-----|---------|
| [GatherB_discrete](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/GatherB_discrete.md) | 该指令会根据索引值index将源操作数按DataBlock（32B）收集到目的操作数dstReg中。 |
| [Gather_discrete](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/Gather_discrete.md) | 该指令会根据索引值index将源操作数按元素收集到目的操作数dstReg中。 |
| [Load](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/Load.md) | Reg矢量计算数据搬运接口，支持从Unified Buffer（UB）非32字节对齐的源地址srcAddr搬运至RegTensor，搬运量为VL（256B）。 |
| [LoadAlign_MaskReg](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/LoadAlign_MaskReg.md) | LoadAlign能够实现数据从Unified Buffer（UB）搬运至MaskReg。 |
| [LoadAlign_continuous](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/LoadAlign_continuous.md) | LoadAlign能够实现数据从Unified Buffer（UB）连续搬运至RegTensor。 |
| [LoadAlign_noncontinuous](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/LoadAlign_noncontinuous.md) | LoadAlign能够实现数据从Unified Buffer（UB）非连续搬运至RegTensor，有两类接口： 普通搬运接口：完成一次搬运后，UB地址不会自动更新，每次迭代需要手动更新地址。 |
| [LoadUnAlign_continuous](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/LoadUnAlign_continuous.md) | 为提升对不规则内存地址的处理能力，Reg矢量计算支持在数据搬运过程中对非32字节对齐的地址进行访问，降低非对齐访问带来的性能开销。 |
| [MaskGenWithRegTensor_MaskReg](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/MaskGenWithRegTensor_MaskReg.md) | MaskGenWithRegTensor能够实现数据从RegTensor搬运至MaskReg。 |
| [Move](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_load/Move.md) | 对srcReg中的有效元素逐个复制写入dstReg中对应位置处，无效位置保留dstReg原值。 |

**表35** Reg矢量计算-寄存器搬出接口

| 接口 | 功能说明 |
|-----|---------|
| [Scatter_discrete](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_store/Scatter_discrete.md) | 该指令会根据索引值index将源操作数srcReg中的元素分散到目的操作数Unified Buffer（UB）中。 |
| [Store](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_store/Store.md) | Reg矢量计算数据搬运接口，支持从RegTensor搬出至非32字节对齐的Unified Buffer（UB）地址dstAddr。 |
| [StoreAlign_MaskReg](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_store/StoreAlign_MaskReg.md) | StoreAlign能够实现数据从MaskReg搬运至Unified Buffer（UB）。 |
| [StoreAlign_continuous](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_store/StoreAlign_continuous.md) | StoreAlign能够实现数据从RegTensor连续搬运至Unified Buffer（UB），支持单搬出模式和双搬出模式： 单搬出模式：将一个RegTensor（VL）中的VL数据量搬出至UB。 |
| [StoreAlign_noncontinuous](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_store/StoreAlign_noncontinuous.md) | StoreAlign能够实现数据从RegTensor非连续搬运至Unified Buffer（UB），有两类接口： 普通搬运接口：完成一次搬运后，UB地址不会自动更新，每次迭代需要手动更新地址。 |
| [StoreUnAlign_MaskReg](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_store/StoreUnAlign_MaskReg.md) | StoreUnAlign能够实现数据从MaskReg连续搬运至非对齐Unified Buffer（UB），利用非对齐寄存器UnalignRegForStore作为临时缓存区，暂存跨对齐边界的数据，从而实现高效的连续非对齐数据传输。 |
| [StoreUnAlign_continuous](../../../api/SIMD-API/basic_api/reg_vector_compute/reg_data_store/StoreUnAlign_continuous.md) | 为提升对不规则内存地址的处理能力，Reg矢量计算支持在数据搬运过程中对非32字节对齐的地址进行访问，降低非对齐访问带来的性能开销。 |

**表36** Reg矢量计算-散播与收集接口

| 接口 | 功能说明 |
|-----|---------|
| [Gather](../../../api/SIMD-API/basic_api/reg_vector_compute/scatter_operations/Gather.md) | 该指令会根据索引值indexReg将源操作数srcReg按元素收集到目的操作数dstReg中。 |

**表37** Reg矢量计算-算术计算接口

| 接口 | 功能说明 |
|-----|---------|
| [Abs](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Abs.md) | 对实数类型 对srcReg中的有效元素逐个取绝对值，并将结果写入dstReg对应位置，计算公式如下： dstReg_i = |srcReg_i| 对复数类型 对srcReg中有效元素逐个取模，并将结果写入dstReg对应位置，计算公式如下： dstReg_i  |
| [Add](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Add.md) | 该接口根据mask，对源操作数srcReg0、srcReg1进行按元素求和操作，将结果写入目的操作数dstReg。 |
| [AddC](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/AddC.md) | 该接口根据mask，对源操作数srcReg0、srcReg1及输入进位carrySrc进行按元素求和操作，将结果写入目的操作数dstReg，同时将每个元素的进位结果写入carry（存放进位的MaskReg寄存器）。 |
| [Adds](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Adds.md) | 该接口根据mask，对源操作数srcReg按元素与标量scalarValue进行求和操作，将结果写入目的操作数dstReg。 |
| [Div](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Div.md) | 该接口根据mask，对源操作数srcReg0、srcReg1进行按元素相除操作，将结果写入目的操作数dstReg。 |
| [Exp](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Exp.md) | 该接口根据mask，对源操作数srcReg进行按元素指数操作，将结果写入目的操作数dstReg。 |
| [LeakyRelu](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/LeakyRelu.md) | 该接口用于按元素执行Leaky ReLU（Leaky Rectified Linear Unit）操作：当源操作数中某元素大于0时，直接将该元素写入目的操作数；否则将该元素乘以标量值scalarValue后写入目的操作数。 |
| [Ln](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Ln.md) | 该接口根据mask逐元素对源操作数求自然对数，将结果写入目的操作数。 |
| [Log](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Log.md) | 该接口根据mask逐元素对源操作数求自然对数（以e为底），将结果写入目的操作数。 |
| [Log10](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Log10.md) | 该接口根据mask逐元素对源操作数求以10为底的对数，将结果写入目的操作数。 |
| [Log2](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Log2.md) | 该接口用于根据mask对输入数据srcReg进行求以2为底的对数操作，将结果写入dstReg。 |
| [Max](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Max.md) | 该接口根据mask，对源操作数srcReg0、srcReg1进行按元素求最大值操作，将结果写入目的操作数dstReg。 |
| [Maxs](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Maxs.md) | 该接口根据mask，对源操作数srcReg按元素与标量相比，如果比标量大，则取源操作数值；如果比标量小，则取标量值，将结果写入目的操作数dstReg。 |
| [Min](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Min.md) | 该接口根据mask，对源操作数srcReg0、srcReg1进行按元素求最小值操作，将结果写入目的操作数dstReg。 |
| [Mins](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Mins.md) | 该接口根据mask，对源操作数srcReg按元素与标量相比，如果比标量大，则取标量值，比标量小，则取源操作数值，将结果写入目的操作数dstReg。 |
| [Mul](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Mul.md) | 该接口根据mask对输入数据srcReg0、srcReg1按元素相乘操作，将结果写入dstReg。 |
| [Mull](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Mull.md) | 该接口根据mask对输入数据srcReg0、srcReg1按元素相乘操作，将乘法结果的低位部分写入dstReg0，溢出（高位）部分写入dstReg1。 |
| [Muls](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Muls.md) | 矢量内每个元素与标量求积，计算公式如下： dstReg_i = srcReg_i \times scalarValue |
| [Neg](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Neg.md) | 该接口用于根据mask对输入数据srcReg进行取相反数操作，将结果写入dstReg。 |
| [Prelu](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Prelu.md) | 该接口用于实现PReLU（Parametric ReLU）激活函数：当源操作数srcReg0中对应元素大于0时，直接将该元素写入目的操作数dstReg； |
| [Relu](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Relu.md) | 该接口根据mask对输入数据srcReg进行Relu（线性整流）操作，将结果写入dstReg，计算公式如下： dstReg_i = \max(0, srcReg_i) |
| [Sqrt](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Sqrt.md) | 该接口根据mask，对源操作数srcReg逐元素做平方根运算，将结果写入目的操作数dstReg。 |
| [Sub](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/Sub.md) | 该接口根据mask，对源操作数srcReg0、srcReg1进行按元素求差操作，将结果写入目的操作数dstReg。 |
| [SubC](../../../api/SIMD-API/basic_api/reg_vector_compute/basic_arithmetic/SubC.md) | 该接口根据mask，对源操作数srcReg0、srcReg1及输入进位carrySrc进行按元素求差操作，减法运算在硬件底层通过补码加法实现，将结果写入目的操作数dstReg，同时将每个元素的进位结果写入carry（存放进位的MaskReg寄存器）。 |

**表38** Reg矢量计算-比较与选择接口

| 接口 | 功能说明 |
|-----|---------|
| [Compare](../../../api/SIMD-API/basic_api/reg_vector_compute/compare_and_select/Compare.md) | 逐元素比较两个RegTensor大小，根据模板参数指定的比较模式，将比较结果写入目的操作数MaskReg中对应比特位，如果比较后的结果为真，则输出结果的对应比特位为1，否则为0。 |
| [Compares](../../../api/SIMD-API/basic_api/reg_vector_compute/compare_and_select/Compares.md) | 该接口用于逐元素比较源操作数中每个元素与一个标量的大小关系，根据模板参数指定的比较模式（LT/GT/GE/EQ/NE/LE）判断结果是否为真，若为真则输出MaskReg对应比特位置1，否则置0。 |
| [Select](../../../api/SIMD-API/basic_api/reg_vector_compute/compare_and_select/Select.md) | 给定两个源操作数srcReg0和srcReg1，根据mask的比特位值选取元素，得到目的操作数dstReg。 |
| [Squeeze](../../../api/SIMD-API/basic_api/reg_vector_compute/compare_and_select/Squeeze.md) | 将传入的srcReg中被mask选择的有效元素依次复制到dstReg中，有效元素在dstReg中从低到高连续排列，剩余位置元素置为0，如图1所示。 |

**表39** Reg矢量计算-复合计算接口

| 接口 | 功能说明 |
|-----|---------|
| [AbsSub](../../../api/SIMD-API/basic_api/reg_vector_compute/composite_compute/AbsSub.md) | 该接口根据mask将srcReg0与srcReg1逐元素相减再求绝对值，计算结果写入dstReg。 |
| [Axpy](../../../api/SIMD-API/basic_api/reg_vector_compute/composite_compute/Axpy.md) | 该接口根据mask对dstReg、srcReg和标量scalarValue按元素执行乘加操作，将结果写入dstReg。 |
| [ExpSub](../../../api/SIMD-API/basic_api/reg_vector_compute/composite_compute/ExpSub.md) | srcReg0与srcReg1相减，差值作为e的指数计算，根据mask将计算结果写入dstReg。 |
| [MulAddDst](../../../api/SIMD-API/basic_api/reg_vector_compute/composite_compute/MulAddDst.md) | 该接口按元素将srcReg0与srcReg1相乘再加上dstReg，根据mask将计算结果写入dstReg。 |
| [MulDstAdd](../../../api/SIMD-API/basic_api/reg_vector_compute/composite_compute/MulDstAdd.md) | 该接口用于完成dstReg与srcReg0逐元素相乘再加srcReg1的融合乘加运算，根据mask将计算结果写回dstReg。 |
| [MulsCast](../../../api/SIMD-API/basic_api/reg_vector_compute/composite_compute/MulsCast.md) | 该接口用于将源操作数src与标量scalar相乘，再按照CAST\_ROUND模式将结果转换为half类型，根据mask将计算结果写入目的操作数dst。 |

**表40** Reg矢量计算-逻辑计算接口

| 接口 | 功能说明 |
|-----|---------|
| [And](../../../api/SIMD-API/basic_api/reg_vector_compute/logical_compute/And.md) | （对RegTensor操作）、"basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"（对MaskReg操作）。 |
| [Not](../../../api/SIMD-API/basic_api/reg_vector_compute/logical_compute/Not.md) | （对RegTensor操作）、"basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"（对MaskReg操作）。 |
| [Or](../../../api/SIMD-API/basic_api/reg_vector_compute/logical_compute/Or.md) | （对RegTensor操作）、"basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"（对MaskReg操作）。 |
| [ShiftLeft](../../../api/SIMD-API/basic_api/reg_vector_compute/logical_compute/ShiftLeft.md) | ShiftLeft指令根据mask对源操作数srcReg0，按照srcReg1对应元素进行左移操作，将结果写入目的操作数dstReg。 |
| [ShiftLefts](../../../api/SIMD-API/basic_api/reg_vector_compute/logical_compute/ShiftLefts.md) | ShiftLefts指令根据mask对源操作数srcReg，按照scalarValue进行左移操作，将结果写入目的操作数dstReg。 |
| [ShiftRight](../../../api/SIMD-API/basic_api/reg_vector_compute/logical_compute/ShiftRight.md) | ShiftRight指令根据mask对源操作数srcReg0，按照srcReg1对应元素进行右移操作，将结果写入目的操作数dstReg。 |
| [ShiftRights](../../../api/SIMD-API/basic_api/reg_vector_compute/logical_compute/ShiftRights.md) | ShiftRights指令根据mask对源操作数srcReg，按照scalarValue进行右移操作，将结果写入目的操作数dstReg。 |
| [Xor](../../../api/SIMD-API/basic_api/reg_vector_compute/logical_compute/Xor.md) | （对RegTensor操作）、"basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"（对MaskReg操作）。 |

**表41** Reg矢量计算-归约计算接口

| 接口 | 功能说明 |
|-----|---------|
| [PairReduceElem](../../../api/SIMD-API/basic_api/reg_vector_compute/reduction_compute/PairReduceElem.md) | 将传入的srcReg中相邻两个数值相加，并将产生的结果保存在dstReg中的低位位置。 |
| [Reduce](../../../api/SIMD-API/basic_api/reg_vector_compute/reduction_compute/Reduce.md) | 归约指令根据ReduceType，将数据集合简化为单一值或者更小的集合。 |
| [ReduceDataBlock](../../../api/SIMD-API/basic_api/reg_vector_compute/reduction_compute/ReduceDataBlock.md) | 归约指令根据ReduceType，将数据集合在每个DataBlock（32B）内简化为单一值。 |

**表42** Reg矢量计算-数据压缩接口

| 接口 | 功能说明 |
|-----|---------|
| [Pack](../../../api/SIMD-API/basic_api/reg_vector_compute/data_compression/Pack.md) | 将源操作数srcReg中的元素选取低8位（对于b16类型）、低16位（对于b32类型）、低32位（对于b64类型）写入dstReg的低半部分或高半部分。 |
| [UnPack](../../../api/SIMD-API/basic_api/reg_vector_compute/data_compression/UnPack.md) | 对于无符号整型，将源操作数srcReg中低半部分或高半部分的元素以高位填0扩充位宽的方式写入dstReg。 |
| [Unsqueeze](../../../api/SIMD-API/basic_api/reg_vector_compute/data_compression/Unsqueeze.md) | 将dstReg中数据根据mask进行解压缩。 |

**表43** Reg矢量计算-数据填充接口

| 接口 | 功能说明 |
|-----|---------|
| [Duplicate](../../../api/SIMD-API/basic_api/reg_vector_compute/data_padding/Duplicate.md) | 支持Scalar和Tensor两种模式： Scalar模式：将scalarValue广播到寄存器，并保存在dstReg中（如果有mask，则保存在dstReg中被mask筛选的位置）。 |

**表44** Reg矢量计算-数据重排接口

| 接口 | 功能说明 |
|-----|---------|
| [DeInterleave](../../../api/SIMD-API/basic_api/reg_vector_compute/data_reorder/DeInterleave.md) | 给定源操作数寄存器srcReg0和srcReg1，将srcReg0和srcReg1中的元素解交织存入结果操作数dstReg0和dstReg1中。 |
| [Interleave](../../../api/SIMD-API/basic_api/reg_vector_compute/data_reorder/Interleave.md) | 给定源操作数寄存器srcReg0和srcReg1，将srcReg0和srcReg1中的元素交织存入结果操作数dstReg0和dstReg1中。 |

**表45** Reg矢量计算-直方图计算接口

| 接口 | 功能说明 |
|-----|---------|
| [Histograms](../../../api/SIMD-API/basic_api/reg_vector_compute/histogram_compute/Histograms.md) | 对直方图数据进行统计，在目的操作数dstReg的基础数据上加上源操作数srcReg数据的统计结果，包括数据的频率统计和累计统计。 |

**表46** Reg矢量计算-索引操作接口

| 接口 | 功能说明 |
|-----|---------|
| [Arange](../../../api/SIMD-API/basic_api/reg_vector_compute/index_operations/Arange.md) | 递增模式时，该函数以传入的scalarValue的值为起始值，生成递增的索引；递减模式时，该函数以传入的scalarValue的值为终止值，生成递减的索引，并将索引保存在dstReg中。 |

**表47** Reg矢量计算-掩码寄存器计算接口

| 接口 | 功能说明 |
|-----|---------|
| [DeInterleave](../../../api/SIMD-API/basic_api/reg_vector_compute/MaskReg_compute/DeInterleave.md) | 将源操作数src0和src1中的元素解交织存入目的操作数dst0和dst1中。 |
| [Interleave](../../../api/SIMD-API/basic_api/reg_vector_compute/MaskReg_compute/Interleave.md) | 将源操作数src0和src1中的元素交织存入目的操作数dst0和dst1中。 |
| [Move](../../../api/SIMD-API/basic_api/reg_vector_compute/MaskReg_compute/Move.md) | 将src中的元素复制到dst中的对应位置。 |
| [MoveMask](../../../api/SIMD-API/basic_api/reg_vector_compute/MaskReg_compute/MoveMask.md) | 从SetVectorMask设置的掩码寄存器{MASK1, MASK0}中读取Mask值，并按模板参数T对应的数据格式转换后写入返回值MaskReg。 |
| [Pack](../../../api/SIMD-API/basic_api/reg_vector_compute/MaskReg_compute/Pack.md) | 根据所选的低位模式或高位模式，将源操作数src的偶数位bit，提取到目的操作数dst的低半部分或高半部分。 |
| [Select](../../../api/SIMD-API/basic_api/reg_vector_compute/MaskReg_compute/Select.md) | 给定两个源操作数src0和src1，根据mask的比特位值选取元素，得到目的操作数dst。 |
| [UnPack](../../../api/SIMD-API/basic_api/reg_vector_compute/MaskReg_compute/UnPack.md) | 根据所选的低位模式或高位模式，将源操作数src的低半部分或高半部分，展开到目的操作数dst。 |

**表48** Reg矢量计算-类型转换接口

| 接口 | 功能说明 |
|-----|---------|
| [Cast](../../../api/SIMD-API/basic_api/reg_vector_compute/type_conversion/Cast.md) | Cast用于数据类型精度转换，将源操作数数据类型转换成目的操作数数据类型，能够实现浮点转整数、浮点转浮点、整数转浮点、整数转整数的数据类型转换。 |
| [Truncate](../../../api/SIMD-API/basic_api/reg_vector_compute/type_conversion/Truncate.md) | 将源操作数中的浮点数元素截断为整数值（保留原数据类型），并存入目的操作数。 |

**表49** Reg矢量计算-同步控制接口

| 接口 | 功能说明 |
|-----|---------|
| [LocalMemBar](../../../api/SIMD-API/basic_api/reg_vector_compute/sync_control/LocalMemBar.md) | Reg矢量计算宏函数内不同流水线之间的同步指令。 |

**表50** Reg矢量计算-系统变量访问接口

| 接口 | 功能说明 |
|-----|---------|
| [ClearSpr](../../../api/SIMD-API/basic_api/reg_vector_compute/system_variable_access/ClearSpr.md) | 该接口用于对指定的特殊目的寄存器（SpecialPurposeReg）进行清零操作。 |

**表51** Reg矢量计算-VF调用接口

| 接口 | 功能说明 |
|-----|---------|
| [asc_vf_call](../../../api/SIMD-API/basic_api/reg_vector_compute/vf_call/asc_vf_call.md) | 在SIMD编程场景下使用。 |

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

Reg矢量计算高性能算子样例位于`examples/01_simd_cpp_api/05_best_practices/02_reg_compute`目录下，包括[gelu_high_performance样例](../../../../../examples/01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_high_performance)（基于Reg矢量计算的GELU激活函数高性能力示例）、[gelu_eltwise_high_performance样例](../../../../../examples/01_simd_cpp_api/05_best_practices/02_reg_compute/gelu_eltwise_high_performance)、[softmax_high_performance样例](../../../../../examples/01_simd_cpp_api/05_best_practices/02_reg_compute/softmax_high_performance)（基于Reg矢量计算的Softmax高性能力示例）。

### 新增SIMT编程模型<a name="section_simt"></a>

#### 特性应用场景说明

- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等更早架构上，AI Core仅支持SIMD（单指令多数据）编程模型，对复杂控制流、离散数据访问等不规则场景的编程灵活性不足。
- 3510架构新增SIMT（单指令多线程）编程模型，采用以SIMD为主、SIMT为辅的编程范式：向量计算的一部分算力搭配SIMT能力，针对复杂控制流、离散访存等不规则场景提供线程级并行的编程灵活性；同时支持SIMD与SIMT混合编程。

#### 功能说明

- SIMT编程模型以线程为基本执行单元，一条指令同时驱动多个独立线程，每个线程处理一个数据元素，线程可根据代码分支（如if-else）独立执行。当前仅Ascend 950PR/Ascend 950DT支持，详细说明请参考[编程模型](../../programming_guide/programming_model/programming_model_overview.md)。
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

- SIMT编程与SIMD/SIMT混合编程的算子实现与性能优化指导（[SIMT算子实现](../../operator_practice/simt_operator_impl/simt_operator_impl.md)、[SIMD与SIMT混合编程算子实现](../../operator_practice/simd_simt_hybrid_operator_impl/simd_simt_hybrid_operator_impl.md)、[SIMD与SIMT混合编程算子优化](../../operator_practice/simd_simt_hybrid_optimization/simd_simt_hybrid_optimization.md)）覆盖了3510架构下SIMT线程配置、UB内存访问、分支判断、快速整数除法等实践场景，仅供参考。
- [SIMT算子优化](../../operator_practice/simt_operator_optimization/simt_operator_optimization.md)提供了950专属的SIMT性能优化指导，包括[原子指令优化](../../operator_practice/simt_operator_optimization/instruction_optimization/atomic_instruction_optimization.md)、[通过类型对齐提升搬运效率](../../operator_practice/simt_operator_optimization/memory_access/type_alignment_transfer.md)（950支持1B/2B/4B/8B四种位宽访存指令，通过`alignas`类型对齐到更大位宽可提升GM搬运效率）、[SIMT UB Bank冲突避免](../../operator_practice/simt_operator_optimization/memory_access/avoid_ub_bank_conflict_simt.md)等。
- SIMD与SIMT混合编程相关样例位于[examples/05_simd_simt_hybrid目录](../../../../../examples/05_simd_simt_hybrid)，包括[simd_simt_gather_and_adds样例](../../../../../examples/05_simd_simt_hybrid/00_introduction/simd_simt_gather_and_adds)（入门样例）、[simd_simt_high_performance样例](../../../../../examples/05_simd_simt_hybrid/02_best_practices/simd_simt_high_performance)（混合编程高性能力示例）、[simd_simt_grid_dim_config样例](../../../../../examples/05_simd_simt_hybrid/02_best_practices/simd_simt_grid_dim_config)（SIMT线程网格配置）等。

### Matmul高阶API新增数据类型支持<a name="section_matmul_dtype"></a>

#### 特性应用场景说明

- 3510架构扩展了矩阵计算的算力能力，Matmul类高阶API新增支持fp8、hifloat8、float、bfloat16_t、half、int8_t等多种数据类型的矩阵乘加计算，新增类型较2201架构的half/int8_t等类型范围更广，可覆盖低比特量化、纯浮点高精度等更多业务场景。
- 2201架构支持的int4b_t数据类型在3510架构的Cube计算单元不再支持，需先通过Cast转换为int8_t后再进行矩阵计算，具体请参考[矩阵计算兼容性说明](../3510_arch_migration/2201_to_3510_guide/basic_api_migration.md#section17560113713122)，兼容样例请参考[matmul_s4样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/matmul_s4)。

#### 功能说明

Matmul类高阶API新增支持的数据类型组合（A矩阵×B矩阵，Bias矩阵，C矩阵）如下：

**表53** Matmul高阶API新增数据类型说明<a name="table_matmul_dtype"></a>

| A矩阵 | B矩阵 | Bias矩阵 | C矩阵 | 说明 |
|-------|-------|---------|-------|------|
| fp8_e4m3fn_t、fp8_e5m2_t | fp8_e4m3fn_t、fp8_e5m2_t | float、half、bfloat16_t | fp8_e4m3fn_t、half、bfloat16_t、float | 3510架构新增 |
| hifloat8_t | hifloat8_t | float、half、bfloat16_t | hifloat8_t、half、bfloat16_t、float | 3510架构新增 |
| float | float | bfloat16_t | float、half、bfloat16_t | 3510架构新增 |
| bfloat16_t | bfloat16_t | bfloat16_t | float、half、bfloat16_t | 3510架构新增 |
| half | half | bfloat16_t | float、half、bfloat16_t | 3510架构新增 |
| int8_t | int8_t | int32_t | bfloat16_t | 3510架构新增 |

- 使用要点：

  - 上述新增类型组合的详细参数配置（tiling、workspace等）请参考[Matmul高阶API文档](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/Matmul_usage.md)及各数据类型组合对应样例。
  - Matmul高阶API在3510架构还支持MX（MicroScaling）矩阵乘场景：通过[SetTensorScaleA](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorScaleA.md)、[SetTensorScaleB](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetTensorScaleB.md)接口（Ascend 950PR/Ascend 950DT新增，内部实现使用SSBuffer，需使能[`ENABLE_CV_COMM_VIA_SSBUF`](#section_enable_option)）设置左右矩阵的量化系数矩阵，配合[LoadData MX搬运](#section_mx)实现MX格式的小数低比特矩阵乘。样例请参考[mmad_mx样例](../../../../../examples/01_simd_cpp_api/03_basic_api/03_matrix_compute/mmad_mx)。
  - 3510架构新增数据类型fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t/fp4x2_e1m2_t/fp4x2_e2m1_t等内置数据类型本身，请参考[数据类型介绍](../../../api/SIMD-API/basic_api/data_structures/builtin_data_types.md)；量化/反量化/伪量化类高阶API（AscendQuant/AscendDequant/AscendAntiQuant）也扩展支持了上述类型，详情请参考[高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md)。
  - **Matmul输出量化/反量化场景**：3510架构新增支持的Matmul输出随路量化/反量化数据类型组合包括：int8_t×int8_t→bfloat16_t、fp8_e4m3fn_t/fp8_e5m2_t→fp8_e4m3fn_t/half/bfloat16_t/float、hifloat8_t→hifloat8_t/half/bfloat16_t/float（均仅Ascend 950PR/Ascend 950DT支持）；通过[SetDequantType](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Tiling/SetDequantType.md)（Tiling侧）、[SetQuantScalar](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetQuantScalar.md)/[SetQuantVector](../../../api/SIMD-API/adv_api/cube_compute/Matmul_Kernel/SetQuantVector.md)（核函数侧）配置量化/反量化模式，详细说明请参考[矩阵乘输出的量化/反量化](../../operator_practice/simd_operator_impl/matrix_advanced_api/feature_scenarios/matmul_output_quant_dequant.md)。
  - **Mix融合算子Scenario 2（L0C直接写UB）**：在CV融合算子的高阶极限打磨阶段，3510架构支持Scenario 2方式（仅Ascend 950PR/Ascend 950DT支持）——AIC侧通过Fixpipe（携带`FixpipeParamsArch3510`参数）将L0C计算结果直接写入UB，AIV侧无需从GM读取数据，直接在UB上进行GELU/LeakyReLU等计算后写回GM，省去GM中转的读写开销。该场景是[L0C->UB搬运数据通路](#section_fixpipe_l0c2ub)在融合算子中的典型应用，详细说明请参考[matmul_leakyrelu_fusion_operator极限优化](../../operator_practice/typical_operator_cases/simd_operator_impl/matmul_leakyrelu_fusion_operator/extreme_optimization.md)。

### 高阶API扩展能力总览<a name="section_adv_api_ext"></a>

#### 特性应用场景说明

- 除Matmul类高阶API外，3510架构在数学计算、数据归一化、量化操作、归约操作、排序操作、索引计算、数据过滤、张量变换等类高阶API上也扩展支持了新的数据类型、算法配置与排布转换场景。
- 这些能力基于3510架构硬件算力扩展（如fp8/hifloat8等新数据类型支持、AIV核Regbase架构等），使能后可用于对应业务场景。详细兼容性说明请参考[高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md)。

#### 功能说明

3510架构高阶API扩展能力总览如下：

**表54** 高阶API扩展能力总览<a name="table_adv_api_ext"></a>

| API类别 | 涉及API | 扩展能力 | 详细说明 |
|---------|--------|---------|---------|
| 数学计算 | Tanh、Asin、Sin、Acos、Cos、Log、Atan、Fmod | 扩展支持算法配置（通过模板参数配置API使用的算法，提供高精度、高性能的算法选择） | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 数学计算 | Power | 扩展支持uint8_t、int8_t、uint16_t、int16_t、uint32_t、bfloat16_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 数学计算 | Sign | 扩展支持int64_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 数据归一化 | Normalize、WelfordUpdate | 扩展支持bfloat16_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 数据归一化 | LayerNorm | 扩展支持求方差 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 数据归一化 | WelfordFinalize | 扩展支持算法配置（指定计算方差时是否使用修正系数） | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 量化操作 | AscendQuant、AscendDequant、AscendAntiQuant | 扩展支持PRE_TOKEN、PRE_GROUP量化方式；扩展支持fp8_e5m2_t、fp8_e4m3fn_t、hifloat8_t、fp4x2_e1m2_t、fp4x2_e2m1_t等数据类型的量化和反量化 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 归约操作 | ReduceSum | 扩展支持int32_t、uint32_t、int64_t、uint64_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 归约操作 | ReduceMax、ReduceMin | 扩展支持int8_t、uint8_t、int16_t、uint16_t、bfloat16_t、int32_t、uint32_t、int64_t、uint64_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 排序操作 | TopK | 使用RADIX_SELECT算法时，扩展支持uint8_t、int8_t、uint16_t、int16_t、uint32_t、int32_t、bfloat16_t、uint64_t、int64_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 排序操作 | Sort | 扩展支持算法配置（指定排序算法以及降序升序排序） | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 索引计算 | Arange | 扩展支持int64_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 数据过滤 | DropOut | 扩展支持bfloat16_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 张量变换 | Transpose | 新增支持数据排布转换场景：二维转置或三维的后两位转置、三维中的第一维和第二维互换、三维中的第一维和第三维互换、使用交织指令对二维ND2NZ转置 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 张量变换 | BroadCast | 扩展支持动态Shape；扩展支持int16_t、uint16_t、bfloat16_t、int32_t、uint32_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 张量变换 | Fill | 扩展支持uint8_t、int8_t、bfloat16_t、uint64_t、int64_t数据类型 | [高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md#section14195124865717) |
| 数学计算 | [BitwiseAnd](../../../api/SIMD-API/adv_api/math_compute/BitwiseAnd_interface/BitwiseAnd.md)、[BitwiseNot](../../../api/SIMD-API/adv_api/math_compute/BitwiseNot_interface/BitwiseNot.md)、[BitwiseOr](../../../api/SIMD-API/adv_api/math_compute/BitwiseOr_interface/BitwiseOr.md)、[BitwiseXor](../../../api/SIMD-API/adv_api/math_compute/BitwiseXor_interface/BitwiseXor.md)、[LogicalAnd](../../../api/SIMD-API/adv_api/math_compute/LogicalAnd_interface/LogicalAnd.md)、[LogicalAnds](../../../api/SIMD-API/adv_api/math_compute/LogicalAnds_interface/LogicalAnds.md)、[LogicalNot](../../../api/SIMD-API/adv_api/math_compute/LogicalNot_interface/LogicalNot.md)、[LogicalOr](../../../api/SIMD-API/adv_api/math_compute/LogicalOr_interface/LogicalOr.md)、[LogicalOrs](../../../api/SIMD-API/adv_api/math_compute/LogicalOrs_interface/LogicalOrs.md)、[LogicalXor](../../../api/SIMD-API/adv_api/math_compute/LogicalXor_interface/LogicalXor.md) | 位运算/逻辑运算接口，仅Ascend 950PR/950DT支持 | [BitwiseAnd](../../../api/SIMD-API/adv_api/math_compute/BitwiseAnd_interface/BitwiseAnd.md) |
| 数学计算 | [Fma](../../../api/SIMD-API/adv_api/math_compute/Fma_interface/Fma.md)、[Clamp](../../../api/SIMD-API/adv_api/math_compute/Clamp_interface/Clamp.md)、[Hypot](../../../api/SIMD-API/adv_api/math_compute/Hypot_interface/Hypot.md)、[Rint](../../../api/SIMD-API/adv_api/math_compute/Rint_interface/Rint.md)、[SinCos](../../../api/SIMD-API/adv_api/math_compute/SinCos_interface/SinCos.md)、[Where](../../../api/SIMD-API/adv_api/math_compute/Where_interface/Where.md)、[IsFinite](../../../api/SIMD-API/adv_api/math_compute/IsFinite_interface/IsFinite.md)、[IsInf](../../../api/SIMD-API/adv_api/math_compute/IsInf_interface/IsInf.md)、[IsNan](../../../api/SIMD-API/adv_api/math_compute/IsNan_interface/IsNan.md) | 融合乘加/限幅/数学函数/判断类接口，仅Ascend 950PR/950DT支持 | [Fma](../../../api/SIMD-API/adv_api/math_compute/Fma_interface/Fma.md) |
| 量化操作 | [Quantize](../../../api/SIMD-API/adv_api/quantization/Quantize.md)、[Dequantize](../../../api/SIMD-API/adv_api/quantization/Dequantize.md)、[AntiQuantize](../../../api/SIMD-API/adv_api/quantization/AntiQuantize.md) | 量化/反量化/伪量化接口，仅Ascend 950PR/950DT支持 | [Quantize](../../../api/SIMD-API/adv_api/quantization/Quantize.md) |
| 随机函数 | [PhiloxRandom](../../../api/SIMD-API/adv_api/random_functions/PhiloxRandom.md) | 基于Philox算法的随机数生成接口，仅Ascend 950PR/950DT支持 | [PhiloxRandom](../../../api/SIMD-API/adv_api/random_functions/PhiloxRandom.md) |

#### 使用说明

- 上述扩展能力的具体API原型、模板参数配置及使用方式，请参考对应API文档及[高阶API迁移指导](../3510_arch_migration/2201_to_3510_guide/advanced_api_migration.md)。
- 高阶API相关样例位于`examples/01_simd_cpp_api/04_advanced_api`目录，各API对应样例请参考该目录说明。

### Hccl通信新增能力<a name="section_hccl"></a>

#### 特性应用场景说明

在多卡通信算子中，Hccl高阶API基于窗口（Window）机制实现多卡之间的数据收发。3510架构新增了CCU（Compute Communication Unit）服务端类型（HCCL_SERVER_TYPE_CCU），并配套新增了通信配置与数据下发相关接口，支撑多卡通信场景下的归约类型配置与窗口内数据自定义收发。

#### 功能说明

- [SetReduceType](../../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Tiling/SetReduceType.md)（Ascend 950PR/Ascend 950DT新增）：Tiling侧配置接口，用于设置Reduce操作类型，仅对有归约操作的通信任务生效。例如ReduceScatter任务在输入为int8_t/hifloat8_t/fp8_e5m2_t/fp8_e4m3fn_t时，输出数据类型必须为half/bfloat16_t/float之一。
- [AlltoAllvWrite](../../../api/SIMD-API/adv_api/HCCL_communication/HCCL_Kernel/AlltoAllvWrite.md)（Ascend 950PR/Ascend 950DT新增）：Kernel侧通信数据写入接口，通信域内每张卡可自定义给其它卡发送的数据量（sendSizes）及数据在内存中的偏移（sendOffsets），并支持配置对端窗口写入偏移（remoteWinOffset）与本卡接收数据量（localDataSize），返回任务标识handleId。
- 使用要点：

  - AlltoAllvWrite调用前需要先调用InitV2和SetCcTilingV2接口完成初始化；CCU服务端默认仅支持FullMesh算法，单次最大通信数据量不超过256M。
  - 一个通信域内所有Prepare和InterHcclGroupSync的调用总次数不超过63次。
  - 未指定下发核时，AlltoAllvWrite只能在AIC或AIV之一上调用；AI CPU服务端暂不支持上述能力。

### 其它增强说明

除上述特性外，3510架构还有以下增强与兼容性样例，具体请参考[2201到3510架构变更](../3510_arch_migration/2201_to_3510_arch_changes.md)：

- **AI Core核数增加**：3510架构AI Core核数较2201架构增加，支持更大的片上并行规模。
- **UB容量提升**：3510架构UB容量提升（总容量256KB，16个bank组成8个bank group，每个bank group包含2个bank，每个bank 16KB），若发生UB冲突，可参考[避免UB的bank冲突](../../operator_practice/simd_operator_optimization/memory_access/avoid_ub_bank_conflict/overview.md)解决；相关样例请参考[bank_conflict_3510样例](../../../../../examples/01_simd_cpp_api/05_best_practices/04_memory_access/bank_conflict_3510)（UB Bank冲突场景验证与分析（架构3510））。
- **数据类型扩展**：3510架构扩展了支持的数据类型（如fp8_e4m3fn_t/fp8_e5m2_t、hifloat8_t、fp4x2_e1m2_t/fp4x2_e2m1_t等），具体可参考[数据类型介绍](../../../api/SIMD-API/basic_api/data_structures/builtin_data_types.md)。其中bfloat16_t、hifloat8_t、fp8_e5m2_t、fp8_e4m3fn_t、fp8_e8m0_t、fp4x2_e2m1_t、fp4x2_e1m2_t、int4x2_t等数据类型在Host端支持以C++模板形式定义与声明（较A2/A3仅支持bfloat16_t范围更广），编译约束请参考[约束说明](../../programming_guide/compilation_and_execution/operator_compilation/constraints.md)。
- **新增复数数据类型**：新增[complex32/complex64](../../../api/SIMD-API/basic_api/aux_data_structures/complex32-complex64.md)数据类型（complex32表示实部和虚部都是half类型的复数，位宽为32位；complex64表示实部和虚部都是float类型的复数，位宽为64位），仅Ascend 950PR/Ascend 950DT支持。
- **GlobalTensor类型重解释**：新增[ReinterpretCast（GlobalTensor类型重解释）](../../../api/SIMD-API/basic_api/data_structures/GlobalTensor/ReinterpretCast.md)接口，可将当前GlobalTensor重解释为用户指定的新类型，仅Ascend 950PR/Ascend 950DT支持。
- **SIMT超大容量寄存器**：3510架构为SIMT应用程序提供总容量为128KB的超大容量寄存器（每个Thread可用的寄存器数和Thread数量相关），SIMT访问GM需经过SIMT DCache中转（最大128KB Data Cache，复用UB作为cacheline，访存粒度为128B），详细说明请参考[NPU架构规格](../../programming_guide/advanced_programming/hardware_implementation/architecture_spec/npu_arch_3510.md)。
- **新增Cache类型**：3510架构新增支持SIMT DCache与NDDMA Cache（2201架构仅支持L2 Cache、DCache和ICache），其中SIMT DCache用于SIMT线程访问Global Memory时的数据缓存，NDDMA Cache用于ND-DMA指令（[NDDMA多维搬运](#section_nddma)）的数据缓存，多核间数据一致性说明请参考[Cache一致性](../../programming_guide/advanced_programming/memory_model/cache_coherence.md)。完整Cache类型与功能说明请参考[系统缓存概述](../../../api/SIMD-API/basic_api/cache_control/system_cache_overview.md)。
- **Cache控制接口增强**：3510架构下[DataCacheCleanAndInvalid](../../../api/SIMD-API/basic_api/cache_control/DataCacheCleanAndInvalid.md)接口新增支持`CACHELINE_ATOMIC`参数（原子操作过程中保证DCache和GM的一致性，仅Ascend 950PR/Ascend 950DT支持，A3/A2等其它产品暂未支持），用于原子操作场景的Cache一致性保证。
- **UB空间使用与编译选项**：UB总容量为256KB，其中部分空间可能用于系统预留或划分为Data Cache。3510架构下Ascend C接口内部使用**2KB**预留UB空间（2201架构为8KB），用于指令兼容或存储中间数据。以下编译选项生效，可调整可用的UB空间大小：
  - `--cce-disable-asc-reserved-ubuf`：禁用Ascend C接口使用预留UB空间，开启后依赖预留UB空间的接口不可用（3510架构下使用预留UB空间的API范围请参考[使用预留UB空间的API列表](../../programming_guide/programming_model/ai_core_simd_programming/cpp_tensor_programming/static_tensor_programming.md#使用预留ub空间的api范围)中的表6）。
  - `--cce-disable-vf-stack-reserved-ubuf`：禁用SIMD VF栈预留的UB空间（3510架构生效，开启后可释放6KB预留空间作为普通UB使用，但编译器无法再使用该空间缓存寄存器溢出数据，需保证寄存器不溢出）。
  - SIMD与SIMT混合编程场景下，开启上述两个编译选项后，用户最多可使用的实际UB空间大小为`248KB + 6KB + 2KB - 32KB = 224KB`。
- **性能优化编译宏**：`NO_OVERLAP_IN_MULTI_REPEAT`（950架构专属）可在没有地址重叠的情况下移除不必要的内存同步指令，提升基础API高维切分计算场景的性能，详情请参考[内置编译宏开关](../../programming_guide/compilation_and_execution/operator_compilation/ai_core_operator_compilation.md#内置编译宏开关)。
- **SuperKernel二进制融合特性（试验）**：3510架构初次支持SuperKernel特性（算子的二进制融合技术，将多个子核函数融合为一个超级核函数，减少Task调度开销、实现算子间流水并行），当前在Ascend 950PR/Ascend 950DT为试验特性（后续版本可能调整，不保证兼容性），支持通过PyTorch图模式（npugraph_ex/GE图模式）开启，详细说明请参考[SuperKernel原理](../../programming_guide/advanced_programming/super_kernel/principles.md)。
- **SIMT协作组调度约束**：Ascend 950PR/Ascend 950DT的SIMT架构**不支持独立线程调度**，一个Warp内的多个[协作组](../../programming_guide/advanced_programming/simt_cooperative_groups.md)之间应避免存在相互等待或数据依赖，否则可能导致卡死。
- **DCI编译选项（性能优化，950独占）**：`--cce-no-dcache-flush`编译选项可在算子尾部插入DCI（DataCacheInvalid）指令替代默认的DCCI（DataCacheCleanAndInvalid）指令使DCache失效，减少数据从DCache同步回GM（Clean）的过程，降低算子尾开销。适用于开发者通过`*__gm__*`改写GM内存或调用GlobalTensor.SetValue时已自行调用DataCacheCleanAndInvalid保证Cache一致性的场景，详细说明请参考[设置DCI编译选项来减少算子尾开销](../../operator_practice/simd_operator_optimization/overhead_optimization/dci_compile_option.md)。
- **兼容性样例**：涉及2201到3510架构变更（如L1 Buffer到GM通路删除、SetLoadDataBoundary删除、int4b_t不支持、L0A Buffer分形变化、Fill接口不支持直接初始化L0A/L0B等）的兼容性适配样例，请参考[兼容性样例说明](../3510_arch_migration/2201_to_3510_arch_changes.md)中"影响"列对应的[06_compatibility_guide样例目录](../../../../../examples/01_simd_cpp_api/06_compatibility_guide)，包括[data_copy_l1togm样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/data_copy_l1togm)、[set_loaddata_boundary样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/set_loaddata_boundary)、[matmul_s4样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/matmul_s4)、[pattern_transformation样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/pattern_transformation)、[fill样例](../../../../../examples/01_simd_cpp_api/06_compatibility_guide/fill)。
