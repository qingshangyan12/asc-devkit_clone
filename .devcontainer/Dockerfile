# Build:
#   docker buildx build --network host --build-arg REPO_MIRROR=https://storage.googleapis.com/git-repo-downloads/repo -t ascendc:ubuntu24.04 .devcontainer/
#   docker buildx build --network host -t ascendc:ubuntu24.04 .devcontainer/

# Stage 0: extract conda (multi-arch, matches host architecture automatically)
FROM continuumio/miniconda3:latest AS conda-base

# Stage 1: main image
FROM ubuntu:24.04

# ============================================================
# Build arguments — mirror sources
# ============================================================
ARG GENERAL_MIRROR=mirrors.huaweicloud.com
ARG CONDA_MIRROR=https://mirrors.tuna.tsinghua.edu.cn/anaconda
ARG REPO_MIRROR=https://mirrors.tuna.tsinghua.edu.cn/git/git-repo

# ============================================================
# Base environment
# ============================================================
ENV TZ=Asia/Shanghai \
    LANG=C.UTF-8 \
    SHELL=/bin/zsh \
    DEBIAN_FRONTEND=noninteractive \
    PATH=/opt/conda/bin:$PATH

USER root
WORKDIR /tmp

# ============================================================
# APT mirror & system packages (single layer to avoid stale cache)
# ============================================================
RUN arch="$(dpkg --print-architecture)" && \
    if [ "${arch}" = "arm64" ]; then \
      sed -i "s@//ports.ubuntu.com@//${GENERAL_MIRROR}@g" /etc/apt/sources.list.d/ubuntu.sources; \
    else \
      sed -i "s@//archive.ubuntu.com@//${GENERAL_MIRROR}@g; s@//security.ubuntu.com@//${GENERAL_MIRROR}@g" \
        /etc/apt/sources.list.d/ubuntu.sources; \
    fi && \
    apt update && \
    apt install -y --no-install-recommends \
      tzdata lsb-release curl wget gnupg2 openssl ca-certificates openssh-client pciutils \
      build-essential cmake ninja-build ccache pkg-config autoconf automake libtool gperf \
      gdb gdbserver git git-lfs lcov vim htop tree zsh clangd \
    && ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone \
    && apt clean all \
    && rm -rf /var/lib/apt/lists/*

# ============================================================
# repo tool
# ============================================================
RUN curl -fsSL ${REPO_MIRROR} -o /usr/local/bin/repo && \
    chmod a+x /usr/local/bin/repo

# ============================================================
# git global config
# ============================================================
RUN git config --global core.autocrlf input && \
    git config --global core.editor vim && \
    git config --global color.ui auto && \
    git config --global alias.lg "log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"

# ============================================================
# oh-my-zsh + plugins (via gitcode mirror)
# ============================================================
RUN git clone --depth=1 https://gitcode.com/ohmyzsh/ohmyzsh.git /root/.oh-my-zsh && \
    cp /root/.oh-my-zsh/templates/zshrc.zsh-template /root/.zshrc && \
    git clone --depth=1 https://gitcode.com/zsh-users/zsh-autosuggestions \
        /root/.oh-my-zsh/custom/plugins/zsh-autosuggestions && \
    git clone --depth=1 https://gitcode.com/zsh-users/zsh-syntax-highlighting \
        /root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting && \
    sed -i 's/plugins=(git)/plugins=(git z zsh-autosuggestions zsh-syntax-highlighting)/' /root/.zshrc && \
    sed -i 's/ZSH_THEME="robbyrussell"/ZSH_THEME="agnoster"/' /root/.zshrc && \
    chsh -s "$(which zsh)" root

# ============================================================
# conda — copied from stage 0, no architecture-specific download needed
# ============================================================
COPY --from=conda-base /opt/conda /opt/conda
RUN cat >> /root/.condarc <<EOF
channels:
  - defaults
show_channel_urls: true
default_channels:
  - ${CONDA_MIRROR}/pkgs/main
  - ${CONDA_MIRROR}/pkgs/r
  - ${CONDA_MIRROR}/pkgs/msys2
custom_channels:
  conda-forge: ${CONDA_MIRROR}/cloud
  pytorch: ${CONDA_MIRROR}/cloud
EOF
RUN cat >> /etc/pip.conf <<EOF
[global]
index-url = https://${GENERAL_MIRROR}/repository/pypi/simple
trusted-host = ${GENERAL_MIRROR}
default-timeout = 120
EOF

COPY requirements.txt /tmp/
RUN set -eux; \
    conda create -y -n py312 python=3.12 pip && \
    conda run -n py312 pip install --no-cache-dir -r /tmp/requirements.txt && \
    conda clean -afy && \
    rm -rf /root/.cache/pip

# ============================================================
# Shell customization
# ============================================================
RUN conda init zsh && \
    echo "conda activate py312" >> /root/.zshrc && \
    echo "export REPO_URL=\"${REPO_MIRROR}\"" >> /root/.zshrc

# ============================================================
# node — copied from stage 0, no architecture-specific download needed
# ============================================================
ARG NODE_MAJOR=26
RUN arch="$(dpkg --print-architecture)" && \
    case "$arch" in \
      amd64) node_arch="x64" ;; \
      arm64) node_arch="arm64" ;; \
      *) echo "Unsupported architecture: $arch" >&2; exit 1 ;; \
    esac && \
    NODE_TARBALL="$(curl -fsSL "https://${GENERAL_MIRROR}/nodejs/latest-v${NODE_MAJOR}.x/SHASUMS256.txt" | awk -v arch="$node_arch" '$2 ~ "linux-" arch "\\.tar\\.gz$" {print $2; exit}')" && \
    test -n "$NODE_TARBALL" && \
    curl -fsSL "https://${GENERAL_MIRROR}/nodejs/latest-v${NODE_MAJOR}.x/${NODE_TARBALL}" -o "/tmp/${NODE_TARBALL}" && \
    tar -xzf "/tmp/${NODE_TARBALL}" -C /usr/local --strip-components=1 && \
    rm -f "/tmp/${NODE_TARBALL}"

RUN npm config set registry https://${GENERAL_MIRROR}/repository/npm/ && \
    npm cache clean -f && \
    npm install -g @openai/codex@latest @anthropic-ai/claude-code@latest

# AscendC banner
RUN printf '\n\
echo "\\033[1;36m"\n\
echo " █████╗ ███████╗ ██████╗███████╗███╗   ██╗██████╗  ██████╗"\n\
echo "██╔══██╗██╔════╝██╔════╝██╔════╝████╗  ██║██╔══██╗██╔════╝"\n\
echo "███████║███████╗██║     █████╗  ██╔██╗ ██║██║  ██║██║     "\n\
echo "██╔══██║╚════██║██║     ██╔══╝  ██║╚██╗██║██║  ██║██║     "\n\
echo "██║  ██║███████║╚██████╗███████╗██║ ╚████║██████╔╝╚██████╗"\n\
echo "╚═╝  ╚═╝╚══════╝ ╚═════╝╚══════╝╚═╝  ╚═══╝╚═════╝  ╚═════╝"\n\
echo "\\033[0m"\n' >> /root/.zshrc

# Ascend driver environment
RUN printf '\n[ -f /usr/local/Ascend/driver/bin/setenv.bash ] && source /usr/local/Ascend/driver/bin/setenv.bash\n' \
    >> /root/.zshrc

RUN rm -rf /tmp/*

WORKDIR /root
ENTRYPOINT ["/bin/zsh"]
