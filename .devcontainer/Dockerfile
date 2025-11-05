# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.134.0/containers/cpp/.devcontainer/base.Dockerfile
ARG VARIANT="bullseye"

FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN mv /etc/apt/sources.list /tmp/ \
  && apt update \
  && mv /tmp/sources.list /etc/apt/ \
  && apt update

RUN ARCH=$(dpkg --print-architecture) \
  && curl -fsSL https://download.docker.com/linux/debian/gpg | sudo apt-key add - \
  && curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg \
  && sudo mv bazel.gpg /etc/apt/trusted.gpg.d/ \
  && echo "deb [arch=${ARCH}] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list \
  && curl -fsSL https://packages.redis.io/gpg | sudo gpg --dearmor -o /usr/share/keyrings/redis-archive-keyring.gpg \
  && echo "deb [signed-by=/usr/share/keyrings/redis-archive-keyring.gpg] https://packages.redis.io/deb $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/redis.list \
  && apt-get update \
  && apt install -y software-properties-common \
  && add-apt-repository "deb [arch=${ARCH}] https://download.docker.com/linux/debian $(lsb_release -cs) stable" \
  && apt-get update \
  && export DEBIAN_FRONTEND=noninteractive \
  && apt-get -y install --no-install-recommends \
  apt-transport-https \
  ca-certificates \
  ninja-build \
  libssl-dev \
  bazel

## redis # installed manually below for 6.9/7.x
# && rm -rf /var/lib/apt/lists/* \
# && apt-get clean

RUN sudo apt install -y \
  yamllint \
  curl \
  gnupg2 \
  vim \
  graphviz xdot \
  docker-ce-cli \
  tree \
  moreutils \
  telnet \
  rlwrap \
  iputils-ping \
  ssh-askpass \
  npm \
  lua5.4 \
  liblua5.4-dev \
  tmux \
  libeigen3-dev

RUN curl -L -O https://github.com/gabime/spdlog/archive/refs/tags/v1.9.2.tar.gz \
  && tar xvfz v1.9.2.tar.gz \
  && cd spdlog-1.9.2 && mkdir build && cd build \
  && cmake -G "Ninja" .. \
  && ninja install \
  && cd ../.. \
  && rm -rf spdlog-1.9.2

RUN GO_ARCH=$(dpkg --print-architecture) \
  && curl -L -O https://golang.org/dl/go1.15.5.linux-${GO_ARCH}.tar.gz \
  && tar -C /usr/local -xzf go1.15.5.linux-${GO_ARCH}.tar.gz \
  && rm go1.15.5.linux-${GO_ARCH}.tar.gz

RUN BUILDIFIER_ARCH=$(dpkg --print-architecture) \
  && case "${BUILDIFIER_ARCH}" in \
    amd64) BUILDIFIER_URL=https://github.com/bazelbuild/buildtools/releases/download/3.5.0/buildifier-linux-amd64 ;; \
    arm64) BUILDIFIER_URL=https://github.com/bazelbuild/buildtools/releases/download/3.5.0/buildifier-linux-arm64 ;; \
    *) echo "Unsupported architecture: ${BUILDIFIER_ARCH}" && exit 1 ;; \
  esac \
  && curl -L -o buildifier ${BUILDIFIER_URL} \
  && chmod +x buildifier \
  && mv buildifier /usr/local/bin

RUN wget https://github.com/actor-framework/actor-framework/archive/refs/tags/0.18.6.tar.gz \
  && tar xvfz 0.18.6.tar.gz \
  && cd actor-framework-0.18.6 \
  && ./configure --generator=Ninja \
  && cd build \
  && cmake -G Ninja .. \
  && ninja install \
  && cd ../.. \
  && rm -rf  actor-framework-0.18.6 \
  && rm 0.18.6.tar.gz

RUN curl -L -O https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.7.0.tar.gz \
  && tar xvfz yaml-cpp-0.7.0.tar.gz \
  && cd yaml-cpp-yaml-cpp-0.7.0 \
  && mkdir build \
  && cd build \
  && cmake -G "Ninja" .. \
  && ninja install \
  && cd ../.. \
  && rm -rf yaml-cpp-yaml-cpp-0.7.0  \
  && rm yaml-cpp-0.7.0.tar.gz

RUN curl -L -O https://github.com/redis/hiredis/archive/refs/heads/master.zip \
  && unzip master.zip \
  && cd hiredis-master \
  && make \
  && make install \
  && cd .. \
  && rm master.zip \
  && rm -rf hiredis-master

RUN apt install -y libzmq3-dev \
  && git clone https://github.com/zeromq/libzmq.git \
  && cd libzmq \
  && ./autogen.sh \
  && ./configure \
  && make \
  && make install 

RUN git clone https://github.com/zeromq/cppzmq.git \
  && cd cppzmq \
  && mkdir build \
  && cd build \
  && cmake .. \
  && make -j"$(nproc)" install

RUN wget https://github.com/redis/redis/archive/7.0-rc2.tar.gz \
  && tar xvfz 7.0-rc2.tar.gz \
  && cd redis-7.0-rc2 \
  && make \
  && make install \
  && cd .. \
  && rm -rf redis-7.0-rc2/ \
  && rm 7.0-rc2.tar.gz

RUN git clone --depth=1 https://github.com/gcc-mirror/gcc.git \
  && sudo apt install -y flex \
  && cd gcc \
  && ./contrib/download_prerequisites \
  && mkdir build && cd build \
  && GNU_ARCH=$(gcc -dumpmachine) \
  && ../configure -v --build=${GNU_ARCH} --host=${GNU_ARCH} --target=${GNU_ARCH} --enable-checking=release --enable-languages=c,c++ --disable-multilib \
  && make -j"$(nproc)" \
  && sudo make install \
  && cd ../.. \
  && rm -rf gcc

RUN wget https://apt.llvm.org/llvm.sh \
  && chmod +x llvm.sh \
  && ./llvm.sh 14 all

RUN BZL_ARCH=$(dpkg --print-architecture) \
  && wget https://get.bzl.io/linux_${BZL_ARCH}/v1.3.19/bzl \
  && mv bzl /usr/local/bin \
  && chmod +x /usr/local/bin/bzl

RUN apt update \
  # && apt install -y doxygen clang-format-15 clangd-15 clang-tidy-15 protobuf-compiler
  && apt install -y doxygen protobuf-compiler

RUN locale-gen en_US.UTF-8
ENV LANG='en_US.UTF-8' LANGUAGE='en_US:en' LC_ALL='en_US.UTF-8'

RUN groupadd docker \
  && usermod -aG docker vscode

USER vscode
WORKDIR /home/vscode

RUN GCLOUD_ARCH=$(dpkg --print-architecture) \
  && case "${GCLOUD_ARCH}" in \
    amd64) GCLOUD_FILE=google-cloud-sdk-371.0.0-linux-x86_64.tar.gz ;; \
    arm64) GCLOUD_FILE=google-cloud-sdk-371.0.0-linux-arm.tar.gz ;; \
    *) echo "Unsupported architecture: ${GCLOUD_ARCH}" && exit 1 ;; \
  esac \
  && curl -O https://dl.google.com/dl/cloudsdk/channels/rapid/downloads/${GCLOUD_FILE} \
  && tar xvfz ${GCLOUD_FILE} \
  && cd google-cloud-sdk \
  && ./install.sh -q \
  && cd ~ \
  && rm ${GCLOUD_FILE}

RUN /usr/local/go/bin/go get github.com/bazelbuild/buildtools/buildifier >> /home/vscode/results
