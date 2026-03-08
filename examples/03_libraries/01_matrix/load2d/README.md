# b8/b16/b32数据类型下，基于load2d指令的功能演示

## 概述

本样例展示了9种不同的数据搬运与转置配置场景（暂不支持非对齐场景），主要用于说明在矩阵运算中，使用load2d作为搬运指令，数据如何在不同的存储层级（GM、L1、L0A、L0B）之间流动，以及是否启用了随路转置功能。
<table>
    <thead>
        <tr>
            <th>scenarioNum</th>
            <th>数据类型</th>
            <th>GM->L1</th>
            <th>GM->L0A</th>
            <th>GM->L0B</th>
            <th>L1->L0A</th>
            <th>L1->L0B</th>
            <th>A矩阵使能转置</th>
            <th>B矩阵使能转置</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>1</td>
            <td>b8</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>2</td>
            <td>b8</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>3</td>
            <td>b16</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>√</td>
        </tr>
        <tr>
            <td>4</td>
            <td>b16</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
        </tr>
        <tr>
            <td>5</td>
            <td>b16</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>6</td>
            <td>b16</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>√</td>
            <td>√</td>
        </tr>
        <tr>
            <td>7</td>
            <td>b16</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>8</td>
            <td>b32</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
        </tr>
        <tr>
            <td>9</td>
            <td>b32</td>
            <td>×</td>
            <td>√</td>
            <td>√</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
            <td>×</td>
        </tr>
    </tbody>
