# read_write_sync样例

## 概述
本样例演示了SIMD场景下，基于RegBase编程范式下读操作与写操作之间依赖场景下的同步，样例中使用到了寄存器保序这一关键特性，可以优化读写之间的同步指令。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── read_write_sync
│   ├── scripts
│   │   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                 // 编译工程文件
│   ├── data_utils.h                   // 数据读入写出函数
│   ├── read_write_sync.asc            // AscendC算子实现 & 调用样例
│   └── README.md                      // 样例介绍
```

## 算子描述
- 算子功能：  
  算子输入一个长度为1024的向量A和一个长度为1024的向量B，数据类型为float，进行向量相加操作。
- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="3" align="center">AIV算子</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">1024</td><td align="center">float</td></tr>
  <tr><td align="center">y</td><td align="center">1024</td><td align="center">float</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">z</td><td align="center">1024</td><td align="center">float</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">read_write_sync</td></tr>
  </table>
- 算子实现：  
  主要包括以下步骤：
  1. 初始化参数
  2. 将数据从GM搬运到UB
  3. 调用AddVF进行计算，在vf函数中每次迭代读取向量A到reg0，读取向量B的数据到reg1，进行相加后结果写到reg0，随后将reg0的数据保存到dstAddr，写指令与Add指令都用到了寄存器reg0，读指令与Add指令都用到了寄存器reg1，触发了寄存器保序机制，可以优化掉读写之间的同步指令
  4. 将结果从UB搬运回GM
  
  - 调用实现  
    使用内核调用符<<<>>>调用核函数。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
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
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DNPU_ARCH=dav-3510 ..;make -j;                                     # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DRUN_MODE=cpu` 或 `-DRUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DRUN_MODE=cpu -DNPU_ARCH=dav-3510 ..;make -j; # cpu调试模式
  cmake -DRUN_MODE=sim -DNPU_ARCH=dav-3510 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `NPU_ARCH` | `dav-3510`（默认） | NPU 架构：dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```