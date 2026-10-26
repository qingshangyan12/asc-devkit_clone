# [WIP] 双包 Server 打点性能对照（仅实验，不得合入）

共同原始基线 9dfffb2ba02d3f1fb80e1f4e6b7fc4e4d1a92158，两个包均保留该基线中的 Thread 定义，包含 10 月 23 日与 HCOMM 配套合入的接口变更。使用包含配套 HCOMM 变更的黄区环境，不再回退适配 A5_4p 的 20261021 toolkit。

A：只有外层测试计时。B：外层区间内包含 thread_local POD 三阶段状态重置、时间戳读取、计算和一条 MC2_PERF RUN_INFO。
两者外层均从 Mc2ServerKernel 入口读钟，到主流程（B 包括 Finish 日志返回）之后读钟；MC2_AB_TEST 日志和序号递增在区间外。错误返回也记录 status。
B 保留 48 字节 POD，无自定义构造/析构、无记录数组；每线程首次 TLS 初始化与每次入口重置不同。热态统计不代表一次性初始化成本。

每个 kernel 一次 collective，1 次冷启动、20 次 warmup、100 次热态，重复 10 轮。仅热态参与统计，AB、BA 两个 block，按同序号轮均值计算 B-A。外层包含等消息与通信等待；差值是净增量而非纯 CPU 指令成本。日志异步落盘可能在区间外；Host launch_to_complete 作为辅助。

需要分别出新的带签名安装包。旧 ABCD 包没有 MC2_AB_TEST 日志，不适用于此脚本。构建/签名包/NPU 验证分别记录，未签名构建不代表板测通过。
