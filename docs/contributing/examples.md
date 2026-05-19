# 示例规范

## 1. 示例目录概览

### 1.1 顶层组织

```
examples/
├── README.md                # 样例总览与 npu-arch 说明
├── README_en.md             # 英文说明
├── 01_simd_cpp_api/         # SIMD C++ API 样例
├── 02_simd_c_api/           # SIMD C API 样例
└── 03_simt_api/             # SIMT API 样例
```

### 1.2 各 API 类型示例一览

| 目录 | 编程模型 | 覆盖内容 |
|------|---------|---------|
| `01_simd_cpp_api/` | C++ 高阶/基础 API | 入门、工具、功能特性、类库、最佳实践、兼容性 |
| `02_simd_c_api/` | C API | 入门、工具、功能特性 |
| `03_simt_api/` | SIMT API | 入门、调试工具、核心特性、实践参考 |

---

## 2. 目录层级规范

### 2.1 三级目录结构

```
examples/<NN>_<api_type>/<NN>_<category>/<NN>_<subcategory>/
```

- 第一级：API 类型 + 数字前缀确保排序（`01_simd_cpp_api/`）
- 第二级：功能类别 + 数字前缀（`00_introduction/`、`01_utilities/`）
- 第三级：具体示例（`00_quickstart/`、`01_vector/`）

### 2.2 C++ API 的 6 大类别

```
01_simd_cpp_api/
├── 00_introduction/              # 入门：quickstart、vector、matrix、fusion、reg
├── 01_utilities/                 # 调试工具：printf、assert、dump、cpudebug、profiling、sanitizer、clock、simulator
├── 02_features/                  # 功能特性：编译、调用、framework、basic_api、aicpu、aclrtc
├── 03_libraries/                 # API 类库：matrix、activation、normalization、quantization、reduce、sort、index、filter、tensor_transformation、random、math、utils
├── 04_best_practices/            # 最佳实践：vector、matrix、reg_vector、fusion、memory_access
└── 05_compatibility_guide/       # 兼容性参考：data_copy_l1togm、fill、matmul_s4、pattern_transformation、scatter、set_loaddata_boundary
```

### 2.3 C API 的 3 大类别

```
02_simd_c_api/
├── 00_introduction/              # 入门：quickstart、add（sync/async/delicacy_async）、reg_base_add
├── 01_utilities/                 # 工具
└── 02_features/                  # 功能特性：c_api（vector_compute sync/async/delicacy_async）
```

### 2.4 SIMT API 的类别

```
03_simt_api/
├── 00_introduction/              # 入门
├── 01_utilities/                 # 调试工具
├── 02_features/                  # 核心特性
└── 03_best_practices/           # 实践参考
```

---

## 3. 单个示例文件组成

每个示例目录（第三级）必须包含以下文件：

| 文件 | 用途 | 是否必需 |
|------|------|---------|
| `<name>.asc` | Kernel 源码（AscendC 语言） | 必需 |
| `CMakeLists.txt` | 独立构建脚本 | 必需 |
| `README.md` | 示例说明文档 | 必需（推荐） |

### 3.1 .asc 文件

Kernel 源码使用 `.asc` 扩展名，对应 AscendC 编程语言：

```
hello_world.asc
c_api_add.asc
matmul_s4.asc
```

命名规则：snake_case，与目录用途一致。

### 3.2 CMakeLists.txt

每个示例目录有独立的 `CMakeLists.txt`，支持独立构建：

```bash
cd examples/01_simd_cpp_api/00_introduction/00_quickstart/hello_world_npu
mkdir build && cd build
cmake ..
make
```

CMakeLists.txt 中需包含 npu-arch 编译选项，开发者根据实际执行环境调整 `--npu-arch` 参数。

### 3.3 README.md

每个示例目录应有 README.md，包含：

1. 示例功能说明
2. 运行步骤
3. 预期输出
4. 注意事项和依赖说明

---

## 4. 命名规范

### 4.1 目录命名

- 使用数字前缀确保排序：`00_quickstart`、`01_vector`、`02_matrix`
- 英文小写 + 下划线（snake_case）
- 类别目录表示内容领域，具体示例目录表示功能点

### 4.2 文件命名

| 文件类型 | 命名规则 | 示例 |
|---------|---------|------|
| Kernel 源码 | snake_case + `.asc` | `hello_world.asc`、`c_api_add.asc` |
| 构建脚本 | 固定名称 `CMakeLists.txt` | |
| 说明文档 | 固定名称 `README.md` | |
| 英文说明 | 固定名称 `README_en.md` | |

---

## 5. npu-arch 编译选项

开发者需根据实际执行环境修改 CMakeLists.txt 中的 `--npu-arch` 参数。

| 产品型号 | npu-arch 参数 |
|---------|-------------|
| Ascend 950PR / Ascend 950DT | `--npu-arch=dav-3510` |
| Atlas A3 训练系列 / Atlas A3 推理系列 | `--npu-arch=dav-2201` |
| Atlas A2 训练系列 / Atlas A2 推理系列 | `--npu-arch=dav-2201` |
| Atlas 推理系列 AI Core | `--npu-arch=dav-2002` |

---

## 6. 新增示例操作清单

### 6.1 规划阶段

```
□ 确定示例归属的 API 类型（simd_cpp_api / simd_c_api / simt_api）
□ 确定示例归属的功能类别（introduction / utilities / features / libraries / best_practices）
□ 确定示例目录名（数字前缀 + snake_case）
```

### 6.2 文件创建

```
□ 创建 examples/<api_type>/<category>/<example_name>/ 目录
□ 编写 <name>.asc Kernel 源码
□ 编写 CMakeLists.txt（参考同类别下现有示例）
□ 编写 README.md（包含功能说明、运行步骤、注意事项）
□ 如需英文说明，编写 README_en.md
```

### 6.3 验证

```
□ 运行示例确保编译通过
□ 运行示例确保功能正确
□ 检查 README.md 描述与实际行为一致
```

---

## 7. 与测试的区别

| 维度 | 示例（examples/） | 测试（tests/） |
|------|-----------------|-------------|
| 目的 | 教学和参考，展示 API 用法 | 验证 API 正确性 |
| 构建方式 | 独立 CMakeLists.txt，独立构建 | 统一 CMake 系统，批量构建 |
| 文件后缀 | `.asc`（Kernel） | `.cpp` |
| 运行环境 | 需要 NPU 环境 | CPU Debug / NPU |
| 文档要求 | 必须含 README.md | 不需要 |

---

## 8. 常见约定

### 8.1 示例复杂度

- 入门示例（`00_introduction/`）：最小可运行代码，展示基本调用方式
- 工具示例（`01_utilities/`）：展示单一调试/分析工具的使用
- 类库示例（`03_libraries/`）：展示单个 API 的典型用法
- 最佳实践（`04_best_practices/`）：展示性能优化技巧，可包含多 API 组合

### 8.2 代码风格

示例中的 Kernel 代码应与项目整体 C++ 代码风格一致（参考 [code-style.md](code-style.md)），同时注重可读性，适当添加注释说明关键步骤。
