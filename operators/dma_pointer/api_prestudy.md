# API 预研报告

## 芯片代际

- 分析代际：DAV_3510（`__NPU_ARCH__ == 3510`），对应 Ascend 950PR / Ascend 950DT（Atlas A5）。
- 判定依据：新增声明和实现均由 `__NPU_ARCH__ == 3510` 条件编译保护（`include/basic_api/kernel_operator_data_copy_intf.h:473`、`impl/basic_api/kernel_operator_data_copy_intf_impl.h:1892`、`include/basic_api/kernel_operator_cache_intf.h:55`、`impl/basic_api/kernel_operator_cache_intf_impl.h:89`）；测试目录名为 `ascendc_case_ascend950pr_9599`；`npu-arch` 映射表确认 Ascend950PR/DT 对应 DAV_3510 / `__NPU_ARCH__=3510`。
- 代际特有能力：DAV_3510 具备 248 KB UB、256 KB L0C、4 KB BT，CubeCore:VectorCore 为 1:2；新增 UB→L1/SSBuffer、NDDMA、SIMT、SIMD-RegBase、CCU 等能力。本次代码实际涉及裸地址空间指针（`__gm__`、`__ubuf__`、`__cbuf__`、`__cc__`）、UB↔L1、L0C 搬运和 NDDMA；未使用 SIMT、RegBase 或 CCU API。
- 重要代际差异：DAV_3510 已删除 L1→GM 硬件通路；仓内官方文档 `DataCopy_L1ToGM_continuous.md` 与 `DataCopy_L1ToGM_highdim_split.md` 均明确标注 Ascend 950PR/DT 不支持。因此任何 `DataCopy(__gm__ T*, __cbuf__ T*, ...)` 只能视为待确认/不支持路径，不能仅因模板可实例化就认定功能有效。

## 代码中实际出现的 API

- 核心数据搬运：`DataCopy`、`DataCopyPad`。
- 其他公开 Basic API：`DataCopyL1ToUB`、`DataCachePreload`、`DataCacheCleanAndInvalid`、`ICachePreLoad`、`SetLoopModePara`、`ResetLoopModePara`、`NdDmaDci`（声明/文档依赖，测试中未调用）。
- 测试/数据结构方法：`LocalTensor::InitBuffer`、`LocalTensor::GetPhyAddr`、`GlobalTensor::SetGlobalBuffer`、`LocalTensor::SetAddr`、`InitOutput`。
- 实现层内部入口：`DataCopy*Impl`、`DataCopyPad*Impl`、`DataCopyCheck`、`CheckNd2NzParams`、`CheckNz2NdParams`、`DataCopyGetPhyStartIndex`、`DataCopyGetOffsetList`、`DcciGMImpl`、`DcciUBImpl`、`DataCachePreloadImpl`。这些不是用户公开 API，不应由上层直接依赖。
- 未出现核心清单中的 `AllocTensor`、`FreeTensor`、`EnQue`、`DeQue`、向量计算和归约 API。

## 数据搬运

### DataCopy

- 代际差异：
  - DAV_3510 的连续/高维 GM↔UB、GM→L1、UB↔UB、UB→L1、L1→UB、L0C→L1/GM、ND2NZ、DN2NZ、NZ2ND、Slice 和 NDDMA 路径有对应实现或文档支持。
  - L1→GM 在 Ascend 950PR/DT 上不支持，但新增接口仍声明/实现并在头文件检查中调用：`include/basic_api/kernel_operator_data_copy_ptr_intf.h:28-34,67-76,100-103`、`impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:39-42,226-230,289-293`、`tests/api/basic_api/ascendc_header_checker/kernel_operator_data_copy_intf.cpp:436,443,482`。后续检视必须把它作为架构兼容性重点。
  - UB→L1 在 DAV_3510 推荐通过 `ENABLE_CV_COMM_VIA_SSBUF=true` 使用硬通道；关闭时需借助 Matmul 高阶 API 注册并绕行 GM。
  - 仓内 `DataCopy_L0CToUB.md` 标注 Ascend 950PR/DT 不支持该接口语义，而本次新增 `__cc__`→`__ubuf__` 和 `__ubuf__`→`__cc__` 重载；需以 DAV_3510 当前 CANN 头文件/上板结果确认，不能沿用其他代际结论。
