# SIMT栈空间大小配置样例

## 概述

本样例基于Ascend C SIMT编程方式实现一维 `float` 输入的逐元素计算，通过两个 case 展示未配置 SIMT 栈空间时的问题，以及通过 `aclInit` 读取配置文件增大 SIMT 每线程栈空间后的解决方法。

## 支持的产品

- Atlas 350 加速卡

## 目录结构

```text
00_stack_size_config/
├── CMakeLists.txt          # CMake编译文件
├── acl_stack_config.json   # aclInit读取的SIMT栈空间配置文件
├── stack_size_config.asc   # SIMT栈空间配置调用样例
└── README.md               # 样例说明文档
```

## 算子描述

- 算子功能：
  样例在每个 SIMT 线程内定义较大的线程私有局部数组，制造较高的栈空间需求。case1 调用 `aclInit(nullptr)`，不设置 SIMT 栈空间大小，用于复现栈空间不足导致的运行失败或挂起问题；case2 调用 `aclInit("acl_stack_config.json")` 设置 `simt_stack_size` 和 `simt_divergence_stack_size`，用于展示配置后同一 Kernel 可以正常完成计算。单个输出元素的计算公式为：

  ```text
  output[i] = sum(input[i] + k * 0.001), k = 0, 256, 512, ..., 1792
  ```

- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SIMT栈空间配置样例</td></tr>
  <tr><td rowspan="2" align="center">输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">128</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">输出</td><td align="center">output</td><td align="center">128</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel名称</td><td colspan="4" align="center">stack_size_kernel</td></tr>
  </table>

- 数据切分：
  * block数：1
  * 每个 block 线程数：128
  * 总线程数：128
  * 单线程处理：1个 `float` 输出元素，并在线程私有栈上使用 `2048 * sizeof(float)` 字节的局部数组
  * 结果预期：输出与 Host 侧按相同公式计算的 golden 数据一致

- 算子实现：
  Kernel 根据全局线程号读取一个输入元素，在线程私有局部数组中写入 2048 个临时 `float` 数据，然后按固定步长累加局部数组中的部分元素并写入输出。Host 侧提供 `--case1` 和 `--case2` 两条执行路径：`--case1` 不传 ACL 配置文件，`--case2` 传入 ACL 配置文件并在 Kernel 执行后拷回输出，与 golden 数据比较精度。

- 调用实现：
  case1 在初始化 ACL 时传入 `nullptr`，不设置 SIMT 栈空间大小：

  ```cpp
  aclInit(nullptr);
  stack_size_kernel<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>(
      input_device, output_device, total_length);
  ```

  case2 在初始化 ACL 时传入 `acl_stack_config.json`，通过该配置文件设置 SIMT 栈空间大小，再使用内核调用符 `<<<>>>` 启动Kernel。

  ```cpp
  aclInit("acl_stack_config.json");
  stack_size_kernel<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>(
      input_device, output_device, total_length);
  ```

  `acl_stack_config.json` 中的关键配置如下：

  ```json
  {
    "StackSize": {
      "simt_stack_size": 16384,
      "simt_divergence_stack_size": 1024
    }
  }
  ```

  其中 `simt_stack_size` 用于设置 SIMT 算子每个线程的栈空间大小，`simt_divergence_stack_size` 用于设置 SIMT 算子的分支栈空间大小，单位均为 Byte，取值需为 128 的整数倍。

## 编译运行

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

  默认路径，root用户安装CANN软件包：

  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

  默认路径，非root用户安装CANN软件包：

  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

  指定路径install_path，安装CANN软件包：

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build                     # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..    # 配置工程
  make -j                                        # 编译工程
  ./demo                                         # 执行case2，默认读取build目录下的acl_stack_config.json
  ```

  如需指定其他 ACL 配置文件路径，可执行：

  ```bash
  ./demo --case2 /path/to/acl_stack_config.json
  ```

  如需单独复现未配置栈空间大小的问题，可执行 case1。该路径可能返回运行错误，也可能在 Kernel 同步阶段挂起，建议使用 `timeout` 避免命令长时间不返回：

  ```bash
  timeout 20s ./demo --case1
  ```
