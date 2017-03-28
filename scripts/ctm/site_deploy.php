<?php

/**
 * +-----------------------------------------------------------+
 * |                                                           |
 * | code for the site deploy command                          |
 * |                                                           |
 * +-----------------------------------------------------------+
 * | version : 1                                               |
 * +-----------------------------------------------------------+
**/

/**
 * 
 **/
function create_sites() {
  if (!file_exists(ABS_CONFIG_PATH."/sites.php")) {
    $fd = fopen(ABS_CONFIG_PATH."/sites.php", 'w');
    fwrite($fd, "<?php\n");
    fclose($fd);
    setRight('dev');
  }
}

/**
 * 
 * */
function create_site() {
  global $SITE_URLS, $URL_ALIAS, $URL_SITES;
  
  setRight('dev');
  copy(ABS_SOURCE_PATH.'/default.settings.php', ABS_SITES_PATH.'/default/default.settings.php');
  if(!is_dir(ABS_SITE_PATH)) {
    echo "Create site directory...\n";
    rcopy(ABS_SITES_PATH.'/default', ABS_SITE_PATH);
    rcopy(ABS_MEDIAS_PATH.'/default', ABS_MEDIA_PATH);
    setRight('dev');
    unlink(ABS_SITE_PATH.'/default.settings.php');
  } else {
    echo "Reinitializing setting.php...\n";
    if(!is_dir(ABS_MEDIA_PATH)) {
      copyr(ABS_MEDIAS_PATH.'/default', ABS_MEDIA_PATH);
    }
    if(is_file(ABS_SITE_PATH.'/settings.php')) {
      rename(ABS_SITE_PATH.'/settings.php', ABS_SITE_PATH.'/settings.php.orig');
    }
    setRight('dev');
  }
  echo "Create settings.php...\n";
  $content = file_get_contents(ABS_SITES_PATH.'/default/default.settings.php');
  $PATTERN = '/base_root_list\'] = array.*/';
  $RESULT  = "base_root_list'] = array('".BASE_ROOT_LIST."');\n";
  $content = preg_replace($PATTERN, $RESULT, $content, 1);
  $PATTERN = '/\'environnement\'] = \'.*/';
  $RESULT  = "'environnement'] = '".ENV."';\n";
  $content = preg_replace($PATTERN, $RESULT, $content, 1);
  if(ALIAS0 != "") {
    $PATTERN = '/\'base_path\'] = \'.*/';
    $RESULT  = "'base_path'] = '".ALIAS0."';\n";
    $content = preg_replace($PATTERN, $RESULT, $content, 1);
  }
  $parse = parse_url(DATABASE);
  $PATTERN = '/\$databases = array\(\);/';
  $RESULT   = "\$databases['default']['default'] = array(\n";
  $RESULT  .= "'driver' => '".$parse['scheme']."',\n";
  $RESULT  .= "'port' => '".$parse['port']."',\n";
  $RESULT  .= "'database' => '".str_replace('/','',$parse['path'])."',\n";
  $RESULT  .= "'username' => '".$parse['user']."',\n";
  $RESULT  .= "'password' => '".$parse['pass']."',\n";
  $RESULT  .= "'host' => '".$parse['host']."',\n";
  $RESULT  .= "'charset' => 'utf8mb4',\n";
  $RESULT  .= "'collation' => 'utf8mb4_general_ci',\n";
  $RESULT  .= ");\n";
  $content = preg_replace($PATTERN, $RESULT, $content, 1);
  file_put_contents(ABS_SITE_PATH.'/settings.php', $content);
  
  $content = file_get_contents(ABS_CONFIG_PATH.'/sites.php');
  $content = explode("\n", $content);
  $content = preg_grep('/'.ID.'/', $content, 1);
  foreach($URL_SITES as $f) {
    $content = preg_grep('/'.$f.'/', $content, 1);
    $content[] = '$sites[\''.$f.'\'] = \''.ID.'\';';
  }
  $content = implode("\n",$content);
  file_put_contents(ABS_CONFIG_PATH.'/sites.php', $content);
  copy(ABS_SITE_PATH.'/settings.php', ABS_SITES_PATH.'/default/default.settings.php');
  setRight('dev');
}

/**
 * 
 **/
function read_config() {
  
  //local
  $local_file = ABS_CONFIG_PATH.'/'.ID.LOCAL_CONF;
  if (!file_exists($local_file)) {
    $example_local = ABS_CONFIG_PATH.'/'.EXAMPLE.LOCAL_CONF;
    echo "\n\e[31m\e[1mFile local_file} is missing create it by copy of example_local}\e[0m\n\n";
    exit(1);
  }
  include $local_file;
  if (empty($SITE_URLS) || 
      DATABASE == "" || DATABASE == "mysql://db_user:db_password@localhost:3306/db_name" || 
      ADMIN_MAIL == "") {
    echo "\n\e[31m\e[1mFile global_file} is empty\e[0m\n\n";
    exit(1);
  }

  //global
  $global_file=ABS_CONFIG_PATH.'/'.ID.GLOBAL_CONF;
  if (!file_exists($global_file)) {
    $example_global = ABS_CONFIG_PATH.'/'.EXAMPLE.GLOBAL_CONF;
    echo "\n\e[31m\e[1mFile global_file} is missing create it by copy of example_global}\e[0m\n\n";
    exit(1);
  }
  include $global_file;
  if (SITE_NAME == "" || LANG == "" || PROFIL == "") {
    echo "\n\e[31m\e[1mFile global_file} is empty or wrong values\e[0m\n\n";
    exit(1);
  }
  include ABS_SOURCE_PATH.'/url.php';
}

/**
 * 
 **/
