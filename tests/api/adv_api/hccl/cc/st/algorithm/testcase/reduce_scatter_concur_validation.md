# MC2 ReduceScatter SoleMeshConcur 迁移与验证

## 基线与范围

- asc-devkit 基线：`1a201e7660d844258886fc35b33e6ff859211cf8`。
- HCCL 行为参考：`58311980f575bc9d870e6990b25ebecb2b6f2448`。
- 分支：`feat/mc2-reducescatter-sole-mesh-concur`；本轮未提交、未推送。
- 仅修改 asc-devkit。没有修改公共 API、序列化结构、类成员布局或 hcomm。

## 实现差异

| 项目 | 迁移前 | 迁移后 |
|---|---|---|
| 注册名 | InsReduceScatterMesh1DZAxisDetour | 仅保留 AicpuReduceScatterSoleMeshConcur，删除旧名称注册 |
| AICPU 自动选择 | 双层物理网络，总量 >1536 MiB 选择 Detour | 双层物理网络，2P 单 rank 输出 >=4 MiB 优先选择 Concur；其他对应大数据分支选择 MeshChunk |
| 分流 | 2P 固定 0.25，其余 0.5 | level0 权重为 templateRankSize-1，level1 按 portGroupSize 分配 |
| 尾块 | 固定比例拆分 | 保持上游元素对齐策略，最后 channel 承接余量 |
| 资源恢复 | 单 channel 时可能保留旧层级计数 | 每次恢复重新设置两层数量，检查 peer/channel 数量 |
| 可选物理层缺失 | 仍继续查询该层 links | 直接跳过；必需层缺失仍报错 |
| PostCopy 失败 | KernelRun 忽略返回值 | 向调用方传播错误 |

逻辑 topology 单层与物理网络两层是两个概念，不可混淆。新名称的 Sole executor
资源预检均限制为支持的完整 Mesh、单层逻辑拓扑、AICPU/AICPU_TS，排除 64-bit/PROD。
自动选择额外排除纯 Host-DPU；未修改 CCU selector 的优先级。

保留 MC2 的 mc2_ops_hccl 命名空间、strideCount、输入地址/容量校验及本地 executor。
不迁移 CostModel、对称内存、公共框架、HCCL 的全部循环容量策略。
没有新增 join/fence/notify，也没有改变 PreSync -> 数据任务 -> PostSync -> PostCopy 顺序。
旧算法名称不再注册；指定算法时须使用 AicpuReduceScatterSoleMeshConcur。内部 Detour 模板类名不变。
sequence executor 复用该模板，因此会采用新的分流权重，但其算法选择和层级编排未修改。

## 已执行验证

2026-09-22，在 A5_4p 的 cann-compile 专用工作区：

| 验证 | 结果与边界 |
|---|---|
| git diff --check | 通过 |
| 生产包 build.sh --pkg -j32 | 未通过：未修改的 tiling 代码使用 NpuArch::DAV_5162 等枚举，环境头文件缺失对应定义 |
| mc2_client | 生产构建中编译、链接成功 |
| mc2_server | 单独构建 device target，AArch64 编译、链接成功；不是整包验收 |
| 隔离 primitive 测试 | 15 项通过，含新增 SoleMeshConcur 多 channel、多循环、stride 数据语义测试 |
| 算法测试 target | 79 项通过，含新增 5 项 Concur 选择、分流、资源恢复、拒绝检查 |
| NPU 精度、长稳、性能 | 未执行，不以 CPU 仿真结果替代 |

新增仿真覆盖 2/4/8 rank、INT8/FP16/BF16/FP32、不同 count/stride、三条 channel、
不同权重、尾块和循环切换。按真实算法展开结果模拟 thread 队列及通知依赖，
验证无任务依赖死锁、规约值正确和输出保护区完整。
数值模拟不是硬件浮点精度标准，也不能验证真实 transport 的可见性或带宽。

分流测试另覆盖零数据、无 level1、无效 dtype 大小、溢出、零权重及非法层级数量。
选择测试覆盖 4 MiB 前后、1536 MiB 总量边界、64-bit/PROD 和缺少第二物理层。

## 复跑方式

在已配置 CANN 和测试依赖的仓库根目录：

```bash
cmake -S . -B build_concur_test \
  -DENABLE_TEST=ON \
  -DTEST_MOD=ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC \
  -DASCEND_CANN_PACKAGE_PATH="$ASCEND_HOME_PATH" \
  -DCANN_3RD_LIB_PATH="$PWD/build" \
  -DCMAKE_BUILD_TYPE=Debug
cmake --build build_concur_test \
  --target ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC -j16
```

该 target 的依赖会运行隔离 primitive 测试，随后运行算法测试。任一测试失败应阻止验收。

## 板上待验收

- 在具备两层物理网络的 2P 环境验证 >=4 MiB 自动选择；普通 2P 不代表具备该拓扑。
- 指定 AicpuReduceScatterSoleMeshConcur 验证 2/4/8P；日志确认本地 Detour 模板、两层真实 channel 和分流数量。
- 同通信域反复切换 shape/dtype/stride，覆盖尾块、循环边界和资源重复创建销毁。
- 验证 PCIe mixed CLOS、非 PCIe CLOS、纯 Host-DPU、缺少可选/必需物理层及强制不支持算法的行为。
- 回归 sequence Detour、多层网络、普通 Mesh、MeshChunk 与 AllGather channel 申请。
- 精度通过后再比较性能；生产整包成功及目标网络 NPU 验证完成前，不声明完整交付。
