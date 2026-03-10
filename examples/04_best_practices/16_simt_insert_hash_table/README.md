# SIMT实现InsertHashTable算子样例

## 概述

利用SIMT能够离散寻址的能力，能够实现较为复杂的算子逻辑，本样例利用SIMT的离散访存能力和大量多线程优势，实现高效地向hash table 中插入大量元素。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构

```
├── 16_simt_hash_map
│   ├── CMakeLists.txt          # cmake编译文件
│   ├── insert_hash_table.asc   # Ascend C算子实现 & 调用样例
|   └── README.md
```

## 算子描述

- 算子功能：  
  Hash table是一种高效的关联容器数据结构，它能以接近常数时间复杂度O(1)实现对数据的存储和查找。本算子高效地向hash table中插入大量键值对。

- 算子规格：  
  key：int64_t
  value: float[4]
  将 8\*1024 个键值对插入 容量为 16\*1024 的hash table中

- 基础知识：  
  hash table是用于存储键值对的关联容器，key映射到关联的value，因此可以通过查找key来获取value。
  hash table在内存中的核心结构是一个“桶（Bucket）”数组。每个桶可以存储一个键值对。
  插入操作的基本流程是：首先，计算待插入键值对的key的哈希值；接着，通过对哈希值进行取模等运算，定位到数组中的一个特定桶，如果该桶当前为空，则将键值对存入其中。

- 哈希冲突
  不同的key可能计算出相同的哈希值，或者哈希值不同但映射到了同一个数组索引上，这种情况被称为“哈希冲突”。解决哈希冲突的策略有多种，本样例使用开放寻址法，它更适合NPU实现。开放寻址法中当一个位置被占用时，算法按照线性探测规则去寻找下一个空闲的位置

- 算子实现：  
  算子使用MurMurHash3算法计算key的哈希值。每个线程负责一个键值对的插入，写入数据前先通过原子指令获取桶的写入权限的机制来解决多线程场景下的哈希冲突问题。

- 调用实现：    
  使用内核调用符<<<>>>调用核函数。

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
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..; make -j;            # 编译工程
  ./demo                        # 执行样例
  ```
  执行结果如下，说明精度对比成功。
  ```
  [Success] find all key-value in hash table.
  ```