- 函数签名：本次在既有 Tensor 重载之外，为 DAV_3510 增加同名裸指针重载。主要形式为：
  - `DataCopy(__ubuf__ T* dst, __gm__ T* src, DataCopyParams/count)`；
  - `DataCopy(__gm__ T* dst, __ubuf__ T* src, DataCopyParams/count)`；
  - `DataCopy(__cbuf__ T* dst, __gm__/__ubuf__ T* src, ...)`、`DataCopy(__ubuf__ T* dst, __cbuf__ T* src, ...)`；
  - ND2NZ/DN2NZ/NZ2ND、Slice、`DataCopyEnhancedParams`、`DataCopyCO12DstParams` 和 `MultiCopyParams`（`NdDmaParams` 的兼容名称）变体；
  - 完整声明见 `include/basic_api/kernel_operator_data_copy_ptr_intf.h:19-176`，实现见 `impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:20-434`。
- 对齐要求：
  - GM 地址按元素类型自然对齐（部分矩阵文档按最低 1 字节描述）；UB、L1 地址通常必须 32 字节对齐。
  - L0C NZ 源地址通常要求 64 字节对齐；L0C→GM 的 ND 目的地址最低 1 字节对齐，NZ 目的地址 32 字节对齐。
  - `count` 连续搬运要求 `count * sizeof(T)` 为 32 字节倍数；否则硬件按文档向下截断到 32 字节对齐长度。
- `DataCopyParams` 限制：常规 GM/UB/L1 高维切分中 `blockCount` 为 `[0,4095]`，`blockLen/srcGap/dstGap` 为 `[0,65535]`；`blockLen`、`srcGap`、`dstGap` 通常以 DataBlock（32 B）为单位。DAV_3510 上 `blockCount==0` 或 `blockLen==0` 为 NOP。个别 L0C/Bias/Fixpipe 通路的单位和偶数约束不同，不能机械复用常规参数。
- Slice 限制：地址分别满足 GM 自然对齐和 UB 32 B 对齐；维数不超过 8；数组长度需与 `dimValue` 一致；源/目的 `burstLen` 相等；`dimValue>1` 时 `burstLen` 必须为 1；横向搬运字节数必须是 32 B 倍数；`startIndex < endIndex <= shapeValue`。裸指针无法从 Tensor 元数据取得 shape，因此新增实现要求调用方显式填写 `SliceInfo::shapeValue`（`impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:94-113,150-169`）。
- ND2NZ/DN2NZ/NZ2ND 限制：ND2NZ 在 DAV_3510 支持 `enableSmallC0`；输入或关键维度为 0 时为 NOP；L1 目的地址 32 B 对齐，重叠写入顺序不保证；各结构体字段必须满足官方文档范围。NZ2ND 是软仿易用性接口，参数范围需按 `Nz2NdParamsFull` 文档校验。
- NDDMA 限制：仅 DAV_3510/相应新代际使用；总地址跨度不能超过 40 bit（1 TB），循环目的区间不得交织/重叠；NDDMA Cache 为 32 KB，多核读写同一 GM 前必须先调用 `NdDmaDci()` 刷新。代码暴露入口在 `include/basic_api/kernel_operator_data_copy_ptr_intf.h:175-176` 和 `impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:429-434`，头文件检查调用在 `tests/api/basic_api/ascendc_header_checker/kernel_operator_data_copy_intf.cpp:529-531`，但测试没有先调用 `NdDmaDci()`，编译检查不等于运行时使用范例。
- 同步机制：`DataCopy` 本身不强制要求 `EnQue/DeQue`；若数据进入 TQue 流水，生产者/消费者必须按队列语义配对。多个搬运的目的地址有重叠时，必须按通路插入 `PipeBarrier<PIPE_MTE2>()` 或 `PipeBarrier<PIPE_MTE3>()` 串行化。裸指针重载不会携带 Tensor 的逻辑位置/队列事件信息，调用方需自行保证地址空间和同步正确。
- 精度约束：纯同类型 DMA 不做数值计算。L0C 搬出与增强搬运可能附带量化/ReLU/格式转换；float32 量化按处理器 10 bit 尾数语义，量化参数不得为 Inf/NaN/非规格化数，且 ReLU 标志与量化配置必须一致。启用精度转换时，部分通路 `blockLen` 单位需折半，必须按对应 L0C 文档配置。
- 代码中的使用位置：声明 `include/basic_api/kernel_operator_data_copy_intf.h:20-470`、`include/basic_api/kernel_operator_data_copy_ptr_intf.h:19-176`；实现 `impl/basic_api/kernel_operator_data_copy_intf_impl.h:51-1889`、`impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:20-434`；裸指针头文件检查 `tests/api/basic_api/ascendc_header_checker/kernel_operator_data_copy_intf.cpp:426-531`；运行测试 `tests/api/basic_api/ascendc_case_ascend950pr_9599/ascendc_case_ascend950pr_9599_aiv_basic/test_data_copy.cpp:113-135`。

