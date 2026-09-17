# incre_flash_attention（+meta）Exception Dump 能力构建与系统性调测报告

| 项目 | 内容 |
|---|---|
| 报告类型 | 异常现场自动保存（Exception Dump）能力构建 + 系统性功能调测 |
| 调测对象 | `incre_flash_attention`（主算子）及设备侧 Tiling 前置算子 `incre_flash_attention_meta` |
| 代码基线 | `ops-transformer/examples/fast_kernel_launch_example/csrc/incre_flash_attention(_meta)/ascend910_93/`，commit `ecfc11431` |
| 实测基线 | CANN 9.1.T103（2026-09-07，公开接口版） |
| 容器/编译器 | docker `ops_dev_sw`，bisheng clang 15.0.5（aarch64） |
| Python 栈 | Python 3.11.9 + torch 2.7.1 + torch_npu 2.7.1.post2 + ascend_ops 1.0.0 |
| 硬件 | Ascend 910C（910_9392，NPU 0 卡） |
| 配套 | [README.md](README.md)｜[test_plan.md](test_plan.md)｜[test_report.md](test_report.md) |

---

## 1. 摘要

针对 NPU 算子 AICore 异常后"现场丢失、难以离线复现"的定位痛点，本工作在 IFA 算子上完整构建并实测了 **Runtime 层外挂式异常输入 Dump 能力**：算子发生 AICore error 时，回调自动把该次执行的全部输入 tensor（含 meta 算子产出的 metadata 与内联 tiling 结构）从设备内存保存为宿主机 bin 文件，实现"案发现场"完整留档。

回调链路全部基于 **CANN 公开标准接口**（`aclrtSetExceptionInfoCallback` + `aclrtBinarySetExceptionCallback` 等，仅依赖 `-lacl_rt -lascendcl` + 公开头文件），提供两条链路：

| 链路 | 注册方式 | 适用场景 | 算子侧改动 |
|---|---|---|---|
| **引导链路**（默认） | `RegisterPublicExceptionDump(kernelName)` → 全局回调首次异常时自动挂载 per-binary 回调 | 零算子改动，最简接入 | 无 |
| **原生三段式**（推荐） | `RegisterBySymbol(stub)` → launch 前预挂载 | 需算子 TU 内导出 kernel stub | +30 行补丁 |

核心结论：

| 验证项 | 结果 | 证据 |
|---|---|---|
| 异常捕获 | PASS | `deviceId=0, taskId=3, streamId=47, retcode=0x7bc87` |
| 全输入 dump 完整性 | PASS | `saved_inputs=8/8` + `args_raw.bin`（1264 B） |
| dump 数据正确性 | PASS | 7/7 tensor **bit-exact 一致**（含故障注入值原样保留） |
| 原生三段式注册 | PASS | 8/8 模板实例 launch 前 HIT |
| 跨链路 md5 一致 | PASS | `query.bin` md5 = `6789400e0fadc7fa271b0b8ed8f29e0d`（两链路产物一致） |
| 正常路径零开销 | PASS | 无注入基线零产物、回调零触发 |

---

## 2. 问题定义与能力目标

### 2.1 痛点

Ascend 算子发生 AICore error 时，默认行为是任务失败上抛（本例 Python 侧表现为 `SUSPECT REMOTE ERROR, error code 507057`），随后进程退出。定位此类问题时面临两个困难：

1. **现场丢失**：异常后设备内存中的输入数据随进程退出而失效，无法事后回看"这次执行到底喂了什么数据"；
2. **复现依赖**：坏数据往往来自上游（如被污染的 block table、异常的序列长度），脱离原始输入无法稳定复现。

### 2.2 能力目标与验收标准

| 目标 | 验收标准（全部达成） |
|---|---|
| G1 自动：异常即 dump，无需人工干预 | AICore error 发生后 `task_fail_dump/` 自动出现完整产物 |
| G2 外挂：零侵入算子代码（引导链路） | 仅 ctypes 加载外部 so + 注册回调，算子仓无 dump 相关改动 |
| G3 完整：覆盖算子全部关键输入 | 8/8 输入落盘（含 metadata、args 区原文） |
| G4 可信：dump 数据即"案发现场" | 与构造侧逐位 bit-exact 比对，注入的损坏值原样保留 |
| G5 公开接口：随版本升级稳定 | 仅依赖 `-lacl_rt -lascendcl` + 公开头文件 |
| G6 可复用：形成方法论 | 五步范式 + 可重放的脚本/补丁/文档全套交付 |

---

## 3. 异常 Dump 机制原理

### 3.1 公开接口回调链路

#### 3.1.1 引导链路（默认，零算子改动）

