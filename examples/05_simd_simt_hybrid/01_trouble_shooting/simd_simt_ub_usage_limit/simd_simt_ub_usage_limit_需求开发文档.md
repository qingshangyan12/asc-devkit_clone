## 需求

开发 SIMD&SIMT 混合编程样例工程 `simd_simt_ub_usage_limit`，通过一个静态 UB 越界导致精度失败的故障场景和四个正确使用或边界研究场景，说明 MainScalar、SIMD VF、SIMT VF 对 Unified Buffer（UB）的使用方式、容量计算规则和越界定位方法。本工作项属于 SIMT 样例开发，各场景由编译选项 `SCENARIO_NUM=0~4` 独立选择，一次构建只编译并运行一个 Case。

## 背景

SIMD&SIMT 混合编程中，MainScalar、SIMD VF 和 SIMT VF 会共同使用单个 Vector Core 的 UB。开发者既需要区分静态 UB 与动态 UB 的分配方式，也需要理解 VF 栈预留、SIMT Data Cache、不同执行位置的静态声明以及 kernel launch 动态 UB 参数对可用空间的共同影响。

实际问题不仅表现为 UB 总量超过上限。即使静态 UB 与动态 UB 的申请总量合法，单个静态数组的错误下标仍可能越过自身边界并覆盖相邻动态 UB，造成无运行时错误但结果错误的静默精度问题。样例需要提供可复现故障、正确使用对照、逐 word 容量边界和统一排查步骤，帮助开发者从精度异常、启动错误和同步错误中定位 UB 越界。

## 实现方案

**方案**

1. 使用 `SCENARIO_NUM=0~4` 在编译期选择一个独立 Case；每次构建只编译和运行当前场景，默认选择 Case 1。
2. 样例采用 `__global__ __vector__` mixed kernel，由 MainScalar 按场景调用 SIMD VF 或 1024 线程的 SIMT VF；Host 通过 launch 参数配置动态 UB。
3. 静态 UB 在 MainScalar、SIMD VF 和 SIMT VF 中分别声明并累计占用；动态 UB 由 Host 在 kernel 启动时统一配置，三个执行位置分别声明不同名称的动态 UB 数组，并验证这些数组的首地址相同、对应同一块空间。
4. Case 0复现越界故障，Case 1~4分别提供正确用法和容量边界对照；Host 校验输出，并打印实际容量、首尾数据、精度结果或启动错误码。

**Case 0：复现静态 UB 越界导致的精度问题**

1. **目标**：复现 UB 总量未超限，但静态数组访问越过自身边界并破坏动态 UB 的精度故障。
2. **实现方法**：申请 4KB 静态 UB 和 8KB 动态 UB；SIMT VF 初始化动态输入后，MainScalar 向静态数组末尾之外写入 4B，再由 SIMT VF 完成逐元素加一计算。
3. **预期结果**：kernel 可正常启动和结束，但动态输入首元素被覆盖，最终仅 `output[0]` 与 Golden 不一致，并输出 `precision_check=FAILED (fault reproduced)`。
4. **执行结果**：故障按预期复现，关键输出如下：

```text
# static_bytes：MainScalar静态数组大小；dynamic_bytes：Host配置的两段动态UB总大小
# overflow_bytes：静态数组实际访问超出自身边界的字节数
[Case0] static_bytes=4096, dynamic_bytes=8192, overflow_bytes=4
# golden_first：output[0]正确值；actual_first：input[0]被踩后计算得到的实际值
[Case0] golden_first=0xc0000001, actual_first=0xdeadbef0
# mismatch_count：与Golden不一致的输出元素数量
[Case0] mismatch_count=1
# FAILED (fault reproduced)：精度校验失败，但表示预期的越界故障已成功复现
[Case0] precision_check=FAILED (fault reproduced)
```

**结论**：UB 申请总量合法不代表访问合法；越界可能不报运行时错误，而是静默破坏相邻数据，定位时必须检查每个指针的实际访问范围。

**Case 1：展示无 VF 场景的静态 UB用法**

1. **目标**：展示不调用 VF 时 MainScalar 使用静态 UB 的基本方式。
2. **实现方法**：MainScalar 申请并写入 4KB 静态数组，不调用 SIMD VF 或 SIMT VF，kernel 启动时动态 UB 配置为 0；将静态数组首尾值拷贝到 GM 供 Host 校验。
3. **预期结果**：静态 UB 首尾数据正确，程序输出 `Case 1 (no VF static UB): PASSED`。
4. **执行结果**：4KB 静态 UB 首尾数据校验正确，关键输出如下：

