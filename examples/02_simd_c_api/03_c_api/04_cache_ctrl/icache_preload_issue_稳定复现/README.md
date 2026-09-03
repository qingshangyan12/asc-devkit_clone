# asc_icache_preload异常场景复现样例

## 概述

本样例使用纯Ascend C和ACL复现以下现象：Vector Core执行越界长度的`asc_icache_preload`后，当前kernel和第一次同步可能正常完成，但后续无关kernel的同步返回设备错误。本样例不依赖Python、PyTorch或torch_npu。

> **注意：** 本样例用于复现异常输入场景，不是`asc_icache_preload`的合法用法。`prefetch_len`的单位为2 KB；Vector Core的ICache大小为16 KB，接口要求`prefetch_len < 8`。样例中的`8`是刻意设置的越界值。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── icache_preload_issue
│   ├── CMakeLists.txt
│   ├── icache_probe_kernel.asc
│   ├── unrelated_probe_kernel.asc
│   ├── main.cpp
│   ├── README.md
│   └── README_en.md
```

## 样例流程

1. 动态加载第一个kernel库，根据命令行参数启动调用或不调用`asc_icache_preload`的kernel。
2. 调用`aclrtSynchronizeStream`，输出`FIRST_SYNC_OK`或`FIRST_SYNC_FAIL`。
3. 动态加载第二个独立kernel库，启动一个写入128字节GM的无关kernel。
4. 再次同步；失败时输出`PROBE_FAIL acl_error=<错误码>`，成功时校验两个kernel的输出并输出`PROBE_OK`。

## 编译运行

配置CANN环境后，在本样例目录执行：

```bash
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
```

运行不调用preload的对照组：

```bash
./demo --no-preload
```

预期输出：

```text
FIRST_SYNC_OK
PROBE_OK
```

运行越界preload复现组：

```bash
./demo --preload
```

原issue环境中的典型现象为第一次同步成功，第二次同步报告`507035`：

```text
FIRST_SYNC_OK
PROBE_FAIL acl_error=507035
```

实际错误出现阶段和错误码以运行环境为准。
