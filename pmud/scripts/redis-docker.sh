#!/bin/bash

set -e
set -u
set -o pipefail

PMUD_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/.."
REDIS_VOLUME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/../redis-volume"
mkdir -p $REDIS_VOLUME/data
mkdir -p $REDIS_VOLUME/conf 
mv $PMUD_DIR/conf/redis.conf $REDIS_VOLUME/conf
ln -nsf $REDIS_VOLUME/conf/redis.conf $PMUD_DIR/conf

docker container rm pmud-redis 2> /dev/null || true
docker run \
  --name pmud-redis \
  -v $REDIS_VOLUME/conf:/usr/local/etc/redis \
  -v $REDIS_VOLUME/data:/data \
  -p 6379:6379 \
  -d \
  redis:7.0-rc2 \
  redis-server /usr/local/etc/redis/redis.conf --loglevel warning
