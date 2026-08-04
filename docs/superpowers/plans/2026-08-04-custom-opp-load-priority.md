# Custom OPP Load Priority Fix Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 修复默认路径安装或升级自定义算子包时空`config.ini`未注册当前vendor的问题，并统一`ASCEND_CUSTOM_OPP_PATH`的运行时查找语义。

**Architecture:** `install.sh`和`upgrade.sh`分别包含相同的`update_load_priority`函数，解析并规范化唯一的`load_priority`配置项，将当前vendor去重后置于首部。资料只修改与安装目的地、RUN包路径及动态库目录相关的错误表述；验证使用临时目录直接执行真实脚本，不新增测试文件。

**Tech Stack:** Bash、Markdown、Git、CMake算子包脚本

---

### Task 1: 隔离复现当前缺陷

**Files:**
- Read: `cmake/asc/fwk_modules/scripts/install.sh`
- Read: `cmake/asc/fwk_modules/scripts/upgrade.sh`

- [ ] **Step 1: 使用空config.ini执行当前install.sh**

在临时目录创建空的`${ASCEND_OPP_PATH}/vendors/config.ini`和最小`packages`目录，从临时工作目录执行仓库中的真实`install.sh`。

```bash
repo_root=$(git rev-parse --show-toplevel)
verify_root=$(mktemp -d)
trap 'rm -r "$verify_root"' EXIT
mkdir -p "$verify_root/default/opp/vendors" "$verify_root/default/packages"
: > "$verify_root/default/opp/vendors/config.ini"
(
    cd "$verify_root/default"
    ASCEND_OPP_PATH="$verify_root/default/opp" \
        bash "$repo_root/cmake/asc/fwk_modules/scripts/install.sh" --force
)
sed -n 'l' "$verify_root/default/opp/vendors/config.ini"
```

Expected before fix: `--force`跳过与本问题无关的共享库校验，installer exits `0`, prints `SUCCESS`, and `config.ini` remains empty.

- [ ] **Step 2: 使用load_priority空值执行当前upgrade.sh**

```bash
mkdir -p "$verify_root/upgrade/opp/vendors" "$verify_root/upgrade/packages"
printf 'load_priority=\n' > "$verify_root/upgrade/opp/vendors/config.ini"
(
    cd "$verify_root/upgrade"
    ASCEND_OPP_PATH="$verify_root/upgrade/opp" \
        bash "$repo_root/cmake/asc/fwk_modules/scripts/upgrade.sh"
)
sed -n 'l' "$verify_root/upgrade/opp/vendors/config.ini"
```

Expected before fix: upgrader exits `0`, prints `SUCCESS`, and the value remains empty.

### Task 2: 修复安装与升级脚本

**Files:**
- Modify: `cmake/asc/fwk_modules/scripts/install.sh`
- Modify: `cmake/asc/fwk_modules/scripts/upgrade.sh`

- [ ] **Step 1: 在两个脚本中增加一致的配置更新函数**

在`log`函数后增加：

```bash
update_load_priority() {
    local config_file="$1"
    local line_count=0
    local priority_line=""
    local priority_value=""
    local item=""
    local existing=""
    local duplicate=0
    local new_value=""
    local -a items=()
    local -a new_items=("${vendor_name}")

    line_count=$(grep -cE '^[[:space:]]*load_priority[[:space:]]*=' "${config_file}" 2>/dev/null)
    if [ "${line_count}" -gt 1 ]; then
        log "[ERROR] multiple load_priority entries found in ${config_file}"
        return 1
    fi

    if [ "${line_count}" -eq 1 ]; then
        priority_line=$(grep -m1 -E '^[[:space:]]*load_priority[[:space:]]*=' "${config_file}")
        priority_value=${priority_line#*=}
        IFS=',' read -r -a items <<< "${priority_value}"
        for item in "${items[@]}"; do
            item=$(printf '%s' "${item}" | tr -d '[:space:]')
            [ -z "${item}" ] && continue
            [ "${item}" = "${vendor_name}" ] && continue

            duplicate=0
            for existing in "${new_items[@]}"; do
                if [ "${item}" = "${existing}" ]; then
                    duplicate=1
                    break
                fi
            done
            [ "${duplicate}" -eq 0 ] && new_items+=("${item}")
        done
    fi

    new_value=$(IFS=,; echo "${new_items[*]}")
    if [ "${line_count}" -eq 0 ]; then
        printf 'load_priority=%s\n' "${new_value}" >> "${config_file}"
    else
        sed -i -E "0,/^[[:space:]]*load_priority[[:space:]]*=.*/s//load_priority=${new_value}/" "${config_file}"
    fi
    if [ $? -ne 0 ] || ! grep -Fxq "load_priority=${new_value}" "${config_file}"; then
        log "[ERROR] update load_priority in ${config_file} failed"
        return 1
    fi

    return 0
}
```