```
RegisterPublicExceptionDump(kernelName)
  └─ aclrtSetExceptionInfoCallback(global_exception_cb)        ← 全局回调（进程级）
       └─ 首次异常:
            ├─ aclrtGetDeviceId/TaskId/StreamId/ErrorCodeFromExceptionInfo(info)  ← 异常定位
            ├─ aclrtGetFuncHandleFromExceptionInfo(info, &funcHandle)
            ├─ aclrtGetFunctionName(funcHandle, ...)           ← 上报 kernel 名（mangled 全名）
            ├─ aclrtFunctionGetBinary(funcHandle, &binHandle)   ← 三段式后半程
            ├─ aclrtBinarySetExceptionCallback(binHandle, binary_exception_cb, nullptr)
            │    （挂载成功后，同 binary 后续异常直走 per-binary 回调）
            ├─ aclrtGetArgsFromExceptionInfo(info, &devArgsPtr, &devArgsLen)
            └─ dump：args D2H → ptrIndex 解析输入指针 → 逐输入 D2H → 落盘
```

首次异常时全局回调完成三段式挂载；同 binary 后续异常由 per-binary 回调直接处理（不再经过全局回调的挂载逻辑）。

#### 3.1.2 原生三段式链路（推荐，需算子侧导出 stub）

```
RegisterBySymbol(stub)   ← 在 kernel launch 前调用
  ├─ aclrtGetFuncBySymbol(stub, &funcHandle)
  ├─ aclrtFunctionGetBinary(funcHandle, &binHandle)
  └─ aclrtBinarySetExceptionCallback(binHandle, binary_exception_cb, nullptr)

异常发生时:
  └─ binary_exception_cb(info, userData)
       ├─ aclrtGetArgsFromExceptionInfo(info, &devArgsPtr, &devArgsLen)
       └─ dump：args D2H → ptrIndex 解析 → 逐输入 D2H → 落盘
```

无需全局回调，无需等首次异常，launch 前即完成精确粒度挂载。

### 3.2 为什么"算子挂了，现场还在"（关键机制解释）

这是本能力成立的物理基础：

- kernel 的 **args 区**（入参指针 + 内联 tiling）在 launch 时由 host 写入 device 内存，**kernel 执行失败 ≠ args 区失效**；
- 各输入 tensor 的 device 内存同样未被释放（异常使 kernel 内部计算出错，不回收已分配内存）；
- 回调在 host 侧被 Runtime 调起，此时执行 `aclrtSetDevice(deviceid)` 建立设备上下文后，仍可用 `aclrtMemcpy`（D2H）读取这些区域。

因此回调内完成"读 args → 取各输入指针 → 逐个 D2H → 落盘"，即可在进程退出前抢救出完整现场。本例实测 ~39 MB 输入在回调内同步落盘，稳定成功。

### 3.3 两层能力架构

```
┌────────────────────────────────────────────────────────────┐
│ Python 层（ctypes 加载 so）                                 │
│   引导链路:   RegisterPublicExceptionDump(kernel)           │
│   三段式:     RegisterBySymbol(stub)                        │
│   输入登记:   AddInputInfo(name, idx, size, shape, dtype)   │
└──────────────────────────┬─────────────────────────────────┘
                           ▼
┌────────────────────────────────────────────────────────────┐
│ L1 通用捕获层（与算子无关，可整体复用）                      │
│  ① 提取公共信息（device/task/stream/retcode/异常类型）      │
│  ② kernel 名匹配过滤（mangled 子串匹配）                   │
│  ③ args 区 D2H → host                                      │
│  ④ 按登记的 name/ptrIndex/size 提取输入指针并 D2H 落盘      │
└──────────────────────────┬─────────────────────────────────┘
                           ▼
┌────────────────────────────────────────────────────────────┐
│ L2 算子适配层（每个新算子接入时唯一需要做的功课）            │
│  ① kernel 符号名适配（模板 mangled 名匹配，§4.2）           │
│  ② args 布局表（32 槽 ptrIndex 校准，§4.3）                 │
│  ③ tiling 内联结构解析（§4.4）                              │
│  ④ meta 前置算子输出（metadata）的登记（§4.5）              │
│  ⑤ stub 导出（原生三段式链路所需，§4.6）                    │
└────────────────────────────────────────────────────────────┘
```

---

## 4. IFA 算子侧的关键适配分析（L2 层）

### 4.1 调测对象执行形态

