# NPU 算子异常信息自动 Dump 使用文档

> NPU kernel 运行时异常发生后，系统会自动 dump 出问题算子的异常上下文与 kernel 参数（args），用于离线问题定位（可按算子 args 布局进一步还原输入输出 tensor）。该能力覆盖基于 AscendCL Runtime 下发的自定义算子 kernel（__AICore kernel__）。接入采用外挂库方案，算子源码无需改动，通过 Python API `enable_exception_dump(kernels)` 一键开启，支持按 kernel name 前缀做过滤。
>
> 在异常自动 dump 之外，Python 模块另提供__输入/输出 tensor dump__ 能力（纯 Python 实现，不依赖 so）：在算子调用边界把输入/输出 tensor 的完整数据（`.bin`）与元信息（`.json`）落盘，与异常产物 `_args.bin` 中的 device 指针交叉验证，详见 1.6 节。

---

## 0. 能力总览

| 能力点 | 实现层 | 开启方式 | 产物 | 详见 |
|---|---|---|---|---|
| 异常自动 dump | C++ 外挂库回调（`libxpuops_exception_dump.so`） | `enable_exception_dump(["kernel前缀"])`，算子源码零改动 | `{kernel}_dev{id}_task{id}_{ts}_info.txt` / `_args.bin`（异常时刻公共信息 + 原始 kernel args） | 1.1、1.5、2.2 |
| 输入 tensor dump | 纯 Python（不依赖 so、无需编译） | `run_with_tensor_dump(..., dump_inputs=True)`（调用边界自动）或 `dump_tensors(..., stage="input")`（手动） | `input_{tag}_{name}_dev{idx}_{ts}.bin/.json`（数据 + 元信息） | 1.6 |
| 输出 tensor dump | 纯 Python（不依赖 so、无需编译） | `run_with_tensor_dump(..., dump_outputs=True)`（仅算子正常返回后产出） | `output_{tag}_{name}_dev{idx}_{ts}.bin/.json`（可作对拍基准） | 1.6 |
| dump 目录 / 级别配置 | 环境变量（两类能力共用目录） | `XPUOPS_DUMP_DIR` / `XPUOPS_DUMP_LEVEL` / `XPUOPS_DUMP_LIB_DIR` | — | 1.5 |

> 换环境部署时的实机验证步骤与预期结果见 `docs/VERIFICATION_GUIDE.md`，验证结论与本文档逐条对齐。

---

## 1. IFA 算子 Demo 验证

包内提供一个可直接运行的 demo（IFA），覆盖 AICore kernel 异常 dump 完整链路，用于验证工具功能与环境可用性。

### 1.1 方案机制简述

外挂库 `libxpuops_exception_dump.so` 采用__单层设计__：注册一个 AscendCL Runtime 异常回调，异常发生时统一完成公共信息提取、args 拷贝与落盘，不依赖任何算子级 C++ 代码：

- **注册接口运行期自适应**：CANN 9.2.0 及以上使用 `aclrtExceptionInfoCallbackRegister`，旧版本（如 9.1）回退 `aclrtSetExceptionInfoCallback`（该接口自 CANN 9.2.0 起标记废弃、2027/9/30 后移除）。同一份 so 兼容新旧 CANN，无需按版本重新编译（原理：so 编译时不引用版本特定符号，运行期探测当前 CANN 实际提供的接口后选择注册路径）；
- __kernel name 前缀过滤__：`enable_exception_dump(["incre_flash_attention"])` 指定关心的 kernel（前缀匹配，不匹配的 kernel 打印一条 skip 日志、不落盘）；不传参则对所有 kernel 生效；
- __幂等__：重复调用 `enable_exception_dump` 无副作用，回调仅注册一次，后续调用仅更新前缀列表。

```ini
异常发生
   │  Runtime 上报（公共信息 + device args）
   ▼
[通用异常回调]
   ├─ 提取公共信息：device / task / stream / thread / error code / kernel name
   ├─ 前缀过滤：kernel 不在启用列表 → skip
   ├─ args D2H（device → host 拷贝完整 kernel args）
   └─ 落盘：{kernel}_dev{id}_task{id}_{ts}_info.txt   公共信息
            {kernel}_dev{id}_task{id}_{ts}_args.bin   原始 kernel args（含全部 GM 指针与 tiling）
```

