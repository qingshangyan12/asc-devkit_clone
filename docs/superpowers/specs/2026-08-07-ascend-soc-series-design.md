# ASCEND_SOC_SERIES 构建配置设计

## 目标

新增公共构建配置 `ASCEND_SOC_SERIES`，用于选择一个或多个 short SoC series。
新配置允许用户采用平台惯用的混合大小写写法，例如 `Ascend910B` 和
`Ascend950`，同时保证既有编译、打包、安装和运行时查找路径不发生变化。

`ASCEND_SOC_SERIES` 配置的是 series，不是 full SKU。例如 `Ascend950` 合法，
`Ascend950PR_9599` 则是非法用户输入。full SKU 仍是编译框架的内部细节，由既有
`SOC_MAP_EXT` 在调用 OPC 前根据 short series 映射得到。

算子 `op_host` 中的 `AddConfig` 同样允许平台惯用的混合大小写 short series。
opbuild 生成算子信息库时按大小写不敏感的方式匹配 AddConfig，并始终以小写
canonical series 生成文件名和信息库内容。

## 术语

- **short series**：构建、打包和运行时路径使用的平台系列简称，例如
  `ascend910b`、`ascend950`。
- **平台惯用写法**：同一 short series 的混合大小写展示形式，例如
  `Ascend910B`、`Ascend950`。本文不再将它严格称为英文排版意义上的 TitleCase。
- **canonical series**：对用户输入执行整体小写转换后的 short series。
- **full SKU**：传给 OPC 的具体芯片型号，例如 `Ascend950PR_9599`、
  `Ascend910B1`。

## 现有行为

`cmake/asc/fwk_modules/config.cmake` 定义 `ASCEND_COMPUTE_UNIT`，默认值为
`ascend910b`，并逐项使用小写 `ASC_VALID_SOC_LIST` 校验配置值。

校验后，同一组小写 series 会继续流经既有构建链路：

- opbuild 通过 `OPS_PRODUCT_NAME` 和 `--compute_unit` 接收 series；
- Kernel target 名和编译中间目录包含 series；
- 二进制和配置产物写入 `binary/<series>`；
- OPP 安装路径使用 `kernel/<series>` 和 `config/<series>`；
- opregistry 打包记录相同的小写 series path key；
- Kernel 编译通过 `SOC_MAP_EXT` 将 short series 映射为 full SKU，再通过
  `--soc_version` 传给 OPC。

当前 opbuild 的 cfg generator 使用小写 `compute_unit` 精确查找 AddConfig map。
如果算子写成 `AddConfig("Ascend950", config)`，该查找无法命中并会静默使用默认
配置。在未传入 `compute_unit` 的 `JOIN_OP_DEF` 场景，cfg generator 还会直接使用
AddConfig 原始 key 生成 `aic-<soc>-ops-info.ini`，从而可能产生混合大小写文件名。

## 配置契约

### ASCEND_SOC_SERIES 输入

`ASCEND_SOC_SERIES` 接受由一个或多个 short SoC series 组成的 CMake list。
入口对完整值执行 `string(TOLOWER)`；该操作不改变 CMake 的分号列表结构。

| 用户输入 | canonical 值 | 结果 |
| --- | --- | --- |
| `Ascend950` | `ascend950` | 接受 |
| `ASCEND910B` | `ascend910b` | 接受 |
| `Ascend910B;Ascend950` | `ascend910b;ascend950` | 接受 |
| `Ascend950PR_9599` | `ascend950pr_9599` | 拒绝，属于 full SKU |
| `Ascend910B1` | `ascend910b1` | 拒绝，属于 full SKU |
| 未知 series | 小写输入 | 拒绝 |

实现不得新增混合大小写合法表、full SKU 合法表或 full SKU 到 series 的反向映射。
既有 `ASC_VALID_SOC_LIST` 是构建入口判断合法 series 的唯一数据源。

### 使用方式

`ASCEND_SOC_SERIES` 支持与既有 `ASCEND_COMPUTE_UNIT` 相同的 CMake 配置方式。

