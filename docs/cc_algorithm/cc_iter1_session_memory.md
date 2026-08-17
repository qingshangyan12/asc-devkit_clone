# 集合通信算法迭代 1 会话恢复记忆

## 1. 下个 Session 先读什么

按以下顺序恢复：

1. 本文档：当前机器、分支、约束、关键决策和验证状态。
2. `docs/cc_algorithm/cc_iter1_completion.md`：完整成果路径和执行命令。
3. `.agent/skills/migrate-hccl-kfc-algorithms/SKILL.md`：后续算法迁移标准流程。
4. `/mnt/c/data/design_docs/cc_algorithm/cc_iter1_plan.md`：原始需求和算法列表。

## 2. 当前状态快照

| 项目 | 当前值 |
|---|---|
| 日期 | 2026-08-06 |
| asc-devkit | `/mnt/c/data/gitcodes/asc-devkit` |
| 分支 | `cc0806_2` |
| HEAD | `2f582c106dc7b15e965bde4bfff1892051ee00b1` |
| HCCL | `/mnt/c/data/gitcodes/hccl` |
| HCCL HEAD | `530d77c111ede79f390e1e14cc27a50c93ae2e01` |
| CANN | `/home/houyu/Ascend/cann-9.2.0` |
| xiaoting ref | `pr_4721` |
| xiaoting commit | `0dbb7ac870a2f6fb74260e347c5c348a4d236ca9` |
| 计划 | `/mnt/c/data/design_docs/cc_algorithm/cc_iter1_plan.md` |

asc-devkit 工作区包含本次迁移的 tracked 修改和大量 untracked 新文件，尚未提交。`.git` 在当前执行环境只读，不要尝试写 index，也不要清理、回退或覆盖现有修改。

## 3. 用户最终确认的范围

需要完成并保留：

- 单 die AllGather：AICPU + CCU/KFC。
- 单 die ReduceScatter：AICPU + CCU/KFC。
- 单 die AllReduce：AICPU OneShot/TwoShot + CCU/KFC。
- 单 die AllToAllV：AICPU + CCU/KFC。
- 单 die固定 AllToAll：CCU/KFC。
- AICPU ST 必须 e2e。
- CCU 只要求 host ST，因为没有 CCU 仿真器。

本次明确不做：

- HalfAllToAllV。
- 双 die。
- DPU ST。
- CCU device 仿真结论。

旧迁移目录中保留了一些双 die、DPU 和旧 selector 文件。它们没有列入本轮 CMake/注册/有效 selector，不要误删用户已有文件，也不要重新接入。

## 4. 已完成实现

### 4.1 算法

- AllReduce AICPU：`InsAllReduceMesh1DOneShot`、`InsAllReduceMesh1DTwoShot`。
- AllReduce CCU：`CcuAllReduceMesh1DMem2Mem`，KFC dispatch 内执行 ReduceScatter + AllGather 组合。
- AllToAllV AICPU：`InsAlltoAllVMesh1D`。
- 固定 AllToAll CCU：`CcuAlltoAllMesh1D`。
- AllToAllV CCU：`CcuAlltoAllVMesh1D`。
- AllGather、ReduceScatter 的 CCU selector 已收紧到单层、单 die Mesh1D。

主要新增目录：

```text
impl/adv_api/detail/hccl/cc/src/ops/all_reduce/
impl/adv_api/detail/hccl/cc/src/ops/all_to_all_v/
tests/api/adv_api/hccl/cc/st/algorithm/testcase/mc2_ccu/
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/ccu/
tests/api/adv_api/hccl/cc/st/algorithm/utils/src/mc2_ccu/
```

### 4.2 KFC

xiaoting MR 4721 的三项设计结论为“方向正确，吸收并修正”：

1. 删除 `kfc_server_op.cc/.h`：正确。KFC Server 是资源申请时替换末端 kernel 的常驻服务，不是独立 HCCL op。
2. 新增 `kfc_server_config.h`：正确。用于多 opId dispatch 和 Server cache hash。
3. 重写 `DispatchKfcSubKernel`：正确。host 必须生成包含多个 opId 条件分支的 CCU IR。

在她的方案上做了这些修正：

- `inline thread_local` 不兼容仓库 C++14，改为函数内 `static thread_local`。
- AllReduce、ReduceScatter 的 opId 都包含 `reduceType`。
- dispatch 补充 AllReduce、固定 AllToAll、AllToAllV。
- cache hash 覆盖 algTag 和完整 opId 列表。
- KFC template 校验并继承原始算法 kernel 参数，再替换末端 kernel。

关键文件：

