# 代码概要

算子: DMA Pointer API 扩展 | 功能: 为 DAV 3510 上的 `DataCopy` / `DataCopyPad` / `DataCopyL1ToUB` / Cache API 增加原生地址空间指针重载，并保持 Tensor 重载不变 | 侧别: Kernel 侧

## 代码脉络

**入口**: 用户 Kernel 调用 `AscendC::DataCopy` / `DataCopyPad` / `DataCopyL1ToUB` / `DataCachePreload` / `DataCacheCleanAndInvalid` → 公开头文件做重载解析 → DAV 3510 指针实现转发给架构底层 DMA/Cache primitive。触发条件是 `defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)`，证据见 `include/basic_api/kernel_operator_data_copy_intf.h:473-475`、`impl/basic_api/kernel_operator_data_copy_intf_impl.h:1892-1894` 和 Cache 接口的 `:55-67` / `:89-113`。

**数据流**:

`__gm__` ↔ `__ubuf__` / `__cbuf__` ↔ `__cc__` 原生指针 → 指针重载保留参数结构 → `DataCopy*Impl` / `DataCopyPad*Impl` / `Dcci*Impl` → MTE1/MTE2/MTE3 或 Cache 指令。指针路径不构造 `LocalTensor` / `GlobalTensor`，因此无 `GetPosition()` 动态分支，地址空间类型本身决定路径。

**计算核心**: 本改动不做数值计算，而是搬运/格式转换/补齐/Cache 维护。循环主要出现在 Slice 辅助函数 `DataCopyPtrSliceGm2Ub` (`impl/...ptr_intf_impl.h:90-136`) 和 `DataCopyPtrSliceUb2Gm` (`:146-192`)，循环语义是遍历展开后的 slice offset list，逐 slice 下发 DMA。

**分支覆盖**:

| 分支条件 | 位置(文件:行) | 触发场景 | 处理逻辑 | 涉及 API |
|---------|-------------|---------|---------|----------|
| `__NPU_ARCH__ == 3510` | `include/basic_api/kernel_operator_data_copy_intf.h:473` | Ascend 950/DAV 3510 编译 | 暴露指针宣言 | `DataCopy*` |
| `__NPU_ARCH__ == 3510` | `impl/basic_api/kernel_operator_data_copy_intf_impl.h:1892` | DAV 3510 实现 | 纳入指针实现 | `DataCopy*Impl` |
| `shapeValue != 0` | `impl/basic_api/kernel_operator_data_copy_ptr_intf_impl.h:94,150` | 指针 Slice 搬运 | 强制显式 shape，因指针无 Tensor shape 元数据 | `ASCENDC_ASSERT` |
| `srcOffsetListSize == dstOffsetListSize` | `...ptr_intf_impl.h:112,168` | Slice 展开后 | 防止源/目标 slice 数不等 | `ASCENDC_ASSERT` |
| stride 是 32B 倍数 | `...ptr_intf_impl.h:122,178` | Slice GM↔UB | 对齐时用常规 DMA，否则走 Slice/Pad primitive | `DataCopyGM2UBImpl`, `DataCopySlice*Impl` |
| `ASCEND_IS_AIC` | `...ptr_intf_impl.h:385,416,432` | UB→GM Pad/NDDMA 运行在 Cube Core | 直接返回，限定 AIV 路径 | `DataCopyPadUB2GMImpl`, `DataCopyWithNDDMAImpl` |
| `enableSmallC0` | `...ptr_intf_impl.h:62-67` | GM→L1 ND2NZ | 编译期选择 small-C0 | `DataCopyGM2L1ND2NZImpl` |
| `PaddingMode mode` | `...ptr_intf_impl.h:367-420` | Normal/Compact 补齐 | 编译期选择 stride/padding 语义 | `DataCopyPad*Impl` |
| Cache 指针仅 DAV 3510 | `kernel_operator_cache_intf_impl.h:89-113` | Cache pointer API | 按 GM/UB 地址空间路由到 DCCI | `DataCachePreloadImpl`, `DcciGMImpl`, `DcciUBImpl` |

