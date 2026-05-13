# SIMT编程模式实现Scatter算子样例

## 概述

样例基于Ascend C SIMT编程方式实现与PyTorch `scatter(input, dim, index, src)`一致的Scatter算子，将`src`中的元素按照`index`指定的`dim`轴位置写入输入张量，展示简化场景离散内存写访问类算子的开发方法。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构

```
├── basic_scatter
│   ├── CMakeLists.txt
│   ├── scatter.asc             # SIMT实现scatter调用样例
|   └── README.md
```

## 算子描述

- 算子功能：  
  scatter算子基于输入张量input生成输出张量output，并将src中的元素按照index指定的位置写入output。对index中的每个位置`[i][j]`，当`dim = 0`时，算子输出计算公式为：
  
  ```
  output[index[i][j]][j] = src[i][j]
  ```

  当`dim = 1`时，算子输出计算公式为：
  
  ```
  output[i][index[i][j]] = src[i][j]
  ```

  本样例构造的index不会让多个线程写入同一输出位置，避免并发写冲突影响结果校验。

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">scatter</td></tr>
  </tr>
  <tr><td rowspan="4" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">3 * 5 / 2 * 4</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">index</td><td align="center">2 * 5 / 2 * 2</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  <tr><td align="center">src</td><td align="center">2 * 5 / 2 * 2</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">output</td><td align="center">3 * 5 / 2 * 4</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">scatter_custom</td></tr>
  </table>

- 数据切分：  
  * 最大线程数：2048线程/Block
  * Block数量：根据index元素数量和AI Vector Core数量自动计算
  * 单线程处理：通过grid-stride循环处理一个或多个index元素

- 算子实现：  
  scatter算子的实现流程为先将input复制为output初值，再从Global Memory读取index和src。核函数根据当前线程处理的index线性位置反推多维坐标，将该坐标在dim轴上的值替换为index值，计算output偏移后写入src元素。

- 调用实现：  
  使用内核调用符<<<>>>调用核函数。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。
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
  cmake ..; make -j;            # 编译工程
  ./scatter                     # 执行样例
```
  执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```