```text
impl/adv_api/detail/hccl/cc/src/common/kfc_server_config.h
impl/adv_api/detail/hccl/cc/src/common/hccl_alloc_ctx_res.cc
impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/ccu_temp_kfc_server.cc
impl/adv_api/detail/hccl/cc/src/ops/kfc_server/template/ccu/kernel/ccu_kernel_kfc_server.cc
```

### 4.3 本地调用链修复

主线 `cann_host_bridge` 原来把 AllReduce、AllToAll 和 AllToAllV 转发给安装版 `libhccl.so`，导致迁移后的本地 selector/executor 实际不执行。本轮已关闭这些 bridge：

```text
impl/adv_api/detail/hccl/cc/src/ops/op_common/cann_host_bridge.cc
```

这是后续迁移最容易遗漏的问题。若 ST 看不到本地 selector 日志，优先检查 bridge。

## 5. HCCL 新提交对比结论

HCCL HEAD 为 `530d77c1`。相对已迁移版本的新增高性能内容主要属于：

- UBX AIV 优化。
- 多层/三级拓扑限制和选择策略。
- 本轮范围外的双 die 或其他引擎路径。

本轮是单层、单 die、非 AIV，因此没有机械迁移上述代码；只吸收与当前 KFC Mesh1D 直接相关的接口和 selector 收紧。后续扩大范围时必须重新按 revision 对比，不能把本文结论当作永久跳过依据。

## 6. 已验证结果

### 6.1 AICPU e2e

二进制：

```text
/mnt/c/data/gitcodes/asc-devkit/build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC
```

最终结果：

```text
[==========] 84 tests from 4 test suites ran. (165119 ms total)
[  PASSED  ] 84 tests.
```

### 6.2 CCU host

二进制：

```text
/mnt/c/data/gitcodes/asc-devkit/build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/mc2_ccu/ascendc_ut_adv_api_hccl_ccu_mc2_host
```

最终结果：

```text
[==========] 12 tests from 1 test suite ran. (58 ms total)
[  PASSED  ] 12 tests.
```

### 6.3 构建和包

```text
[100%] Built target mc2_client
device 外部工程：成功
CPack External generator：成功
```

最终包：

```text
/mnt/c/data/gitcodes/asc-devkit/build_out/cann-asc-devkit_9.1.0_linux-x86_64.run
```

SHA256：

```text
3c23e014aea7ebc8a666a8989ba20f7c8e103195e8314b73f9de95c287fc1f78
```

CANN 内可复用 makeself：

```text
/home/houyu/Ascend/cann-9.2.0/tools/op_project_templates/ascendc/customize/cmake/util/makeself
/home/houyu/Ascend/cann-9.2.0/x86_64-linux/tikcpp/ascendc_kernel_cmake/fwk_modules/util/makeself
```

`pkg-config 2.5.1` 已安装，不需要下载 makeself。

## 7. 快速恢复命令

```bash
cd /mnt/c/data/gitcodes/asc-devkit
git branch --show-current
git status --short
git diff --check

git -C /mnt/c/data/gitcodes/hccl rev-parse HEAD
source /home/houyu/Ascend/cann/set_env.sh
pkg-config --version
```

复跑测试：

```bash
/mnt/c/data/gitcodes/asc-devkit/build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/\
ascendc_ut_adv_api_hccl_cc_algorithm_ascend950pr_9599_AIC

/mnt/c/data/gitcodes/asc-devkit/build/tests/api/adv_api/hccl/cc/ut/algorithm/testcase/mc2_ccu/\
ascendc_ut_adv_api_hccl_ccu_mc2_host
```

增量构建：

```bash
cmake --build /mnt/c/data/gitcodes/asc-devkit/build --target mc2_client -- -j4
```

## 8. 不要重复或误操作

- 不要重新 fetch MR 4721；本地已有 `pr_4721`。
- 不要下载 makeself；CANN 和 `build/makeself` 中已有完整版本。
- 不要恢复 `kfc_server_op.cc/.h`。
- 不要重新打开 AllReduce/AllToAll(V) 的 `cann_host_bridge`。
- 不要把旧双 die selector/executor 加回 CMake。
- 不要声称 CCU device 已验证。
- 不要因为 AICPU trace 输出巨大而误判测试卡死；大数据 ReduceScatter 用例单例可运行约 40 至 55 秒。
- 不要回退当前 dirty worktree 中任何用户或迁移修改。

## 9. 下个 Session 的第一项检查

若继续迁移其他算法，先使用 `.agent/skills/migrate-hccl-kfc-algorithms/SKILL.md`，重新确认新算法的 `OPS`、`SOURCE_REV`、`TOPO_SCOPE` 和 `EXCLUSIONS`。若只是提交或整理当前成果，先完整审阅 `git status` 中的新增目录，再按仓库流程提交；当前会话没有创建 commit。
