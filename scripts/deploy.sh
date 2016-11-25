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

# Site iparameters
HOSTNAME=
ENVI="prod"
DIR_NAME=
SITE_NAME=
SITE_DIR=
MEDIA_DIR=
CHOWN=
ZIP=
DATABASE=
EXTERNAL=false
REMOVE=false
MOCK=0
UPDATE=false
LIST=false
PHASE="0"
ALT=
DUMP=false

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
   echo "$(basename $0) <options> <site-ref>     --> install or create a site (can by combined with -r and -p)"
   echo "$(basename $0) -r <site-ref>            --> remove installation of a site"
   echo "$(basename $0) -p <site-ref>            --> create a dumpfile of the database corresponding to the site-ref"
   echo "$(basename $0) -u                       --> update all installed site"
   echo "$(basename $0) -z <tgz-file>            --> update a drupal farm with the zip-file provided by package.sh"
   echo "$(basename $0) -l                       --> print list of all site present in this distribution"
   echo ""
   echo ""
   echo "= Installation and creation options :"
   echo "====================================="
   echo " -h <'url','url2','url3'>                                 : list of all urls managed by this website between simple quote separated by commat"
   echo " -d <mysql://db_user:db_password@db_host:db_port/db_name> : database url"
   echo " -m (optional)                                            : activate the auto-login system DONT USE IT ON PRODUCTION SERVER"
   echo " -o <user>:<group> (optional)                             : apply 'chown -R <user>:<group> *' on all directory created or modified by this script"
   echo " -e <environnement> (optional)                            : can by 'dev', 'rec', 'int', 'qual', 'prod'. by default : '${ENVI}'"
   echo " -a <id> (optional)                                       : specify an alternative config file in config directory like : site_<site-ref><id>.conf"
   echo " -s [1|2|3] (optional)                                    : step by step. You can execute only the desired phase of deployment. FOR DEVELOPMENT OF THIS SCRIPT ONLY"
   echo "     phase 1 : creation of directories and settings file"
   echo "     phase 2 : drupal installation (drush)"
   echo "     phase 3 : cutting of settings.php file"
   echo ""
   echo ""
   echo "= Creation only options :"
   echo "========================="
   echo " -x                                                       : mean 'internet site', else 'intranet site'"
   echo ""
   echo ""
   echo "= Using ctm.conf :"
   echo "=================="
   echo "You can use a configuration file with name \"${SITE_PREFIX}<site-ref>.conf\" placed in the directory 'config'. File synthax is :"
   echo ""
   echo " HOSTNAME=\"'url','url2','url3'\""
   echo " MOCK=1"
   echo " DATABASE=\"mysql://db_user:db_password@db_host:db_port/db_name\""
   echo " CHOWN=\"user:group\""
   echo " ENVI=\"prod\""
   echo ""
   echo ""
   echo "= Exemples :"
   echo "============"
   echo "Installing 1 site in production (without configuration file): "
   echo "./$(basename $0) -h \"'http://mido.bi-dev.com','https://mido.bi-dev.com'\" -d \"mysql://mido_website:password@localhost:3306/mido_website\" mido"
   echo ""
   echo "Installing 1 site in developpement (without configuration file) : "
   echo "./$(basename $0) -m -e dev -h \"'http://mido.bi-dev.com','https://mido.bi-dev.com'\" -d \"mysql://mido_website:password@localhost:3306/mido_website\" mido"
   echo ""
   echo "Updating all sites : "
   echo "./$(basename $0) -u"
   echo ""
   echo "list all sites : "
   echo "./$(basename $0) -l"
   echo ""
   echo "remove installed site : "
   echo "./$(basename $0) -r mido"
   echo ""
   echo "Creating a site in developpement (without configuration file) : "
   echo "./$(basename $0) -mx -e dev -h \"'http://mido.bi-dev.com','https://mido.bi-dev.com'\" -d \"mysql://mido_website:password@localhost:3306/mido_website\" mido"
   echo ""
   echo "Installing or creating a site with configuration file : "
   echo "./$(basename $0) mido"
   echo ""
}

#
# get parameter
#
getoptions(){
  while true; do
    case "$1" in
      -a)
        shift;
        ALT="$1"
        ;;
      -d)
        shift;
        DATABASE=$1
        ;;
      -e)
        shift;
        ENVI="$1"
        ;;
      -h)
        shift;
        HOSTNAME="$1"
        ;;
      -l)
        LIST=true
        ;;
      -m)
        MOCK=1
        ;;
      -o)
        shift;
        CHOWN=$1
        ;;
      -p)
        DUMP=true
        ;;
      -r)
        REMOVE=true
        ;;
      -s)
        shift;
        PHASE="$1"
        ;;
      -u)
        UPDATE=true
        ;;
      -x)
        EXTERNAL=true
        ;;
      -z)
        shift;
        ZIP="$1"
        ;;
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

