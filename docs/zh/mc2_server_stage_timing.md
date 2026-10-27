# A5 Server 三段 mean 计时

通过 Mc2CcTilingConfig::SetDebugMode(4) 开启，默认关闭。GetTiling 将 debugMode 写入 init tiling；Host 在 HcclAllocOpResCtx 中写入 OpResCtx.res[Mc2ServerDebugConfig::DEBUG_MODE_RES_INDEX]（索引3），保留结构布局及 version 语义。res[0..2] 保持原有 CCU 用途。无 tiling 的资源入口保持0，不开启测量。

当前支持单context、每kernel一次通信的统计。多context入口不启用；多消息、重复展开、计时异常或通信错误通过批次status标记为无效。开关关闭时各打点入口不读取时间戳。

阶段：init（启用检查后到消息循环开始）、msg_parse（循环开始到接收解析完成，含等消息）、sq_expand（解析完成到CCoreWait/算法编排/CCoreRecord提交完成）。

固定thread_local POD保存当前四个时间戳、有效性与cache状态，另保存三个uint64_t累计和、批次范围及计数。无样本数组、排序或动态分配。每30次输出一条RUN_INFO，位于CommKfcDispatcher::Run返回之后：

```text
[MC2_SERVER_PERF] schema=2 first=31 last=60 count=30 init_mean_ns=100.000000 msg_parse_mean_ns=200.000000 sq_expand_mean_ns=300.000000 cache=0 status=0
```

cache为0 miss、1 hit、-1未知、2混合。status非0整批无效。均值为ns，保留6位小数；不提供分位数。日志和统计位于三段区间外，仍影响第30次kernel整体延迟。

计数按同一线程连续同一OpResCtx，关闭或切换context丢弃未满批次；launch序号为该线程启用期间累计序号，不因清任务缓存重置。不足30条不输出。进程退出不补打印尾批。

配套hccl_api_test feat/perf-acceptance使用schema7：每轮新MPI进程，1冷启动+29warmup（首批丢弃），逐次清缓存后30次miss，再30次hit；共10轮。清缓存kernel不参与计数且不计入耗时。三段均按cache分别汇总mean，默认8卡每种状态n=2400。

验证：tests/server_stage_timing/run_mock.py为本机C++14日志契约检查，非目标CANN编译或NPU验证。目标签名包由用户出包后验证。
