# MC2 Server 性能打点（WIP）

配套测试与解析：[hccl_api_test PR #21](https://gitcode.com/jiangxinyu3/hccl_api_test/merge_requests/21) 的 `perf_acceptance`。本功能无需额外 CMake 参数，按普通流程构建包含本修改的 asc-devkit 包即可。需要安装通过目标设备签名校验的配套 Client/Server 包。

## 运行时开关

在执行测试的每个 Host rank 进程环境中设置：

```bash
export MC2_PERF_ACCEPTANCE=1
# 按 perf_acceptance/README.md 启动 client 模式测试
```

MPI 启动器需将该变量传给所有 rank。仅精确字符串 `1` 开启；未设置、`0`、空字符串及其他值均关闭。关闭示例：

```bash
unset MC2_PERF_ACCEPTANCE
```

变量在 Host 每次组装 Server launch 参数时读取，不依赖 Device 进程继承 Host 环境变量。

- 关闭：args[0] 保持原 `MC2_AICPU_SIMPLE_CTX_PROTOCOL`。
- 开启：args[0] 编码为 `CommKfcParamDesc`，version=15、itemNum=1；验收 cache 策略见下文，其余字段清零。
- args[1] 仍为 Device OpResCtx 地址，参数块长度及 OpResCtx 布局不变。
- Device 仅在普通描述符 version=15 时开启本次 kernel 的探针；其他版本和旧 simple-context 协议关闭。退出时清除线程启用状态。

15 是本功能保留的协议版本，不能作为其他描述符用途重复分配。旧版 Mc2ServerKernel 可能继续执行而忽略此版本的打点语义；无日志必须判为未覆盖，不能宣称采集成功。该约定限于 Mc2ServerKernel，不适用于其他 AICPU 入口。

## 指标与限制

输出 `MC2_PERF` 和 `MC2_PERF_END`，包括 server_entry_to_poll、server_poll_to_parsed、server_parsed_to_submit。以 AICPU CLOCK_MONOTONIC 计时，不与 Host/AICore 时间直接相减。记录真实 cache hit/miss，unknown/mixed 不作为有效命中分类。每线程最多 2048 条，溢出通过 dropped 明示。

关闭时跳过探针读时钟、记录和输出，但仍有运行时分支和线程局部启用状态访问，不宣称零开销。开启时 kernel 退出才打印记录。launch 是线程局部的启用采集次数，不是 MPI iteration；需按实际 PID/device/worker 与测试轮次映射。

当前验收程序按每 kernel 一次 Prepare/repeat=1 使用。entry_to_poll 为入口到 while 轮询开始；poll_to_parsed 包含等待 client 消息、读取以及 FormatOpParamFromMsg 参数解析；parsed_to_submit 从解析成功返回到 LaunchOpenCcorePost 返回，包含 CCoreWait、资源/DFX、算法编排/cache 和 CCoreRecord 提交，不代表通信执行时间。需对照无探针运行评估扰动。

## 验证状态

运行时开关的本地 stub 检查覆盖未设置/非法值/精确值1、version 0～15、单/多 context 数量传递、空参数，以及关闭→开启→关闭的时钟和日志调用行为。stub 检查不代表 NPU 验证。

此前上板加载未签名打点包被拒绝（cms verify failed / Package has no header），Server 指标尚未取得实测样本。运行时开关不取消设备签名要求；需要合规签名的配套包后验证日志完整性、cache hit/miss 和探针扰动。

新三段格式须搭配 measurement_schema=2 的 perf_acceptance；旧四段日志不能作为新三段数据使用。本次修改尚未完成签名包及 NPU 验证。

## 受控热态缓存对照

Host 设置 `MC2_PERF_ACCEPTANCE=1` 且 `MC2_PERF_CACHE_POLICY=paired` 时，在 version=15
描述符的 isDyn bit0 编码 paired；仅该验收版本、单 context 使用此语义，结构体大小不变。
未指定/非 paired 或未开启打点时保持自然缓存路径。

真实 hit 成功提交后暂存 tag 和清理回调（移动 tag，不新增缓存 key）。客户端 Query/Wait 完成后发送 Finalize，
dispatcher 正常退出 while 循环后才调用清理 API，并解除 manager 登记；异常退出不清理可能仍在使用的缓存。
miss 捕获后不清理，下一轮可 replay；hit 后清理，下一轮可重新 capture。三段时间戳不包含清理调用，
记录回调/tag 的探针开销未扣除。清理返回非零则 Server 返回错误，不静默退化。

每 launch 输出 `[MC2_PERF_CACHE_CONTROL] launch=... policy=paired cleared=0|1 status=...`。
新脚本要求每组进程每 rank 热态覆盖两种真实 cache 状态；不支持 cache 的算法、未适配旧包、日志缺失均会报不完整。
该控制仅服务每 kernel 一次 collective、repeat=1 的验收载荷，不适用于多 context/多消息使用方式。
