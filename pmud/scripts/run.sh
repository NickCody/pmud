#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export LD_LIBRARY_PATH=$SCRIPT_DIR

echo "Killing existing instance..."
pkill pmud || true
echo "Waiting..."

while true; do 
  ./pmud --config-file=.caf_pmud
  if [ $? -eq 98 ]; then
    echo "Waiting for port..."
    sleep 5
  else
    echo "Started!"
    break;
  fi
done

