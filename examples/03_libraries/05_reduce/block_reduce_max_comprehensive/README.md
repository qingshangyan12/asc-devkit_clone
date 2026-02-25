## 概述

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍


## 算子描述

## 测试场景说明

提供四种场景

| 场景 | 类型 | EnableCounterMask |
|------|------|-------|
| 1 | half | false |
| 2 | half | true |
| 3 | float | false |
| 4 | float | true |

<!-- 设计一些测试数据，参数说明 -->
<!-- 
| 场景 | 类型 | repeatTime | tailMask | srcBlkStride | dstRepStride | srcRepStride | 输入 | 输出 | 说明 |
|------|------|-----------|------|--------------|--------------|--------------|-------|------|------|
| 1 | half | 2 | 85 | 1 | 1 | 8 | 341(256 + 85)=>352(32位对齐) | 22(16 + 6)=>32(32位对齐) | 2个repeat+非对齐的mask，连续数据 |
| 2 | half | 1 | 341 | 1 | 1 | 8 | 341(256 + 85) | 22(16 + 6) | 2个repeat+非对齐的mask，连续数据 |
| 3 | float | 2 | 12 | 1 | 1 | 8 | 140(128 + 12)=>144 | 18(16 + 2)=>24 | float基础场景, 2个repeat+非对齐的mask，连续数据 |
| 4 | float | 1 | 140 | 1 | 1 | 8 | 140=>144 | 18=>24 | float基础场景, 2个repeat+非对齐的mask，连续数据 |
| 1 | half | 2 | 0 | 2 | 1 | 8 | 384 | 16 | block间有gap |
| 1 | half | 2 | 0 | 1 | 2 | 8 | 256 | 24=>32 | 输出repeat间有gap |
| 1 | half | 2 | 0 | 1 | 1 | 16 | 384 | 16 | src repeat间有gap | 
-->

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
SRC_DATA_SIZE=352
DST_DATA_SIZE=32

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
