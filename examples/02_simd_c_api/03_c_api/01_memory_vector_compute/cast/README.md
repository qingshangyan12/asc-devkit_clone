# cast样例

## 概述

本样例基于Memory矢量计算接口[asc_half2int4](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md)和[asc_half2int32](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md)，演示half数据向上取整转换为int4或int32。

本样例支持两种类型转换场景，通过CMake编译参数`SCENARIO_NUM`选择。

| `SCENARIO_NUM` | 类型转换 |
| --- | --- |
| `0` | half转换为打包int4 |
| `1` | half转换为int32 |

## 支持产品及CANN版本

| 产品 | CANN软件版本 |
| --- | --- |
| Atlas A2训练/推理系列、Atlas A3训练/推理系列 | >= CANN 9.2.0 |

## 目录结构

```text
cast/
├── scripts/gen_data.py       # 输入和golden数据生成脚本
├── CMakeLists.txt            # 编译工程文件
├── data_utils.h              # 二进制数据读取工具
├── cast.asc                  # C API内核和主机调用代码
├── README.md
└── README_en.md
```

## 样例描述

本样例将512个half输入元素从GM搬入UB后进行向上取整类型转换，再将结果搬回GM。主机侧使用内核调用符`<<<>>>`调用`cast_kernel`，并按输出字节逐一比对`output/golden.bin`。

**场景0：half转换为打包int4**

- 样例功能：将half数据向上取整转换为int4b_t数据，并打包为uint8_t输出。
- 样例规格：

  | 项目 | 规格 |
  | --- | --- |
  | 样例类型（OpType） | AIV样例 |
  | 样例输入 | `x`，[1, 512]，half |
  | 样例输出 | `y`，[1, 512]，int4b_t，两个元素打包为1个uint8_t |
  | 核函数名 | `cast_kernel` |

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将512个half元素从GM搬入UB。
  - 计算：调用`asc_half2int4_ru`按CEIL模式舍入，并将结果饱和到[-8, 7]。
  - 搬出：将256B打包结果通过`asc_copy_ub2gm_align`从UB搬回GM。

**场景1：half转换为int32**

- 样例功能：将half数据向上取整转换为int32_t数据。
- 样例规格：

  | 项目 | 规格 |
  | --- | --- |
  | 样例类型（OpType） | AIV样例 |
  | 样例输入 | `x`，[1, 512]，half |
  | 样例输出 | `y`，[1, 512]，int32_t |
  | 核函数名 | `cast_kernel` |

- 样例实现：
  - 搬入：调用`asc_copy_gm2ub_align`将512个half元素从GM搬入UB。
  - 计算：调用`asc_half2int32_ru`按CEIL模式舍入，得到512个int32_t元素。
  - 搬出：将2048B结果通过`asc_copy_ub2gm_align`从UB搬回GM。

数据生成脚本使用包含正数、负数及饱和边界的确定性half输入生成golden数据。

## 编译运行

在本样例根目录执行以下步骤。

- 配置环境变量

  请根据CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)配置环境变量。

```bash
source ${install_path}/cann/set_env.sh
```

  > **说明：** `${install_path}`为CANN包安装目录；root用户默认为`/usr/local/Ascend`，非root用户默认为`${HOME}/Ascend`。

- NPU运行

```bash
SCENARIO_NUM=0                                                                                  # 执行场景0
mkdir -p build && cd build;                                                                       # 创建并进入build目录
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # 编译工程（默认NPU模式）
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM                                       # 生成测试输入数据和真值数据
./demo                                                                                           # 执行编译生成的可执行程序，执行样例
```

- 编译选项说明

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU架构：Atlas A2训练/推理系列、Atlas A3训练/推理系列 |
| `SCENARIO_NUM` | `0`、`1` | 样例场景：half转换为打包int4、half转换为int32 |

- 执行结果

输出与golden数据比较通过时，打印如下信息：

```text
test pass!
```
