AscendC::Hccl::AllReduce：融合算子 Kernel 调用 AllReduce 高阶 API，传入运行时输入输出地址、count、数据类型和归约类型。
-> HcclImpl::AllReduce：校验归约类型，并构造 HCCL_CMD_ALLREDUCE 通信参数。
-> CommonPrepareImpl：分配通信 handle，将通信参数组装成消息，并记录 repeat、消息位置和通信类型。
-> SendMsgToServer：将 HcclMsg 写入 AICore 与 KFC/AICPU server 的共享消息区。
-> SetCommitTurnCntToGm：当调用 AllReduce<true> 时直接更新提交计数，触发 server 执行该通信任务。
AscendC::Hccl::Commit：当调用 AllReduce<false> 时，由 Kernel 显式提交指定 handle 对应的通信任务。
-> HcclImpl::Commit：校验 handle，并通过 SetCommitTurnCntToGm 更新共享消息区中的提交计数。
KFC/AICPU server 消息轮询：持续检查共享消息区中是否出现 AICore 写入的有效 HcclMsg。
-> GetCurrentMsg：从共享消息区读取当前待处理的 HcclMsg。
-> PrepareProcess：校验消息序号，并将有效通信消息交给 server 编排。
-> CommKfcAicpuServer::Orchestrate：根据消息中的 opParamKey 匹配资源上下文，并按 repeat 次数执行通信任务。
-> FormatOpParamFromMsg：使用 Host 侧保存的基础 OpParam 和当前 HcclMsg 构造本次运行参数。
-> FormatOpenOpParamDataFromMsg：处理 repeat 偏移，并将消息字段更新到运行参数。
-> CreateOpParamByBaseOpParam：写入 sendBuffer、recvBuffer、dataCnt、dataType、reduceType 和 stream。
-> LaunchOpenCcoreWait：等待 AICore 通过 AllReduce<true> 或 Commit 更新提交计数。
-> LaunchOpenAicpuKernelServer：使用运行参数和已恢复的资源上下文启动本地 AICPU 算法。
-> useCannBridge：按算子类型判断 Device 执行是否交给外部 CANN。
-> false：使用本地 mc2_ops_hccl registry；当前 AllReduce 走此分支。
-> CollAlgExecRegistryV2::GetAlgExec：根据 HCCL_CMD_ALLREDUCE 和 InsAllReduceMesh1DTwoShot 获取 executor。
-> RunWithMc2TaskCache 走task cache
-> HcommBatchModeStart 用于开启批量模式，在HcommBatchModeStart和HcommBatchModeEnd之间的所有数据面接口调用（如HcommLocalCopy、HcommWrite等）将被缓存，不会立即执行。所有操作将在调用HcommBatchModeEnd时统一提交并执行。
-> BuildMc2AicpuTaskCachePlan 判定能否启用任务缓存（首次执行时捕获底层 hcomm 原语调用序列，之后同 tag 直接 replay，跳过重新编排），并生成缓存计划 Mc2AicpuTaskCachePlan
-> 如果无法开启cache 每次走func() 真实执行
-> Mc2AicpuTaskCacheLookup 查询缓存
-> isHit=true 命中路径（重放）
-> Mc2AicpuTaskCacheExecute 重放  -> HcommAicpuTsTaskCacheExecute
-> isHit=false 未命中路径（首次捕获）
-> Mc2AicpuTaskCacheStart 启动捕获
-> func() 真实执行一次（同时被捕获）
-> InsV2AllReduceSoleExecutor::Orchestrate：恢复 channel 和 thread，并读取运行时 count 与数据类型。
-> InsV2AllReduceSoleExecutor::OrchestrateLoop：根据 HCCL buffer 容量计算单轮 count，循环处理全部数据。
-> InsTempAllReduceMesh1DTwoShot::KernelRun：切分当前轮数据，依次执行 ReduceScatter 和 AllGather。
-> InsTempAllReduceMesh1DTwoShot::RunReduceScatter：交换各 rank 的目标切片，并在本地完成归约。
-> InsTempAllReduceMesh1DTwoShot::RunAllGather：交换已归约的数据片，生成每个 rank 的完整输出。
-> Mc2AicpuTaskCacheEnd 发布缓存；失败则清缓存
-> HcommBatchModeEnd
-> LaunchOpenCcorePost：更新完成计数，通知 AICore 当前 repeat 的通信任务已经完成。
AscendC::Hccl::Wait：等待完成计数达到预期值，确保 AllReduce 输出可以被后续计算安全使用。