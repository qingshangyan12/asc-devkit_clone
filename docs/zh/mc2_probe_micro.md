# [WIP] 三阶段打点微基准，仅实验不得合入

基于 9dfffb2ba02d3f1fb80e1f4e6b7fc4e4d1a92158，不包含旧 toolkit ABI 回退，不修改实际通信阶段。Mc2ServerKernel 校验 args 后、读取通信上下文前，每个线程首次进入时执行微基准，随后继续原通信流程。

单包内 A 只包含外层读钟和编译器屏障；B 在相同边界内调用真实 Start/PollBegin/Receive/Parsed/Cache/Submitted/Finish，包括 POD TLS 重置、四次内部读钟、计算、校验及一条 RUN_INFO。参数固定为一次成功消息和 cache hit，不实际执行三阶段，不调用通信函数。虚构阶段时间仅用于执行日志路径，不可作为业务阶段耗时。

每组先20对warmup，再100/1000对正式样本，AB/BA交替。样本缓冲区在测量区间外，逐条微基准结果在整组测量结束后打印。warmup排除首次TLS/日志初始化；一次性初始化成本不在热态结果内。仅最后一条实际打点日志在B区间内。

每进程每卡预期一套数据，重复线程/缺失日志由解析器拒绝，避免错误混合。每个rank独立测量，跨卡同时打印可能影响日志压力。100与1000组对比用于观察连续日志压力，异步后台落盘全部成本不一定包含。该值不代表通信端到端增量。

使用 hccl_api_test feat/perf-abcd-20261024 的 perf_abcd_tmp 单包脚本。安装包放 package_root/M；python3 compare.py --all 自动安装、构建、运行和解析。新签名包和目标构建/板测待黄区验证。