- [ ] **Step 2: 让install.sh统一调用函数**

保留文件创建和权限设置，将现有`found_vendors`、`found_vendor`和`sed`代码替换为：

```bash
config_file=${targetdir}/vendors/config.ini
if [ ! -f "${config_file}" ]; then
    touch "${config_file}"
    if [ $? -ne 0 ]; then
        log "[ERROR] create ${config_file} failed"
        exit 1
    fi
    apply_chmod "${config_file}" "640" "${INSTALL_FOR_ALL}"
    if [ $? -ne 0 ]; then
        exit 1
    fi
fi
update_load_priority "${config_file}"
if [ $? -ne 0 ]; then
    exit 1
fi
```

- [ ] **Step 3: 让upgrade.sh创建或更新配置并修正告警**

将权限告警缩减为检查目录权限，不再声称`ASCEND_CUSTOM_OPP_PATH`控制安装目录。将脚本末尾的配置处理替换为：

```bash
config_file=${targetdir}/vendors/config.ini
if [ ! -f "${config_file}" ]; then
    touch "${config_file}"
    if [ $? -ne 0 ]; then
        log "[ERROR] create ${config_file} failed"
        exit 1
    fi
fi
update_load_priority "${config_file}"
if [ $? -ne 0 ]; then
    exit 1
fi
```

- [ ] **Step 4: 执行语法检查**

```bash
bash -n cmake/asc/fwk_modules/scripts/install.sh
bash -n cmake/asc/fwk_modules/scripts/upgrade.sh
```

Expected: both commands exit `0` with no output.

### Task 3: 修正资料语义

**Files:**
- Modify: `docs/zh/guide/编程指南/高级编程/Aclnn算子工程化开发/编译与部署/基本流程.md`
- Modify: `docs/zh/guide/编程指南/高级编程/Aclnn算子工程化开发/编译与部署/算子动态库和静态库编译.md`
- Modify: `docs/zh/guide/编程指南/附录/FAQ/算子包部署时出现权限不足报错.md`

- [ ] **Step 1: 说明默认安装的注册机制**

将“框架会自动扫描并加载”改为：安装脚本会将当前vendor写入`$ASCEND_OPP_PATH/vendors/config.ini`的`load_priority`配置项，框架按照该配置加载默认路径中的算子包。

- [ ] **Step 2: 修正动态库目录和RUN包路径示例**

将“动态库libcust_opapi.so的绝对路径”改为“动态库libcust_opapi.so所在目录”，并将示例改为：

```bash
ASCEND_CUSTOM_OPP_PATH=<path1>/vendors/vendor_name1:<path2>/lib/:<path3>/vendors/vendor_name3:<path4>/lib/
```

- [ ] **Step 3: 修正权限FAQ**

使用当前安装脚本的告警文本，处理步骤说明`--install-path`控制安装目的地，安装后执行生成的`set_env.bash`，由该脚本配置运行时查找环境变量。

### Task 4: 隔离回归和资料审计

**Files:**
- Verify: `cmake/asc/fwk_modules/scripts/install.sh`
- Verify: `cmake/asc/fwk_modules/scripts/upgrade.sh`
- Verify: `docs/zh/guide/**/*.md`

- [ ] **Step 1: 使用Task 1相同命令验证空配置修复**

Expected after fix:

```ini
load_priority=customize
```

- [ ] **Step 2: 验证已有列表去重和优先级**

分别使用`load_priority=vendor1,vendor2`和`load_priority=vendor1,customize,vendor1,,vendor2`作为输入。

Expected:

```ini
load_priority=customize,vendor1,vendor2
```

- [ ] **Step 3: 验证损坏配置和指定路径安装**

两条`load_priority`应导致默认安装非零退出且不输出`SUCCESS`。带`--install-path=<temp>`执行时，不得修改`${ASCEND_OPP_PATH}/vendors/config.ini`。

- [ ] **Step 4: 审计环境变量语义**

```bash
rg -n "ASCEND_CUSTOM_OPP_PATH" cmake/asc/fwk_modules/scripts docs/zh/guide
```

Expected: 安装目录只由`--install-path`或默认`ASCEND_OPP_PATH`描述；`ASCEND_CUSTOM_OPP_PATH`只用于运行时查找自定义算子包或动态库。

- [ ] **Step 5: 执行最终检查**

```bash
bash -n cmake/asc/fwk_modules/scripts/install.sh
bash -n cmake/asc/fwk_modules/scripts/upgrade.sh
git diff --check
git status --short --branch
```

Expected: syntax checks and diff check exit `0`; status only contains the scoped script, document, design and plan changes.