**关键变量流转**:

| 变量 | 来源 | 用途 | 流转路径 |
|------|------|------|------|
| `dst` / `src` | Kernel 外部输入 | 搬运地址 | API 指针 → 架构 Impl → DMA intrinsic |
| `repeatParams` | Kernel 外部输入或 `DataCopyCheck` 构造 | block count/length/stride | `DataCopy` → `DataCopy*Impl` |
| `cacheMode` | 指针路径固定为 `0` | GM Cache 行为 | wrapper → DAV 3510 Impl |
| `srcShapeInfo` / `dstShapeInfo` | `SliceInfo[].shapeValue` | Slice 偏移展开 | shape → start index/offset list → DMA 循环 |
| `srcOffsetListSize` / `dstOffsetListSize` | `DataCopyGetOffsetList` | slice 数目与循环边界 | 初始化 0 → helper 填充 → assert → loop |
| `elementCount` | 测试常量 | count 模式验证 | 32 元素 round-trip / 5 元素 non-aligned pad |

**核心 API**: `DataCopy`, `DataCopyPad`, `DataCopyL1ToUB`, `DataCachePreload`, `DataCacheCleanAndInvalid`, `DataCopy*Impl`, `DcciGMImpl`, `DcciUBImpl`, `ASCENDC_ASSERT`。

**输出**: 目标指针所指 GM/UB/L1/L0C 内存或 Cache 状态。同步契约主要由 `__inout_pipe__(MTE2/MTE3)` 和底层 primitive 保证；本层无 `EnQue/DeQue`。

## 算子业务语义（Kernel 侧）

**数学运算**: 搬运不变量为“源有效元素在目标空间中保持值，仅格式转换/Pad 路径改变布局或填充边界”。输入输出是 1 个源地址到 1 个目标地址；Cache API 是 1 个目标地址到一次 cache 状态操作。

**计算模式**: Compile-Time Branch Dispatch。主信号是 `if constexpr`/模板参数 `enableSmallC0`、`PaddingMode mode`、`subBlockId`、`dim`、`NdDmaConfig`，外加编译期架构宏路由。

**同步契约**: GM→UB/L1 为 MTE2，UB/L1→GM 为 MTE3，L1↔L0C 由 MTE1/相应底层实现承担。没有 TQue，因此不存在 `EnQue/DeQue` 交接；也无 `CrossCoreSetFlag` / `WaitFlag` / `PipeBarrier`。

### 分支业务含义

| 分支条件 | 位置(文件:行) | 业务含义 | 处理逻辑 |
|---------|-------------|---------|---------|
| 32B 对齐 | `ptr_intf_impl.h:122,178` | 选择硬件快速常规 DMA 路径 | stride 换算为 32B 单位 |
| 非 32B 对齐 | `ptr_intf_impl.h:129,185` | 非对齐 slice 边界处理 | stride 保留字节语义并走 Slice Impl |
| `ASCEND_IS_AIC` | `ptr_intf_impl.h:385,416,432` | 该 DMA 只能在 Vector Core 生效 | AIC 返回 |
| `mode` | `ptr_intf_impl.h:367-420` | Normal 保留 pad/stride，Compact 连续布局 | 模板分发到 Pad Impl |

### 模板参数语义

| 参数 | 取值 | 业务含义 |
|------|------|---------|
| `T` / `U` | 支持的数据类型 | 源/目标元素类型和部分 L0C 转换组合 |
| `enableSmallC0` | `false/true` | ND2NZ/DN2NZ 是否开启 small-C0 |
| `PaddingMode mode` | `Normal/Compact` | 补齐与 stride 布局策略 |
| `subBlockId` | 默认 `0` | L1→UB 子块选择 |
| `dim` | 编译期维数 | NDDMA 循环维数 |
| `config` | `kDefaultNdDmaConfig` 或自定义 | NDDMA pad/轴融合等策略 |
| `entireType` / `dcciDst` | CacheLine / DcciDst 枚举 | Cache line 范围与 DCCI 目标 |

