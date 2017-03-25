<?php

/**
 * 
 */
function site_remove($force) {
  setRight('dev');
  @unlink(ABS_CONFIG_PATH.'/settings-'.ID.'.php');
  @unlink(ABS_CONFIG_PATH.'/mock-'.ID.'.xml');
  @unlink(ABS_CONFIG_PATH.'/masquerade-'.ID.'.xml');
  @unlink(ABS_SCRIPTS_PATH.'/@'.ID);
  @unlink(ABS_SCRIPTS_PATH.'/@'.ID.'.bat');
  if($force == '-f') {
    @unlink(ABS_SITE_PATH.'/settings.php');
  }
  echo "\nRemoving ".ID."...\n\n";
}