#
# Validation des url
#
validate_url() {
  if [ -z $HOSTNAME ]; then
    if [ ${REMOVE} = false ]; then
      echo -e "hostname list missing                                                 \e[31m\e[1m[fail]\e[0m"
      printHelp
      exit 1
    fi
  fi
}

#
# check site exist
#
validate_exist() {
  if [ -e ${CONFIG_PATH}/settings-${DIR_NAME}.php ]; then
    echo -e "This site is already installed                                        \e[31m\e[1m[fail]\e[0m"
    exit 1
  fi
}

#
# check info db
#
validate_db() {
  if [ -z ${DATABASE} ]; then
    if [ ${REMOVE} = false ]; then
      echo -e "database url missing                                                  \e[31m\e[1m[fail]\e[0m"
      printHelp
      exit 1
    fi
  fi
}

#
# check command line valid
#
validate_cmd() {
  if [ -z $SITE_NAME ]; then
    if [ ${UPDATE} = false ]; then
      if [ ${LIST} = false ]; then
        if [ ${REMOVE} = false ]; then
          if [ $DUMP = false ]; then
            if [ -z $ZIP ]; then
              printHelp
              exit 0
            fi
          fi
        fi
      fi
    fi
    if [ ${REMOVE} = true ]; then
      echo -e "site name missing                                                  \e[31m\e[1m[fail]\e[0m"
      printHelp
      exit 1
    fi
    if [ $DUMP = true ]; then
      echo -e "site name missing                                                  \e[31m\e[1m[fail]\e[0m"
      printHelp
      exit 1
    fi
  fi
}

#
# remove existing file
#
remove_site() {
  if [ $REMOVE = true ]; then
    if [ -e ${CONFIG_PATH}/settings-${DIR_NAME}.php ]; then
      chmod -R 777 ${CONFIG_PATH}
      rm ${CONFIG_PATH}/settings-${DIR_NAME}.php ${CONFIG_PATH}/mock-${DIR_NAME}.xml ${CONFIG_PATH}/masquerade-${DIR_NAME}.xml
      echo -e "Removing ${SITE_NAME}...                                \e[32m\e[1m[ok]\e[0m"
    fi
  fi
}

#
#read conf files
#
read_conf() {
  if [ -e ${CONFIG_PATH}/${DIR_NAME}.conf ]; then
    source ${CONFIG_PATH}/${DIR_NAME}.conf
  fi
  if [ -e ${SITE_DIR}/${DIR_NAME}.conf.php ]; then
    source ${SITE_DIR}/${DIR_NAME}.conf.php
  fi
}

#
# check if link exist for windows and create symlink
#
validate_os() {
  IS_WINDOW=false
  if [ ! -z $OS ]; then
    WIN=`echo ${OS} | grep -i Windows`
    if [ ! -z $WIN ]; then
      echo "You are on Windows"
      IS_WINDOW=true
      if [ ! -e "${SITE_PATH}/sites.php" ]; then
        echo -e "You must create symlink before launch this command :                                                 \e[31m\e[1m[fail]\e[0m"
        echo ""
        echo "in directory : ${SITE_PATH} :"
        echo "     use : mklink sites.php ..\..\config\sites.php"
        echo "in directory : ${HTDOCS_PATH} :"
        echo "     use : mklink /D media ..\media"
        echo ""
        exit 1
      else
        if [ ! -e "${HTDOCS_PATH}/media" ]; then
          echo -e "You must create symlink before launch this command :                                                 \e[31m\e[1m[fail]\e[0m"
          echo "in directory : ${HTDOCS_PATH} :"
          echo "   use : mklink /D media ..\media"
          echo ""
          exit 1
        fi
      fi
    fi
  fi
  if [ ! -e "${SITE_PATH}/sites.php" ]; then
    chmod 777 ${SITE_PATH}
    cd ${SITE_PATH};ln -s ../../config/sites.php sites.php
    chmod ${CHMOD_CODE} ${SITE_PATH}
  fi
  if [ ! -e "${HTDOCS_PATH}/media" ]; then
    chmod 777 ${HTDOCS_PATH}
    cd ${HTDOCS_PATH};ln -s ../media media
    chmod ${CHMOD_CODE} ${HTDOCS_PATH}
  fi
}

