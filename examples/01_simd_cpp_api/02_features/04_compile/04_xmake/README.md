# xmake构建Add算子样例

## 概述

本样例介绍如何使用xmake的Ascend C toolchain构建并运行Add算子。样例采用**C_API**实现核函数，通过`xmake.lua`声明`ascendc`工具链与NPU架构，使用`xmake`完成编译，使用`xmake run xmake_add`运行可执行程序。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| ---- | ------------ |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

> **说明：** 本样例采用membase同步C_API（`asc_copy_*_sync` / `asc_add_sync`），产品支持范围与[C_API同步Add样例](../../../../02_simd_c_api/00_introduction/01_add/c_api_sync_add)一致。Ascend 950PR/Ascend 950DT请参考RegBase C_API样例。

## 目录结构介绍

```text
├── 04_xmake
│   ├── xmake.lua      // xmake工程文件
│   ├── add.asc        // Ascend C C_API Add算子实现与调用样例
│   └── README.md      // 样例说明
```

## 样例描述

- 样例功能：

  Add计算公式：

  ```text
  z = x + y
  ```

  样例使用8个核完成计算，每个核处理2048个`float`元素，数据总量为`8 × 2048 = 16384`。

- 样例实现：

  - Kernel实现：

    C_API输入数据需要先搬运进片上存储，再使用计算接口完成加法，最后搬出到外部存储。流程分为三步：

    1. 将Global Memory（`__gm__`）上的输入`x`、`y`搬运到Unified Buffer（`__ubuf__`）中的`xLocal`、`yLocal`；
    2. 对`xLocal`、`yLocal`执行加法，结果写入`zLocal`；
    3. 将`zLocal`搬运回Global Memory上的输出`z`。

    核函数入口调用`asc_init()`初始化运行时状态；数据搬运与计算使用同步接口（`asc_copy_gm2ub_sync` / `asc_add_sync` / `asc_copy_ub2gm_sync`），确保流水完成后再进入下一步。实现风格参考[C_API同步Add样例](../../../../02_simd_c_api/00_introduction/01_add/c_api_sync_add)。

  - 调用实现：

    Host侧在同一个`add.asc`文件中生成输入数据，使用内核调用符`<<<>>>`调用核函数，ACL接口失败时直接抛出异常，结果拷回后完成精度校验。

## xmake工程写法说明

本样例的核心构建脚本为`xmake.lua`，主要配置如下：

```lua
-- 通过option暴露NPU架构，避免在脚本中硬编码
option("npu_arch")
    set_default("dav-2201")
    set_showmenu(true)
    set_description("NPU architecture for this sample (dav-2201 for Atlas A2/A3)")
option_end()

add_rules("mode.debug", "mode.release")

target("xmake_add")
    set_kind("binary")              -- 生成可执行文件
    set_toolchains("ascendc")       -- 使用Ascend C工具链（bisheng）
    add_files("add.asc")            -- 添加源文件
    add_ascnpuarchs(get_config("npu_arch"))  -- 传入NPU架构号
```

与同目录CMake样例相比，主要差异如下：

| 项目 | CMake样例（如`00_basic_compile`） | 本样例（xmake） |
| ---- | -------------------------------- | --------------- |
| 工程文件 | `CMakeLists.txt` | `xmake.lua` |
| 工具链加载 | `find_package(ASC)` + `project(... LANGUAGES ASC CXX)` | `set_toolchains("ascendc")` |
| 架构配置 | `-DCMAKE_ASC_ARCHITECTURES=dav-xxxx` | `xmake f --npu_arch=dav-xxxx` |
| 源文件声明 | `add_executable(demo ...)` | `target(...)` + `add_files(...)` |
| 编译命令 | `cmake .. && make -j` | `xmake` |
| 运行命令 | `./demo` | `xmake run xmake_add` |

`option` / `get_config` 的用法可参考[xmake配置选项文档](https://xmake.io/zh/guide/project-configuration/define-options.html)。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 安装xmake

  本样例要求xmake版本不低于3.0.9。Linux/macOS可执行以下命令安装；Windows及其他安装方式请参考[xmake安装说明](https://xmake.io/zh/guide/quick-start.html#安装)。

  ```bash
  # curl 一键安装（推荐）
  curl -fsSL https://xmake.io/shget.text | bash

  # 或使用官方安装脚本
  bash <(curl -fsSL https://xmake.io/shget.text)
  ```

  安装完成后执行`xmake --version`确认版本，并确保`~/.local/bin`（或安装目录）已加入`PATH`。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，选择对应配置环境变量的命令，确保可通过`ASCEND_HOME_PATH`或`ASCEND_TOOLKIT_HOME`找到CANN开发套件包。

  - 默认路径，root用户安装CANN软件包

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径`install_path`，安装CANN软件包

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  > **说明：** 下列命令以 `XMAKE_ROOT=y` 开头。xmake 出于安全考虑默认拒绝以root身份运行构建脚本，该变量用于显式放行，因此**仅在root用户下需要**。**普通用户不需要该变量**，直接执行 `xmake f --npu_arch=dav-2201`、`xmake`、`xmake run xmake_add` 即可。

  ```bash
  # 配置NPU架构（可选；默认dav-2201）
  XMAKE_ROOT=y xmake f --npu_arch=dav-2201

  # 编译
  XMAKE_ROOT=y xmake

  # 运行
  XMAKE_ROOT=y xmake run xmake_add
  ```

  各产品型号对应的架构版本号请通过[对应关系表](../../../../../docs/zh/guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md#npu-arch)查询。本样例当前支持`dav-2201`（Atlas A2/A3系列产品）。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | ---- | ------ | ---- |
  | `npu_arch` | `dav-2201`（默认） | NPU架构，通过命令行传入，避免在`xmake.lua`中硬编码 |

  查看全部配置项可执行：

  ```bash
  xmake f --help
  ```

- 执行结果

  执行结果如下，说明精度对比成功。

  ```text
  test pass!
  ```
