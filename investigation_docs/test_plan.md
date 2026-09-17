# IFA（incre_flash_attention + meta）Exception Dump 测试方案

> 配套：[README.md](README.md)（执行手册）｜[test_report.md](test_report.md)（实测结果）｜[incre_flash_attention_exception_dump_report.md](incre_flash_attention_exception_dump_report.md)（技术原理）

---

## 1. 测试目标

| # | 目标 | 验证手段 |
|---|---|---|
| G1 | Exception Dump 公开接口可用：注册、异常触发、全输入落盘、数据正确 | D1–D7 |
| G2 | 原生三段式链路可用：stub 导出 → funcHandle → binHandle → per-binary 回调 | D8–D10 |
| G3 | 正常路径零开销：无异常时零 dump 产物、回调零触发 | D4 对照 baseline |
| G4 | 产物一致性：dump 产物在两条链路间 bit-exact / md5 交叉一致 | D5 / D7 / D10 |
| G5 | Demo 可复现：自建最小 kernel 三段式链路全通 | E1 |

## 2. 被测对象与测试范围

| 对象 | 说明 |
|---|---|
| ascend_ops 包内 `incre_flash_attention` 主算子 | `npu_fused_infer_attention_score`，Ascend C `<<<>>>` 直调，8 个模板实例 |
| `incre_flash_attention_meta` 前置算子 | 生成 metadata（含 usedCoreNum），随主算子输入一并 dump |
| 回调 so（公开接口） | `src/exception_dump_public.cpp`，双链路：全局回调引导 + 原生三段式 |
| 算子侧补丁 | `patches/ops_transform_local.patch`（构建适配）、`patches/ifa_kernel_symbol_export.patch`（stub 导出） |

前置条件：算子功能正常（先跑官方冒烟 `tests/incre_flash_attention/test.py`，在 `<ops仓>/examples/fast_kernel_launch_example/` 下）。

不在范围：算子功能缺陷修复本身、AICPU 算子（非 AICore 异常场景）、多卡场景。

## 3. 测试环境

| 项 | 基线 |
|---|---|
| CANN | 9.1.T103 |
| 硬件 | Ascend 910C（910_9392，NPU 0 卡） |
| 容器/编译器 | docker `ops_dev_sw`，bisheng clang 15.0.5 + g++ |
| Python 栈 | Python 3.11.9 + torch 2.7.1 + torch_npu 2.7.1.post2 + ascend_ops 1.0.0 |
| 算子基线 | ops-transformer `ecfc11431` + 本包补丁 |

新版本看护按 README §4.1 准备环境并重编译全部 so。

## 4. 故障注入设计

三个基础场景（脚本内 `scene1` / `scene2` / `baseline`）：

| 场景 | 注入方式 | 异常机制 | 验证点 |
|---|---|---|---|
| scene1 | host 侧把 `block_table[0,0]` 写为 `0x7FFFFFFF`（非法 PA） | kernel 按 block table 取 KV cache，PA 越界 → 硬件 trap → AICore error | 全输入 dump + 数据 bit-exact（含损坏值） |
| scene2 | host 侧把 `actual_seq_kvlen[0]` 写为 `0x7FFFFFFF` | 读越界 → AICore error | 同进程第二次异常由 per-binary 回调接管 |
| baseline | 无注入 | 不异常 | 零 dump 产物、回调零触发（对照组） |

判定原则：**以回调触发（kernel 名匹配）与 dump 产物为准**；异常码数值（0x7bc87 / 507057）为实测参考值，不做强校验。

## 5. 用例设计

### 5.1 D 系列——公开接口 Exception Dump（主路径）

前置：README §4.3 第 1 步重编译回调 so；命令在 `test/` 目录内执行。

| 编号 | 用例 | 操作 | 通过标准 |
|---|---|---|---|
| D1 | 引导链路注册 | `test_public_dump.py` Step1 日志 | `RegisterPublicExceptionDump ret=0` |
| D2 | scene1 全输入 dump | `test_public_dump.py scene1` | 回调触发（retcode=0x7bc87）；三段式挂载成功；`Saved 8/8 inputs`；exit code 2 属预期 |
| D3 | scene2 per-binary 接管 | `test_public_dump.py all`（单进程 scene1→scene2） | 同进程第二次异常由 binary-cb 接管并 dump 8/8；末段 baseline 报 507057 属预期 |
| D4 | baseline 零误触发 | 独立进程 `test_public_dump.py baseline` | 正常结束；无新增 `dev*_ts*` 目录；回调零触发 |
| D5 | dump 数据 bit-exact | `verify_public_dump.py` | `7/7 tensor bit-exact 一致`；`usedCoreNum=18` |
| D6 | tiling 解析 | `parse_ifa_tiling.py` | args 指针区与布局表一致；BaseParams `12/12 字段一致` |
| D7 | md5 交叉验证 | `md5sum task_fail_dump/dev*/query.bin` | 所有目录一致，基线值 `6789400e0fadc7fa271b0b8ed8f29e0d` |
| D8 | 原生三段式注册 | `test_ifa_native3stage.py` Step1 | 8/8 模板实例 `RegisterBySymbol` HIT |
| D9 | 原生三段式 dump | `test_ifa_native3stage.py` scene1 | binary-cb dump 8/8；无全局回调参与 |
| D10 | 三段式数据验证 | `verify_public_dump.py` + md5 | 7/7 bit-exact；md5 与 D7 一致 |

### 5.2 E 系列——最小 Demo（外部样例）

| 编号 | 用例 | 操作 | 通过标准 |
|---|---|---|---|
| E1 | 最小 kernel 三段式 | `demo/demo_mini_symbol.py` | stub → [HIT] funcHandle（launch 前后均命中）；mini_launch 输出 `[41, 100]`；funcHandle → binHandle 三段式全通 |

## 6. 执行顺序与依赖

```
环境准备（README §4.1）
  └─ 算子构建安装（README §4.2，D8–D10 需加 symbol 补丁）
       └─ 官方功能冒烟（前置条件）
            ├─ 重编译回调 so → D1–D7（每场景独立进程，产物即时验证）
            ├─ D8–D10（依赖 symbol 补丁）
            └─ E1（无 IFA 依赖，可并行）
```

执行约束（详见 README §6）：注入场景 exit code 2 属预期；scene1/baseline 分进程执行（D3 用 `all` 模式同进程）；复跑前 `rm -rf task_fail_dump`；baseline 零产物按时间戳对照判定。

## 7. 通过标准（总判定）

| 级别 | 标准 |
|---|---|
| 阻断（必须全过） | D1–D7、D4 零误触发、D5 bit-exact |
| 主线（应全过） | D8–D10（原生三段式）、E1 |
| 版本比对 | dump 布局 8/8、tiling 12/12、md5 与基线一致；任一不一致 → 触发 README §7 复查清单 |

## 8. 结果记录要求

- 每轮看护记录：CANN 版本、执行日期、各用例结果、异常码实测值、md5 值；
- dump 产物保留最近一轮（`task_fail_dump/dev*` 目录），归档关键日志片段；
- 与基线不一致项必须关联 README §7 对应复查项，给出结论后闭环。
