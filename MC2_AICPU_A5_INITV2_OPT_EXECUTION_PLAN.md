# A5 AICPU MC2 InitV2 优化执行计划

## 目标与范围

本计划只覆盖 A5（`dav-3510`）AICPU MC2 的 `InitV2` 路径。目标是先做改动风险小、收益明确的优化，
不改变消息协议、不改 server 交互语义、不影响 A2/A3/960 的现有行为。

对应代码：

- [InitV2 实现](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h)
- [InitInner 实现](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h)
- [HcclImpl 成员布局](./impl/adv_api/detail/hccl/common/hccl_aicpu_def.h)
- [A5 AICPU UT](./tests/api/adv_api/hccl/test_hccl_aicpu_950.cpp)

## 推荐排序原则

优先级按如下方式综合评估：

- **改动影响**：只影响 A5 AICPU、代码行数少、无协议/布局变化者优先。
- **可能收益**：减少 GM 读、减少循环、减少 release 分支、改善布局者优先。
- **验证难度**：能用现有 A5 AICPU UT 覆盖者优先。

优先级计算采用简单评分：`优先级 = 收益 / 风险`。

## 改动点与优先级

| 优先级 | 改动点 | 代码位置 | 预期收益 | 风险 | 建议 |
|---|---|---|---|---|---|
| P0 | 删除 A5 `InitV2` 中的 `devType_` 赋值 | [hccl_aicpu.h](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h) | 减少一次 tiling 字段读取和无效状态赋值 | 极低：A5 后续无读取 | 立即做 |
| P1 | `InitInner` 数组初始化改为宽位写入 | [hccl_aicpu_impl.h](./impl/adv_api/detail/hccl/common/hccl_aicpu_impl.h) | 63 次循环写变为约 8 次写，减少循环控制 | 低：需要保证不越界 | 第二步做 |
| P2 | InitV2 异常分支统一放入 ASSERT 宏 | [hccl_aicpu.h](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h) | release 减少手写分支和跳转 | 低：CPU/debug 语义需保持 | 第三步做 |
| P3 | A5-only InitV2/InitInner 拆分 | [hccl_aicpu.h](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h) | 隔离 A5 实现，为后续裁剪做准备 | 中：涉及模板和文件组织 | 第四步做 |
| P4 | A5-AICPU 瘦 OpResCtx | [hccl_msg.h](./include/adv_api/hccl/internal/hccl_msg.h) | 408B 缩到约 80B，减少 D2H/H2D 拷贝和 server 遍历 | 高：host、server、launch、UT 均需同步 | 最后做 |

说明：

- P0 虽然单次收益小，但改动几乎无风险，适合作为第一步基线验证。
- P1 是 InitV2 内部最明确的性能收益点。
- P2 先保持行为不变，只把手写分支收敛到已有 ASSERT 宏。
- P3 是结构性准备，不建议和 P0-P2 混在同一个 patch。
- P4 不建议在第一批做；它涉及结构布局和 launch 分发，应该等 InitV2 行内优化验证稳定后再做。

## 改动顺序

### 第 1 步：删除 A5 不使用的 `devType_` 赋值

改动内容：

1. 仅在 `__NPU_ARCH__ == 3510` 分支中删除：

   ```cpp
   devType_ = initTilingPtr->devType;
   ```

2. 保留 `debugMode_`、`tilingBaseAddr_`、`hcclContext_` 赋值。
3. 不修改成员 `devType_` 本身，避免影响 A2/A3/960。

验证目标：

- A5 AICPU UT 全部通过。
- A2/A3/960 编译目标不受影响。
- InitV2 带 tiling / 不带 tiling 两种路径均正常。

### 第 2 步：数组初始化改为宽位地址写入

改动内容：

1. 为 A5 增加 `FillInt8A5` 或 `InitHandleMsgPositionA5`。
2. `handleIdMsgPosition_` 共 63 字节：
   - 前 56 字节按 7 个 `uint64_t` 写入 `0xFFFFFFFFFFFFFFFF`；
   - 剩余 7 字节按 `uint64_t` 带掩码写入，掩码低 7 字节为 1。
3. 使用 `static_assert(HCCL_MAX_HANDLE_ID == 63)` 锁定当前布局。
4. A2/A3 保持原循环。

推荐实现：

