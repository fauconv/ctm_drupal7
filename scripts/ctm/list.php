<?php

function list_proj() {
  echo "\n";
  echo "= Not installed sites :\n";
  echo "=======================\n";
  $dirs = scandir(ABS_SITES_PATH);
  foreach($dirs as $name) {
    if(is_dir(ABS_SITES_PATH.'/'.$name) && $name != "default" && 
        $name != "." && $name != ".." && $name != "all") {
      if (!file_exists(ABS_CONFIG_PATH.'/settings-'.$name.'.php')) {
        echo " - $name";
        if (file_exists(ABS_CONFIG_PATH.'/'.$name.LOCAL_CONF)) {
          echo " (Local config file present)";
        }
        if (file_exists(ABS_CONFIG_PATH.'/'.$name.GLOBAL_CONF)) {
          echo "\n";
        } else {
          echo "\e[31m\e[1m (Global config file absent)\e[0m\n";
        }
      }
    }
  }
  echo "\n";
  echo "= Live sites :\n";
  echo "==============\n";
  foreach($dirs as $name) {
    if(is_dir(ABS_SITES_PATH.'/'.$name) && $name != "default" && 
        $name != "." && $name != ".." && $name != "all") {
      if (file_exists(ABS_CONFIG_PATH.'/settings-'.$name.'.php')) {
        echo " - $name";
      }
    }
  }
  echo "\n";
}
