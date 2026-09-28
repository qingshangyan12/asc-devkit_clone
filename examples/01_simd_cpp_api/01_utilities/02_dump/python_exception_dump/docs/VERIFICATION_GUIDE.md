# 服务器实机验证手册（VERIFICATION GUIDE）

> **用途**：在另一台服务器环境上实机复现验证本包交付的代码与文档（NPU 算子异常自动 dump + 输入/输出 tensor dump），确认改动可复现。
> **用法**：按章节顺序执行，每步给出可直接复制的命令与预期结果；全部通过即复现成功。任何一步的实际结果与预期不符，请记录（步骤号、命令、实际输出）并反馈，用于修正代码或文档。
> **对齐关系**：本文所有预期结论均出自 `README.md`（对应章节随文标注）。README 描述不清晰或有误之处，同样欢迎在验证后反馈修正。
> **路径约定**：假定包解压在 `exception_dump_package/`，以下命令均在包根目录执行（容器内 bash）。

---

## 1. 验证范围与结论对照表

| # | 验证项 | 执行步骤 | 预期结论（对齐 README） |
|---|---|---|---|
| V1 | 包完整性、Python 源码语法、算子库依赖 | 第 2 章 | 文件齐全，`py_compile` 全部通过，`ascend_ops` 可导入且 IFA 算子已注册（README 1.2 / RELEASE_NOTES 第 1 章） |
| V2 | 外挂库编译链路可复现（可选） | 第 3 章 | `build.sh` 产出 `lib/libxpuops_exception_dump.so`（README 1.3） |
| V3 | 异常自动 dump（C++ 回调链路） | 第 4 章 | `_args.bin` 1264 B + `_info.txt`，脚本校验全过、退出码 0（README 1.4 验证结果） |
| V4 | 输入 tensor dump | 第 4 章 | 正常 / 污染两次调用各产出成对 `input_*.bin/.json`（README 1.6） |
| V5 | 输出 tensor dump | 第 4 章 | 正常调用产出 `output_*.bin/.json`；异常调用不产出（README 1.6.3） |
| V6 | 污染输入现场保全 | 第 4 章 | 污染批次 block_table 首 int32 == `0x0FFFFFF0`（README 1.6.3） |
| V7 | 产物元信息与离线读回 | 第 5 章 | `.json` 字段、大小一致，离线读回 shape/dtype/数值正常（README 1.6.2） |
| V8 | data_ptr 与 `_args.bin` 交叉验证（可选） | 第 6 章 | 异常批次 input 的 `data_ptr` 在 args 中命中（README 1.1.1 / 1.6.2） |
| V9 | tensor dump 不依赖外挂库 so | 第 7 章 | so 缺失时 `dump_tensor` 可用、`enable_exception_dump` 明确报错（README 2.3 FAQ） |
| V10 | dump 级别 / 前缀过滤 / 幂等 / 目录配置 | 第 8 章 | 与 README 1.1 / 1.5 / 2.3 描述的行为一致 |

---

## 2. 环境准备与前置检查（V1）

### 2.1 环境要求（与 README 1.2 一致）

| 项目 | 版本 |
|---|---|
| 开发容器 | ops_dev_sw |
| CANN | cann-9.1.T103（9.2.0+ 亦可，注册接口运行期自适应，见 README 1.1） |
| PyTorch / torch_npu | 2.7.1 / 2.7.1 |
| 算子库 | ascend_ops（pip 安装，含 IFA 算子） |
| 硬件 | Ascend 910（910_93） |

```bash
docker exec -it ops_dev_sw bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh
cd exception_dump_package
```

### 2.2 包完整性检查

```bash
find . -type f | sort
```

预期输出（共 18 个文件，时间戳/`__pycache__` 等副产物不算）：

