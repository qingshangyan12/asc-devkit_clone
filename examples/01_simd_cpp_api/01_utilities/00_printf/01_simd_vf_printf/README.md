# SimdVF Printf接口功能说明

## 概述

本样例介绍simd_vf侧printf接口使用方法，通过`__asc_simd_vf::printf_impl`接口在vector函数中打印调试信息。同时演示simd_vf侧的asc_dump_ubuf/asc_dump数据打印接口，以及simt_vf侧的printf接口。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 01_simd_vf_printf
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据生成脚本
│   │   └── verify_result.py    // 验证输出日志的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── simd_vf_printf.asc      // Ascend C样例实现&调用样例
```

## 样例描述

- 样例功能：

  使用vector编程模式，展示simd_vf printf接口的基本使用方法。核函数内部依次调用simd_vf/simt_vf侧printf接口打印不同类型的数据。

  本样例展示以下printf功能：
  1. `__asc_simd_vf::printf_impl`打印int/uint/float/string格式数据
  2. `__asc_simd_vf::printf_impl`打印hex/ptr格式数据
  3. simd_vf侧`printf`打印纯字符串
  4. simd_vf侧`asc_dump_ubuf`/`asc_dump`打印UB数据
  5. simt_vf侧`printf`打印线程信息

- simd_vf printf接口支持格式：

  | 格式符 | 说明 | 示例 |
  |--------|------|------|
  | `%d` | 整型打印 | `__asc_simd_vf::printf_impl(fmt, 10);` |
  | `%u` | 无符号整型打印 | `__asc_simd_vf::printf_impl(fmt, 20U);` |
  | `%x` | 十六进制打印 | `__asc_simd_vf::printf_impl(fmt, 255);` |
  | `%f` | 浮点型打印(float) | `__asc_simd_vf::printf_impl(fmt, 3.14f);` |
  | `%s` | 字符串打印 | `__asc_simd_vf::printf_impl(fmt, "test");` |
  | `%p` | 指针打印 | `__asc_simd_vf::printf_impl(fmt, ptr);` |

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
  ./demo > ../output/log.txt 2>&1                                           # 执行样例并保存日志
  python3 ../scripts/verify_result.py ../output/log.txt                     # 验证输出日志是否正确
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`（默认） | NPU架构：dav-3510对应Ascend 950PR/Ascend 950DT |

- 执行结果
  最终执行结果如下，说明日志校验成功。
  ```bash
  [Success]! All validations passed.
  ```
