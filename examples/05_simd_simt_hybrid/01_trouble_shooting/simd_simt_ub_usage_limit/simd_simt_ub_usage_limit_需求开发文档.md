## 需求

开发 SIMD&SIMT 混合编程样例工程 `simd_simt_ub_usage_limit`，通过一个静态 UB 越界导致精度失败的故障场景和四个正确使用或边界研究场景，说明 MainScalar、SIMD VF、SIMT VF 对 Unified Buffer（UB）的使用方式、容量计算规则和越界定位方法。本工作项属于 SIMT 样例开发，各场景由编译选项 `SCENARIO_NUM=0~4` 独立选择，一次构建只编译并运行一个 Case。

## 背景

SIMD&SIMT 混合编程中，MainScalar、SIMD VF 和 SIMT VF 会共同使用单个 Vector Core 的 UB。开发者既需要区分静态 UB 与动态 UB 的分配方式，也需要理解 VF 栈预留、SIMT Data Cache、不同执行位置的静态声明以及 kernel launch 动态 UB 参数对可用空间的共同影响。

实际问题不仅表现为 UB 总量超过上限。静态数组的错误下标可能跨过用户内存和预留空间后落入 SIMT Data Cache，破坏SIMT访问GM的缓存数据，造成无运行时错误但结果错误的静默精度问题。样例需要提供可复现故障、修复对照、容量边界和统一排查步骤，帮助开发者从精度异常、启动错误和同步错误中定位 UB 越界。

## 实现方案

**方案**

1. 使用 `SCENARIO_NUM=0~4` 在编译期选择一个独立 Case；每次构建只编译和运行当前场景，默认选择 Case 1。
2. 样例采用 `__global__ __vector__` mixed kernel，由 MainScalar 按场景调用 SIMD VF 或 1024 线程的 SIMT VF；Host 通过 launch 参数配置动态 UB。
3. 静态 UB 在 MainScalar、SIMD VF 和 SIMT VF 中分别声明并累计占用；动态 UB 由 Host 在 kernel 启动时统一配置，三个执行位置分别声明不同名称的动态 UB 数组，并验证这些数组的首地址相同、对应同一块空间。
4. Case 0通过编译选项切换Data Cache越界故障与边界检查修复，Case 1~4分别提供正确用法和容量边界对照；Host 校验输出，并打印实际容量、首尾数据、精度结果或启动错误码。

**Case 0：复现并修复静态 UB 越界破坏Data Cache的精度问题**

1. **目标**：使用简单逐元素加一算子，复现静态 UB错误访问落入SIMT Data Cache后造成的静默精度异常，并提供边界检查修复。
2. **实现方法**：kernel申请216KB静态UB，不配置动态UB，保留8KB空间和32KB Data Cache；SIMT VF先读取GM输入建立缓存，MainScalar再通过静态数组起始地址之后224KB的错误下标写入Data Cache，随后SIMT VF再次读取GM并计算。`CASE0_ENABLE_FIX`开启时，在写入前检查下标是否小于静态数组元素数，阻止非法访问。
3. **预期结果**：问题模式至少出现一个Golden不匹配元素并输出`precision_check=FAILED (fault reproduced)`，具体错误下标允许变化；修复模式输出`mismatch_count=0`和`precision_check=PASSED (out-of-bounds write blocked)`。
4. **执行结果**：问题与修复模式均按预期运行，关键输出如下：

```text
# 问题模式：faulty_byte_offset=229376表示从静态数组起点偏移224KB，落入Data Cache起点
[Case0] mode=fault, static_bytes=221184, dynamic_bytes=0, reserved_bytes=8192, dcache_bytes=32768, faulty_byte_offset=229376
# 错误下标受Data Cache映射状态影响；actual为缓存被覆盖后参与加一计算的结果
[Case0] mismatch_count=1
[Case0] first_mismatch_index=896, expected=0xc0000381, actual=0xdeadbef0
[Case0] precision_check=FAILED (fault reproduced)

# 修复模式：边界检查阻止同一个错误下标执行写入
[Case0] mode=fixed, static_bytes=221184, dynamic_bytes=0, reserved_bytes=8192, dcache_bytes=32768, faulty_byte_offset=229376
[Case0] mismatch_count=0
[Case0] precision_check=PASSED (out-of-bounds write blocked)
```

**结论**：静态 UB越界可能跨过预留空间并静默破坏Data Cache，导致SIMT读取GM时出现不稳定的精度异常；定位时必须结合UB布局检查实际访问地址，修复时应校验运行时下标。

**Case 1：研究无SIMT场景的可用UB大小**