```cpp
__aicore__ inline void InitHandleMsgPositionA5(int8_t* dst)
{
    constexpr uint32_t count = HCCL_MAX_HANDLE_ID;
    constexpr uint32_t fullCount = count / sizeof(uint64_t);
    constexpr uint32_t tailCount = count % sizeof(uint64_t);
    uint64_t* dst64 = reinterpret_cast<uint64_t*>(dst);
    for (uint32_t i = 0; i < fullCount; ++i) {
        dst64[i] = UINT64_MAX;
    }
    if constexpr (tailCount != 0U) {
        const uint64_t tailMask = (1ULL << (tailCount * 8U)) - 1ULL;
        dst64[fullCount] = tailMask;
    }
}
```

验证目标：

- UT 中新增或确认以下场景：
  - 首次 Init 后，Prepare/Wait 能正常找到 message position；
  - 未 Prepare 的 handle 调 Wait/Query 仍返回失败；
  - 多 handle 顺序 Prepare/Commit/Wait 正常。
- 编译器无越界告警。
- 与原循环实现的初始化结果完全一致。

### 第 3 步：异常分支统一放入 ASSERT 宏

改动内容：

1. 将 `InitV2` 中 `hcclContext_ == nullptr`、`opType` 越界判断统一改为
   `ASCENDC_HCCL_API_ASSERT`。
2. 将 `InitInner` 中重复初始化和 `msgAddr == 0` 分支统一改为 ASSERT 宏。
3. 保留 512B 对齐计算。
4. release 模式下由调用方保证不变量；CPU/debug 模式仍返回原失败语义。

验证目标：

- CPU UT 中无效 context、重复 Init、无效 opType 用例仍按预期失败。
- release 编译通过。
- 正常路径行为不变。

### 第 4 步：A5-only InitV2/InitInner 拆分

改动内容：

1. 将 A5 专用实现拆到 `platform_v310/hccl_aicpu_a5.h` 或使用明确的
   `#if __NPU_ARCH__ == 3510` 分支。
2. A5-only InitV2 只保留：
   - 读 `debugMode_`；
   - 记录 `tilingBaseAddr_`；
   - 设置 `hcclContext_`；
   - 调用 A5-only InitInner。
3. A5-only InitInner 只保留单队列路径，不再判断 `queueNum_`。
4. 不修改 A2/A3/960 通用路径。

验证目标：

- A5 AICPU UT 全部通过。
- 960 复用相关文件时行为不变；如果 960 共用 A5-only 分支，需显式确认后再合入。
- 编译产物中 A5 路径分支数量减少。

### 第 5 步：A5-AICPU 瘦 OpResCtx

改动内容：

1. 新增 `OpResCtxA5Aicpu`，保留：
   - `magic/version`
   - `workspace`
   - `rankId/rankSize`
   - `algInfo[2]`
   - `opParamSize[2]`
   - 必要的 `opType` 信息
2. host 分配路径为 AICPU_TS 生成瘦结构。
3. AICPU server 按 `magic/version` 识别瘦结构。
4. `Mc2CcKernelLaunch` 不再依赖 `commEngine` 判断上下文类型，改为按 magic/version 分发。
5. 保持公共 `OpResCtx` 给 CCU/960 使用，不裁剪公共结构。

验证目标：

- AICPU host 分配、server 初始化、资源缓存、launch 全链路通过。
- CCU UT 全部通过。
- A5 AICPU ST 通过。
- D2H/H2D 拷贝大小从 408B 降到约 80B。
- Server `algInfo` 遍历从 8 次降到 2 次。

## 统一验证方法

### 编译验证

推荐目标：

```bash
cmake --build build_out --target ascendc_ut_adv_api_kernel_ascend950pr_9599_AIC -j8
```

如需要确认跨架构不受影响，再构建：

```bash
cmake --build build_out --target ascendc_ut_adv_api_kernel_ascend910B1_AIC -j8
cmake --build build_out --target ascendc_ut_adv_api_kernel_ascend950pr_9599_AIV -j8
```

### 功能验证

运行 A5 AICPU UT：

```bash
ctest --test-dir build_out -R ascendc_ut_adv_api_kernel_ascend950pr_9599_AIC --output-on-failure
```

重点用例：

- InitV2 带 tiling。
- InitV2 不带 tiling。
- AllGather / AllReduce / ReduceScatter / AlltoAll / AlltoAllV。
- 无效 context / 重复 Init / 无效 opType。

### 性能验证

每步完成后记录：

