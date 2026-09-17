# IFA（incre_flash_attention + meta）Exception Dump 测试结果报告

> 配套：[test_plan.md](test_plan.md)（用例定义与通过标准）｜[README.md](README.md)（执行手册）
> 实测基线：CANN 9.1.T103（2026-09-07，Ascend 910C）

---

## 1. 结果总览

| 系列 | 用例 | 结果 | 备注 |
|---|---|---|---|
| D 公开接口 dump | D1–D7 | **PASS** | 引导链路（零算子改动） |
| D 公开接口 dump | D8–D10 | **PASS** | 原生三段式（symbol 补丁 + bisheng 重编 wheel） |
| E Demo | E1 | **PASS** | 自建最小 kernel 三段式全通 |

**核心结论**：公开接口两条链路（引导 / 原生三段式）全部打通，dump 产物在两链路间 **bit-exact 一致、md5 交叉一致**；正常路径零误触发。

---

## 2. D1–D4——引导链路（`test_public_dump.py`，零算子改动）

D1 注册（Step1 日志）：

```
[INFO] RegisterPublicExceptionDump('incre_flash_attention') ret=0   ← 全局回调挂载成功
```

D2 scene1（block_table 注入）关键日志：

```
[ExcDumpPub] (global-cb) Task fail detected: deviceId=0, taskId=3, streamId=47, retcode=0x7bc87
[ExcDumpPub] (global-cb) reported kernel: _Z21incre_flash_attentionILh0ELh1ELh0EE...   ← 公开接口直接给出全名，子串匹配命中
[ExcDumpPub] (global-cb) 3-stage chain armed: funcHandle=0xaaabb3562ba0 -> binHandle=0xaaabb3508920 -> BinarySetExceptionCallback OK
[ExcDumpPub] (global-cb) devArgs: ptr=0x12c100000080, len=1264, numInputs=8
[ExcDumpPub] (global-cb) Done. Saved 8/8 inputs to ./task_fail_dump/dev0_task3_ts<ts>
```

说明：公开访问器族（DeviceId/TaskId/StreamId/ErrorCode 等）不含异常类型字段，`expandType` 为旧内部接口字段，本链路日志不再输出。

- 三段式挂载在**首次异常回调内**完成：`GetFuncHandleFromExceptionInfo → FunctionGetBinary → BinarySetExceptionCallback`；
- devArgs `ptr=0x12c100000080, len=1264` 与 launch 侧下发的 args 区一致，8 输入 dev_addr 全部一致；
- 脚本以 exit code 2 结束（异常被捕获属预期路径）。

D3 scene2（同进程第二次异常）：由 **binary-cb（per-binary 回调）接管**并完成 dump 8/8——三段式挂载后续链路验证通过。

D4 baseline：无注入正常跑完，**零 dump 产物、回调零误触发**。

## 3. D5/D6/D7——数据验证

| 用例 | 结果 | 证据 |
|---|---|---|
| D5 bit-exact | **7/7 tensor 一致** | `verify_public_dump.py`：7 个数值输入逐位相等（含注入的 `block_table[0,0]=0x7FFFFFFF` 原样保留）；metadata `usedCoreNum=18` 与 batchSize 自洽 |
| D6 tiling | **12/12 字段一致** | `parse_ifa_tiling.py` 解析 `args_raw.bin`：`seqSize=8320`（=65×128 对齐）、`maxBlockNumPerBatch=65`、`scaleValue=0.0883883`（=1/√128）等全命中；args[0..31] 指针区与布局表逐槽一致 |
| D7 md5 交叉 | **一致** | `query.bin` md5 = `6789400e0fadc7fa271b0b8ed8f29e0d`，引导链路各 dump 目录及原生三段式产物完全一致 |

## 4. D8/D9/D10——原生三段式（`test_ifa_native3stage.py`，算子打 `ifa_kernel_symbol_export.patch` + bisheng 重编 wheel，**不注册全局回调**）