```text
# PASSED：4KB静态UB首尾值正确；本Case不调用VF，动态UB配置为0
Case 1 (no VF static UB): PASSED
```

**结论**：无 VF 场景可以只使用编译期分配的静态 UB，不需要额外配置动态 UB。

**Case 2：研究不同 VF 配置下的可用静态 UB 上限**

1. **目标**：比较无 VF、仅调用 SIMD VF、调用 SIMD VF 并关闭 VF 栈预留三种配置下，当前 kernel 可使用的静态 UB 上限。
2. **实现方法**：通过编译选项切换 VF 调用方式并调整`static_ub`大小；kernel 实际写满数组并校验首尾元素，以“当前值运行成功、增加 4B 启动失败”确定可用静态 UB 边界。本 Case 不配置动态 UB。
3. **预期结果**：无 VF 时`static_ub`上限为 `262112B`；仅调用 SIMD VF 且保留 VF 栈时为 `253920B`；关闭 VF 栈预留后为 `260064B`。三种场景均另有固定的 32B 静态结果区。
4. **执行结果**：三种场景的最大静态数组配置运行通过，超过对应边界 4B 后启动失败，关键输出如下：

```text
# static_bytes：可配置static_ub数组大小，不包含固定的32B静态结果区
# launch_ret/sync_ret：0表示启动/同步成功；107000表示UB资源超限导致kernel启动失败
# result：PASSED表示边界内首尾校验通过；FAILED是增加4B后的预期超限结果
# 不调用 SIMD VF
[Case2] static_bytes=262112, launch_ret=0, sync_ret=0, result=PASSED
[Case2] static_bytes=262116, launch_ret=107000, sync_ret=107000, result=FAILED

# 调用 SIMD VF，默认配置
[Case2] static_bytes=253920, launch_ret=0, sync_ret=0, result=PASSED
[Case2] static_bytes=253924, launch_ret=107000, sync_ret=107000, result=FAILED

# 调用 SIMD VF，关闭 VF 栈预留
[Case2] static_bytes=260064, launch_ret=0, sync_ret=0, result=PASSED
[Case2] static_bytes=260068, launch_ret=107000, sync_ret=107000, result=FAILED
```

**结论**：VF 调用及其栈预留会改变当前 kernel 的可用静态 UB 上限。由于 Case 2 未配置动态 UB，实测结果不能直接用于证明静态 UB 与动态 UB 组合场景的边界。关闭预留后还需考虑编译器额外占用，并确认 VF 不存在寄存器溢出。

**Case 3：验证 SIMT VF使用动态 UB的容量边界**

1. **目标**：验证仅包含 SIMT VF 时动态 UB 的配置方法和容量上限。
2. **实现方法**：Host 通过 `CASE3_DYNAMIC_BYTES` 设置 kernel 动态 UB；1024 个 SIMT 线程以 grid-stride 方式写满动态区，MainScalar 读取首尾元素并交由 Host 校验。
3. **预期结果**：保留 8KB VF 栈空间和至少 32KB Data Cache 后，32B 静态结果区之外的动态 UB 最大为 `221152B`；该值运行通过，增加 4B 至 `221156B` 时 kernel 启动失败并返回 `107000`。
4. **执行结果**：最大合法配置运行通过，增加 4B 后启动失败，关键输出如下：

```text
# max_dynamic_bytes：扣除32B静态结果区后的动态UB理论上限
# configured_dynamic_bytes：本次kernel launch实际配置的动态UB大小
# CASE3_DYNAMIC_BYTES=221152
[Case3] max_dynamic_bytes=221152, configured_dynamic_bytes=221152
# PASSED：SIMT VF写满动态区且首尾数据校验正确
Case 3 (SIMT VF dynamic UB): PASSED

# CASE3_DYNAMIC_BYTES=221156
[Case3] max_dynamic_bytes=221152, configured_dynamic_bytes=221156
# ret 107000：配置值超过上限4B，kernel启动失败
[ERROR] [Bisheng] Launch kernel failure! ret 107000
# FAILED：预期的容量超限结果
Case 3 (SIMT VF dynamic UB): FAILED
```

