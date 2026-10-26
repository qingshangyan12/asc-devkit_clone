# AICPU MC2 高阶 API A5 性能优化方案

## 背景

本方案只针对 A5（Ascend 950PR/950DT，NPU 架构 `dav-3510`）的 AICPU MC2 路径，目标是降低
`InitV2`、`Prepare`、`Commit`、`Wait`、`Finalize` 的固定开销。优化原则是保守小改，不改消息协议、
不改 server 端处理语义、不改变现有 API 返回行为。

A5 AICPU 路径的关键事实：

- `__NPU_ARCH__ == 3510` 时，`hccl_impl.h` 选择 `platform_v310/hccl_aicpu.h`。
- A5 `InitV2` 不再读取 `queueNum`，`HcclImpl` 的 `queueNum_` 使用类内默认值 `0`，即只走
  `singleMsg` 路径。
- `InterHcclGroupSync` 在 A5 AICPU 实现中为空函数；`srcGroupID/srcHandleID` 只在 A2
  (`platform_v220`) 的组间同步消息中实际使用。
- A5 AICPU 消息组装走 `AssembleHcclMsgV2`，不使用 A2 的函数内静态序列号状态。

## 涉及文件

- `impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h`
- `impl/adv_api/detail/hccl/common/hccl_aicpu_def.h`
- `impl/adv_api/detail/hccl/common/hccl_utils.h`
- `impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h`

## 代码入口与性能影响总览

影响程度按当前实现的固定开销估计：

- **高**：引入多次 GM flush/轮询，或随 rank/handle 数量线性增加；直接进入同步关键路径。
- **中**：每次调用固定增加若干分支、数组访问或 1-2 次 flush；在 API 被高频调用时累计明显。
- **低**：少量标量赋值或分支，单次影响小，但有助于减小代码体积和固定指令数。

