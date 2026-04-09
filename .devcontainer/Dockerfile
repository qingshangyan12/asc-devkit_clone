# docker build --network host -t ascendc_images:ubuntu24.04 .
# docker run -it --rm ascendc_images:ubuntu24.04
FROM ubuntu:24.04

# ============================================================
# 镜像源配置
# 清华源:   mirrors.tuna.tsinghua.edu.cn
# 中科大源: mirrors.ustc.edu.cn
# 阿里源:   mirrors.aliyun.com
# ============================================================
ARG APT_MIRROR=mirrors.tuna.tsinghua.edu.cn
ARG PYPI_MIRROR=https://pypi.tuna.tsinghua.edu.cn/simple
ARG CONDA_MIRROR=https://mirrors.tuna.tsinghua.edu.cn/anaconda
ARG REPO_MIRROR=https://mirrors.tuna.tsinghua.edu.cn/git/git-repo

# ============================================================
# 基础环境变量
# ============================================================
USER root
WORKDIR /tmp
ENV TZ=Asia/Shanghai
ENV LANG=C.UTF-8
ENV SHELL=/bin/zsh

# ============================================================
# apt 换源 & 系统包安装
# ============================================================
RUN sed -i "s@//ports.ubuntu.com@//${APT_MIRROR}@g" /etc/apt/sources.list.d/ubuntu.sources && \
    apt update
RUN apt install -y \
      tzdata lsb-release curl wget gnupg2 openssl \
      build-essential cmake ninja-build ccache pkg-config autoconf automake libtool gperf \
      gdb gdbserver git git-lfs lcov vim htop tree zsh \
      && rm -rf /var/lib/apt/lists/*
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# ============================================================
# repo 工具
# ============================================================
RUN curl -fsSL ${REPO_MIRROR} -o /usr/local/bin/repo && \
    chmod a+x /usr/local/bin/repo

# ============================================================
# git 全局配置
# ============================================================
RUN git config --global core.autocrlf input && \
    git config --global core.editor vim && \
    git config --global color.ui auto && \
    git config --global alias.lg "log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"

# ============================================================
# oh-my-zsh + 插件（需要访问 GitHub，偶尔会变动）
# ============================================================
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended && \
    git clone --depth=1 https://github.com/zsh-users/zsh-autosuggestions \
        /root/.oh-my-zsh/custom/plugins/zsh-autosuggestions && \
    git clone --depth=1 https://github.com/zsh-users/zsh-syntax-highlighting \
        /root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting && \
    sed -i 's/plugins=(git)/plugins=(git z zsh-autosuggestions zsh-syntax-highlighting)/' /root/.zshrc && \
    sed -i 's/ZSH_THEME="robbyrussell"/ZSH_THEME="agnoster"/' /root/.zshrc && \
    chsh -s $(which zsh) root

# ============================================================
# miniconda + Python 环境
# ============================================================
RUN wget -q ${CONDA_MIRROR}/miniconda/Miniconda3-latest-Linux-aarch64.sh \
         -O /tmp/miniconda.sh && \
    bash /tmp/miniconda.sh -b -p /opt/conda && \
    rm /tmp/miniconda.sh
ENV PATH=/opt/conda/bin:$PATH
RUN printf "[global]\nindex-url = ${PYPI_MIRROR}\n" > /etc/pip.conf
RUN printf "channels:\n\
  - ${CONDA_MIRROR}/pkgs/main\n\
  - ${CONDA_MIRROR}/pkgs/free\n\
show_channel_urls: true\n\
channel_priority: strict\n" > /root/.condarc && \
    conda create -q -y -n py310 python=3.10 && \
    conda create -q -y -n py312 python=3.12 && \
    conda clean -ya

# ============================================================
# shell 个性化配置
# ============================================================
RUN conda init zsh && \
    echo "conda activate py312" >> /root/.zshrc && \
    echo "export REPO_URL=\"${REPO_MIRROR}\"" >> /root/.zshrc

# AscendC banner
RUN cat >> /root/.zshrc << 'EOF'

echo "\033[1;36m"
echo " █████╗ ███████╗ ██████╗███████╗███╗   ██╗██████╗  ██████╗"
echo "██╔══██╗██╔════╝██╔════╝██╔════╝████╗  ██║██╔══██╗██╔════╝"
echo "███████║███████╗██║     █████╗  ██╔██╗ ██║██║  ██║██║     "
echo "██╔══██║╚════██║██║     ██╔══╝  ██║╚██╗██║██║  ██║██║     "
echo "██║  ██║███████║╚██████╗███████╗██║ ╚████║██████╔╝╚██████╗"
echo "╚═╝  ╚═╝╚══════╝ ╚═════╝╚══════╝╚═╝  ╚═══╝╚═════╝  ╚═════╝"
echo "\033[0m"
EOF

RUN rm -rf /tmp/*
WORKDIR /root
ENTRYPOINT /bin/zsh
