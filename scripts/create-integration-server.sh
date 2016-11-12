#!/bin/bash


echo '# Checking for available container for update'
RUNNING=$(docker inspect --format="{{ .State.Running }}" $CONTAINER 2> /dev/null)




while [[ $# > 1 ]]
do
key="$1"

case $key in
    -s|--site)
    SITE="$2"
    shift # past argument
    ;;
    -f|--filepath)
    FILE_PATH=$(dirname "${2}")
    FILE_NAME="${2##*/}"
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

echo 'SITE ACTIVATED ' $SITE
if [ -z $SITE ]; then
  echo "SITE PARAM IS NOT SET !!! "
  exit -1
fi

if [ -z $FILE_PATH ]; then
  echo "PACKAGE_PATH PARAM IS NOT SET !!! "
  exit -1
fi


if [ -z $FILE_NAME ]; then
  echo "FILE_NAME PARAM IS NOT SET !!! "
  exit -1
fi


echo "setting parameter"

CONTAINER=cd_apache
BASE_PATH=$(pwd)/..
SCRIPTS_PATH=$BASE_PATH/scripts
DOCKER_PATH=$SCRIPTS_PATH/site_$SITE
PACKAGE_PATH=$FILE_PATH"/package"
echo '*******************'
echo '* SITE         :'$SITE
echo '* PACKAGE_PATH :'$PACKAGE_PATH
echo '* FILE_NAME    :'$FILE_NAME
echo '*******************'


if [ $? -eq 1 ]; then
  RUNNING="false"
fi



# TODO: remove this line when update will be fixed
RUNNING="false"
# If container is not running, recreate it
if [ "$RUNNING" = "false" ]; then
  
  cd $TARGET_PATH

  echo '###### Stack is down, Starting full install'
  echo '# Clean workspace'
  docker-compose -f $DOCKER_PATH/docker-compose.cd.$SITE.yml kill
  docker-compose -f $DOCKER_PATH/docker-compose.cd.$SITE.yml rm -f
 


 
  echo '# Copy TARBALL : '$FILE_PATH' to '$PACKAGE_PATH
  rm -rf $PACKAGE_PATH || exit 1
  mkdir $PACKAGE_PATH
  cp $FILE_PATH'/'$FILE_NAME $PACKAGE_PATH || exit 1

  echo '# Copy integration config'
  cp $SCRIPTS_PATH/site_$SITE/site_$SITE.conf $PACKAGE_PATH || exit 1
  
  echo '# Copy installation script'
  cp $SCRIPTS_PATH/install.sh $PACKAGE_PATH || exit 1

  #echo '# Waiting fiew seconde the stack start'
  #sleep 5s # Moche ==> JE CONFIRME C'EST TRES MOCHE !
  echo '# Build and run the integration stack'
  cp $SCRIPTS_PATH/docker/Dockerfile.build $FILE_PATH
  docker-compose -f $DOCKER_PATH/docker-compose.cd.$SITE.yml build  || exit 1
  docker-compose -f $DOCKER_PATH/docker-compose.cd.$SITE.yml up -d   || exit 1

 
  echo '# Deploy $SITE on the apache container'
  docker exec cd_apache_$SITE bash -c 'whoami' || exit 1
  docker exec cd_apache_$SITE bash -c 'sh /tmp/package/install.sh -s '$SITE' -n '$PACKAGE_NAME || exit 1

  echo '# Push to docker registry'
   

  echo '###### Stack successfully installed'
  exit 0
fi

# else update it
#if [ "$RUNNING" = "true" ]; then
#  echo '###### Stack is running, Starting site update'
#  cd scripts
#  npm install && gulp deploy || exit 1
#  echo "# Packaging update: $BUILD_TAG"
#  TGZ=$(./package.sh -p -u $BUILD_TAG)
#  mv ../$TGZ ../app/update.tgz || exit 1

#  echo '# Deploy update on the apache container'
#  docker exec cd_apache bash -c 'cd /var/www/scripts && ./deploy.sh -z update.tgz' || exit 1
#  docker exec cd_apache bash -c 'chown -R www-data:www-data /var/www' || exit 1
#  echo '###### Stack successfully updated'
#  exit 0
#fi
