#!/bin/bash

set -e
set -u
set -o pipefail

PMUD_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/.."

export REDIS_HOST=${1:-localhost}
export REDIS_PORT=${2:-6379}
export LD_LIBRARY_PATH=$PMUD_DIR/_solib_k8/_U@caf_S_S_Cmain___Ulib:$PMUD_DIR/_solib_k8/_U@hiredis_S_S_Cmain___Ulocal_Slib

echo "Using:"
echo "REDIS_HOST=$REDIS_HOST"
echo "REDIS_PORT=$REDIS_PORT"
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

cd $PMUD_DIR
pwd
util/yaml-redis-loader -p places < conf/world/places.yml
util/yaml-redis-loader -p items < conf/world/items.yml
util/yaml-redis-loader -p monsters < conf/world/monsters.yml