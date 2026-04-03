# Fmod样例

## 概述

本样例基于Fmod高阶API实现按元素浮点数取余的功能。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```plain
├── fmod
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── fmod.asc                // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  按元素计算两个浮点数a，b相除后的余数。

  计算公式如下：
  $$dstTensor_i = Fmod(src0Tensor_i, src1Tensor_i)$$

  $$Fmod(x, y) = x - Trunc(x / y) * y$$

  其中，Trunc为向零取整操作。举例如下：

  Fmod(2.0, 1.5) = 0.5

  Fmod(-3.0, 1.1) = -0.8

- 样例规格：  
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> fmod </td></tr>

  <tr><td rowspan="4" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src0</td><td align="center">[1, 159]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">src1</td><td align="center">[1, 159]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[1, 159]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">fmod_custom</td></tr>
  </table>

- 样例实现：  
  本样例中实现的是固定shape为输入src0[1, 159]、src1[1, 159]，输出dst[1, 159]的fmod_custom样例。

  - Kernel实现

    使用Fmod高阶API计算取余运算，可选择使用临时buffer和指定计算元素个数

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行  

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DNPU_ARCH=dav-2201 ..;make -j;   # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

  **CPU调试模式编译：** 添加 `-DRUN_MODE=cpu` 参数

  ```bash
  cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..;make -j
  ```

  **NPU仿真模式编译：** 添加 `-DRUN_MODE=sim` 参数

  ```bash
  cmake -DRUN_MODE=sim -DNPU_ARCH=dav-2201 ..;make -j
  ```

  **编译选项说明：**

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | `RUN_MODE` | 运行模式 | `npu`、`cpu`、`sim` | `npu` |
  | `NPU_ARCH` | NPU硬件架构 | `dav-2201`、`dav-3510` | `dav-2201` |

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```
