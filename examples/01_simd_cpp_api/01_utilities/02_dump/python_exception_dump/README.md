# NPU 算子异常信息自动 Dump

> NPU kernel 运行时异常发生时，自动 dump 问题算子的异常上下文与 kernel 参数（args），用于离线问题定位（可按算子 args 布局还原输入输出 tensor）；另提供纯 Python 的输入/输出 tensor dump，可与异常产物中的 device 指针交叉验证。外挂库方案，算子源码零改动。
>
> English version: [README_en.md](README_en.md)

## 1. 能力一览

| 能力 | 开启方式 | 产物 |
|---|---|---|
| 异常自动 dump（C++ 外挂库回调） | `enable_exception_dump(["kernel前缀"])` | `{kernel}_dev{id}_task{id}_{ts}_info.txt`（异常公共信息）+ 同前缀 `_args.bin`（原始 kernel args：各输入/输出 tensor 的 device 指针 + tiling，结合算子 args 布局可离线还原） |
| 输入/输出 tensor dump（纯 Python，不依赖 so、无需编译） | `run_with_tensor_dump(op, tag=...)`（调用边界自动）或 `dump_tensors(..., stage=...)`（手动） | `input_{tag}_{name}_dev{idx}_{ts}.bin/.json`（数据 + 元信息；json 中 `data_ptr` 与 `_args.bin` 对应指针一致，可交叉验证）；`output_*` 同理，仅正常返回后产出 |

要点：kernel name 前缀过滤（不匹配仅打一条 skip 日志）；重复开启幂等；回调注册接口按 CANN 版本运行期自适应（9.2.0 前后接口不同，同一份 so 均可用）；kernel 异常时输出 tensor 未写回，`output_*` 仅在算子正常返回后产出，输出指针现场由 `_args.bin` 提供。

配置（环境变量）：`XPUOPS_DUMP_DIR`（dump 目录，两类能力共用，默认 `./exception_dump/`）｜`XPUOPS_DUMP_LEVEL`（0=仅公共信息，1=公共信息+args，默认 1，仅影响异常 dump）｜`XPUOPS_DUMP_LIB_DIR`（so 所在目录，默认工程内 `lib/`）。

## 2. 快速开始（复现 demo）

环境要求：Atlas A2 训练系列产品 / Atlas A2 推理系列产品 ｜ 已安装并完成环境配置的 CANN（source `set_env.sh` 后生效） ｜ 与所装 CANN 版本匹配的 PyTorch / torch_npu ｜ pip 包 `ascend_ops`（demo 调用的 IFA 算子所在包）。

```bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh     # CANN 环境
cd xpuops_exception_dump && bash build.sh && cd ..      # 编译外挂库 so（产物输出到 lib/，必选）
python3 examples/demo_ifa.py
```

demo 流程：正常调用 IFA 算子（调用边界自动 dump 输入/输出）→ 故障注入污染 block_table 触发 AICore kernel 异常 → 外挂库回调自动 dump 异常信息与 args → 脚本自检产物，全部通过时退出码 0。关键预期输出：

```text
[SIZE-OK] args.bin: 1264 bytes == expected 1264
[INFO-OK] kernel_name / error_code / args_dumped（3 条）
[INPUT-OK] / [OUTPUT-OK] / query 大小 / 污染 block_table 读回
[PASS] IFA exception dump OK
```

产物输出至包根 `./exception_dump_ifa/`。错误码具体数值与 CANN 版本相关（不同版本可能不同，判定以 kernel_name 命中 + args 落盘为准）。外挂库 so 为环境相关二进制、不入库，运行前须先编译（见上文命令；tensor dump 为纯 Python、无此要求）。

## 3. 接入自己的算子

```python
from xpuops.debug import enable_exception_dump, run_with_tensor_dump, dump_tensors

# 1) 开启异常 dump：前缀取 demangle 后的 kernel 函数名（模板参数已去掉），
#    不确定时以异常日志 Kernel Name 打印值为准；不传参则对全部 kernel 生效；幂等
enable_exception_dump(["my_kernel"])

# 2)（可选）自动 dump 输入/输出 tensor：异常时输入已落盘、输出不可得（kernel 未写回）
out = run_with_tensor_dump(torch.ops.my_lib.my_op, tag="my_op", *args, **kwargs)

# 3)（可选）手动 dump 指定 tensor
dump_tensors({"query": q, "block_table": bt}, stage="input")
```

无需修改任何 C++ 代码、无需重编业务工程；仅异常 dump 需加载 so，tensor dump 为纯 Python。验证：触发 kernel 异常后，确认 dump 目录生成 `_args.bin` 与 `_info.txt`（error_code / kernel_name 与 Runtime 日志一致），使用了步骤 2/3 时另确认 `input_*` / `output_*` 产物已生成。

## 4. FAQ

| 现象 | 处理 |
|---|---|
| 日志 `kernel '...' not in enabled prefix list, skip dump` | 按日志实际 kernel 名修正前缀，或不传参（对全部 kernel 生效） |
| dump 目录没有 `_args.bin` | `XPUOPS_DUMP_LEVEL` 被设为 0（仅公共信息），改回 1 |
| 异常调用后没有 `output_*` 产物 | 正常行为：异常时输出未写回；输出基准需在正常路径 dump（见第 1 节要点） |
| `_info.txt` 中 `args_dumped : no` / 日志 `args unavailable` | CANN 异常信息缺 args，检查 CANN 版本 |
| `FileNotFoundError: libxpuops_exception_dump.so not found` | 先编译：`cd xpuops_exception_dump && bash build.sh`（so 二进制不入库）；或设 `XPUOPS_DUMP_LIB_DIR` 指向 so 目录（仅异常 dump 需要 so，tensor dump 不受影响） |
| 找不到 `xpuops` 模块 | 将 `xpuops_exception_dump/python` 加入 `PYTHONPATH`（demo 脚本已自动处理） |
| 离线读回 `input_*` / `output_*` tensor | 任意有 torch 的环境即可（无需 NPU）：按 `.json` 元信息用 `torch.frombuffer` 以字节视角还原 |

## 5. 目录结构

```text
README.md / README_en.md      本文档（中文 / English）
examples/demo_ifa.py          demo：正常调用 + 故障注入 + 产物自检（可直接运行）
xpuops_exception_dump/        外挂库工程：src/（C++ 回调）、python/xpuops/（Python API）、
                              build.sh（产物输出到 lib/，so 需自行编译、二进制不入库）
```
