<?php
/**
 * +-----------------------------------------------------------+
 * |                                                           |
 * | Lib for URL manipulation                                  |
 * |                                                           |
 * +-----------------------------------------------------------+
 * | version : VERSION_APP                                  |
 * +-----------------------------------------------------------+
 **/
global $SITE_URLS, $URL_ALIAS, $URL_SITES;

$results = array();
$BASE_ROOT = array();
$URL_ALIAS = array();
foreach($SITE_URLS as $key => $site) {
  $results[$key] = parse_url($site);
  if(!empty($results[$key]['port']) && $results[$key]['port'] != 80) {
    $port = ':'.$results[$key]['port'];
    $results[$key]['port'] .= '.';
  } else {
    $results[$key]['port'] = '';
    $port = '';
  }
  $BASE_ROOT[$key] = $results[$key]['scheme'].'://'.$results[$key]['host'].$port;
  if(empty($results[$key]['path']) || strlen($results[$key]['path']) <= 1) {
    $results[$key]['path'] = '';
    $path = '';
  } else {
    $path = str_replace('/', '.',$results[$key]['path']);
  }
  $URL_ALIAS[$key] = $results[$key]['path'];
  $URL_SITES[$key] = $results[$key]['port'].$results[$key]['host'].$path;
}
define('HOST0', $results[0]['host']);
define('BASE_ROOT_LIST', implode('\', \'',$BASE_ROOT));
define('ALIAS0', $URL_ALIAS[0]);

