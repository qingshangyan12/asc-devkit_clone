# SIMT printf runtime解析样例

## 概述

本样例单独实现一个SIMT算子，用于验证device侧写入printf ring buffer、host侧模拟runtime解析printf数据并回写读指针的完整流程。

device侧逻辑参考 `asc-devkit/impl/utils/debug/asc_printf_simt_impl.h`，保留 `PrintTlvInfoHead`、参数区、格式串和参数字符串写入方式，区别是printf workspace地址由核函数入参传入，不依赖 `g_sysSimtPrintFifoSpace` 全局变量。

算子启动配置为64个block、每个block 2048个线程；每个线程循环打印128条printf日志。host侧额外申请一块GM维测缓冲区，按全局线程号保存一条 `PrintfCallDiag`。维测仅关注 `simt_printf_to_workspace` 内的函数调用，两个函数调用之间仅保留一个边界打点：每次调用前记录调用点，最后一次 `dcci` 后记录完成态。

host侧逻辑参考 `runtime/src/runtime/core/src/dfx/printf.cc` 中的 `InitSimtPrintf`、`GetReadLenAndAddr`、`ParseSimtPrintf`、`PrintSimtDump` 流程：

- 初始化2MB printf空间：`BlockInfo | BlockReadInfo | ring buffer | BlockWriteInfo`
- host创建轮询线程，线程入口先调用 `aclrtSetDevice(deviceId)`，再每0.2s D2H拷贝workspace
- 每次host拷贝出的2MB workspace原始数据落盘为 `printf_workspace_round_<round>.bin`
- 根据 `readIdx % remainLen` 和 `writeIdx % remainLen` 得到待读取窗口
- 打印读指针后的第一个 `PrintTlvInfoHead` 结构体字段和56字节原始hex
- 将ring buffer中连续或回绕的数据整理为可解析的 `PrintTlv`
- 解析 `DUMP_SIMT_PRINTF` TLV并打印
- 将 `readIdx += processedLen` 回写到device侧 `BlockReadInfo`
- 算子结束后将 `PrintfCallDiag` 维测GM回传host，按调用点和before/after阶段解析统计，并打印未完成线程样例

## 编译运行

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
./demo
```

`./demo` 会启动高并发printf压力写入，预期输出包含host轮询解析和读指针更新。由于总日志量为 `64 * 2048 * 128` 条，实际输出会非常大。截断示例：

```text
[HOST_PARSE] dump raw workspace: printf_workspace_round_0.bin (2097152 bytes)
[HOST_PARSE] FirstPrintTlvInfoHeadAfterRead: offset=80 type=0xf0f00f0f length=168 ...
[RUNTIME_PARSE] seq=0 block=0 thread=0 global=0 text=workspace-param
[HOST_PARSE] readIdx 0 -> 704, writeIdx=1408, processedLen=704, packIdx=8
[HOST_PARSE] dump raw workspace: printf_workspace_round_1.bin (2097152 bytes)
[HOST_PARSE] FirstPrintTlvInfoHeadAfterRead: offset=784 type=0xf0f00f0f length=168 ...
[RUNTIME_PARSE] seq=1 block=0 thread=0 global=0 text=workspace-param
[HOST_PARSE] readIdx 704 -> 180224, writeIdx=180224, processedLen=179520, packIdx=1024
```

## raw文件解析

构建后会同时生成 `parse_printf_workspace`，可解析 `./demo` 落盘的 `printf_workspace_round_<round>.bin`：

```bash
./parse_printf_workspace printf_workspace_round_0.bin printf_workspace_round_1.bin
```

解析工具会输出：

- `BlockInfo`
- `BlockReadInfo`
- `BlockWriteInfo`
- 读指针后的第一个 `PrintTlvInfoHead` 字段和raw hex
- 根据 `readIdx % remainLen` 到 `writeIdx % remainLen` 组成的读取窗口
- 窗口内每个 `DUMP_SIMT_PRINTF` TLV及还原后的printf文本
