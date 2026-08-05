# SIMD VF FIFO压力测试

## 概述

本目录提供一键式FIFO打印压力测试，覆盖单个SIMD VF内的大批量打印、多个不同SIMD VF及重复VF调用、AICore与SIMD VF交错打印，以及整型、十六进制、浮点型格式化输出。

每次进程执行5次独立Kernel，每轮输出16408条记录，单次进程共82040条记录和10MB以上日志。`run.sh`默认执行3次进程级迭代，总输出超过30MB。`acl.json`将每核SIMD打印空间设置为16MB；每条编码记录仍小于2KB，持续触发2KB预留UB FIFO的等待、传输和复用。

## 运行

环境中已配置`CANN_PATH`、`ASCEND_HOME_PATH`或`ASCEND_TOOLKIT_HOME`时，直接执行：

```bash
./run.sh
```

脚本也会自动识别`/usr/local/Ascend/cann`。可通过`ASC_ARCH`、`JOBS`和`ITERATIONS`覆盖默认的`dav-3510`、并行编译数和3次迭代。

成功时输出`FIFO_STRESS PASS`。每条记录都包含round、类别和序号，`verify_log.py`会逐轮检查记录全集、重复、缺号、格式化数值、阶段标记和日志字节数，任何数据丢失都会使测试失败。

## 日志

每次运行会重新生成`logs/`：

- `configure.log`：CMake配置日志。
- `build.log`：编译日志。
- `run_N.log`：第N次迭代的完整设备与宿主端输出，供人工复核。
- `verify_N.log`：第N次迭代的自动校验结果。
- `summary.log`：全部迭代的状态和字节数汇总。
