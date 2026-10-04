# Issue #1822 分析：IBSet 的 `gmWorkspace` 空间约束描述不清晰

## 问题结论

[Issue #1822](https://gitcode.com/cann/asc-devkit/issues/1822) 反馈的问题存在。文档原文想表达乘法，但未对 Markdown 中的星号进行转义，导致 `*` 被解析为强调标记而未在页面上显示。最终渲染为“核数32ByteseventID_max”，用户无法确认这里是乘法还是文本粘连。

**原有约束的计算公式本身正确，错误只在 Markdown 展示。** 当工作区需要覆盖 `0..eventID_max` 和 `0..blockIdx_max` 的地址范围时，原公式能够准确覆盖实现可访问的最大偏移及其对应的 32 Bytes 标志位。

**原资料对 `eventID_max` 和 `blockIdx_max` 的确定方法说明不充分，本次需要一并修复。** [IBSet 参数说明](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBSet.md#L51) 只给出了 `blockIdx` 的范围，没有说明 `eventID` 的编号规则和范围；[约束说明](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBSet.md#L64) 仅称两者是对应参数的最大值，没有说明统计范围、动态参数如何处理，以及它们与 `gmWorkspace` 的对应关系。

当前 [IBSet 约束说明](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBSet.md#L64) 中的公式原意为：

$$
\text{核数} \times 32\text{字节} \times \mathrm{eventID\_max} + \mathrm{blockIdx\_max} \times 32\text{字节} + 32\text{字节}
$$

其中 `eventID_max` 和 `blockIdx_max` 分别是实际使用的 `eventID` 和 `blockIdx` 的最大值，两者都从 0 开始计数。该公式也可等价写成：

$$
(\text{核数} \times \mathrm{eventID\_max} + \mathrm{blockIdx\_max} + 1) \times 32\text{字节}
$$

## 正确性证据

原公式可由以下代码直接推导：

| 证据 | 代码或文档 | 结论 |
| --- | --- | --- |
| `gmWorkspace` 的元素类型 | [`IBSet` 接口实现](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L142) | `gmWorkspace` 是 `GlobalTensor<int32_t>`，单个元素占 4 Bytes。 |
| `operator[]` 的偏移单位 | [`GlobalTensor::operator[]` 文档](docs/zh/api/SIMD-API/basic_api/data_structures/GlobalTensor/operator.md#L29) 和 [实现](impl/basic_api/kernel_tensor_impl.h#L1663) | 下标表示元素数偏移；对 `int32_t` 会按 `offset` 个元素移动地址。 |
| 同步标志的地址 | [`IBSet` 寻址代码](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L160) | 偏移为 `blockNum * 8 * eventID + blockIdx * 8` 个 `int32_t`，换算后为 `(核数 × eventID + blockIdx) × 32 Bytes`。 |
| 每个标志的占用空间 | [`ONE_BLK_SIZE` 定义](impl/basic_api/utils/kernel_utils_constants.h#L253) 和 [`IBSet` 的 `DataCopy`](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L163) | `ONE_BLK_SIZE` 为 32，每次从该地址读写 `32 / sizeof(int32_t) = 8` 个元素，即 32 Bytes。 |
| MIX 模式的核数 | [`blockNum` 计算](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L146) | 默认为 `GetBlockNum()`；相关架构的 MIX 模式下为 `GetBlockNum() × 2`，与文档一致。 |
| 配对接口一致性 | [`IBWait` 寻址代码](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L202) | `IBWait` 使用与 `IBSet` 完全相同的偏移计算。 |

## 公式推导

`IBSet` 的实现使用下列表达式定位同步标志，见 [kernel_operator_block_sync_intf_impl.h](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L160)：

```cpp
auto localSyncGM = gmWorkspace[blockNum * 8 * eventID + blockIdx * 8];
```

`gmWorkspace` 的元素类型为 `int32_t`，因此索引中的 `8` 个元素占 `8 × 4 = 32 Bytes`。对给定的 `eventID` 和 `blockIdx`，访问起始字节偏移为：

$$
(\text{核数} \times \mathrm{eventID} + \mathrm{blockIdx}) \times 32\text{字节}
$$

每个同步标志占用 32 Bytes，所以最大偏移后还需要增加 32 Bytes，得到文档中的最小空间公式。[IBWait 的实现](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L202) 使用了相同的寻址方式，证明配对接口的空间约束应保持一致。

`blockNum` 的取值与文档现有说明一致：默认为 `GetBlockNum()`；在相关架构的 MIX 模式下，`isAIVOnly` 为 `false` 时取 `GetBlockNum() × 2`，见 [IBSet 实现](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L146) 和 [MIX 模式分支](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L153)。

因此，当最大参数为 `eventID_max` 和 `blockIdx_max` 时，最后一个标志的起始偏移为：

$$
(\text{核数} \times \mathrm{eventID\_max} + \mathrm{blockIdx\_max}) \times 32\text{字节}
$$

再加上该标志本身占用的 32 Bytes，得到：

$$
\begin{aligned}
&(\text{核数} \times \mathrm{eventID\_max} + \mathrm{blockIdx\_max} + 1) \times 32\text{字节} \\
={}&\text{核数} \times 32\text{字节} \times \mathrm{eventID\_max} + \mathrm{blockIdx\_max} \times 32\text{字节} + 32\text{字节}
\end{aligned}
$$

这与原约束完全一致，所以不需要改变公式的计算含义。

> **边界说明：** 上述公式按 `eventID_max` 和 `blockIdx_max` 构成的完整地址范围申请空间，是安全且与现有接口约束一致的算法。如果业务只使用少量、稀疏的 `(eventID, blockIdx)` 组合，严格的理论最小值可按所有实际访问偏移的最大值加 32 Bytes 计算；此时原公式可能略有富余，但不会导致越界或功能错误。

## `eventID_max` 和 `blockIdx_max` 的确定方法

两个最大值都应在**共享同一个 `gmWorkspace` 的全部 `IBSet`/`IBWait` 调用**中统计。

- `blockIdx_max`：所有相关调用中，传入 `blockIdx` 实参可能取到的最大值。`blockIdx` 的有效范围为 `[0, 核数 - 1]`，但 `blockIdx_max` 并不必然等于 `核数 - 1`；如果只为核 0 建立同步标志，则 `blockIdx_max = 0`。从 [IBSet/IBWait 样例](examples/01_simd_cpp_api/03_basic_api/05_sync_control/ib_set_wait/ib_set_wait.asc#L45) 可以看到，核 1 等待核 0 时，`IBWait` 和 `IBSet` 都传入 `blockIdx = 0`。因此，该参数在典型用法中表示被等待、负责发送通知的核的逻辑索引，现有的“等待核的 idx 号”表述存在歧义。
- `eventID_max`：所有相关调用中，传入 `eventID` 实参可能取到的最大值。`eventID` 是用户为软件同步事件指定的非负逻辑编号，与 `blockIdx` 一起确定 `gmWorkspace` 中的唯一标志位。建议从 0 开始连续编号，以避免不必要的空间空洞。当多个同步事件可能同时使用同一个 `blockIdx` 时，应使用不同的 `eventID`；已完成且不会重叠的同步事件可以复用编号。当前仓库文档和接口实现中未发现 `eventID` 的独立固定上限；它的可用范围受 `gmWorkspace` 大小和偏移计算范围限制。
- 如果实参是运行时计算得到的，不能只按某次运行的实际值申请空间，而应使用所有合法输入和分支下可能取到的上界。

现有样例中，`IBSet` 和 `IBWait` 都传入 `(blockIdx, eventID) = (0, 0)`，见 [`IBWait` 调用](examples/01_simd_cpp_api/03_basic_api/05_sync_control/ib_set_wait/ib_set_wait.asc#L45) 和 [`IBSet` 调用](examples/01_simd_cpp_api/03_basic_api/05_sync_control/ib_set_wait/ib_set_wait.asc#L80)。因此该样例中 `blockIdx_max = 0`、`eventID_max = 0`，按公式计算的最小空间为：

$$
(\text{核数} \times 0 + 0 + 1) \times 32\text{字节} = 32\text{字节}
$$

如果需要使用 2 个核在 `eventID = 0` 下分别建立标志，传入的最大 `blockIdx` 为 1，则 `blockIdx_max = 1`、`eventID_max = 0`，最小空间为 64 Bytes。

## InitDetermineComputeWorkspace 公式核对

[InitDetermineComputeWorkspace 原约束](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/InitDetermineComputeWorkspace.md#L53) 中的公式计算含义正确，但原文“`GetBlockNum()*32+32Bytes`”缺少括号且单位不统一，容易产生歧义。明确的空间公式为：

$$
\begin{aligned}
\text{gmWorkspace空间大小} &\geq \mathrm{GetBlockNum}() \times 32\text{字节} \\
\text{ubWorkspace空间大小} &\geq (\mathrm{GetBlockNum}() + 1) \times 32\text{字节}
\end{aligned}
$$

代码证据如下：

| 证据 | 代码或文档 | 结论 |
| --- | --- | --- |
| 每个核的标志元素数 | [`B32_DATA_NUM_PER_BLOCK` 定义](impl/basic_api/utils/kernel_utils_constants.h#L315) | 常量值为8；元素类型为 `int32_t`，因此每个核占用 `8 × 4 = 32字节`。 |
| GM初始化和搬运范围 | [dav_3510初始化实现](impl/basic_api/dav_3510/kernel_operator_determine_compute_sync_impl.h#L52) | 初始化和搬运元素数均为 `B32_DATA_NUM_PER_BLOCK * blockNum`，即 `GetBlockNum() × 32字节`。 |
| UB额外的轮次值 | [dav_3510初始化实现](impl/basic_api/dav_3510/kernel_operator_determine_compute_sync_impl.h#L60) 和 [NotifyNextBlock实现](impl/basic_api/dav_3510/kernel_operator_determine_compute_sync_impl.h#L113) | 实现在 `blockNum * 8` 位置额外读写一个 `int32_t`轮次值，该位置位于每核标志区之后。 |
| UB分配对齐 | [`TPipe::InitBuffer` 的 `len` 说明](docs/zh/api/SIMD-API/basic_api/resource_management/TPipe/InitBuffer.md#L104) | 内存块长度按32字节对齐，额外的 `int32_t`轮次值需要一个32字节块，因此UB公式为 `(GetBlockNum() + 1) × 32字节`。 |
| 完整样例的空间定义 | [`GM_WORKSPACE_SIZE` 和 `UB_WORKSPACE_SIZE`](examples/01_simd_cpp_api/03_basic_api/05_sync_control/sequential_block_sync/sequential_block_sync.asc#L21) | 样例分别使用 `BLOCK_NUM * 32` 和 `BLOCK_NUM * 32 + 32`，与上述公式一致。 |

受支持的多个架构实现使用了同样的元素数、GM搬运范围和UB轮次值位置，可交叉参考 [dav_c220实现](impl/basic_api/dav_c220/kernel_operator_determine_compute_sync_impl.h#L27)、[dav_m200实现](impl/basic_api/dav_m200/kernel_operator_determine_compute_sync_impl.h#L27)、[dav_m510实现](impl/basic_api/dav_m510/kernel_operator_determine_compute_sync_impl.h#L26) 和 [dav_v516实现](impl/basic_api/dav_v516/kernel_operator_determine_compute_sync_impl.h#L26)。

该空间约束同时适用于配套的 [NotifyNextBlock](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/NotifyNextBlock.md#L55) 和 [WaitPreBlock](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/WaitPreBlock.md#L55)，三份文档应保持相同的公式表达。

## AIC调用行为核对

在[AIC/AIV分离模式](docs/zh/guide/programming_guide/advanced_programming/hardware_implementation/basic_architecture.md)下，两对核间同步接口均仅在AIV核上执行同步逻辑。在AIC核中调用时，接口直接结束，不产生核间同步效果，但具体实现形式不同：

| 接口 | 代码证据 | AIC调用行为 |
| --- | --- | --- |
| `IBSet` | [`ASCEND_IS_AIC`分支](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L147) | 显式执行`return`，不设置同步标志。 |
| `IBWait` | [`ASCEND_IS_AIC`分支](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L189) | 显式执行`return`，不等待同步标志。 |
| `WaitPreBlock` | [dav_c220实现](impl/basic_api/dav_c220/kernel_operator_determine_compute_sync_impl.h#L66)和[dav_3510实现](impl/basic_api/dav_3510/kernel_operator_determine_compute_sync_impl.h#L84) | 同步逻辑全部位于`ASCEND_IS_AIV`条件体内；AIC核跳过条件体后直接结束。 |
| `NotifyNextBlock` | [dav_c220实现](impl/basic_api/dav_c220/kernel_operator_determine_compute_sync_impl.h#L88)和[dav_3510实现](impl/basic_api/dav_3510/kernel_operator_determine_compute_sync_impl.h#L106) | 同步逻辑全部位于`ASCEND_IS_AIV`条件体内；AIC核跳过条件体后直接结束。 |
| `InitDetermineComputeWorkspace` | [dav_c220实现](impl/basic_api/dav_c220/kernel_operator_determine_compute_sync_impl.h#L27)和[dav_3510实现](impl/basic_api/dav_3510/kernel_operator_determine_compute_sync_impl.h#L46) | 初始化逻辑全部位于`ASCEND_IS_AIV`条件体内；AIC核不会初始化工作区。 |

因此，约束说明应明确限定为“分离模式”：不能把上述AIC行为泛化到耦合架构或产品支持表中的“AI Core”概念。

## 影响范围

这是文档渲染问题，不是 `IBSet` 接口实现或空间计算公式错误，因此不需要修改接口声明、实现或公式含义。

同样的未转义公式也出现在 [IBWait 约束说明](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBWait.md#L64) 中。`IBSet` 和 `IBWait` 成对使用，建议同步修改两份文档，避免只修复单个页面后仍然存在不一致。

## 修复方案

1. 将 [IBSet.md](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBSet.md#L66) 和 [IBWait.md](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBWait.md#L66) 中的公式改为 LaTeX 公式，并使用规范的 `\times` 表示乘法。
2. 补充最大值的统计范围：`eventID_max` 和 `blockIdx_max` 分别是共享同一 `gmWorkspace` 的全部 `IBSet`/`IBWait` 调用中，两个实参可能取到的最大值；动态实参应按所有合法路径的上界确定。
3. 补充 `eventID` 是非负逻辑事件编号、建议从 0 开始连续编号，并说明可能重叠使用的同步事件需使用不同编号。
4. 将 `blockIdx` 的“等待核的 idx 号”改为无歧义的描述，明确其为同步标志对应的逻辑核索引，且配对的 `IBSet` 和 `IBWait` 必须传入相同值。
5. 约束文案中明确 `gmWorkspace` 空间公式，并说明 `eventID_max` 和 `blockIdx_max` 分别为共享该 `gmWorkspace` 的全部 `IBSet`/`IBWait` 调用中，传入 `eventID` 和 `blockIdx` 可能取到的最大值。如果参数在运行时确定，应按其可能取值的上界计算。
6. 保留并规范 AIVOnly 和 MIX 模式的核数说明：AIVOnly 模式下为 `GetBlockNum()`，MIX 模式下为 `GetBlockNum() × 2`。
7. 将 `InitDetermineComputeWorkspace`、`NotifyNextBlock` 和 `WaitPreBlock` 的空间约束改为无歧义的 LaTeX 公式，并保持三份文档一致。

## 验证建议

- 本地渲染两份 Markdown，确认乘号、下划线变量名和括号均完整显示。
- 检查 `IBSet` 与 `IBWait` 两个页面的公式和模式说明一致。
- 用实现中的偏移公式复核边界值。例如，实际使用 2 个核、`eventID_max = 0`、`blockIdx_max = 1` 时，最小空间为 `(2 × 0 + 1 + 1) × 32 Bytes = 64 Bytes`。
- 使用静态常量、分支上界和编号复用三类用例，验证 `eventID_max` 和 `blockIdx_max` 的确定方法不会造成空间不足。
- 执行仓库的 Markdown 链接和格式检查，确认没有引入新的文档问题。

## Issue 回复建议

感谢反馈。经确认，这里的原意是做乘法，原约束的计算公式本身正确，`gmWorkspace` 的最小空间可表示为 `(核数 × eventID_max + blockIdx_max + 1) × 32 Bytes`。当前文档中乘号使用了未转义的 `*`，被 Markdown 误解析为强调标记，导致页面上的乘号丢失；同时，原资料确实没有说清 `eventID_max` 和 `blockIdx_max` 的确定方法。我们将同时修复 `IBSet` 和 `IBWait` 文档：使用不会被误解析的公式表达，明确这两个最大值应在共享同一 `gmWorkspace` 的全部 `IBSet`/`IBWait` 调用中统计，动态参数按所有合法路径的取值上界确定，并补充 `eventID` 的编号规则及 `blockIdx` 的无歧义说明；预计于 2026 年 10 月 9 日前完成修复。
