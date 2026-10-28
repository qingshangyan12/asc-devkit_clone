# MC2 CCU Host 性能打点（WIP）

配套测试与解析：[hccl_api_test 的 perf_acceptance/ccu](hccl_api_test/perf_acceptance/ccu/README.md)。本功能无需额外 CMake 参数，按普通流程构建包含本修改的 asc-devkit 包即可。需要安装配套 mc2_client 库，并保证编译与运行使用的内部上下文头文件、Client/Server 包匹配。设备侧包仍需满足目标设备签名要求；本次探针位于 Host，不修改设备参数协议。

## 运行时开关

在执行测试的每个 Host rank 进程环境中设置：

```bash
export MC2_PERF_ACCEPTANCE=1
# 按 perf_acceptance/ccu/README.md 启动 server 模式测试
```

MPI 启动器需将该变量传给所有 rank。仅精确字符串 `1` 开启；未设置、`0`、空字符串及其他值均关闭。关闭示例：

```bash
unset MC2_PERF_ACCEPTANCE
```

沿用 AICPU 探针的总开关命名。变量在 Host 每次进入 `Mc2CcKernelLaunch` 时读取，不依赖 Device 进程继承 Host 环境变量。配套 suite 在 server 模式自动开启，在 client 模式显式关闭。

- 关闭：保持原资源准备和提交路径，跳过探针读时钟、记录和日志输出。
- 开启：初始化本次 launch 的线程局部状态，暂存三个 Host 区间的起止时间戳，返回前统一输出记录及 END。
- 正常和提前返回均由 `KernelScope` 输出已有记录，退出后清除线程启用状态；提前返回不会静默当作成功采集。
- 公开 API、launch 参数及 OpResCtx 布局保持不变；CCU 无需通过 AICPU 的 `CommKfcParamDesc` version=14/15 向 Device 传递开关。

旧版 mc2_client 不会输出本探针日志；无日志必须判为未覆盖，不能宣称采集成功。该开关在本实现中服务 CCU Host launch 路径；不要将 AICPU launch 的不完整记录作为 CCU 结果。

## 指标与限制

输出 `MC2_CCU_PERF` 和 `MC2_CCU_PERF_END`。沿用 AICPU 探针的逐指标起止时间戳及 END 完整性记录形式，通过 `HCCL_RUN_INFO` 写入 Host HCCL 日志。以 **Host CLOCK_MONOTONIC** 计时，不与 AICPU/AICore 时间直接相减。每线程每 launch 最多暂存 3 条，溢出通过 dropped 明示。

| metric | 区间 |
| --- | --- |
| host_ctx_load | Start 到 PollBegin，包含入口参数检查及 CopyOpResCtxToHost 上下文 D2H |
| host_param_load | PollBegin 到 Parsed，包含设备类型检查、引擎分发及 CopyOpParamToHost 算子参数 D2H |
| host_kernel_submit | Parsed 到 Submitted，包含参数加载结果检查、comm 提取和 LaunchCcuKernel（资源读取/反序列化、mission 检查及所有 HcommCcuKernelLaunch 提交） |

基础接口沿用 `Mc2PerfProbe` 的命名与签名：`Enabled()`、`Get()`、`Now()`、`Add(metric, begin, end, status)`、`Start()`、`PollBegin()`、`Parsed()`、`Submitted(status)`、`Dump()` 和 `KernelScope(enabled)`。Host 调用点读取环境变量后传入 KernelScope，探针头文件不负责读取运行环境。

`Parsed()` 对应 Host 参数加载完成边界：使用同一个时间戳结束 `host_param_load`，并作为 `host_kernel_submit` 的起点；`Submitted(status)` 记录终点和提交状态。其形式参照 AICPU 的 `Parsed()`/`Submitted()`，含义为 Host 参数加载完成。参数加载失败提前返回时保留已有记录，不生成提交区间，END 不会被判为完整。

关闭时仍有运行时分支和线程局部启用状态访问，不宣称零开销。开启时在 API 退出才打印记录；时间戳和记录操作的探针开销未扣除。launch 是线程局部的启用采集次数，不是 MPI iteration；需按实际 PID 和测试轮次映射。当前配套负载限定每 rank 单线程调用，不能直接用于同 PID 多线程 launch 的统计。

