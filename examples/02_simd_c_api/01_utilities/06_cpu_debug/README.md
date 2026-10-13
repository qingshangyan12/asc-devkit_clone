# CPU Debug直调样例说明

## 概述

本样例以 Add 计算为载体，重点演示 Ascend C CPU Debug 功能的使用流程，包括 CPU 域编译、可执行程序运行、通过 GDB 进入调试模式，以及在核函数中设置断点、单步执行和查看变量。样例用于帮助开发者在不依赖 NPU 运行的情况下，对 Ascend C 核函数逻辑进行本地调试和问题定位。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.1.0 |

## 目录结构介绍

```
├── 06_cpu_debug
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── cpu_debug.asc           // Ascend C样例实现 & 调用样例
│   └── README.md               // 样例说明文档
```

## 样例描述

- CPU Debug介绍：
  CPU Debug功能支持对CPU执行过程中的运行状态进行调试，主要通过GDB工具实现。GDB调试支持设置断点、查看寄存器和内存状态、单步执行、查看调用栈等常用调试操作。

- 样例介绍：
  本样例使用 C API 实现`float`逐元素加法。每个block处理`2048`个数据：`asc_copy_gm2ub_align`将输入搬入UB，同步后调用SIMD VF内的`asc_add`计算，最后`asc_copy_ub2gm_align`将结果写回GM。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  ./cpu_debug
  ```
  上述命令已在`dav-3510` NPU环境下通过精度校验。若需调试CPU域，可使用独立的`build_cpu`目录并配置`-DCMAKE_ASC_RUN_MODE=cpu`；本次迁移在所用CANN 9.3.0容器中，该模式出现UB地址越界并未通过精度校验，不能视作已验证的运行路径。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`、`cpu` | `npu`已验证；`cpu`可用于CPU域编译，但本环境的CPU模拟器精度未通过 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：Ascend 950PR&950DT系列产品 |

- 执行结果
  执行结果如下，说明执行成功。
  ```bash
  [Success] Case accuracy is verification passed.
  ```
- 进入gdb模式调试
  > **说明：** 需使用 GDB 12.1及以上版本。

  CPU域程序支持通过gdb设置断点、单步执行和查看变量。需单独执行`cmake -S . -B build_cpu -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510`并编译，再进入`build_cpu`目录运行：
  ```bash
  gdb --args ./cpu_debug
  ```
  CPU Debug通过为每个核函数启动单独的子进程来模拟NPU的执行逻辑，因此使用gdb调试时，需要设置`follow-fork-mode`让gdb跟踪子进程，才能在核函数内部断点调试。进入gdb后，先设置跟踪子进程模式：

  ```text
  (gdb) set follow-fork-mode child
  ```

  然后按需进行调试，常用操作：

  ```text
  # 在核函数入口处设置断点
  (gdb) break add_custom

  # 运行程序
  (gdb) run

  # 单步执行
  (gdb) next

  # 继续执行到下一个断点
  (gdb) continue
  ```
