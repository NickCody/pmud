#!/bin/bash

shopt -s globstar
clang-format -i **/*.{h,cc,cpp}