```text
./README.md
./docs/RELEASE_NOTES.md
./docs/VERIFICATION_GUIDE.md
./examples/demo_ifa.py
./output_samples/ifa/incre_flash_attention_dev0_task3_20260924_103307_467_args.bin
./output_samples/ifa/incre_flash_attention_dev0_task3_20260924_103307_467_info.txt
./xpuops_exception_dump/CMakeLists.txt
./xpuops_exception_dump/build.sh
./xpuops_exception_dump/include/xpuops_exception_dump.h
./xpuops_exception_dump/lib/libxpuops_exception_dump.so
./xpuops_exception_dump/python/xpuops/__init__.py
./xpuops_exception_dump/python/xpuops/_binding.py
./xpuops_exception_dump/python/xpuops/debug.py
./xpuops_exception_dump/src/exception_callback.cpp
./xpuops_exception_dump/src/exception_callback.h
./xpuops_exception_dump/src/init.cpp
./xpuops_exception_dump/src/utils.cpp
./xpuops_exception_dump/src/utils.h
```

### 2.3 Python 源码语法自检

```bash
python3 -m py_compile \
    xpuops_exception_dump/python/xpuops/__init__.py \
    xpuops_exception_dump/python/xpuops/debug.py \
    xpuops_exception_dump/python/xpuops/_binding.py \
    examples/demo_ifa.py \
    && echo "[OK] py_compile passed"
```

预期：无报错，末行输出 `[OK] py_compile passed`。

### 2.4 算子库依赖检查（ascend_ops）

demo 经 pip 包 `ascend_ops` 调用 IFA 算子：`import ascend_ops` 触发 torch 算子注册（副作用导入）。**无需下载 ops-transformer 源码仓、无需单算子编译**——该仓仅为 README 1.1.1 标注的算子源码出处，非运行依赖；本包自身可能需要编译的只有外挂库 so（第 3 章，可选）。

```bash
python3 -c "import ascend_ops, torch; torch.ops.ascend_ops.npu_fused_infer_attention_score; print('[OK] ascend_ops ready')"
```

预期输出 `[OK] ascend_ops ready`。

若报 `ModuleNotFoundError`（未安装）或 `AttributeError`（已装但未注册成功）：

- 优先从内部 pip 源安装：`pip3 install ascend_ops`（版本须与运行环境 CANN / 芯片匹配，见 2.1 环境表）；
- 拿不到 pip 包时按 2.4.1 节从 ops-transformer 源码仓自行构建（`ascend_ops` wheel 即产自该仓 `examples/fast_kernel_launch_example`，仓内有标准构建命令），属目标环境准备问题，与本包工具无关；
- 版本不匹配的典型表现：import 报 undefined symbol、算子调用报错，或 `args.bin` 大小偏离 1264 B（见第 10 章 FAQ）。

#### 2.4.1 源码编译 ascend_ops（兜底路径）

`ascend_ops` pip 包的开源出处即 ops-transformer 仓（`https://gitcode.com/cann/ops-transformer`，atomgit 有同名镜像），wheel 由仓内 `examples/fast_kernel_launch_example` 目录构建（基于 PyTorch Extension，IFA 算子源码在其 `csrc/incre_flash_attention/ascend910_93/op_kernel/`）。**建议直接在 demo 运行容器内构建**——wheel 链接当前环境的 torch / torch_npu / CANN 符号，跨环境拷贝需保证三者版本一致。

```bash
# 0) 前置：CANN 已 source（2.1 节）；gcc >= 9.4、python >= 3.8、torch >= 2.6.0 及配套 torch_npu
#    （demo 运行环境本身即满足）
# 1) 下载与 CANN 版本配套的源码分支（配套关系见 cann/release-management 仓）
git clone -b ${tag_version} https://gitcode.com/cann/ops-transformer.git && cd ops-transformer
#    cann-9.1 环境选 9.1.x 分支；不确定时先 clone 后 git tag -l 查看可选标签。
#    官方明确提示：master 分支存在与 CANN 版本不匹配的风险

# 2) 进入 fast_kernel_launch_example（ascend_ops wheel 构建目录）
cd examples/fast_kernel_launch_example

# 3) 安装构建依赖
python3 -m pip install -r requirements.txt

# 4) 指定芯片型号（demo 为 910_93 / Atlas A3；该变量缺省 ascend910b，不设置会编译错款型）
export NPU_SOC_VERSION=ascend910_93

# 5) 构建 wheel（产物在 dist/，名称形如 ascend_ops-1.0.0-cp3x-abi3-{arch}.whl）
python3 -m build --wheel -n

# 6) 安装（覆盖环境中已有版本）
python3 -m pip install dist/*.whl --force-reinstall --no-deps

# 7) 回到 2.4 开头的检查命令验证
python3 -c "import ascend_ops, torch; torch.ops.ascend_ops.npu_fused_infer_attention_score; print('[OK] ascend_ops ready')"
```