### DataCopyPad

- 代际差异：Ascend 950PR/DT 支持 GM→UB、UB→GM、GM→L1、UB→L1；DAV_3510 的 `DataCopyExtParams` 具有更宽的 `blockCount` 和 40 bit stride 语义，并支持 `PaddingMode`。GM→L1 仅在 Ascend 950PR/DT 文档中标为支持。
- 函数签名：Tensor 官方原型使用 `DataCopyExtParams`/`DataCopyPadExtParams<T>`；本次新增同等裸指针重载，并保留 `DataCopyParams`/`DataCopyPadParams` 兼容形式。声明见 `include/basic_api/kernel_operator_data_copy_ptr_intf.h:143-172`，实现见 `impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:367-427`。
- 对齐要求：GM 地址最低 1 字节/自然对齐；UB 地址 32 字节对齐。GM→UB/L1 的 `leftPadding`、`rightPadding` 各自占用字节数不得超过 32 B；`blockLen` 必须是 `sizeof(T)` 的整数倍；b64 的 `paddingValue` 只能为 0。
- 参数限制（DAV_3510）：
  - GM→UB/L1：`blockCount` `[0,65535]`，`blockLen` `[0,2^21-1]` 字节，`srcStride` `[-blockLen,2^40-1]`，`dstStride` `[0,65535]`。
  - UB→GM：`blockCount` `[0,65535]`，`blockLen` `[0,2^21-1]`，`srcStride` `[0,65535]`，`dstStride` `[0,2^40-1]`。
  - `blockCount==0` 或 `blockLen==0` 为 NOP；Compact 模式下 `dstStride/leftPadding/rightPadding` 无效。
  - UB→L1 的 `srcStride/dstStride` 在 DAV_3510 为 `int64_t`，取值 `[0,65535]`。
- 同步机制：与 `DataCopy` 一样，重叠目的区域需显式流水屏障；接口本身不替代队列同步。
- 精度约束：填充值类型必须与源数据一致；除可选填充外不做数值运算。
- 代码中的使用位置：既有声明/实现 `include/basic_api/kernel_operator_data_copy_intf.h:388-446`、`impl/basic_api/kernel_operator_data_copy_intf_impl.h:1417-1812`；指针声明/实现 `include/basic_api/kernel_operator_data_copy_ptr_intf.h:143-172`、`impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:367-427`；头文件检查 `tests/api/basic_api/ascendc_header_checker/kernel_operator_data_copy_intf.cpp:502-514`；运行测试 `tests/api/basic_api/ascendc_case_ascend950pr_9599/ascendc_case_ascend950pr_9599_aiv_basic/test_data_copy_pad.cpp:184-205`。

