#!/bin/bash

set -e
set -u
set -o pipefail

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

export REDIS_HOST=${REDIS_HOST:-$( curl http://metadata.google.internal/computeMetadata/v1/instance/attributes/REDIS_HOST -H "Metadata-Flavor: Google" 2> /dev/null)}
export REDIS_PORT=${REDIS_HOST:-$( curl http://metadata.google.internal/computeMetadata/v1/instance/attributes/REDIS_PORT -H "Metadata-Flavor: Google" 2> /dev/null)}

if [ -z $REDIS_HOST ]; then
    REDIS_HOST=localhost
    REDIS_PORT=6379
fi
echo "Using:"
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

