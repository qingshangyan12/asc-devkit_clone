# TensorAPI Conv2D功能详细设计

## 1 需求及场景分析

### 1.1 需求背景

TensorAPI已提供Copy、Mmad、Layout和Tensor等基础能力。Conv2D需要在这些能力之上支持特征图格式转换、Img2Col展开、分块矩阵乘以及结果搬出，使用户可通过统一的Tensor和Layout接口实现Conv2D Forward。

本方案不新增单一的`Conv2D`函数，而是通过TensorAPI现有接口组合完成卷积计算。

### 1.2 典型场景

- Global Memory特征图为NCHW、NHWC或NC1HWC0格式。
- 卷积支持padding、stride和dilation参数配置。
- 输出空间或输出通道较大时，按M/N/K方向切分到多核或核内循环。
- 计算结果以NC1HWC0、NHWC或NCHW格式搬回Global Memory。

### 1.3 设计目标

1. 通过LayoutPattern描述特征图格式，并在Copy入口自动分发对应通路。
2. 通过`Img2ColParams`配置卷积窗口，将L1特征图展开为L0A矩阵。
3. 复用Mmad完成分块矩阵乘和K方向累加。
4. 支持通过Slice和显式Layout进行分核及核内tiling。
5. 使数据搬运、计算和格式转换均由Tensor的Location和LayoutPattern统一表达。

### 1.4 当前范围

- 实现Conv2D Forward基础计算和M/N/K tiling。
- 当前不在Conv2D通路中集成group、depthwise、bias、随路量化或激活融合。
- 输入和输出数据类型必须满足Copy和Mmad各通路已有的类型校验；本功能不扩展新的数据类型组合。

## 2 方案概述

Conv2D能力通过扩展TensorAPI现有Layout和Copy分发体系实现，不新增独立的`Conv2D`对外接口。Layout层新增`NCHWLayoutPtn`、`NHWCLayoutPtn`和`NC1HWC0LayoutPtn`，并为三种Pattern注册对应的`MakeFrameLayout`构造逻辑，使特征图的Shape、Stride和数据格式能够随Tensor类型传递。后续Copy入口读取源、目的Tensor的LayoutPattern，在编译期选择具体搬运实现。

特征图从Global Memory搬入L1时，在`CopyGM2L1Routing`中增加以`NC1HWC0LayoutPtn`为目的Pattern的路由。源Pattern为NC1HWC0时执行同格式搬运；源Pattern为NHWC时分发到nd2nz通路；源Pattern为NCHW时分发到dn2nz通路。各实现从Layout的Shape和Stride中获取H、W、C及源步长，生成底层搬运参数，将L1中的特征图统一组织为NC1HWC0格式。

L1到L0A阶段复用`CopyL12L0A`接口，并通过`CopyAtom.with(const Img2ColParams<T>&)`绑定卷积参数。`CopyTraits<CopyL12L0A>`接收参数后生成携带`Img2ColParams`的Atom；Copy入口识别源Pattern为`NC1HWC0LayoutPtn`时，固定选择Img2Col模式，并通过`CopyL12L0ARouting`分发到`LoadDataL12L0AImg2Col`。这样保持了统一的`Copy(atom, dst, src)`调用形式，卷积参数不需要增加到通用Copy函数原型中。

`LoadDataL12L0AImg2Col`从源Layout读取特征图高度、宽度和通道数，并使用`Img2ColParams`中的M/K扩展长度、起始位置、padding、stride、filter和dilation信息配置FMatrix、padding寄存器及repeat参数，最后调用底层Img2Col搬运指令，将L1的NC1HWC0特征图直接展开为L0A中的NZ矩阵。`Img2ColParams<T>`的模板类型用于保证`padValue`的位模式与L0A目的Tensor的元素类型一致。

权重搬入和矩阵计算不新增卷积专用接口。权重继续通过现有GM到L1及L1到L0B Copy通路搬入，以ZN Layout描述；展开后的L0A矩阵和L0B权重由现有`Mmad`接口计算。M、N、K及L0C初始化或累加行为通过`MmadAtom.with(MmadParams)`传递，因此Conv2D只复用已有矩阵计算能力，不改变Mmad分发逻辑。

结果搬出阶段在`CopyL0C2GMRouting`中增加从`NZLayoutPtn`到`NC1HWC0LayoutPtn`、`NHWCLayoutPtn`和`NCHWLayoutPtn`的路由。NC1HWC0输出复用通用L0C搬出实现，NHWC输出使用nz2nd通路，NCHW输出使用nz2dn通路。各通路根据目的Layout计算输出空间、通道数和目的步长，使L0C矩阵可以直接写回目标特征图格式。

整个方案依赖Location和LayoutPattern进行编译期分发。入口层对GM、L1、L0A、L0B和L0C的位置关系进行静态校验，具体通路继续复用现有数据类型检查。新增逻辑只负责格式路由和Img2Col参数转换，不引入运行时虚函数或额外中间数据搬运。

## 3 测试用例场景说明

| 编号 | 测试场景 | 主要验证点 | 对应用例 |
| :--- | :--- | :--- | :--- |
| C01 | Conv2D Layout构造 | NCHW、NHWC和NC1HWC0的Shape、Stride及Pattern正确 | `ConvFeatureMapLayouts` |
| C02 | NHWC特征图搬入 | GM NHWC正确分发到L1 NC1HWC0，nd2nz参数正确 | `CopyGm2L1NHWC2NC1HWC0` |
| C03 | NCHW特征图搬入 | GM NCHW正确分发到L1 NC1HWC0，dn2nz参数正确 | `CopyGm2L1NCHW2NC1HWC0` |
| C04 | NC1HWC0特征图搬入 | 完整W和部分W Slice场景的burst长度、stride及H循环正确 | `CopyGm2L1NC1HWC0*` |
| C05 | Img2Col搬入 | L1 NC1HWC0到L0A NZ分发成功，卷积窗口参数可正确传递 | `CopyL12L0AImg2Col` |
| C06 | 权重搬入 | GM到L1以及L1 ZN到L0B ZN的Shape、Stride和子块偏移正确 | L1到L0B现有Copy UT |
| C07 | Mmad累加 | 首个K块初始化L0C，后续K块累加，M/N/K参数正确 | Mmad现有UT |
| C08 | NC1HWC0结果搬出 | L0C NZ正确写回GM NC1HWC0 | `CopyL0C2GMNZ2NC1HWC0` |
| C09 | NHWC结果搬出 | nz2nd的M/N和dst stride正确 | `CopyL0C2GMNZ2NHWC` |
| C10 | NCHW结果搬出 | nz2dn的channel寄存器、M/N和dst stride正确 | `CopyL0C2GMNZ2NCHW` |
| C11 | 边界tiling | M块不按输出行对齐、首尾padding、K多次累加和末尾块正确 | Conv2D tiling样例 |
| C12 | 端到端精度 | 随机输入与CPU Conv2D参考结果比较，验证整条Copy/Img2Col/Mmad/Copy流程 | `conv2d_forward_tiling_tensor_api` sim/npu测试 |

端到端精度测试至少覆盖以下参数组合：

- 无padding和非零padding；
- stride为1和大于1；
- dilation为1和大于1；
- M/N/K恰好整除base块和存在末尾块；
- M tile从输出行中间开始；
- NCHW、NHWC和NC1HWC0输入输出组合。
