# hifloat8_t SwiGLU静态Tensor样例

## 概述

本样例基于静态Tensor编程方式，实现hifloat8_t输入和输出的SwiGLU激活算子，展示“反量化-计算-再量化”（DQ-Compute-Q）开发模式。算子在float数据上完成sigmoid和逐元素乘法，使用CAST_ROUND将结果量化回hifloat8_t。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构

```
├── CMakeLists.txt       // 编译工程文件
├── data_utils.h         // Host侧文件读写函数
├── hif8_swiglu.asc      // Ascend C Kernel和Host调用代码
├── scripts
│   ├── gen_data.py      // 生成hifloat8_t输入和参考输出
│   ├── hif8.py          // hifloat8_t CAST_ROUND编解码
│   └── verify_result.py // 解码并校验输出
└── README.md
```

## 算子规格

算子一次处理2048个元素，输入和输出均为ND格式。

| 名称 | 输入/输出 | Shape | 数据类型 |
| --- | --- | --- | --- |
| g | 输入 | (1, 2048) | hifloat8_t |
| u | 输入 | (1, 2048) | hifloat8_t |
| z | 输出 | (1, 2048) | hifloat8_t |

计算公式为：

$$z_i = Q(D(g_i) \cdot \sigma(D(g_i)) \cdot D(u_i))$$

其中，$D$为hifloat8_t到float的无损转换，$Q$为float到hifloat8_t的CAST_ROUND转换。

## 实现说明

- 使用`LocalTensor`构造函数按字节偏移直接规划UB空间，不使用TPipe/TQue。
- 使用`DataCopy`完成hifloat8_t数据搬运。
- 使用RegBase `Cast`完成hifloat8_t与float之间的转换，在寄存器中执行SwiGLU计算。
- 使用`CAST_ROUND`完成再量化，中间float数据不落UB。
- 使用`SetFlag/WaitFlag`同步MTE2、Vector和MTE3流水。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  mkdir -p build && cd build                         # 创建并进入build目录
  cmake -DCMAKE_ASC_RUN_MODE=npu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j                                             # 编译工程
  python3 ../scripts/gen_data.py                     # 生成测试输入数据
  ./demo                                              # 执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin  # 验证输出结果
  ```

  使用NPU仿真模式时，将`CMAKE_ASC_RUN_MODE`设置为`sim`：

  ```bash
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim ..
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

  > **注意：** 切换运行模式前需清理build目录中的CMake缓存，或重新创建build目录。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构 |
  | `CMAKE_VF_MODE` | `true`、`false`（默认） | 是否开启SIMD VF融合 |

- 执行结果

  执行结果如下，说明精度对比成功。

  ```text
  test pass!
  ```

输出校验脚本会先将结果解码为float，再与CAST_ROUND生成的参考结果比较。
