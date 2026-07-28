# 头文件和库文件说明

## 接口分类

Ascend C采用SIMD+SIMT双模并行的核心设计，保留了SIMD高能效、高吞吐量的核心优势，又通过SIMT模型补齐了离散数据、复杂分支等不规则场景的计算能力。根据两种编程模型，接口按照下表所示分类，下文为了描述方便，将本文中的接口统称为Ascend C接口。

**表 1**  关键接口类别

| 接口类别 | 描述 |
| --- | --- |
| SIMD C API | 匹配 C 语言开发习惯，支撑实现极致性能 |
| SIMD 基础API | 匹配 C++ Tensor 开发习惯，支撑实现极致性能 |
| SIMD 高阶API | 封装通用的单核算法实现，提供良好的泛化性能，开发效率高 |
| SIMT API | 适用于离散矢量算子，充分发挥 SIMT 在离散场景的优势，同时匹配业界编程习惯 |

## 调用接口依赖的头文件和库文件说明

安装固件、驱动及CANN软件包后，编译、运行应用程序时才能引用到Ascend C接口的头文件、库文件。

您需要根据实际使用的Ascend C接口来include依赖的文件，各头文件的用途如下表所示。

Ascend C接口的头文件在“$\{INSTALL\_DIR\}/include/"目录下。$\{INSTALL\_DIR\}请替换为CANN软件安装后文件存储路径。以root用户安装为例，安装后文件默认存储路径为：/usr/local/Ascend/cann。

**表 2**  SIMD头文件列表

| 定义接口的头文件 | 用途 | 对应的库文件 |
| --- | --- | --- |
| c_api/asc_simd.h | 提供SIMD C API对外接口。 | 不涉及 |
| basic_api/kernel_basic_intf.h | 提供SIMD基础API对外接口。 | 不涉及 |
| adv_api/kernel_api.h | 提供SIMD高阶API对外接口。 | 不涉及 |

**表 3**  SIMT API头文件列表

| 定义接口的头文件 | 用途 | 对应的库文件 |
| --- | --- | --- |
| simd/asc_simt.h | 提供SIMT API对外接口。 | 不涉及 |

**表 4**  SIMD 基础API头文件根据功能细分列表

| 定义接口的头文件 | 用途 | 对应的库文件 |
| --- | --- | --- |
| basic_api/kernel_basic_intf.h | 提供SIMD基础API对外接口，包含以下矢量计算、矩阵计算对外接口。 | 不涉及 |
| basic_api/kernel_vec_intf.h | 提供SIMD 基础API矢量计算对外接口。 | 不涉及 |
| basic_api/kernel_cube_intf.h | 提供SIMD 基础API矩阵计算对外接口。 | 不涉及 |