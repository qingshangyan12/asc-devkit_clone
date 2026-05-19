# CMake 构建规范

## 1. 构建系统概览

### 1.1 构建层级

```
CMakeLists.txt (根)
├── project(asc-devkit)
├── cmake/                    # CMake 基础设施模块
│   ├── config.cmake          # 编译器、安装路径、构建类型
│   ├── dependencies.cmake    # 依赖解析
│   ├── fetch_cann_cmake.cmake # CANN CMake 基础设施
│   ├── version_info.cmake    # 版本信息
│   └── asc/                  # Kernel cmake 模块（打包分发用）
│       ├── asc-config.cmake
│       ├── ascendc.cmake
│       └── legacy_modules/   # 遗留项目模板
├── impl/CMakeLists.txt       # 各 API 实现子目录
│   ├── aicpu_api/
│   ├── adv_api/
│   │   ├── CMakeLists.txt
│   │   ├── detail/
│   │   └── tiling/CMakeLists.txt
│   ├── basic_api/
│   ├── c_api/
│   └── utils/
├── tools/CMakeLists.txt      # 构建工具
├── tests/CMakeLists.txt      # 测试（ENABLE_TEST 门控）
└── examples/                 # 各示例独立 CMakeLists.txt
```

### 1.2 根 CMakeLists.txt 关键流程

```cmake
include(cmake/fetch_cann_cmake.cmake)
init_cann_project()

add_subdirectory(impl)
add_subdirectory(tools)

if(ENABLE_TEST)
    add_subdirectory(tests)
endif()
```

### 1.3 构建设置

- `PRODUCT_SIDE host` — 默认 host 侧构建
- Device 侧构建通过 `cmake/device/` 模块进行交叉编译
- 示例使用 `find_package(ASC REQUIRED)` 和 ASC 语言编译 Kernel

---

## 2. Target 命名规范

### 2.1 Target 类型与命名

| Target 类型 | 命名模式 | 示例 |
|------------|---------|------|
| STATIC library | `snake_case` 功能描述 | `tiling_api`、`aicpu_api` |
| INTERFACE library | `snake_case`，常用 `_headers` 后缀 | `tikcfw_headers`、`tiling_api_headers`、`kernel_tiling_headers` |
| EXECUTABLE (test) | `ascendc_ut_<api_type>_<scope>_${product_type}` | `ascendc_ut_adv_api_kernel_ascend910B1_AIV` |
| custom_target | 行为描述 | `gen_kernel_tiling`、`hostapi_headers` |
| project (测试项目) | PascalCase 描述 | `HeaderChecker` |

### 2.2 Test Executable 命名公式

```
ascendc_ut_<API_层级>_<测试范围>_${product_type}
```

**API 层级：** `adv_api`、`basic_api`、`c_api`  
**测试范围：** `kernel`（Kernel 侧）、`tiling`（Tiling 侧）  
**product_type 示例：** `ascend610`、`ascend310p`、`ascend910B1_AIV`、`ascend910B1_AIC`

完整示例：
```
ascendc_ut_adv_api_kernel_ascend610
ascendc_ut_adv_api_kernel_ascend910B1_AIV
ascendc_ut_adv_api_tiling_ascend910B1_AIC
```

---

## 3. 文件组织方式

### 3.1 库源文件：显式列举

`STATIC` library 的源文件必须逐一列举，不使用 glob：

```cmake
add_library(tiling_api STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/math/axpy_tiling_impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/exp_tiling_impl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/sin_tiling_impl.cpp
    # ... 逐一添加
)
```

### 3.2 测试源文件：file(GLOB ...)

测试文件按 product type 分组使用 glob 收集：

```cmake
file(GLOB ASCENDC_TEST_ASCEND310P_CASE_SRC_FILES
    math/*.cpp
    activation/*.cpp
    normalization/*.cpp
)
```

### 3.3 递归收集：file(GLOB_RECURSE ...)

```cmake
file(GLOB_RECURSE ASCENDC_TEST_ASCEND310P_CASE_SRC_FILES_API_CHECK
    api_check/math/*.cpp
)
```

### 3.4 头文件库：install(DIRECTORY ...)

仅头文件的 API 模块（如 C API）通过目录安装来集成：

```cmake
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CUSTOM_INSTALL_DIR}
    FILES_MATCHING PATTERN "*.h"
)
```

### 3.5 条件编译：Generator Expression

```cmake
$<$<STREQUAL:${product_type},ascend610>:../common/tik_pv_wrapper.cpp>
```

