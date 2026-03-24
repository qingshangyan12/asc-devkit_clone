# 纯SIMT编程模式实现Gather算子样例

## 概述
样例基于Ascend C纯SIMT编程方式实现Gather算子，从输入张量中采集指定的m行数据，展示离散内存访问类算子的开发方法。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构
```
├── pure_simt_gather
│   ├── CMakeLists.txt         # cmake编译文件
│   ├── gather_v2.asc          # SIMT实现gatherv2调用样例
│   ├── gather.asc             # SIMT实现gather调用样例
|   └── README.md
```

## 算子描述
### 1. gather算子

- 算子功能：  
  gather算子实现了从形状为M * N的二维向量input中获取指定索引的m行数据的功能, 这m行的行索引由输入index指定。算子输出output第i行数据计算公式为：  
  
  ```
  output[i] = input[index[i]]
  ```

- 算子规格：  
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">gather</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">M, N</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">index</td><td align="center">m (m < M, m < 65535 * 2048)</td><td align="center">uint32_t</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">output</td><td align="center">m, N</td><td align="center">float</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">gather_custom</td></tr>
  </table>

- 数据切分：  
  * gridDim：根据具体输入shape动态分配，最大不超过65535
  * blockDim：根据具体输入shape动态分配，最大不超过2048
  * 单线程处理：1行
  * 最大处理能力：65535 * 2048= 134215680行

- 算子实现：  
  gather算子的实现流程为从输入input（Global Memory）中获取指定索引的数据。基于上述数据切分，首先计算线程应处理数据的索引，然后通过赋值操作将一行数据存储到Global Memory上。由于计算过程相对简单，设置核函数的最大线程数限制为2048。

- 调用实现：  
  使用内核调用符<<<>>>调用核函数。

### 2. gatherv2算子
- 算子功能：  
  gatherv2算子实现了从多维输入张量input中按照指定维度axis收集数据的功能，indices张量指定了要收集的索引位置。支持batch_dims批量处理模式， 让不同的 batch 使用不同的索引集合。
  例如输入张量 input 的 shape 为 (2, 2, 3, 2) 和索引张量 indices 的 shape 为 (2, 2)：
  ```
  input:
    [[[[ 1,  2],
      [ 3,  4],
      [ 5,  6]],

      [[ 7,  8],
        [ 9, 10],
        [11, 12]]],


    [[[13, 14],
      [15, 16],
      [17, 18]],

      [[19, 20],
        [21, 22],
        [23, 24]]]]
  
  indices:
    [[1, 2],
    [0, 1]]
  ```
  axis=2, batch_dims=1 的结果：
  ```
  output:
    [[[[ 3,  4],
      [ 5,  6]],

      [[ 9, 10],
      [11, 12]]],

    [[[13, 14],
      [15, 16]],

      [[19, 20],
      [21, 22]]]]
  ```
- 算子规格：    
  <table>
  <tr><td rowspan="1" align="center">算子类型</td><td colspan="4" align="center">gatherv2</td></tr>
  </tr>
  <tr><td rowspan="5" align="center">算子输入</td><td align="center">name</td><td align="center">data type</td><td align="center">format</td><td align="center">约束/说明</td></tr>
  <tr><td align="center">input</td><td align="center">float</td><td align="center">ND</td><td align="center">-</td></tr>
  <tr><td align="center">indices</td><td align="center">uint32_t / int32_t</td><td align="center">ND</td><td align="center">indices.shape[0:batch_dims] = input.shape[0:batch_dims]</td></tr>
  <tr><td align="center">axis</td><td align="center">int32_t</td><td align="center">-</td><td align="center">batch_dims <= axis < input.rank</td></tr>
  <tr><td align="center">batch_dims</td><td align="center">int32_t</td><td align="center">-</td><td align="center">0 <= batch_dims <= min(input.rank, indices.rank)</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">output</td><td align="center">float</td><td align="center">ND</td><td align="center">output.shape = input.shape[0:batch_dims] + indices.shape[batch_dims:] + input.shape[axis+1:]</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">gather_custom_v2</td></tr>
  </table>


## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量   
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  ./gather                      # 执行样例
  ./gatherv2
  ```
  执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```
