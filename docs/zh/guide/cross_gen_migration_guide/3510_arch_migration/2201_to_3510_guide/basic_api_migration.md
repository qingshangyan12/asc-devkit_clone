# 基础API迁移指导<a name="ZH-CN_TOPIC_0000002470508258"></a>

本节针对[NPU架构版本3510](../../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)的芯片变更对基础API兼容性产生的影响进行说明，并提供基础API的兼容性适配方案。

## 矢量计算<a id="vector-compute"></a>

-   **3510架构默认不支持Subnormal功能。**

    **说明**：Subnormal浮点数指的是指数位全为0、尾数不为0的浮点数，用于表示比最小正常数更小的值，避免“下溢为0”。3510版本默认不支持Subnormal，Subnormal浮点数在计算中被视为0。

    **兼容方案**：对于支持config参数的基础API，可以通过设置config模板参数来配置Subnormal计算模式。对于不传入config参数的部分基础API及高阶API，可通过编译选项`--cce-ftz`配置Subnormal处理方式，该选项默认为`true`。`--cce-ftz=false`时保留Subnormal，`--cce-ftz=true`时采用FTZ（Flush-To-Zero）模式。软件模拟通过精度扩展等方式处理Subnormal数据，避免其下溢为0。

    **表1**  涉及Subnormal的API和config参数说明

    | Ascend C基础API | 兼容说明 |
    |-----------------|---------|
    | Exp、Ln、Reciprocal、Sqrt、Rsqrt、Div | 以Ln接口为例来进行说明。<br>通过LnConfig结构体的参数algo来配置Subnormal计算模式。algo取值如下：<br>- LnAlgo::INTRENSIC，Subnormal处理方式受编译选项--cce-ftz控制，该选项默认为true。<br>- LnAlgo::PRECISION_1ULP_FTZ_TRUE，使用单指令计算得出结果，采用FTZ模式。<br>- LnAlgo::PRECISION_1ULP_FTZ_FALSE，支持Subnormal数据计算。<br>该参数默认值DEFAULT_LN_CONFIG的取值如下：<br>`constexpr LnConfig DEFAULT_LN_CONFIG = { LnAlgo::INTRINSIC };` |

    **迁移示例**：

    迁移前（2201）：硬件默认支持Subnormal，直接调用即可。

    ```cpp
    AscendC::Ln<half>(dstLocal, srcLocal, count);
    ```

    迁移后（3510）：通过LnConfig模板参数配置algo为PRECISION_1ULP_FTZ_FALSE，软件仿真保留Subnormal。注意config为const引用型模板参数，实参必须具备静态存储期，使用static constexpr。

    ```cpp
    static constexpr AscendC::LnConfig CONFIG = { AscendC::LnAlgo::PRECISION_1ULP_FTZ_FALSE };
    AscendC::Ln<half, CONFIG>(dstLocal, srcLocal, count);
    ```

    **对应样例**：[Subnormal兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/subnormal)

## 数据搬运<a name="section7530159122210"></a>

