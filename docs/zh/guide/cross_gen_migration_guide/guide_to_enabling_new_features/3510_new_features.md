# 3510新特性使用说明

本文介绍[NPU架构版本3510](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)新增特性的能力说明与使能方法，帮助从其它架构迁移到3510架构的开发者使能并使用硬件新增的数据通路等能力。3510架构的整体变更情况请参考[2201到3510架构变更](../3510_arch_migration/2201_to_3510_arch_changes.md)。

## 特性总览

**表1** 3510新特性总览<a name="table_3510_new_feature_overview"></a>

| 特性 | 适用产品 | 特性说明 | 使能方式 | 详细说明 |
|------|---------|---------|---------|---------|
| 新增UB->L1搬运数据通路 | Ascend 950PR/Ascend 950DT | 支持将数据从Unified Buffer（UB）直接搬运到L1 Buffer，无需经过Global Memory（GM）中转，缩短数据搬运链路、提升性能 | 配置编译选项`ENABLE_CV_COMM_VIA_SSBUF=true` | [新增UB->L1搬运数据通路](#section_ub2l1) |

## 特性说明

### 新增UB->L1搬运数据通路<a name="section_ub2l1"></a>

#### 特性应用场景说明

- 在矩阵计算（Cube计算）相关算子（如Matmul算子、Vector与Cube融合算子）中，Vector核（AIV）侧计算或搬运产生的矩阵数据存放于UB（TPosition为VECIN/VECCALC/VECOUT），需要作为Cube核（AIC）侧矩阵乘加计算的输入矩阵使用。该类算子的典型数据流为：数据先从GM搬运到UB，再由UB搬运到L1 Buffer，之后通过LoadData从L1 Buffer搬入L0A/L0B Buffer并执行Mmad矩阵乘加计算。
- 在[NPU架构版本2201](../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)等更早架构上，UB与L1 Buffer之间没有直接的硬件搬运通路，UB->L1 Buffer的数据搬运需要经过GM中转（UB->GM->L1 Buffer），或借助Matmul高阶API的workspace GM空间实现，搬运链路长、性能开销大。
- 3510架构新增UB->L1 Buffer硬件数据通路，使能后数据可从UB直接搬运到L1 Buffer（无需经过GM中转），缩短数据搬运链路、提升算子性能。

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
