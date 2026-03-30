# HelloWorld样例

## 概述

本样例通过使用<<<>>>内核调用符来完成样例核函数在NPU侧运行验证的基础流程，核函数内通过printf打印输出结果。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── hello_world_npu
│   ├── CMakeLists.txt      // 编译工程文件
│   └── hello_world.asc     // Ascend C样例实现 & 调用样例
```

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
  - NPU 模式（默认）
    ```bash
    mkdir -p build && cd build;
    cmake -DRUN_MODE=npu -DNPU_ARCH=dav-2201 ..;make -j;
    ./demo
    ```
  - CPU 调试模式
    ```bash
    mkdir -p build && cd build;
    cmake -DRUN_MODE=cpu -DSOC_VERSION=Ascend910B1 -DINSTALL_DIR=/usr/local/Ascend/cann ..;make -j;
    ./demo
    ```

  | 参数 | 说明 | 默认值 |
  |------|------|--------|
  | `RUN_MODE` | 运行模式：`npu` 或 `cpu` | `npu` |
  | `NPU_ARCH` | NPU 架构（npu 模式）：`dav-2201`（Atlas A2 系列）、`dav-3510`（Atlas A3 系列/Ascend 950 系列） | `dav-2201` |
  | `SOC_VERSION` | 芯片版本（cpu 模式）：如 `Ascend910B1` | `Ascend910B1` |
  | `INSTALL_DIR` | CANN 安装路径（cpu 模式） | `/mnt/workspace/gitCode/cann/Ascend/cann` |

  执行结果如下，说明执行成功。
  ```bash
  [Block (0/8)]: Hello World!!!
  [Block (1/8)]: Hello World!!!
  [Block (2/8)]: Hello World!!!
  [Block (3/8)]: Hello World!!!
  [Block (4/8)]: Hello World!!!
  [Block (5/8)]: Hello World!!!
  [Block (6/8)]: Hello World!!!
  [Block (7/8)]: Hello World!!!
  ```