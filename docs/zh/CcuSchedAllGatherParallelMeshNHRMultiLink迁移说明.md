# CcuSchedAllGatherParallelMeshNHRMultiLink 迁移说明

## 1. 迁移目标

本次迁移把 HCCL 主仓中的 `CcuSchedAllGatherParallelMeshNHRMultiLink` 接入 asc-devkit 的 MC2 CCU KFC Server 路径。目标不是只注册一个算法名，而是打通以下完整链路：

```text
AllGather tiling / selector
        ↓
Parallel executor：识别 Mesh × CLOS 两级 rank
        ↓
Mesh CCU template + NHR CCU template：申请 channel / kernel 资源
        ↓
KFC Server：把两个普通 CCU kernel 替换成两个常驻 mission
        ↓
Ascend C Hccl client：把本次 AllGather 参数写入 XN 请求槽
        ↓
mission 0（Mesh）  ║  mission 1（NHR）
       phase 0 并发，完成后通过 GM flag 汇合
       phase 1 并发
        ↓
KFC Server 写完成 CKE，Hccl::Wait(handle) 可以继续
```

上游算法注册入口：

- [HCCL Parallel 算法注册](https://gitcode.com/cann/hccl/blob/master/src/ops/all_gather/algorithm/executor/ins_v2_all_gather_parallel_executor.cc)
- [HCCL AllGather selector](https://gitcode.com/cann/hccl/blob/master/src/ops/all_gather/selector/all_gather_auto_selector.cc)

asc-devkit 迁移入口：

- [asc-devkit Parallel executor](../../impl/adv_api/detail/hccl/cc/src/ops/all_gather/executor/ins_v2_all_gather_parallel_executor.cc)
- [asc-devkit AllGather selector](../../impl/adv_api/detail/hccl/cc/src/ops/all_gather/selector/all_gather_auto_selector.cc)

## 2. 算法本身在做什么

设总 rank 数为：

```text
P = M × N

M = rankSizeLevel0：一个 Mesh 组中的 rank 数
N = rankSizeLevel1：同一 Mesh 位置对应的 CLOS/NHR 组数

rankIdxLevel0 = rankId % M
rankIdxLevel1 = rankId / M
```

每个 rank 的输入切成两部分：`part0` 和 `part1`。算法不是简单地同时跑一次 Mesh 和一次 NHR；它有两个存在数据依赖的阶段：

```text
                       phase 0
part0 ── Mesh：组内汇聚 ───────────┐
                                  ├─ barrier
part1 ── NHR：跨组同位置汇聚 ──────┘

                       phase 1
已按 Mesh 汇聚的 part0 ── NHR 跨组传播 ──→ 所有 rank 都有完整 part0
已按 NHR  汇聚的 part1 ── Mesh 组内传播 ─→ 所有 rank 都有完整 part1
```

更接近执行代码的表达是：

```cpp
// mission 0: Mesh
MeshAllGather(part0, localMesh);
meshDone = 1;
wait(nhrDone == 1);
MeshAllGatherForEachClosGroup(part1AlreadyGatheredByNhr);

// mission 1: NHR
NhrAllGather(part1, sameMeshPositionAcrossGroups);
nhrDone = 1;
wait(meshDone == 1);
NhrAllGatherForEachMeshPosition(part0AlreadyGatheredByMesh);
```

phase 0 中两条路径使用数据的不同半区，因此可以并发；phase 1 必须等两条 phase 0 路径都完成，否则会传播尚未收全的数据。

对应实现：

- [Mesh 两阶段 CCU kernel](../../impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/ccu/kernel/ccu_kernel_kfc_all_gather_mesh1d_mem2mem.cc)
- [NHR 两阶段 CCU kernel](../../impl/adv_api/detail/hccl/cc/src/ops/all_gather/template/ccu/kernel/ccu_kernel_kfc_all_gather_nhr1d_multi_jetty_mem2mem.cc)

## 3. 选择条件

HCCL 主仓选择该算法的核心条件为：

| 输入 | 条件 |
|---|---|
| engine | `CCU_SCHED` |
| level-0 topology | UBX、非 PCIe mix 的 `MESH_1D_CLOS` |
| 单 rank 数据量 | `> 512 KiB` 且 `< 16 MiB` |
| 拓扑比例 | CLOS 数是 Mesh 数的整数倍 |
| 特例排除 | Mesh 数等于 CLOS 数且总 rank 数不超过 4 时，仍选已有 Concurrent 算法 |

asc-devkit 当前还没有 HCCL 主仓对应的 `PipelineMeshNHR` 和 `SoleNHRMultiLink` KFC 迁移版本。因此，大于等于 16 MiB 或 CLOS/Mesh 不整除时，CCU 分支返回 `NOT_MATCH`；外层 `AutoSelectorBase` 随后回退到已有 AICPU 算法，不能静默错选 Mesh-only 算法。

## 4. 从 tiling 到 CCU mission 的调用链

### 4.1 注册与资源分配

1. [hccl_tiling.cpp](../../impl/adv_api/tiling/hccl/hccl_tiling.cpp) 把算法名加入可用 CCU 算法集合。
2. [hccl_alloc_ctx_res.h](../../impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.h) 把字符串映射到 `AlgorithmType::CcuSchedAllGatherParallelMeshNHRMultiLink`。
3. [Parallel executor](../../impl/adv_api/detail/hccl/cc/src/ops/all_gather/executor/ins_v2_all_gather_parallel_executor.cc) 使用 `TopoMatchUBX` 得到：
   - level 0：当前 rank 所在的本地 Mesh；
   - level 1：跨 Mesh、但 `rankIdxLevel0` 相同的 rank 集合。
4. executor 分别调用 Mesh/NHR template 的 `CalcRes`，生成两份 CCU kernel/channel 资源。
5. [KFC Server template](../../impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/ccu_temp_kfc_server.cc) 保留这两份资源，但把 kernel 入口替换成两个常驻 KFC mission，并把两级 rank size/index 传给 mission。

HCCL 主仓使用 `TopoMatchTwoLevel`。asc-devkit 当前没有这个类；其已有 `TopoMatchUBX + Parallel executor` 已经把 `MESH_1D_CLOS` 的单层描述拆成等价的 Mesh/CLOS 两个 rank 集合，因此这里复用现有框架，而不是另造一套拓扑抽象。

### 4.2 AICore client 发布请求

[CcuPrepareForParallelAllGatherM2M](../../impl/adv_api/detail/hccl/ccu/hccl_ccu_v0_prepare.h) 由 Ascend C `Hccl::AllGather/Commit` 路径调用。它完成：

1. 按元素个数一分为二，保证不会从一个元素的字节中间切开；
2. 计算 repeat 对应的输入/输出基址和 rank 输出 stride；
3. 计算 Mesh/NHR 搬运的 go-size；
4. 清零两个 phase flag；
5. 把 32 个 XN 槽写入 KFC 请求队列。

协议在 [kfc_server_message.h](../../impl/adv_api/detail/hccl/common/kfc_server_message.h) 中集中定义：

| XN 槽 | 内容 |
|---:|---|
| 0 | op id/token |
| 1–2 | input/output GM 地址 |
| 3–7 | 总大小、stride、两个分片大小及偏移 |
| 8–9 | 两个 phase flag 的 GM 地址 |
| 10–17 | part0/part1 的四组 go-size |
| 18–21 | NHR 单 jetty 的普通/尾块大小 |
| 22–29 | 保留 |
| 30–31 | Mesh/NHR phase-0 完成 flag 的实际存储 |

两个 flag 放在同一请求记录的保留 XN 槽中，因此其生命周期覆盖两个 mission 的本次执行，也不会额外占用一个不受 KFC 队列管理的 workspace。

### 4.3 KFC Server 消费请求

[kfc_server_protocol.h](../../impl/adv_api/detail/hccl/cc/src/common/kfc_server_protocol.h) 指定该算法有两个 mission：mission 0 为 Mesh，mission 1 为 NHR。

[KFC Server dispatch](../../impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.cc) 从请求记录加载 XN 参数，然后按 mission role 分派到两个两阶段 kernel。两个 kernel 都复用原子 Mesh/NHR template compiler 实现中的：

- channel 资源初始化；
- `PreSync/PostSync`；
- CCU `Read/Write/Wait` 指令序列构造；
- repeat 和 go-size 展开。

迁移新增的只是两阶段的数据布局、repeat/stride 接线以及两个 mission 间的 phase barrier，没有复制一套底层 channel 协议。

## 5. 关键适配决策

| 项目 | 处理方式 | 原因 |
|---|---|---|
| executor | 复用 `InsV2AllGatherParallelExecutor` | 保留 HCCL 的数据二分与两级拓扑语义 |
| topology matcher | `TopoMatchUBX` | asc-devkit 已能产出等价 Mesh/CLOS rank 集合，没有 `TopoMatchTwoLevel` |
| mission 数 | 2 | Mesh 与 NHR 必须真正并发推进，不能退化为单 mission 串行 |
| NHR jetty 数 | 1 | 与 HCCL 该算法的 canonical template 配置一致；已有 Concurrent 算法仍保持 4 |
| phase barrier | 请求记录槽 30/31 中的 GM flag | 两个独立 CCU mission 之间需要可见的汇合点 |
| XN 槽数 | 32 | KFC 每 mission 当前最多加载 32 个 XN，编译期 `static_assert` 防止协议越界 |
| 大消息 fallback | CCU 分支 `NOT_MATCH`，外层回退 AICPU | asc-devkit 尚缺 Pipeline/SoleNHR 对应迁移，不能用错误算法冒充 |

## 6. 校验范围与结果

### 已完成的无卡校验

| 校验 | 结果 | 覆盖内容 |
|---|---|---|
| `git diff --check` | 通过 | 空白符和 patch 格式 |
| 独立 C++14 协议测试 | 通过 | 两 mission role、32 槽布局及 flag 边界 |
| `mc2_client` Host 构建 | 通过 | selector、executor、template、KFC Server template、CCU template compiler 代码均完成编译和链接 |
| `cann_device` 构建与打包 | 通过 | aarch64 device 配置、MC2 Server、tiling API、设备包生成 |
| 二进制符号检查 | 通过 | `libmc2_client.so` 中存在目标算法名及两个新增 CCU kernel 符号 |
| 算法/selector 单测 | 73/73 通过 | 512 KiB/16 MiB 边界、CCU→AICPU fallback、NHR 步骤/stride/channel 既有回归 |
| CCU bridge 单测 | 51/51 通过 | algorithm map、enum、mission protocol、指定算法的 `CcuSelectAlg` 路径 |
| CCU DSL/模拟器回归 | 84/84 通过 | 既有 CCU 指令构造、资源和算法场景无回归；当前 fixture 只支持单 mission，未冒充目标双 mission 数值测试 |
| CCU 编译检查 | 通过 | `asccomm_ccu_compile_check` 编译了新增 Mesh/NHR kernel 和 KFC Server dispatch |
| 相关 MeshChunk 回归 | 14/14 通过 | 构建算法测试目标时由工程自动执行的相关回归 |

安装包中的 `tikicpulib-config.cmake` 把库路径写成了 `tools/lib`，实际库位于 `tools/tikicpulib/lib`。校验时只在 `/tmp` 建了目录映射，让 CMake 使用现有库，没有修改 CANN 安装目录。构建脚本遇到 Python 3.13 移除 `distutils` 时，同样只在 `/tmp` 使用了最小兼容 shim。本次未安装或修改任何系统 Python/系统包。

### 无卡环境不能完成的项目

- 多 rank 数值正确性；
- phase flag 的真实 CCU/GM 可见性和死锁验证；
- UBX channel 的实际建链；
- 512 KiB–16 MiB 区间的性能收益；
- 与 HCCL 主仓实现的设备侧逐字节输出对比。

上卡时至少应覆盖：`M×N = 4×2、8×2、8×4`，FP16/BF16/FP32，奇数 `count`，in-place/out-of-place，连续及非零 `strideCount`，以及 `512 KiB`、`512 KiB+1 element`、`16 MiB-1 element`、`16 MiB` 四个选择边界。
