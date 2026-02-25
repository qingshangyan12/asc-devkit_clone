# BlockReduceMax 综合样例

## 概述

本样例全面展示 BlockReduceMax API 的使用方法，涵盖 half 和 float 两种数据类型，以及多种 mask、stride 配置场景。BlockReduceMax 是一个 Vector 算子，用于对每个 block 内的所有元素求最大值。

### BlockReduceMax 基本特性

- **每个 repeat 处理 256 byte 数据**
- **256 byte 分为 8 个 block，每个 block 32 byte**
- **每个 block 求最大值，输出 1 个值**
- **1 个 repeat：输入 256 byte，输出 8 个值**

### 数据类型支持

| 数据类型 | 每个 block 元素数 | 1 个 repeat 输入 | 1 个 repeat 输出 |
|---------|------------------|-------------------|-------------------|
| half (2B) | 16 | 128 个 half | 8 个 half |
| float (4B) | 8 | 64 个 float | 8 个 float |

---

## 场景说明

| 场景 | 类型 | repeatTime | tailMask | srcBlkStride | dstRepStride | srcRepStride | 输入 | 输出 | 说明 |
|------|------|-----------|------|--------------|--------------|--------------|-------|------|------|
<!-- 设计一些测试数据 -->
| 1 | half | 2 | 85 | 1 | 1 | 8 | 341(256 + 85)=>352(32位对齐) | 22(16 + 6)=>32(32位对齐) | 2个repeat+非对齐的mask，连续数据 |
| 2 | half | 1 | 341 | 1 | 1 | 8 | 341(256 + 85) | 22(16 + 6) | 2个repeat+非对齐的mask，连续数据 |
| 3 | float | 2 | 12 | 1 | 1 | 8 | 140(128 + 12)=>144 | 18(16 + 2)=>24 | float基础场景, 2个repeat+非对齐的mask，连续数据 |
| 4 | float | 1 | 140 | 1 | 1 | 8 | 140=>144 | 18=>24 | float基础场景, 2个repeat+非对齐的mask，连续数据 |

| 1 | half | 2 | 0 | 2 | 1 | 8 | 384 | 16 | block间有gap |
| 1 | half | 2 | 0 | 1 | 2 | 8 | 256 | 24=>32 | 输出repeat间有gap |
| 1 | half | 2 | 0 | 1 | 1 | 16 | 384 | 16 | src repeat间有gap |

---

## API 参数详解

### mask

**含义**：控制一个 repeat 内参与计算的元素个数

**单位**：元素个数

**说明**：
- half 类型：mask ≤ 128
- float 类型：mask ≤ 64
- 输出元素数 = ceil(mask / 每个block元素数)
- half：输出数 = ceil(mask / 16)
- float：输出数 = ceil(mask / 8)

**示例**：
- half 类型，mask = 128 → 8 个 block 全部参与 → 输出 8 个值
- half 类型，mask = 64 → 4 个 block 参与 → 输出 4 个值
- half 类型，mask = 100 → 6.25 个 block → 输出 7 个值（场景11）

### repeatTime

**含义**：重复执行次数

**单位**：次数

**说明**：
- 总输入 = repeatTime × 256 byte
- 总输出 = repeatTime × 输出元素数

**示例**：
- repeatTime = 2 → 处理 512 byte（16 个 block）

### srcBlkStride

**含义**：同一个 repeat 内，block 之间的步长

**单位**：block 数

**说明**：
- 控制在同一个 repeat 内，从第 i 个 block 跳到第 i+1 个 block 时，需要跳过多少个 block
- srcBlkStride = 1：连续 block
- srcBlkStride = 2：block 之间有 1 个 block 的 gap

**示例**：
- 场景5：srcBlkStride = 2，block 之间有 gap

### dstRepStride

**含义**：不同 repeat 之间，dst 首地址偏移

**单位**：元素数

**说明**：
- 控制第 i 次 repeat 的输出和第 i+1 次 repeat 的输出之间的间隔
- dstRepStride = 1：连续输出
- dstRepStride = 2：输出之间有 1 个元素的 gap

**示例**：
- 场景6：dstRepStride = 2，输出 repeat 间有 gap

### srcRepStride

**含义**：不同 repeat 之间，src 首地址偏移

**单位**：block 数（32 byte）

**说明**：
- 控制第 i 次 repeat 的输入和第 i+1 次 repeat 的输入之间的间隔
- 对于 half 类型，每个 repeat 读取 8 个 block
- srcRepStride = 8：连续数据（第 0 次从 block 0 开始，第 1 次从 block 8 开始）
- srcRepStride = 16：repeat 之间有 gap（第 0 次从 block 0 开始，第 1 次从 block 16 开始）

**示例**：
- 场景7：srcRepStride = 16，src repeat 间有 gap

---

## mask 不对齐处理

当 mask 不是每个 block 元素数的整数倍时，会出现 mask 不对齐的情况。

**示例（场景11）**：
- 数据类型：half
- mask = 100
- 每个 block 元素数：16
- 参与计算的 block 数：ceil(100 / 16) = 7
- 前 6 个 block：全部 16 个元素参与计算
- 第 7 个 block：只有 100 - 6×16 = 4 个元素参与计算
- 输出：7 个最大值

---

## 编译运行

### 配置环境变量

请根据当前环境上 CANN 开发包的安装方式，选择对应配置环境变量的命令。

- 默认路径，root 用户安装 CANN 软件包
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- 默认路径，非 root 用户安装 CANN 软件包
  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

- 指定路径 install_path，安装 CANN 软件包
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

### 样例执行

```bash
# 选择场景（1-11）
SCENARIO=1
REPEAT_TIME=2
TAIL_MASK=85
SRC_BLK_STRIDE=1
DST_REP_STRIDE=1
SRC_REP_STRIDE=8
SRC_DATA_SIZE=341
DST_DATA_SIZE=22

# 创建并进入 build 目录
mkdir -p build && cd build

# 编译工程
cmake .. -DSCENARIO_NUM=$SCENARIO -DREPEAT_TIME=$REPEAT_TIME -DTAIL_MASK=$TAIL_MASK -DSRC_BLK_STRIDE=$SRC_BLK_STRIDE -DDST_REP_STRIDE=$DST_REP_STRIDE -DSRC_REP_STRIDE=$SRC_REP_STRIDE -DSRC_DATA_SIZE=$SRC_DATA_SIZE -DDST_DATA_SIZE=$DST_DATA_SIZE
make -j

# 生成测试输入数据
python3 ../scripts/gen_data.py --scenario $SCENARIO --repeatTime $REPEAT_TIME --tailMask $TAIL_MASK --src_size $SRC_DATA_SIZE --dst_size $DST_DATA_SIZE --srcBlkStride $SRC_BLK_STRIDE --dstRepStride $DST_REP_STRIDE --srcRepStride $SRC_REP_STRIDE

# 执行样例
./demo

# 验证输出结果
python3 ../scripts/verify_result.py
```

执行结果如下，说明精度对比成功：
```bash
test pass!
```

---

## 支持的产品

- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品