## Tiling 业务语义（Tiling 侧）

不适用。输入文件中未出现 `gert::TilingContext`、TilingData 结构或 `tilingData->field` 读取；本任务是 Basic API Kernel 头文件扩展，不是独立算子的 Host Tiling。

## 变量溯源

| 变量 | 声明(文件:行) | 初始化(文件:行) | 校验(文件:行) | 来源类型 |
|------|-------------|----------------|-------------|---------|
| `dst`, `src` | `include/...ptr_intf.h:19-177` | 调用者传入 | 由 C++ 地址空间类型和底层 Impl 约束；本 wrapper 无 null 校验 | 外部输入 |
| `repeatParams` | `ptr_intf_impl.h:21-59` | 调用者传入 | count 模式由 `DataCopyCheck` 做 32B 对齐校验 (`data_copy_intf_impl.h:786-842`) | 外部输入/辅助生成 |
| `shapeValue` | `SliceInfo` 字段，使用于 `ptr_intf_impl.h:94-105,150-161` | 调用者填充 | `ASCENDC_ASSERT(srcSliceInfo[0].shapeValue != 0)` | 外部输入 |
| `srcOffsetListSize` | `ptr_intf_impl.h:97,153` | `0` | 与 `dstOffsetListSize` 相等断言 `:112,168` | 内部辅助输出 |
| `cacheOffset` | `kernel_operator_cache_intf.h:57` | 调用者传入 | 底层 `DataCachePreloadImpl` 使用 `static_assert` 限定 `int16_t/int64_t` (`dav_3510/kernel_operator_cache_impl.h:31-37`) | 外部输入 |
| `elementCount` | `test_data_copy.cpp:114`; `test_data_copy_pad.cpp:186` | `32`; `5` | GTest 逐元素比对 `:132-134`; `:202-204` | 编译期常量 |
| `STRIDE_UPPER_BOUND` | `test_data_copy_pad.cpp:119` | `1ULL << 40` | 边界 -1 不 raise，边界值 raise (`:164-181`) | 编译期常量 |

## 函数清单

> 表中用“重载族”表示同名且只在地址空间/参数结构上变化的完整函数集；行范围覆盖该族的所有定义。

