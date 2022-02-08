#!/bin/bash

set -e
set -u
set -o pipefail

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export REDIS_HOST=$1
export REDIS_PORT=$2
export ENV=$3

echo "Using:"
echo "ENV=$ENV"
echo "REDIS_HOST=$REDIS_HOST"
echo "REDIS_PORT=$REDIS_PORT"

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

