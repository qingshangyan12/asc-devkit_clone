# asc_half2float.md 文档修复清单

> **源文件：** `Codex_vs_CannBot/CannBot/doc/3510/api/asc_half2float.md`
> **生成时间：** 2026-09-02
> **对应报告：** `asc_half2float_quality_report.md`
> **总分：** 79/100

---

## 修复项列表

### 🔴 严重（立即修复）

#### 修复1：新增"返回值说明"章节

- **位置：** asc_half2float.md:64（在"参数说明"章节后、"数据类型支持"章节前插入）
- **问题：** API有2个重载返回`vector_float`类型，但文档缺少"返回值说明"章节。
- **当前内容：** 无此章节
- **修复为：**
  ```markdown
  ## 返回值说明

  **表 2** 返回值说明

  | 返回值类型 | 含义 |
  |-----------|------|
  | vector_float | 转换后的float类型结果矢量。仅在返回值重载版本（无dst参数）中使用，结果存储在返回的矢量数据寄存器中。 |
  | void | 无返回值。仅在带dst参数的重载版本中使用，结果直接写入dst参数指定的矢量数据寄存器中。 |
  ```

---

### 🟠 重要（高优先级）

#### 修复2：补充地址对齐约束

- **位置：** asc_half2float.md:90（约束说明章节末尾追加）
- **问题：** 未说明配套数据搬运接口的对齐要求。
- **当前内容：**
  ```markdown
  - 由于源操作数与目的操作数类型位宽比为 1:2，必须通过 src_pos 参数指定读取源操作数索引的偶数位或奇数位。
  ```
- **在其后追加：**
  ```markdown
  - 调用本接口前通过`asc_loadalign`加载源数据时，源地址需按照32字节对齐；通过`asc_storealign`存储结果时，目的地址需按照32字节对齐。
  ```

#### 修复3：补充异常和边界值处理约束

- **位置：** asc_half2float.md:90（约束说明章节末尾追加）
- **问题：** 未说明half特殊值（inf/nan/0）转换为float后的结果。
- **在其后追加：**
  ```markdown
  - 特殊值处理：half类型的inf转换为float后仍为inf；nan转换为float后仍为nan；+0/-0转换为float后保持符号不变；half的MAX/MIN转换为float后对应float的MAX/MIN范围内值。
  ```

#### 修复4：补充元素数量范围约束

- **位置：** asc_half2float.md:90（约束说明章节末尾追加）
- **问题：** 未说明VL大小与元素数量的约束关系。
- **在其后追加：**
  ```markdown
  - 由于源操作数与目的操作数位宽比为1:2，当src_pos取EVEN时，读取src中索引为0、2、4...的元素；取ODD时读取索引为1、3、5...的元素。有效输出元素数量为VL对应float元素数量，输入half元素数量为输出float元素数量的2倍。
  ```

#### 修复5：补充互斥API说明

- **位置：** asc_half2float.md:146（关联API章节开头插入）
- **问题：** 缺少互斥API警告框。
- **当前内容：**
  ```markdown
  ## 关联API

  | 相似API | 与当前API区别 | 选择建议 |
  ```
- **在`## 关联API`后、表格前插入：**
  ```markdown
  > **注意**
  > 本接口无严格互斥API。但asc_half2float与asc_float2half为逆向操作，不可在同一矢量寄存器上同时调用，否则会导致数据竞争。
  ```

#### 修复6：补充前置知识段落和概念链接