- **主算子 + meta 前置算子两段式**：`npu_fused_infer_attention_score_metadata` 先在设备侧生成 metadata（含 `usedCoreNum` 等，用于 blockDim），再由主算子消费；
- **主算子为 `<<<>>>` 模板直下发**：host 侧按 8 个模板实例（`<FD, LAYOUT, ANTIQ>` 组合）选择性地直接 launch，不走 aclrtLaunchKernel 封装；
- **tiling 结构体内联在 args 区**：紧跟 32 个入参指针之后（本例 offset 256 起）。

这三点分别决定了 §4.2 的名字匹配问题、§4.3 的 args 布局问题、§4.4 的 tiling 解析方式、§4.6 的 stub 导出需求。

### 4.2 kernel 符号名适配：mangled name 子串匹配

**现象**：注册 `incre_flash_attention` 后异常发生，回调报 `Kernel name mismatch (got=_Z21incre_flash_attentionILh0ELh1ELh0EE..., expected=incre_flash_attention)`，静默不 dump。

**根因（Itanium ABI 编码规则）**：C++ 模板函数编译为 kernel 符号时按 Itanium ABI 编码，函数名中的模板实参直接编进符号串：

```
_Z21incre_flash_attentionILh0ELh1ELh0EE...
 │  │                    │
 │  └── 21 = 函数名长度   └── 模板实参编码（Lh0 = unsigned char 0）
 └── _Z 前缀
```

注册名 `incre_flash_attention` 是 mangled 符号中的**子串**（`_Z21` 后紧跟的 21 个字符恰为函数名本体），但若回调只做 `strncmp` 前缀比较，`_Z21...` 显然不以注册名开头，必然失配。

**适配**：`match_kernel_name` 在前缀匹配失败后追加子串匹配：

```cpp
if (strncmp(ki.kernelName, g_kernelName, strlen(g_kernelName)) == 0) return true;  // 前缀
return strstr(ki.kernelName, g_kernelName) != nullptr;  // 追加：mangled 名子串匹配
```

公开接口版通过 `aclrtGetFunctionName` 直接获得 mangled 全名，子串匹配逻辑保留作为双保险。**通用结论：所有 `<<<>>>` 模板直下发算子都需要此适配。**

### 4.3 kernel args 布局与 ptrIndex 校准

**为什么必须校准**：回调按"args 区第 N 个 8 字节槽"取输入的 device 地址。不同算子的 args 布局完全由其 launch 代码决定，猜错槽位 → dump 到别的指针 → 数据对不上。IFA 还有一个关键差异：**`<<<>>>` 直下发时 args[0] 直接就是第一个形参 `query`**，不存在某些封装路径在前置的 fftsAddr 槽。

**IFA 布局表**（依据 `LAUNCH_INCRE_FA` 宏的形参顺序，0-based）：

| idx | 形参 | 本场景 | idx | 形参 | 本场景 |
|---|---|---|---|---|---|
| 0 | query | ✔ 输入 | 16 | kvPaddingSize | null |
| 1 | key | ✔ 输入 | 17 | keyAntiquantScale | ✔ dequant_scale_key |
| 2 | value | ✔ 输入 | 18 | keyAntiquantOffset | null |
| 3 | pseShift | null | 19 | valueAntiquantScale | ✔ dequant_scale_value |
| 4 | attenMask | null | 20 | valueAntiquantOffset | null |
| 5 | actualSeqLengthsQ | null | 21/22 | key/valueSharedPrefix | null |
| 6 | actualSeqLengths | ✔ actual_seq_kvlen | 23 | actualSharedPrefixLen | null |
| 7–13 | 量化类指针 | null | 24–26 | rope 类指针 | null |
| 14 | blocktable | ✔ block_table | 27 | dequantScaleQuery | null |
| 15 | queryPaddingSize | null | 28 | metaData | ✔ metadata（meta 算子输出） |
| — | — | — | 29/30 | attentionOut / softmaxLse | 输出（30 为 null） |
| — | — | — | 31 | workspace | 非空 |
| — | — | — | 256B 起 | tilingData | 内联结构体 |

**校准方法（实证而非推断）**：回调临时增加 args 探测——异常触发时打印 args 区前 40 个 8 字节槽的原始值，与 Python 侧打印的各 tensor `data_ptr()` 逐一对照：

```
args[ 0] = 0x000012c081200000   ↔  data_ptr(query)             ✔
args[ 1] = 0x000012c081400000   ↔  data_ptr(key_cache)         ✔
args[ 2] = 0x000012c082800000   ↔  data_ptr(value_cache)       ✔
args[ 6] = 0x000012c081249600   ↔  data_ptr(actual_seq_kvlen) ✔
args[14] = 0x000012c081248200   ↔  data_ptr(block_table)       ✔
args[17] = 0x000012c081249800   ↔  data_ptr(dequant_scale_key) ✔
args[19] = 0x000012c081249a00   ↔  data_ptr(dequant_scale_value)✔
args[28] = 0x000012c081249c00   ↔  data_ptr(metadata)          ✔
（未启用输入的槽位实测为 0，与布局表逐项吻合）
```

