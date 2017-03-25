<?php

/**
 * @param string $file
 */
function site_back($file) {
  echo "\ngo back to previous snapshot...\n";
  
  if (empty($file)) {
    if(!is_dir(ABS_DUMP_PATH)) {
      echo "\n\e[31m\e[1mNo dump file !\e[0m\n\n";
      exit(1);
    }
    $file="";
    $dirs = scandir(ABS_DUMP_PATH);
    foreach($dirs as $name) {
      if(is_file(ABS_DUMP_PATH.'/'.$name)) {
        $file=${ABS_DUMP_PATH}.'/'.$name;
      }
    }
    if (empty($file)) {
      echo "\n\e[31m\e[1mNo dump file !\e[0m\n\n";
      exit(1);
    }
  } else {
    $file=ABS_DUMP_PATH.'/'.$file;
    if(!is_file($file)) {
      echo "\n\e[31m\e[1mFile ($file) does not exist !\e[0m\n\n";
      exit(1);
    }
  }
  echo $file;
  
  $cmd = ABS_SCRIPTS_PATH.'/'.'drush @'.ID." sql-cli < $file";
  system($cmd);
  echo "\ngo back to previous snapshot...\n\n";
}