- **位置：** asc_half2float.md:18（功能说明章节开头插入）
- **问题：** 首次提到AIV、VL、Vector Function等概念未加链接，无前置知识段落。
- **当前内容：**
  ```markdown
  ## 功能说明

  头文件为：`#include "c_api/reg_compute/reg_convert.h"`
  ```
- **修改为：**
  ```markdown
  ## 功能说明

  > **前置知识：** 阅读本文档前，建议先了解矢量编程模型和Vector Function概念。

  头文件为：`#include "c_api/reg_compute/reg_convert.h"`
  ```

---

### 🟡 一般（中优先级）

#### 修复7：删除中英文间多余空格（6处）

| 行号 | 原文 | 修复后 |
|------|------|--------|
| 22 | `类型位宽比为 1:2，读取数据时` | `类型位宽比为1:2，读取数据时` |
| 86 | `本接口在非 AIV 上调用直接返回` | `本接口在非AIV上调用直接返回` |
| 88 | `mask 需通过掩码设置接口预先赋值后再传入` | `mask需通过掩码设置接口预先赋值后再传入` |
| 89 | `mask 掩码位为 0 时，dst 对应元素置 0` | `mask掩码位为0时，dst对应元素置0` |
| 94 | `以下示例演示 asc_half2float 将 half 类型数据转换为 float 类型` | `以下示例演示asc_half2float将half类型数据转换为float类型` |
| 136 | `完整示例请参考 asc-devkit 仓库` | `完整示例请参考asc-devkit仓库` |

#### 修复8：补充专业术语首次出现解释

| 行号 | 原文 | 修复后 |
|------|------|--------|
| 22 | `本接口仅在 AIV 上生效` | `本接口仅在AIV（AI Vector，矢量计算核心）上生效` |
| 22 | `一个 VL 大小的数据` | `一个VL（Vector Length，矢量长度）大小的数据` |
| 70 | `TPosition: VREG` | `TPosition: VREG（Vector Register，矢量数据寄存器）` |

#### 修复9：修正HTML注释格式

- **位置：** asc_half2float.md:3
- **当前内容：**
  ```markdown
  <!-- 标签说明 -->
  ```
- **修复为：**
  ```markdown
  <!--
  文件序号: [序号]
  一级章节: 基础API
  二级章节: 数据转换
  起始行号: 1
  -->
  ```

#### 修复10：补充术语分层关联

- **位置：** asc_half2float.md:22
- **问题：** 高层章节直接使用底层硬件名AIV，未做抽象层关联。
- **当前内容：** `本接口仅在 AIV 上生效`
- **修复为：** `本接口仅在AIV（矢量计算核心，负责矢量计算指令执行）上生效`
- **说明：** 此修复与修复8合并执行。

#### 修复11：为src_pos参数补充示意图

- **位置：** asc_half2float.md:73（参数说明表格后插入）
- **问题：** src_pos的EVEN/ODD位置选择概念复杂，纯文字描述超过3句但无图。
- **修复操作：** 在参数说明表格后插入示意图，展示half元素如何按EVEN/ODD位置映射到float元素：
  ```
  src(half):  [h0] [h1] [h2] [h3] [h4] [h5] [h6] [h7]
                           ↓ EVEN              ↓ ODD
  dst(float): [f0=h0/h2]    [f1=h2/h3]    [f2=h4/h5]    [f3=h6/h7]
  ```
- **说明：** 建议使用正式图片替换ASCII示意图，展示VL大小的half数据如何按偶数/奇数索引拆分映射到float结果。

---

### 🟢 轻微（低优先级）

#### 修复12：补充代码关键字注释

- **位置：** asc_half2float.md:117
- **当前内容：**
  ```cpp
  __global__ __vector__ void asc_half2float_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
  ```
- **修复为：**
  ```cpp
  __global__ __vector__ void asc_half2float_kernel(__gm__ uint8_t* output, __gm__ uint8_t* input) // 全局矢量核函数入口
  ```

#### 修复13：补充Host/Kernel侧标注

- **位置：** asc_half2float.md:22（功能说明段落中补充）
- **当前内容：** `该接口根据 mask 将 src 中的每个参与计算的 half 类型元素转换为 float 类型`
- **修复为：** `本接口为Kernel侧接口。该接口根据 mask 将 src 中的每个参与计算的 half 类型元素转换为 float 类型`

#### 修复14：补充Minimal示例

- **位置：** asc_half2float.md:94（调用示例章节开头，现有示例前插入）
- **当前内容：** `以下示例演示 asc_half2float 将 half 类型数据转换为 float 类型，从偶数位读取源数据。`
- **在其后插入Minimal示例：**
  ```markdown
  最小调用示例：
  ```cpp
  vector_float dst;
  vector_half src;
  vector_bool mask = asc_create_mask_b8(PAT_ALL);
  asc_half2float(dst, src, mask, ASC_POSITION_EVEN); // 从偶数位读取，执行half转float
  ```
  ```

#### 修复15：删除示例中冗余的预加载操作

- **位置：** asc_half2float.md:111
- **问题：** `asc_loadalign(dst, ...)`向dst预加载float数据，但后续`asc_half2float(dst, ...)`会覆盖dst内容，预加载操作冗余。
- **当前内容：**
  ```cpp
  vector_float dst;
  vector_half src;
  asc_loadalign(dst, reinterpret_cast<__ubuf__ float*>(output)); // 从UB加载到矢量数据寄存器
  asc_loadalign(src, reinterpret_cast<__ubuf__ half*>(input));
  ```
- **修复为：**
  ```cpp
  vector_float dst;
  vector_half src;
  asc_loadalign(src, reinterpret_cast<__ubuf__ half*>(input)); // 从UB加载half源数据到矢量数据寄存器
  ```

---

## 无法自动修复的内容

| 项目 | 说明 | 原因 |
|------|------|------|
| 样例库链接 | 文档第136行提到"完整示例请参考asc-devkit仓库"但未提供具体链接 | 需确认asc-devkit仓库中是否存在对应的完整样例目录后补充链接 |
| 理论性能引用 | 文档无理论性能附录引用 | 需确认是否存在Vector类理论性能附录页面后补充引用链接 |
| 互斥API确认 | 修复5中标注的互斥关系需与开发团队确认 | 无法仅从代码层面确认asc_half2float与asc_float2half是否存在寄存器级互斥 |
| src_pos示意图 | 修复11中的示意图建议使用正式图片 | 需设计团队制作正式示意图替换ASCII文本 |
| 资源占用情况 | 需说明UB/寄存器资源占用和eventID使用 | 需与硬件规格文档核实具体资源占用数值 |
| 特殊值处理细节 | 修复3中的特殊值转换规则需与硬件实现核实 | half→float的特殊值转换行为需确认是否遵循IEEE 754标准 |

---

## 修复优先级汇总

| 优先级 | 修复编号 | 数量 | 预计影响 |
|--------|---------|------|---------|
| 🔴 严重 | 修复1 | 1 | 新增必需章节，结构完整性+3分 |
| 🟠 重要 | 修复2-6 | 5 | 补充约束覆盖率和可获取性，准确性+4分，规范性+3分 |
| 🟡 一般 | 修复7-11 | 5 | 格式规范和可读性提升，语言+3分，规范性+3分 |
| 🟢 轻微 | 修复12-15 | 4 | 细节完善，各维度小幅提升 |

**全部修复后预计得分：** ~92/100
