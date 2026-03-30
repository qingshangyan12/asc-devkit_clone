# 重构规范详细参考

> 本文档为SKILL的详细补充，提供代码示例和检查清单。参考模板代码见 `set_atomic_add_refactor/` 目录。

---

## 一、重构要点（按文件汇总）

### 1.1 `{api_name}.asc`

| 要点 | 说明 |
|------|------|
| **多核→单核** | numBlocks>1改为1，适配地址偏移和tiling |
| **模板化** | 使用`template<typename T>`，GlobalTensor/LocalTensor/TQue类型参数化 |
| **SetSaturationMode** | API调用前设置饱和模式（区分浮点型/整型） |
| **内核函数** | 模板化 + 显式实例化支持多种类型 |
| **主函数** | 命令行参数解析(dtype, saturation)、类型分发switch |

**模板代码**：见 `set_atomic_add_refactor/set_atomic_add.asc`

---

### 1.2 `gen_data.py`

| 要点 | 说明 |
|------|------|
| 特殊值 | 浮点型：min, max, +0, -0, NaN, +INF, -INF；整型：min, max, +0, -0 |
| 单输入 | `generate_one_input()` → 7个(浮点)或4个(整型) |
| 双输入 | `generate_two_inputs()` → 49个(浮点)或16个(整型)组合 |
| Golden | 分别计算sat和non-sat两套结果 |

**模板代码**：直接拷贝 `set_atomic_add_refactor/scripts/gen_data.py`

---

### 1.3 `gen_data_utils.py`

| 函数 | 说明 |
|------|------|
| `get_dtype_limits()` | 获取各类型数值范围(min/max/0/NaN/INF) |
| `generate_one_input()` | 生成单目API测试数据 |
| `generate_two_inputs()` | 生成双目API测试数据组合 |
| `save_bin()` | 保存二进制文件 |
| `get_special_names()` | 获取特殊值名称列表 |

**模板代码**：直接拷贝 `set_atomic_add_refactor/scripts/gen_data_utils.py`

---

### 1.4 `verify_result.py`

| 要点 | 说明 |
|------|------|
| 输出格式 | 参考 `references/output-format.md` |
| 颜色 | non-sat: 橙色，sat: 蓝色 |
| 多输入遍历 | `range(num_special * num_special)` - 遍历所有组合 |

**模板代码**：直接拷贝 `set_atomic_add_refactor/scripts/verify_result.py`

---

### 1.5 `test.sh`

```bash
DTYPES=("int16_t" "int32_t" "half" "float")  # 根据API支持的数据类型
INDICES=(0 1 2 3)

for i in "${!DTYPES[@]}"; do
    ./build/add ${INDICES[$i]} 1  # sat模式
    ./build/add ${INDICES[$i]} 0  # non-sat模式
done
```

**模板代码**：直接拷贝 `set_atomic_add_refactor/test.sh`

---

## 二、检查清单

### 2.1 算子代码 (.asc)
- [ ] 类模板化 `template <typename T>`
- [ ] GlobalTensor/LocalTensor/TQue 类型参数化
- [ ] Init 函数添加 `isSaturation` 参数
- [ ] 内核函数模板化 + 显式实例化
- [ ] 添加 `SetSaturationMode()` 函数
- [ ] 多核改为单核
- [ ] 命令行参数解析 (dtype, saturation)
- [ ] DataType 枚举定义
- [ ] 类型分发 switch 语句
- [ ] 动态文件路径构建

### 2.2 Python脚本
- [ ] 拷贝模板代码（gen_data.py, gen_data_utils.py, verify_result.py）
- [ ] 根据API支持的数据类型修改 DTYPES/INDICES
- [ ] verify_result.py 遍历所有组合（多输入场景不是num_special次）
- [ ] 输出格式符合 `references/output-format.md`

---

## 三、文件结构

```
{api_name}_refactor/
├── {api_name}.asc           # 核心算子 (模板化)
├── CMakeLists.txt          # 构建配置
├── data_utils.h            # 文件读写
├── test.sh                 # 测试脚本
├── scripts/
│   ├── gen_data.py         # 数据生成 (拷贝模板)
│   ├── gen_data_utils.py  # 工具模块 (拷贝模板)
│   └── verify_result.py   # 结果验证 (拷贝模板)
├── input/                  # 输入数据
└── output/                 # 输出+golden
```

---

## 四、参考模板位置

| 文件 | 路径 |
|------|------|
| 算子代码 | `set_atomic_add_refactor/set_atomic_add.asc` |
| 数据生成 | `set_atomic_add_refactor/scripts/gen_data.py` |
| 工具模块 | `set_atomic_add_refactor/scripts/gen_data_utils.py` |
| 结果验证 | `set_atomic_add_refactor/scripts/verify_result.py` |
| 测试脚本 | `set_atomic_add_refactor/test.sh` |
| 输出格式 | `references/output-format.md` |
