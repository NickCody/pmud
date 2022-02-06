# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.134.0/containers/cpp/.devcontainer/base.Dockerfile
ARG VARIANT="bullseye"

FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN curl -fsSL https://download.docker.com/linux/debian/gpg | sudo apt-key add -

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add - \
    && echo "deb http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye main" >> /etc/apt/sources.list \
    && echo "deb-src http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye main" >> /etc/apt/sources.list \
    && echo "deb http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye-12 main"  >> /etc/apt/sources.list \
    && echo "deb-src http://apt.llvm.org/bullseye/ llvm-toolchain-bullseye-12 main" >> /etc/apt/sources.list \
    && curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg \
    && sudo mv bazel.gpg /etc/apt/trusted.gpg.d/ \
    && echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list \
    && apt-get update \
    && apt install -y software-properties-common \
    && add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/debian $(lsb_release -cs) stable" \
    #&& sudo add-apt-repository "deb [arch=amd64] https://archive.ubuntu.com/ubuntu $(lsb_release -sc) universe" \
    && apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    apt-transport-https \
    ca-certificates \
    curl \
    gnupg2 \
    libssl-dev \
    vim \
    graphviz xdot \
    ninja-build \
    clang-format \
    docker-ce-cli \
    bazel \
    tree \
    moreutils \
    telnet \
    rlwrap \
    iputils-ping \
    ssh-askpass \
    && rm -rf /var/lib/apt/lists/* \
    && apt-get clean

RUN curl -L -O https://golang.org/dl/go1.15.5.linux-amd64.tar.gz \
    && tar -C /usr/local -xzf go1.15.5.linux-amd64.tar.gz \
    && rm go1.15.5.linux-amd64.tar.gz

RUN curl -L -O https://github.com/bazelbuild/buildtools/releases/download/3.5.0/buildifier \
    && chmod +x buildifier \
    && mv buildifier /usr/local/bin

RUN curl -L -O https://github.com/actor-framework/actor-framework/archive/c2be26e7f9e3e84ef14730590fc4e7b25fb9d29a.zip \
    && unzip -d caf c2be26e7f9e3e84ef14730590fc4e7b25fb9d29a.zip \
    && cd caf/actor-framework-c2be26e7f9e3e84ef14730590fc4e7b25fb9d29a/ \
    && ./configure --generator=Ninja \
    && cd build \
    && cmake -G "Ninja" .. \
    && ninja install \
    && cd ../../.. \
    && rm -rf caf

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

RUN locale-gen en_US.UTF-8
ENV LANG='en_US.UTF-8' LANGUAGE='en_US:en' LC_ALL='en_US.UTF-8'

RUN groupadd docker \
    && usermod -aG docker vscode

USER vscode
WORKDIR /home/vscode

RUN curl -O https://dl.google.com/dl/cloudsdk/channels/rapid/downloads/google-cloud-sdk-371.0.0-linux-x86_64.tar.gz \
    && tar xvfz google-cloud-sdk-371.0.0-linux-x86_64.tar.gz \
    && cd google-cloud-sdk \
    && ./install.sh -q --rc-path ~/.bashrc \
    && cd ~ \
    && rm google-cloud-sdk-371.0.0-linux-x86_64.tar.gz

RUN cd /home/vscode \
    && echo "export PATH=\$PATH:/usr/local/go/bin:/home/vscode/go/bin" >> /home/vscode/.bashrc \
    && echo "if [ -f ~/.localrc ]; then source ~/.localrc; fi" >> /home/vscode/.bashrc

RUN /usr/local/go/bin/go get github.com/bazelbuild/buildtools/buildifier >> /home/vscode/results

