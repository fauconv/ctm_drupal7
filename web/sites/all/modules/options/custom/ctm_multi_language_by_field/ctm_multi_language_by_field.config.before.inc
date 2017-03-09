<?php

/**
 * @file
 * fonctions execute apres le formulaire d'installation
 * format des fonction ici :
 * ctm_installation_config_XXXXXX_after($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 * ATTENTION il s'agit bien de fonction CTM_INSTALLATION
 */

/**
 * add prefix for english url + french
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_multi_language_prefix_before($profile, $roles, $op = 'update') {
  if($op == 'update') {
    $list = locale_language_list();
    if(!isset($list['fr'])) {
      locale_add_language('fr');
    }
  } else {
    locale_add_language('fr'); // it is necessary to ad a second language
  }
  db_update('languages')
      ->fields(array('prefix' => 'en'))
      ->fields(array('weight' => 100)) // default language must be the last
      ->condition('language', 'en')
      ->execute();
}