#
# adapt chmod right to environment
check_right() {
  if [ $ENVI = "dev" ]; then
    CHMOD_MEDIA="777"
    CHMOD_CODE="777"
    CHMOD_MEDIA_FILE="666"
    CHMOD_CODE_FILE="666"
  fi
}

#
# phase 0 (all phase)
#
phase_0() {
  phase_1
  phase_2
  phase_3
}

#
# phase 1 (init dir and settings)
#
phase_1() {
  if [ ! -d "${SITE_DIR}" ]; then
    echo "Phase 1 : Create site..."
    chmod 777 $SITE_PATH $MEDIA_PATH
    cp -r ${SITE_PATH}/default $SITE_DIR
    cp -R  ${MEDIA_PATH}/default $MEDIA_DIR
    chmod -R 777 $SITE_DIR $MEDIA_DIR
    rm ${SITE_DIR}/header.settings.php
    chmod $CHMOD_CODE $SITE_PATH $MEDIA_PATH
    echo -e "Phase 1 : Create site...                                                        \e[32m\e[1m[ok]\e[0m"
  else
    echo "Phase 1 : Reinitializing installation of site..."
    chmod -R 777 $SITE_DIR
    chmod 777 $MEDIA_PATH
    if [ ! -d $MEDIA_DIR ]; then
      cp -R  ${MEDIA_PATH}/default $MEDIA_DIR
    fi
    chmod -R 777 $MEDIA_DIR
    grep -B 10000 "====== CUT HERE ======" ${SITE_PATH}/default/settings.php | grep -v "====== CUT HERE ======" > ${SITE_DIR}/settings2.php
    chmod 777 ${SITE_DIR}/settings2.php
    grep -A 10000 "====== CUT HERE ======" ${SITE_DIR}/settings.php >> ${SITE_DIR}/settings2.php
    mv ${SITE_DIR}/settings.php ${SITE_DIR}/settings.php.orig
    mv ${SITE_DIR}/settings2.php ${SITE_DIR}/settings.php
    echo -e "Phase 1 : Reinitializing installalation of site...                                \e[32m\e[1m[ok]\e[0m"
  fi
  echo "Phase 1 : Configuring site..."
  chmod -R 777 $MEDIA_DIR $SITE_DIR ${SITE_PATH}/default
  PATTERN="base_root_list'] = array.*\$"
  RESULT="base_root_list'] = array\(${HOSTNAME}\);"
  sed "s|${PATTERN}|${RESULT}|1" ${SITE_DIR}/settings.php > ${SITE_DIR}/settings2.php
  rm ${SITE_DIR}/settings.php
  PATTERN="'environnement'] = '.*\$"
  RESULT="'environnement'] = '${ENVI}';"
  sed "s|${PATTERN}|${RESULT}|1" ${SITE_DIR}/settings2.php > ${SITE_DIR}/settings.php
  rm ${SITE_DIR}/settings2.php
  URL=`echo $HOSTNAME | sed 's|,| |g' | sed 's|http[s]*://||g'`
  chmod 777 ${CONFIG_PATH}/sites.php
  for f in ${URL}
  do
    echo -n "\$sites[" >> ${CONFIG_PATH}/sites.php
    echo -n ${f}  >> ${CONFIG_PATH}/sites.php
    echo -n "] = '" >> ${CONFIG_PATH}/sites.php
    echo -n ${DIR_NAME} >> ${CONFIG_PATH}/sites.php
    echo "';" >> ${CONFIG_PATH}/sites.php
  done
  chmod $CHMOD_CODE_FILE ${CONFIG_PATH}/sites.php
  cp ${SITE_DIR}/settings.php ${SITE_PATH}/default/default.settings.php
  chmod 777 ${SITE_PATH}/default/default.settings.php
  echo -e "Phase 1 : Configuring site...                                                   \e[32m\e[1m[ok]\e[0m"
}

#
# phase 2
# drupal install + create conf file with profil
#
phase_2() {
  echo "Phase 2 : Installing site..."
  if [ $EXTERNAL = true ]; then
    PROFIL="ctm_internet"
  else
    PROFIL="ctm_intranet"
  fi
  URL0=`echo $HOSTNAME | sed 's|,.*||g' | sed 's|http[s]*://||g' | sed "s|'||g" | sed "s|/.*||g"`
  cd ${SITE_DIR}
  drush site-install $PROFIL -y --account-name="developer" --account-mail="webmaster@${URL0}" --site-mail="no-reply@${URL0}" --site-name="${SITE_NAME}" --sites-subdir="${DIR_NAME}" --db-url="${DATABASE}" ctm_commun_form.authentication_use="${MOCK}" install_configure_form.update_status_module='array(FALSE,FALSE)' install_configure_form.site_default_country='CH' install_configure_form.date_default_timezone='Europe/Zurich'
  drush ctm_queue
  if [ $ENVI = 'dev' ]; then
    drush ctm_tools devel
    DUMP=true
    dump
  fi

  chmod -R 777 ${SITE_DIR}
  echo "EXTERNAL=${EXTERNAL}" > ${SITE_DIR}/${DIR_NAME}.conf.php
  cd ${SCRIPT_PATH}
  echo -e "Phase 2 : Installing site...                                                    \e[32m\e[1m[ok]\e[0m"
}

