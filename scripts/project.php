#!/usr/bin/php
<?php
/**
 *+-----------------------------------------------------------+
 *|                                                           |
 *| CTM Manager                                               |
 *|                                                           |
 *| Batch to Manage the entire multi-site/farm/factory/project|
 *|                                                           |
 *+-----------------------------------------------------------+
 *| version : VERSION_APP                                  |
 *+-----------------------------------------------------------+
**/

/**
 * constants
 */
define('SOURCE_PATH','ctm');
define('SCRIPT_NAME',basename($argv[0]));
define('ABS_SCRIPT_PATH',dirname(__FILE__));
if(!file_exists(ABS_SCRIPT_PATH.'/'.SOURCE_PATH.'/path.php')) {
  echo "\n\e[31m\e[1mCTM is not correctly installed\e[0m\n\n";
  exit(1);
}
include_once ABS_SCRIPT_PATH.'/'.SOURCE_PATH.'/path.php';
include_once ABS_SOURCE_PATH.'/'.'site_deploy.php';
include_once ABS_SOURCE_PATH.'/'.'site_back.php';
include_once ABS_SOURCE_PATH.'/'.'site_remove.php';
include_once ABS_SOURCE_PATH.'/'.'list.php';
include_once ABS_SOURCE_PATH.'/'.'site_dump.php';

/**
 * show help
 */
function showHelp() {
  echo "\n";
  echo " CTM Manager  Version ".VERSION_APP;
  echo "\n\n";
  echo "= Usage :\n";
  echo "=========\n";
  echo "  commands about the entire farm:\n";
  echo "  -------------------------------\n";
  echo "\n";
  echo "  ".SCRIPT_NAME." package <version> : create a package for deployment in production of a project \n";
  echo "  ".SCRIPT_NAME." unpack <file>     : deploy package in production\n";
  echo "  ".SCRIPT_NAME." list              : list all web-site (site-id) in this project\n";
  echo "  ".SCRIPT_NAME." update            : update and rebuild all web-site in production or dev \n";
  echo "  ".SCRIPT_NAME." set (dev|prod)    : set files rights protection. dev => all files writable\n";
  echo "\n";
  echo "  commands about a site of the farm:\n";
  echo "  ----------------------------------\n";
  echo "\n";
  echo "  ".SCRIPT_NAME." site deploy <site_id> -s <step> : create or install (if already exist) a web-site in the \n";
  echo "                                                  project for development (drupal install)\n";
  echo "                                                  => you must set <ID>".LOCAL_CONF." and <ID>".GLOBAL_CONF." before.\n";
  echo "  ".SCRIPT_NAME." site remove <site-id> [-f]      : remove an web-site (installed or not). With -f remove also settings.php in the site directory\n";
  echo "  ".SCRIPT_NAME." site update <site-id>           : update and rebuild a web-site in production or dev\n";
  echo "  ".SCRIPT_NAME." site back <site-id> [file]      : site configuration and data go back before the last snapshot.\n";
  echo "                                                  if a file is specified, the script use that file as snapshot instead of the last snapshot\n";
  echo "                                                  The file must be in dump directory\n";
  echo "  ".SCRIPT_NAME." site snapshot <site-id>         : make a snapshot (backup) of the site to go back to this point later\n";
  echo "\n";
  echo " = More help :\n";
  echo " =============\n";
  echo "  Read install.md in \"docs\" directory for more information\n\n\n";
  exit (1);
}

/**
 * check if site already exist
 */
function validate_exist() {
  if (file_exists(ABS_CONFIG_PATH.'/settings-'.DIR_NAME.'.php')) {
    echo "\nThis site is already installed                                        \e[31m\e[1m[fail]\e[0m\n\n";
    exit (1);
  }
}

/**
 * set access rights
 * @param string $env
 */
function setRight($env) {
  if ($env == 'prod') {
    $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator(ABS_ROOT_PATH), RecursiveIteratorIterator::SELF_FIRST); 
    foreach($iterator as $item) { 
        chmod($item, 0550); 
    }
    $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator(ABS_MEDIAS_PATH), RecursiveIteratorIterator::SELF_FIRST); 
    foreach($iterator as $item) { 
        chmod($item, 0770); 
    } 
  } else {
    $iterator = new RecursiveIteratorIterator(new RecursiveDirectoryIterator(ABS_ROOT_PATH), RecursiveIteratorIterator::SELF_FIRST); 
    foreach($iterator as $item) { 
        chmod($item, 0770); 
    }
  }
}

/**
 * 
 * @param string $dir
 */
function rrmdir($dir) {
  if (is_dir($dir)) {
    $files = scandir($dir);
    foreach ($files as $file)
    if ($file != "." && $file != "..") rrmdir("$dir/$file");
    rmdir($dir);
  }
  else if (file_exists($dir)) unlink($dir);
} 

/**
 * 
 * @param string $src
 * @param string $dst
 */
function rcopy($src, $dst) {
  if (file_exists($dst)) rrmdir($dst);
  if (is_dir($src)) {
    mkdir($dst);
    $files = scandir($src);
    foreach ($files as $file)
    if ($file != "." && $file != "..") rcopy("$src/$file", "$dst/$file"); 
  }
  else if (file_exists($src)) copy($src, $dst);
}

/**
 * main
 */
if ($argc == 1) {
  showHelp();
}

switch ($argv[1]) {
  case 'set':
    if(empty($argv[2])) {
      echo "\n\e[31m\e[1mParameter missing !\e[0m\n";
      showHelp();
    }
    setRight($argv[2]);
    break;
  case 'list':
    list_proj();
    break;
  case 'site':
    if (empty($argv[3])) {
      echo "\n\e[31m\e[1mSite id missing !\e[0m\n";
      showHelp();
    }
    define('ID', preg_replace('/[^a-z]+/','',$argv[3]));
    if(ID == "") {
      echo "\n\e[31m\e[1mSite id can only contain lowercase !\e[0m\n\n";
      exit (1);
    }
    define('SITE_PATH',SITES_PATH.'/'.ID);
    define('ABS_SITE_PATH',ABS_SITES_PATH.'/'.ID);
    define('MEDIA_PATH',MEDIAS_PATH.'/'.ID);
    define('ABS_MEDIA_PATH',ABS_MEDIAS_PATH.'/'.ID);
    switch($argv[2]) {
      case 'deploy':
        if(empty($argv[4])) $argv[4] ='';
        if(empty($argv[5])) $argv[5] ='';
        site_deploy($argv[4], $argv[5]);
        break;
      case'dump':
        dump();
        break;
      case 'remove':
        if(empty($argv[4])) $argv[4] ='';
        site_remove($argv[4]);
        break;
      case 'back':
        if(empty($argv[4])) $argv[4] ='';
        site_back($argv[4]);
        break;
      default:
        echo "\n\e[31m\e[1mUnknown command !\e[0m\n";
        showHelp();
    }
    break;
  default:
    echo "\n\e[31m\e[1mUnknown command : $1 !\e[0m\n";
    showHelp();
}

echo "\n";
exit(0);
