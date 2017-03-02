#!/bin/bash
#+-----------------------------------------------------------+
#|                                                           |
#| CTM Manager                                               |
#|                                                           |
#| Batch to Manage the entire multi-site/farm/factory/project|
#|                                                           |
#+-----------------------------------------------------------+
#| version : VERSION_SCRIPT                                  |
#+-----------------------------------------------------------+

#
# const
#
SOURCE_PATH='ctm'
SOURCE_SCRIPT='ctm_path'
SOURCE_URL='ctm_url'


# Site parameters
HOSTNAME=
ENVI="prod"
DIR_NAME=
SITE_NAME=
ABS_SITE_PATH=
ABS_MEDIA_PATH=
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

#CTM paths
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
cd ${ABS_DCF_PATH}

#
# display help
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
   echo "= Using configuration file :"
   echo "============================"
   echo "You can use a configuration file with name \"${ABS_CONFIG_PATH}/${SITE_PREFIX}<site-ref>${CONF_EXT}\". File synthax is :"
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
        SITE_PATH=${SITES_PATH}/${DIR_NAME}
        MEDIA_PATH=${MEDIAS_PATH}/${DIR_NAME}
        ABS_SITE_PATH=${ABS_SITES_PATH}/${DIR_NAME}
        ABS_MEDIA_PATH=${ABS_MEDIAS_PATH}/${DIR_NAME}
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
  source ${ABS_SCRIPT_PATH}/${SOURCE_PATH}/${SOURCE_URL}
}