`args.bin` 是异常发生时 kernel 的**完整入参**：各输入/输出 tensor 的 device 指针 + tiling 结构体等标量参数。结合算子的 args 布局定义即可离线还原每个 tensor 的地址与大小（指针值 + tiling 中的 shape 字段），是异常定位的核心数据。

### 1.1.1 输入/输出 tensor dump（Python 侧，调用边界）

C++ 回调只能拿到原始 args（指针 + tiling），不具备 tensor 布局知识，无法通用还原 tensor 数据；而输入/输出 tensor 在 __Python 调用边界__ 天然完整可获取（含 shape/dtype/数据）。因此输入/输出 tensor dump 由 Python 模块实现（`dump_tensor` / `run_with_tensor_dump` 等，见 1.6 节），与异常自动 dump 互补：

```ini
算子调用边界（Python 侧）
   ├─ 调用前：dump 全部输入 tensor（数据 + 元信息） -> {stage}_{op}_{name}_{dev}_{ts}.bin/.json
   ├─ 正常返回后：dump 全部输出 tensor             -> 同上（stage=output）
   └─ 异常时：输入已落盘（故障注入后的现场），输出不可得
      （kernel 未正常写回；输出 tensor 的指针现场由异常回调的 _args.bin 提供）
```

两类产物的联动关系：tensor dump 的 `.json` 中记录了 dump 时刻的 device 端地址（`data_ptr`），与异常 `_args.bin` 中对应入参的 GM 指针一致，可交叉验证"异常 kernel 即本次调用"；`_args.bin` 提供异常时刻的指针现场，tensor dump 提供调用边界的真实数据（含正常调用的输出基准，可用于对拍）。

示例算子 `incre_flash_attention`（IFA，Ascend C 实现，跑在 AICore）源码位于 ops-transformer 仓 `examples/fast_kernel_launch_example/csrc/incre_flash_attention/ascend910_93/op_kernel/`；demo 运行时通过 pip 算子库 `ascend_ops` 调用该算子（该 pip 包与 ops-transformer 源码对应），算子内核在 host 侧以 Ascend C `<<< >>>` 语法直调下发。前缀以异常日志打印的 `Kernel Name:` 实际值为准。

### 1.2 环境准备

| 项目 | 版本 |
|---|---|
| 开发容器 | ops_dev_sw |
| CANN | cann-9.1.T103 |
| PyTorch / torch_npu | 2.7.1 / 2.7.1 |
| 算子库 | ascend_ops（pip 安装，含 IFA 算子） |
| 硬件 | Ascend 910（910_93） |

```bash
docker exec -it ops_dev_sw bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh
```

包目录结构（本包路径 `exception_dump_package/`）：

```ini
exception_dump_package/
├── README.md                        # 本文档
├── docs/
│   ├── RELEASE_NOTES.md             # 交付版本说明：文件清单、关键机制速览
│   └── VERIFICATION_GUIDE.md        # 服务器实机验证手册：步骤、预期输出与结论对照
├── xpuops_exception_dump/           # 外挂库工程本体
│   ├── include/                     # 公开 C 接口 xpuops_exception_dump.h
│   ├── src/                         # C++ 源码（callback / init / 工具，共 5 个文件）
│   ├── python/xpuops/               # Python 接口（enable_exception_dump + tensor dump 三件套）
│   ├── lib/libxpuops_exception_dump.so  # 已编译产物（910_93 + cann-9.1.T103 环境）
│   ├── CMakeLists.txt
│   └── build.sh                     # 编译脚本（产物输出到 lib/）
├── examples/
│   └── demo_ifa.py                  # demo 脚本（可直接运行）
└── output_samples/                  # demo 的 dump 产物样例
    └── ifa/                         # 2 个文件（异常 dump 历史产物），前缀 incre_flash_attention_dev0_task3_20260924_103307_467
```