这份对照同时完成了布局表正确性验证 + ptrIndex 登记值确认。后续 bit-exact 比对构成第三重验证。

### 4.4 tiling 结构内联解析

args 区总长 1264 B = 前 256 B（32 槽 × 8 B 指针）+ 1008 B（tiling 及后续上下文）。回调额外把 args 区原文存为 `args_raw.bin`，离线按 `op_kernel/incre_flash_attention_tilingdata.h` 中 `IncreFlashAttentionBaseParams` 的字段序解析（offset 256 起，每字段 4 B）：

```
batchSize=18  seqSize=8320  qSeqSize=1  headSize=128  headSizeV=128
blockSize=128  maxBlockNumPerBatch=65  scaleValue=0.0883883461  kvHeadNum=1
qHeadNum=64  nNumOfQInOneGroup=64  batchContinuousFlag=1
```

**校验（12/12 一致）**。两个值得解释的值：

- `seqSize=8320 ≠ KV_SEQ=8192`：Paged Attention 虚拟序列长度按 blockSize 对齐补齐（8192 → 8320 = 65×128），佐证字段语义正确；
- `scaleValue=0.0883883 = 1/√128`：与 Python 侧 `softmax_scale` 一致。

tiling 被正确解析意味着：**从 dump 产物可以离线反推本次执行的完整输入规格**（B/H/D/block 数等），即使不回看构造脚本也能重建等价输入。

### 4.5 meta 前置算子的衔接

metadata 是 meta 算子的**运行时输出**（设备侧 Tiling 前置计算的结果，含 `usedCoreNum=18` 等），因此它的登记时机必须在 meta 算子执行之后：

```python
metadata = torch.ops.ascend_ops.npu_fused_infer_attention_score_metadata(**meta_param)
lib.AddInputInfo(b"metadata", 28, metadata.numel() * metadata.element_size(),
                 str(tuple(metadata.shape)).encode(), str(metadata.dtype).encode())
```

调测中 meta 算子本身不注入故障（保证 tiling/metadata 正确，故障集中在主算子输入侧，问题边界清晰）。metadata 同样被完整 dump（4096 B），其首字段 `usedCoreNum=18` 与 batchSize 一致（每 batch 一核），可作为 dump 侧的自洽性检查点。

### 4.6 stub 导出：原生三段式链路的前置条件

#### 4.6.1 问题：IFA 主算子 stub 外部不可获取

`aclrtGetFuncBySymbol` 的入参必须是 **kernel stub 函数指针**（不是名字字符串）。对 IFA 主算子，外部 `dlsym` 取不到该地址，原因有二：

1. **模板实例化 + LTO 优化**：8 个模板实例的 stub 符号被内联为本地符号，so 符号表中不存在；
2. **`__aicpu__` kernel 不在注册表**：meta 算子是 AICPU 类型，即使 dlsym 到地址也不命中。

#### 4.6.2 解决：算子 TU 内导出 stub

给主算子 TU 打补丁（`patches/ifa_kernel_symbol_export.patch`，+30 行），在定义 kernel 的翻译单元内取地址导出：

```cpp
extern "C" const void *ifa_get_kernel_symbol(uint32_t fd, uint32_t layout, uint32_t antiquant)
{
    switch (fd * 100u + layout * 10u + antiquant) {
        case 0:  return (const void *)&incre_flash_attention<0, 0, 0>;
        // ... 共 8 个模板实例
        default: return nullptr;
    }
}
```

Python 侧通过 ctypes 调用该导出函数获取 stub 地址，再传给 `RegisterBySymbol` 完成三段式注册。

#### 4.6.3 接口支持性实验证据

通过自建最小 AICore kernel（`demo/mini_kernel.cpp`）排除 IFA 干扰，验证 `aclrtGetFuncBySymbol` 对 `<<<>>>` 直调 kernel 的支持性：

| 测试 | 结果 | 结论 |
|---|---|---|
| stub 指针，**so 加载后、launch 前** | HIT → funcHandle | `cceModuleCtor` 在 so 加载时即完成 eager 注册 |
| stub 指针，launch 执行后 | HIT（同一 funcHandle） | launch 后仍可查询 |
| `__cce_rtKernelEagerLaunch` 内部函数地址 | MISS | 注册表 key 是 kernel stub，不是 launch 函数 |
| 名字字符串 | MISS | 入参必须是函数指针 |
| TU 内 `&kernel` 与 dlsym stub | 完全一致 | 取地址模式成立 |
| funcHandle → binHandle | 全通 | 三段式后半程有效 |

