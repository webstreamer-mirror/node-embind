FROM ubuntu:xenial

LABEL maintainer="Mingyi Zhang <mingyi.z@outlooke.com>"

ENV PYENV_ROOT=/opt/pyenv \
    PATH=/opt/pyenv/shims:${PATH}

RUN dpkg --add-architecture i386 \
    && echo "# aliyun"> /etc/apt/sources.list \
    && echo deb-src http://archive.ubuntu.com/ubuntu xenial main restricted >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial main restricted >> /etc/apt/sources.list \
    && echo deb-src http://mirrors.aliyun.com/ubuntu/ xenial main restricted multiverse universe >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted >> /etc/apt/sources.list \
    && echo deb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted multiverse universe >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial universe >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial multiverse >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial-updates multiverse >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse >> /etc/apt/sources.list \
    && echo deb-src http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse >> /etc/apt/sources.list \
    && echo deb http://archive.canonical.com/ubuntu xenial partner >> /etc/apt/sources.list \
    && echo deb-src http://archive.canonical.com/ubuntu xenial partner >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted >> /etc/apt/sources.list \
    && echo deb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted multiverse universe >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial-security universe >> /etc/apt/sources.list \
    && echo deb http://mirrors.aliyun.com/ubuntu/ xenial-security multiverse >> /etc/apt/sources.list \
    && apt-get update \
    && apt-get upgrade \
    && apt-get -qq install -y --no-install-recommends \
       sudo=1.8.* \
       build-essential=12.* \
       wget=1.17.* \
       git=1:2.7.* \
       libc6-dev-i386=2.23-* \
       g++-multilib=4:5.3.* \
       libgmp-dev=2:6.1.* \
       libmpfr-dev=3.1.* \
       libmpc-dev=1.0.* \
       libc6-dev=2.23-* \
       nasm=2.11.* \
       dh-autoreconf=11 \
       ninja-build=1.5.*  \
       libffi-dev=3.2.* \
       libssl-dev=1.0.2* \
       pkg-config=0.29.1-0ubuntu1 \
       subversion=1.9.3-2ubuntu1.1 \
       zlib1g-dev=1:1.* \
       libbz2-dev=1.* \
       libsqlite3-dev=3.* \
       libreadline-dev=6.* \
       xz-utils=5.* \
       curl=7.* \
       libncurses5-dev=6.* \
       libncursesw5-dev=6.* \
       liblzma-dev=5.* \
       ca-certificates \
       && rm -rf /var/lib/apt/lists/* \
       && groupadd 1001 -g 1001 \
       && groupadd 1000 -g 1000 \
       && groupadd 2000 -g 2000 \
       && groupadd 999 -g 999 \
       && useradd -ms /bin/bash ci -g 1001 -G 1000,2000,999 \
       && printf "ci:ci" | chpasswd \
       && adduser ci sudo \
       && printf "ci ALL= NOPASSWD: ALL\\n" >> /etc/sudoers \
       && wget --no-check-certificate --quiet -O /tmp/pyenv-installer https://github.com/pyenv/pyenv-installer/raw/master/bin/pyenv-installer \
       && chmod +x /tmp/pyenv-installer \
       && /tmp/pyenv-installer \
       && rm /tmp/pyenv-installer \
       && update-alternatives --install /usr/bin/pyenv pyenv /opt/pyenv/bin/pyenv 100 \
       && PYTHON_CONFIGURE_OPTS="--enable-shared" pyenv install 2.7.15 \
       && pyenv global 2.7.15 \
       && chown -R ci:1001 /opt/pyenv \
       && find /opt/pyenv -iname __pycache__ -print0 | xargs -0 rm -rf\
       && update-alternatives --install /usr/bin/python python /opt/pyenv/shims/python 100 \
       && update-alternatives --install /usr/bin/pip pip /opt/pyenv/shims/pip 100 
       
USER ci
WORKDIR /home/ci

RUN printf 'eval "$(pyenv init -)"\n' >> ~/.bashrc \
    && printf 'eval "$(pyenv virtualenv-init -)"\n' >> ~/.bashrc