> `output_samples/` 的 `_info.txt` 中记录的绝对路径为产物生成时所在环境的历史信息，与当前包路径不同属正常现象。demo 运行后包根可能多出 CANN 自动生成的 `extra-info/` 目录、Python 的 `__pycache__/`，均为运行时副产物，可安全删除。

### 1.3 编译外挂库

```bash
cd exception_dump_package/xpuops_exception_dump
bash build.sh        # 内部执行 cmake + make，产物输出到 lib/
```

编译产物：`lib/libxpuops_exception_dump.so`。包内已附带一份同环境编译好的 so，运行环境一致时可直接使用、跳过本步。工程无任何包外依赖，仅需 CANN 环境（`set_env.sh` 已 source）。

> 注意：`build.sh` 会覆盖 `lib/` 下已附带的 so。

### 1.4 Demo：incre_flash_attention（AICore kernel）

```bash
cd exception_dump_package
python3 examples/demo_ifa.py
```

demo 核心流程（完整代码见 `examples/demo_ifa.py`）：

```python
from xpuops.debug import enable_exception_dump, run_with_tensor_dump

enable_exception_dump(["incre_flash_attention"])   # 按 kernel 前缀开启，可重复调用（幂等）
# 1. 构造 query/key/value/block_table 等输入，正常跑一遍算子（验证环境与输入正确）
#    调用边界自动 dump 输入 tensor（input_*.bin/.json）与输出 tensor（output_*.bin/.json）
out, lse = run_with_tensor_dump(
    torch.ops.ascend_ops.npu_fused_infer_attention_score, tag="npu_fused_infer_attention_score", **params)
# 2. 故障注入：将 block_table 内容改为非法值 -> 触发 AICore kernel 异常
#    （污染输入在调用前自动落盘；异常时输出不可得）
# 3. callback 自动 dump 公共信息 + kernel args，脚本最后自动校验产物大小与信息
```

__验证结果__（异常自动 dump 链路已在 910_93 + cann-9.1.T103 环境实测通过）：终端输出 `[SIZE-OK] args.bin: 1264 bytes == expected 1264`、3 条 `[INFO-OK]`（kernel_name / error_code / args_dumped）与 `[PASS] IFA exception dump OK`；产物生成在包根下的 `./exception_dump_ifa/`（demo 内部固定了该输出目录，可自行删除）。输入/输出 tensor dump 校验项（`[INPUT-OK]` / `[OUTPUT-OK]` / query 大小 / 污染 block_table 读回）为本次新增能力的预期行为，产物说明见下表，其服务器实测待环境恢复后补充（实现为纯 Python 标准接口，见 1.6 节设计说明）。换环境复现验证的完整步骤与逐项预期结果见 `docs/VERIFICATION_GUIDE.md`。

__产物清单__（`output_samples/ifa/` 内附一份历史产物供比对，仅含异常 dump 两件套）：

| 产物 | 大小 | 说明 |
|---|---|---|
| `_args.bin` | 1264 B | 原始 kernel args：32 个 GM 指针（各输入/输出 tensor 的 device 地址）+ TilingData 结构体 |
| `_info.txt` | - | 公共信息块：timestamp / device_id / task_id / stream_id / thread_id / error_code / kernel_name / raw_kernel / args_bytes 等 |
| `input_{op}_{name}_dev{id}_{ts}.bin/.json` | - | 输入 tensor 数据与元信息（Python 调用边界产出，正常与异常调用各落盘一份） |
| `output_{op}_{name}_dev{id}_{ts}.bin/.json` | - | 输出 tensor 数据与元信息（仅正常调用产出，如 attention out / lse，可作对拍基准） |

故障注入方式与异常表现：demo 将 block_table 全部填为非法块索引 `0x0FFFFFF0`，kernel 核内按表索引访问 GM 越界，Runtime 上报异常 error code 0x7bc87（507015），Python 侧表现为 `RuntimeError`。

### 1.5 dump 配置与产物说明

dump 目录与级别由环境变量控制：

