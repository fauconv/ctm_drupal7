<?php

/**
 * dump
 */
function dump() {
  echo "\ndumping database...\n";
  $file = date('Ymd_his');
  $cmd = ABS_SCRIPTS_PATH.'/'.'drush @'.ID." sql-dump > ".ABS_DUMP_PATH."/".ID."_$file.sql";
  system($cmd);
}