注意：

- 重复构建前先清理编译缓存：`python setup.py clean`；
- 源码分支与 CANN 版本严格配套（仓 README"版本配套"章节）。若所选分支与包内历史产物（`output_samples/`，20260924 生成）的源码版本不同，IFA kernel args 布局可能变化，表现为 demo `args.bin` 大小偏离 1264 B——按第 10 章 FAQ 记录实际值反馈即可，不影响 dump 链路本身的验证；
- 编译报找不到 `ASCEND_HOME_PATH` 等环境变量时，回到 2.1 节确认 `set_env.sh` 已 source。

---

## 3. 编译外挂库（V2，可选）

目的：验证"源码 + build.sh"构建链路在新环境可复现。运行环境与包内已附 so 编译环境一致时，可跳过本步直接使用包内 so（README 1.3）。

```bash
cd xpuops_exception_dump
bash build.sh
```

预期：cmake + make 正常结束，末行输出：

```text
build output: /<包路径>/exception_dump_package/xpuops_exception_dump/lib/libxpuops_exception_dump.so
```

> 注意（README 1.3）：`build.sh` 会**覆盖** `lib/` 下已附带的 so。若编译环境 CANN 版本与运行环境不一致，验证 demo 前请确认使用的 so 与运行环境匹配。

---

## 4. 主验证：运行 demo（V3 ~ V6）

### 4.1 运行

```bash
cd exception_dump_package
python3 examples/demo_ifa.py
echo "exit code: $?"
```

### 4.2 终端输出预期（按出现顺序）

**（a）正常调用阶段**：

```text
[XPUOPS][INFO] exception dump callback registered via aclrtSetExceptionInfoCallback
```

> 注册接口名因 CANN 版本而异：cann-9.1 打印 `aclrtSetExceptionInfoCallback`，9.2.0+ 打印 `aclrtExceptionInfoCallbackRegister`，均属正常（README 1.1）。

```text
===== step1: normal run (dump input & output tensors) =====
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_query' (4096 bytes) -> .../exception_dump_ifa/input_npu_fused_infer_attention_score_query_dev0_<ts>.bin
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_key' (163840 bytes) -> ...
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_value' (163840 bytes) -> ...
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_actual_seq_kvlen' (16 bytes) -> ...
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_block_table' (40 bytes) -> ...
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_dequant_scale_key' (256 bytes) -> ...
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_dequant_scale_value' (256 bytes) -> ...
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_metadata[0]' (... bytes) -> ...
...（metadata 为序列时逐项展开，项数因算子版本而异；标量参数不 dump）
[XPUOPS][INFO] dumped output tensor 'npu_fused_infer_attention_score_ret[0]' (4096 bytes) -> ...
[XPUOPS][INFO] dumped output tensor 'npu_fused_infer_attention_score_ret[1]' (... bytes) -> ...
normal run OK, output shape: (2, 8, 1, 128)
```

**（b）故障注入阶段**：

