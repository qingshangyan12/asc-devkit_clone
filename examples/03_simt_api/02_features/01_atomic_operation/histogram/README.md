# SIMT编程模式实现Histogram样例

## 概述

本样例展示了如何利用 Ascend C 的SIMT的`asc_atomic_add`接口，高效统计输入字节序列中每个字节值的出现频率。功能示意如下图：</br>
![intro.png](figure/introduction.png)</br>

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构

```text
histogram/
├── figure                     # 样例算法实现图
├── histogram.asc              # Histogram样例功能实现
├── CMakeLists.txt             # 样例构建脚本
└── README.md                  # 样例说明文档
```

## 样例描述

- 样例功能：
  Histogram样例对固定大小的输入字节流进行统计，统计各个字节数值（0-255）出现的频率。输入数据的Shape为 `294912` 。

- 样例规格：
  <table>
  <tr><td align="center">样例类型(OpType)</td><td colspan="4" align="center">histogram</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">294912</td><td align="center">uint8</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">actual_histogram</td><td align="center">256</td><td align="center">uint32</td><td align="center">ND</td></tr>
  </table>

- 数据切分：
  * 第一阶段Kernel `merge_block_local_histogram`：
    * ThreadBlock数：采用最大物理核数`72`。
    * 每个ThreadBlock线程数：采用默认值1024。
    * 单线程处理：单线程处理：按grid-stride遍历`uint32`输入，每次解析4个字节并更新所属Warp的局部直方图。
  * 第二阶段Kernel `merge_block_local_histogram`：
    * ThreadBlock数：`256`，每个ThreadBlock负责一个桶（bin）。
    * 每个ThreadBlock线程数：为了使用规约算法，采用高于72的第一个2的n次幂即`128`。
    * 单线程处理：每个线程读取若干份局部直方图中同一个桶的计数并累加，再在ThreadBlock内归约得到最终计数。
  * 结果预期：NPU输出的256个桶（bin）计数应与CPU基于同一输入计算得到的结果一致。

## 样例实现
整体流程分为两个 Kernel。第一阶段先将输入切分到72个ThreadBlock中处理，每个ThreadBlock生成一份局部直方图；第二阶段每个ThreadBlock按桶（bin）合并所有局部直方图，得到最终256桶的结果。
### 阶段一

  - 本阶段每个Warp都会维护一份局部直方图，当前的Warp的线程只会针对对应的局部直方图进行竞争写入，线程按grid-stride读取输入并通过`asc_atomic_add`将每个字节对应桶的计数加1，然后在ThreadBlock内合并所有Warp的局部直方图，局部直方图暂存在`UB`中，ThreadBlock内所有Warp的局部结果合并后写入`GM`。
  - 其中`asc_atomic_add`用于对同一Warp的局部直方图中的桶计数做原子累加，保证多个线程同时命中同一个bin时更新结果正确。但是由于原子冲突严重会影响性能，因此按Warp拆分局部直方图，可把原子更新限制在较小范围内，减少直接更新全局直方图带来的竞争开销。</br>
    </br>![local_his.png](figure/local_his.png)</br>
  

### 阶段二
- 每个ThreadBlock负责一个桶，读取第一阶段产生的72份局部直方图中该桶的计数，在UB中完成归约后写入GM。</br>
</br>
![merge_his.png](figure/merge_his.png)
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
  mkdir -p build && cd build      # 创建并进入 build 目录
  cmake ..                        # 配置工程
  make -j                         # 编译样例
  ./histogram                     # 执行样例
  ```

执行结果如下，说明精度对比成功。

```text
Running histogram256 on Ascend C SIMT for fixed xxx bytes
Validation passed
```
