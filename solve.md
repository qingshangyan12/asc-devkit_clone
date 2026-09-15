# 修改方案与验证记录

## 修改方案

本次超时的直接证据是同一 kernel 的 AIC/AIV 分别停在等待指令。修复重点是让流水控制 flag 的生产/消费次数与 task slot 完全一致，尤其覆盖 `isValid == false` 的预热、尾排空和空分片场景。

建议在 `mlp_sparse_lightning_indexer_grad_kl_loss/op_kernel/sparse_lightning_indexer_grad_kl_loss_base.h` 做以下修改：

1. 在 `DeterProcess` 和 `MainProcess` 进入循环前计算统一的 `pipelineLoopCount`，AIC 与 AIV 使用同一 tiling 字段、同一上限；不要让 AIC 依据 `aicIdx` 的局部尾块条件提前结束。
2. 每个 slot 先执行一次跨核握手，再根据 `runInfo*.isValid` 执行计算。AIC 侧 `CrossCoreSetFlag<2, PIPE_MTE2>(14)` 与 AIV 侧 `CrossCoreWaitFlag<2, PIPE_MTE3>(14)` 必须处于同一个无条件循环中；>不得放入 `isValid` 分支。
3. 将末尾 `extraLoopTimes` 的 flush 改成同一 `pipelineLoopCount` 的最后两个 slot，并为无效 slot 写入显式的空任务状态。这样 `ProcessVector0/1/2`、`ComputeMm1/2/5/6` 可以跳过计算，但不会跳过对
应的同步协议。
4. 保留 `constInfo.aivIdx = GetBlockIdx(); constInfo.aicIdx = constInfo.aivIdx / 2;` 的配对关系，同时增加静态断言/调试日志确认 AIV 的 `aivIdx` 范围为 `2 * aicIdx` 和 `2 * aicIdx + 1`；禁止使
用未经 `GetTaskRation()` 校正的 block 数计算流水轮数。

建议的伪代码如下，分别替换两个 process 中当前以局部有效任务驱动的循环边界：

```cpp
const int64_t pipelineLoopCount = CalcPipelineLoopCount(tilingData, GetBlockNum());
for (int64_t taskId = 0; taskId < pipelineLoopCount; ++taskId) {
    LoadRunInfo(taskId, runInfo0);
    if ASCEND_IS_AIC {
        CrossCoreSetFlag<2, PIPE_MTE2>(14);
    } else {
        CrossCoreWaitFlag<2, PIPE_MTE3>(14);
    }
    if (runInfo0.isValid) { /* V0/C1/C2 work */ }
    if (runInfoNeg1.isValid) { /* V1/MM work */ }
    if (runInfoNeg2.isValid) { /* V2 work */ }
}
```

`CalcPipelineLoopCount` 必须使用 host 侧 tiling 已经确定的最大分片数，并至少加 2 个 preload/flush slot；不能从当前 core 的 `isValid` 数量推导。

## 反汇编到具体源码行

根据运行时 PC 做入口归一化后，故障点可以落到以下源码行（当前对象无 DWARF，因此这是按指令序列和唯一匹配的同步 API 反推的行号）：

| 核 | 运行时 PC | 对象内偏移 | 反汇编指令 | 对应源码 |
|---|---:|---:|---|---|
| AIC core 0 | `0x1588f36c12e8` | `0x333c` | `WAIT_FLAG.MTE1.MTE2 X31` | [`sparse_lightning_indexer_grad_kl_loss_service_cube.h:516`](/mnt/workspace/gitCode/cann/timeout/mlp_sparse_lightning_indexer_grad_kl_loss/op_kernel/sparse_lightning_indexer_grad_kl_loss_service_cube.h:516) |
| AIV core 1 | `0x1588f36ce5a8` | `0xbeec` | `WAIT_FLAG_DEV X0` | [`sparse_lightning_indexer_grad_kl_loss_vector.h:675`](/mnt/workspace/gitCode/cann/timeout/mlp_sparse_lightning_indexer_grad_kl_loss/op_kernel/sparse_lightning_indexer_grad_kl_loss_vector.h:675) |

AIC 点位于 `ComputeMm1()` 的 `for (kOuterIdx)`/`for (dIdx)` 内，在 `CopyInMm1BToL1()` 前等待 `SYNC_MTE21_FLAG[gatherPingPongFlag & 1]`；其配对生产位于同一函数第 524 行。AIV 点位于 `ProcessVector1()`，第 674 行先 `CrossCoreSetFlag<0x1, PIPE_MTE3>(0x8)`，第 675 行再等待 `CrossCoreWaitFlag<0x1, PIPE_MTE2>(0x8)`。因此现场停留的具体卡死语句是 AIC 第 516 行和 AIV 第 675 行，根因方向应优先检查 AIC 第 524 行与 AIV 第 674 行对应的生产/消费次数及 ping-pong 索引是否一致。


## 静态验证