| 函数 | 签名 | 行范围 | 角色 |
|------|------|--------|------|
| `DataCopy` pointer basic | `(__ubuf__/__cbuf__/__gm__ T* dst, matching src, DataCopyParams)` 7 重载 | `ptr_intf_impl.h:20-60` | 入口/转发 |
| `DataCopy` pointer format | `Nd2NzParams` 3 重载; `Dn2NzParams` 1 重载; `Nz2NdParamsFull` 1 重载 | `:62-88,258-263` | 格式转换入口 |
| `DataCopyPtrSliceGm2Ub` | `(__ubuf__ T*, __gm__ T*, SliceInfo[], SliceInfo[], uint32_t)` | `:90-136` | Slice 核心辅助 |
| `DataCopy` GM→UB Slice | 同上，公开重载 | `:138-144` | 入口 |
| `DataCopyPtrSliceUb2Gm` | `(__gm__ T*, __ubuf__ T*, SliceInfo[], SliceInfo[], uint32_t)` | `:146-192` | Slice 核心辅助 |
| `DataCopy` UB→GM Slice | 同上，公开重载 | `:194-200` | 入口 |
| `DataCopy` pointer count | GM↔UB/L1、UB↔UB/L1 共 7 重载 | `:202-256` | Level-2 入口 |
| `DataCopy` pointer enhanced | `DataCopyParams + DataCopyEnhancedParams` 共 10 重载（含 L0C） | `:265-340` | Enhanced 入口 |
| `DataCopy` CO1→L1/GM | `DataCopyCO12DstParams` 2 重载 | `:342-352` | CO1 输出 |
| `DataCopyL1ToUB` | `DataCopyParams` / `count` 2 重载 | `:354-366` | L1→UB 入口 |
| `DataCopyPad` pointer | old/ext params，GM→UB/L1、UB→GM/L1，共 8 重载 | `:368-428` | 非对齐/填充入口 |
| `DataCopy` NDDMA pointer | `(__ubuf__ T*, __gm__ T*, MultiCopyParams<T,dim>)` | `:430-436` | NDDMA 入口 |
| Tensor `DataCopy` 族 | Tensor basic/format/slice/count/enhanced/pad/NDDMA 重载 | `kernel_operator_data_copy_intf_impl.h:42-1869` | 原有 API 入口 |
| `DataCopyCheck` | `(uint32_t count, DataCopyParams&)` | `data_copy_intf_impl.h:785-843` | count 对齐校验/参数生成 |
| `NdDmaDci` | `()` | `data_copy_intf_impl.h:1873` | NDDMA Cache 维护 |
| `SetLoopModePara` / `ResetLoopModePara` | `(LoopModeParams, DataCopyMVType)` / `(DataCopyMVType)` | `:1875-1890` | DMA Loop Mode 配置 |
| Cache Tensor 重载 | `DataCachePreload(GlobalTensor)`; `DataCacheCleanAndInvalid(Global/LocalTensor)` | `kernel_operator_cache_intf_impl.h:51-87` | 原有 Cache API |
| Cache pointer 重载 | `DataCachePreload(__gm__*)`; `DataCacheCleanAndInvalid(__gm__/__ubuf__*)` | `:90-113` | 新增 Cache API |
| `ICachePreLoad` / `GetICachePreloadStatus` | `(int64_t)` / `()` | `:115-117` | ICache 辅助 |
| `MainDataCopyKernel<T>` | `(__gm__ uint8_t*, __gm__ uint8_t*, int32_t, bool)` | `test_data_copy.cpp:29-65` | CPU 测试内核 |
| `DataCopyPointerSuite.GmUbRoundTrip` | GTest | `test_data_copy.cpp:112-136` | pointer 功能测试 |
| `MainDataCopyPadKernel<T>` | `(__gm__ uint8_t*, __gm__ uint8_t*, __gm__ int32_t)` | `test_data_copy_pad.cpp:34-81` | CPU 测试内核 |
| `ExpectSrcStrideRaiseCount` / `ExpectDstStrideRaiseCount` | `(int64_t,uint64_t)` | `test_data_copy_pad.cpp:136-162` | 边界检查辅助 |
| `DataCopyPadPointerSuite.NonAlignedGmUbRoundTrip` | GTest | `test_data_copy_pad.cpp:184-206` | pointer Pad 功能测试 |
| Cache header-check kernels | `KernelTestDataCachePreload1`, `...CleanAndInvalid1/3`, `...ICachePreLoad1`, `...PointerOverloads` | `kernel_operator_cache_intf.cpp:18-80` | 编译实例化入口 |
| DataCopy legacy header-check kernels | `KernelTestDataCopy1..26`, `KernelTestCopy1..3`, Pad/loop-mode kernels | `kernel_operator_data_copy_intf.cpp:19-424` | 原有编译覆盖 |
| DataCopy pointer header-check kernels | `KernelTestDataCopyPointerBasic/Format/Enhanced/Pad/SliceAndNdDma` | `kernel_operator_data_copy_intf.cpp:427-532` | 新增重载编译覆盖 |

## API 调用索引

