#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

docker build -t vscode-cpp-container -f .devcontainer/Dockerfile .
docker run -it --rm -v $SCRIPT_DIR:/workspaces/vscode-cpp-container -w /workspaces/vscode-cpp-container vscode-cpp-container "./run-container.sh"