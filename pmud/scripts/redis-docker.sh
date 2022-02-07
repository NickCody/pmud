#!/bin/bash

set -e
set -u
set -o pipefail

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
REDIS_VOLUME=$(readlink -f "$SCRIPT_DIR/../redis-volume")
mkdir -p $REDIS_VOLUME/data
mkdir -p $REDIS_VOLUME/conf 
cp $SCRIPT_DIR/redis.conf $REDIS_VOLUME/conf

docker container rm pmud-redis 2> /dev/null || true
docker run \
  --name pmud-redis \
  -v $REDIS_VOLUME/conf:/usr/local/etc/redis \
  -v $REDIS_VOLUME/data:/data \
  -p 6379:6379 \
  -d \
  redis \
  redis-server /usr/local/etc/redis/redis.conf --save 10 1 --loglevel warning