### DataCopyL1ToUB

- 代际差异：该公开接口仅支持 Ascend 950PR/DT，且仅适用于 Cube:Vector=1:2 的 Mix 算子；DAV_3510 满足 1:2 架构条件。
- 函数签名：`DataCopyL1ToUB(__ubuf__ T* dst, __cbuf__ T* src, const uint32_t count)` 或 `DataCopyParams` 版本；`subBlockId` 默认 0。
- 对齐/参数限制：L1 和 UB 地址遵循 32 B 对齐；`subBlockId` 只能是 0 或 1；数据类型按文档为 b8/b16/b32/b64；常规 `DataCopyParams` 使用 32 B DataBlock 单位。
- 同步机制：多个指令目的地址重叠时必须使用 `PipeBarrier` 串行化。
- 代码中的使用位置：`include/basic_api/kernel_operator_data_copy_ptr_intf.h:137-141`、`impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:353-365`、`tests/api/basic_api/ascendc_header_checker/kernel_operator_data_copy_intf.cpp:447-448`。

## 缓存控制

### DataCachePreload

- 代际差异：Ascend 950PR/DT 支持；每次仅预加载一个 Cache Line 到 DCache。
- 函数签名：官方 Tensor 形式为 `DataCachePreload(const GlobalTensor<uint64_t>& src, T cacheOffset)`；本次新增 `DataCachePreload(__gm__ uint64_t* src, T cacheOffset)`（`include/basic_api/kernel_operator_cache_intf.h:57`、`impl/basic_api/kernel_operator_cache_intf_impl.h:91-94`）。
- 参数限制：`src` 元素类型固定为 `uint64_t`；`cacheOffset` 单位 byte，文档支持 `int16_t`、`int64_t`。连续调用建议不超过 4 次，否则 Reservation Station 可能拥塞，指令可能按 NOP 处理并阻塞 Scalar 流水。
- 同步/精度：无 EnQue/DeQue 配对，无数值精度问题；调用方需确保预取地址合法。
- 代码中的使用位置：`tests/api/basic_api/ascendc_header_checker/kernel_operator_cache_intf.cpp:67-73`。

### DataCacheCleanAndInvalid

- 代际差异：Ascend 950PR/DT 支持 `CACHELINE_ATOMIC`，而 A2/A3 等文档标为暂不支持；Local/`__ubuf__` 形式只允许 AIV 调用。
- 函数签名：支持 `DataCacheCleanAndInvalid<T, CacheLine, DcciDst>(__gm__/__ubuf__ T*)`，以及省略 `DcciDst` 的 GM 形式；声明 `include/basic_api/kernel_operator_cache_intf.h:60-66`，实现 `impl/basic_api/kernel_operator_cache_intf_impl.h:96-112`。
- 对齐/参数限制：`SINGLE_CACHE_LINE` 以 64 B Cache Line 为粒度；若地址不按 64 B 对齐，只处理传入地址到下一 64 B 边界的部分。`ENTIRE_DATA_CACHE` 刷全 DCache，耗时大，性能敏感路径慎用。
- 模式约束：`SINGLE_CACHE_LINE` 时 `dst` 生效而 `dcciDst` 不生效；`ENTIRE_DATA_CACHE` 时相反。多核同时写同一 Cache Line 会有覆盖风险，不能把本接口当作跨核一致性同步原语。
- 同步/精度：无队列配对和数值精度问题；用于 Scalar/DMA 访问 GM 的 DCache 一致性维护。
- 代码中的使用位置：`tests/api/basic_api/ascendc_header_checker/kernel_operator_cache_intf.cpp:74-78`。

## 内存管理

### InitBuffer