1. **目标**：比较不调用VF、调用SIMD VF、调用SIMD VF并关闭VF栈预留三种配置下的可用UB大小。
2. **实现方法**：使用静态数组作为容量探针并实际访问首尾元素，以“当前值成功、增加4B启动失败”定位边界。
3. **执行结果**：无VF、SIMD VF默认配置、关闭VF栈预留时，探针上限分别为`262112B`、`253920B`、`260064B`；三种kernel均另含32B结果区。
4. **结论**：VF调用和预留空间会改变可用UB大小，关闭预留后仍需确认VF没有寄存器溢出。

**Case 2：研究有SIMT场景的可用UB大小**

1. **目标**：使用静态和动态两种容量探针验证有SIMT VF时的相同UB上限。
2. **实现方法**：`CASE2_UB_BYTES`配置探针大小，`CASE2_USE_STATIC_UB`选择声明方式；SIMT线程写满探针，Host校验首尾元素。
3. **执行结果**：扣除固定32B结果区后，两种探针均在`221152B`通过，在`221156B`启动失败并返回`107000`。
4. **结论**：默认配置下包含SIMT VF时用户静态UB与动态UB总量不超过216KB。

**Case 3：验证混合编程中的UB共用关系**

1. **目标**：验证MainScalar、SIMD VF和SIMT VF的静态UB累加，以及不同位置声明的动态UB首地址相同。
2. **实现方法**：三处静态UB合计`7264B`；三处分别声明不同名称的动态数组，回传首地址并通过首尾数据交接验证共享关系。
3. **执行结果**：动态UB为`213920B`时三个地址均为`0x1c60`且校验通过，增加4B至`213924B`时启动失败并返回`107000`。
4. **结论**：不同执行位置的静态UB分别占用并累加；动态UB全局只有一份，三个执行位置取得相同首地址。

**Case 4：观察小静态UB越界进入不同UB区域的结果**

1. **目标**：从4KB静态数组出发，比较越界写入未申请UB、6KB VF栈预留、2KB Ascend C API预留和Data Cache时的表现。
2. **实现方法**：不配置动态UB。Data Cache取128KB上限，因此布局依次为4KB静态UB、`[4KB,120KB)`未申请UB、`[120KB,126KB)`VF栈预留、`[126KB,128KB)`Ascend C预留和`[128KB,256KB)`Data Cache。`CASE4_ACCESS_REGION=0~4`分别选择合法静态区和上述四类目标。
3. **执行结果**：合法静态区、未申请UB、6KB预留和2KB预留场景均正常结束且无精度误差；Data Cache场景正常结束但产生1个Golden不匹配，首个错误为`index=896`、实际值`0xDEADBEF0`。
4. **结论**：越界是否产生可见异常取决于目标区域是否承载有效数据。无报错或本次精度通过不代表访问合法；6KB与2KB区域用途不同，必须分别分析。

## 出口标准

- **版本**：主线交付，CANN 9.2.0 及以上。
- **产品与芯片**：文档声明支持 Ascend 950PR/Ascend 950DT，编译架构为 `dav-3510`；实际验收环境为 `ascend950pr_9599`。
- **场景完整性**：`SCENARIO_NUM=0~4` 均可独立配置和构建；一次构建只包含所选 Case，不依赖其它 Case 的输出或状态。
- **故障复现与修复**：Case 0问题模式稳定复现静态UB错误访问落入Data Cache并产生精度不匹配，输出`precision_check=FAILED (fault reproduced)`；修复模式阻止越界写入，输出`mismatch_count=0`和`precision_check=PASSED (out-of-bounds write blocked)`。
- **正向验收**：Case 1三种边界内配置、Case 2静态/动态探针以及Case 3的213920B动态UB配置均运行通过。
- **边界验收**：Case 1、Case 2和Case 3的上限增加4B后触发kernel启动失败，预期错误码为107000；Case 4五个目标区域均能独立构建并输出启动、同步和精度观察结果。
- **容量说明**：README与中英文资料明确说明256KB物理UB、6KB VF栈预留、2KB Ascend C API预留、SIMT Data Cache及216KB SIMT用户UB，并说明混合场景下静态UB累加、动态UB共享的规则。
- **测试文档**：STC覆盖Case 0故障与修复、Case 1三种边界、Case 2静态/动态边界、Case 3混合共享关系，以及Case 4合法静态区、未申请UB、6KB预留、2KB预留和Data Cache五种目标；缺陷记录表保持空白供人工测试填写。
- **交付件**：`ub_usage_limit.asc` 实现 Case 0~4；`CMakeLists.txt` 提供架构、场景和容量编译选项；`README.md` 与 `README_en.md` 说明场景、容量口径、实测结果和定位方法；`simd_simt_ub_usage_limit_需求开发文档.md` 记录本需求；另行交付 STC Base。