| 环境变量 | 含义 | 默认值 |
|---|---|---|
| `XPUOPS_DUMP_DIR` | dump 输出目录（不存在时自动创建；__同时作用于异常 dump 与 1.6 节的 tensor dump__） | `./exception_dump/` |
| `XPUOPS_DUMP_LEVEL` | 0=仅公共信息；1=公共信息 + args（仅影响异常 dump） | 1 |
| `XPUOPS_DUMP_LIB_DIR` | 指定 so 所在目录（Python 绑定查找 so 用） | 工程内 `lib/` |

产物命名：`{kernel_name}_dev{device_id}_task{task_id}_{时间戳}_args.bin` 与同前缀 `_info.txt`。

异常公共信息同时打印到 stdout（`[XPUOPS]` 前缀）：

```text
========== XPU Exception Dump ==========
Timestamp:    20260924_103307_467
Device ID:    0
Task ID:      3
Stream ID:    47
Thread ID:    191133
Error Code:   0x7bc87
Kernel Name:  incre_flash_attention
Raw Kernel:   _Z21incre_flash_attentionILh0ELh1ELh0EE...（修饰名）
Args Size:    1264 bytes
=========================================
```

### 1.6 输入/输出 Tensor Dump（Python 侧）

#### 1.6.1 能力与 API

Python 模块 `xpuops.debug` 提供 tensor dump 三件套，覆盖"dump 输入 tensor、dump 输出 tensor"两类需求。实现为纯 Python（仅使用 torch / torch_npu 标准接口：`.cpu()` D2H 同步拷贝、`view(torch.uint8)` 字节导出），__不依赖外挂库 so、无需重新编译、不改动任何 C++ 代码__：

| API | 说明 |
|---|---|
| `run_with_tensor_dump(func, *args, tag=None, dump_inputs=True, dump_outputs=True, dump_dir=None, **kwargs)` | 调用边界自动 dump：调用前递归扫描 args/kwargs 中全部 tensor（含 dict/tuple 嵌套）落盘为 `input_*`；正常返回后 dump 返回值中全部 tensor 为 `output_*`；异常原样上抛（此时输入已落盘、输出不可得）。单个 tensor dump 失败仅告警不中断业务 |
| `dump_tensors(named_tensors, stage=None, dump_dir=None)` | 批量 dump 一组命名 tensor；`stage` 常用 `"input"` / `"output"`（仅作文件名前缀），如 `dump_tensors({"query": q, "block_table": bt}, stage="input")` |
| `dump_tensor(tensor, name, stage=None, dump_dir=None)` | dump 单个 tensor（上面两者的底层原语），返回 `.bin` 路径 |

```python
from xpuops.debug import run_with_tensor_dump, dump_tensors

# 方式一：调用边界自动 dump 输入/输出（推荐，异常场景输入现场自动保全）
out, lse = run_with_tensor_dump(
    torch.ops.ascend_ops.npu_fused_infer_attention_score,
    tag="npu_fused_infer_attention_score", **fa_param)

# 方式二：手动指定要 dump 的 tensor（如只关心部分输入）
dump_tensors({"query": fa_param["query"], "block_table": fa_param["block_table"]}, stage="input")
```

#### 1.6.2 产物格式与读回

每个 tensor 产出两个文件（与异常 dump 同目录，命名风格对齐）：

- `{stage}_{tag}_{name}_dev{idx}_{时间戳}.bin`：tensor 原始数据字节（host 侧）。dtype 以字节视角原样导出（bf16/fp16 等同样支持），不做数值转换；
- 同前缀 `.json`：元信息，字段如下。

| 字段 | 说明 |
|---|---|
| `name` / `stage` | tensor 名（调用边界自动命名，如 `query`、`metadata[0]`；`run_with_tensor_dump` 场景下 name 自带算子标签前缀 `{tag}_`）/ 阶段（input/output）。注：`tag` 不作为独立 json 字段，已融入文件名与 `name` 前缀 |
| `shape` / `dtype` / `itemsize` / `numel` / `nbytes` | 形状、类型（如 `torch.bfloat16`）、单元素字节数、元素数、总字节数 |
| `device` / `data_ptr` | dump 时刻所在设备（如 `npu:0`）与 device 端地址——__与异常 `_args.bin` 中对应 GM 指针一致，可交叉验证__ |
| `bin_file` / `timestamp` | 数据文件名、生成时间（与 C++ 侧时间戳风格一致） |