```text
===== step2: trigger exception (poisoned block_table) =====
[XPUOPS][INFO] dumped input tensor 'npu_fused_infer_attention_score_query' (4096 bytes) -> ...（污染批次，时间戳晚于 (a) 批次）
...（污染批次的全部输入逐个 dump，大小与 (a) 批次相同）
caught expected RuntimeError: ...（含 507015 / EE1001 等错误码字样，文案因 CANN 版本而异）
[XPUOPS][INFO] ========== XPU Exception Dump ==========
[XPUOPS][INFO] Timestamp:    <ts>
[XPUOPS][INFO] Device ID:    0
[XPUOPS][INFO] Task ID:      <n>
[XPUOPS][INFO] Stream ID:    <n>
[XPUOPS][INFO] Thread ID:    <n>
[XPUOPS][INFO] Error Code:   0x7bc87
[XPUOPS][INFO] Kernel Name:  incre_flash_attention
[XPUOPS][INFO] Raw Kernel:   _Z21incre_flash_attentionILh0ELh1ELh0EE...（修饰名）
[XPUOPS][INFO] Args Size:    1264 bytes
[XPUOPS][INFO] =========================================
[XPUOPS][INFO] dumped raw kernel args (1264 bytes) -> .../exception_dump_ifa/incre_flash_attention_dev0_task<n>_<ts>_args.bin
[XPUOPS][INFO] args hexdump (total 1264 bytes, show 256):
...（十六进制行，前 256 B 为 32 个 GM 指针）
```

> 异常回调日志与 `caught expected RuntimeError` 的先后顺序可能因上报时机不同而互换，属正常现象。
> Error Code `0x7bc87`（507015，核内 GM 越界）为 cann-9.1.T103 + 910_93 的实测值；不同 CANN 版本 / 芯片可能打印不同错误码。判定原则：**kernel_name 命中 `incre_flash_attention`、args 正常落盘且大小 1264 B，即主链路通过**；错误码不同请记录实际值反馈（本文 5.1 同）。

**（c）产物校验阶段**（README 1.4 验证结果逐条对齐）：

```text
===== step3: check dump products =====
  <全部产物按文件名排序逐行列出：文件名 (字节数)>
[SIZE-OK] args.bin: 1264 bytes == expected 1264
[INFO-OK] info.txt contains 'kernel_name : incre_flash_attention'
[INFO-OK] info.txt contains 'error_code  : 0x7bc87'
[INFO-OK] info.txt contains 'args_dumped : yes'
[INPUT-OK] input tensor dump: <N> files
[OUTPUT-OK] output tensor dump: <M> files
[SIZE-OK] input query tensor: 4096 bytes == expected 4096
[DATA-OK] poisoned block_table dump detected: input_npu_fused_infer_attention_score_block_table_dev0_<ts>.bin (first int32 == 0x0FFFFFF0)
[PASS] IFA exception dump OK
```

**退出码**：`exit code: 0`。出现任何 `[FAIL]` 行或退出码非 0 即该项不通过，记录输出反馈。

### 4.3 dump 产物清单预期

产物目录为包根下的 `./exception_dump_ifa/`（demo 内部固定，README 1.4；可自行删除后重跑）。

| 类别 | 命名模式 | 数量 | 大小 | 来源 |
|---|---|---|---|---|
| 异常公共信息 | `incre_flash_attention_dev{dev}_task{task}_{ts}_info.txt` | 1 | 约 0.5 KB | C++ 回调 |
| 异常原始 args | 同前缀 `_args.bin` | 1 | **1264 B** | C++ 回调 |
| 正常批次输入 | `input_npu_fused_infer_attention_score_{name}_dev0_{ts}.bin` + 同名 `.json` | 每个输入 2 个文件，`name` ∈ `query` / `key` / `value` / `actual_seq_kvlen` / `block_table` / `dequant_scale_key` / `dequant_scale_value` / `metadata_0...` | query 4096 B / key、value 163840 B / block_table 40 B 等 | Python 调用边界 |
| 污染批次输入 | 同上（时间戳晚于正常批次，大小相同） | 同上 | 同上 | Python 调用边界 |
| 正常批次输出 | `output_npu_fused_infer_attention_score_ret_0_dev0_{ts}.bin/.json`、`ret_1` 同理 | 每个输出 2 个文件 | ret_0（attention out）4096 B | Python 正常返回后 |