方式一，在 `CMakePresets.json` 的 `cacheVariables` 中配置：

```json
"ASCEND_SOC_SERIES": {
    "type": "STRING",
    "value": "Ascend910B;Ascend950"
}
```

方式二，在 CMake configure 命令行通过 `-D` 配置；多值中的分号需要避免被 shell
解释：

```bash
cmake -S . -B build_out '-DASCEND_SOC_SERIES=Ascend910B;Ascend950'
```

方式三，在顶层 `CMakeLists.txt` 中直接设置。仓库现有算子工程样例在
`find_package(ASC REQUIRED)` 之后设置 `ASCEND_COMPUTE_UNIT`，新配置必须兼容
相同顺序：

```cmake
find_package(ASC REQUIRED)
set(ASCEND_SOC_SERIES Ascend910B Ascend950)
```

也允许在 `find_package(ASC REQUIRED)` 之前设置。无论采用哪种方式，都不得同时
定义 `ASCEND_SOC_SERIES` 和 `ASCEND_COMPUTE_UNIT`。

### AddConfig 输入

`op_host` 的 AddConfig key 仍表示 short series，但匹配不区分大小写。例如：

```cpp
this->AICore().AddConfig("Ascend950", config);
```

它与构建配置 `ASCEND_SOC_SERIES=Ascend950` 或 canonical
`ASCEND_COMPUTE_UNIT=ascend950` 匹配。opbuild 不修改 AddConfig 注册 API 的存储
结构，而是在消费 AddConfig map 时建立 canonical 小写视图。

AddConfig key 的语义仍是 short series，但本需求不收紧其既有非法值处理。此次改动
不新增 AddConfig 合法性校验，也不把 full SKU 反向映射为 short series。例如
`Ascend950PR_9599` 只会归一化为 `ascend950pr_9599`，仍按既有非法 AddConfig
路径处理，不会被当作合法的 `ascend950`。

同一个算子内，任意两个 AddConfig key 如果转小写后相同，opbuild 必须报错并终止
生成，无论两份配置内容是否一致。例如以下写法属于重复配置：

```cpp
this->AICore().AddConfig("ascend950", configA);
this->AICore().AddConfig("Ascend950", configB);
```

禁止依赖 map 顺序静默覆盖或选择其中一份配置。

### 兼容与互斥

- `ASCEND_SOC_SERIES` 与 `ASCEND_COMPUTE_UNIT` 只要同时定义，CMake configure
  就立即失败，不比较两者内容。
- 只定义 `ASCEND_SOC_SERIES` 时，将其小写值作为当前作用域内的
  `ASCEND_COMPUTE_UNIT`，供既有链路继续使用。
- 只定义 `ASCEND_COMPUTE_UNIT` 时保持原行为；本需求不放宽旧配置的大小写规则。
- 两者都未定义时，继续使用既有默认值 `ascend910b`。
- 在已有 CMake build 目录中切换新旧配置面，可能因 cache 残留导致两者同时定义。
  此时按互斥规则报错，调用方需要显式删除旧 cache 项或使用新的 build 目录。

## 设计

### 构建入口归一化

归一化和互斥检查封装为 CMake 内部公共解析函数，输出供当前消费作用域使用的
canonical `ASCEND_COMPUTE_UNIT`。解析需要覆盖两个时机：

1. `cmake/asc/fwk_modules/config.cmake` 加载时，处理 preset、命令行 `-D` 或在
   `find_package` 前设置的变量。
2. `npu_op_code_gen`、`npu_op_package_add` 等实际消费 SoC 配置的公共构建入口，
   再次解析调用方当前作用域中的变量，以支持在 `find_package` 后通过 `set()` 配置。

解析规则如下：

1. 检测 `ASCEND_SOC_SERIES` 与用户显式配置的 `ASCEND_COMPUTE_UNIT` 是否同时
   定义；若是则 configure 失败。
2. 仅定义 `ASCEND_SOC_SERIES` 时，使用 CMake `string(TOLOWER)` 转为小写，写入
   当前消费作用域的 `ASCEND_COMPUTE_UNIT`。
