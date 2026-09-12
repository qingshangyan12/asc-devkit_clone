# AddCustom：随算子包交付 JIT 编译资料

此目录包含完整 host、kernel、tiling 头文件和 CMake 配置。需要包含本功能的配套 SDK；仅设置变量不能使旧 SDK 支持该功能。

## 构建与安装

先加载已安装 SDK 环境，在本目录运行：

```bash
source /usr/local/Ascend/cann/set_env.sh
cmake -S . -B build \
  -Dvendor_name=jit_add_custom -DASCEND_COMPUTE_UNIT=ascend910b \
  -DPACKAGE_TYPE=RUN -DKERNEL_SPEC_MODE=Normal \
  -DCMAKE_INSTALL_PREFIX="$PWD/installed"
cmake --build build -j4
cmake --install build
```

如 find_package 无法找到 ASC，使用该 SDK 实际提供的 asc-config.cmake 所在目录设置 ASC_DIR。不要把其他版本的 CMake 模块与本版本工具混用。

`PACKAGE_TYPE` 是此示例为了切换场景设置的变量，值为 RUN、SHARED、STATIC；公共接口仍是 npu_op_package(TYPE ... CONFIG KERNEL_SPEC_MODE ...)。每种组合使用不同 build 目录。原 `package` target 可继续生成 RUN 归档；上面的 cmake --install 不等于执行 RUN 安装器。

## 结果检查

- Normal 导出 basic，SK 导出 sk，All 同时导出两者，None 默认关闭。
- manifest 在 build/ascendc_kernels/kernel_spec/<soc> 下；其中 kernels 是入口列表。
- RUN 安装到 vendors/jit_add_custom/op_spec/libcust_compile_db.so，op_spec 与 op_api 同级。
- SHARED/STATIC 将资源 object 加入最终 package 库，源码安装到 share/jit_add_custom/src，不额外安装 JIT SO。
- 多芯片沿用 ASCEND_COMPUTE_UNIT 列表，聚合本次所有资源，相同 resource_id 保留完整一份。

RUN 使用时，将 ASCEND_CUSTOM_OPP_PATH 设置为实际安装的 vendors/jit_add_custom 目录。图框架应从动态二进制取得 resource_id 和 kernel symbol，再将实际参数传给 aclrtcKernelSpecialization。此工程负责构建、交付资源，没有包含张量执行和数值比较程序。

## 日志与限制

需要保留临时文件时，在 package_add 前增加：

```cmake
npu_op_kernel_options(ascendc_kernels ALL OPTIONS --save-temp-files)
```

默认一个 package、一个 kernel library，使用默认包路径并开启原源码和 binary 打包。STATIC 保留 manifest 原 source_file，不自动替换为安装相对路径；源码布局和检索由使用方组织，不额外增加 STATIC 路径限制。多资源库链接、SK 实际执行及 RUN 升级/卸载不能由本示例证明。

接口链路、源码定位规则、错误行为以及已知生产算子限制见仓内 [完整说明](../../../../../../docs/zh/jit_host_package.md)。