> 文件名中的 `ret[0]`、`metadata[0]` 等含 `[` `]` 的名字已安全化为 `ret_0`、`metadata_0`（`.json` 内 `name` 字段保留原始名，README 1.6.2）。
> 判定正常批次 / 污染批次：按文件名中的时间戳排序，早者为正常批次、晚者为污染批次（demo 先正常后故障注入）。
> 污染批次**没有** `output_*` 产物属预期行为（kernel 异常时输出未写回，README 1.6.3 / FAQ）。

---

## 5. 产物内容核对（V7）

### 5.1 `_info.txt` 核对（对照 `output_samples/ifa/` 历史样例）

```text
===== XPU Exception Dump =====
timestamp   : <ts>                 <- 因运行时间而异
device_id   : 0
task_id     : <n>                  <- 因运行而异
stream_id   : <n>                  <- 因运行而异
thread_id   : <n>                  <- 因运行而异
error_code  : 0x7bc87              <- 关键校验项（不同 CANN 版本可能不同，见 4.2 注）
kernel_name : incre_flash_attention    <- 关键校验项，须与实际异常日志一致
raw_kernel  : _Z21incre_flash_attentionILh0ELh1ELh0EE...（修饰名）
args_bytes  : 1264                 <- 关键校验项
args_dumped : yes                  <- 关键校验项
dump_level  : 1
```

### 5.2 `.json` 元信息核对（以正常批次 query 为例）

任选一个 `input_npu_fused_infer_attention_score_query_dev0_*.json`，内容应形如：

```json
{
  "name": "npu_fused_infer_attention_score_query",
  "stage": "input",
  "shape": [2, 8, 1, 128],
  "dtype": "torch.bfloat16",
  "itemsize": 2,
  "numel": 2048,
  "nbytes": 4096,
  "device": "npu:0",
  "data_ptr": "0x<十六进制 device 地址>",
  "bin_file": "input_npu_fused_infer_attention_score_query_dev0_<ts>.bin",
  "timestamp": "<ts>"
}
```

核对点：字段齐全（README 1.6.2 字段表）；`bin_file` 与 `.json` 同目录同名；`nbytes` 与 `.bin` 实际大小一致；`stage` 为 `input` / `output`。注意 json 中**没有**独立的 `tag` 字段（tag 已融入文件名与 `name` 前缀）。

### 5.3 离线读回验证（README 1.6.2 读回方式，无 NPU 环境亦可执行）

```bash
python3 - <<'EOF'
import json, glob, os
import torch

dump_dir = "./exception_dump_ifa"

# 1) 正常批次（时间戳最早）的 query 读回
qjson = sorted(glob.glob(os.path.join(dump_dir, "input_npu_fused_infer_attention_score_query_dev*.json")))[0]
meta = json.load(open(qjson, encoding="utf-8"))
print("query meta:", {k: meta[k] for k in ("name", "stage", "shape", "dtype", "itemsize", "numel", "nbytes", "device")})
assert meta["shape"] == [2, 8, 1, 128] and meta["dtype"] == "torch.bfloat16" and meta["nbytes"] == 4096

buf = open(os.path.join(dump_dir, meta["bin_file"]), "rb").read()
assert len(buf) == meta["nbytes"], "bin size != meta.nbytes"
t = torch.frombuffer(bytearray(buf), dtype=torch.uint8).view(torch.bfloat16).reshape(meta["shape"])
print("query readback:", tuple(t.shape), t.dtype, "first values:", t.flatten()[:4].tolist())

# 2) 正常批次 attention 输出读回（数值应为有限值，可作对拍基准）
ojson = sorted(glob.glob(os.path.join(dump_dir, "output_npu_fused_infer_attention_score_ret_0_dev*.json")))[0]
ometa = json.load(open(ojson, encoding="utf-8"))
obuf = open(os.path.join(dump_dir, ometa["bin_file"]), "rb").read()
out = torch.frombuffer(bytearray(obuf), dtype=torch.uint8).view(torch.bfloat16).reshape(ometa["shape"])
print("attention out readback:", tuple(out.shape), out.dtype,
      "finite:", bool(torch.isfinite(out.float()).all()))

print("[OK] offline readback verified")
EOF
```

预期输出：

