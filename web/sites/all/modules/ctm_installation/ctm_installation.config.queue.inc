<?php

/**
 * generate xmlsitemap
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_xmlsitemap_queue($profile, &$roles, $op = 'update') {
  ctm_installation_delete_alias_all();
  ctm_installation_generate_static_alias();
  ctm_installation_queue_generate_alias(array('node','term'));
}

function ctm_installation_config_auto_perm_queue($profile, &$roles, $op = 'update') {
  
  //reset permissions
  $all_perm = array_keys(module_invoke_all('permission'));
  foreach($roles as $rid) {
    if($rid != $roles[ADMINISTRATEUR]) {
      user_role_revoke_permissions($rid, $all_perm);
    }
  }
  
  // ADMINISTRATEUR management
  user_role_grant_permissions($roles[ADMINISTRATEUR], $all_perm);
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
  
  $matrice = ctm_installation_get_permission_matrix();
  foreach($matrice as $module => $cts) {
    foreach($cts as $ct => $rolesMat) {
      if($ct != 'default') {
        foreach($rolesMat as $role => $groups) {
          if($role != ADMINISTRATEUR) {
        foreach($groups as $group => $active) {
          if($active) {
            ctm_installation_set_permissions($ct, $group, $role, 'grant', $module);
              }
            }
          }
        }
      }
    }
  }
}