**结论**：`aclrtGetFuncBySymbol` **支持** `<<<>>>` 直调 kernel。曾出现"不可用"误判，失败原因是传错了 symbol（名字字符串 / AICPU kernel stub / 外部 dlsym 不到的模板内联符号），而非接口本身不支持。

---

## 5. 故障场景构建与注入机制

### 5.1 输入构造

与 `tests/incre_flash_attention/test.py` 完全一致的功能负载：B=18, QH=64, KVH=1（64:1 GQA）, q_seq=1, KV=8192, block=128, D=128, BNSD layout，BF16 query + INT8 KV cache（NZ 格式）+ BF16 反量化 scale。`torch.manual_seed(42)` 固定随机种子——**这是 bit-exact 验证的前提**：验证脚本用同种子重建参考输入，与 dump 产物逐位比对。

### 5.2 注入场景与物理机制

| 场景 | 注入（kernel 执行前） | 为什么必然触发 AICore error |
|---|---|---|
| scene1 | `block_table[0,0] = 0x7FFFFFFF` | PA 按 `blockTable[i] × blockByteSize` 计算 KV 块 GM 基址；0x7FFFFFFF 乘出的地址远超 KV cache 实际映射范围 → 越界访问 |
| scene2 | `actual_seq_kvlen[0] = 0x7FFFFFFF` | 设备侧读实际 KV 长度决定内层循环边界；长度爆炸 → 依序索引 block table 直至越界读 |
| baseline | 不注入 | —（对照组：验证回调不误触发） |

注入点选择依据：模拟**真实世界的坏数据**（上游写入损坏的索引/长度），而非构造算子代码分支错误——最贴近"线上问题复现"场景，且完全零代码侵入。

### 5.3 异常码的层次关系

- 设备侧回调收到 `retcode=0x7bc87`（AICore 异常码；公开访问器族 DeviceId/TaskId/StreamId/ErrorCode/Args/FuncHandle/FunctionName 不含异常类型字段，`expandType` 为旧内部接口字段）；
- Python 侧同步看到 `SUSPECT REMOTE ERROR, error code is 507057`。

两者是**不同层的错误表示**（设备异常码 vs 框架上抛码），不需要也不应当做数值换算。异常码可能随 CANN 版本变化，**以回调触发（kernel 名匹配）与 dump 是否落盘为判定标准**。

---

## 6. 能力构建范式（五步法，可复制到其他算子）

> 对任意 `<<<>>>` 或 aclrtLaunchKernel 下发的算子通用，L1 层 so 可原样复用。

**S1 确定 kernel 符号名**
- 做法：读算子 host 侧 launch 代码（IFA 为 `LAUNCH_INCRE_FA` 宏展开的 `<<<blockDim, nullptr, aclstream>>>`），取被调用 kernel 的函数名。
- 验证：异常日志中 `Kernel name matched`（失配会打印 got/expected 两者）。
- 坑：模板 kernel 上报 mangled 名 → 必须带子串匹配适配（§4.2）。

**S2 校准 args 布局（ptrIndex）**
- 做法：从 launch 代码抄录形参顺序为布局表；运行一次带 args 探测的 dump，与 Python 侧 `data_ptr()` 对照实证。
- 验证：非空槽位置、指针值双重吻合（§4.3）；最终以 bit-exact 比对收口。
- 坑：不同 launch 封装 args[0] 语义不同（有无 fftsAddr 前置槽）；可选输入为 null 时槽位**占位但不跳过**，索引以形参序为准。

**S3 准备回调 so**
- 命令：`g++ ... -lacl_rt -lascendcl`（见 README §4.3 第 1 步，全部公开库/头）。
- 验证：`nm -D *.so | grep -E ' T '` 看导出符号。

**S4 注册回调并登记输入**
- 引导链路：`RegisterPublicExceptionDump(kernelName)`；
- 原生三段式：算子 TU 导出 stub（§4.6）→ `RegisterBySymbol(stub)`；
- 输入登记：所有输入 tensor 创建完成后、kernel 执行前逐个 `AddInputInfo`（meta 类运行时生成的输入在其生成后补登记）；每次用例前 `ClearInputInfo()` 防残留。
- 坑：kernel 名必须 bytes（`b"..."`）；注册放模块级而非每个用例内。

