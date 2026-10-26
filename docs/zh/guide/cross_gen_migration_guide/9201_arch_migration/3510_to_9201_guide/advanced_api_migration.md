# 高阶API及Tensor API迁移指导

3510到9201的高阶API和Tensor API迁移，需结合核配比、底层计算精度和数据搬运方式判断。接口名称相同不代表特定扩展策略或参数配置可以直接沿用。

**表1** 兼容性问题索引

| 接口 | 受影响场景 | 兼容方案 |
|------|------------|----------|
| Matmul（兼容性材料中记为MatMul） | 使用SplitMMatmulPolicy或SplitNMatmulPolicy，并依赖1:2配比 | 按1:1重新设计切分与协作，重新生成Tiling。 |
| Silu、Swish、Gelu、FasterGelu | 实际使用fp16 Exp计算路径 | 进行精度回归，高精度场景采用支持的fp32路径。 |
| Tensor API Copy | L0C到UB搬运依赖dualdst | 按单个AIV重新设计目的布局、后处理和同步。 |

## Matmul切分策略<a id="matmul-policy-migration"></a>

**说明**：SplitMMatmulPolicy、SplitNMatmulPolicy依赖的Cube与Vector协作方式需要适配1:1。仅把执行配置中的两个AIV改成一个，不能保证原策略处理完整输出。这些策略的公开性和可用性还需核对配套SDK，不能直接依赖内部策略定义。

**兼容方案**：

1. 确认实际使用的Matmul策略及目标SDK支持范围，优先采用目标SDK公开支持的策略。
2. 按一个AIV重新确定M/N轴切分、Cube结果分发、Vector处理量和工作区布局。
3. 重新计算L0C到UB输出、UB/L1缓冲区、每轮同步及Tiling。原按两个AIV分别分配的输出不能继续保留一半未处理。
4. 验证M/N/K尾块、多轮迭代、累加、Bias及实际启用的后处理分支，检查漏算、重复计算和输出顺序。

对于未使用上述策略的Matmul调用，不应仅凭接口名判定不兼容；仍需根据实际使用的数据类型和目标产品支持情况完成重新编译与验证。

## 激活函数精度<a id="activation-precision-migration"></a>

**说明**：fp16 Exp的部分输入在9201与3510之间存在1 ULP差异，可能传递到Silu、Swish、Gelu和FasterGelu结果中。高阶API最终输出的误差受算法和后续运算影响，不能直接认为也固定为1 ULP。

**兼容方案**：

- 确认接口配置和实际计算精度。输入为fp16但内部已经转换为fp32的路径，不应直接按fp16 Exp差异处理。
- 优先使用目标SDK公开支持的高精度算法配置或fp32计算路径，并重新核对临时空间需求。
- 需要自行扩展精度时，先将输入转换为fp32，在fp32中执行所需计算，再按输出类型转换；不要只扩大中间张量类型而保留fp16指数计算。
- 对照数学参考实现验证误差，同时比较3510和9201结果。测试零附近、饱和区、极值及实际模型输入分布。

基础Exp的转换示例见[Exp精度迁移](./reg_api_migration.md#exp-precision-migration)。

## Tensor API Copy<a id="tensor-copy-migration"></a>

**说明**：本项针对Copy的L0C到UB且依赖dualdst的场景，不代表全部Copy调用均不兼容。9201不支持原来向两个AIV的UB分发Cube输出的方式。

**兼容方案**：按单个AIV重新定义目的张量布局和有效数据范围，将原两个AIV的后处理改为单个AIV分批处理；同步重算缓冲区容量、数据偏移、生产者/消费者依赖及复用时机。具体方法参考[L0C到UB搬运](./basic_api_migration.md#l0c-to-ub-migration)。

**验证建议**：通过带行列编号的输入验证搬运覆盖范围和排列，分别覆盖整块、尾块、跨缓冲区轮转和后处理融合场景。
