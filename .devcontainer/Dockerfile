# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.134.0/containers/cpp/.devcontainer/base.Dockerfile
ARG VARIANT="bullseye"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN curl -fsSL https://download.docker.com/linux/debian/gpg | sudo apt-key add -


RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
        apt-transport-https \
        ca-certificates \
        curl \
        gnupg2 \
        libssl-dev \
        vim \
        software-properties-common \
        graphviz xdot \
        ninja-build \
    &&  add-apt-repository \
        "deb [arch=amd64] https://download.docker.com/linux/debian \
        $(lsb_release -cs) \
        stable" \
    && curl -fsSL https://bazel.build/bazel-release.pub.gpg | gpg --dearmor > bazel.gpg \
    && sudo mv bazel.gpg /etc/apt/trusted.gpg.d/ \
    && echo "deb [arch=amd64] https://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list \
    && apt-get update \
    && apt-get -y install docker-ce-cli bazel \
    && rm -rf /var/lib/apt/lists/*

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

RUN locale-gen en_US.UTF-8
ENV LANG='en_US.UTF-8' LANGUAGE='en_US:en' LC_ALL='en_US.UTF-8'

RUN groupadd docker \
    && usermod -aG docker vscode

USER vscode
WORKDIR /home/vscode

RUN cd /home/vscode \
    && echo "export PATH=$PATH:/usr/local/go/bin:/home/vscode/go/bin" >> /home/vscode/.bashrc \
    && /usr/local/go/bin/go get github.com/bazelbuild/buildtools/buildifier >> /home/vscode/results