**S5 验证闭环**
- 构造固定 seed 的可控故障 → 触发异常 → 三重验证：
  1. 完整性：`saved_inputs=N/N` + `inputs_info.txt` 与登记一致；
  2. 正确性：同 seed 重建参考输入，逐 tensor bit-exact 比对（注入值按位置打补丁后参与比对）；
  3. 自洽性：args 指针区与 `data_ptr()` 对照；tiling 解析与调用参数对照。
- 坑：dump 目录以 **CWD** 为基准落盘，复跑前 `rm -rf task_fail_dump` 清理。

---

## 7. 逐步骤操作指南

> 全部命令在 `ops_dev_sw` 容器内执行。详见 [README.md](README.md) §4（执行手册），本节给出关键步骤与预期。

### 7.1 环境准备

```bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh
export NPU_SOC_VERSION=ascend910_93
cat $ASCEND_HOME_PATH/aarch64-linux/ascend_toolkit_install.info | grep version   # → 9.1.T103
npu-smi info | head -6
```

### 7.2 算子构建与安装

```bash
cd <ops仓>/examples/fast_kernel_launch_example
git apply <本包>/patches/ops_transform_local.patch            # 构建适配（未打过时）
git apply <本包>/patches/ifa_kernel_symbol_export.patch       # 可选：原生三段式所需
export OPS_BUILD_FILTER=incre_flash_attention,incre_flash_attention_meta
python3 setup.py clean && python3 -m build --wheel --no-isolation
pip install dist/ascend_ops-1.0.0-*.whl --force-reinstall --no-deps
python3 -c "import torch,ascend_ops; print(torch.ops.ascend_ops.npu_fused_infer_attention_score is not None)"  # → True
```

### 7.3 编译回调 so

```bash
cd <本包>/src
g++ -g -std=c++11 -fPIC -shared exception_dump_public.cpp -o exception_dump_public.so \
    -lacl_rt -lascendcl -L$ASCEND_HOME_PATH/lib64 -I$ASCEND_HOME_PATH/include
nm -D exception_dump_public.so | grep -E ' T '
#   → AddInputInfo / ClearInputInfo / RegisterPublicExceptionDump / RegisterBySymbol
```

### 7.4 引导链路 dump（主路径）

```bash
cd <本包>/test && rm -rf task_fail_dump
python3 test_public_dump.py scene1     # 全局回调 dump + 三段式挂载（exit code 2 为预期）
python3 verify_public_dump.py          # 7/7 bit-exact
python3 parse_ifa_tiling.py            # 12/12 字段一致
python3 test_public_dump.py scene2     # per-binary 接管（独立进程）
python3 test_public_dump.py baseline   # 零误触发（独立进程）
```

### 7.5 原生三段式 dump

```bash
python3 test_ifa_native3stage.py       # 8/8 注册 + scene1 dump（需 symbol 补丁）
```

### 7.6 数据验证

```bash
# bit-exact（需 torch）：
python3 verify_public_dump.py
#   预期：7/7 tensor BIT-EXACT；metadata usedCoreNum=18

# tiling 解析（离线）：
python3 parse_ifa_tiling.py
#   预期：args[0..31] 指针区 + BaseParams 12/12 字段一致

# md5 交叉：
md5sum task_fail_dump/dev*/query.bin
#   预期：6789400e0fadc7fa271b0b8ed8f29e0d
```

**bit-exact 为什么可信**：三重条件——同 seed 重建（数据生成确定）+ shape/dtype 按 `inputs_info.txt` 还原（解释无歧义）+ `torch.equal` 逐位比较（无容差）。int8/int32/int64 按字节 view；bfloat16 按 uint16 读入后 `view(torch.bfloat16)` 避免浮点路径引入噪声。

---

## 8. 调测结果与证据链

### 8.1 引导链路 scene1 回调日志

```
[ExcDumpPub] (global-cb) Task fail detected: deviceId=0, taskId=3, streamId=47, retcode=0x7bc87
[ExcDumpPub] (global-cb) reported kernel: _Z21incre_flash_attentionILh0ELh1ELh0EE...
[ExcDumpPub] (global-cb) 3-stage chain armed: funcHandle=0xaaabb3562ba0 -> binHandle=0xaaabb3508920 -> BinarySetExceptionCallback OK
[ExcDumpPub] (global-cb) devArgs: ptr=0x12c100000080, len=1264, numInputs=8
[ExcDumpPub] (global-cb) Done. Saved 8/8 inputs to ./task_fail_dump/dev0_task3_ts<ts>
```

- 三段式挂载在首次异常回调内完成；
- devArgs `ptr=0x12c100000080, len=1264` 与 launch 侧下发的 args 区一致，8 输入 dev_addr 全部一致；
- 同进程 scene2 由 binary-cb 接管 dump 8/8（三段式后续链路验证通过）；
- baseline 正常结束，零产物、回调零误触发。

