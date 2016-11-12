<?php

/**
 * Config de du systeme de mail comporte 3 modules :
 * mimemail, phpmailer, et mailsystem
 */
function ctm_installation_config_node_block_permissions_after($profile, $roles, $op = 'update') {
  $permission_list = array_merge($permission_list, array(
      'maintain nodeblock',
  ));
  user_role_grant_permissions($roles[ADMIN_SITE], $permission_list);
}

/**
 * Ajout des configuration pour le type de contenu block
 */
function ctm_installation_config_content_type_block_after($profile, $roles, $op = 'update') {
  $type = 'block';
  ctm_installation_set_default_block_perms($type, $roles, $profile);
}
