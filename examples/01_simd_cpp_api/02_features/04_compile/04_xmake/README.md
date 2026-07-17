# xmake构建Add算子样例

## 概述

本样例介绍如何使用xmake的Ascend C toolchain构建并运行Add算子。样例通过`xmake.lua`直接声明`ascendc`工具链，使用`xmake`完成编译，使用`xmake run xmake_add`运行可执行程序。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| ---- | ------------ |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```text
├── 04_xmake
│   ├── xmake.lua      // xmake工程文件
│   ├── add.asc        // Ascend C Add算子实现与调用样例
│   └── README.md      // 样例说明
```

## 样例描述

- 样例功能：

  Add计算公式：

  ```text
  z = x + y
  ```

- 样例实现：

  - Kernel实现：

    每个Block将Global Memory上的输入数据搬运到Unified Buffer，执行Add计算后再将结果搬回Global Memory。数据搬运与计算阶段之间调用`asc_sync`完成流水同步。

  - 调用实现：

    Host侧在同一个`add.asc`文件中生成固定输入数据，使用内核调用符`<<<>>>`调用核函数，并在结果拷回后完成精度校验。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 安装xmake

  本样例要求xmake版本不低于3.0.9。执行以下命令安装xmake，其他安装方式请参考[xmake安装说明](https://xmake.io/zh/guide/quick-start.html#安装)。

  ```bash
  curl -fsSL https://xmake.io/shget.text | bash
  ```

- 配置环境变量

  请根据当前环境中CANN Toolkit的安装方式配置环境变量，确保xmake可以通过`ASCEND_HOME_PATH`或`ASCEND_TOOLKIT_HOME`找到CANN Toolkit。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN Toolkit安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`目录。

- 样例执行

  ```bash
  xmake
  xmake run xmake_add
  ```

- 编译选项说明

  `xmake.lua`中通过如下配置指定Ascend C工具链和NPU架构：

  ```lua
  set_toolchains("ascendc")
  add_ascnpuarchs("dav-2201")
  ```

  如需适配Ascend 950PR/Ascend 950DT，可将`dav-2201`修改为`dav-3510`。

- 执行结果

  执行结果如下，说明精度对比成功。

  ```text
  test pass!
  ```