3. 未定义新旧配置时，在消费入口使用既有默认值 `ascend910b`。
4. 使用不变的 `ASC_VALID_SOC_LIST` 校验 canonical `ASCEND_SOC_SERIES` list。
5. 新配置非法时，错误信息明确指出只接受 SoC series、不接受 full SKU，并打印
   支持的 series 列表。
6. 旧 `ASCEND_COMPUTE_UNIT` 保持现有校验时机和大小写行为；本需求不借机收紧
   `find_package` 后设置旧变量的历史行为。

为可靠区分“用户显式配置”与默认值，模块加载阶段不应提前把默认值物化为公共
`ASCEND_COMPUTE_UNIT` cache 项。解析函数在新旧变量均未定义时惰性返回
`ascend910b`；解析得到的 canonical 值只赋给当前消费函数的局部
`ASCEND_COMPUTE_UNIT`。模块加载阶段只校验当时已经存在的用户输入，不把新配置
永久回写为旧公共变量。这样后续 `set(ASCEND_SOC_SERIES ...)` 仍能被识别为唯一输入，
而用户后续同时 `set()` 新旧变量时也能无歧义报错。

解析完成后不再引入新的下游接口。opbuild 仍接收 `--compute_unit`，Python 构建工具
仍接收 `--compute-unit`，打包逻辑仍消费 canonical `ASCEND_COMPUTE_UNIT`。

### opbuild AddConfig 归一化

opbuild 消费每个算子的 `GetAICoreConfigs()` 结果时，先建立 canonical 配置视图：

```text
canonical 小写 series -> {原始 AddConfig key, OpAICoreConfig}
```

构建该视图时：

1. 对每个 AddConfig key 执行 ASCII 小写转换。
2. 不新增合法性校验，非法 series 和 full SKU 保持既有忽略、默认回退或
   `SOC_VERSION_INVALID` 行为。
3. 若两个不同原始 key 的 canonical key 相同，则记录明确错误并使本次 opbuild
   生成失败；完全相同的原始 key 在 AddConfig 内部仍保持后一次覆盖前一次。
4. 保留原始 key，以便读取依赖原始注册 key 的 `OpAICoreConfig`、MC2 等配置。
5. 后续匹配、去重、支持列表和输出命名只使用 canonical key。

canonical 视图应作为 opbuild 内部的单一归一化结果复用，避免 cfg generator 与
ACLNN generator 分别实现不同的大小写和重复检测规则。

### cfg 算子信息生成

传入 `compute_unit` 时，cfg generator 使用小写 compute unit 查询 canonical 配置
视图。命中后必须使用对应的 AddConfig 自定义配置；未注册该 series 时才保持既有
默认配置回退行为。

未传入 `compute_unit` 时，cfg generator 遍历 canonical 配置视图生成算子信息，
输出文件统一命名为：

```text
aic-<canonical-series>-ops-info.ini
```

文件内容涉及 series 的字段也使用 canonical 小写值，不将 AddConfig 原始大小写写入
算子信息库。非法 AddConfig 继续沿用现有生成行为，仅输出名称被归一化为小写。

### ACLNN 算子信息生成

ACLNN generator 已在部分位置调用 `ToLower`，本次需统一改为复用同一 canonical
配置视图，以保证：

- AddConfig 支持列表中的 soc name 始终为小写 canonical series；
- compute unit 与 AddConfig 按大小写不敏感方式匹配；
- 同 series 的不同大小写 AddConfig 不会生成重复枚举或重复支持项，而是明确报错；
- 非法 AddConfig 继续 warning 并生成 `SOC_VERSION_INVALID`，不升级为硬错误；
- 仍可通过保存的原始 key 读取 MC2 和具体 `OpAICoreConfig`。

### 路径与 OPC 边界