离线读回（任意有 torch 的环境即可，无需 NPU）：

```python
import json, torch

with open("input_npu_fused_infer_attention_score_query_dev0_20260927_101010_123.json", encoding="utf-8") as fp:
    meta = json.load(fp)
with open(meta["bin_file"], "rb") as fp:          # bin_file 与 json 同目录
    buf = fp.read()

dtype = getattr(torch, meta["dtype"].rsplit(".", 1)[-1])   # "torch.bfloat16" -> torch.bfloat16
t = torch.frombuffer(bytearray(buf), dtype=torch.uint8)    # frombuffer 需可写 buffer，用 bytearray 包装
t = t.view(dtype).reshape(meta["shape"])                   # 字节视角 -> 原生 dtype -> 原 shape
```

#### 1.6.3 设计边界（重要）

- __异常时刻输出不可得__：kernel 异常时未正常写回输出，`run_with_tensor_dump` 在异常路径不产出 `output_*`；输出 tensor 的指针现场由异常回调的 `_args.bin` 提供，正常调用的输出 dump（对拍基准）需在正常路径产出；
- __输入 dump 时点__：`run_with_tensor_dump` 在算子下发前落盘输入，对故障注入场景即"污染后的现场"（demo 的 block_table 污染值即以此保全，读回首 int32 == `0x0FFFFFF0`）；
- __与 C++ 回调的分工__：C++ 回调持有异常时刻的 args 指针现场但无 tensor 布局知识；Python 调用边界持有完整 tensor 但在异常后 device 内存可能已释放。两者产物互补，不在回调内做 tensor 还原（保证单层设计与零算子级代码的既有边界）；
- __dtype 兼容__：`.bin` 为字节视角导出，bf16/fp16/int8/fp32 等通用（规避 numpy 对 bf16 的限制）；空 tensor 落盘 0 字节 `.bin`，元信息照常记录。

---

## 2. 业务算子接入指南

### 2.1 接入前提与判定

1. 算子为 AICore kernel，通过 AscendCL Runtime 下发，最常见即 Ascend C `<<< >>>` 语法直调（本包 demo 算子即采用该方式），也支持 `aclrtLaunchKernelWithHostArgs` / `aclrtLaunchKernelWithConfig`；
2. 拿得到该 kernel 的 args 布局定义（形参顺序、tiling 结构体）即可离线解析 `_args.bin`，还原各 tensor 的 device 地址与大小——__没有布局也能拿到原始 args，只是无法进一步解析出 tensor__；
3. 输入/输出 tensor dump（1.6 节）不依赖上述两点，只要业务在 Python 侧持有 torch tensor 即可使用（纯 Python 实现，不要求重新编译或改动 C++ 代码）。

### 2.2 接入步骤

**Step 1：开启**（新算子接入仅需此步，业务代码任意位置，建议初始化阶段）

```python
from xpuops.debug import enable_exception_dump

enable_exception_dump(["my_kernel"])    # 前缀列表；不传参则对所有 kernel 生效
```

无需编写任何 C++ 代码、无需重新编译业务工程与外挂库。前缀取 demangle 后的 kernel 函数名（模板参数已去掉，同一 kernel 的所有模板实例匹配同一前缀），不确定时以异常日志中 `Kernel Name:` 打印值为准。

**Step 2（可选）：dump 输入/输出 tensor**

需要保全调用现场数据（输入/输出 tensor）时，将算子调用包一层即可（详见 1.6 节）：

```python
from xpuops.debug import run_with_tensor_dump

out = run_with_tensor_dump(torch.ops.my_lib.my_op, tag="my_op", *args, **kwargs)  # 自动 dump 输入+输出
```

**Step 3（可选）：配置**

```bash
export XPUOPS_DUMP_DIR=/data/dump        # dump 目录（异常 dump 与 tensor dump 共用），默认 ./exception_dump/
export XPUOPS_DUMP_LEVEL=1               # 0=仅公共信息，1=公共信息+args（仅影响异常 dump）
export XPUOPS_DUMP_LIB_DIR=/path/to/so   # so 不在工程内 lib/ 时指定
```

