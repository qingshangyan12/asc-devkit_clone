# Div样例

## 概述

本样例基于C API编程接口实现Div运算，主要调用asc_div接口。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
div
│   ├── scripts/             // 测试脚本目录
│   │   └── gen_data.py      // 生成测试输入和golden数据
│   ├── CMakeLists.txt       // 编译工程文件
│   ├── data_utils.h         // 数据读写工具函数
│   ├── div.asc              // Ascend C算子实现 & 调用样例
│   └── README.md            // 样例说明文档
```

## 样例描述

- 样例功能：

  样例计算float类型数据，使用asc_div接口执行除法运算，计算公式如下：
  $$z_i = \frac{x_i}{y_i}$$

- 样例规格
  | 项目 | name | shape | data type |
  | --- | --- | --- | --- |
  | 样例类型(OpType) | AIV样例 |  |  |
  | 样例输入 | x | [1, 1024] | float |
  | 样例输入 | y | [1, 1024] | float |
  | 样例输出 | z | [1, 1024] | float |
  | 核函数名 | kernel_div |  |  |

- 样例实现
    - 实现流程：
        1. 在Host侧分配内存并初始化输入数据
        2. 将数据从Host内存搬运至Device全局内存
        3. 调用核函数，在Vector核上执行计算
        4. 核函数内部：通过DataCopy将输入数据从Global Memory搬入Unified Buffer
        5. 在Unified Buffer上调用VF（Vector Function）函数
        6. div_vf函数内部执行asc_load、asc_div、asc_store操作
        7. 将计算结果从Unified Buffer搬回Global Memory
        8. 在Host端进行精度验证和golden data对比（允许1 ULP误差）

  - 约束条件：
      - 输入数据长度必须为GetVecLen()的整数倍
      - 除数不能为0，实际应用中需要增加除数为0的检查
      - 样例目前仅支持float数据类型

## 编译运行

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake ..;make -j;                                                         # 编译工程
  python3 ../scripts/gen_data.py;                                           # 生成测试输入数据
  ./demo                                                                    # 执行样例
  ```

  使用 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。
- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果  
  执行结果如下，说明精度对比成功（允许1 ULP误差）。
  ```bash
  test pass!
  ```