当前验收程序按每 launch 一次 collective/repeat=1 使用；日志中的 seq=0、cache=-1 仅为兼容 schema=2 的固定字段，探针不收集消息序号或缓存状态。三个区间反映 Host 资源读取与提交，不代表 CCU 微码执行或通信时间。三个区间共享阶段边界，其和覆盖 Start 到 Submitted；提交后的返回值检查和退出日志不在此区间内，不能作为完整 API 总耗时。需对照无探针运行评估扰动，尤其注意日志输出对后续 Client 启动间隔的影响。

日志示例（省略 HCCL 日志前缀）：

```text
[MC2_CCU_PERF] schema=2 pid=123 launch=31 seq=0 metric=host_ctx_load begin_ns=100 end_ns=200 cache=-1 status=0
[MC2_CCU_PERF] schema=2 pid=123 launch=31 seq=0 metric=host_param_load begin_ns=200 end_ns=500 cache=-1 status=0
[MC2_CCU_PERF] schema=2 pid=123 launch=31 seq=0 metric=host_kernel_submit begin_ns=500 end_ns=900 cache=-1 status=0
[MC2_CCU_PERF_END] schema=2 pid=123 launch=31 records=3 dropped=0 valid=7 status=0
```

valid 的 bit0/1/2 分别表示上述三个区间已经记录。成功结果要求 records=3、dropped=0、valid=7、status=0；提前返回默认 status=-1，提交调用失败记录实际返回码，时钟失败或逆序为 -2。cache=-1 表示未分类；配套报表统一为 `na`，不当作有效 cache hit/miss。

Host 日志保存在 suite 设置的 `ASCEND_PROCESS_LOG_PATH` 下；启用 slog stdout 打印时也可能镜像到 MPI stdout。解析器检查逐段记录和 END 的完整性，合并轮转日志，并校验、去重 stdout 镜像。缺段、重复、错误 PID、溢出、失败状态或无 END 均判为不完整。

配套框架另在 Host 程序测量 `host_resource_prepare`（HcclAllocComResourceByTiling 调用），不属于本探针的三个区间。正式结果按每卡每轮 30 次的均值汇总，p50/p99 为这些批次均值的分位数，不是单次执行尾延迟；详细定义见 [测试实现与统计口径](../../../hccl_api_test/perf_acceptance/ccu/docs/METHODOLOGY.md)。

## 验证状态

运行时开关的本地 stub 检查覆盖未设置/非法值/精确值1、关闭→开启→关闭的时钟和日志调用行为、退出清除启用状态、延迟输出、提前返回、时钟失败/逆序和缓冲溢出。stub 检查不代表 NPU 验证。

配套采集框架的本地合成数据检查覆盖逐段/END 审计、Host 日志及 stdout 镜像、旧 schema 兼容、统计口径、Excel 导出及进程失败记录。此前 AIC/AIV Client 算子已编译通过；本次 Host 探针重构尚未完成完整库/MPI 构建、配套包安装及 NPU 验证，不沿用 AICPU 文档中的上板结论。

新逐段日志须搭配 `measurement_schema=2` 的 CCU perf_acceptance；旧单行 schema=1 结果仍可按原口径解析，不与新格式混用。CCU 的 schema 独立于 AICPU schema。

从 asc-devkit_2 根目录执行本地探针检查：

```bash
python3 -m unittest discover -s tests/python -p test_mc2_ccu_perf_probe.py
```

## 当前采集范围

当前仅保留 Client 的 Init/Prepare/Commit/Wait/Finalize 接口计时，以及 Host 的资源准备和三个提交区间计时。Client 计时位于配套测试程序的 AIC/AIV 算子中，Host 计时位于本探针及 Host 测试程序中。

KFC 初始化、收到消息后的处理、SQE/cache 诊断及算法执行阶段计时不在本次采集范围内，后续单独设计。`PollBegin/Parsed/Submitted` 在此处仅表示 Host 资源上下文加载、参数加载和提交完成边界，不表示 KFC 内部执行阶段。`Receive()` 及消息序号/cache 状态占位已移除。