- 代际差异：DAV_3510 支持 TPipe/TBufPool 的标准 `InitBuffer`，UB 用户可用容量典型值为 248 KB；实际容量仍应通过平台接口获取，不能硬编码。
- 官方函数签名：`TPipe/TBufPool::InitBuffer(T& que, uint8_t num, uint32_t len)` 或 `InitBuffer(TBuf<pos>& buf, uint32_t len)`；`len` 单位 byte，非 32 B 对齐时向上补齐。TBufPool 默认最多 4 个 Buffer ID，可配置到 16。
- 本次测试实际调用的是 `LocalTensor::InitBuffer(offset, length)` 测试/仿真辅助方法，而非上述标准 TPipe/TBufPool API：`test_data_copy.cpp:46,51,56,126`、`test_data_copy_pad.cpp:55,60,81,193`。它没有生产环境队列分配/生命周期语义，不应把 CPU 单测写法直接复制到正式 Kernel。
- 配对/生命周期：本次代码未使用 `AllocTensor/FreeTensor/EnQue/DeQue`，因此无可检查的队列配对。正式 Kernel 若改用 TQue，必须保证 Alloc/Free 和 EnQue/DeQue 完整配对且总 Buffer 占用不超过实际 UB/L1 容量。

## 代际专属 API

### NDDMA DataCopy / NdDmaDci

- 仅在新代际路径使用；见上文 DataCopy 的 NDDMA 约束。
- 关键顺序：`NdDmaDci()` 刷新 NDDMA Cache → NDDMA `DataCopy`。缺少刷新在多核共享 GM 场景可能读取旧数据。

### DAV_3510 裸地址空间指针

- `__gm__`、`__ubuf__`、`__cbuf__`、`__cc__` 是编译器地址空间限定符，重载分派依赖限定符本身；普通 C++ 指针或错误的 `reinterpret_cast` 不能证明真实物理 Buffer 类型正确。
- Tensor 版本还能通过 `LocalTensor::GetPosition()` 分辨逻辑位置，裸指针没有此信息；因此显式逐通路重载比通用 `GetPhyAddr()` 适配更安全，但调用方必须承担位置、对齐、容量、同步和生命周期校验。
- 本次没有使用 `RegTensor`、`MaskReg`、`asc_vf_call`、`__simd_vf__`、`LoadAlign`、`StoreAlign` 等 SIMD-RegBase/SIMT API。

## 未匹配 API（代码中使用但不在核心清单中的）

- `ICachePreLoad`：指令 Cache 预取，位于原有 cache 头文件/检查用例中，本次指针化变更未修改其语义。
- `SetLoopModePara` / `ResetLoopModePara`：配置并复位搬运 LoopMode；必须成对恢复，现有 `test_data_copy_pad.cpp:67-71` 按 OUT→UB 和 UB→OUT 顺序设置/复位。
- `LocalTensor::GetPhyAddr`：测试中用来构造 `__ubuf__` 裸指针；仅返回物理地址，不携带位置和长度元数据。
- `GlobalTensor::SetGlobalBuffer`、`LocalTensor::SetAddr`、`InitOutput`：测试/张量初始化辅助接口，不属于本次新增 API。
- `DataCopy*Impl` / `DataCopyPad*Impl` / `Dcci*Impl`：内部 ISASI 封装入口，不是稳定公开 API；其架构约束仍必须由公开重载完整保留。

## 日落 API

- 已使用 `clause.get_sunset_api.py` 获取 CANN 9.2.0-beta.2 最新日落清单；本次文件未检测到 `aclrt*`、`aclnn*`、`acl.op.*` 日落符号，也未包含已日落的 `op_proto/inc` 头文件路径或 `libopapi.so` 链接项。

## 预研资料来源

- 本仓官方文档：`docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_*.md`、`DataCopyPad_*.md`，`cube_compute_ISASI/cube_compute_load/DataCopy*.md`，`cube_compute_ISASI/cube_compute_store/DataCopy*.md`，`cache_control/DataCache*.md`，`resource_management/*/InitBuffer.md`。
- 架构映射与能力：`npu-arch` 技能的 `references/npu-hardware-params.md`（产品映射、DAV_3510 Buffer 与数据通路差异）。
- 日落接口：`ascendc-code-review/scripts/clause.get_sunset_api.py`，拉取版本 CANN 9.2.0-beta.2。
