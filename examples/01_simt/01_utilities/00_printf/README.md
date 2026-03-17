# 基于gather算子的SIMT时间戳打点功能实现样例

## 概述

本样例演示在SIMT编程下使用```printf()```接口实现上板打印进行功能调试的方法。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构

```
├── 00_printf
│   ├── CMakeLists.txt         # cmake编译文件
│   ├── printf.asc             # Ascend C算子实现加printf打印的调用样例
|   └── README.md
```

## 算子描述

- 算子功能:

  本样例详细展示了在SIMT实现函数中使用```printf()```接口的实践方式，实现对算子执行过程中打印每个线程的变量信息。


- 算子实现:
  ```cpp
  __simt_vf__ __launch_bounds__(THREAD_COUNT) inline void simt_printf(
    __gm__ float* input,
    __gm__ uint32_t* index,
    __gm__ float* gather_output,
    uint32_t in_shape0,
    uint32_t in_shape1,
    uint32_t index_total_length)
{
    printf("SIMT print index_total_length: %u\n", index_total_length);
    printf("SIMT print index addr: %p\n", index);
    printf("SIMT print input[0]:  %f\n", *input);
}
  ```

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
  ./demo                        # 执行样例
  ```
  执行后有如下打印信息，说明打印功能正常。
  ```
  SIMT print index_total_length: 12288
  ...
  SIMT print input[0]: 1.118000.
  ```
