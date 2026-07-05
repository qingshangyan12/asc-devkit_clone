# xmake构建Add算子样例

## 概述

本样例介绍如何使用xmake的Ascend C toolchain构建并运行Add算子。样例通过`xmake.lua`直接声明`ascendc`工具链，使用`xmake`完成编译，使用`xmake run xmake_add`运行可执行程序。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```text
├── xmake_add
│   ├── xmake.lua      // xmake工程文件
│   ├── add.asc        // Ascend C Add算子实现与调用样例
│   └── README.md      // 样例说明
```

## 样例描述

- 样例功能：

  Add计算公式：

  ```python
  z = x + y
  ```

- 样例实现：

  - Kernel实现
    将Global Memory上的输入数据搬运到Local Memory，执行Add计算后再搬回Global Memory。

  - 调用实现
    Host侧生成固定输入数据，使用内核调用符`<<<>>>`调用核函数，并在结果拷回后完成精度校验。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请确保当前环境已安装xmake，且xmake可使用`ascendc` toolchain。请根据当前环境上CANN开发套件包的安装方式配置环境变量，确保xmake可以通过`ASCEND_HOME_PATH`或`ASCEND_TOOLKIT_HOME`找到CANN SDK。

  默认路径，root用户安装CANN软件包时，可执行：

  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

  默认路径，非root用户安装CANN软件包时，可执行：

  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

  指定路径install_path安装CANN软件包时，可执行：

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

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

  ```bash
  test pass!
  ```