| 用例 | 结果 | 证据 |
|---|---|---|
| D8 注册 | **8/8 全部 HIT** | 8 个模板实例（`<FD,LAYOUT,ANTIQ>` 组合）`ifa_get_kernel_symbol` → `RegisterBySymbol`：GetFuncBySymbol → FunctionGetBinary → BinarySetExceptionCallback 全通，**launch 前完成挂载** |
| D9 dump | **8/8** | scene1 注入后 binary-cb 直接触发并 dump 8/8，全程无全局回调参与 |
| D10 验证 | **7/7 + md5 一致** | bit-exact 7/7；`query.bin` md5 与 D7 引导链路产物一致 |

## 5. E1——最小 Demo（`demo/demo_mini_symbol.py`，自建 kernel，与 IFA 无关）

| 验证项 | 结果 |
|---|---|
| stub 指针（so 加载后、**launch 前**） | **HIT** → funcHandle（`cceModuleCtor` 在 so 加载时完成 eager 注册） |
| stub 指针（launch 执行后） | HIT（同一 funcHandle） |
| kernel 名字符串 / launch 内部函数地址 | MISS（确认入参必须是 kernel stub 函数指针） |
| TU 内 `&kernel` 与 dlsym stub 地址 | 完全一致 |
| funcHandle → `aclrtFunctionGetBinary` → binHandle | 全通 |
| `mini_launch` 输出 | `[41, 100]`（数值正确） |

## 6. symbol 接口探索记录（结论：`aclrtGetFuncBySymbol` 支持 `<<<>>>` 直调 kernel）

接入过程中对 `aclrtGetFuncBySymbol` 入参形式的排查记录（详见主报告 §4.6）：

| 候选 symbol | 返回码 | 判定 |
|---|---|---|
| kernel 名字符串 | 0x1a224 | 入参形式错误（要求函数指针） |
| meta 算子 stub 指针（`__aicpu__` kernel） | 0x1a224 | 调用形式正确，但 AICPU kernel 不在 funcHandle 注册表 |
| nullptr 对照 | 0x186a0 | 参数校验失败，证明 0x1a224 = "注册表中无此地址" |
| IFA 主算子 stub | （外部取不到） | 模板 + 同 TU launch，stub 被内联为本地符号，dlsym 不可得 → 需算子 TU 内导出（补丁） |
| mini kernel / 补丁导出的 IFA stub | **HIT** | 接口本身支持，入参必须是 kernel stub 函数指针 |

## 7. 场景矩阵汇总

| 场景 | 注入 | 引导链路 | 原生三段式 |
|---|---|---|---|
| scene1 | block_table 块 id 越界 | 触发，8/8 dump，7/7 bit-exact | 触发，8/8 dump，7/7 bit-exact |
| scene2 | actual_seq_kvlen 越界 | 触发，binary-cb 接管 dump | —（同机制抽验一致） |
| baseline | 无 | 正常结束，零产物 | —（D9 场景内对照） |

md5 交叉验证：`query.bin` = `6789400e0fadc7fa271b0b8ed8f29e0d`，两链路产物完全一致。

## 8. 已知边界（非缺陷）

1. **多场景需分进程（D3 除外）**：同进程 AICore error 后设备流状态未恢复，紧接的后续 launch 报 507057（`all` 模式 baseline 段异常属此现象，非误触发）；D3（per-binary 接管）例外，必须以 `all` 模式同进程完成；
2. **双回调各 dump 一次**：引导链路挂载完成后，同一异常 global-cb 与 binary-cb 可能各落盘一次（内容一致，时间戳目录区分）；
3. **全局回调为进程级**：与宿主框架（torch_npu）注册的全局回调会互相覆盖，集成时优先 per-binary 链路。

## 9. 结论

1. **G1–G5 全部达成**（映射见 test_plan.md §1）：公开接口 dump、原生三段式、零误触发、产物一致性、Demo 复现均有实测证据；
2. 回调库仅依赖公开头文件与 `-lacl_rt -lascendcl`，满足随 CANN 版本升级的稳定性要求；
3. 达到纳入日常看护与对外发布的条件：阻断级用例（D1–D7）+ 主线用例（D8–D10、E1）可作为每轮新 CANN 版本的固定回归集。