| API | 行号 | 上下文 |
|-----|------|--------|
| `DataCopyGM2UBImpl` | `ptr_intf_impl.h:23,125,269` | GM→UB basic/slice/enhanced |
| `DataCopyGM2L1Impl` | `:29,277` | GM→L1 basic/enhanced |
| `DataCopyUB2GMImpl` | `:35,181,285` | UB→GM basic/slice/enhanced |
| `DataCopyL12GMImpl` | `:41,293` | L1→GM |
| `DataCopyUB2UBImpl` | `:47,309` | UB→UB |
| `DataCopyUB2L1Impl` | `:53,301` | UB→L1 |
| `DataCopyL12UBImpl` | `:59,317,356` | L1→UB |
| `DataCopyGM2L1ND2NZImpl` | `:66` | GM→L1 ND2NZ |
| `DataCopyGM2UBND2NZImpl` | `:73` | GM→UB ND2NZ |
| `DataCopyUB2L1ND2NZImpl` | `:80` | UB→L1 ND2NZ |
| `DataCopyGM2L1DN2NZImpl` | `:86` | GM→L1 DN2NZ |
| `DataCopySliceGm2UBImpl` / `DataCopySliceUB2GMImpl` | `:131,187` | 非对齐 Slice |
| `DataCopyUB2GMNZ2NDImpl` | `:261` | UB→GM NZ2ND |
| L0C data-copy Impl 族 | `:324,331,338,344,350` | L1/UB/L0C/GM 间 Enhanced/CO1 搬运 |
| `DataCopyPad*Impl` 族 | `:371,378,388,395,402,409,419,426` | old/ext Pad 路径 |
| `DataCopyWithNDDMAImpl` | `:435` | 多维 DMA |
| `DataCachePreloadImpl` | `kernel_operator_cache_intf_impl.h:55,93` | Tensor/pointer preload |
| `DcciGMImpl` / `DcciUBImpl` | `:68,74,85,99,105,111` | Cache clean/invalidate |
| `DataCopy` | `test_data_copy.cpp:60-64,129-130` | Tensor 原有路径与 pointer round-trip |
| `DataCopyPad` | `test_data_copy_pad.cpp:65,70-73,76,198-199` | Tensor 和 pointer 非对齐 round-trip |

## 常量清单

| 常量 | 值 | 位置(行) | 用途 |
|------|-----|---------|------|
| `K_MAX_SHAPE_DIM` | `8` | `impl/utils/sys_macros.h:53` | Slice shape 数组上限 |
| `MAX_SLICE_SIZE` | `6 * 256` | `impl/basic_api/utils/kernel_utils_constants.h:59` | Slice offset list 容量 |
| `ONE_BLK_SIZE` | `32` bytes | `kernel_utils_constants.h:253` | DMA block/对齐粒度 |
| `TOTAL_UB_SIZE` | `248 * 1024` | `kernel_utils_constants.h:174` (`__NPU_ARCH__ == 3510`) | DAV 3510 UB 可用容量 |
| `TOTAL_L1_SIZE` | `512 * 1024` | `kernel_utils_constants.h:176` | DAV 3510 L1 容量 |
| `TOTAL_L0C_SIZE` | `256 * 1024` | `kernel_utils_constants.h:179` | DAV 3510 L0C 容量 |
| `VECTOR_REG_WIDTH` | `256` | `kernel_utils_constants.h:180` | DAV 3510 vector register width |
| `AIV_CORE_NUM` | `72` | `kernel_utils_constants.h:277-278` | 3510/5102 默认 AIV 核数常量 |
| `nd2NzLimit` | `16384` | `data_copy_intf_impl.h:107` | ND2NZ n/stride 参数上限 |
| `elementCount` | `32` | `test_data_copy.cpp:114` | 32 个 int32，128B pointer round-trip |
| `elementCount` | `5` | `test_data_copy_pad.cpp:186` | 20B 非对齐 Pad round-trip |
| `STRIDE_UPPER_BOUND` | `1ULL << 40` | `test_data_copy_pad.cpp:119` | GM stride 边界 |
| `config` | `{false, unsetPad, unsetPad, false}` | `kernel_operator_data_copy_intf.cpp:528-529` | NDDMA header-check 编译期配置 |

## 跨文件防御摘要

