# [WIP] AICPU 三阶段打点 ABC 微基准，仅实验不得合入

基于 9dfffb2ba02d3f1fb80e1f4e6b7fc4e4d1a92158，无旧 toolkit ABI 回退。Mc2ServerKernel 校验 args 后、通信处理前，每个线程首次进入时测量，随后继续原通信流程。

A：空基线，只有外层读钟和编译器屏障。
B：POD TLS重置、真实Start/PollBegin/Receive/Parsed/Cache/Submitted/Finish接口、四次内部读钟、耗时计算与校验，不打印。
C：与B相同，再打印一条RUN_INFO。
B/C使用同一不可内联Finish函数，以bool控制日志；两者返回相同Totals结构，在终点前用编译器屏障约束结果生成，终点后读取并写入测试用volatile sink。结果存储/保留的共同成本不能解释为纯生产环境指令成本。

固定20组warmup + 100组正式样本，只保留100组。每组轮换ABC、ACB、BAC、BCA、CAB、CBA六种顺序；100不能整除6，顺序组数相差不超过1。测量区间不执行实际通信阶段；模拟状态为一次成功消息、cache hit。warmup排除首次TLS/日志初始化。

结束全部测量后输出protocol=2的100条MC2_PROBE_MICRO，每条包含a_ns/b_ns/c_ns。仅C输出探针日志，含warmup共120条。B/C共享launch序号，因此C序号非连续，由实际顺序推导并审核。每rank/PID预期恰好一套数据，重复worker或缺日志拒绝汇总。

指标逐组计算：B-A为probe_overhead；C-B为log_overhead；C-A为total_overhead。保留负值，分别统计mean/p50/p90/p99；连续日志压力、异步后台和跨卡并发影响不等于实际通信端到端增量。

操作者安装PR #6984新签名包并设置CANN/MPI/日志环境。hccl_api_test feat/perf-abcd-20261024，perf_abcd_tmp内运行python3 compare.py --all；只继承当前环境，不安装包。旧protocol=1包及结果不能用于当前解析器。

本地Python回归、C++14模拟日志及RunOnce检查通过，不代表目标Linux构建或NPU性能；黄区签名出包和板测待验证。
