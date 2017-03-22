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
  $matrice = ctm_installation_get_permission_matrice();
  foreach($matrice as $module => $cts) {
    foreach($cts as $ct => $roles) {
      foreach($roles as $role => $groups) {
        foreach($groups as $group => $active) {
          if($active) {
            ctm_installation_set_permissions($ct, $group, $role, 'grant', $module);
          } else {
            ctm_installation_set_permissions($ct, $group, $role, 'revoke', $module);
          }
        }
      }
    }
  }
}