### 8.2 原生三段式实测

| 验证项 | 结果 |
|---|---|
| 8 个模板实例 `RegisterBySymbol` 三段式注册 | **8/8 全部 HIT**（launch 前完成挂载） |
| scene1 注入后回调触发 | **binary-cb dump 8/8**，全程无全局回调参与 |
| 数据正确性 | **7/7 bit-exact**，`query.bin` md5 与引导链路一致 |

### 8.3 完整性：8/8 输入 + args 原文

`inputs_info.txt` 实测：

| 输入 | ptrIndex | shape | dtype | size |
|---|---|---|---|---|
| query | 0 | (18, 64, 1, 128) | bfloat16 | 294912 B |
| key_cache | 1 | (1170, 1, 4, 128, 32) | int8 | 19169280 B |
| value_cache | 2 | (1170, 1, 4, 128, 32) | int8 | 19169280 B |
| actual_seq_kvlen | 6 | (18,) | int64 | 144 B |
| block_table | 14 | (18, 65) | int32 | 4680 B |
| dequant_scale_key | 17 | (1, 1, 128) | bfloat16 | 256 B |
| dequant_scale_value | 19 | (1, 1, 128) | bfloat16 | 256 B |
| metadata | 28 | (1024,) | int32 | 4096 B |

另落盘 `args_raw.bin`（1264 B）。

### 8.4 场景矩阵汇总

| 场景 | 注入 | 引导链路 | 原生三段式 |
|---|---|---|---|
| scene1 | block_table 越界 | 触发，8/8，7/7 bit-exact | 触发，8/8，7/7 bit-exact |
| scene2 | actual_seq_kvlen 越界 | 触发，binary-cb 接管 | —（同机制抽验一致） |
| baseline | 无 | 正常，零产物 | —（D9 场景内对照） |

md5 交叉：`query.bin` = `6789400e0fadc7fa271b0b8ed8f29e0d`，两链路产物一致。

---

## 9. 关键问题与解决记录

| # | 现象 | 根因 | 解决 | 验证 |
|---|---|---|---|---|
| 1 | 注册成功但异常后不 dump | `<<<>>>` 模板 kernel 上报 Itanium mangled 名；回调仅 `strncmp` 前缀匹配 | `match_kernel_name` 追加 `strstr` 子串匹配 | 日志 `Kernel name matched`，dump 落盘 |
| 2 | 回调命中但数据对不上 | IFA `<<<>>>` 直下发时 args[0] 即第一个形参，无 fftsAddr 前置槽；ptrIndex 按经验值错位 | 加 args 探测，与 `data_ptr()` 逐槽对照，重建布局表 | 指针区吻合 + bit-exact 收口 |
| 3 | tiling 解析 `struct.error` | 字段格式串误用 numpy 记法 `u4`，Python struct 合法格式为 `I`/`f` | 修正格式串 | 12/12 字段通过 |
| 4 | wheel 编译失败 / pytest 无输出 | SK、aclprof 新 API 与本机 CANN 版本差异 + 无关算子拖累；测试为脚本式设计 | 适配补丁 + `OPS_BUILD_FILTER` 白名单；改用 `python3 test.py` 直跑 | 构建安装成功 |
| 5 | 曾误判 `aclrtGetFuncBySymbol` 不支持 `<<<>>>` | 传错 symbol（名字字符串 / AICPU stub / 外部 dlsym 不到的模板内联符号） | 自建 mini kernel 证明 stub 指针可命中；IFA 打补丁 TU 内导出 stub | 8/8 实例注册 HIT |
| 6 | `mini_kernel` 链接报 `undefined symbol: AscendLaunchKernelWithHostArgs` | g++ 不含 fast kernel launch 胶水函数 | 改用 bisheng 链接 | 编译通过 |
| 7 | Python 脚本 `Stream` 对象属性错误 | `torch.npu.current_stream().cuda_stream` 不适用于 NPU | 改为 `.npu_stream` | stream 传递正确 |
| 8 | `exp_callback.py` ctypes 回调崩溃 | Python CFUNCTYPE 在 runtime 线程无 GIL | 回调必须 C 层实现 | 已按此设计 |

---

## 10. 边界、限制与注意事项