| 关联文件 | 关键发现 | 位置(文件:行) | 影响范围 |
|---------|---------|-------------|---------|
| `kernel_operator_data_copy_intf.h` | 仅 DAV 3510 include pointer 公开宣言 | `:473-475` | 不影响其它架构 |
| `kernel_operator_data_copy_intf_impl.h` | 仅 DAV 3510 include pointer 定义 | `:1892-1894` | 与宣言宏对称 |
| `kernel_operator_data_copy_ptr_intf_impl.h` | Slice pointer 必须提供 shape，并校验 src/dst slice 数相等 | `:94-96,112-114,150-152,168-170` | 防止越界/错配搬运 |
| `kernel_operator_data_copy_intf_impl.h` | count 模式必须 `count*sizeof(T)` 32B 对齐 | `:786-842` | 所有 pointer count 重载复用 |
| `dav_3510/kernel_operator_data_copy_impl.h` | UB/L1 绝对地址和 tensor size 受容量断言保护 | `:33-50` | UB↔L1 底层路径 |
| `dav_3510/kernel_operator_data_copy_impl.h` | GM stride 上限检查 | `:1099-1115` | DataCopyPad Ext 路径 |
| `dav_3510/kernel_operator_cache_impl.h` | `cacheOffset` 仅支持 int16/int64 | `:31-37` | pointer/Tensor preload |
| `kernel_reg.h` | DCCI GM/UB 底层模板 | `:126-147` | Cache pointer 重载 |
| runtime tests | 32 元素 basic 和 5 元素非对齐 round-trip | `test_data_copy.cpp:112-136`; `test_data_copy_pad.cpp:184-206` | 值保真与 Pad 路径 |
| header checker | 新增 basic/format/enhanced/pad/slice/NDDMA/cache 指针实例化 | `kernel_operator_data_copy_intf.cpp:427-532`; `kernel_operator_cache_intf.cpp:67-80` | 重载解析和模板实例化 |

## TilingData 值域溯源

| TilingData 字段 | Host 侧计算(文件:行) | 公式 | 输入参数 | 约束 |
|---------------|---------------------|------|---------|------|
| 无 | 输入文件未使用 TilingData | 不适用 | 不适用 | 不适用 |

## 芯片架构参数

| 参数 | 值 | 来源 | 影响范围 |
|------|-----|------|---------|
| 架构 ID | `3510` | 公开/实现 include guard | pointer API 可见性 |
| AIV 核数常量 | `72` | `kernel_utils_constants.h:277-278` | 架构通用 AIV 管理；本 wrapper 不做多核切分 |
| UB | `248 KiB` | `kernel_utils_constants.h:174` | UB 地址/容量校验 |
| L1 | `512 KiB` | `kernel_utils_constants.h:176` | L1 地址/容量校验 |
| L0C | `256 KiB` | `kernel_utils_constants.h:179` | Enhanced/CO1 路径 |
| 对齐 | `32 B` | `ONE_BLK_SIZE`, `kernel_utils_constants.h:253` | count 模式、stride 换算、Slice 快慢路径 |
| Cache line | `64 B` | `kernel_utils_constants.h:178` | DCCI/Cache 操作 |

## 代码关联

**上游文件**:

| 文件路径 | 关联方式 | 依据 |
|----------|----------|------|
| `include/basic_api/kernel_operator_data_copy_intf.h` | 公开入口 | 包含 pointer 宣言并继续 include impl |
| `include/basic_api/kernel_operator_data_copy_ptr_intf.h` | 公开重载宣言 | DAV 3510 下宣言 40+ 指针重载 |
| `include/basic_api/kernel_struct_data_copy.h` | 参数结构 | `DataCopyParams`, `SliceInfo`, NDDMA 结构 |
| `include/basic_api/kernel_operator_cache_intf.h` | Cache 公开入口 | DAV 3510 指针宣言 |
| `kernel_operator.h` | 测试聚合头 | runtime/header-check 默认 include |

**下游文件/API**:

