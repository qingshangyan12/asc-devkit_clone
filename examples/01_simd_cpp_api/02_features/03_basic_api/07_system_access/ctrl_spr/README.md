# CtrlSpr样例

## 概述

本样例基于SetCtrlSpr、GetCtrlSpr、ResetCtrlSpr接口实现对CTRL寄存器（控制寄存器）的特定比特位的设置、读取和重置功能，并通过原子累加操作验证功能是否正确。

本样例演示了设置、读取和重置CTRL[8:6]和CTRL[10:9]比特位：
- CTRL[8:6]用于控制数据从L0C Buffer/Unified Buffer/L1 Buffer搬运至Global Memory时原子操作的使能及数据类型选择。设置为1时，使能原子操作，数据类型为float。
- CTRL[10:9]用于控制原子操作的类型，设置为0时选择ADD操作。
- 使用完寄存器后，用ResetCtrlSpr接口将寄存器设置回默认值，防止其他的数据搬运造成影响。

样例使用2个核进行验证，每个核搬运256个float元素到同一目标地址，通过原子累加操作验证CTRL寄存器设置后的功能效果。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── ctrl_spr
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── ctrl_spr.asc            // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  验证CTRL寄存器操作的完整流程及其功能效果：
  1. CopyIn阶段：从GM搬运数据到UB
  2. Compute阶段：SetCtrlSpr（设置CTRL[8:6]=1使能float原子操作，CTRL[10:9]=0选择ADD操作）→ GetCtrlSpr（读取验证）
  3. CopyOut阶段：DataCopy（从UB搬运到GM进行原子累加）→ ResetCtrlSpr（重置寄存器）→ GetCtrlSpr（读取验证）

- 样例规格：
  <table>
  <caption>表1：样例规格说明</caption>
  <tr><td rowspan="1" align="center">核数</td><td colspan="4" align="center">2</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">src</td><td align="center">[2, 256]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">dst_init（dst的初始值）</td><td align="center">[1, 256]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td><td align="center">output_ctrl</td><td align="center">[2, 8]</td><td align="center">int64_t</td><td align="center">ND</td></tr>
  <tr><td align="center">output_atomic_add</td><td align="center">[1, 256]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">kernel_ctrl_spr</td></tr>  
  </table>

- 输出说明：
  - output_ctrl.bin：寄存器值验证输出，每个核输出8个int64_t值（前4个为CTRL[8:6]设置后的值=1，后4个为CTRL[8:6]重置后的值=0）
  - output_atomic_add.bin：原子累加功能验证输出，2个核的数据累加到同一目标地址


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  mkdir -p build && cd build;                                          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                 # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py                                       # 生成测试输入数据和golden数据
  ./demo                                                               # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py                                  # 验证输出结果是否正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功：
  ```bash
  test pass
  ```