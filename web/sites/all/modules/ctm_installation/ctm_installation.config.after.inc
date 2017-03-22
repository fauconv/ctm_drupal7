<?php

/**
 * @file
 * fonction de configuration executer apres le formulaire d'installation
 * format des fonction ici :
 * ctm_installation_config_XXXXXX($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 */

/**
 * gestion des droits de base et role developer
 */
function ctm_installation_config_permissions_after($profile, $roles, $op = 'update') {
  
  // ADMINISTRATEUR management
  user_role_grant_permissions($roles[ADMINISTRATEUR], array_keys(module_invoke_all('permission')));
  $permissions = array_keys(module_invoke_all('permission'));
  $permission_list = array(
      'show format tips',
      'show more format tips link',
      'customize ckeditor',
      'publish any content',
      'unpublish any content',
      'publish editable content',
      'unpublish editable content',
      'allow users to pick menu_link templates',
      'allow users to pick user templates',
      'allow users to pick features templates',
      'allow users to pick main-menu templates',
      'allow users to pick management templates',
      'allow users to pick navigation templates',
      'allow users to pick user-menu templates',
      'allow users to pick rules_config templates',
  );
  $permission_list = array_intersect($permission_list, $permissions);
  user_role_revoke_permissions($roles[ADMINISTRATEUR], $permission_list);
  ctm_installation_queue_add_role(array(1), $roles[ADMINISTRATEUR]);

  //cas specifique au profil d'install
  if ($profile == CTM_INTERNET) {
    user_role_grant_permissions(DRUPAL_ANONYMOUS_RID, array('access content', 'display EU Cookie Compliance popup'));
  }
  elseif ($profile == CTM_INTRANET) {
    user_role_grant_permissions(DRUPAL_AUTHENTICATED_RID, array('access content'));
  }
  
  //create ADMIN_SITE
  if ($op != 'update') {
    $base_root_list = variable_get('base_root_list');
    $url = str_replace('http://', '', str_replace('https://', '', $base_root_list[0]));

    //create admin_site user
    $new_user = new stdClass();
    $new_user->name = 'admin_site';
    $new_user->pass = user_password();
    $new_user->mail = $new_user->name . '@' . $url;
    $new_user->status = 1;
    $new_user->roles = array($roles[ADMIN_SITE] => ADMIN_SITE);
    user_save($new_user);
    $message = '  User name: ' . $new_user->name . '  User password: ' . $new_user->pass;
    drupal_set_message($message);
  }

  }

/**
 * configuration du module local
 */
function ctm_installation_config_local_after($profile, $roles, $op = 'update') {
  //Initialise le premier jour de la semaine
  variable_set('date_first_day', 1);
  //Les utilisateurs ne peuvent pas definir leur propre fuseau horaire
  variable_set('configurable_timezones', 0);
}

/**
 * config du cache
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_cache_after($profile, $roles, $op = 'update') {
  variable_set('block_cache_bypass_node_grants', TRUE);
  variable_set('cache', TRUE);
  variable_set('block_cache', TRUE);
  variable_set('cache_lifetime', '0');
  variable_set('page_cache_maximum_age','86400');
  $env = variable_get('environnement','prod');
  if($env != 'dev') {
    variable_set('preprocess_css',1);
    variable_set('preprocess_js',1);
  }
}

/**
 * configuration de xautoload
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_xautoload_after($profile, $roles, $op = 'update') {
  ctm_installation_add_to_queue('_ctm_installation_xautoload_batch', array(), '', 'ctm_installation_check_cli');
}
