# 目录结构与文件命名规范

## 1. 顶层目录总览

```
asc-devkit/
├── include/              # 公开 API 声明 (.h)
├── impl/                 # 内部实现 (.h + .cpp)
├── tests/                # 测试代码
├── examples/             # 示例代码 (.asc + .cpp)
├── docs/                 # 文档
├── tools/                # 构建工具
├── cmake/                # CMake 模块和宏
├── scripts/              # 辅助脚本
├── build.sh              # 构建入口脚本
├── CMakeLists.txt        # 根构建文件
├── .clang-format         # C++ 格式化配置
├── .pre-commit-config.yaml # Pre-commit 钩子
├── CONTRIBUTING.md       # 贡献总流程
└── README.md             # 项目说明
```

---

## 2. include/ ↔ impl/ 镜像关系

`include/` 和 `impl/` 顶层目录结构一一对应：

| include/ | impl/ | 说明 |
|----------|-------|------|
| `include/basic_api/` | `impl/basic_api/` | 基础 API（kernel operator 接口与结构体） |
| `include/adv_api/` | `impl/adv_api/` | 高阶 API（数学库、归一化、矩阵乘等） |
| `include/c_api/` | `impl/c_api/` | C API（指令层封装） |
| `include/simt_api/` | `impl/simt_api/` | SIMT API |
| `include/tensor_api/` | `impl/tensor_api/` | Tensor API（experimental） |
| `include/utils/` | `impl/utils/` | 工具类 |
| `include/aicpu_api/` | `impl/aicpu_api/` | AI CPU API |

### 详细映射规则

| include 路径 | impl 路径 |
|-------------|----------|
| `include/basic_api/xxx.h` | `impl/basic_api/xxx_impl.h` |
| `include/basic_api/xxx_intf.h` | `impl/basic_api/xxx_intf_impl.h` |
| `include/basic_api/reg_compute/xxx.h` | `impl/basic_api/dav_<arch>/xxx_impl.h` |
| `include/basic_api/core_mng/xxx.h` | `impl/basic_api/dav_<arch>/xxx_impl.h` |
| `include/adv_api/math/xxx.h` | `impl/adv_api/detail/math/xxx/xxx_common_impl.h` |
| `include/adv_api/math/xxx_tiling.h` | `impl/adv_api/tiling/math/xxx_tiling_impl.cpp` |
| `include/adv_api/<cat>/xxx.h` | `impl/adv_api/detail/<cat>/xxx/xxx_common_impl.h` |
| `include/c_api/xxx/xxx.h` | `impl/c_api/xxx_impl/xxx_impl.h` |

---

## 3. 文件命名规范

### 3.1 后缀约定

| 后缀 | 含义 | 所在目录 |
|------|------|---------|
| `_intf.h` | 接口声明（interface） | `include/` |
| `_impl.h` | 实现定义（implementation） | `impl/` |
| `_intf_impl.h` | 接口 + 架构分发实现 | `impl/` |
| `_tiling.h` | Tiling 接口声明 | `include/` |
| `_tiling_impl.cpp` | Tiling 宿主侧实现 | `impl/adv_api/tiling/` |
| `_struct_*.h` | 类型 / 结构体定义 | `include/` |
| `_common_impl.h` | 通用算法实现 | `impl/adv_api/detail/` |
| `_check.h` | 参数校验逻辑 | `impl/adv_api/detail/api_check/` |
| `_config.h` | 配置参数定义 | `include/` 或 `impl/` |
| `_utils.h` | 工具函数 | `include/` 或 `impl/` |

### 3.2 命名规则

- 全部使用 **snake_case**（小写字母 + 下划线）
- 无空格、无连字符、无大写
- 英文单词，避免缩写（除非是广泛认可的缩写如 `vec`、`intf`）

**正确示例：**
```
kernel_operator_vec_binary_intf.h
kernel_struct_fixpipe.h
kernel_operator_common_intf_impl.h
axpy_common_impl.h
exp_check_common.h
```