1. **覆盖范围**：dump 的是**输入侧** tensor。输出（args[29]）与 workspace（args[31]）地址同样可从 args 区取到，如需登记追加 `AddInputInfo` 即可；但异常路径上输出的写穿内容属未定义数据，定位价值有限。
2. **进程退出时序**：AICore error 后进程以错误码退出，dump 必须在回调内**同步**完成（`aclrtSetDevice` → D2H → 落盘 → `aclrtResetDevice`）。~39 MB 输入稳定成功；更大输入量需评估回调内耗时与 host 内存。
3. **正常路径零开销的机理**：注册只是登记函数指针与过滤名单，Runtime 仅在任务失败时才调用回调；kernel 正常执行路径不经过任何 dump 逻辑（baseline 实测零产物、零日志）。
4. **输出目录以 CWD 为基准**：产物落在回调触发进程的当前工作目录 `./task_fail_dump/`；复跑前清理。baseline 的"零产物"判定标准是**本轮无新增 `dev*_ts*` 目录**（对照时间戳），而非目录为空。
5. **退出码语义**：注入场景 exit code 2 属预期成功路径；baseline 正常结束 exit code 0。自动化以日志与 dump 产物判定，勿按 exit code 判失败。
6. **多场景分进程**：同进程 AICore error 后设备流状态未恢复，紧接的后续 launch 会报 507057（非回调误触发）。
7. **双回调各 dump 一次**：引导链路挂载完成后，同一异常 global-cb 与 binary-cb 可能各落盘一次（内容一致，时间戳目录区分）。
8. **全局回调为进程级**：`aclrtSetExceptionInfoCallback` 若宿主框架也注册会互相覆盖——集成到官方框架时优先原生三段式链路。
9. **so 与环境绑定**：包内 so 为 aarch64 + CANN 9.1.T103 编译产物，新环境必须重编译。
10. **mangled 名补丁是通用前提**：未做子串匹配的回调实现在任何模板直下发算子上都会静默失配——接入新算子时最先确认此点。

---

## 11. 后续演进建议

1. **Python 封装**：将 ctypes 注册收敛为 `enable_exception_dump(kernels)` 一类接口，屏蔽 so 加载/argtypes 细节；
2. **handler 注册表**：把"布局表 + tiling 解析"沉淀为按 kernel name 注册的 L2 handler，新算子接入从"写脚本"降为"填一张布局表"；
3. **dump 目录/级别可配**：环境变量化（`DUMP_DIR` / `DUMP_LEVEL`），支持大 tensor 场景仅记录元信息不搬数据；
4. **输出侧选择性 dump**：对"写坏输出"类问题（NaN 检测）扩展输出登记；
5. **多 stream 并发验证**：补充并发异常场景的压力测试。

---

## 附录 A：环境与版本信息（实测核查值）

| 项 | 实测基线 |
|---|---|
| 容器 | ops_dev_sw |
| CANN | 9.1.T103 |
| 编译器 | bisheng（clang 15.0.5，aarch64） |
| 设备 | Ascend 910C（910_9392，24 AIC + 48 AIV，HBM 62GB），NPU 0 卡 |
| Python / torch | 3.11.9 / 2.7.1 + torch_npu 2.7.1.post2 |
| 算子包 | ascend_ops 1.0.0（cp311-abi3-linux-aarch64 wheel） |
| kernel args 规模 | 1264 B = 256 B 指针区 + 1008 B tiling 区 |
| 异常码 | 设备侧 retcode=0x7bc87；Python 侧 507057 |

## 附录 B：术语表

| 术语 | 含义 |
|---|---|
| AICore error | kernel 在 AI Core 上执行失败（访存越界、非法指令等），Runtime 以 `RT_EXCEPTION_AICORE` 上抛 |
| args 区 | kernel launch 时下发的设备侧参数缓冲：入参指针（每槽 8 B）+ 内联 tiling 等标量结构 |
| tiling | host 侧为 kernel 计算的分块/调度参数结构体，IFA 中内联于 args 区 offset 256 起 |
| ptrIndex | 某输入的 device 指针在 args 区中的槽位序号（0-based，8 B/槽） |
| mangled name | C++ 符号按 Itanium ABI 编码后的名字；模板实参直接编入符号串 |
| bit-exact | 逐位相等比较（`torch.equal`），无任何数值容差 |
| GQA | Grouped Query Attention，本场景 QH:KVH = 64:1 |
| Paged Attention（PA） | KV cache 分块管理，按 block table 中块 id × 块字节大小计算 GM 地址 |
| kernel stub | kernel 函数指针，`aclrtGetFuncBySymbol` 接口的关键入参 |
| eager 注册 | cce 编译器在 so 加载时（`cceModuleCtor`）自动注册 kernel stub 到 Runtime 注册表 |
| 三段式 | `GetFuncBySymbol → FunctionGetBinary → BinarySetExceptionCallback` 的 per-binary 回调挂载链路 |
