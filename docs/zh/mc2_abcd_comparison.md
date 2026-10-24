# [WIP] ABCD B — 仅用于性能对照，不合入

共同基线：`9dfffb2ba02d3f1fb80e1f4e6b7fc4e4d1a92158`（2026-10-24 拉取的 upstream/master）。
本分支直接基于该提交，不以其他对照分支为父提交。线上按默认构建参数出独立签名包。

本版本：48 字节 POD TLS 状态，四次读钟、计算与 volatile 结果保留；不打印统计。

A/B/C/D 均只用于性能对照。B/C 的探针头文件只差最终日志调用；包身份字符串按版本不同。
使用自然缓存，不开启 paired、不修改算法编排，不依赖 HCOMM PR #6198，不使用 version 或环境变量开启计时。
B/C/D 默认执行探针；A 没有逐次探针调用。共同的 module constructor 仅在 SO 加载时输出一次
`[MC2_ABCD_BUILD] variant=B base=... protocol=1`，用于核验实际加载版本；该共同开销不在热态逐次路径。
构造函数运行时机和日志归属仍需目标包验证，缺少标识时框架拒绝采集结果，不凭安装路径推断已加载。

D 取自此次简化前的本地三段数组探针快照，header SHA256=`1b3853ce3eeca001e2913fed79343dc44b377ac60f232a18712252a66da4f1a3`；
不是带 HCOMM count/detail 的旧包。D 的 paired 功能未启用，所有版本均保持自然缓存路径。
B/C 只对每 kernel 一次 collective 输出有效性能；多消息会标记无效，但通信流程仍执行。

临时对照框架：hccl_api_test 分支 `feat/perf-abcd-20261024` 的 `perf_abcd_tmp/`。
同一个测试二进制依次测四套安装包，采集 Host 外部 launch→两个 stream 完成的耗时及完整热态循环耗时。
这些是外部可观察增量，不等于纯 AICPU 执行耗时。B−A、C−B、C−A、D−C 按独立进程轮次计算。
正式 perf_acceptance 不用于这四个包的解析。

已做本地 C++14 stub 编译/链接及日志条数检查、B/C 源码差异检查；目标 Linux/CANN 全量构建、
线上签名出包、安装和 NPU 实测均未验证。此 PR 不包含性能收益结论。