**结论**：包含 SIMT VF 时，静态 UB 与动态 UB 合计不超过 216KB；动态 UB 的启动配置还必须覆盖程序实际访问的最大地址。

**Case 4：验证混合编程中的 UB共用关系**

1. **目标**：展示 MainScalar、SIMD VF 和 SIMT VF 混合执行时静态 UB 与动态 UB 的共同使用关系。
2. **实现方法**：三者分别申请并访问静态数组，静态 UB 总量为 `7264B`；SIMT VF由负责静态数组末元素的线程自行读取并回传该值；Host 只配置一块动态 UB，MainScalar、SIMD VF、SIMT VF分别声明`main_dynamic_ub`、`simd_dynamic_ub`、`simt_dynamic_ub`，通过各自数组的首元素完成A→B→C交接，并分别读取动态区末元素。
3. **预期结果**：动态 UB 配置为 `213920B` 时，三处不同名称数组的首元素交接和末元素校验通过，证明它们首地址相同，并输出 `dynamic_handoff=PASSED`；增加 4B 至 `213924B` 时 kernel 启动失败并返回 `107000`。
4. **执行结果**：最大合法配置运行通过，增加 4B 后启动失败，关键输出如下：

```text
# static_total_bytes：MainScalar、SIMD VF、SIMT VF的静态UB累计值
# max_dynamic_bytes：216KB用户UB扣除全部静态UB后的动态UB上限
# configured_dynamic_bytes：本次kernel launch实际配置的动态UB大小
# CASE4_DYNAMIC_BYTES=213920
[Case4] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213920
# dynamic_handoff：三处通过各自声明的动态UB数组完成A→B→C交接，验证数组首地址相同
Case 4 usage: static_total_bytes=7264, dynamic_bytes=213920, dynamic_handoff=PASSED
# PASSED：静态UB累计值、动态UB边界、首尾访问和数据交接均正确
Case 4 (mixed UB): PASSED

# CASE4_DYNAMIC_BYTES=213924
[Case4] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213924
# ret 107000：配置值超过上限4B，kernel启动失败
[ERROR] [Bisheng] Launch kernel failure! ret 107000
# FAILED：预期的容量超限结果
Case 4 (mixed UB): FAILED
```

**结论**：不同执行位置声明的静态 UB 需要累加并从同一用户 UB 容量中扣除；MainScalar、SIMD VF、SIMT VF在不同位置分别声明动态 UB 数组时，这些数组的首地址相同，共同对应 Host 在 kernel 启动时配置的动态 UB 空间。

## 出口标准

- **版本**：主线交付，CANN 9.2.0 及以上。
- **产品与芯片**：文档声明支持 Ascend 950PR/Ascend 950DT，编译架构为 `dav-3510`；实际验收环境为 `ascend950pr_9599`。
- **场景完整性**：`SCENARIO_NUM=0~4` 均可独立配置和构建；一次构建只包含所选 Case，不依赖其它 Case 的输出或状态。
- **故障复现**：Case 0 稳定复现 4B静态 UB 越界覆盖动态 `input[0]`，仅产生一个不匹配元素，并输出 `precision_check=FAILED (fault reproduced)`。
- **正向验收**：Case 1 正常运行；Case 2 的三种通过配置、Case 3 的 221152B配置和 Case 4 的 213920B配置均输出 `PASSED`。
- **边界验收**：Case 2、Case 3、Case 4 的上限增加 4B后触发 kernel 启动失败；预期错误码为 107000，预期失败结果按边界验证通过处理。
- **容量说明**：README 与中英文资料明确说明 256KB物理 UB、8KB VF 栈预留、32KB SIMT Data Cache、216KB SIMT 用户 UB，以及混合场景下静态 UB累加、动态 UB共享的规则。
- **测试文档**：STC 覆盖 Case 0 故障复现、Case 1 基本用法、Case 2 三种静态边界、Case 3 和 Case 4 的边界内/边界外配置及编译参数校验；缺陷记录表保持空白供人工测试填写。
- **交付件**：`ub_usage_limit.asc` 实现 Case 0~4；`CMakeLists.txt` 提供架构、场景和容量编译选项；`README.md` 与 `README_en.md` 说明场景、容量口径、实测结果和定位方法；`simd_simt_ub_usage_limit_需求开发文档.md` 记录本需求；另行交付 STC Base。