- 日志中的 AIC/AIV 等待点已与反汇编复核：AIC 相对 PC `0x333c` 对应 `0x6c13e8 WAIT_FLAG.MTE1.MTE2`，AIV 相对 PC `0xbeec` 对应 `0x6ce5a8 WAIT_FLAG_DEV`。
- 源码检查确认 `DeterProcess`（约 436-503 行）和 `MainProcess`（约 528-604 行）当前均以 task/runInfo 条件驱动计算，并在每轮执行 flag14 握手；因此修改应集中在统一循环边界和空 slot flush，不能
只增加 host 侧 `Synchronize`。
- `InitConstInfo`（约 208-213 行）确认 AIV/AIC 的索引配对公式；验证要求 `aivIdx / 2 == aicIdx`，且 `subBlockIdx == aivIdx % 2`。
- 使用 CANN `llvm-objdump` 对原对象和调试副本验证，两个等待指令均存在；对象格式为 `elf64-hiipu/hiipu64`。

## 尚需的回归验证

当前工作区没有可用 NPU 现场和该版本完整 device 构建入口，因此尚未修改源码、编译新 kernel 或运行设备回归。实施上述修改后必须：

1. 使用 `-g` 编译并确认新 `.o` 含 `.debug_*` 段；
2. 在原失败 shape、首块、整块、尾块和空分片上运行至少三次；
3. 检查 AIC/AIV 每个 task slot 的 set/wait 次数相等，且不再出现 `0x7150025`；
4. 用新对象反汇编将 PC 映射到修改后的源码行，并执行 release 编译回归。
因此，本文件确认了修改位置、同步协议和可验证条件；“设备上已修复”需完成上述编译与 NPU 回归后才能确认。

## 2026-09-15 按方案修改与 CANN 8.5.0 验证

已修改 `npu-ai-operation-kernel/training/mlp_sparse_lightning_indexer_grad_kl_loss/op_kernel/sparse_lightning_indexer_grad_kl_loss_base.h`：

- 新增 `CalcPipelineLoopCount()`，以 tiling 的 `multiCoreParams.coreNum` 和 `bS1Index[]` 计算所有核共享的最大任务轮数，并统一加两个 pipeline drain slot。
- `DeterProcess` 改为按 `slot < pipelineLoopCount` 运行；空 slot 仍执行 flag14 的 AIC set/AIV wait，只跳过 V0/C1/V1/C2/V2 计算。
- `MainProcess` 改为按 AIC 的 `bS1Index[aicIdx] + slot` 访问全局 S1 任务，使用同一 `pipelineLoopCount` 和两个尾部 slot，不再按 batch 局部尾块提前结束。
- AIC/AIV 的配对索引继续由 `aivIdx / 2` 和 `aivIdx % 2` 决定；`coreNum` 优先使用 host tiling 值，缺省时才回退 `GetBlockNum()`。
- 两个 process 在进入流水前显式清零三个 `runInfos[].isValid`，保证前两个 preload slot 是确定的空任务。

使用 `/usr/local/Ascend/cann-8.5.0/set_env.sh` 完成了 Release kernel 构建：

```bash
bash ./build.sh --build-dir=training/mlp_sparse_lightning_indexer_grad_kl_loss \
  --arch=ascend910b -j4
```

构建结果为成功，生成了两个 mix kernel 对象（普通和 deterministic）、对应 JSON 元数据以及安装包；CANN `llvm-objdump` 可解析生成对象为 `elf64-hiipu`。首次编译发现 `ASCEND_IS_AIV` 是语句宏，不>能写进逻辑表达式，已改为 AscendC 要求的嵌套 `if ASCEND_IS_AIV` 形式后重新构建通过；加入 `runInfos` 初始化后再次完整构建仍成功。

现场有 16 张 910B（`npu-smi info` 均为 Health OK）。尝试将生成安装包通过临时 `ASCEND_OPP_PATH` 接入并运行 NPU 用例时，运行环境在 Cast 初始化阶段报 `SpaceRegistry is nullptr / OP tiling_funcs NULL`；该错误发生在目标算子执行前，说明临时 OPP 包注册方式不完整，不能作为 kernel 超时或修复失败的证据。当前已完成源码级同步协议验证和 CANN 8.5.0 编译验证，尚未完成原失败 shape 的设备回归；
设备回归应在目标机器按正式安装脚本安装该包后重复执行，并检查 task 48867/`0x7150025` 是否消失。


## 2026-09-15 编译回归尝试

已加载 `/home/developer/Ascend/cann-9.2.0/set_env.sh`，并使用 ops-transformer 构建入口执行：

```bash
bash build.sh --opkernel --ops=mlp_sparse_lightning_indexer_grad_kl_loss \
  --soc=ascend910_93 --build-type=Debug -j4 --incremental
```

CMake 正确识别了 CANN 包、`ascend910_93` 和 `Debug` 配置，但在配置算子时失败：算子工程的 `CMakeLists.txt` 依赖 `../../cmake/op_project.cmake`，当前 workspace 和 `/home/developer/Ascend/cann-9.2.0` 均不存在该文件；因此没有生成新的 device `.o`，也没有执行 NPU 回归。失败不是编译器或设备错误，而是算子工程模板依赖缺失。