#
# phase 3
# cutting of setting.php file
#
phase_3() {
  echo "Phase 3 : Cutting settings..."
  chmod -R 777 ${SITE_PATH}/default ${SITE_DIR}
  chmod 777 ${CONFIG_PATH}
  rm ${SITE_PATH}/default/default.settings.php
  chmod -R $CHMOD_CODE ${SITE_PATH}/default
  grep -B 10000 "====== CUT HERE ======" ${SITE_DIR}/settings.php | grep -v "====== CUT HERE ======" > ${CONFIG_PATH}/settings-${DIR_NAME}.php
  cp ${SITE_PATH}/default/header.settings.php ${SITE_DIR}/settings2.php
  chmod 777 ${SITE_DIR}/settings2.php
  grep -A 10000 "====== CUT HERE ======" ${SITE_DIR}/settings.php >> ${SITE_DIR}/settings2.php
  rm -f ${SITE_DIR}/settings.php
  mv ${SITE_DIR}/settings2.php ${SITE_DIR}/settings.php
  if [ -e ${SITE_DIR}/settings.php.orig ]; then
    rm -f ${SITE_DIR}/settings.php
    mv ${SITE_DIR}/settings.php.orig ${SITE_DIR}/settings.php
  fi
  cp ${CONFIG_PATH}/example.mock-default.xml ${CONFIG_PATH}/mock-${DIR_NAME}.xml
  cp ${CONFIG_PATH}/example.masquerade-default.xml ${CONFIG_PATH}/masquerade-${DIR_NAME}.xml
  # compilation is incomming so stay in 777
  chmod 777 -R ${SITE_DIR}
  chmod $CHMOD_CODE_FILE ${SITE_DIR}/settings.php
  chmod -R $CHMOD_CODE ${CONFIG_PATH}
  if [ "${CHOWN}" != "" ]; then
    chown -R ${CHOWN} ${CONFIG_PATH} ${SITE_DIR} ${MEDIA_DIR}
  fi
  echo -e "Phase 3 : Cutting settings...                                                   \e[32m\e[1m[ok]\e[0m"
}

#
# unzip task
#
zip() {
  if [ ! -z $ZIP ]; then
    cd $BASEPATH
    tar -xzf $ZIP
    if [ -e directory.properties ]; then
      source directory.properties
      rm ${HTDOCS}
      ln -s $DIR ${HTDOCS}
      UPDATE=true
      rm directory.properties
      rm directory.properties
    else
      echo "${ZIP} is not a valid ctm package"
      exit 1
    fi
  fi
}

#
# update task
#
update() {
  if [ $UPDATE = true ]; then
    cd ${SITE_PATH}
    file=`date +%y%m%d_%H%M%S`
    for D in `find "." -type d`
    do
      NAME=`echo $D | sed 's|\./||g'`
      if [ "${NAME}" != "default" -a "${NAME}" != "." -a "${NAME}" != ".." -a "${NAME}" != "all" ]; then
        if [ -e ${CONFIG_PATH}/settings-${NAME}.php ]; then
          echo "updating ${NAME} ...";
          cd ${SITE_PATH}/${NAME}
          drush sql-dump > ${DUMP_DIR}/${NAME}_${file}.sql
          drush ctm_update -y;
          echo -e "updating ${NAME} ...                                \e[32m\e[1m[ok]\e[0m";
        fi
      fi
    done
  fi
}

