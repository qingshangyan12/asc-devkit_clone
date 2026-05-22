# 基于Add的SIMT时间戳打点功能实现样例

## 概述

本样例基于 SIMT Add，演示在 SIMT 核函数中使用 `clock()` 接口实现时间戳打点的方法，用于记录计算前后的时间戳并统计执行耗时。该接口适用于SIMD&SIMT混合编程场景或者SIMT编程场景。


## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.0.0-beta.2

## 目录结构

```
├── 06_clock
│   ├── add.asc                # SIMT Add与clock打点实现样例
│   ├── CMakeLists.txt         # cmake编译文件
│   └── README.md
```

## 样例描述

- 样例功能:

  基于 SIMT 实现 Add 计算，并在 SIMT 核函数内使用 `clock()` 记录执行周期。

- Add功能说明:

  Add 对两个输入 Tensor `x`、`y` 逐元素相加，并将结果写入输出 Tensor `z`。本样例中，`x`、`y`、`z` 的长度均为 `48 * 256`，每个 SIMT 线程处理一个元素。计算关系如下：

  ```text
  z[i] = x[i] + y[i]
  ```

- clock打点说明:

  核函数中在 Add 计算前调用一次 `clock()` 记录开始时间戳，在写回计算结果后再次调用 `clock()` 记录结束时间戳，并通过二者差值输出当前线程的执行周期。

  ```text
  execute_cycle = clock_after_add - clock_before_add
  ```

  因为 Add 的计算过程较短，样例更适合用于展示 `clock()` 接口的使用方法和打点位置，而不是用于评估复杂计算场景的性能。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上 CANN 开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

  默认路径，root 用户安装 CANN 软件包：

  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

  默认路径，非 root 用户安装 CANN 软件包：

  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

  指定路径 `install_path`，安装 CANN 软件包：

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j
  ./demo
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：`dav-3510` 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明时间打点功能和精度对比成功。

  ```text
  simt execute cycle : 22289
  ...
  [Success] Case accuracy is verification passed.
  ```