**Step 4：验证**

正常运行业务 → 触发（或等待）kernel 异常 → 确认 dump 目录生成 `{kernel}_dev{dev}_task{task}_{ts}_args.bin` 与 `_info.txt`，核对 error_code / kernel_name 与 Runtime 日志一致；使用了 Step 2 时另确认 `input_*` / `output_*` 产物已生成。

### 2.3 常见问题（FAQ）

| 现象 | 原因与处理 |
|---|---|
| 日志显示 `kernel '...' not in enabled prefix list, skip dump` | kernel 不在启用前缀列表。查看该行日志中的实际 kernel 名，修正 `enable_exception_dump` 的前缀；或调用时不传参（对全部 kernel 生效） |
| 日志显示 `args unavailable`，`_info.txt` 中 `args_dumped : no` | CANN 异常信息中 args 缺失，检查 CANN 版本 |
| dump 目录没有 `_args.bin` | `XPUOPS_DUMP_LEVEL` 为 0（仅公共信息）；设为 1 开启 args 落盘 |
| 异常调用后没有 `output_*` 产物 | 正常行为：kernel 异常时输出未写回，输出数据不可得（见 1.6.3 节）；输出 tensor 的指针现场在 `_args.bin` 中，正常调用的输出基准需在正常路径 dump |
| `input_*` 中出现 `metadata[0]` 之类名字 | 调用边界自动扫描对 tuple/dict 嵌套参数的自动命名；如需自定义名称用 `dump_tensors({"别名": tensor}, stage="input")` |
| 如何离线读回 `input_*` / `output_*` 的 tensor | 见 1.6.2 节读回示例（`.json` 元信息 + `torch.frombuffer` 字节视角还原，无需 NPU 环境） |
| Python 报 `FileNotFoundError: libxpuops_exception_dump.so not found` | 绑定模块按 `XPUOPS_DUMP_LIB_DIR` 环境变量 → 工程内 `lib/` 的顺序查找 so。设置 `XPUOPS_DUMP_LIB_DIR` 指向 so 所在目录（仅异常 dump 需要加载 so；tensor dump 系纯 Python 实现，不受此影响） |
| 找不到 `enable_exception_dump` | demo 脚本已将 `xpuops_exception_dump/python` 加入 `sys.path`；自行接入时将该目录加入 `PYTHONPATH` 或 `sys.path` |
| 想更换 dump 目录/级别 | 设置环境变量 `XPUOPS_DUMP_DIR` / `XPUOPS_DUMP_LEVEL`，见 1.5 节 |
| demo 运行后多出 `extra-info/`、`__pycache__/` 目录 | CANN Runtime 与 Python 解释器的运行时副产物，与本工具无关，可安全删除 |
| 需要在另一台服务器上完整验证本包能力 | 按 `docs/VERIFICATION_GUIDE.md` 执行：环境前置检查 → demo 主链路 → 产物核对 → 配置项/so 解耦分项验证，预期结论与本文档逐条对齐 |

---

## 附录：源码文件说明

| 文件 | 说明 |
|---|---|
| `include/xpuops_exception_dump.h` | 外挂库公开 C 接口 |
| `src/init.cpp` | 初始化入口 `xpuopsExceptionDumpInit`（幂等） |
| `src/exception_callback.cpp` / `.h` | 通用异常回调：公共信息提取、前缀过滤、args D2H、落盘（注册接口运行期自适应） |
| `src/utils.cpp` / `.h` | 日志、时间戳、环境变量、文件读写工具 |
| `python/xpuops/debug.py` | 对外接口：`enable_exception_dump`（异常自动 dump）与 `dump_tensor` / `dump_tensors` / `run_with_tensor_dump`（输入/输出 tensor dump，纯 Python） |
| `python/xpuops/_binding.py` | ctypes 加载 so 与参数转换（仅异常 dump 链路使用） |
| `python/xpuops/__init__.py` | 包入口 |

