# 内存一致性未验证结论

本文档记录`docs/zh/guide/编程指南/高级编程/内存模型/内存一致性.md`中尚未通过本目录样例直接验证的结论。未列入本文档的结论不代表已经被完整覆盖，只表示已有样例或官方资料足以支撑当前表述。

## 已用本目录样例覆盖的结论

- `memory_consistency_demo_c_api`验证同一Stream中前序Kernel写入后，后序Kernel能够读取到该写入。
- `memory_consistency_demo_c_api`验证不同Stream之间通过Event建立依赖后，等待方Stream能够读取到被等待方Stream发布的数据。
- `memory_consistency_demo_c_api`验证同一Stream内异步H2D拷贝先于后序Kernel完成，Kernel能够读取到Host异步拷贝写入的数据。
- `demo_c_api`和`scalar_gm_211_demo_c_api`验证普通Scalar访问GM时，内存顺序需要配合DCCI/DSB处理DCache可见性。
- `ub_dcache_241_demo_c_api`验证`CTRL[49]`影响Scalar访问UB路径，UB DCCI后读端可获取Vector写入的新值。

## 尚未直接验证的结论

| 结论 | 未验证原因 | 后续验证方向 |
| --- | --- | --- |
| 跨流水缺少`asc_sync_notify/asc_sync_wait`时，Vector可能读取到MTE2尚未搬入完成的UB数据 | 负例依赖流水时序，不一定稳定复现 | 构造大规模MTE2搬入和紧随其后的Vector校验，统计无同步时的错误率 |
| Double Buffer反向依赖缺失时，下一轮搬入可能覆盖上一轮仍在计算或搬出的UB数据 | 需要构造高并发流水重叠，且错误受时序影响 | 使用ping/pong复用同一UB区域，分别关闭`V->MTE2`或`MTE3->V`同步并统计错误 |
| `asc_sync_inter_arrive/asc_sync_inter_wait`在复杂Kernel类型下的核间同步行为 | 该接口对Kernel类型、flag_id、pipe和资源独占有约束 | 单独构造满足接口约束的多核Kernel，覆盖组间同步和flag_id复用场景 |
| 原子操作不能提供完整acquire/release或seq_cst语义 | 当前样例只覆盖原子与DCache混用的一部分风险 | 构造“普通数据+原子标志”的发布/获取协议，对比有无DCCI/DSB和同步等待 |
| Runtime内存语义同步`aclrtValueWait/aclrtValueWrite`与Kernel轮询同一Device内存的交互 | 本目录暂未新增Runtime内存语义同步样例 | 新增双Stream样例，一个Stream下发ValueWait，另一个Stream下发Kernel或ValueWrite解除等待 |
| 默认Stream与显式Stream之间没有隐式同步 | 需要Host侧多Stream调度样例，且负例可能受调度影响 | 构造默认Stream和显式Stream分别读写同一Device内存，对比加入Event前后的结果 |
