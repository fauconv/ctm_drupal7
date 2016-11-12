#!/bin/bash
#+---------------------------------------------------+
#| Batch to install/deploy/update Drupal             |
#+---------------------------------------------------+
#| version : VERSION_SCRIPT                          |
#+---------------------------------------------------+
VERSION_SCRIPT="1.0.0"
ABSOLUTE_PATH=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)/`basename "${BASH_SOURCE[0]}"`
BASEPATH=`dirname ${ABSOLUTE_PATH}`
BASEPATH=`dirname ${BASEPATH}`

CONFIG_PATH="${BASEPATH}/config"
SCRIPT_PATH="${BASEPATH}/scripts"
HTDOCS="htdocs"
HTDOCS_PATH="${BASEPATH}/${HTDOCS}"
SITE_PATH="${HTDOCS_PATH}/sites"
MEDIA_PATH="${BASEPATH}/media"
DUMP_DIR="${BASEPATH}/dump"
SITE_PREFIX="site_"
CHMOD_MEDIA="770"
CHMOD_CODE="550"
CHMOD_MEDIA_FILE="660"
CHMOD_CODE_FILE="440"

#
#affichage de l aide
#
printHelp(){
   echo ""
   echo "$(basename $0)  Version ${VERSION_SCRIPT}"
   echo ""
   echo ""
   echo "= Usage :"
   echo "========="
   echo "$(basename $0) <site-ref>     --> compile front office for site-ref"
   echo ""
}

#
# get parameter
#
getoptions(){
  while true; do
    case "$1" in
      "")
        break
        ;;
      *)
        SITE_NAME=$1
        DIR_NAME=${SITE_PREFIX}$1
        SITE_DIR=${SITE_PATH}/${DIR_NAME}
        MEDIA_DIR=${MEDIA_PATH}/${DIR_NAME}
        ;;
    esac
    shift
  done
}

build() {
  if [ -e ${SCRIPT_PATH}/${DIR_NAME} ]; then
    cd ${SCRIPT_PATH}/${DIR_NAME}
    echo '# NPM INSTALL'
    npm install
    echo '# GULP'
    gulp deploy --site=${SITE_NAME}
  else
    echo -e "Project does not exist :                                                 \e[31m\e[1m[fail]\e[0m"
  fi
}

#
# main
#
getoptions $@
echo ""
if [ ! -z $SITE_NAME ]; then
  build
else
  printHelp
fi
echo ""
exit 0