```text
query meta: {'name': 'npu_fused_infer_attention_score_query', 'stage': 'input', 'shape': [2, 8, 1, 128], 'dtype': 'torch.bfloat16', 'itemsize': 2, 'numel': 2048, 'nbytes': 4096, 'device': 'npu:0'}
query readback: (2, 8, 1, 128) torch.bfloat16 first values: [...]
attention out readback: (2, 8, 1, 128) torch.bfloat16 finite: True
[OK] offline readback verified
```

---

## 6. 交叉验证：`data_ptr` 与 `_args.bin`（V8，可选进阶）

原理（README 1.1.1 / 1.6.2）：异常批次 input `.json` 中的 `data_ptr` 是下发前记录的 device 地址，应能在异常 `_args.bin` 的 GM 指针区（前 32 个 8 字节指针）中找到，证明"异常 kernel 即本次调用"。

```bash
python3 - <<'EOF'
import json, glob, os, struct

dump_dir = "./exception_dump_ifa"
args_path = sorted(glob.glob(os.path.join(dump_dir, "incre_flash_attention_*_args.bin")))[0]
args_buf = open(args_path, "rb").read()
print("args.bin:", os.path.basename(args_path), len(args_buf), "bytes")

hit = 0
for jf in sorted(glob.glob(os.path.join(dump_dir, "input_*_dev*.json"))):
    meta = json.load(open(jf, encoding="utf-8"))
    ptr = int(meta["data_ptr"], 16)
    if ptr and struct.pack("<Q", ptr) in args_buf:
        hit += 1
        print("[HIT]", os.path.basename(jf), "data_ptr", meta["data_ptr"])
print("cross-check: hit %d input(s)" % hit)
EOF
```

预期：

- 污染批次（时间戳较晚）的 input `.json` **全部或绝大多数命中**——其 tensor 即异常 kernel 的实际入参；
- 正常批次不命中属**预期行为**（args 记录的是异常那次调用）；
- 判定线：`hit >= 1` 即机制成立。若**全部**未命中，请记录现象反馈（可能与算子内部 buffer 转换或 metadata 布局有关，不影响 V3~V7 主链路结论）。

---

## 7. 分项验证：tensor dump 不依赖外挂库 so（V9）

依据（README 1.6 / FAQ）：tensor dump 为纯 Python 实现；so 在首次调用 `enable_exception_dump` 时才懒加载。

**场景 A：仅用 tensor dump（不触发 so 加载）**

```bash
python3 - <<'EOF'
import os, sys
sys.path.insert(0, os.path.abspath("xpuops_exception_dump/python"))
import torch, torch_npu
torch.npu.set_device(0)
from xpuops.debug import dump_tensor          # import 阶段不加载 so
t = torch.arange(8, dtype=torch.int32).npu()
p = dump_tensor(t, "so_free_check", stage="input", dump_dir="./tmp_so_free_check")
print("[OK] tensor dump works without loading so:", p)
EOF
```

预期：输出 `[OK] tensor dump works without loading so: ./tmp_so_free_check/so_free_check_input_dev0_<ts>.bin`。

**场景 B：so 缺失时的负向验证（验证后务必恢复 so）**

```bash
mv xpuops_exception_dump/lib/libxpuops_exception_dump.so /tmp/
python3 - <<'EOF'
import os, sys
sys.path.insert(0, os.path.abspath("xpuops_exception_dump/python"))
import torch, torch_npu
torch.npu.set_device(0)
from xpuops.debug import enable_exception_dump, dump_tensor
try:
    enable_exception_dump()
    print("[FAIL] should have raised FileNotFoundError")
except FileNotFoundError as e:
    print("[OK] enable_exception_dump fails clearly:", e)
p = dump_tensor(torch.arange(8, dtype=torch.int32).npu(), "so_free_check2",
                stage="input", dump_dir="./tmp_so_free_check")
print("[OK] tensor dump still works:", p)
EOF
mv /tmp/libxpuops_exception_dump.so xpuops_exception_dump/lib/    # 立即恢复
ls xpuops_exception_dump/lib/                                      # 确认 so 已恢复
```

