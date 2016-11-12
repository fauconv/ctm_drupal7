#!/bin/bash
#+-----------------------------------------+
#| Batch to package Drupal for deployement |
#+-----------------------------------------+
#| version : VERSION_SCRIPT                |
#+-----------------------------------------+
VERSION_SCRIPT="1.0.0"
ABSOLUTE_PATH=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)/`basename "${BASH_SOURCE[0]}"`
BASEPATH=`dirname ${ABSOLUTE_PATH}`
BASEPATH=`dirname ${BASEPATH}`
CONFIG_PATH="${BASEPATH}/config"
SCRIPT_PATH="${BASEPATH}/scripts"
HTDOCS="htdocs"
HTDOCS_PATH="${BASEPATH}/${HTDOCS}"
RELEASE_PATH="${HTDOCS_PATH}/releases"
SITE_PATH="${HTDOCS_PATH}/sites"
MEDIA_PATH="${BASEPATH}/media"
TARGET_PATH="${BASEPATH}/target"
SITE_PREFIX="site_"

# Site info
DEV=false
UPDATE=false
PROD=false
VERSION=""
DISPLAY=false
EXCLUDE_LIST=
SITE_NAME=
DIR_NAME=
SITE_DIR=
MEDIA_DIR=

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
   echo "$(basename $0) -v                                       --> display the current version of the drupal farm"
   echo "$(basename $0) -d -n <packagename.tgz> [-u] <version-number> [-r <site-ref>] --> create a tgz file with the distribution for developpement server"
   echo "$(basename $0) -p -n <packagename.tgz> [-u] <version-number> [-r <site-ref>] --> create a tgz file with the distribution for production server"
   echo ""
   echo ""
   echo "= Options :"
   echo "===================================="
   echo " -u  package distribution for update only (not for a full installation of drupal farm). It is the option to use for a deployment with deploy.sh"
   echo " -r <site-ref> package distribution for a specific site only"
   echo " -n <packagename.tgz> for the package name"
   echo ""
 }

#
# recuperation des parametres
#
getoptions(){
  while true; do
    case "$1" in
      -d)
        DEV=true
        ;;
      -v)
        DISPLAY=true
        ;;
      -p)
        PROD=true
        ;;
      -u)
        UPDATE=true
        ;;
      -n)
        shift;
        NAME=$1
        ;;  
      -r)
        shift;
        SITE_NAME=$1
        DIR_NAME=${SITE_PREFIX}$1
        SITE_DIR=${SITE_PATH}/${DIR_NAME}
        MEDIA_DIR=${MEDIA_PATH}/${DIR_NAME}
        ;;
      "")
        break;
        ;;
      *)
        VERSION=$1
        ;;
    esac
    shift;
  done
}

#
# load release file
#
loaddata() {
  if [ -e ${RELEASE_PATH}/release.properties ]; then
    source ${RELEASE_PATH}/release.properties
  else
    echo "release file not found"
    exit 1
  fi
}

#
#display task
#
display() {
  if [ $DISPLAY = true ]; then
    echo "Distribution: ${dist}"
    echo "Application : ${app}"
    echo "Build       : ${build}"
    echo ""
    exit 0
  fi
}

#
#fill the release file
#
setrelease() {
  chmod -R 777 ${RELEASE_PATH}
  echo "dist=${dist}" > ${RELEASE_PATH}/release.properties
  echo "app=${VERSION}" >> ${RELEASE_PATH}/release.properties
  BUILD=`date "+%Y%m%d"`
  echo "build=${BUILD}" >> ${RELEASE_PATH}/release.properties
}

#
# exclude unwanted site
#
excludelist() {
  if [ ! -z $DIR_NAME ]; then
    cd ${SITE_PATH}
    for D in `find "." -type d`
    do
      NAME=`echo $D | sed 's|\./||g'`
      if [ "${NAME}" != "default" -a "${NAME}" != "." -a "${NAME}" != ".." -a "${NAME}" != "all" ]; then
        if [ ! ${DIR_NAME} = ${NAME} ]; then
          EXCLUDE_LIST="${EXCLUDE_LIST} --exclude=${NAME}"
        fi
      fi
    done
  fi
}

#
#zip file
#
zipdata() {
  mkdir ${TARGET_PATH}
  cd $BASEPATH
  chmod 777 . ${HTDOCS}
  mv ${HTDOCS} ${HTDOCS}_${VERSION}_${BUILD}
  if [ $UPDATE = true ]; then
    echo "DIR=\"${HTDOCS}_${VERSION}_${BUILD}\"" > directory.properties    rm directory.properties
  else
    ln -s ${HTDOCS}_${VERSION}_${BUILD} ${HTDOCS}
    tar -czf ${TARGET_PATH}/${NAME} config/example* media scripts docs patches ${HTDOCS} ${HTDOCS}_${VERSION}_${BUILD} --exclude=scripts/site_blancpain --exclude=scripts/site_${SITE_NAME}/bower_components --exclude=scripts/site_${SITE_NAME}/node_modules --exclude=${HTDOCS}_${VERSION}_${BUILD}/media/* --exclude=${HTDOCS}_${VERSION}_${BUILD}/sites/sites.php
    rm ${HTDOCS}
  fi
  mv ${HTDOCS}_${VERSION}_${BUILD} ${HTDOCS}
}

#
# check param
#
checkparam() {
if [ -z $NAME ]; then
    printHelp
    exit 1 
fi

if [ $DEV = $PROD ]; then
    printHelp
    exit 1 
fi

  #if [ $VERSION = ""  ]; then
  #  printHelp
  #  exit 1
  #fi
}

#
# recreate symlink
#
setsymlink() {
  if [ ! -e "${SITE_PATH}/sites.php" ]; then
    chmod 777 ${SITE_PATH}
    cd ${SITE_PATH};ln -s ../../config/sites.php sites.php
    chmod 550 ${SITE_PATH}
  fi
  if [ ! -e "${HTDOCS_PATH}/media" ]; then
    chmod 777 ${HTDOCS_PATH}
    cd ${HTDOCS_PATH};ln -s ../media media
    chmod 550 ${HTDOCS_PATH}
  fi
  chmod 770 ${SCRIPT_PATH}/*
}

#
# main
#
getoptions $@
loaddata
display
checkparam
setrelease
setsymlink
excludelist
zipdata
echo "Package created "${TARGET_PATH}/${NAME}
exit 0
