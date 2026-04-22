# move_reg样例

## 概述
本样例基于RegBase编程范式实现基于Mask的选择性数据拷贝功能。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── move_reg
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── move_reg.asc                   // AscendC样例实现 & 调用样例
│   └── README.md                      // 样例介绍
```

## 样例描述
- 样例功能：  
  输入三个数据类型为float，shape为[1, 256]的向量x、y和cond，使用MaskGenWithRegTensor接口从cond数据的RegTensor中提取部分bit生成MaskReg，使用Select接口根据mask从x和y中选择数据：mask位为1选择x，mask位为0选择y，输出结果向量z。x数据的加载使用LoadUnAlignPre/LoadUnAlign非对齐接口，z数据的存储使用StoreUnAlign/StoreUnAlignPost非对齐接口。

  **场景1：单核模式**
  - 一个核处理全部数据
  - 样例规格：
    <table>
    <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">AIV样例</td></tr>
    <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
    <tr><td></td><td align="center">x</td><td align="center">[1, 256]</td><td align="center">float</td></tr>
    <tr><td></td><td align="center">y</td><td align="center">[1, 256]</td><td align="center">float</td></tr>
    <tr><td></td><td align="center">cond</td><td align="center">[1, 256]</td><td align="center">float</td></tr>
    <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[1, 256]</td><td align="center">float</td></tr>
    <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">move_reg</td></tr>
    </table>
  - 样例实现：
    MaskSelectCopyVF函数内先使用LoadUnAlignPre/LoadUnAlign加载cond数据到RegTensor，再通过MaskGenWithRegTensor&lt;float, 4&gt;从condReg的第4个b32偏移位置提取mask，然后使用LoadUnAlign加载x和y数据，调用Select根据mask完成选择性拷贝，最后通过StoreUnAlign/StoreUnAlignPost将结果写回UB。
    - 调用实现
      使用内核调用符`<<<>>>`调用核函数，启动1个核。

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
  mkdir -p build && cd build;                                                    # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;                           # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                                 # 生成测试输入数据
  ./demo                                                                         # 执行编译生成的可执行程序
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
