#!/bin/bash


while [[ $# > 1 ]]
do
key="$1"

case $key in
    -s|--site)
    CONFIG_FILE=/tmp/package/site_"$2".conf
    shift # past argument
    ;;
    -f|--filename)
    TARBALL=/tmp/package/"$2"
    shift # past argument
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
            # unknown option
    ;;
esac
shift # past argument or value
done

#EXTRACT PARAMETER
WWW_PATH=/var/www
TARBALL=/tmp/package/test.tgz
CONFIG_FILE=/tmp/package/site_mido.conf
WORKING_DIRECTORY=/tmp/workdir
UPDATE_MODE=FULL


#MAIN FILE
echo '########################'
echo '# param WWW_PATH:<'$WWW_PATH'>'
echo '# param TARBALL:<'$TARBALL'>'
echo '# param CONFIG_FILE:<'$CONFIG_FILE'>'
echo '# param WORKING_DIRECTORY:<'$WORKING_DIRECTORY'>'
echo '########################'

if [ -z $WWW_PATH ]; then
  echo "WWW_PATH IS NOT SET !!! "
 	WWW_PATH=/var/www
fi

if [ -z $TARBALL ]; then
  echo "TARBALL IS NOT SET !!! "
  exit -1
fi

if [ -z $CONFIG_FILE ]; then
  echo "CONFIG_FILE IS NOT SET !!! "
  exit -1
fi

if [ -z $WORKING_DIRECTORY ]; then
  echo "WORKING_DIRECTORY IS NOT SET !!! "
  exit -1
fi


if [ $UPDATE_MODE = 'FULL' ]; then
	echo '# FULL COPY'
	echo '# Untar <'$TARBALL'>'
	cd $WWW_PATH || exit 1
	tar -xzf $TARBALL || exit 1
	cp $CONFIG_FILE $WWW_PATH/config/ || exit 1
fi

if [ $UPDATE_MODE = 'INCREMENTAL' ]; then
	echo '# ONCREMENTAL Copy config'
	echo '# create working directory'
	rm -rf $WORKING_DIRECTORY || exit 1
	mkdir $WWW_PATH || exit 1
	cd $WORKING_DIRECTORY || exit 1
	tar -xzf $TARBALL || exit 1
	cp $CONFIG_FILE $WORKING_DIRECTORY/config/ || exit 1
	echo '# Rsync to WWW'
	rsync -av $WORKING_DIRECTORY/* $WWW_PATH
	rm -rf $WORKING_DIRECTORY || exit 1
fi

echo '# Deploy'
cd $WWW_PATH/scripts || exit 1
chmod +x deploy.sh || exit 1
./deploy.sh mido || exit 1
echo '# Change owner www-data'
chown -R www-data:www-data $WWW_PATH || exit 1
echo '# DONE'