function create_drush_alias() {
  global $SITE_URLS, $URL_ALIAS, $URL_SITES;
  echo "create drush alias...\n";
  $data  = "<?php\n";
  $data .= "\$options['uri'] = '".$URL_SITES[0]."';\n";
  $data .= "\$options['root'] = '".ABS_DOCUMENT_ROOT."';\n";
  file_put_contents(ABS_DRUSH_ALIAS.'/'.ID.'.alias.drushrc.php', $data);
}

/**
 * 
 **/
function get_lang() {
  include_once ABS_DOCUMENT_ROOT.'/includes/bootstrap.inc';
  $pofile="drupal-".VERSION.'.'.LANG.'.po';
  $major = explode('.', VERSION);
  $major = $major[0];
  $popath=ABS_SITES_PATH.'/'.ID.'/'.TRANSLATIONS_PATH.'/'.$pofile;
  if (!file_exists($popath)) {
    echo "Download translation : http://ftp.drupal.org/files/translations/".$major.".x/drupal/pofile";
    $ch = curl_init("http://ftp.drupal.org/files/translations/".$major.".x/drupal/pofile"); 
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
    $content = curl_exec($ch); 
    file_put_contents($popath, $content);
  }
}

/**
 * 
 **/
function create_htaccess() {
  global $SITE_URLS, $URL_ALIAS, $URL_SITES;
  echo "Update .htaccess...\n";
  $content = file_get_contents(ABS_DOCUMENT_ROOT.'/.htaccess');
  foreach($URL_ALIAS as $f) {
    if($f != "") {
      if(strpos($content, $f.'/index.php') === FALSE) {
        $TEXT="CTM_MANAGER_TAG\nRewriteCond %{REQUEST_URI} ^".$f."/\nRewriteRule ^ /".$f."/index.php [L]\n";
        $content = str_replace('CTM_MANAGER_TAG',$TEXT,$content);
      }
    }
  }
  file_put_contents(ABS_DOCUMENT_ROOT.'/.htaccess', $content);
}

/**
 * 
 **/
function finalize() {
  echo "Cutting settings...\n";
  setRight('dev');
  $content = file_get_contents(ABS_SITE_PATH.'/settings.php');
  $content = explode("\n", $content);
  $part1 = array();
  $cut = 0;
  foreach($content as $key => $value) {
    if(strpos($value, "====== CUT HERE ======") !== FALSE) {
      $cut = $key;
      break;
    }
    $part1[] = $value;
  }
  $part1 = implode("\n", $part1);
  file_put_contents(ABS_CONFIG_PATH.'/settings-'.ID.'.php', $part1);
  
  if(file_exists(ABS_SITE_PATH.'/settings.php.orig')) {
    unlink(ABS_SITE_PATH.'/settings.php');
    rename(ABS_SITE_PATH.'/settings.php.orig', ABS_SITE_PATH.'/settings.php');
  } else {
    copy(ABS_SOURCE_PATH.'/header.settings.php', ABS_SITE_PATH.'/settings.php');
    setRight('dev');
    $end = count($content);
    $part2 = array();
    for($i = $cut+1;$i < $end; $i++) {
      $part2[] = $content[$i];
    }
    $part2 = implode("\n", $part2);
    file_put_contents(ABS_SITE_PATH.'/settings.php', $part2, FILE_APPEND);
  }
  copy(ABS_CONFIG_PATH.'/example.masquerade-default.xml', ABS_CONFIG_PATH.'/masquerade-'.ID.'.xml');
  copy(ABS_CONFIG_PATH.'/example.mock-default.xml', ABS_CONFIG_PATH.'/mock-'.ID.'.xml');
  copy(ABS_SOURCE_PATH.'/default.settings.php', ABS_SITES_PATH.'/default/default.settings.php');
  setRight(ENV);
}

/**
 * 
 **/
function addcmd() {
  $content = file_get_contents(ABS_SCRIPTS_PATH.'/drush');
  $content = str_replace("drush.php \"", "drush.php @".ID." \"", $content);
  file_put_contents(ABS_SCRIPTS_PATH.'/@'.ID, $content);
  $content = file_get_contents(ABS_SCRIPTS_PATH.'/drush.bat');
  $content = str_replace("%*", "@".ID." %*", $content);
  file_put_contents(ABS_SCRIPTS_PATH.'/@'.ID.".bat", $content);
}

/**
 * 
 * @param type $s
 * @param type $step
 **/
function site_deploy($s, $step) {
  read_config();
  setRight('dev');
  if($s == "-s" && $step == "1" || $s == "") {
    create_sites();
    create_site();
    create_htaccess();
    create_drush_alias();
    addcmd();
  }
  if ($s == "-s" && $step == "2" || $s == "") {
    if (LANG == "en" || LANG == "EN") {
      $LOCAL="";
    } else {
      $LOCAL="--locale=\"".LANG."\"";
      get_lang();
    }
    setRight('dev');
    chdir(ABS_SITE_PATH);
    system("php \"".ABS_DRUSH_PATH."/drush.php\" site-install ".PROFIL." -y $LOCAL --sites-subdir=\"".ID."\" --root=\"".ABS_DOCUMENT_ROOT."\" --account-name=\"".ADMIN_NAME."\" --account-mail=\"".ADMIN_MAIL."\" --site-mail=\"no-reply@".HOST0."\" --site-name=\"".SITE_NAME."\" --db-url=\"".DATABASE."\"");
    system(ABS_SCRIPTS_PATH."/drush @".ID." ctm_queue");
    if(ENV == "dev") {
      system(ABS_SCRIPTS_PATH."/drush @".ID." ctm_tools devel");
      dump();
    }
    echo "\nDONT FORGET TO NOTE the administrator password display above this message\n\n";
  }
  if ($s == "-s" && $step == "3" || $s == "") {
    finalize();
  }
  setRight(ENV);
}


