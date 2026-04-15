# TransData样例

## 概述

本样例基于TransData高阶API实现数据排布格式转换功能，支持将输入数据的排布格式转换为目标排布格式。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```plain
├── transdata
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── transdata.asc           // Ascend C样例实现 & 调用样例
```

## 样例描述

将输入数据的排布格式转换为目标排布格式。  
除维度顺序变换外，其中涉及到C轴和N轴的拆分，具体转换方式为，C轴拆分为C1轴、C0轴，N轴拆分为N1轴、N0轴。对于位宽为16的数据类型的数据，C0和N0固定为16，C1和N1的计算公式如下：

$$ C1 = (C + C0 - 1) / C0 $$

$$ N1 = (N + N0 - 1) / N0 $$

本样例支持以下四种数据格式转换场景

### 场景1：NCDHW -> FRACTAL_Z_3D (mode = 1)

- 样例规格：
  <table border="2" align="center">
  <caption>表1：场景1样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> transdata </td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[16, 16, 1, 3, 5]</td><td align="center">half</td><td align="center">NCDHW</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[1, 1, 3, 5, 1, 16, 16]</td><td align="center">half</td><td align="center">FRACTAL_Z_3D</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">transdata_custom</td></tr>
  </table>

  **说明**：
  - 输入shape为[N, C, D, H, W] = [16, 16, 1, 3, 5]
  - 输出shape为[D, C1, H, W, N1, N0, C0] = [1, 1, 3, 5, 1, 16, 16]

### 场景2：FRACTAL_Z_3D -> NCDHW (mode = 2)

- 样例规格：
  <table border="2" align="center">
  <caption>表2：场景2样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> transdata </td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[1, 1, 3, 5, 1, 16, 16]</td><td align="center">half</td><td align="center">FRACTAL_Z_3D</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[16, 16, 1, 3, 5]</td><td align="center">half</td><td align="center">NCDHW</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">transdata_custom</td></tr>
  </table>

  **说明**：
  - 输入shape为[D, C1, H, W, N1, N0, C0] = [1, 1, 3, 5, 1, 16, 16]
  - 输出shape为[N, C, D, H, W] = [16, 16, 1, 3, 5]
  - 这是场景1的逆操作

### 场景3：NCDHW -> NDC1HWC0 (mode = 3)

- 样例规格：
  <table border="2" align="center">
  <caption>表3：场景3样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> transdata </td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[16, 16, 1, 3, 5]</td><td align="center">half</td><td align="center">NCDHW</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[16, 1, 1, 3, 5, 16]</td><td align="center">half</td><td align="center">NDC1HWC0</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">transdata_custom</td></tr>
  </table>

  **说明**：
  - 输入shape为[N, C, D, H, W] = [16, 16, 1, 3, 5]
  - 输出shape为[N, D, C1, H, W, C0] = [16, 1, 1, 3, 5, 16]

### 场景4：NDC1HWC0 -> NCDHW (mode = 4)

- 样例规格：
  <table border="2" align="center">
  <caption>表4：场景4样例输入输出规格</caption>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center"> transdata </td></tr>

  <tr><td rowspan="3" align="center">样例输入</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[16, 1, 1, 3, 5, 16]</td><td align="center">half</td><td align="center">NDC1HWC0</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">dst</td><td align="center">[16, 16, 1, 3, 5]</td><td align="center">half</td><td align="center">NCDHW</td></tr>

  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">transdata_custom</td></tr>
  </table>

  **说明**：
  - 输入shape为[N, D, C1, H, W, C0] = [16, 1, 1, 3, 5, 16]
  - 输出shape为[N, C, D, H, W] = [16, 16, 1, 3, 5]
  - 这是场景3的逆操作

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
  SCENARIO=1
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake -DNPU_ARCH=dav-2201 -DSCENARIO=$SCENARIO ..;make -j;   # 编译工程
  python3 ../scripts/gen_data.py --mode $SCENARIO  # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  ```

  **编译选项说明**

  | 参数 | 说明 | 可选值 | 默认值 |
  | ---- | ---- | ----- | ------ |
  | `RUN_MODE` | 运行模式：`npu`（NPU模式）、`cpu`（CPU调试模式）、`sim`（NPU仿真模式） | `npu`、`cpu`、`sim` | `npu` |
  | `NPU_ARCH` | NPU硬件架构 | `dav-2201`、`dav-3510` | `dav-2201` |
  | `SCENARIO` | 场景 | `1`、`2`、`3`、`4` | `1` |

  **NPU_ARCH 对应产品**

  | NPU_ARCH | 对应产品 |
  |----------|---------|
  | `dav-2201` | Atlas A2 训练/推理系列产品、Atlas A3 训练/推理系列产品 |
  | `dav-3510` | Ascend 950PR/Ascend 950DT |

  **多模式编译示例**

  - CPU调试模式：

    ```bash
    cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-2201 ..;make -j
    ```

  - NPU仿真模式：

    ```bash
    cmake -DRUN_MODE=sim -DNPU_ARCH=dav-2201 ..;make -j
    ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

  执行结果如下，说明精度对比成功。

  ```bash
  test pass!
  ```

  **说明**：
  - SCENARIO参数用于选择不同的转换场景：
    - SCENARIO=1：NCDHW -> FRACTAL_Z_3D
    - SCENARIO=2：FRACTAL_Z_3D -> NCDHW
    - SCENARIO=3：NCDHW -> NDC1HWC0
    - SCENARIO=4：NDC1HWC0 -> NCDHW
  - 每个场景需要重新编译并生成对应的测试数据
