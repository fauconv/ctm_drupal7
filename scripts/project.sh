#!/bin/bash
#+-----------------------------------------------------------+
#|                                                           |
#| CTM Manager                                               |
#|                                                           |
#| Batch to Manage the entire multi-site/farm/factory/project|
#|                                                           |
#+-----------------------------------------------------------+
#| version : VERSION_APP                                     |
#+-----------------------------------------------------------+

#paths and init
SOURCE_PATH='ctm'
SOURCE_SCRIPT='path.sh'
SCRIPT_NAME=$(basename $0)
ABS_SCRIPT_PATH=$(dirname `readlink -e $0`);
if [ "$ABS_SCRIPT_PATH" = "" ]; then
  ABS_SCRIPT_PATH=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)
fi
if [ ! -f "${ABS_SCRIPT_PATH}/${SOURCE_PATH}/${SOURCE_SCRIPT}" ]; then
  echo ""
  echo -e "\e[31m\e[1mCTM is not correctly installed\e[0m"
  echo ""
  exit 1
fi
source ${ABS_SCRIPT_PATH}/${SOURCE_PATH}/${SOURCE_SCRIPT}

php "${ABS_SCRIPT_PATH}/project.php" $@