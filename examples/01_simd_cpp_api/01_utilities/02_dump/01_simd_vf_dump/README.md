# SimdVF Dump接口功能说明

## 概述

本样例介绍simd_vf侧asc_dump系列接口使用方法，在vector函数中通过`__asc_simd_vf::asc_dump_ubuf`、`asc_dump`、`__asc_simd_vf::asc_dump_reg`等接口实现UB和寄存器数据的可视化打印。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品

## 目录结构介绍

```
├── 01_simd_vf_dump
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据生成脚本
│   │   └── verify_result.py    // 验证输出日志的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── simd_vf_dump.asc        // Ascend C 样例实现 & 调用样例
```

## 样例描述

- 样例功能：

  使用vector编程模式，展示simd_vf侧asc_dump系列接口的基本使用方法。核函数内部通过`asc_vf_call`调用simd_vf函数，在vector函数中使用以下dump接口：

  1. `__asc_simd_vf::asc_dump_ubuf<T>` - 打印UB数据
  2. `asc_dump<T>` - 打印UB/Reg数据（通用接口）
  3. `__asc_simd_vf::asc_dump_reg<T>` - 打印寄存器数据

- simd_vf dump接口用法：

  | 接口 | 地址类型 | 示例 |
  |------|----------|------|
  | `__asc_simd_vf::asc_dump_ubuf<T>` | `__ubuf__` | `__asc_simd_vf::asc_dump_ubuf<float>(addr, lines, count);` |
  | `asc_dump<T>` | `__ubuf__` / Reg | `asc_dump<float>(addr, lines, count);` |
  | `__asc_simd_vf::asc_dump_reg<T>` | Reg | `__asc_simd_vf::asc_dump_reg<float>(reg, lines, count);` |

  参数说明：
  - 模板参数 `T`：指定数据类型（float/half等）
  - `addr`：UB数据地址或Reg寄存器对象
  - `lines`：打印行数
  - `count`：每行打印的元素个数

  注意：开启dump输出需要编译时定义宏 `ASCENDC_SIMD_VF_DEBUG=1`（CMake中通过`ASCENDC_SIMD_VF_DEBUG`选项控制，默认ON）。

- 调用实现

  在`__global__ __vector__`核函数内使用`asc_vf_call<FuncName>()`调用simd_vf函数。

## 编译运行

- 配置环境变量
  在本样例根目录下执行如下步骤，编译并执行样例。
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

- 样例执行
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                      # 编译工程
  python3 ../scripts/gen_data.py                                            # 生成测试目录
  ./demo > ../output/log.txt 2>&1                                           # 执行样例并保存日志
  python3 ../scripts/verify_result.py ../output/log.txt                     # 验证输出日志是否正确
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认）、`dav-2201` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |
  | `ASCENDC_SIMD_VF_DEBUG` | `ON`（默认）、`OFF` | 开启/关闭 simd_vf dump 调试输出 |

- 执行结果
  最终执行结果如下，说明日志校验成功。
  ```bash
  [Success]! All validations passed.
  ```