- InitV2 单次耗时。
- 连续 Prepare/Commit/Wait 场景下的 InitV2 均值。
- release 指令数或汇编行数。
- 修改前后对比：均值、P50、P99。

建议固定变量：

- 相同 3510 环境。
- 相同数据规模和 repeat。
- 相同编译选项。
- 至少 1000 次 InitV2，取均值和 P99。

## 验证结果记录

> 以下表格在每步实施后填写。当前尚未修改代码，均为“未开始”。

| 步骤 | 改动点 | 编译是否通过 | 功能是否正常 | 性能提升情况 | 结论 |
|---|---|---|---|---|---|
| 1 | 删除 A5 不使用的 `devType_` 赋值 | 通过 | 通过 | 待实测；减少 1 次 tiling 读取和 1 次无效赋值 | 已完成 |
| 2 | 数组初始化改为宽位地址写入 | 通过 | 构建通过 | 待复测 7+4+2+1 分段宽写版本 | 已恢复为分段版本 |
| 3 | 异常分支统一放入 ASSERT 宏 | 通过 | 通过 | 待实测；InitInner 中 workspace 空值分支改为 ASSERT | 已完成 |
| 4 | A5-only InitV2/InitInner 拆分 | 未开始 | 未开始 | 未开始 | 未开始 |
| 5 | A5-AICPU 瘦 OpResCtx | 未开始 | 未开始 | 未开始 | 未开始 |

## 当前状态

P0 和 P1 已完成代码修改：

- [hccl_aicpu.h:33](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h)：A5 分支不再读取
  `devType_`。
- `InitInner` 中 `handleIdMsgPosition_` 的宽位初始化已回退，恢复原始逐元素循环。
- P0 仍保留在 [hccl_aicpu.h](./impl/adv_api/detail/hccl/impl/platform_v310/hccl_aicpu.h)。

验证记录：

- 构建目标：`ascendc_ut_adv_api_hccl_cc_aicpu_mc2`
- 构建结果：通过，exit code 0。
- 功能测试：22/22 passed。
- 性能测试：宽位初始化包多次复测的 Init 均值出现过约 250、270、300、313、314 ticks。该波动范围
  大于最初单次 314.6875 与 278.10875 的差异，因此不能把首次差异归因为 P1 回归。当前先回退 P1，
  保留 P0；需用同日基线与 P0-only 包做交替 A/B 复测后再决定是否重新引入 P1。

### 结果波动分析

当前输出中 `n=800`、`rank_count=8`，对应 8 个 rank 各 100 个热态样本，即只有 1 次 process repeat。
这种口径能反映单次进程内的采样，但不能覆盖进程启动、设备状态、缓存状态和频率的轮次间波动。

建议复测时：

1. 将 `sampling.repeats` 提高到 10，使每个 Client 指标的 n 达到 8000。
2. 不只看合并 mean，同时看 p50/p99 和每个 rank/process 的分布。
3. 基线与 P0-only 包同日交替执行，例如 B、P0、B、P0、B、P0。
4. 固定相同 `--devices` 和 rank 映射，避免把卡间差异误当成代码差异。

### A/B 实测结果

| 原语 | 优化前 mean | 优化后 mean | mean 变化 | 优化前 p50 | 优化后 p50 |
|---|---:|---:|---:|---:|---:|
| a2a | 300.925 | 263.701 | -37.224 (-12.4%) | 312 | 247 |
| a2av | 284.559 | 258.796 | -25.763 (-9.1%) | 258 | 236 |
| ag | 288.786 | 282.617 | -6.169 (-2.1%) | 274 | 272 |
| ar | 301.502 | 276.072 | -25.430 (-8.4%) | 323 | 258 |
| rs | 294.117 | 276.995 | -17.122 (-5.8%) | 310 | 259 |

结论：上表对应“64 字节对齐 + 8 次 64 位写”版本。10 轮复测显示“7 次 64 位写 + 4/2/1 字节尾部写”
版本 mean 为 300-307 ticks，明显差于对齐 8 写版本，因此最终保留对齐 8 写版本。

P2 补充记录：

- `InitInner` 中 `msgAddr == 0` 的手写分支已改为 `ASCENDC_HCCL_API_ASSERT`。
- 构建目标 `ascendc_ut_adv_api_hccl_cc_aicpu_mc2` 通过。
- 手动运行该 UT，22/22 passed。
