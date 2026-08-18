# Checker 分层和算法校验矩阵

## 1. 判断放置位置

使用以下问题判断 Checker 归属：

1. 换成另一个 collective 后规则仍完全成立吗？
   - 是：放 `hccl_verifier/ccu_checker.*` 等公共层。
2. 规则涉及固定 Rank 拓扑、XN index、TaskArg 下标、操作次数或最终数据布局吗？
   - 是：放 `hccl_verifier/semantics_check/ccu_<collective>_semantics_checker.*`。
3. 逻辑是在执行操作，而不是判断结果吗？
   - 是：放 `sim_world`，不要放 Checker。
4. 逻辑是在捕获外部 API 调用吗？
   - 是：放 `hccl_depends_stub` 或 `hccl_proxy`。

## 2. CCU 公共 Checker

### CcuLogicalResourceChecker

跨所有 CCU 算法检查：

- ResourceHandle 非零、不同资源类型不碰撞。
- Variable、Address、Event、CcuBuffer 集合一致。
- LocalAddr/RemoteAddr 拥有有效 Address 和 Token。
- Operation 的 dst/src 引用匹配资源类型。
- IF/WHILE begin/end peerIndex 有效。
- GroupCopy Operation 引用存在的 descriptor。

### CcuProgramChecker

- 先执行逻辑资源检查。
- `ArgDescriptor.id` 从 0 连续排列。
- 每个参数恰有一个 `LOAD_ARG`。
- `LOAD_ARG.index` 与目标 Variable 正确绑定。

### CcuTaskArgChecker

- 运行参数数量等于编译态 LoadArg 数量。
- 按 `TASK_PARAM_SLOT_COUNT == 13` 分逻辑 `TaskParam` 组。
- 每组的 `argSize` 和 `copiedArgCount` 正确。
- `argId -> group/slot -> value` 无错位。

当前代码中的局部变量名使用 `mission`，但模型没有完整的 Mission/SQE 生命周期和并发语义，不能据此宣称已验证多 Mission。

### CcuRuntimeChecker

- simulator 产生的 Rank 统计数量正确。
- 所有 Rank 最终完成。

这些 Checker 不知道某算法应该有多少参数、多少 Write 或输出应是什么。

## 3. CCU AllToAllV 专用 Checker

`CcuAllToAllVSemanticsChecker` 检查：

- Mesh1D full-mesh Channel、peer 唯一性和 output/token Channel Variable。
- 当前 10 个 LoadArg：input/output/token、两个 base offset、parameter address 和四个 goSize。
- 参数表中 `tailSize/loopNum/sendOffset/recvOffset` 四字段的连续 `ccu::Load` 顺序和步长。
- PreSync/PostSync notify 契约、remote Write、self LocalCopy 和 EventWait 关系。
- 多 Rank 非对称长度、零长度 peer 和非连续 offset 的最终数据结果。
- 参数内存越界、输出破坏、TaskArg peer 重绑定和缺参能够被拒绝。

该 Checker 验证的是逻辑 Representation 和 Host 仿真语义，不验证真实 Translator 指令、物理资源绑定、微码装载或硬件执行。

当前 CCU 专用交付只有 AllToAllV Mesh1D。不要把尚未存在的 CCU AllGather/ReduceScatter Checker 描述成当前能力；接入对应产品 kernel 后，再按真实 Program 新增专用 Checker。

## 4. AICPU AllGather 语义

最终 OUTPUT 应满足：

- 连续覆盖 `rankSize * dataSize`。
- 第 r 段只来自 Rank r 的 INPUT。
- 非 Reduce 语义，每段只有一个 source。
- source offset 和 destination offset 连续正确。
- Task DAG 中同步和内存访问无非法冲突。

## 5. AICPU ReduceScatter 语义

Rank r 的 OUTPUT 应满足：

- 总长度等于单 Rank `dataSize`。
- 每个输出片段具有 rankSize 个源 Rank。
- 每个源来自 INPUT。
- 源 offset 对应输入中的第 r 个 scatter 分片。
- 多源片段的 reduceType 与请求一致。
- DAG 中 Reduce 的同步和读写关系合法。

## 6. CCU ReduceScatter 应新增的专用校验

在实现 CCU ReduceScatter 时，至少设计：

- kernelArg、Rank 拓扑和 Channel 角色。
- LoadArg 数量、顺序和值。
- Reduce 原语的 src/dst、长度、dataType、reduceOp。
- 分片 owner 与 remote/local 地址关系。
- 每轮同步和归约阶段顺序。
- 分块循环、并行参数和尾块逻辑。
- 每个 Rank 只输出自己的 reduced shard。
- 输出每个元素是所有 Rank 对应输入的指定归约结果。
- 溢出、数据类型边界和不支持 reduceOp 的拒绝路径。

需要新增 Reduce/WriteReduce 原语时，先扩展公共 IR、捕获 Stub 和 RuntimeWorld；固定 reduce 序列和输出规则仍留在 ReduceScatter semantics checker。

## 7. 负例设计

每类 Checker 至少用一个定向破坏验证：

| Checker | 典型破坏 |
|---|---|
| Register 生命周期 | End-before-Start、重复 Start、组内某 kernel 编译失败 |
| 资源 | Address 指向 Variable handle、Event 重复、未知 descriptor |
| LoadArg | 缺 ID、重复 ID、目标 Variable 错位 |
| TaskArg | 少一个参数、交换两个参数、Mission slot 错位 |
| 同步 | mask 为 0、Channel peer 错误、post-sync 顺序交换 |
| GroupCopy | bufferBase、Loop index、goSize 绑定或 residual 错误 |
| Runtime | 缺参数、越界、无法推进造成死锁 |
| 最终语义 | 执行后翻转一个输出字节，Checker 必须失败 |

仅有正例不能证明 Checker 真正检查了目标契约。
