# 用户自定义TBufPool样例

## 概述

本样例基于EXTERN_IMPL_BUFPOOL宏辅助用户自定义TBufPool，适用于开发者有自定义内存块分配需求的场景。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── customized_tbuf_pool
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本
│   │   └── verify_result.py    // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── customized_tbuf_pool.asc // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能

  本样例以Add计算为例展示自定义TBufPool使用方法，适用于以下场景：
  1. 内存资源复用：内存资源有限时，手动指定UB（Unified Buffer）/L1（L1 Buffer）内存资源复用。
  2. 自定义的内存块分配需求：非连续内存块、内存块在不同TQue之间共享。

- 样例规格

  <table>
    <tr>
      <td align="center">类别</td>
      <td align="center">name</td>
      <td align="center">shape</td>
      <td align="center">data type</td>
      <td align="center">format</td>
    </tr>
    <tr>
      <td rowspan="2" align="center">样例输入</td>
      <td align="center">x</td>
      <td align="center">[1, 65536]</td>
      <td align="center">half</td>
      <td align="center">ND</td>
    </tr>
    <tr>
      <td align="center">y</td>
      <td align="center">[1, 65536]</td>
      <td align="center">half</td>
      <td align="center">ND</td>
    </tr>
    <tr>
      <td align="center">样例输出</td>
      <td align="center">z</td>
      <td align="center">[1, 65536]</td>
      <td align="center">half</td>
      <td align="center">ND</td>
    </tr>
    <tr>
      <td align="center">核函数名</td>
      <td colspan="4" align="center">customized_tbuf_pool_custom</td>
    </tr>
  </table>

- 样例实现

  - Kernel实现

    - 使用EXTERN_IMPL_BUFPOOL宏实现自定义TBufPool类MyBufPool。
    
    - 调用TPipe::InitBufPool接口为MyBufPool划分BUF_SIZE * 3大小的内存，并通过MyBufPool的InitBuffer接口实现TQue和TBuf的内存分配。

    - 调用DataCopy基础API，将数据从GM（Global Memory）搬运到UB（Unified Buffer）。

    - 调用Add接口，对两个输入tensor执行加法操作。

    - 调用DataCopy基础API，将计算结果从UB（Unified Buffer）搬运至GM（Global Memory）。

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
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;    # 编译工程，默认npu模式
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|---------|--------|
  | CMAKE_ASC_RUN_MODE | 运行模式 | npu, cpu, sim | npu |
  | CMAKE_ASC_ARCHITECTURES | NPU硬件架构 | dav-2201, dav-3510 | dav-2201 |

- 执行结果

  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```