| 优化点 | 需要修改的代码片段 | 当前方式是否影响性能 | 影响程度 | 判断依据 |
|---|---|---|---|---|
| InitV2 去掉 A5 不涉及赋值 | [`platform_v310/hccl_aicpu.h:30`](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h) | 会 | 低 | `devType_` 在 A5 AICPU 后续路径无读取，属于无效标量赋值；单次开销小，但增加指令和状态。 |
| InitV2 异常分支 ASSERT 化 | [`platform_v310/hccl_aicpu.h:33`](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h) | 会 | 低 | 多个手写 `if` 留在 release 路径；正常调用虽不进入分支，但仍有比较和跳转指令。 |
| Init 数组初始化地址写入 | [`hccl_aicpu_impl.h:116`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 中 | 63 次 `int8_t` 写加循环控制；改成 8 次 64 位写可减少循环和访存指令。 |
| Prepare type 模板化 | [`hccl_aicpu_impl.h:287`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 中 | `commType` 在入口已知，但当前仍运行时判断 dtype、AlltoAllV 和 tiling 表；模板化后非 AlltoAllV 可消掉多条分支和查表。 |
| Prepare 去掉 queueNum 判断 | [`hccl_aicpu_impl.h:138`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 低到中 | A5 恒为单队列，但 SendMsg/Prepare/Finalize 多处重复判断；单次分支小，累计可减固定指令。 |
| srcGroupID 判断裁剪 | [`hccl_aicpu_impl.h:157`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 低 | A5 `InterHcclGroupSync` 为空实现，`srcGroupID` 分支在 A5 不可达；删除后少一次分支和旧组装路径引用。 |
| 通用 FlushDataCache | [`hccl_utils.h:19`](./impl/adv_api/detail/hccl/common/hccl_utils.h) | 会 | 高 | 每次调用都是 Barrier + DCCI + DSB；当前多处按字段重复 flush，同步原语次数是主要固定开销。 |
| AlltoAllV 扩展消息 flush | [`hccl_aicpu_impl.h:178`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 高（随 rank 增长） | 四个数组分段 flush；rank 越大 flush 次数越多，且每次都带同步原语。 |
| Commit apiStats 去除 | [`hccl_aicpu_impl.h:240`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 中 | 每次 Commit 在必要发布后又做 2 次 `FlushDataCache`，可能使同步原语数量接近翻倍。 |
| Commit stepSize 裁剪 | [`hccl_aicpu_impl.h:501`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 有条件 | 中（仅 AlltoAllV） | 非 AlltoAllV 很快返回；AlltoAllV 会读 tiling。若确认 A5 不使用 stepSize，可减少 GM 读和分支。 |
| Wait waitStats 去除 | [`hccl_aicpu_impl.h:460`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 中 | 每次 Wait 前额外 2 次 flush，然后再进入真正完成计数轮询；高频 Wait 累计开销明显。 |
| Wait 完成计数轮询 | [`hccl_aicpu_impl.h:268`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 高，但本方案暂不改语义 | 每轮含 A5 1000-cycle throttle 和 DCCI/DSB，是同步延迟的主要来源；第一批仅统计，不改协议。 |
| Finalize 单队列化 | [`hccl_aicpu_impl.h:342`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 低 | A5 队列数恒为 1，当前仍有队列数计算和循环；主要减少固定分支，收益小于 DFX 删除。 |
| Finalize msgStats/snapshots 去除 | [`hccl_aicpu_impl.h:528`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 会 | 中 | `msgStats` 额外 flush；snapshots 随 handle 数量循环，最多 63 次，且每项 flush，多 Prepare 场景开销明显。 |
| `handleId2CurrSliceId_` 删除 | [`hccl_aicpu_def.h:125`](./impl/adv_api/detail/hccl/common/hccl_aicpu_def.h) | 会 | 低 | 当前 AICPU 实现未见引用；主要收益是减小对象布局和初始化成本。 |

需要特别注意：`Wait` 的完成计数轮询和消息槽空闲等待虽然影响最大，但它们承担同步语义。本方案第一批只裁剪
A5 明确不可达的分支和 DFX 写入，不改消息协议、不改 server 交互次数，避免引入功能风险。

## 修改方案

### 1. InitV2

代码入口：

- [InitV2 当前实现](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h)
- [InitInner 数组初始化](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

#### 1.1 异常分支放入 ASSERT 宏

代码链接：

- [InitV2 context 空指针检查](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h)
- [InitV2 opType 范围检查](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h)
- [InitInner 重复初始化检查](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [InitInner msgAddr 空值和对齐分支](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

`InitV2` 和 `InitInner` 中以下条件属于调用方错误或内部不变量，不需要在 release 模式下保留
分支判断：

- `context == nullptr`
- `msgAddr == 0`
- `msgAddr` 未按 512B 对齐（保留对齐计算本身）
- `opType >= HCCL_CMD_ALL`
- 重复初始化

处理方式：

- 将手写 `if + ASCENDC_HCCL_API_ASSERT` 改成统一的
  `ASCENDC_HCCL_API_ASSERT(cond, return, ...)`。
- 对 `msgAddr == 0` 这类当前已经直接 `return` 的分支，改为 ASSERT 后仍保留相同返回行为：
  debug/CPU UT 下返回，release 下由调用方保证不变量。
- 不修改 512B 对齐计算逻辑。

#### 1.2 去除 A5 不涉及的变量赋值

代码链接：

- [InitV2 中的 devType_ 赋值](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h)
- [queueNum_ 成员定义](./impl/adv_api/detail/hccl/common/hccl_aicpu_def.h)
- [devType_ 成员定义](./impl/adv_api/detail/hccl/common/hccl_aicpu_def.h)
- [设备侧 OpResCtx 定义](./include/adv_api/hccl/internal/hccl_msg.h)
- [host 侧 OpResCtx 定义](./impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.h)
- [AICPU server 消费 OpResCtx](./impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/device/aicpu_kfc/decoupler/comm_kfc_aicpu_server.cc)

在 3510 专用实现中删除以下赋值：

- `devType_`：A5 AICPU 路径没有后续读取。
- `queueNum_`：A5 默认值即为 0，且不会从 tiling 赋值。
- 旧版本相关状态：仅当 A5 单独实现后确认无引用再删除成员，不影响 A2/A3。

补充结论：`OpResCtx` 本身可以压缩，但不建议直接修改公共结构。原因是该结构同时服务 CCU 与
AICPU server：CCU 仍使用 `xnAddr/ckeAddr/res[0]`，AICPU server 使用 `workspace/rankSize/`
`algInfo/opParamSize`。如果直接删公共字段，会破坏 CCU 和 AICPU server 的兼容性。

建议为 A5-AICPU 定义专用瘦上下文，例如 `OpResCtxA5Aicpu`，并保留一个版本/魔数字段用于识别。
公共 `OpResCtx` 继续保留给 CCU 与兼容路径。

A5-AICPU 链路当前实际需要的字段：

| 字段 | A5-AICPU API 侧 | A5-AICPU server 侧 | 说明 | 可否压缩 |
|---|---|---|---|---|
| `version` | 可不读 | 可不读，建议保留为类型识别字段 | 用于区分瘦上下文与通用上下文 | 保留，可缩成 32 位 |
| `workspace` | `InitV2` 读取 | server 建消息区 | 消息区地址 | 必须保留 |
| `workspaceSize` | A5 API 不读 | server 不直接读 | 目前主要用于校验/日志 | 可移除或缩成 32 位 |
| `rankId` | `GetRankId()` 读取 | 不直接读 | rank 标识 | 可缩成 32 位 |
| `rankSize` | `GetRankDim()` 读取 | server 校验和消息区初始化 | rank 数 | 可缩成 32 位 |
| `algInfo[8]` | `InitV2/SetCcTilingV2` 读取 | server 遍历 8 项 | 当前真正支持匹配的是 2 项，A5 场景可先缩成 2 项 | 高收益 |
| `xnAddr` | A5 API 不读 | A5 AICPU server 不读 | CCU 使用 | A5-AICPU 可删 |
| `ckeAddr` | A5 API 不读 | A5 AICPU server 不读 | CCU 使用 | A5-AICPU 可删 |
| `sprAddr` | 不读 | 不读 | 预留 | A5-AICPU 可删 |
| `res[8]` | 不读 | 不读 | CCU/预留；AICPU server 未访问 | A5-AICPU 可删 |
| `resCtx` | 不读 | 不读 | 资源下发预留 | A5-AICPU 可删 |
| `opType[8]` | 无 tiling Init 时读 `opType[0]` | 不读 | A5-AICPU 只需要 1 项 | 高收益 |
| `algorithmType[8]` | 不读 | 不读 | 选择器/CCU 使用 | A5-AICPU 可删 |
| `opParamSize[8]` | 不读 | server 读取 `opParamSize[i]` | 需要与 `algInfo` 数量一致 | 可缩成 2 项 |
| `isKfc[8]` | 不读 | 不读 | CCU/KFC 选择使用 | A5-AICPU 可删 |
| `commEngine` | 不读 | 不读 | launch 侧通过通用 `OpResCtx` 判断 | 若 launch 改为按版本/上下文类型分发，可删 |

推荐 A5-AICPU 瘦上下文布局：

`cpp
struct OpResCtxA5Aicpu {
    uint32_t magic;       // 或 version
    uint32_t workspaceSize;
    uint64_t workspace;
    uint32_t rankId;
    uint32_t rankSize;
    uint32_t algNum;      // 当前固定 2
    uint32_t reserved;
    AlgInfoCompact algInfo[2]; // offset 可视 tiling 地址宽度决定是否缩成 uint32_t
    uint64_t opParamSize[2];
};
`

预估大小：当前通用 `OpResCtx` 为 408 字节（0x198）；上述布局约 80 字节（0x50），可减少约 80%。
如果把 `magic/version` 保持为 64 位或增加更多校验字段，约在 88-96 字节，仍有约 76%-78% 的缩减。

性能影响：

- `InitV2` 本身只读 `workspace`，直接减少 GM 读的效果有限；主要收益是缩小设备内存占用和
  host/device 拷贝量。
- 无 tiling 的 `InitV2` 还会读 `opType[0]` 和 `algInfo[0]`；当前这两个字段分别位于结构尾部和
  中部。瘦上下文可以把 `workspace/rank/opType/algInfo` 排列到相邻位置，改善 GM 读局部性。
- AICPU server `AddOpContext` 当前固定遍历 8 个 `algInfo`；瘦上下文把遍历缩到 2 项，可减少
  server 初始化时的空项检查。
- `SetCcTilingV2` 当前遍历 `HCCL_ALG_SUPPORT_NUM=2`，该路径收益不大。
- `Mc2CcKernelLaunch` 目前按 `sizeof(OpResCtx)` 做 D2H 拷贝；如果 A5-AICPU 使用瘦上下文，可减少
  一次拷贝的数据量，对 launch 前置耗时有一定收益。

实施约束：

1. 不能只改设备侧结构。host 侧 `HcclAllocOpResCtx`、AICPU server、launch 路径和 UT 必须同步识别
   新布局。
2. `Mc2CcKernelLaunch` 当前通过 `commEngine` 分发 AICPU/CCU。引入瘦上下文后建议改为按
   `version/magic` 识别上下文类型，避免依赖已删除的 `commEngine` 字段。
3. 资源缓存 key 与序列化逻辑仍使用 `algInfo[0].opParam`，需要确认瘦上下文的 key 字段保持一致。
4. A5-AICPU 专用结构建议新增，不修改公共 `OpResCtx`，避免影响 CCU 和 960。

#### 1.2.1 公共 OpResCtx 的裁剪边界

代码链接：

- [CCU InitV2 读取 workspace/xnAddr/ckeAddr](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_ccu_v0.h)
- [CCU GetAlgorithmType 读取 opType/algorithmType](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_ccu_v0.h)
- [CCU scratch/dieSplit/missionCnt 读取 res[0..2]](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_ccu_v0.h)
- [AICPU server 读取 opParamSize](./impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/device/aicpu_kfc/decoupler/comm_kfc_aicpu_server.cc)
- [launch 读取 commEngine 分发](./impl/adv_api/detail/hccl/cc/src/common/hccl_mc2.cc)

如果把公共 `OpResCtx` 截断到 `sprAddr`，CCU 和 AICPU server 会直接功能受损，因此不能这样裁剪：

- `xnAddr`：CCU 写 XN 消息、准备队列时使用。
- `ckeAddr`：CCU 计算 commit/wait CKE 地址时使用。
- `sprAddr`：全仓未发现真实读写，可以删除。
- `res[0]`：CCU scratch 地址。
- `res[1]`：CCU die split 千分比。
- `res[2]`：CCU KFC mission 数。
- `opType[]`：CCU `GetAlgorithmType` 按 prepareType 匹配。
- `algorithmType[]`：CCU 根据匹配结果选择 mission 策略。
- `opParamSize[]`：AICPU server 加载 OpParam 时使用。
- `commEngine`：`Mc2CcKernelLaunch` 用它区分 AICPU/CCU launch。

公共结构中可以考虑清理的字段：

- `sprAddr`：未使用。
- `resCtx`：`OpResCtx::resCtx` 未发现真实读写；注意不要与 `OpParam::resCtx` 混淆，后者仍被使用。
- `isKfc[]`：`OpResCtx::isKfc` 只在 host 写入、日志和 UT 中出现；设备侧实际判断使用的是
  `OpParam::isKfc`。清理日志和 UT 后可删，但属于兼容性变更。

因此：

- **A5-AICPU 专用结构**：可以不保留 `xnAddr/ckeAddr/sprAddr/res[]`，只保留 API 与 AICPU server
  需要的字段。
- **公共 `OpResCtx`**：不能截断到 `sprAddr`；至少还要保留 `res[0..2]`、`opType[]`、
  `algorithmType[]`、`opParamSize[]` 和 `commEngine`。

建议实现方式：

- 在 `platform_v310/hccl_aicpu.h` 中为 3510 增加 A5 专用的 `InitV2A5` 或直接将现有
  `InitV2` 包在 `#if __NPU_ARCH__ == 3510` 内实现为 A5-only 版本。
- A5-only 版本只保留：
  1. 读取 `debugMode_`；
  2. 记录 `tilingBaseAddr_`；
  3. 设置 `hcclContext_`；
  4. 调用精简版 `InitInner`。

#### 1.3 数组初始化改为地址写入

代码链接：

- [handleIdMsgPosition_ 63 次循环初始化](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [handleIdMsgPosition_ 数组定义](./impl/adv_api/detail/hccl/common/hccl_aicpu_def.h)

`InitInner` 中 `handleIdMsgPosition_[HCCL_MAX_HANDLE_ID]` 初始化为 `-1`，当前为 63 次循环。

建议新增设备侧小工具：

`cpp
__aicore__ inline void FillInt8(__gm__ void* dst, int8_t value, uint32_t count)
`

或直接按 `int64_t` 视图写入：

- 63 个 `int8_t` 可视为 8 个 `int64_t`。
- 首个 56 字节写入 `0xFFFFFFFFFFFFFFFF`。
- 尾部 7 字节单独写 `0xFF`，或用掩码写入，避免越界。

实现要求：

- 不引入越过数组长度的写。
- `HCCL_MAX_HANDLE_ID` 改变时逻辑仍成立，建议用 `static_assert(HCCL_MAX_HANDLE_ID == 63)` 锁定，
  或用 `count / 8` 和 `count % 8` 生成通用实现。
- A2/A3 保持原循环实现，A5 才切换。

### 2. Prepare

代码入口：

- [CommonPrepareImpl](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [SendMsgToServer](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [AlltoAllV 扩展消息写入](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

#### 2.1 type 改为模板参数

代码链接：

- [CommonPrepareImpl 运行时 commType 判断](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [AlltoAllV 运行时分支](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [SendMsgToServer 内的 commType 查表](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

当前 `CommonPrepareImpl` 通过 `param.commType.prepareType` 做运行时判断，A5 支持的 prepare 入口
都能在编译期确定类型：

- `AllReduce`
- `AllGather`
- `ReduceScatter`
- `AlltoAll`
- `AlltoAllV`

建议把公共实现改为：

`cpp
template <bool commit, HcclCMDType cmdType>
__aicore__ inline HcclHandle CommonPrepareImpl(...)
`

入口函数显式传入类型：

`cpp
return CommonPrepareImpl<commit, HcclCMDType::HCCL_CMD_ALLREDUCE>(...);
`

A5 分支中用 `if constexpr` 处理：

- `cmdType == HCCL_CMD_ALLTOALLV`：保留扩展消息写入。
- 其他类型：跳过 `SendMsgToServer(param.paramExt)` 和扩展参数校验。
- `cmdType == HCCL_CMD_BATCH_WRITE`：A5 MC2 不支持，保持现有不支持语义，不进入模板特化。

#### 2.2 A5 单独实现，去掉 queueNum 判断

代码链接：

- [SendMsgToServer queueNum 判断](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [CommonPrepareImpl queueNum 判断](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [SendFinalizeMsg queueNum 判断](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

A5 中 `queueNum_ == 0` 恒成立。新增 A5 专用 `SendMsgToServerA5` / `CommonPrepareImplA5`：

- 消息地址固定使用 `singleMsg.sendMsgs + curMsgPosition_[0]`。
- 不判断 `queueNum_`。
- `queId` 固定为 0。
- 不访问 `multiMsg`。
- `Finalize` 中 `totalQueNum` 固定为 1。

A2/A3 继续使用原实现，保证兼容。

#### 2.3 srcGroupID 只保留参数，不做判断

代码链接：

- [srcGroupID 运行时分支](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [A5 InterHcclGroupSync 空实现](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h)

确认结果：A5 `InterHcclGroupSync` 是空实现，A5 Prepare 路径不会传入有效 `srcGroupID`。

A5 专用 `SendMsgToServer` 中：

- 保留函数参数 `srcGroupID/srcHandleID`，保持调用签名和 ABI 不变。
- 删除 `srcGroupID < 0` 运行时判断。
- 始终走普通消息组装路径。
- `AssembleHcclMsg(param, srcGroupID, srcHandleID, dst)` 的组间同步分支只保留在 A2 实现。

### 3. FlushDataCache

代码入口：

- [当前 FlushDataCache 实现](./impl/adv_api/detail/hccl/common/hccl_utils.h)
- [A5 AICPU 轮询 flush](./impl/adv_api/detail/hccl/common/hccl_utils.h)

当前每次 `FlushDataCache` 都执行：

1. `AscendC::Barrier()`
2. `NOP`
3. `DataCacheCleanAndInvalid`
4. `DataSyncBarrier<ALL>`

参考 asc-comm 的做法：

- 按 cache line 批量执行 `DataCacheCleanAndInvalid`，避免一个对象按字段多次 flush。
- A5 轮询读使用 `ReadGmBypassDCache`，并用 `Nop` 做 backoff，避免高频读请求造成总线压力。

建议拆分场景：

#### 3.1 消息写发布

代码链接：

- [当前 FlushDataCache 同步原语](./impl/adv_api/detail/hccl/common/hccl_utils.h)
- [HcclMsg 发布](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [TurnCnt 发布](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [ResetFinishedTurnCnt 逐项 flush](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [AlltoAllV 四个数组逐段 flush](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

新增或改造 A5 专用 `CacheWriteThrough`：

- 输入：GM 地址和长度。
- 按 64B cache line 对齐起止地址。
- 对覆盖的 cache line 执行 `DataCacheCleanAndInvalid`。
- 一批连续字段只做一次 flush，例如 `TurnCnt`、`HcclMsg`。

适用位置：

- `SendMsgToServer` 发布 `HcclMsg`。
- `SetCommitTurnCntToGm` 发布 `TurnCnt`。
- `ResetFinishedTurnCnt` 批量清理。
- AlltoAllV 扩展消息的四个数组，可按每次 `MAX_DCCI_CNT` 覆盖范围合并成一次或少量几次 flush。

#### 3.2 轮询读

代码链接：

- [A5 AICPU 轮询 FlushDataCacheAicpu](./impl/adv_api/detail/hccl/common/hccl_utils.h)
- [WaitFinishCntFromGm 轮询循环](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

A5 保留现有 `FlushDataCacheAicpu` 的 1000 NOP throttle，避免总线 hang。

不建议第一批直接把 `WaitFinishCntFromGm` 改成 `ReadGmBypassDCache`，因为该函数是同步语义核心路径。
建议第一批只做统计确认：

- 记录单次 Wait 的循环次数和耗时。
- 如实测轮询开销占比较高，再评估使用 asc-comm 风格的
  `ReadGmBypassDCache + Nop<800/1000> + DataSyncBarrier` 替换。

### 4. Commit

代码入口：

- [Commit](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [SetCommitTurnCntToGm 与 apiStats](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

#### 4.1 报错判断放入 ASSERT 宏

代码链接：

- [Commit 初始化状态检查](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [Commit handleId 范围检查](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [Commit 超次数检查](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

以下判断改为 `ASCENDC_HCCL_API_ASSERT`：

- `handleId` 范围检查。
- `commitCnt` 超过 `repeat` 允许次数。
- 未初始化时的调用顺序检查。

release 模式下这些分支消失，正常调用路径只保留计数更新和发布。

#### 4.2 去掉 stepSize

代码链接：

- [GetStepSizeByHandle 实现](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [Commit 中的 stepSize 读取](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [Wait 中的 stepSize 读取](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

A5 AICPU 路径的 [`GetStepSizeByHandle`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
只在 `AlltoAllV` 场景读取 tiling 的 `stepSize`，且在
[`Commit`](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
和 `Wait` 中各查一次。若当前 A5 优化范围只覆盖 MC2 主集合通信且不启用该 step 语义，则 A5 专用 `Commit` 中：

- 固定每次 Commit 增量为 1。
- 删除 `GetStepSizeByHandle(handleId)` 调用。

约束：如果 A5 `AlltoAllV` 会使用 `stepSize != 0` 的 tiling，则不能删除；需要先通过 tiling 生成
逻辑和用例确认。确认不使用后再删。

#### 4.3 去掉 apiInfo 统计

代码链接：

- [SetCommitTurnCntToGm 中的 commitStats 双 flush](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

A5 专用 `SetCommitTurnCntToGm` 删除：

`cpp
__gm__ TurnCnt* apiInfo = ...commitStats[...];
FlushDataCache(apiInfo);
apiInfo->cnt += turnCnt;
FlushDataCache(apiInfo);
`

保留：

- `handleIdCommitTurnCnt_[handleId] += turnCnt`
- 等待 server 清理旧 commit 计数
- 写 `commitGM->cnt`
- 写 `commitGM->valid`
- 发布 `commitGM`

### 5. Wait

代码入口：

- [Wait 与 waitStats](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [WaitFinishCntFromGm 轮询](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

#### 5.1 去掉 waitStats DFX

代码链接：

- [Wait 中的 waitStats 双 flush](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

A5 专用 `Wait` 删除：

`cpp
if (workingFlag_) {
    __gm__ TurnCnt* apiInfo = ...waitStats[...];
    FlushDataCache(apiInfo);
    ++apiInfo->cnt;
    FlushDataCache(apiInfo);
}
`

保留：

- handle 合法性 ASSERT。
- wait 调用次数检查。
- `handleIdWaitCallNum_` 更新。
- `WaitFinishCntFromGm` 同步等待。
- 返回值语义。

### 6. Finalize

代码入口：

- [SendFinalizeMsg](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [Finalize 与 msgStats/snapshots](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [ResetFinishedTurnCnt](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

#### 6.1 queueNum 单队列化

代码链接：

- [Finalize 中 queueNum 分支](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [SendFinalizeMsg 中 totalQueNum 计算](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [Wait 中 queueNum 提前返回](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)

A5 中 `queueNum_ == 0`，因此：

- `totalQueNum` 固定为 1。
- 非工作核分支固定只递增 `curMsgPosition_[0]`。
- 删除 `!workingFlag_ && queueNum_ == 0` 中的 `queueNum_` 判断。

#### 6.2 去掉 msgStats 与 snapshots DFX

代码链接：

- [Finalize 调用 UpdateControlMsgCount](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [Finalize snapshots 循环写入](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [UpdateControlMsgCount 实现](./impl/adv_api/detail/hccl/common/hccl_aicpu_common.h)

删除以下 DFX：

- `UpdateControlMsgCount` 调用（`msgStats`）。
- `apiStats.snapshots` 循环写入。

保留：

- sync 模式下等待最后一个任务完成。
- 发送 Finalize 消息。
- 等待 server 写入 `FINALIZE_FINISH_CNT`。
- `ResetFinishedTurnCnt`。
- 消息位置递增和上限 ASSERT。

## 同粒度的补充候选

以下项与上述修改同粒度，建议一起评估：

1. **Prepare 参数校验模板化**：`CheckCommonPrepareParamValid` 也按 `cmdType` 模板化。非
   AlltoAllV 类型跳过 rank 循环和扩展指针校验；dtype 校验用 `DATA_TYPE_MAP` 的
   `static_assert` 或模板类型派生，减少运行时查表。
2. **消息槽空闲检查前移**：A5-only Prepare 在写本地参数前先检查目标消息槽是否空闲；若忙则提前返回
   INVALID_HANDLE_ID 或进入等待，避免先更新 handle 状态再阻塞。注意该改动会改变忙时副作用，
   需单独确认。
3. **重复 tiling 查找去重**：`CheckCommonPrepareParamValid`、`SendMsgToServer` 和
   `GetStepSizeByHandle` 都可能按 `commType` 查表。A5-only 路径在 `CommonPrepareImpl` 开头查一次，
   后续作为参数传递。
4. **AlltoAllV 扩展消息 flush 合并**：当前四个数组逐段 flush。按 64B cache line 计算覆盖范围，
   使用批量 `CacheWriteThrough`，可减少 DCCI 次数。
5. **ResetFinishedTurnCnt 批量化**：当前按消息位置循环写并逐项 flush。A5 可按连续 cache line
   批量写和批量 flush，长度从 `0..curMsgPosition_[0]` 扩展到覆盖整段的最小 cache line 对齐范围。
6. **DEBUG/INFO 日志开关**：热路径中的 `KERNEL_LOG(KERNEL_INFO, ...)` 改成编译期开关，release
   不生成格式化参数和分支。
7. **调试模式检查缓存**：`debugMode_` 在 `HCCL_ONLY_COMPUTE` 时会跳过等待。若一个 kernel 内多次
   Commit/Wait，可把判断结果缓存在局部变量或成员位标志中，减少重复 GM 侧状态读取。
8. **`handleId2CurrSliceId_` 删除**：AICPU 实现中未见引用，确认后可从 A5 类布局中移出或删除，
   减小对象尺寸和初始化成本。
9. **A5-only 文件拆分**：把 A5 专用实现拆到 `platform_v310/hccl_aicpu_a5.h`，公共逻辑只留必要部分。
   这样可以避免大量 `#if`，并让编译器只实例化 3510 路径。

## 验证方案

### 功能验证

以 A5 AICPU 路径为重点：

1. 运行 `tests/api/adv_api/hccl/test_hccl_aicpu_950.cpp` 相关 CPU UT。
2. 覆盖以下场景：
   - `InitV2` 带 tiling 和不带 tiling。
   - AllReduce / AllGather / ReduceScatter / AlltoAll / AlltoAllV 的 Prepare。
   - `repeat > 1` 时的 Commit/Wait 次数约束。
   - sync / async Finalize。
   - 无效 handle、未初始化调用、超次数 Commit/Wait（CPU/debug 模式验证）。
3. 若确认 A5 不使用 `stepSize`，需要补充一个 tiling 中 `stepSize != 0` 的确认用例或明确记录不支持。

### 性能验证

在相同 3510 环境、相同数据规模和重复次数下对比：

- 单次 `InitV2` 耗时。
- 每类 Prepare 的单次耗时。
- Commit/Wait 的单次耗时。
- Finalize 耗时。
- AlltoAllV 扩展消息发布耗时。

建议输出：

- 修改前后均值、P50、P99。
- DCCI/DSB/Barrier 调用次数。
- release 指令数和代码体积变化。

### 兼容性检查

- A2 (`__NPU_ARCH__ == 2201`) 保持原实现和原行为。
- 960 (`9201/9202`) 当前也复用 `platform_v310`，如果共享 `hccl_aicpu_impl.h` 的通用实现，
  需要明确 A5-only 分支只包 `3510`，或确认 950/960 可以同步优化。
- CCU 路径不受影响。

## 实施顺序

1. 增加 A5-only 实现入口，不改变 960/A2 行为。
2. InitV2 精简和数组初始化优化。
3. Prepare 模板化和 `queueNum_` 分支裁剪。
4. Commit/Wait/Finalize 去除 A5 不需要的 DFX 和分支。
5. FlushDataCache 批量发布优化。
6. 补充同粒度候选项，按实测数据决定是否继续。