#
#list task
#
list() {
  if [ $LIST = true ]; then
    echo "= Not installed sites :"
    echo "======================="
    cd ${SITE_PATH}
    for D in `find . -maxdepth 1 -type d`
    do
      NAME=`echo $D | sed 's|\./||g'`
      if [ "${NAME}" != "default" -a "${NAME}" != "." -a "${NAME}" != ".." -a "${NAME}" != "all" ]; then
        if [ ! -e "${CONFIG_PATH}/settings-${NAME}.php" ]; then
          echo -n " - ${NAME}" | sed 's|site_||'
          if [ -e ${CONFIG_PATH}/${NAME}.conf ]; then
            echo " (Config file present)"
          else
            echo ""
          fi
        fi
      fi
    done
    echo ""
    echo "= Live sites :"
    echo "=============="
    for D in `find . -maxdepth 1 -type d`
    do
      NAME=`echo $D | sed 's|\./||g'`
      if [ "${NAME}" != "default" -a "${NAME}" != "." -a "${NAME}" != ".." -a "${NAME}" != "all" ]; then
        if [ -e "${CONFIG_PATH}/settings-${NAME}.php" ]; then
          echo " - ${NAME}" | sed 's|site_||'
          if [ -e ${CONFIG_PATH}/${NAME}.conf ]; then
            echo " (Config file present)"
          else
            echo ""
          fi
        fi
      fi
    done
  fi
}

#
#
#
create_sites() {
  if [ ! -e "${CONFIG_PATH}/sites.php" ]; then
    chmod 777 ${CONFIG_PATH}
    echo "<?php" > ${CONFIG_PATH}/sites.php
    chmod ${CHMOD_CODE} ${CONFIG_PATH}
  fi
}

#
#
#
alt_conf() {
  if [ ! -z ${ALT} ]; then
    DIR_NAME_ORIG=${DIR_NAME}
    DIR_NAME=${DIR_NAME}${ALT}
    SITE_DIR_ORIG=${SITE_DIR}
    SITE_DIR=${SITE_DIR}${ALT}
    MEDIA_DIR_ORIG=${MEDIA_DIR}
    MEDIA_DIR=${MEDIA_DIR}${ALT}
    echo "creating alteration from  ${DIR_NAME_ORIG} to ${DIR_NAME} ...";
    if [ ! -e ${SITE_DIR_ORIG} ]; then
      echo -e "You can not create alteration of an non existing site :                                                 \e[31m\e[1m[fail]\e[0m"
      exit 1
    fi
    if [ -e ${SITE_DIR}/modules ]; then
      echo -e "alteration exist...                                \e[32m\e[1m[ok]\e[0m";
      return
    fi
    if [ ! -e ${SITE_DIR} ]; then
      chmod 777 ${SITE_PATH}
      mkdir ${SITE_DIR}
      cp ${SITE_DIR_ORIG}/settings.php ${SITE_DIR}
      cp ${SITE_DIR_ORIG}/${DIR_NAME_ORIG}.conf.php ${SITE_DIR}/${DIR_NAME}.conf.php
    fi
    if [ ${IS_WINDOW} = true ]; then
      echo "You must create symlink manualy  in ${SITE_DIR} :"
      echo "   mklink /D libraries ..\\${DIR_NAME_ORIG}\\libraries"
      echo "   mklink /D modules ..\\${DIR_NAME_ORIG}\\modules"
      echo "   mklink /D media_export ..\\${DIR_NAME_ORIG}\\media_export"
      echo "   mklink /D themes ..\\${DIR_NAME_ORIG}\\themes"
      echo "   mklink /D translations ..\\${DIR_NAME_ORIG}\\translations"
      exit 1
    fi
    cd ${SITE_DIR}
    ln -s  ../${DIR_NAME_ORIG}/libraries
    ln -s  ../${DIR_NAME_ORIG}/modules
    ln -s  ../${DIR_NAME_ORIG}/media_export
    ln -s  ../${DIR_NAME_ORIG}/themes
    ln -s  ../${DIR_NAME_ORIG}/translations
    echo -e "alteration created ...                                \e[32m\e[1m[ok]\e[0m";
  fi
}

#
#
#
dump() {
  if [ ${DUMP} = true ]; then
    echo -e "dumping database...";
    if [ ! -e $DUMP_DIR ]; then
      chmod +w $BASEPATH
      mkdir $DUMP_DIR
    fi
    cd ${SITE_DIR}
    file=`date +%y%m%d_%H%M%S`
    drush sql-dump > ${DUMP_DIR}/${SITE_NAME}_${file}.sql
    echo -e "dumping database...                                \e[32m\e[1m[ok]\e[0m";
  fi
}

#
#
#
displaytime() {
  runtime=$((end-start))
  res=`date --date="@${runtime}" +%M\'%S`
  echo ""
  echo -e "script executed in \e[32m\e[1m${res}\e[0m"
}

#
# main
#
start=`date +%s`
getoptions $@
echo ""
validate_cmd
if [ ! -z $SITE_NAME ]; then
  create_sites
  validate_os
  alt_conf
  read_conf
  check_right
  dump
  remove_site
  validate_url
  validate_db
  validate_exist
  phase_${PHASE}
fi
zip
update
list
end=`date +%s`
displaytime
echo ""
exit 0