### 3.3 不同 API 类型的命名差异

| API 类型 | 头文件 | 实现文件 | 示例 |
|---------|--------|---------|------|
| 基础 API | `kernel_<name>_intf.h` | `kernel_<name>_impl.h` | `kernel_operator_vec_binary_intf.h` |
| 高阶 API | `<name>.h` | `<name>_common_impl.h` | `axpy.h` / `axpy_common_impl.h` |
| C API | `<category>.h` | `<category>_impl.h` | `vector_compute.h` / `vector_compute_impl.h` |
| Tiling | `<name>_tiling.h` | `<name>_tiling_impl.cpp` | `axpy_tiling.h` |

---

## 4. 架构目录命名

### 4.1 命名格式

架构特定实现统一放在 `impl/<api_type>/dav_<code>/` 目录下，格式为 `dav_` + 架构代码。

### 4.2 架构代码映射

| 目录 | `__NPU_ARCH__` 值 | 对应产品 |
|------|-------------------|---------|
| `dav_c220` | `2201` | Ascend 910B（Atlas A2/A3 训练推理系列） |
| `dav_3510` | `3510` | Ascend 950PR / Ascend 950DT（Atlas A5） |

> 产品型号与 `__NPU_ARCH__` 的完整对应关系见 [__NPU_ARCH__ 文档](../guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md#table65291052154114)。

### 4.3 架构分发模式

在 impl 头文件中使用 `#if / #elif / #endif` 链按架构选择实现：

```cpp
#if __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_common_impl.h"
#elif __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_common_impl.h"
#endif
```

---

## 5. impl/adv_api 内部组织

```
impl/adv_api/
├── CMakeLists.txt
├── detail/                   # 核心实现
│   ├── activation/           # 激活函数
│   ├── api_check/            # 参数校验
│   │   ├── host_apicheck.h
│   │   ├── kernel_api_check.h
│   │   └── kernel_check/     # 每个 API 的校验逻辑
│   │       └── math/
│   │           └── exp/
│   │               └── exp_check_common.h
│   ├── common/               # 公共工具
│   ├── math/                 # 数学运算（53+ 子目录）
│   │   └── exp/
│   │       ├── exp_common_impl.h
│   │       └── exp_3510_impl.h
│   ├── matmul/               # 矩阵乘
│   ├── normalization/        # 归一化
│   ├── reduce/               # 归约
│   └── ...
└── tiling/                   # Tiling（宿主侧内存规划）
    ├── CMakeLists.txt
    └── math/
        └── exp_tiling_impl.cpp
```

---

## 6. 新增 API 操作清单

新增一个完整的 API 时，按以下清单创建和修改文件：

### 6.1 高阶 API 清单

```
□ include/adv_api/<category>/<api>.h
    公开头文件，包含 Doxygen 文档、模板声明、到 impl 的转发

□ include/adv_api/<category>/<api>_tiling.h
    Tiling 接口声明（如 API 需要临时空间）

□ impl/adv_api/detail/<category>/<api>/<api>_common_impl.h
    核心算法实现

□ impl/adv_api/tiling/<category>/<api>_tiling_impl.cpp
    Tiling 接口实现（如需要）

□ impl/adv_api/tiling/CMakeLists.txt
    在 tiling_api 的 add_library 中添加 .cpp

□ impl/adv_api/detail/<category>/CMakeLists.txt（如有）
    添加子目录 add_subdirectory(<api>)

□ include/adv_api/kernel_api.h
    添加 #include "<category>/<api>.h"（含 __NPU_ARCH__ 守卫）

□ include/adv_api/tiling_api.h
    添加 #include "<category>/<api>_tiling.h"

□ tests/api/adv_api/<category>/<api>/test_operator_<api>.cpp
    功能测试

□ tests/api/adv_api/<category>/tiling/test_tiling.cpp
    或新建 Tiling 测试

□ tests/api/adv_api/CMakeLists.txt
    在对应 product type 的 GLOB 中添加测试文件

□ tests/api/basic_api/ascendc_header_checker/check_<api>.cpp
    Header Checker 编译检查文件（确保头文件可独立编译）
```

### 6.2 基础 API 清单

```
□ include/basic_api/kernel_operator_<name>_intf.h
    接口声明

□ impl/basic_api/kernel_operator_<name>_intf_impl.h
    接口 + 架构分发实现

□ impl/basic_api/dav_<arch>/kernel_operator_<name>_impl.h
    每个架构的实现文件

□ tests/api/basic_api/<category>/test_operator_<name>.cpp
    测试

□ include/basic_api/kernel_operator.h（或其他总入口）
    添加 include

□ tests/api/basic_api/ascendc_header_checker/check_<name>.cpp
    Header Checker 编译检查文件
```

### 6.3 C API 清单

```
□ include/c_api/<category>/<category>.h 或添加函数声明到现有文件

□ impl/c_api/<category>_impl/<arch>/<api>_impl.h
    架构特定实现

□ tests/api/c_api/<arch>/<category>/test_<api>_instr.cpp
    Mock 测试

□ docs/api/context/c_api/<category>/<api>.md
    API 文档
```

---

## 7. examples/ 目录组织

### 7.1 目录结构

```
examples/
├── 01_simd_cpp_api/        # SIMD C++ API 示例
│   ├── 00_introduction/    # 入门示例
│   ├── 01_utilities/       # 工具使用（printf、assert 等）
│   ├── 02_features/        # 高级特性
│   ├── 03_libraries/       # API 库使用
│   │   └── 12_math/        # 数学库
│   │       └── exp/        # Exp 示例
│   │           ├── CMakeLists.txt
│   │           ├── exp.asc          # Kernel 源码（.asc 后缀）
│   │           ├── data_utils.h     # 测试数据工具
│   │           └── README.md
│   ├── 04_best_practices/  # 最佳实践
│   └── 05_compatibility_guide/
├── 02_simd_c_api/          # SIMD C API 示例
├── 03_simt_api/            # SIMT API 示例
└── README.md
```

### 7.2 命名规则

- 三级目录组织：`<api_type>/<category>/<api_name>/`
- 数字前缀确保排序：`00_introduction/`、`01_utilities/`
- Kernel 源码文件使用 `.asc` 扩展名
- 每个示例必须有独立的 `CMakeLists.txt`
- 每个示例推荐包含 `README.md`

---

## 8. tests/ 目录组织

```
tests/
├── CMakeLists.txt
├── main.cpp                  # 测试主入口
├── main_global.cpp           # 全局测试入口
├── api/
│   ├── adv_api/              # 高阶 API 测试
│   │   ├── CMakeLists.txt
│   │   ├── math/
│   │   │   └── exp/
│   │   │       ├── test_operator_exp.cpp
│   │   │       └── test_operator_exphighprecision.cpp
│   │   ├── matmul/
│   │   ├── normalization/
│   │   ├── reduce/
│   │   ├── tiling/
│   │   │   └── test_tiling.cpp
│   │   └── ...
│   ├── basic_api/            # 基础 API 测试
│   │   ├── ascendc_header_checker/
│   │   │   ├── CMakeLists.txt         # 独立 project(HeaderChecker)
│   │   │   └── check_*.cpp            # 编译检查文件
│   │   └── ...
│   ├── c_api/                # C API 测试
│   ├── reg_compute_api/
│   ├── simt_api/
│   ├── tensor_api/
│   └── utils/
└── python/                   # Python 相关测试
```

### 8.1 测试文件命名

| 模式 | 示例 |
|------|------|
| `test_operator_<api>.cpp` | `test_operator_exp.cpp` |
| `test_operator_<api>_<variant>.cpp` | `test_operator_matmul_v220.cpp` |
| `test_<component>.cpp` | `test_tiling.cpp`、`test_scheduler_norm.cpp` |
| `check_<target>.cpp` | Header Checker 文件 |