---

## 4. 架构条件编译

### 4.1 PRODUCT_TYPE_LIST

测试按产品类型分组，CMake 中遍历 `PRODUCT_TYPE_LIST`：

```
ascend610, ascend310p, ascend910B1_AIC, ascend910B1_AIV,
ascend950pr_9599_AIC, ascend950pr_9599_AIV, ascend310B1
```

### 4.2 编译宏映射

| 产品类型 | 编译宏 | `__NPU_ARCH__` |
|---------|--------|---------------|
| ascend610 | `__DAV_C220__` | 2201 |
| ascend310p | `__DAV_M300__` | 3002 |
| ascend310B1 | `__DAV_L300__` | 3003 |
| ascend910 | `__DAV_C220__` | 2201 |
| ascend910B1 (AIC/AIV) | `__DAV_C310__` | 3510 |
| ascend950pr_9599 (AIC/AIV) | `__DAV_M500__` | 5102 |

> **说明：** AIC（AI Core）和 AIV（AI Vector）为同一架构的不同流水线变体，共享相同的 `__NPU_ARCH__` 和编译宏。

### 4.3 添加新架构检查点

新增一个架构时需检查以下位置：
1. `impl/basic_api/` 下各 `_intf_impl.h` 的 `#if __NPU_ARCH__` 分支
2. `impl/adv_api/detail/<category>/<api>/` 下的架构条件 include
3. `tests/api/*/CMakeLists.txt` 中的 `PRODUCT_TYPE_LIST`

---

## 5. Target 依赖管理

### 5.1 INTERFACE 库用于头文件传递

```cmake
add_library(tikcfw_headers INTERFACE)
target_include_directories(tikcfw_headers INTERFACE
    ${ASCENDC_INCLUDE_DIR}
)
```

使用 `INTERFACE` 而非 `PUBLIC` 的原因是头文件库不产生编译产物。

### 5.2 链接依赖顺序

```cmake
target_link_libraries(demo
    tiling_api
    register
    platform
    m
    dl
    graph_base
)
```

---

## 6. 新增模块 CMake 集成步骤

### 6.1 高阶 API Tiling 实现

1. 在 `impl/adv_api/tiling/<category>/` 下创建 `xxx_tiling_impl.cpp`
2. 在 `impl/adv_api/tiling/CMakeLists.txt` 的 `tiling_api` target 中添加：
   ```cmake
   ${CMAKE_CURRENT_SOURCE_DIR}/<category>/xxx_tiling_impl.cpp
   ```

### 6.2 新增 API 类别目录

1. 在 `impl/adv_api/detail/` 下创建新类别目录
2. 创建类别 `CMakeLists.txt`：
   ```cmake
   add_subdirectory(<api1>)
   add_subdirectory(<api2>)
   ```
3. 在父级 `CMakeLists.txt` 中 `add_subdirectory(<新类别>)`

### 6.3 新增测试

1. 在 `tests/api/<api_type>/CMakeLists.txt` 中找到对应的产品类型 GLOB 变量
2. 确保新增的测试文件路径被 GLOB 模式覆盖
3. 如使用非标准路径，在 CMakeLists.txt 中显式添加

### 6.4 不要修改的文件

- `cmake/fetch_cann_cmake.cmake` — 基础设施，由仓库维护者管理
- `cmake/config.cmake` — 构建配置，一般不需要改动
- `cmake/asc/` — kernel cmake 打包模块，由仓库维护者管理

---

## 7. 常用构建命令

### 7.1 全量构建

```bash
bash build.sh
```

### 7.2 仅测试

```bash
bash build.sh -t
```

### 7.3 仅运行特定 test target

修改 `build.sh` 中 `build_test()` 函数：

```bash
function build_test() {
    cmake_config
    build ascendc_ut_adv_api_kernel_ascend910B1_AIV    # 替换为目标 target
}
```

然后执行 `bash build.sh -t`。

### 7.4 示例构建

每个示例目录下有独立的 `CMakeLists.txt`，进入示例目录后：

```bash
mkdir build && cd build
cmake ..
make
```

### 7.5 编译安装

参考 `docs/quick_start.md` 中的编译安装说明。

---

## 8. 自定义 Target

### 8.1 代码生成 Target

```cmake
add_custom_target(gen_kernel_tiling ALL
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/gen_tiling.py
    DEPENDS ...
)
```

### 8.2 头文件安装 Target

```cmake
add_custom_target(hostapi_headers ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ...
)
```