#
# check site exist
#
validate_exist() {
  if [ -e ${ABS_CONFIG_PATH}/settings-${DIR_NAME}.php ]; then
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
# create drush alias for the site
#
function create_drush_alias {
  cd $ABS_DRUSH_ALIAS
  echo "<?php" > ${SITE_NAME}.alias.drushrc.php
  echo "\$options['uri'] = '${URL0}';" >> ${SITE_NAME}.alias.drushrc.php
  echo "\$options['root'] = '${ABS_DOCUMENT_ROOT}';" >> ${SITE_NAME}.alias.drushrc.php
}

#
# remove existing file
#
remove_site() {
  if [ $REMOVE = true ]; then
    if [ -e ${ABS_CONFIG_PATH}/settings-${DIR_NAME}.php ]; then
      chmod -R 777 ${ABS_CONFIG_PATH}
      rm ${ABS_CONFIG_PATH}/settings-${DIR_NAME}.php ${ABS_CONFIG_PATH}/mock-${DIR_NAME}.xml ${ABS_CONFIG_PATH}/masquerade-${DIR_NAME}.xml
      echo -e "Removing ${SITE_NAME}...                                \e[32m\e[1m[ok]\e[0m"
    fi
  fi
}

#
#read conf files
#
read_conf() {
  if [ -e ${ABS_CONFIG_PATH}/${DIR_NAME}${CONF_EXT} ]; then
    source ${ABS_CONFIG_PATH}/${DIR_NAME}${CONF_EXT}
  fi
  if [ -e ${ABS_SITE_DIR}/${DIR_NAME}.conf.php ]; then
    source ${ABS_SITE_DIR}/${DIR_NAME}.conf.php
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
    fi
  fi
}

#
# adapt chmod right to environment
#
check_right() {
  if [ $ENVI = "dev" ]; then
    CHMOD_MEDIA="777"
    CHMOD_CODE="777"
    CHMOD_MEDIA_FILE="666"
    CHMOD_CODE_FILE="666"
  fi
}

#
# create htaccess for alias
#
function create_htaccess {
  cd ${ABS_DOCUMENT_ROOT}
  for f in ${URL_ALIAS}
  do
    f=${f}/
    ALIAS=`echo ${f} | sed "s|'||ig" | sed 's|[^/]*/||i' | sed 's|/$||g'`
    if [ ! "${ALIAS}" = "" ]; then
      FOUND=`grep "/${ALIAS}/index.php" .htaccess`
      if [ "$FOUND" = "" ]; then
        TEXT="DCF_MANAGER_TAG\nRewriteCond %{REQUEST_URI} ^/${ALIAS}/\nRewriteRule ^ /${ALIAS}/index.php [L]\n"
        sed "s|DCF_MANAGER_TAG|${TEXT}|" .htaccess > .htaccess2
        rm .htaccess
        mv .htaccess2 .htaccess
      fi
    fi
  done
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
  if [ ! -d "${ABS_SITE_PATH}" ]; then
    echo "Phase 1 : Create site..."
    chmod 777 $ABS_SITES_PATH $ABS_MEDIAS_PATH
    cp -r ${ABS_SITES_PATH}/default ${ABS_SITE_PATH}
    cp -R  ${ABS_MEDIAS_PATH}/default ${ABS_MEDIA_PATH}
    chmod -R 777 ${ABS_SITE_PATH} ${ABS_MEDIA_PATH}
    rm ${ABS_SITE_PATH}/header.settings.php
    chmod $CHMOD_CODE $ABS_SITE_PATH $ABS_MEDIA_PATH
    echo -e "Phase 1 : Create site...                                                        \e[32m\e[1m[ok]\e[0m"
  else
    echo "Phase 1 : Reinitializing installation of site..."
    chmod -R 777 ${ABS_SITE_PATH}
    chmod 777 ${ABS_MEDIAS_PATH}
    if [ ! -d ${ABS_MEDIA_PATH} ]; then
      cp -R  ${ABS_MEDIAS_PATH}/default ${ABS_MEDIA_PATH}
    fi
    chmod -R 777 ${ABS_MEDIA_PATH}
    grep -B 10000 "====== CUT HERE ======" ${ABS_SITES_PATH}/default/settings.php | grep -v "====== CUT HERE ======" > ${ABS_SITE_PATH}/settings2.php
    chmod 777 ${ABS_SITE_PATH}/settings2.php
    grep -A 10000 "====== CUT HERE ======" ${ABS_SITES_PATH}/settings.php >> ${ABS_SITE_PATH}/settings2.php
    mv ${ABS_SITE_PATH}/settings.php ${ABS_SITE_PATH}/settings.php.orig
    mv ${ABS_SITE_PATH}/settings2.php ${ABS_SITE_PATH}/settings.php
    echo -e "Phase 1 : Reinitializing installalation of site...                                \e[32m\e[1m[ok]\e[0m"
  fi
  echo "Phase 1 : Configuring site..."
  chmod -R 777 ${ABS_MEDIA_PATH} ${ABS_SITE_PATH} ${ABS_SITES_PATH}/default
  PATTERN="base_root_list'] = array.*\$"
  RESULT="base_root_list'] = array\(${HOSTNAME}\);"
  sed "s|${PATTERN}|${RESULT}|1" ${ABS_SITE_PATH}/settings.php > ${ABS_SITE_PATH}/settings2.php
  rm ${ABS_SITE_PATH}/settings.php
  PATTERN="'environnement'] = '.*\$"
  RESULT="'environnement'] = '${ENVI}';"
  sed "s|${PATTERN}|${RESULT}|1" ${ABS_SITE_PATH}/settings2.php > ${ABS_SITE_PATH}/settings.php
  rm ${ABS_SITE_PATH}/settings2.php
  chmod 777 ${ABS_CONFIG_PATH}/sites.php
  for f in ${URL_SETTING}
  do
    echo -n "\$sites[" >> ${ABS_CONFIG_PATH}/sites.php
    echo -n ${f}  >> ${ABS_CONFIG_PATH}/sites.php
    echo -n "] = '" >> ${ABS_CONFIG_PATH}/sites.php
    echo -n ${DIR_NAME} >> ${ABS_CONFIG_PATH}/sites.php
    echo "';" >> ${ABS_CONFIG_PATH}/sites.php
  done
  create_drush_alias
  create_htaccess
  chmod $CHMOD_CODE_FILE ${ABS_CONFIG_PATH}/sites.php
  cp ${ABS_SITE_PATH}/settings.php ${ABS_SITES_PATH}/default/default.settings.php
  chmod 777 ${ABS_SITES_PATH}/default/default.settings.php
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
  cd ${ABS_SITE_PATH}
  ${ABS_SCRIPTS_PATH}/drush site-install $PROFIL -y --account-name="${ADMIN_NAME}" --account-mail="webmaster@${HOST0}" --site-mail="no-reply@${HOST0}" --site-name="${SITE_NAME}" --sites-subdir="${DIR_NAME}" --db-url="${DATABASE}" ctm_commun_form.authentication_use="${MOCK}" install_configure_form.update_status_module='array(FALSE,FALSE)' install_configure_form.site_default_country='CH' install_configure_form.date_default_timezone='Europe/Zurich'
  ${ABS_SCRIPTS_PATH}/drush @${SITE_NAME} ctm_queue
  if [ $ENVI = 'dev' ]; then
    ${ABS_SCRIPTS_PATH}/drush @${SITE_NAME} ctm_tools devel
    DUMP=true
    dump
  fi

  chmod -R 777 ${ABS_SITE_PATH}
  echo "EXTERNAL=${EXTERNAL}" > ${ABS_SITE_PATH}/${DIR_NAME}.conf.php
  cd ${ABS_SCRIPTS_PATH}
  echo -e "Phase 2 : Installing site...                                                    \e[32m\e[1m[ok]\e[0m"
}

#
# phase 3
# cutting of setting.php file
#
phase_3() {
  echo "Phase 3 : Cutting settings..."
  chmod -R 777 ${ABS_SITES_PATH}/default ${ABS_SITE_PATH}
  chmod 777 ${ABS_CONFIG_PATH}
  rm ${ABS_SITES_PATH}/default/default.settings.php
  chmod -R $CHMOD_CODE ${ABS_SITES_PATH}/default
  grep -B 10000 "====== CUT HERE ======" ${ABS_SITE_PATH}/settings.php | grep -v "====== CUT HERE ======" > ${ABS_CONFIG_PATH}/settings-${DIR_NAME}.php
  cp ${ABS_SITES_PATH}/default/header.settings.php ${ABS_SITE_PATH}/settings2.php
  chmod 777 ${ABS_SITE_PATH}/settings2.php
  grep -A 10000 "====== CUT HERE ======" ${ABS_SITE_PATH}/settings.php >> ${ABS_SITE_PATH}/settings2.php
  rm -f ${ABS_SITE_PATH}/settings.php
  mv ${ABS_SITE_PATH}/settings2.php ${ABS_SITE_PATH}/settings.php
  if [ -e ${ABS_SITE_PATH}/settings.php.orig ]; then
    rm -f ${ABS_SITE_PATH}/settings.php
    mv ${ABS_SITE_PATH}/settings.php.orig ${ABS_SITE_PATH}/settings.php
  fi
  cp ${ABS_CONFIG_PATH}/example.mock-default.xml ${ABS_CONFIG_PATH}/mock-${DIR_NAME}.xml
  cp ${ABS_CONFIG_PATH}/example.masquerade-default.xml ${ABS_CONFIG_PATH}/masquerade-${DIR_NAME}.xml
  # compilation is incomming so stay in 777
  chmod 777 -R ${ABS_SITE_PATH}
  chmod $CHMOD_CODE_FILE ${ABS_SITE_PATH}/settings.php
  chmod -R $CHMOD_CODE ${ABS_CONFIG_PATH}
  if [ "${CHOWN}" != "" ]; then
    chown -R ${CHOWN} ${ABS_CONFIG_PATH} ${ABS_SITE_PATH} ${ABS_MEDIA_PATH}
  fi
  echo -e "Phase 3 : Cutting settings...                                                   \e[32m\e[1m[ok]\e[0m"
}

#
#list task
#
list() {
  if [ $LIST = true ]; then
    echo "= Not installed sites :"
    echo "======================="
    cd ${ABS_SITES_PATH}
    for D in `find . -maxdepth 1 -type d`
    do
      NAME=`echo $D | sed 's|\./||g'`
      if [ "${NAME}" != "default" -a "${NAME}" != "." -a "${NAME}" != ".." -a "${NAME}" != "all" ]; then
        if [ ! -e "${ABS_CONFIG_PATH}/settings-${NAME}.php" ]; then
          echo -n " - ${NAME}" | sed 's|site_||'
          if [ -e ${ABS_CONFIG_PATH}/${NAME}.conf ]; then
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
        if [ -e "${ABS_CONFIG_PATH}/settings-${NAME}.php" ]; then
          echo " - ${NAME}" | sed 's|site_||'
          if [ -e ${ABS_CONFIG_PATH}/${NAME}.conf ]; then
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
  if [ ! -e "${ABS_CONFIG_PATH}/sites.php" ]; then
    chmod 777 ${ABS_CONFIG_PATH}
    echo "<?php" > ${ABS_CONFIG_PATH}/sites.php
    chmod ${CHMOD_CODE} ${ABS_CONFIG_PATH}
  fi
}

#
#
#
alt_conf() {
  if [ ! -z ${ALT} ]; then
    DIR_NAME_ORIG=${DIR_NAME}
    DIR_NAME=${DIR_NAME}${ALT}
    ABS_SITE_ORIG=${ABS_SITE_PATH}
    ABS_SITE_PATH=${ABS_SITE_PATH}${ALT}
    ABS_MEDIA_ORIG=${ABS_MEDIA_PATH}
    ABS_MEDIA_PATH=${ABS_MEDIA_PATH}${ALT}
    echo "creating alteration from  ${DIR_NAME_ORIG} to ${DIR_NAME} ...";
    if [ ! -e ${ABS_SITE_ORIG} ]; then
      echo -e "You can not create alteration of an non existing site :                                                 \e[31m\e[1m[fail]\e[0m"
      exit 1
    fi
    if [ -e ${ABS_SITE_PATH}/modules ]; then
      echo -e "alteration exist...                                \e[32m\e[1m[ok]\e[0m";
      return
    fi
    if [ ! -e ${ABS_SITE_PATH} ]; then
      chmod 777 ${ABS_SITES_PATH}
      mkdir ${ABS_SITE_PATH}
      cp ${ABS_SITE_ORIG}/settings.php ${ABS_SITE_PATH}
      cp ${ABS_SITE_ORIG}/${DIR_NAME_ORIG}.conf.php ${ABS_SITE_PATH}/${DIR_NAME}.conf.php
    fi
    if [ ${IS_WINDOW} = true ]; then
      echo "You must create symlink manualy  in ${ABS_SITE_PATH} :"
      echo "   mklink /D libraries ..\\${DIR_NAME_ORIG}\\libraries"
      echo "   mklink /D modules ..\\${DIR_NAME_ORIG}\\modules"
      echo "   mklink /D media_export ..\\${DIR_NAME_ORIG}\\media_export"
      echo "   mklink /D themes ..\\${DIR_NAME_ORIG}\\themes"
      echo "   mklink /D translations ..\\${DIR_NAME_ORIG}\\translations"
      exit 1
    fi
    cd ${ABS_SITE_PATH}
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
      chmod +w $ABS_ROOT_PATH
      mkdir $ABS_DUMP_PATH
    fi
    cd ${ABS_SITE_PATH}
    file=`date +%y%m%d_%H%M%S`
    ${ABS_SCRIPTS_PATH}/drush @${SITE_NAME} sql-dump > ${ABS_DUMP_PATH}/${SITE_NAME}_${file}.sql
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
list
end=`date +%s`
displaytime
echo ""
exit 0
