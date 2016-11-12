<?php

/**
 * gestion des droits
 */
function ctm_installation_config_ext_back_permissions_after($profile, $roles, $op = 'update') {
  $permission_list = array(
      'epsacrop create crops',
      'epsacrop edit all crops',
      'epsacrop delete all crops',
  );
  user_role_grant_permissions($roles[CONTRIBUTEUR], $permission_list);
  user_role_grant_permissions($roles[ADMIN_SITE], $permission_list);
}

