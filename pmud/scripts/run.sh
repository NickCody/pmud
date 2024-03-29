#!/bin/bash

set -e
set -u
set -o pipefail

PMUD_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/.."

export REDIS_HOST=${1:-localhost}
export REDIS_PORT=${2:-6379}
LD_LIBRARY_PATH=$PMUD_DIR/_solib_k8/_U@caf_S_S_Cmain___Ulib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PMUD_DIR/_solib_k8/_U@hiredis_S_S_Cmain___Ulocal_Slib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PMUD_DIR/_solib_k8/
export LD_LIBRARY_PATH

echo "Using:"
echo "PMUD_DIR=$PMUD_DIR"
echo "REDIS_HOST=$REDIS_HOST"
echo "REDIS_PORT=$REDIS_PORT"
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

echo "Killing existing instance..."
pkill pmud || true

cd $PMUD_DIR

# ./pmud --config-file=conf/pmud.caf conf/primordia-mud.yml
nohup ./pmud --config-file=conf/pmud.caf conf/primordia-mud.yml > pmud.out 2> pmud.err < /dev/null &

echo "Started pmud"

# while true; do 
#   ./pmud --config-file=conf/pmud.caf conf/primordia-mud.yml
#   if [ $? -eq 98 ]; then
#     echo "Waiting for port..."
#     sleep 5
#   else
#     break;
#   fi
# done

