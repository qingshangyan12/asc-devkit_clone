# Server 三阶段计时

## 范围与日志位置

默认采集，每次 Mc2ServerKernel 返回前一条 HCCL_RUN_INFO；不使用 version、环境变量或编译期开关，也不修改传参 ABI。后续正式开关单独设计。

Mc2ServerKernelBody 的简单上下文和普通描述符两条路径都返回 CommKfcDispatcher::Run 的结果。Run 内 while 循环退出并完成局部对象析构后才回到入口包装函数，Finish 在此计算并打印。提前报错也输出一条无效记录，原有返回值保持不变。日志位于所有三段区间外，但仍在 kernel 返回前，日志调用仍增加整体执行耗时，不声称零扰动。

| 字段 | 起止边界 |
|---|---|
| init_ns | kernel入口Start读取时间戳 → CreateServerList后、消息while循环前 |
| msg_parse_ns | while循环前 → 首次消息FormatOpParamFromMsg成功之后；包含等待消息与解析 |
| sq_expand_ns | 解析成功 → LaunchOpenCcorePost返回后；包含CCoreWait、算法编排/缓存路径、CCoreRecord提交 |

内存为每线程48字节POD及launch序号，没有记录数组、动态分配或跨kernel累计/定期刷日志。每次入口重置控制字段，时间戳由valid位保护。

## 记录契约

`[MC2_SERVER_PERF] schema=1 launch=... messages=... valid=... init_ns=... msg_parse_ns=... sq_expand_ns=... cache=... status=...`

launch是线程内从1开始的调用序号，不是全局kernel ID，Host采集需按PID/device/worker绑定。valid=15且status=0表示四个时间戳齐全、顺序正确、一次消息/一次展开。messages是实际接收消息数。多消息、多repeat、错误/不支持的路径保留通信行为，记录无效状态，不能用最后一次时间戳冒充单次基准。日志中status为测量有效性状态，不替代kernel原返回码。

cache=0：实际缓存lookup miss；1：hit；-1：未完成可分类lookup（如缓存不支持/被禁用/回退）；2：混合标签。只观察现有cache行为，不新增清理或缓存控制。不将unknown归为miss。

## 验收与依赖

配套 hccl_api_test feat/perf-acceptance，measurement_schema=5，读取单条记录并计算us。仅热态汇总；自然缓存miss/hit分开，缺少类别留空，unknown/mixed独立保留。原三条记录+END、paired cache、version=15探针不能混用。不依赖HCOMM PR #6198或原#6763；需要与当前master接口配套的正常CANN/HCOMM包。

本地POD/日志契约测试和框架模拟测试不代表目标Linux编译/签名包/NPU通过，需黄区出包验收。单条RUN_INFO有已观察到的成本；本PR按要求保留默认输出，正式开关另行设计。
