# SIMT Kernel日志功能样例

## 概述

本样例演示在SIMT编程模式下，Ascend日志功能的完整使用方式，包括日志打屏输出、日志落盘存储、日志级别控制等功能。通过配置环境变量，开发者可以灵活控制日志输出行为，辅助算子开发与问题定位。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构

```
├── 00_kernel_log
│   ├── CMakeLists.txt         # cmake编译文件
│   ├── kernel_log.asc         # SIMT Kernel示例代码
|   └── README.md
```

## 日志功能说明

本样例展示Ascend日志功能的三个核心能力：

- **日志打屏**：通过环境变量控制日志是否输出到标准输出（屏幕），便于实时查看
- **日志落盘**：通过环境变量指定日志文件存储路径，便于后续分析
- **日志级别**：通过环境变量控制日志输出级别，按需过滤日志内容

### 日志级别说明

| 值 | 级别 | 说明 |
|----|------|------|
| 0 | DEBUG | 输出所有日志（DEBUG/INFO/WARNING/ERROR），信息最详细 |
| 1 | INFO | 输出INFO及以上级别日志（INFO/WARNING/ERROR） |
| 2 | WARNING | 输出WARNING及以上级别日志（WARNING/ERROR） |
| 3 | ERROR | 仅输出ERROR级别日志 |
| 4 | NULL | 不输出日志 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 配置日志环境变量（仅NPU模式）
  本样例支持通过环境变量控制日志输出行为（以下环境变量仅在NPU模式下生效）：
  ```bash
  export ASCEND_PROCESS_LOG_PATH=./log        # 日志落盘路径
  export ASCEND_SLOG_PRINT_TO_STDOUT=1        # 控制日志是否打印到屏幕（1：开启，0：关闭）
  export ASCEND_GLOBAL_LOG_LEVEL=1            # 控制日志级别
  ```

  > 注意：日志环境变量控制Ascend内部库（RUNTIME/ASCENDCL等）的日志输出。Kernel内部的`printf`输出不受这些环境变量影响。

- 样例执行
  ```bash
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # 默认npu模式
  ./demo
  ```
  使用 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

  编译选项说明：
  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |

  执行后会打印类似以下信息：
  ```
  [INFO] Input shape: 12288
  [INFO] Launching kernel to demonstrate device-side printf logging...
  [SIMT] Block 0: Thread 0 executing, idx = 0, total_length = 12288
  [SIMT] Block 1: Thread 0 executing, idx = 256, total_length = 12288
  ...
  [INFO] Execution completed. Check for device printf log messages above.
  ```

  同时在`./log`目录下会生成日志文件，包含详细的运行日志。
