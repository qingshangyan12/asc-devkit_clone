# 自定义算子包load_priority更新修复设计

## 背景

默认路径安装自定义算子包时，安装脚本需要将当前vendor写入`${ASCEND_OPP_PATH}/vendors/config.ini`的`load_priority`配置项，使框架能够发现新安装的算子包。

当前脚本通过“移除当前vendor后是否还存在其他vendor”判断是否更新配置。当`config.ini`为空、缺少`load_priority`或配置为`load_priority=`时，计算出的剩余vendor为空，脚本会跳过更新并继续输出`SUCCESS`。算子文件虽然已经安装，但框架无法发现算子包。

## 目标

- 默认路径安装或升级后，当前vendor必须在`load_priority`首部且只出现一次。
- 可恢复的空配置由脚本自动修复。
- 配置损坏或写入失败时明确报错，不得输出`SUCCESS`。
- `ASCEND_CUSTOM_OPP_PATH`只表达运行时查找路径，不参与默认安装目录选择。
- 安装脚本、升级脚本和用户资料保持一致。

## 非目标

- 不改变`--install-path`指定目录安装的目录结构和环境变量加载方式。
- 不改变多个算子包之间既有的优先级规则。
- 不重构与`config.ini`更新无关的安装流程。

## 配置更新规则

| 原始状态 | 安装vendor_name1后的结果 |
| --- | --- |
| `config.ini`不存在 | `load_priority=vendor_name1` |
| 文件为空 | `load_priority=vendor_name1` |
| 不存在`load_priority`配置项 | 保留其他内容并追加`load_priority=vendor_name1` |
| `load_priority=` | `load_priority=vendor_name1` |
| `load_priority=vendor_name2,vendor_name3` | `load_priority=vendor_name1,vendor_name2,vendor_name3` |
| `load_priority=vendor_name2,vendor_name1,vendor_name3` | `load_priority=vendor_name1,vendor_name2,vendor_name3` |
| 存在多条`load_priority`配置项 | 报错并退出非零 |
| 配置文件不可写或更新失败 | 报错并退出非零 |

解析配置项时使用锚定表达式`^[[:space:]]*load_priority[[:space:]]*=`，忽略逗号分隔项两侧的空白，过滤空项并去重。除目标配置项外，不修改文件中的其他内容。

## 实现范围

### 安装与升级脚本

在`install.sh`和`upgrade.sh`中使用一致的`update_load_priority`逻辑：

1. 检查目标配置文件和`load_priority`配置项数量。
2. 解析并规范化已有vendor列表。
3. 将当前vendor放到列表首部。
4. 更新已有配置项或追加缺失的配置项。
5. 检查写入命令返回值，并校验最终配置内容。

两个脚本是算子包中的独立入口。本次在两个入口内保留同一小段函数实现，不新增运行时公共脚本依赖。

同时修正`upgrade.sh`中的权限告警：`--install-path`控制安装目录；`ASCEND_CUSTOM_OPP_PATH`仅用于运行时查找，不再描述为安装目录控制变量。

### 资料

修改以下内容：

- “基本流程”：说明默认安装通过`config.ini`中的`load_priority`发现算子包，安装脚本会注册当前vendor。
- “算子动态库和静态库编译”：将动态库“绝对路径”改为动态库所在目录，并为RUN包示例补充`vendors`目录层级。
- “算子包部署时出现权限不足报错”：删除使用`ASCEND_CUSTOM_OPP_PATH`指定安装目录的错误语义，改为使用`--install-path`安装并执行生成的`set_env.bash`配置运行时查找路径。

以下用法保持不变：

- “运行时加载机制”中的查找顺序。
- `optype_collector`扫描外部算子包的配置方式。
- ONNX适配插件通过该变量指定插件动态库目录的方式。

## 验证设计

不新增测试脚本。使用临时目录直接执行仓库中的真实`install.sh`和`upgrade.sh`，不修改本机CANN安装目录，也不向仓库写入验证数据。

验证场景包括：

- 文件不存在、空文件、空配置值和缺少配置项。
- 已有单个或多个vendor。
- 当前vendor已存在、重复出现及空项。
- 多条`load_priority`配置项。
- 配置写入失败。
- 指定`--install-path`时不修改默认路径的`config.ini`。

修改前先通过隔离命令确认Issue场景可以复现；修改后使用相同输入验证配置更新结果。

## 验证

- 运行隔离的安装与升级场景验证命令。
- 对修改的shell脚本执行`bash -n`。
- 检查所有`ASCEND_CUSTOM_OPP_PATH`资料引用，确认安装目的地和运行时查找语义已分离。
- 执行Markdown检查、`git diff --check`并确认分支仅包含本设计范围内的改动。
