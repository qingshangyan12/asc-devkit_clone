# 交付版本说明（RELEASE NOTES）

- 交付包：`exception_dump_package/`
- 参考文档：用户指南（`README.md`）

---

## 1. 目录树与文件清单

```
exception_dump_package/
├── README.md                                   对外交付入口文档（用户指南）
├── docs/
│   ├── RELEASE_NOTES.md                        本文件
│   └── VERIFICATION_GUIDE.md                   服务器实机验证手册：V1~V10 验证项、
│                                               步骤与预期结果、结论记录模板
├── xpuops_exception_dump/                      外挂库工程本体
│   ├── include/
│   │   └── xpuops_exception_dump.h             公开 C 接口
│   ├── src/
│   │   ├── init.cpp                            初始化入口（幂等）
│   │   ├── exception_callback.cpp              通用异常回调（单层：公共信息+前缀过滤+args D2H+落盘）
│   │   ├── exception_callback.h
│   │   ├── utils.cpp                           日志/时间戳/环境变量/文件工具
│   │   └── utils.h
│   ├── python/xpuops/
│   │   ├── __init__.py                         包入口
│   │   ├── debug.py                           enable_exception_dump（异常 dump）
│   │   │                                       + dump_tensor / dump_tensors / run_with_tensor_dump
│   │   │                                       （输入/输出 tensor dump，纯 Python，不依赖 so）
│   │   └── _binding.py                         ctypes 绑定（仅异常 dump 链路使用）
│   ├── lib/
│   │   └── libxpuops_exception_dump.so         已编译产物（910_93 + cann-9.1.T103 环境编译，
│   │                                           与当前 src/ 源码一致，demo 验证基于该 so）
│   ├── CMakeLists.txt                          构建脚本
│   └── build.sh                                一键编译（产物输出到 lib/）
├── examples/
│   └── demo_ifa.py                             demo 脚本（AICore kernel 完整链路 + tensor dump）
└── output_samples/
    └── ifa/                                    demo 异常 dump 历史产物样例
                                                （2 个文件，前缀时间戳 20260924_103307_467）
```

版本确认说明：

- 所有源码文件均为**单一版本**，包内无重复副本；
- `lib/libxpuops_exception_dump.so` 为当前 `src/` 源码的编译产物，demo 验证（AICore kernel 异常 dump，args 1264B + info 三项校验）基于该 so 通过。__本次新增的输入/输出 tensor dump 为纯 Python 实现（`python/xpuops/debug.py`），未改动任何 C++ 源码，故 lib/ 下 so 与 src/ 的一致性继续成立，无需重新编译__；
- `output_samples/` 产物由 demo 运行生成（异常 dump 两件套），`_info.txt` 中记录的绝对路径为生成时环境的历史信息。`input_*` / `output_*` tensor dump 产物由 demo 运行时生成于同一 dump 目录（`XPUOPS_DUMP_DIR`），历史样例未包含该类文件。

---

## 2. 关键机制速览

| 机制 | 一句话说明 |
|---|---|
| 通用异常回调（`exception_callback.cpp`） | kernel 异常时 Runtime 自动触发本回调，提取 device/task/stream/error 等公共信息，D2H 拷贝完整 kernel args，统一落盘 `_info.txt` + `_args.bin`——单层设计，无任何算子级解析代码 |
| 输入/输出 tensor dump（`debug.py`，纯 Python） | 在算子调用边界自动 dump 输入/输出 tensor 的数据（`.bin`）与元信息（`.json`）：调用前保全输入现场（含故障注入后的污染输入），正常返回后保全输出（可作对拍基准）；`.json` 中的 `data_ptr` 与 `_args.bin` 中的 GM 指针一致，可交叉验证 |
| kernel name 前缀过滤 | `enable_exception_dump(["my_kernel"])` 指定关心的 kernel（前缀匹配），不匹配的仅打印 skip 日志、不落盘；不传参对全部 kernel 生效 |
| 接入零 C++ 改动 | 注册动作仅在 Python 侧（或 C 接口）完成，新增业务算子无需修改/新增 C++ 代码、无需重新编译外挂库；tensor dump 同样不依赖 so |
| 开启即幂等（`init.cpp`） | `enable_exception_dump()` 重复调用无副作用，异常回调只注册一次 |
| 注册接口运行期自适应 | CANN 9.2.0+ 使用 `aclrtExceptionInfoCallbackRegister`，旧版本（如 9.1）回退 `aclrtSetExceptionInfoCallback`，一份 so 兼容新旧 CANN |
| 适用边界 | 异常自动 dump 仅支持 AICore kernel（异常信息携带 args）；tensor dump 无此限制，Python 侧持有 torch tensor 即可用 |

---

## 3. 本次变更说明（v1.1）

**新增**：Python 模块输入/输出 tensor dump 能力（需求：dump 输入 tensor、dump 输出 tensor）。

- 新增 API（`python/xpuops/debug.py`，`__init__.py` 同步导出）：
  - `run_with_tensor_dump(func, *args, tag=None, dump_inputs=True, dump_outputs=True, dump_dir=None, **kwargs)`：调用边界自动 dump 输入（`input_*.bin/.json`）与输出（`output_*.bin/.json`）；
  - `dump_tensors(named_tensors, stage, dump_dir=None)`：批量 dump 一组命名 tensor；
  - `dump_tensor(tensor, name, stage=None, dump_dir=None)`：单 tensor dump 原语。
- 产物：数据 `.bin`（字节视角导出，bf16/fp16 等通用）+ 元信息 `.json`（shape/dtype/device/data_ptr 等）；dump 目录复用 `XPUOPS_DUMP_DIR`（与异常 dump 一致）。
- 实现约束（保证服务器环境理论可运行）：仅使用 torch / torch_npu 标准接口（`.cpu()` 同步 D2H、`reshape(-1).view(torch.uint8)` 字节导出），规避 numpy 对 bf16 的限制；不依赖外挂库 so、不改动任何 C++ 代码、无需重新编译。
- 语义边界：异常路径输出 tensor 不可得（kernel 未正常写回），仅 dump 输入；输出指针现场由异常回调 `_args.bin` 提供。详见 README 1.6 节。
- 同步更新：`examples/demo_ifa.py`（正常/异常两阶段接入 `run_with_tensor_dump`，产物校验新增 `[INPUT-OK]` / `[OUTPUT-OK]` / query 大小 / 污染 block_table 读回）、`README.md`（1.1.1 机制、1.4 demo、1.5 配置、1.6 新章节、2.x 接入与 FAQ、附录源码说明）。

**验证状态**：本次变更因服务器资源不足未做运行验证，为理论交付；既有异常自动 dump 链路（args/info 校验）在此前版本已实测通过（910_93 + cann-9.1.T103），本次未改动该链路的任何代码。tensor dump 部署后按 README 1.4 / 1.6 节步骤验证即可；换服务器环境的完整实机验证按 `docs/VERIFICATION_GUIDE.md` 执行（V1~V10 验证项与预期结论，与 README 逐条对齐）。