```text
ASCEND_SOC_SERIES=Ascend950
        |
        | CMake string(TOLOWER)
        v
ASCEND_COMPUTE_UNIT=ascend950
        |
        | ASC_VALID_SOC_LIST
        v
既有编译/打包链路 -----------------------------+
        |                                       |
        | 小写目录与运行时 path key             | SOC_MAP_EXT
        v                                       v
.../<ascend950>/...                     Ascend950PR_9599 -> OPC

op_host AddConfig("Ascend950", config)
        |
        | opbuild canonical 配置视图
        v
AddConfig key=ascend950
        |
        +--> 匹配上述 compute unit
        +--> aic-ascend950-ops-info.ini
        +--> 算子信息库 soc name=ascend950
```

框架通过 `SOC_MAP_EXT` 选择的合法 full SKU 不影响目录名、算子信息库 series 或
运行时 path key。非法 AddConfig 不属于该映射链路；在未传 `compute_unit` 的场景
仍保持既有生成行为，只将其原始 key 统一转为小写。

## 错误处理

以下情况在生成构建目标前或生成算子信息库时失败：

- 新旧构建配置同时定义；
- 新配置归一化后不在 `ASC_VALID_SOC_LIST`；
- 配置 list 中任意一项非法；
- 同一算子存在 canonical series 相同的重复 AddConfig key。

AddConfig 使用 full SKU 或其他非法 series 时不新增错误：cfg generator 保持既有
忽略、默认回退或生成行为，ACLNN generator 保持 warning 和
`SOC_VERSION_INVALID` 行为。

显式传入空值时继续沿用现有 CMake list 语义，本需求不新增空值规则。

## 验证方案

### CMake 配置测试

1. `ASCEND_SOC_SERIES=Ascend950` 得到 canonical `ascend950`。
2. 合法 short series 的任意大小写均能通过新配置入口。
3. 多个混合大小写 series 能正确归一化并保持 list 结构。
4. `Ascend950PR_9599`、`Ascend910B1` 和未知 series 被白名单拒绝。
5. 新旧变量同时定义时始终失败，包括两者内容相同的情况。
6. 只使用旧配置时保持既有行为。
7. 两者均未配置时保持 `ascend910b` 默认值。
8. preset、命令行、`find_package` 前 `set()` 和 `find_package` 后 `set()` 四种入口
   得到相同 canonical 结果。
9. 通过任意入口显式配置新旧变量时均触发互斥错误。

### opbuild 单元测试

1. `compute_unit=ascend950` 能命中 `AddConfig("Ascend950", customConfig)`，生成
   内容使用自定义配置而不是默认配置。
2. 未传 `compute_unit` 时，混合大小写 AddConfig 生成
   `aic-ascend950-ops-info.ini`，不生成混合大小写文件名。
3. ACLNN 支持列表和 soc name list 输出小写 `ascend950`。
4. `AddConfig("ascend950")` 与 `AddConfig("Ascend950")` 同时存在时生成失败。
5. 单个合法 AddConfig 不因大小写不同而失败。
6. full SKU AddConfig 只转小写，不被转换为 short series，且保持既有软处理行为。
7. 非法 AddConfig 在 ACLNN 产物中仍为 `SOC_VERSION_INVALID`。
8. 完全相同的 AddConfig key 重复注册时仍由后一次覆盖前一次。
9. MC2 及依赖原始 AddConfig key 的配置仍能正确读取。

### 集成验证

1. 代表性构建中，target 名、中间 `binary/<series>` 目录、OPP 安装路径和
   opregistry path key 均只包含小写 series。
2. 使用混合大小写 AddConfig 的自定义配置实际进入生成的算子信息库。
3. OPC 命令仍从 `SOC_MAP_EXT` 获得 full SKU。
4. 既有全部小写 `ASCEND_COMPUTE_UNIT` 与 AddConfig 工程产物保持兼容。

## 不在本需求范围内

- 新增或修改 `ASC_VALID_SOC_LIST`、`SOC_MAP_EXT` 条目。
- 将用户配置的 full SKU 视为合法 short series。
- 根据 full SKU 反向推导 short series。
- 给 opbuild 或 Python 构建工具新增 `soc_series` 参数。
- 重命名既有 `compute_unit` API、变量、文件或元数据。
- 修改编译中间目录、OPP 安装目录或运行时二进制查找规则。