预期：`enable_exception_dump` 抛 `FileNotFoundError`（错误信息含 `set XPUOPS_DUMP_LIB_DIR` 提示），**同一进程内** `dump_tensor` 正常产出，两类能力解耦成立；末条 `ls` 确认 so 已放回。

---

## 8. 分项验证：配置与过滤行为（V10）

demo 内部固定了输出目录与级别（`XPUOPS_DUMP_DIR=./exception_dump_ifa`、`XPUOPS_DUMP_LEVEL=1`），因此以下用脚本复用 demo 的输入构造与故障注入逻辑，验证环境变量的通用行为（README 1.5 / 1.1 / 2.3）。

**8.1 前缀过滤 skip + 注册幂等**（预期：不匹配的 kernel 只打 skip 日志、不落盘；重复注册无副作用）

```bash
python3 - <<'EOF'
import os, sys, glob
sys.path.insert(0, os.path.abspath("examples"))
import demo_ifa                                 # 复用输入构造与算子调用（顶层已配置 dump 目录）
import torch
from xpuops.debug import enable_exception_dump

dump_dir = os.path.abspath("./exception_dump_cfg_check")
os.environ["XPUOPS_DUMP_DIR"] = dump_dir       # 覆盖为独立目录，便于核对
os.environ["XPUOPS_DUMP_LEVEL"] = "1"

enable_exception_dump(["prefix_not_match_demo"])   # 不匹配的前缀
enable_exception_dump(["prefix_not_match_demo"])   # 重复调用：幂等，无重复注册日志
try:
    torch.ops.ascend_ops.npu_fused_infer_attention_score(**demo_ifa.gen_inputs(poison_block_table=True))
    torch.npu.synchronize()
except RuntimeError as e:
    print("caught:", str(e)[:120])
files = sorted(os.path.basename(f) for f in glob.glob(dump_dir + "/*"))
print("dump dir files:", files)
assert not files, "[FAIL] expected no dump files for unmatched prefix"
print("[OK] unmatched prefix -> skip dump, no files")
EOF
```

预期：终端出现一条 `[XPUOPS][INFO] kernel 'incre_flash_attention' not in enabled prefix list, skip dump`；`exception dump callback registered via ...` 仅出现**一次**（幂等）；dump 目录为空，末行 `[OK] unmatched prefix -> skip dump, no files`。

**8.2 `XPUOPS_DUMP_LEVEL=0`（仅公共信息，README 1.5）**

```bash
python3 - <<'EOF'
import os, sys, glob
sys.path.insert(0, os.path.abspath("examples"))
import demo_ifa
import torch
from xpuops.debug import enable_exception_dump

dump_dir = os.path.abspath("./exception_dump_cfg_check_level0")
os.environ["XPUOPS_DUMP_DIR"] = dump_dir
os.environ["XPUOPS_DUMP_LEVEL"] = "0"          # 仅公共信息：不落 args.bin

enable_exception_dump(["incre_flash_attention"])
try:
    torch.ops.ascend_ops.npu_fused_infer_attention_score(**demo_ifa.gen_inputs(poison_block_table=True))
    torch.npu.synchronize()
except RuntimeError as e:
    print("caught:", str(e)[:120])
files = sorted(os.path.basename(f) for f in glob.glob(dump_dir + "/*"))
print("dump dir files:", files)
args = [f for f in files if f.endswith("_args.bin")]
infos = [f for f in files if f.endswith("_info.txt")]
assert not args and len(infos) == 1, "[FAIL] level=0 should produce info.txt only"
info = open(os.path.join(dump_dir, infos[0])).read()
assert "dump_level  : 0" in info and "args_dumped : yes" in info
print("[OK] level=0 -> info.txt only (no args.bin)")
EOF
```

预期：仅产出 1 个 `_info.txt`（其中 `dump_level : 0`），**无** `_args.bin`；末行 `[OK] level=0 -> info.txt only (no args.bin)`。

