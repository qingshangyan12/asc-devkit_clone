# docker build --network host -t ascendc_images:ubuntu24.04 .
# docker run -it --rm ascendc_images:ubuntu24.04
FROM ubuntu:24.04

USER root
WORKDIR /tmp

ENV TZ=Asia/Shanghai
ENV LANG=C.UTF-8
RUN sed -i 's@//ports.ubuntu.com@//mirrors.tuna.tsinghua.edu.cn@g' /etc/apt/sources.list.d/ubuntu.sources && apt update

# # 时区 & 语言
# RUN apt install -y tzdata lsb-release
# # 网络 & 安全
# RUN apt install -y curl wget gnupg2 openssl libssl-dev pciutils net-tools
# # 压缩 & 文件处理
# RUN apt install -y unzip zip xz-utils liblzma-dev dos2unix m4 gawk patch
# # 编译工具链
# RUN apt install -y build-essential gcc g++ make cmake ninja-build ccache autoconf automake libtool pkg-config gfortran
# # 开发库
# RUN apt install -y zlib1g zlib1g-dev libsqlite3-dev libffi-dev libbz2-dev libreadline-dev libncurses-dev
# # 数学库（BLAS / LAPACK）
# RUN apt install -y libblas-dev libblas3 liblapack-dev liblapack3 libopenblas-dev
# 调试 & 分析
# RUN apt install -y gdb gdbserver lsof lcov
# 版本控制 & 脚本
# RUN apt install -y git git-lfs expect
# 实用工具
# RUN apt install -y vim tmux htop tree file jq
# RUN apt install -y tzdata lsb-release curl wget gnupg2 openssl pciutils net-tools gawk build-essential cmake ccache autoconf automake libtool pkg-config lcov git git-lfs vim htop tree && rm -rf /var/lib/apt/lists/*

RUN apt install -y \
      tzdata lsb-release \
      curl wget gnupg2 openssl \
      build-essential cmake ninja-build ccache pkg-config \
      gdb gdbserver \
      git git-lfs \
      lcov \
      vim htop tree zsh \
      && rm -rf /var/lib/apt/lists/*
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# oh-my-zsh + plugin
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended && \
    git clone --depth=1 https://github.com/zsh-users/zsh-autosuggestions \
        /root/.oh-my-zsh/custom/plugins/zsh-autosuggestions && \
    git clone --depth=1 https://github.com/zsh-users/zsh-syntax-highlighting \
        /root/.oh-my-zsh/custom/plugins/zsh-syntax-highlighting && \
    sed -i 's/plugins=(git)/plugins=(git z zsh-autosuggestions zsh-syntax-highlighting)/' /root/.zshrc && \
    sed -i 's/ZSH_THEME="robbyrussell"/ZSH_THEME="agnoster"/' /root/.zshrc && \
    chsh -s $(which zsh) root
ENV SHELL=/bin/zsh

# git config
RUN git config --global core.autocrlf input && \
    git config --global core.editor vim && \
    git config --global color.ui auto && \
    git config --global alias.lg "log --color --graph --pretty=format:'%Cred%h%Creset -%C(yellow)%d%Creset %s %Cgreen(%cr) %C(bold blue)<%an>%Creset' --abbrev-commit"

# miniconda
RUN wget -q https://mirrors.tuna.tsinghua.edu.cn/anaconda/miniconda/Miniconda3-latest-Linux-aarch64.sh \
         -O /tmp/miniconda.sh && \
    bash /tmp/miniconda.sh -b -p /opt/conda && \
    rm /tmp/miniconda.sh
ENV PATH=/opt/conda/bin:$PATH
RUN printf "[global]\nindex-url = https://pypi.tuna.tsinghua.edu.cn/simple\n" > /etc/pip.conf
RUN conda config --file /opt/conda/.condarc --remove channels defaults && \
    conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free && \
    conda config --add channels https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/main && \
    conda config --set show_channel_urls yes && \
    conda config --set channel_priority strict && \
    conda create -q -y -n py310 python=3.10 && \
    conda create -q -y -n py312 python=3.12 && \
    conda clean -ya
RUN conda init zsh && \
    echo "conda activate py312" >> /root/.zshrc

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
