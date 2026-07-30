# ST 构建与缺失符号排障

## 1. 先判断失败阶段

```text
compile error：单个源文件类型/声明问题
archive missing：源文件没进入静态库
link undefined：最终目标找不到强符号
runtime dlsym failure：动态库未装载或未导出
test fallback：命中weak stub而非真实实现
```

## 2. 逐层检查

```bash
rg -n '<Symbol>' impl tests
rg -n '<source.cc>' --glob 'CMakeLists.txt' .
nm -C <static-library> | rg '<Symbol>'
readelf -Ws <shared-library> | rg '<Symbol>'
```

随后检查最终测试目标链接命令、静态库顺序、`--whole-archive` 和宏条件。

## 3. 常见原因

- 只添加头文件，没有把 `.cc` 加入 `target_sources`。
- 实现进入产品库，但GTest没有链接产品库。
- ST又声明了一份签名不同的stub。
- `UT_TEST/AICPU_COMPILE` 使真实实现被条件编译掉。
- C与C++ linkage或命名空间不一致。
- weak fallback成功返回，掩盖正式符号缺失。
- 构建目录未重新配置，新增CMake源文件未生效。

## 4. weak fallback规则

- 只有真正可选的外部组件可以提供fallback。
- fallback必须返回可区分的“不支持/未加载”，不能伪造算法成功。
- 正常用例要证明强符号路径被调用。
- 报告测试结果时注明是否链接正式产品实现。

## 5. 最小闭环

```text
目标库可见强符号
  -> GTest可执行文件链接成功
  -> 单case真实进入目标Kernel
  -> 语义Checker与最终结果通过
  -> 相关suite无回归
```
