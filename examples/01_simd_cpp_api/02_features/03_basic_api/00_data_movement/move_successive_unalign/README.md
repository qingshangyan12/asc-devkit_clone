# move_successive_unalign样例

## 概述
本样例在SIMD场景下，基于RegBase编程范式演示连续非对齐搬运样例的核函数直调方法，支持单核运行。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── move_successive_unalign
│   ├── CMakeLists.txt                 // 编译工程文件
│   └── vector_add.asc                 // AscendC样例实现 & 调用样例
```

## 样例功能
- 样例功能：

  输入一个长度为1024的向量，在源操作数和目的操作数地址均非32B对齐的情况下，搬运128个float类型数据。数据流路径为GM(Global Memory)→UB(Unified Buffer)→RegTensor→UB(Unified Buffer)→GM(Global Memory)。
- 样例规格：
  <table border="2" align="center">
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[1024]</td><td align="center">float</td></tr>
  <tr><td rowspan="2" align="center">样例输出</td></tr>
  <tr><td align="center">y</td><td align="center">[1024]</td><td align="center">float</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">move_successive_unalign_kernel</td></tr>
  </table>
- 样例实现：

  主要包括以下步骤：
  1. 初始化参数
  2. 将数据从GM(Global Memory)搬运到UB(Unified Buffer)
  3. 将UB(Unified Buffer)上源操作数和目的操作数地址偏移一个元素（4 Byte）长度，使得起始地址非32B对齐，调用CopyVF从该地址开始搬运128个float数，写回UB(Unified Buffer)
  4. 将数据从UB(Unified Buffer)搬运回GM(Global Memory)

- 调用实现

  使用内核调用符<<<>>>调用核函数。
    
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
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake ..;make -j;                                                         # 编译工程
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```