-   **DataCopy接口不支持L1 Buffer-\>GM通路。**

    **说明**：硬件删除L1 Buffer到GM的通路，无法将数据从L1 Buffer直接搬运到GM中。现有接口不支持L1 Buffer到GM的直接搬运。

    **兼容方案**：对于纯Cube计算场景：在GM多分配一个单位矩阵，通过Mmad矩阵乘法计算输出到L0C Buffer，再从L0C Buffer通过Fixpipe搬运到GM。对于Vector和Cube计算融合场景，可以通过L1 Buffer搬运到Unified Buffer（UB），再搬运到GM。以下以纯Cube计算场景为例进行说明，介绍算子核心流程。

    **迁移示例**：

    迁移前（2201）：GM→L1→GM，直接搬运。

    ```cpp
    AscendC::LocalTensor<T> leftMatrix(AscendC::TPosition::A1, a1Addr, M * K);
    AscendC::DataCopy(leftMatrix, aGlobal, M * K);
    AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE3>(EVENT_ID0);
    AscendC::DataCopy(cGlobal, leftMatrix, M * K);  // L1 -> GM
    ```

    迁移后（3510）：GM→L1→L0A/L0B→L0C→GM，通过矩阵乘法+Fixpipe搬运。

    ```cpp
    CopyGmToL1A(a1Local);  // GM -> L1A
    CopyGmToL1B(b1Local);  // GM -> L1B（B为单位矩阵）
    AscendC::SetFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
    AscendC::WaitFlag<AscendC::HardEvent::MTE2_MTE1>(EVENT_ID0);
    Load2DL1AToL0A(a1Local, a2Local);  // L1A -> L0A
    Load2DL1BToL0B(b1Local, b2Local);  // L1B -> L0B
    AscendC::SetFlag<AscendC::HardEvent::MTE1_M>(EVENT_ID1);
    AscendC::WaitFlag<AscendC::HardEvent::MTE1_M>(EVENT_ID1);
    Compute(co1Local, a2Local, b2Local);  // Mmad矩阵乘，A×单位矩阵=A
    AscendC::SetFlag<AscendC::HardEvent::M_FIX>(EVENT_ID2);
    AscendC::WaitFlag<AscendC::HardEvent::M_FIX>(EVENT_ID2);
    CopyL0CToGm(co1Local);  // L0C -> GM via Fixpipe
    ```

    **对应样例**：[L1到GM搬运兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/data_copy_l1togm)

