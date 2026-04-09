# Dev Container

Ubuntu 24.04 development environment for AscendC NPU kernel development.

## Prerequisites

| Requirement | Version |
|-------------|---------|
| Docker | ≥ 23.0.0 |
| docker buildx | any |
| Ascend driver | installed on host |

Check your Docker version:
```bash
docker --version
```

Install buildx if missing:
```bash
mkdir -p /usr/local/lib/docker/cli-plugins
ARCH=$(uname -m | sed 's/x86_64/amd64/;s/aarch64/arm64/')
curl -fsSL "https://github.com/docker/buildx/releases/latest/download/buildx-linux-${ARCH}" \
     -o /usr/local/lib/docker/cli-plugins/docker-buildx
chmod +x /usr/local/lib/docker/cli-plugins/docker-buildx
```

## Build the Image

```bash
docker buildx build --network host -t ascendc:ubuntu24.04 .devcontainer/
```

To override mirror sources:
```bash
docker buildx build --network host \
  --build-arg APT_MIRROR=mirrors.ustc.edu.cn \
  --build-arg CONDA_MIRROR=https://mirrors.ustc.edu.cn/anaconda \
  --build-arg PYPI_MIRROR=https://pypi.mirrors.ustc.edu.cn/simple \
  -t ascendc:ubuntu24.04 .devcontainer/
```

Available mirrors:

| | Huawei (default) | Tsinghua | USTC |
|-|------------------|----------|------|
| APT | `mirrors.huaweicloud.com` | `mirrors.tuna.tsinghua.edu.cn` | `mirrors.ustc.edu.cn` |
| Conda | — | `mirrors.tuna.tsinghua.edu.cn/anaconda` | `mirrors.ustc.edu.cn/anaconda` |
| PyPI | `repo.huaweicloud.com/repository/pypi/simple` | `pypi.tuna.tsinghua.edu.cn/simple` | `pypi.mirrors.ustc.edu.cn/simple` |

## Open in VS Code

Install the [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) extension, then:

```
Ctrl+Shift+P → Dev Containers: Reopen in Container
```

## Python Environments

Two conda environments are pre-installed:

| Environment | Python | PyTorch |
|-------------|--------|---------|
| `py310` | 3.10 | 2.6.0 |
| `py312` | 3.12 | 2.7.1 |

Default active environment is `py312`.

## Data Directory

The `/data` mount is not included as it varies per developer. Add your own in `devcontainer.json`:

```jsonc
// mounts
"source=/your/data,target=/your/data,type=bind"
```

## NPU Devices

`ASCEND_VISIBLE_DEVICES=all` exposes all available NPU cards automatically. Override at runtime if needed:

```bash
docker run -e ASCEND_VISIBLE_DEVICES=0,1 ...
```
