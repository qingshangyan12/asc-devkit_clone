# Cube 指针化扩展交接说明

当前工作树：`task/cube-pointer-9.0` 对应提交 `753ffada6a984f588d4d70fac5eef28e6e1f18e8`。
本文件供后续 A2 agent 接手编译、复核和提交使用。

## 验收状态

在 Ascend 950PR_9579 / DAV_3510、CANN 9.2.0-beta.1 上，任务压缩包 `test-cases`
中 49 个适用场景已经完成真实 NPU 精度回归：39 个首批场景与 10 个 MX/2DMX
复测场景全部通过；16 个场景依据原始 CMake 的 dav-2201 限制或 950 int4
LoadData2DV2 能力标为 `UNSUPPORTED`，不得计入通过率。

严格判定引用任务书的 opbase experimental standard：浮点要求
`matched_ratio >= 0.99` 且 `max_abs_error` 不超过对应 dtype hard limit；整数按逐
元素 exact 比较。FP16、BF16、FP32、FP8 与 int8/int32 已按真实 golden dtype
分别处理。`mmad_unitflag` 的历史失败来自错误的 K 分块 golden，不是实现精度回退。
按 K=512、kRound=8 从输入重算后，Tensor 和 pointer 均 matched=1.0，最大误差
0.0068359375；corrected golden SHA256 为
`9694032d2421e3ad845302e9eae6361434015f921214fb1e58e743dba01920ee`。

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

1. 在本分支先执行上述基线构建，确认 CMake 编译器和生成 tiling 头可用。
2. 从 `tests/api/basic_api/cube_pointer/run_archive.sh` 选择代表性 Fixpipe、Mmad、
   LoadData、MmadMx 场景，使用同一份附件数据分别跑 Tensor 与 pointer；不要重新
   随机生成第二份输入，也不要替换原 verifier。
3. 检查 `tmp/final-acceptance` 和 `strict-acceptance` 中的 JSON、SHA256、metadata、
   ACL event 统计及原始日志。性能只记录 median/p95；任务书没有额外性能门槛。
4. 修改实现后至少重跑受影响 API 的 Tensor 回归、pointer/mixed 回归和负向地址空间
   编译用例；出现精度差异时按 experimental standard 报告 matched/max error。
5. 完成 code review 后，再按仓库协作规范创建 commit/PR。测试角色此前未 commit 或
   push 文档；本文件仅作为交接草稿。

## 已知附件问题

原始附件保留不变，构建副本只修正已记录问题：嵌套注释终止符、2DMX 未声明类型
`T`、Load3DV2 输入指针误用累加类型、Mmad/MmadMx bias 错用 `__cc__` 而应使用
`__biasbuf__`。MX/2DMX 生成器必须安装 `en_dtypes` 和 `ml_dtypes`；缺失时脚本
会静默回退 uint8，结果不能作为有效 FP4/FP8 验收。

不要把历史的 `mmad_unitflag` golden 误差当作实现失败，也不要把 16 个架构不适用
场景算入 49 个通过场景。详细证据见
`.cannbot/cube-basic-api/strict-acceptance/final-950-acceptance-report.md`。