</table>

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── load2d
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── load2d.asc                  // Ascend C算子实现 & 调用样例
```

## 算子描述

输入A矩阵为Nz数据分形，shape为(m,k)，其中m为外轴，k为内轴
输入B矩阵为Nz数据分形，shape为(k,n)，其中k为外轴，n为内轴  
算子包含以下场景：
- 一、输入数据类型为b8，数据流路径为GM→L1→L0A/L0B，A矩阵不转置（不使能矩阵转置），B矩阵转置（不使能矩阵转置）
- 二、输入数据类型为b8，数据流路径为GM→L0A/L0B，A矩阵不转置（不使能矩阵转置），B矩阵转置（不使能矩阵转置）
- 三、输入数据类型为b16，数据流路径为GM→L1→L0，A矩阵不转置（不使能矩阵转置），B矩阵不转置（使能矩阵转置）
- 四、输入数据类型为b16，数据流路径为GM→L1→L0，A矩阵转置（使能矩阵转置），B矩阵转置（不使能矩阵转置）
- 五、输入数据类型为b16，数据流路径为GM→L1→L0，A矩阵不转置（不使能矩阵转置），B矩阵不转置（使能矩阵转置）
- 六、输入数据类型为b16，数据流路径为GM→L1→L0，A矩阵转置（使能矩阵转置），B矩阵转置（使能矩阵转置）
- 七、输入数据类型为b16，数据流路径为GM→L0A/L0B，A矩阵不转置（不使能矩阵转置），B矩阵转置（不使能矩阵转置）
- 八、输入数据类型为b32，数据流路径为GM→L1→L0，A矩阵不转置（不使能矩阵转置），B矩阵转置（不使能矩阵转置）
- 九、输入数据类型为b32，数据流路径为GM→L0A/L0B，A矩阵不转置（不使能矩阵转置），B矩阵转置（不使能矩阵转置）

算子中主要涉及的通路有GM->L1，GM->L0A/L0B，L1->L0A/L0B，下面将结合代码介绍：

### GM->L1

从GM加载A矩阵到L1
```c++
uint32_t dstOffset = fractalSize;
uint32_t srcOffset = fractalSize;
AscendC::LoadData2DParams loadDataParams;
loadDataParams.repeatTimes = CeilDivision(k, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(m, fractalShape[0]);
loadDataParams.dstGap = CeilDivision(m, fractalShape[0]) - 1;
for (uint16_t i = 0; i < CeilDivision(m, fractalShape[0]); ++i) {
    AscendC::LoadData(a1Local[i * dstOffset], aGM[i * srcOffset], loadDataParams);
}
```
从GM加载B矩阵到L1
```c++
uint32_t dstOffset = fractalSize;
uint32_t srcOffset = fractalSize;
AscendC::LoadData2DParams loadDataParams;
loadDataParams.repeatTimes = CeilDivision(n, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(k, fractalShape[0]);
loadDataParams.dstGap = CeilDivision(k, fractalShape[0]) - 1;
for (uint16_t i = 0; i < CeilDivision(k, fractalShape[0]); ++i) {
    AscendC::LoadData(b1Local[i * dstOffset], bGM[i * srcOffset], loadDataParams);
}
```
### GM->L0A

从GM加载A矩阵到L0A
```c++
uint32_t dstOffset = CeilDivision(k, fractalShape[1]) * fractalSize;
uint32_t srcOffset = fractalSize;
AscendC::LoadData2DParams loadDataParams;
loadDataParams.repeatTimes = CeilDivision(k, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(m, fractalShape[0]);
loadDataParams.dstGap = 0;
loadDataParams.ifTranspose = false;
for (int i = 0; i < CeilDivision(m, fractalShape[0]); ++i) {
    AscendC::LoadData(a2Local[i * dstOffset], aGM[i * srcOffset], loadDataParams);
}
```

### GM->L0B

从GM加载B矩阵到L0B
```c++
uint32_t dstOffset = CeilDivision(n, fractalShape[0]) * fractalSize;
uint32_t srcOffset = CeilDivision(n, fractalShape[0]) * fractalSize;
AscendC::LoadData2DParams loadDataParams;
loadDataParams.repeatTimes = CeilDivision(n, fractalShape[0]);
loadDataParams.srcStride = 1;
loadDataParams.dstGap = 0;
loadDataParams.ifTranspose = false;
for (int i = 0; i < CeilDivision(k, fractalShape[1]); ++i) {
    AscendC::LoadData(b2Local[i * dstOffset], bGM[i * srcOffset], loadDataParams);
}
```

### L1->L0A

从L1加载A矩阵到L0A，不使能转置
```c++
uint32_t dstOffset = CeilDivision(k, fractalShape[1]) * fractalSize;
uint32_t srcOffset = fractalSize;
AscendC::LoadData2DParams loadDataParams;
loadDataParams.repeatTimes = CeilDivision(k, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(m, fractalShape[0]);
loadDataParams.dstGap = 0;
loadDataParams.ifTranspose = false;
for (int i = 0; i < CeilDivision(m, fractalShape[0]); ++i) {
    AscendC::LoadData(a2Local[i * dstOffset], a1Local[i * srcOffset], loadDataParams);
}
```

从L1加载A矩阵到L0A，使能转置
```c++
AscendC::LocalTensor<T> a1Local = inQueueA1.DeQue<T>();
AscendC::LocalTensor<T> a2Local = inQueueA2.AllocTensor<T>();
uint32_t dstOffset = CeilDivision(k, fractalShape[0]) * fractalSize;
uint32_t srcOffset = CeilDivision(k, fractalShape[0]) * fractalSize;
// Nz -> Zz
AscendC::LoadData2DParams loadDataParams;
// 迭代次数，每个迭代可以处理512B数据
loadDataParams.repeatTimes = CeilDivision(k, fractalShape[0]);
// 相邻迭代间，源操作数前一个分形与后一个分形起始地址的间隔，单位：512B
loadDataParams.srcStride = 1;
// 相邻迭代间，目的操作数前一个分形结束地址与后一个分形起始地址的间隔，单位：512B
loadDataParams.dstGap = 0;
// 是否启用转置功能，对每个分形矩阵进行转置，默认为false
loadDataParams.ifTranspose = true;
for (int i = 0; i < CeilDivision(m, fractalShape[1]); ++i) {
    AscendC::LoadData(a2Local[i * dstOffset], a1Local[i * srcOffset], loadDataParams);
}
```

### L1->L0B

从L1加载B矩阵到L0B，不使能转置
```c++
uint32_t dstOffset = CeilDivision(n, fractalShape[0]) * fractalSize;
uint32_t srcOffset = CeilDivision(n, fractalShape[0]) * fractalSize;
AscendC::LoadData2DParams loadDataParams;
loadDataParams.repeatTimes = CeilDivision(n, fractalShape[0]);
loadDataParams.srcStride = 1;
loadDataParams.dstGap = 0;
loadDataParams.ifTranspose = false;
for (int i = 0; i < CeilDivision(k, fractalShape[1]); ++i) {
    AscendC::LoadData(b2Local[i * dstOffset], b1Local[i * srcOffset], loadDataParams);
}
```

从L1加载B矩阵到L0B，不使能转置
```c++
uint32_t dstOffset = CeilDivision(n, fractalShape[0] * fractalNum) * fractalSize * fractalNum;
uint32_t srcOffset = fractalSize * fractalNum;
// Nz -> Zn
AscendC::LoadData2DParams loadDataParams;
loadDataParams.repeatTimes = CeilDivision(n, fractalShape[0] * fractalNum);
loadDataParams.srcStride = CeilDivision(k, fractalShape[0] * fractalNum);
loadDataParams.dstGap = 0;
loadDataParams.ifTranspose = true;
for (int i = 0; i < CeilDivision(k, fractalShape[0] * fractalNum); ++i) {
    AscendC::LoadData(b2Local[i * dstOffset], b1Local[i * srcOffset], loadDataParams);
}
```

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行
  ```bash
  SCENARIO=1 M=64 K=64 N=64
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO -m=$M -k=$K -n=$N   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```