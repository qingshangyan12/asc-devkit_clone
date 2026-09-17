# Cube 指针化扩展交接说明

当前分支：`task/cube-pointer-master`。本文件供后续 A2 agent 接手编译、复核和提交使用。

## 验收状态

**当前不能宣称完整验收通过。** 已完成与未完成的边界如下，复现命令见
`tests/api/basic_api/cube_pointer/ACCEPTANCE.md`。

本轮（2026-09-17）在本机复验通过的项，全部在真实 Ascend950PR_9579 上执行：

- Tensor / 纯指针 / Tensor+指针混合三种调用方式 18/18 数值一致，
  `max_abs_error == 0`（fp16、bf16 × 方阵边长 16/32/64）。
- 5/5 非法操作数在编译期被地址空间、只写或类型约束拒绝。
- `matrix_compile.asc` 编译探针通过，覆盖 LoadData、Mmad、MmadMx、Fixpipe、
  Dump、SetAddrWithOffset、SyncAll、IBSet/IBWait、Determine、PopStackBuffer。
- 双核同步在 Tensor、纯指针和两种混合共 4 种模式下 `SyncAll` 两轮与
  `IBSet/IBWait` 握手全部通过。

尚未完成、不得计入通过的项：

- **CANN 9.0–9.1 未验证。** 任务书指定 9.0–9.1，本机只能用
  9.2.0-beta.1 复验，9.0.0-beta.2 无法编译当前 `master`。
- **dav-2201 指针重载未做编译验证。** `MmadWithSparse` / `LoadDataWithSparse`
  指针重载沿用原始 Tensor 版本的 `#if __NPU_ARCH__ == 2201` 约束，本容器用
  `--npu-arch=dav-2201` 编译 `master` 会在既有头文件
  `impl/basic_api/utils/kernel_utils_struct_norm_sort.h` 的 `__callee__` 属性处
  失败，属该分支在 A2 上的既有编译限制，与本扩展无关，但因此无法在此处证明
  这 2 个重载可用。
- **2.4 清单逐行覆盖未完成**，60 行中仍有部分行没有独立用例。
- **附件回归结论依赖对附件源码的修正**；`mmad_unitflag` 对附件原始 golden 的
  失败仍未消除，不得以修正后的 golden 计入通过。

严格判定引用任务书的 opbase experimental standard：浮点要求
`matched_ratio >= 0.99` 且 `max_abs_error` 不超过对应 dtype hard limit；整数按逐
元素 exact 比较。FP16、BF16、FP32、FP8 与 int8/int32 已按真实 golden dtype
分别处理；`strict_metrics.py` 依据 golden 元数据判断 dtype 与 shape，长度不匹配
即判失败，不再按文件长度猜测类型。

`mmad_unitflag` 的原始 golden 与 Tensor/指针输出不一致（FP32 最大误差
0.01171875，3 个元素超过 0.01），而 Tensor 与指针路径输出逐位相同。按 K=512、
kRound=8 从输入重算的参考解给出最大误差 0.0068359375（corrected golden SHA256
`9694032d2421e3ad845302e9eae6361434015f921214fb1e58e743dba01920ee`）。
这只说明原始 golden 的 K 分块与硬件归约方式不一致，**不能**据此把该用例记为
通过；原始失败记录必须保留。

## 测试入口

先加载 CANN：

```bash
source /usr/local/Ascend/cann-9.2.0-beta.1/cann/set_env.sh
```

附件回归需要设置附件目录和输出目录：

```bash
export CUBE_ARCHIVE_ROOT=/path/to/extracted/test-cases
export CUBE_TEST_OUTPUT=/path/to/output
bash tests/api/basic_api/cube_pointer/run_archive.sh fixpipe_l0c2gm 2
```

真实 Tensor/pointer/mixed 基线：

```bash
bash tests/api/basic_api/cube_pointer/run.sh /path/to/output
bash tests/api/basic_api/cube_pointer/negative.sh /path/to/output-negative
bash tests/api/basic_api/cube_pointer/check_binary.sh /path/to/output
```

构建会调用仓库官方 `gen_kernel_tiling_data_def.py` 生成 tiling 头，并检查依赖
文件确实引用工作树的 `include/kernel_operator.h`。所有运行都有 timeout；同步测试
额外覆盖两核 Tensor、纯指针和混合 `SyncAll`、`IBSet/IBWait`。

## A2 agent 接手步骤

1. 当前脚本硬编码 DAV_3510，A2 不得直接执行。先选择 DAV_C220 匹配的工具链，
   修改测试 target 并核对原 Tensor baseline，确认编译器和生成 tiling 头可用。
2. 从 `tests/api/basic_api/cube_pointer/run_archive.sh` 选择代表性 Fixpipe、Mmad、
   LoadData 场景，使用同一份附件数据分别跑 Tensor 与 pointer；不要重新
   随机生成第二份输入。MX 和 3510 专用接口在 A2 不适用，不运行也不计 PASS。
   保留原 verifier 结果，并使用正确 dtype 的 strict_metrics.py 独立核验权威标准。
3. 逐用例记录 JSON、SHA256、metadata、ACL event 统计及原始日志，并核对是否为
   同一份输入分别跑 Tensor 与 pointer。性能只记录 median/p95；任务书第 3.3 节
   明确没有额外性能门槛。
4. 修改实现后至少重跑受影响 API 的 Tensor 回归、pointer/mixed 回归和负向地址空间
   编译用例；出现精度差异时按 experimental standard 报告 matched/max error。
5. 完成对应架构验证和 code review 后，按仓库协作规范推进 PR；当前交接不代表完整验收。

## 已知附件问题

原始附件保留不变，构建副本只修正已记录问题：嵌套注释终止符、2DMX 未声明类型
`T`、Load3DV2 输入指针误用累加类型、Mmad/MmadMx bias 错用 `__cc__` 而应使用
`__biasbuf__`。MX/2DMX 生成器必须安装 `en_dtypes` 和 `ml_dtypes`；缺失时脚本
会静默回退 uint8，结果不能作为有效 FP4/FP8 验收。

保留 `mmad_unitflag` 对原始 golden 的失败与修正依据，不得以修正后结果覆盖原始记录。
不要把 16 个架构不适用
场景算入 49 个通过场景。本仓内的复现说明见
`tests/api/basic_api/cube_pointer/ACCEPTANCE.md` 与
`tests/api/basic_api/cube_pointer/README.md`。

## 推送注意事项

本机 `git config --global credential.helper=store`，而 `/root/.git-credentials`
里存的是另一个 GitCode 账号（`lunhao2023`）的凭据。git 会优先使用该凭据，导致
以错误身份推送并收到：

```text
<CH.00905403> You are not allowed to push code to this project.
```

推送时须绕过已存凭据，改用 `凭证.md` 中 `Ethan-ki` 的令牌：

```bash
export GIT_ASKPASS=<返回 Ethan-ki 与该令牌的脚本>
export GIT_TERMINAL_PROMPT=0
git -c credential.helper= push origin task/cube-pointer-master
```

推送后用 `git -c credential.helper= ls-remote origin refs/heads/task/cube-pointer-master`
核对远端 SHA 与本地 `HEAD` 一致，不要只看 `git push` 的退出码。
