# optimize_datacopy_pre_allocation_vec样例

## 概述

本样例在double buffer的DataCopy搬运场景下，演示MTE2 Pre-Allocation特性。两块UB buffer（ping/pong）轮换复用，每块buffer同时承担CopyIn（GM→UB，MTE2）、Compute（向量加标量`Adds`，原地计算）、CopyOut（UB→GM，MTE3）三个阶段。由于同一块buffer被MTE2写入、MTE3读出，循环复用时后续迭代的MTE2需等待前序迭代的MTE3完成，形成MTE3→MTE2依赖屏障。使能Pre-Allocation特性后，MTE2搬运指令可提前发出到总线上，待屏障解除后再写入UB，从而隐藏同步屏障延迟，提升MTE2流水利用率。

## 支持的产品
- Ascend 960PR/Ascend 960DT

## 目录结构介绍
```
├── optimize_datacopy_pre_allocation_vec
│   ├── scripts
│   │   ├── gen_data.py                              // 输入数据和真值数据生成脚本
│   │   └── verify_result.py                         // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt                               // 编译工程文件
│   ├── data_utils.h                                 // 数据读入写出函数
│   └── pre_allocation_datacopy.asc                  // AscendC算子实现 & 调用样例
```

## 算子描述
- 算子功能：  
  在double buffer的DataCopy搬运场景下，使能MTE2 Pre-Allocation特性，跳过同一块UB buffer被MTE3读出后才能被下一轮MTE2覆写的MTE3→MTE2屏障，减少同步屏障带来的流水间隙。
- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="3" align="center">AIC算子</td></tr>
  </tr>
  <tr><td rowspan="2" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">8192</td><td align="center">float</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">y</td><td align="center">8192</td><td align="center">float</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">pre_allocation_datacopy</td></tr>
  </table>
- 算子实现：  
  1. 将总长度8192的输入数据沿元素方向切分为8个tile，每个tile 1024个元素。
  2. 分配两块UB buffer（ping/pong）显式管理地址（`TPosition::VECCALC`），两块buffer轮换复用，每块同时承担MTE2写入、V原地计算、MTE3读出。
  3. 每轮迭代执行三步：CopyIn（GM→UB，MTE2搬运，使能Pre-Allocation）、Compute（原地`Adds`，`y=x+1`）、CopyOut（UB→GM，MTE3搬运），通过`SetFlag/WaitFlag`显式管理`MTE3_MTE2`、`MTE2_V`、`V_MTE3`三组硬件同步事件，并交替使用`EVENT_ID0/EVENT_ID1`精确对应两块buffer的复用周期。
  4. 在CopyIn中通过`DataCopy(dst, src, count, true)`的第4个参数`enablePreReadIgnoreSync=true`使能Pre-Allocation特性，使MTE2提前发出GM读请求，跳过`MTE3_MTE2`屏障等待。

  - Pre-Allocation原理：  
    在double buffer场景下，同一块UB buffer被循环复用：当前迭代的MTE3把数据搬出后，下一轮同buffer的MTE2才能写入。不使能Pre-Allocation时，后续迭代的MTE2（GM→UB搬运）需等待前序迭代的MTE3（UB→GM搬出）完成才能发出GM读请求，GM读取延迟期间MTE2流水空闲，形成bubble。使能Pre-Allocation后，MTE2指令可提前发出GM读请求，数据先暂存在总线上，待MTE3完成、屏障解除后立即写入UB，从而隐藏GM读取延迟，提升MTE2流水利用率。相关文档参考[MTE2支持pre-allocate](../../../../../../docs/zh/guide/programming_guide/advanced_programming/mte2_pre_allocation.md)。

  - 调用实现  
    使用内核调用符<<<>>>调用核函数。
## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/zh/quick_start.md)，选择对应配置环境变量的命令。
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
  cmake ..;make -j;                                                         # 编译工程
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
