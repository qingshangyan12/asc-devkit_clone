---
name: okf-yaml-generate
description: 为指定 Ascend DevKit API 文档生成、校验并写入 OKF YAML、Doxygen 关联和样例 marker；处理单文件或目录及其问题报告。
---

# OKF YAML 生成

生成流程使用本目录的 `scripts/controller.py`，不调用旧 OKF skill 或索引工具。
页面分类和允许字段以 `config/source-context-v1.yaml`、`config/document-profile-registry.yaml` 为准；
标签词表和必填证据规则以 `../okf-tag-generate/taxonomy/tag-taxonomy.json`、`config/tag-rules.yaml` 为准。
这些规则由脚本校验，无需按目录手写 YAML 字段或复述解析器逻辑。
`version` 由语义输入提供；可先用 `scripts/lifecycle_stage.py` 扫描解包后的 CANN 头文件和 Release Notes，
输出按函数原型或宏归并的生命周期证据。

## 范围与写入

仅处理 `docs/zh/api/SIMD-API/{c_api,basic_api,adv_api,tensor_api}` 下指定的文件或目录。
每次使用本 skill 的 `tmp/<run-id>` 作为唯一运行目录。头文件和样例是取证输入；
默认写入仅限选中文档的 front matter、有证据的 include Doxygen 注释和样例调用 marker。
脚本可移除旧的 `api_signature` YAML 围栏；不改其他 Markdown 正文、声明、实现、样例代码或索引正文。

用户要求生成或更新时，校验通过后执行 `write`；明确要求 dry-run 或禁止写入时停在 `validate`。
任何校验失败都不得写入。

## 执行顺序

1. `prepare` 扫描目录内全部 Markdown，按正文分类并建立任务包，不按文件名跳过索引页。
   每批最多 25 篇；读取 `classification-report.json` 确认逐页类型。无法分类的页面在报告中
   列明路径和原因，`prepare` 停止且不创建任务包；Profile 能力检查失败时也停止。
2. 对每批非 `reference.index` 文档准备哈希绑定的 `semantic-input.json`：
   `description` 来自已通过 `okf-description-generate validate` 的结果，
   `tag` 每个值有逐字证据，`dependencies` 有结构化结果。
   按 [语义输入契约](references/semantic-contract.md)填写；索引页由链接直接生成，无需语义输入。
3. 运行 `worker`。单文件只需一个 worker；目录批次最多并发 6 个，不按字段拆 agent。
   仅在需要修改 worker 结果或处理 Doxygen、marker 时读 [工件契约](references/artifact-contract.md)。
4. 运行 `resolve-issues` 生成 `resolution-input.json`。有问题时只把该文件交给一轮
   问题处理 agent；仅在证据冲突时回读相关原文。决议按
   [问题处理规则](references/issue-resolution.md)提交；没有问题则跳过问题处理 agent。
5. 运行 `validate`，成功后按请求运行 `write`。读取 `error-report.json`；
   如有问题，最终反馈给出问题数和 `error-report.md` 的绝对路径。

在本 skill 目录执行（单文件用 `--document`，目录用 `--source-dir`）：

```bash
python3 scripts/controller.py prepare --devkit-root /path/to/asc-devkit --document docs/zh/api/SIMD-API/c_api/foo.md --run-dir tmp/run-id
python3 scripts/controller.py worker --task tmp/run-id/batches/batch-01/task.json --semantic-input tmp/run-id/batches/batch-01/semantic-input.json --output tmp/run-id/batches/batch-01/worker-result.json
python3 scripts/controller.py resolve-issues --run-dir tmp/run-id
python3 scripts/controller.py resolve-issues --run-dir tmp/run-id --decisions tmp/run-id/resolution-decisions.json
python3 scripts/controller.py validate --run-dir tmp/run-id
python3 scripts/controller.py write --run-dir tmp/run-id

python3 scripts/lifecycle_stage.py \
  --package 9.0.0=/path/to/cann-9.0.0 \
  --package 9.2.0=/path/to/cann-9.2.0 \
  --release-notes docs/zh/api/appendix/Release_Notes/CANN_9_2_0.md \
  --output tmp/run-id/lifecycle.json
```

索引页的 worker 可省略 `--semantic-input`；没有问题时省略带 `--decisions` 的第二次
`resolve-issues`。worker 可先无语义输入生成供取证用的 scaffold，但该结果不能通过最终校验，
必须用已验证语义输入重新运行。
