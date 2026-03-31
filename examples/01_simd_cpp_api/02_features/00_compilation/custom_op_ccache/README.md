# custom_op_ccache — 自定义算子缓存编译样例

本样例演示如何在 Ascend C 自定义算子工程中集成 **ccache**，提供两种缓存编译模式：

- **单机缓存**：编译结果缓存到本地磁盘，重建时直接命中，无需重复编译
- **分布式缓存**：以 Redis 作为远端共享缓存，多机共用同一缓存池

算子：`AddCustom`（两个 FP16 张量逐元素相加）

---

## 目录结构

```
custom_op_ccache/
├── CMakeLists.txt            # 顶层构建文件，含 ccache 集成逻辑
├── op_host/
│   ├── CMakeLists.txt
│   └── add_custom/
│       └── add_custom_host.cpp   # Tiling / InferShape / OpDef 注册
└── op_kernel/
    ├── CMakeLists.txt
    └── add_custom/
        ├── add_custom_kernel.cpp  # AICore kernel 实现
        └── add_custom_tiling.h    # Tiling 数据结构定义
```

---

## 第一部分：单机缓存

### 环境要求

- ccache >= 3.7
- CMake >= 3.16

### 安装 ccache

```bash
apt install ccache

# 可选：调整缓存大小（默认 5G）
ccache --max-size=10G

# 可选：开启压缩（节省磁盘空间）
ccache --set-config=compression=true
```

### 编译

在本样例根目录下执行：

```bash
source /usr/local/Ascend/cann/set_env.sh
cmake -B build .
cmake --build build -j8
```

CMakeLists.txt 会自动检测 ccache 并设置为编译器 launcher。若未安装 ccache，构建仍正常进行，仅打印 WARNING。

### 验证

在本样例根目录下执行：

```bash
# 清空缓存和统计
ccache --clear && ccache --zero-stats

# 首次编译（cache miss）
cmake -B build . && cmake --build build -j8
ccache --show-stats
# 预期：Misses > 0，Hits = 0

# 清空构建目录后重建（cache hit）
rm -rf build
cmake -B build . && cmake --build build -j8
ccache --show-stats
# 预期：Hits 100%
```

典型输出（二次编译后）：

```
Summary:
  Hits:               4 /    4 (100.0 %)
    Direct:           4 /    4 (100.0 %)
  Misses:             0
Primary storage:
  Hits:               8 /    8 (100.0 %)
  Misses:             0
  Cache size (GB): 0.00 /  5.00 (  0.0%)
```

> **说明**：ccache 覆盖 host 侧 `.cpp` 文件（tiling、aclnn、op_proto 等）。
> kernel 侧由 ASC 工具链自身 pipeline 处理，不经过 `CMAKE_CXX_COMPILER_LAUNCHER`，
> 不在 ccache 统计中体现。

### 常用命令

| 命令 | 说明 |
|------|------|
| `ccache --show-stats` | 查看命中/未命中统计 |
| `ccache --clear` | 清空本地缓存 |
| `ccache --max-size=10G` | 设置缓存上限 |
| `ccache --zero-stats` | 重置统计计数器 |
| `CCACHE_DISABLE=1 cmake --build build` | 临时禁用 ccache |

---

## 第二部分：分布式缓存（ccache + Redis）

### 环境要求

- ccache >= 4.0（需支持 `secondary_storage`，可通过 `ccache --version` 确认输出中含 `redis-storage`）
- Redis 服务，网络可达
- 参与编译的各机器使用**相同版本**的编译器

### 部署 Redis 服务

**方式一：apt 安装**：

```bash
apt install redis-server
redis-server --daemonize yes --bind 0.0.0.0 --port 6379
```

**方式二：Docker**：

```bash
docker run -d --name ccache-redis -p 6379:6379 redis:latest
```

**验证 Redis 可达**（在各编译机器上执行，将 `<REDIS_IP>` 替换为 Redis 服务器的实际 IP 地址）：

```bash
redis-cli -h <REDIS_IP> -p 6379 ping
# 返回 PONG 表示连通
```

### 配置 ccache

在**所有参与编译的机器**上执行以下配置（将 `<REDIS_IP>` 替换为 Redis 服务器的实际 IP 地址）：

```bash
# 配置 secondary storage（持久化写入配置文件）
ccache --set-config=secondary_storage=redis://<REDIS_IP>:6379

# 若各机器编译器安装路径不同但版本一致，需按内容匹配编译器
ccache --set-config=compiler_check=content
```

> 若仅作为缓存消费方（只读），可在 URL 后追加参数（`<REDIS_IP>` 替换为实际 IP）：
> `ccache --set-config=secondary_storage="redis://<REDIS_IP>:6379?read-only=true"`

### 编译

各机器编译步骤与单机模式相同：

```bash
source /usr/local/Ascend/cann/set_env.sh
cmake -B build .
cmake --build build -j8
```

ccache 命中本地缓存（L1）时直接返回；L1 miss 时查询 Redis（L2）；两级均 miss 时调用编译器并将结果同时写入 L1 和 L2。

### 验证

**机器 A（首次编译，写入 Redis）**，在本样例根目录下执行：

```bash
source /usr/local/Ascend/cann/set_env.sh
ccache --clear && ccache --zero-stats
cmake -B build . && cmake --build build -j8
ccache --show-stats
# 预期：Secondary storage Misses > 0，结果已推送至 Redis
```

**确认 Redis 中已有缓存**（`<REDIS_IP>` 替换为实际 IP）：

```bash
redis-cli -h <REDIS_IP> -p 6379 dbsize
# 返回值 > 0
```

**机器 B（命中 Redis 缓存）**，在本样例根目录下执行：

```bash
source /usr/local/Ascend/cann/set_env.sh
ccache --clear && ccache --zero-stats
cmake -B build . && cmake --build build -j8
ccache --show-stats
# 预期：Secondary storage Hits > 0
```