> 说明：level=0 时回调仍完成 args D2H 与公共信息记录（`args_bytes` / `args_dumped` 照常写入 info），仅跳过 args 落盘（README 1.5"0=仅公共信息"）。

**8.3 `XPUOPS_DUMP_DIR` 与 `XPUOPS_DUMP_LIB_DIR`**

```bash
export XPUOPS_DUMP_DIR=/tmp/xpuops_envdir_check
python3 - <<'EOF'
import os, sys
sys.path.insert(0, os.path.abspath("xpuops_exception_dump/python"))
import torch, torch_npu
torch.npu.set_device(0)
from xpuops.debug import dump_tensor
p = dump_tensor(torch.arange(16, dtype=torch.int32).npu(), "envdir_check", stage="input")
assert "/tmp/xpuops_envdir_check" in p, "[FAIL] XPUOPS_DUMP_DIR not respected"
print("[OK] XPUOPS_DUMP_DIR respected:", p)
EOF
ls /tmp/xpuops_envdir_check
unset XPUOPS_DUMP_DIR
```

预期：产物落在 `/tmp/xpuops_envdir_check`（1 对 `.bin/.json`，README 1.5 目录决策链）；tensor dump 目录决策与异常 dump 共用同一环境变量。

`XPUOPS_DUMP_LIB_DIR` 的查找行为已由第 7 章场景 B 顺带覆盖（找不到 so 时报错信息提示设置该变量）；so 放在包内 `lib/` 之外时，`export XPUOPS_DUMP_LIB_DIR=<so 所在目录>` 即可，无需改动代码。

---

## 9. 验证结论记录模板（逐项填写后反馈）

```text
验证环境：
- 机型 / NPU：____
- CANN 版本：____
- torch / torch_npu：____
- ascend_ops：____

验证结论：
- V1  包完整性 + 语法自检 + 算子库（ascend_ops）：通过 / 不通过（差异：____）
- V2  编译链路（可选）：通过 / 跳过 / 不通过（____）
- V3  异常自动 dump：通过 / 不通过（实际 error_code=____，args 大小=____）
- V4  输入 tensor dump：通过（共 __ 个 input .bin）/ 不通过
- V5  输出 tensor dump：通过（共 __ 个 output .bin，异常批次无 output 属预期）/ 不通过
- V6  污染现场保全：通过 / 不通过
- V7  元信息与离线读回：通过 / 不通过
- V8  交叉验证（可选）：命中 __ / 全部 __（或跳过）
- V9  tensor dump 与 so 解耦：场景 A 通过 / 场景 B 通过（so 已恢复：是/否）
- V10 配置项：8.1 通过 / 8.2 通过 / 8.3 通过

README 不清晰或有误之处（附步骤号与实际现象）：
1. ____
2. ____
```

---

## 10. 常见问题排查（验证场景）

| 现象 | 处理（详见 README 2.3 FAQ） |
|---|---|
| `python3` 报找不到 `xpuops` / `enable_exception_dump` | 确认在包根执行；demo 已自动加 `sys.path`，自行脚本需 `sys.path` / `PYTHONPATH` 指向 `xpuops_exception_dump/python` |
| `FileNotFoundError: libxpuops_exception_dump.so not found` | 仅异常 dump 需要 so：确认 `lib/` 下有 so，或 `export XPUOPS_DUMP_LIB_DIR`；tensor dump 不受影响 |
| 算子调用报 `torch.ops.ascend_ops` 不存在 | `ascend_ops` 未安装 / 未注册，确认 pip 包与运行环境一致 |
| 异常未触发、demo 打印 `[WARN] no exception raised?!` | 污染 block_table 未生效或该版本 kernel 对非法索引容忍；记录 CANN/芯片信息反馈 |
| error code 不是 `0x7bc87` | 不同 CANN 版本上报值可能不同；按 4.2 判定原则处理并记录实际值 |
| `args.bin` 大小不是 1264 B | 算子版本（ascend_ops）不同导致 args 布局变化；记录实际大小与版本反馈 |
| demo 目录出现 `extra-info/`、`__pycache__/` | CANN / Python 运行副产物，可安全删除 |