| 文件路径/API | 关联方式 | 依据 |
|----------|----------|------|
| `impl/basic_api/dav_3510/kernel_operator_data_copy_impl.h` | 架构底层 | `kernel_operator_data_copy_base_impl.h:44` 按 3510 include |
| `impl/basic_api/dav_3510/kernel_operator_cache_impl.h` | Cache 底层 | `kernel_operator_cache_intf_impl.h:39-40` |
| `impl/basic_api/kernel_reg.h` | DCCI 底层 | Cache impl 调用 `DcciGMImpl` / `DcciUBImpl` |
| DMA intrinsic | 硬件 API 依赖 | DAV 3510 Impl 内的 `copy_gm_to_ubuf_align_v2` 等 |

## 高性能设计（仅 Kernel 侧）

**流水线模式**: Compile-Time Branch Dispatch + 专用 DMA 管线。该 API wrapper 本身无 vector compute 流水和队列 buffer。

**流水线设计**:

| 机制 | 状态 | 设计意图 |
|------|------|----------|
| EnQue/DeQue 同步 | 无 | 本层是直接指针 DMA，不管理 TQue |
| MTE2/MTE3 pipe annotation | 有 | 向编译器表达 GM→local / local→GM 管线 |
| Buffer 管理模式 | 无自有 Buffer | 使用调用者指针，避免 wrapper 分配开销 |
| 对齐快慢路径 | 有 | 32B 对齐走常规 DMA，否则走 Slice/Pad Impl |

**切分策略**:

| 维度 | 切分方式 | 每核处理量 | 切分粒度 |
|------|----------|------------|---------|
| 多核切分 | 本 API 不切分 | 由调用者指针/参数决定，源码无固定值 | 不适用 |
| Slice 切分 | offset list 逐项搬运 | `srcOffsetListSize` 项 | 单个 slice |
| count 切分 | `count / elementsPer32B` 生成 blockLen | 调用者提供 | 32B |

**Buffer 规划**:

| Buffer | 类型 | 大小(B) | 用途 |
|--------|------|------|------|
| `srcShapeInfo` / `dstShapeInfo` | 栈上定长数组 | `8 * 4 = 32` 各自 | Slice shape |
| `srcOffsetList` / `dstOffsetList` | 栈上定长数组 | `1536 * 4 = 6144` 各自 | Slice 展开 offset |
| DMA 源/目标 | 外部 GM/UB/L1/L0C | 本层不分配 | 实际数据搬运 |

## 跨文件关系（多文件）

| 关系类型 | 源文件 | 目标文件 | 内容 | 位置 |
|---------|--------|---------|------|------|
| include | `kernel_operator_data_copy_intf.h` | `kernel_operator_data_copy_ptr_intf.h` | DAV 3510 pointer 公开宣言 | `:473-475` |
| include | `kernel_operator_data_copy_intf_impl.h` | `kernel_operator_data_copy_ptr_intf_impl.h` | DAV 3510 pointer 定义 | `:1892-1894` |
| include | `kernel_operator_data_copy_base_impl.h` | `dav_3510/kernel_operator_data_copy_impl.h` | DAV 3510 DMA 底层 | `:43-45` |
| 函数调用 | pointer impl | DAV 3510 data-copy impl | 重载转发到 `DataCopy*Impl` | `ptr_intf_impl.h:21-435` |
| 函数调用 | cache pointer impl | DAV 3510 cache impl / `kernel_reg.h` | preload / DCCI | `cache_intf_impl.h:91-111` |
| 测试覆盖 | header checker | 公开头文件 | 编译实例化全类别指针重载 | `data_copy_intf.cpp:427-532`; `cache_intf.cpp:67-80` |
| 测试覆盖 | AIV basic GTest | pointer impl | basic/Pad round-trip 值验证 | `test_data_copy.cpp:112-136`; `test_data_copy_pad.cpp:184-206` |

## 侧别识别结论

- 侧别: **Kernel 侧**。
- 证据: 大量 `__aicore__`、`__global__`、`__gm__/__ubuf__/__cbuf__/__cc__`、`__inout_pipe__(MTE2/MTE3)` 和 DAV 3510 硬件 primitive；没有 `gert::TilingContext`或 Host tiling 函数。
- 模式: Basic API wrapper，不是独立算子 kernel 类；以编译期重载分发和底层 DMA 管线为主。
