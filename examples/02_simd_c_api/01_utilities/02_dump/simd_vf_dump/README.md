# SimdVF Dump接口功能说明


## 概述

本样例介绍simd_vf侧asc_dump系列接口使用方法，在vector函数中通过`asc_dump_ubuf`、`asc_dump_reg`、`asc_dump`等接口实现UB和寄存器数据的可视化打印。Host侧在同一个`.asc`文件中生成输入与真值，核函数从GM读取输入、执行C API加法并写回GM，最后在Host侧比较结果。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.2.0 |

## 目录结构介绍

```
├── simd_vf_dump
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── simd_vf_dump.asc        // 内核、Host调用及结果校验
│   └── README.md               // 样例说明文档
```

## 样例描述

- 样例功能：

  使用vector编程模式，展示simd_vf侧asc_dump系列接口的基本使用方法。Host侧生成输入，核函数通过`asc_vf_call`打印原始与修改后的数据，并使用C API执行加法；Host侧将输出与真值比较。

  1. `asc_dump_ubuf<T>`-打印UB数据
  2. `asc_dump_reg<T>`-打印寄存器数据
  3. `asc_dump<T>`-打印UB/Reg数据（通用接口）

- simd_vf dump接口用法：

  | 接口 | 地址类型 | 示例 |
  |------|----------|------|
  | `asc_dump_ubuf<T>` | `__ubuf__` | `asc_dump_ubuf<float>(input, desc, dump_size);` |
  | `asc_dump_reg<T>` | Reg | `asc_dump_reg<float>(input, desc, dump_size);` |
  | `asc_dump<T>` | `__ubuf__` / Reg | `asc_dump<float>(input, desc, dump_size);` |

  参数说明：
  - 模板参数`T`：指定数据类型（float/half等）
  - `input`：UB数据地址或Reg寄存器对象
  - `desc`：打印控制描述符
  - `dump_size`：单次dump的数据元素个数

- 调用实现

  在`__global__ __vector__`核函数内使用`asc_vf_call`依次打印原始UB和寄存器数据，通过VF中的C API `asc_add_scalar`执行`x + 1.0f`后再打印修改后的UB，最后将结果写回GM。`asc_dump`属于C/C++ API共用的调试接口，模板参数仅用于指定打印数据类型。Host侧直接生成输入并校验输出。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build -j
  ./build/demo
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：Ascend 950PR&950DT系列产品 |

- 执行结果
  最终执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
