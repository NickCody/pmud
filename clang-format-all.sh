#!/bin/bash
(shopt -s globstar;  /usr/lib/llvm-14/bin/clang-format -i **/*.{h,cc,cpp})
