# SIMD VF Assert支持范围样例

## 概述

本样例集中展示SIMD VF `assert`当前可编译的写法及已知编译器限制。默认代码仅启用一个有效assert站点，并在运行时主动触发该断言，验证断言信息经FIFO发布后再触发Vector Core异常。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```text
├── assert_support_matrix
│   ├── CMakeLists.txt                // 编译工程文件
│   ├── assert_support_matrix.asc     // 支持用法和限制用法
│   ├── README.md                     // 中文说明文档
│   └── README_en.md                  // 英文说明文档
```

## 支持范围

默认启用的`SupportedAssert`将以下可编译写法合并到一个有效assert站点中：

- 标量组合条件和布尔中间变量。
- 带格式化字符串及参数的assert。
- `__ubuf__`指针与整数`0`比较。

以下写法集中保留在未实例化模板中，对应调用点默认注释，因此不影响样例编译：

- `__ubuf__`指针与`nullptr`直接比较形成assert条件。当前BiSheng后端报错`Do not know how to expand this operator's operand`。
- 同一SIMD VF函数中存在两个以上有效assert站点。当前BiSheng在VF DAG指令选择阶段触发`stack smashing`并退出。

以上是当前编译器限制，并非assert接口语义限制。需要复现时，每次只取消一个不支持调用点的注释。

## 编译运行

在本样例目录下执行：

```bash
source ${install_path}/cann/set_env.sh
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
./demo
```

程序向核函数传入`5`，使`value < 4U`条件失败。日志应先包含自定义断言信息及`Assertion ... failed`信息，随后流同步返回Vector Core异常；程序识别到该预期异常后返回0。
