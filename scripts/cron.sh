#!/bin/bash
#+----------------------------------------------------------------+
#| Batch to launch cron on all site of the Drupal farm            |
#+----------------------------------------------------------------+
#| version : VERSION_SCRIPT                                       |
#+----------------------------------------------------------------+
VERSION_SCRIPT="1.0.0"
ABSOLUTE_PATH=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)/`basename "${BASH_SOURCE[0]}"`
BASEPATH=`dirname ${ABSOLUTE_PATH}`
BASEPATH=`dirname ${BASEPATH}`
HTDOCS="htdocs"
HTDOCS_PATH="${BASEPATH}/${HTDOCS}"
SITE_PATH="${HTDOCS_PATH}/sites"

cd ${SITE_PATH}

for D in `find "." -type d`
do
  NAME=`echo $D | sed 's|\./||g'`
  if [ "${NAME}" != "default" -a "${NAME}" != "." -a "${NAME}" != ".." -a "${NAME}" != "all" ]; then
    if [ -e ${CONFIG_PATH}/settings-${NAME}.php ]; then
      cd ${SITE_PATH}/${NAME};drush cr;
    fi
  fi
done