-   **不支持SetLoadDataBoundary接口。**

    **说明**：3510架构硬件删除了L1 Buffer的边界值设定相关寄存器，不再支持SetLoadDataBoundary接口。该接口用于设置LoadData（卷积数据搬运）时L1 Buffer的边界值。如果指令在处理源操作数时，源操作数在L1 Buffer上的地址超出设置的边界，则会从L1 Buffer的起始地址开始读取数据。设置为0表示无边界，可以使用整个L1 Buffer。

    **兼容方案**：

    -   [NPU架构版本2201](../../../programming_guide/language_extension/simd_builtin_keywords.md#npu-arch)的接口参数boundaryValue设置为0时与3510架构版本等价。
    -   如果需要在L1 Buffer上循环读取操作数，需要将对应的LoadData（卷积数据搬运）接口手动拆分成多条指令，手动绕回。。

    ![](../../../figures/fig_1_zn.png)

    **迁移示例**：

    迁移前（2201）：通过SetLoadDataBoundary设置边界，单次LoadData自动绕回。

    ```cpp
    AscendC::SetLoadDataRepeat({0, 1, 0});
    AscendC::SetLoadDataBoundary(1024);  // 设置L1边界为1024B
    AscendC::LoadData(a2, leftMatrix, loadData3dParamsPro);
    ```

    迁移后（3510）：删除SetLoadDataBoundary，多次调用LoadData手动绕回。

    ```cpp
    uint16_t dstStride = AscendC::DivCeil(M / 2, 16);
    AscendC::SetLoadDataRepeatWithStride({0, 1, 0, dstStride});
    AscendC::LoadData(a2, leftMatrix, loadData3dParamsPro);
    AscendC::LocalTensor<T> a3 = a2[256];   // 目的地址偏移256
    AscendC::LoadData(a3, leftMatrix, loadData3dParamsPro);
    AscendC::LocalTensor<T> a4 = a2[512];   // 目的地址偏移512
    AscendC::LoadData(a4, leftMatrix, loadData3dParamsPro);
    AscendC::LocalTensor<T> a5 = a2[768];   // 目的地址偏移768
    AscendC::LoadData(a5, leftMatrix, loadData3dParamsPro);
    ```

    **对应样例**：[SetLoadDataBoundary兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/set_loaddata_boundary)

## 矩阵计算<a name="section17560113713122"></a>

-   **Cube计算单元删除int4b\_t数据类型。**

    **说明**：相较于2201架构版本，3510架构版本的Cube计算单元不支持int4b\_t。相关的基础API有LoadData、Mmad和LoadDataWithTranspose，这些接口不再支持int4b\_t。

    **兼容方案**：算子侧通过编写CV融合算子在Vector Core进行int4b\_t到int8\_t的Cast转换，再通过UB搬运到L1后进行Mmad计算。

    **迁移示例**：

    迁移前（2201）：直接使用int4b\_t进行Matmul计算。

    ```cpp
    AscendC::Matmul<A_TYPE_S4, B_TYPE_S4, C_TYPE, BIAS_TYPE, CFG_MDL> mm;
    mm.SetTensorA(gmA, false);  // gmA为int4b_t类型
    mm.SetTensorB(gmB, false);  // gmB为int4b_t类型
    mm.IterateAll(gmC);
    ```

    迁移后（3510）：先在Vector Core将int4b\_t Cast为int8\_t，再进行int8\_t矩阵乘。

    ```cpp
    // Step1 - Vector Core上int4b_t -> int8_t转换
    AscendC::LocalTensor<AscendC::int4b_t> int4SrcLocalTensor =
        srcLocalTensor.ReinterpretCast<AscendC::int4b_t>();
    AscendC::Cast<half, AscendC::int4b_t>(tmpTensor, int4SrcLocalTensor,
        AscendC::RoundMode::CAST_NONE, count * 2);
    AscendC::PipeBarrier<PIPE_V>();
    AscendC::Cast<int8_t, half>(dstLocalTensor, tmpTensor,
        AscendC::RoundMode::CAST_CEIL, count * 2);
    // Step2 - int8_t类型矩阵乘
    AscendC::Matmul<A_TYPE_S8, B_TYPE_S8, C_TYPE, BIAS_TYPE, CFG_MDL> mm;
    mm.SetTensorA(gmA, false);  // gmA已转换为int8_t类型
    mm.SetTensorB(gmB, false);  // gmB已转换为int8_t类型
    mm.IterateAll(gmC);
    ```

    **对应样例**：[int4数据类型下Matmul兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/matmul_s4)

-   **L0A Buffer分形改变，从Zz转换为Nz格式。**

    **说明**：涉及的API有LoadData、Mmad和LoadDataWithTranspose。

    -   2201架构版本，参与矩阵乘计算（A \* B = C）时，ABC矩阵的数据排布格式分别为Zz，Zn，Nz。A、B、C矩阵分别位于L0A Buffer、L0B Buffer、L0C Buffer。

        矩阵A：每个分形矩阵内部是行主序，分形矩阵之间是行主序。分形Shape为16 x \(32B/sizeof\(AType\)\)，大小为512Byte。

        矩阵B：每个分形矩阵内部是列主序，分形矩阵之间是行主序。分形Shape为 \(32B/sizeof\(BType\)\) x 16，大小为512Byte。

        矩阵C：每个分形矩阵内部是行主序，分形矩阵之间是列主序。分形Shape为16 x 16，大小为256个元素。

        ![](../../../figures/fig_2_zn.png)

    -   3510架构版本，参与矩阵乘计算（A \* B = C）时，ABC矩阵的数据排布格式分别为Nz，Zn，Nz。

        矩阵A：每个分形矩阵内部是行主序，分形矩阵之间是列主序。其Shape为16 x \(32B/sizeof\(AType\)\)，大小为512Byte。

        矩阵B：每个分形矩阵内部是列主序，分形矩阵之间是行主序。其Shape为 \(32B/sizeof\(BType\)\) x 16，大小为512Byte。

        矩阵C：每个分形矩阵内部是行主序，分形矩阵之间是列主序。其Shape为16 x 16，大小为256个元素。

        ![](../../../figures/fig_3_zn.png)

    **兼容方案**：非L0A Buffer切分的场景兼容2201版本，L0A Buffer切分的场景需要根据新的分形重新适配。

    在2201架构中，矩阵计算要求左矩阵为ZZ分形（3510中为NZ），右矩阵为ZN分形，由于L1 Buffer的数据分形为NZ，所以2201架构下将左矩阵从L1 Buffer搬运到L0A Buffer需要额外做NZ分形到ZZ分形的转换，3510架构下则不用转换分形。

    ![](../../../figures/fig_4_zn.png)

    **迁移示例**：

    迁移前（2201）：L1→L0A需要NZ→ZZ分形转换，使用旧版LoadData2DParams循环搬运。

    ```cpp
    constexpr uint32_t mBlocks = M / CUBE_BLOCK;
    constexpr uint32_t kBlocks = K * sizeof(T) / C0_SIZE;
    int srcOffset = 0;
    int dstOffset = 0;
    for (uint32_t i = 0; i < mBlocks; ++i) {
        AscendC::LoadData2DParams loadDataParams;
        loadDataParams.repeatTimes = kBlocks;
        loadDataParams.srcStride = mBlocks;
        loadDataParams.ifTranspose = false;
        AscendC::LoadData(a2[dstOffset], a1[srcOffset], loadDataParams);
        srcOffset += CUBE_BLOCK * CUBE_BLOCK;
        dstOffset += K * CUBE_BLOCK;
    }
    ```

    迁移后（3510）：L1→L0A直接按NZ搬运，无需分形转换，使用LoadData2DParamsV2。

    ```cpp
    AscendC::LoadData2DParamsV2 loadDataParams;
    loadDataParams.mStartPosition = 0;
    loadDataParams.kStartPosition = 0;
    loadDataParams.mStep = AscendC::DivCeil(M, CUBE_BLOCK);
    loadDataParams.kStep = AscendC::DivCeil(K * sizeof(T), C0_SIZE);
    loadDataParams.srcStride = AscendC::DivCeil(M, CUBE_BLOCK);
    loadDataParams.dstStride = AscendC::DivCeil(M, CUBE_BLOCK);
    loadDataParams.sid = 0;
    loadDataParams.ifTranspose = false;
    AscendC::LoadData(a2, a1, loadDataParams);
    ```

    **对应样例**：[pattern_transformation兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/pattern_transformation)

-   **3510架构版本硬件架构删除4：2结构化稀疏功能。**<a name="li69092585134"></a>

    **说明**：2201架构支持4:2结构化稀疏，通过`LoadDataWithSparse`将L1 Buffer中的512B稠密权重矩阵搬运到L0B Buffer并同时读取128B索引矩阵实现稀疏化，再通过`MmadWithSparse`执行稀疏矩阵乘加。3510架构不支持结构化稀疏功能，`LoadDataWithSparse`和`MmadWithSparse`均不可用。

    **兼容方案**：不调用`LoadDataWithSparse`进行矩阵稠密转稀疏操作，直接使用`Mmad`进行正常的稠密矩阵计算。稀疏矩阵相关算法可参考MmadWithSparse中的介绍。

    **迁移示例**：

    迁移前（2201）：使用LoadDataWithSparse + MmadWithSparse进行稀疏矩阵乘。

    ```cpp
    AscendC::LoadDataWithSparse(b2Local, b1Local, loadDataSparseParams);
    AscendC::MmadWithSparse(co1Local, a2Local, b2Local, mmadSparseParams);
    ```

    迁移后（3510）：使用LoadData + Mmad进行稠密矩阵乘。

    ```cpp
    AscendC::LoadData(b2Local, b1Local, loadDataParams);
    AscendC::Mmad(co1Local, a2Local, b2Local, mmadParams);
    ```

-   **3510架构版本删除GM-\>L0A Buffer/L0B Buffer通路**

    **说明**：硬件删除GM-\>L0A Buffer/L0B Buffer通路，调用LoadData时，不再支持这些通路。

    **兼容方案**：实现GM→L0A Buffer/L0B Buffer搬运需拆分成两步进行，先从GM搬运到L1 Buffer，再从L1 Buffer搬运到L0A Buffer、L0B Buffer。

    **迁移示例**：

    迁移前（2201）：GM→L0A，单步LoadData直接搬运。

    ```cpp
    AscendC::LoadData(a2Local, aGlobal, loadDataParams);  // 源为GM
    ```

    迁移后（3510）：GM→L1→L0A，两步搬运。

    ```cpp
    // Step1 - GM -> L1
    AscendC::Nd2NzParams intriParams{1, M, K, 0, K, M, 1, 0};
    AscendC::DataCopy(a1Local, aGlobal, intriParams);
    // Step2 - L1 -> L0A
    AscendC::LoadData2DParamsV2 loadDataParams;
    loadDataParams.mStep = AscendC::DivCeil(M, CUBE_BLOCK);
    loadDataParams.kStep = AscendC::DivCeil(K * sizeof(T), C0_SIZE);
    loadDataParams.srcStride = AscendC::DivCeil(M, CUBE_BLOCK);
    loadDataParams.dstStride = AscendC::DivCeil(M, CUBE_BLOCK);
    AscendC::LoadData(a2Local, a1Local, loadDataParams);
    ```

    **对应样例**：[L1到GM搬运兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/data_copy_l1togm)（3510分支实现了GM→L1→L0A/L0B通路拆分）

-   **3510架构版本删除L0A Buffer/L0B Buffer初始化的相关硬件指令。**

    **说明**：2201架构支持通过`Fill`接口直接初始化L0A Buffer、L0B Buffer。3510架构删除了L0A Buffer/L0B Buffer初始化的相关硬件指令，`Fill`接口不支持直接初始化L0A/L0B。

    **兼容方案**：先通过`Fill`接口初始化L1 Buffer，再通过`LoadData`接口将L1 Buffer上的数据搬运到L0A Buffer、L0B Buffer。

    **迁移示例**：

    迁移前（2201）：直接Fill初始化L0A。

    ```cpp
    AscendC::Fill(a2Local, {1, static_cast<uint16_t>(M * K * sizeof(T) / 512), 0, 1});
    ```

    迁移后（3510）：Fill初始化L1，再LoadData搬运到L0A。

    ```cpp
    // Step1 - Fill初始化L1
    AscendC::Fill(a1Local, {1, static_cast<uint16_t>(M * K * sizeof(T) / 32), 0, 1});
    AscendC::PipeBarrier<PIPE_MTE1>();
    // Step2 - LoadData从L1搬运到L0A
    AscendC::LoadData2DParamsV2 loadDataParams;
    loadDataParams.mStep = AscendC::DivCeil(M, CUBE_BLOCK);
    loadDataParams.kStep = AscendC::DivCeil(K * sizeof(T), C0_SIZE);
    loadDataParams.srcStride = AscendC::DivCeil(M, CUBE_BLOCK);
    loadDataParams.dstStride = AscendC::DivCeil(M, CUBE_BLOCK);
    AscendC::LoadData(a2Local, a1Local, loadDataParams);
    ```

    **对应样例**：[Fill兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/fill)

-   **Fixpipe接口参数结构体切换。**

    **说明**：2201架构的Fixpipe接口使用`FixpipeParamsV220`参数结构体。3510架构引入了原生的`FixpipeParamsArch3510`参数结构体，同时也兼容`FixpipeParamsV220`，但推荐使用原生结构体以获得完整能力。

    **两种参数结构体的关键差异**：

    | 对比项 | FixpipeParamsV220（2201） | FixpipeParamsArch3510（3510） |
    |--------|--------------------------|------------------------------|
    | NZ2ND参数 | `ndNum`/`srcNdStride`/`dstNdStride`为顶层字段 | 收纳于`params`子结构（由`format`模板选择类型） |
    | dstStride单位（NZ2NZ场景） | datablock（32字节） | element（元素个数） |
    | dstStride单位（NZ2ND场景） | element | element（与V220一致） |
    | 高级ReLU | 不支持 | 支持`preReluMode`/`preClipReluMode`/`reluScalar`/`vectorRelu` |
    | NZ2DN格式转换 | 不支持 | 支持`CO2Layout::COLUMN_MAJOR` |
    | UB通路控制 | 不支持 | 支持`dualDstCtl`/`subBlockId` |

    > **迁移注意**：在NZ2NZ（非NZ2ND）场景下，`dstStride`的单位从datablock变为element，直接复用V220的值会导致3510上地址计算错误。在NZ2ND（CFG_ROW_MAJOR）场景下，两种结构体的dstStride单位均为element，可直接统一赋值。

    **兼容方案**：2201使用`FixpipeParamsV220`，3510使用`FixpipeParamsArch3510`。对于NZ2ND场景，公共参数（nSize/mSize/srcStride/dstStride/quantPre）两种结构体字段名一致，可统一赋值。

    **迁移示例（NZ2ND场景）**：

    迁移前（2201）：使用FixpipeParamsV220。

    ```cpp
    AscendC::FixpipeParamsV220 fixpipeParams;
    fixpipeParams.ndNum = 1;
    fixpipeParams.srcNdStride = 0;
    fixpipeParams.dstNdStride = 0;
    fixpipeParams.mSize = baseM;
    fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
    fixpipeParams.nSize = baseN;
    fixpipeParams.dstStride = baseN;
    fixpipeParams.quantPre = QuantMode_t::NoQuant;
    AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_ROW_MAJOR>(cGM, cL0C, fixpipeParams);
    ```

    迁移后（3510）：使用FixpipeParamsArch3510原生结构体，NZ2ND参数由`params`子结构管理，公共参数字段名一致。

    ```cpp
    AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::ROW_MAJOR> fixpipeParams;
    fixpipeParams.mSize = baseM;
    fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
    fixpipeParams.nSize = baseN;
    fixpipeParams.dstStride = baseN;
    fixpipeParams.quantPre = QuantMode_t::NoQuant;
    AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_ROW_MAJOR>(cGM, cL0C, fixpipeParams);
    ```

    **迁移示例（NZ2NZ场景，dstStride单位不同）**：

    迁移前（2201）：dstStride单位为datablock（32字节）。

    ```cpp
    AscendC::FixpipeParamsV220 fixpipeParams;
    fixpipeParams.dstStride = baseM * c0Size * sizeof(outputType) / AscendC::ONE_BLK_SIZE;
    fixpipeParams.nSize = CeilAlign(baseN, c0Size);
    fixpipeParams.mSize = baseM;
    fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
    AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(cGM, cL0C, fixpipeParams);
    ```

    迁移后（3510）：dstStride单位为元素个数。

    ```cpp
    AscendC::FixpipeParamsArch3510<AscendC::CO2Layout::NZ> fixpipeParams;
    fixpipeParams.dstStride = baseM * c0Size;
    fixpipeParams.nSize = CeilAlign(baseN, c0Size);
    fixpipeParams.mSize = baseM;
    fixpipeParams.srcStride = CeilAlign(baseM, CUBE_BLOCK);
    AscendC::Fixpipe<outputType, l0cType, AscendC::CFG_NZ>(cGM, cL0C, fixpipeParams);
    ```

    **对应样例**：[Fixpipe参数结构体切换兼容性样例](../../../../../../examples/01_simd_cpp_api/06_compatibility_guide/fixpipe_params_switch)

## 系统变量访问<a name="section34615332215"></a>

-   **不支持CheckLocalMemoryIA，3510架构版本相关寄存器删除。**

    **说明**：CheckLocalMemoryIA监视设定范围内的UB读写行为，如果监视到有设定范围的读写行为则会出现EXCEPTION报错，未监视到设定范围的读写行为则不会报错。

    **兼容方案**：该接口为调测接口，对功能无影响，直接删除调用即可。

    **迁移示例**：

    迁移前（2201）：调用CheckLocalMemoryIA监视UB读写。

    ```cpp
    AscendC::CheckLocalMemoryIA(...);
    ```

    迁移后（3510）：接口删除，直接移除调用。

    ```cpp
    // 接口已删除，无需调用
    ```

## 调测API<a name="section12611957102116"></a>

-   **L1 Buffer上不支持Tensor信息的打印。**

    **说明**：因芯片删除L1 Buffer-\>GM通路，不支持L1 Buffer-\>GM的功能。

    **兼容方案**：该接口为调测接口，对功能无影响。如需调试L1 Buffer上的数据，可先将数据搬运到UB再打印。

    **迁移示例**：

    迁移前（2201）：直接打印L1 Buffer上的Tensor。

    ```cpp
    AscendC::Print(a1Local);
    ```

    迁移后（3510）：先将L1数据搬运到UB，再打印。

    ```cpp
    AscendC::DataCopy(ubLocal, a1Local, count);
    AscendC::SetFlag<AscendC::HardEvent::MTE1_V>(EVENT_ID0);
    AscendC::WaitFlag<AscendC::HardEvent::MTE1_V>(EVENT_ID0);
    AscendC::Print(ubLocal);
    ```
