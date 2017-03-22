<?php

/**
 * helper fonction
 * creer un element par defaut de la matrice des droits (tout a false)
 *    'create' => create content
 *    'view' => view content
 *    'update' => modify content
 *    'delete' => remove content
 *    'publish' => publish
 *    'admin' => administration permissions
 * @return type
 */
function ctm_installation_get_default_perms() {
  return array(
      'create' => FALSE,
      'read' => FALSE,
      'update' => FALSE,
      'delete' => FALSE,
      'publish' => FALSE,
      'admin' => FALSE,
  );
}

/**
 * helper fonction
 * creer un element par defaut de la matrice des droits (tout a vrai sauf admin)
 *    'create' => create content
 *    'view' => view content
 *    'update' => modify content
 *    'delete' => remove content
 *    'publish' => publish
 *    'admin' => administration permissions
 * @return type
 */
function ctm_installation_get_true_perms() {
  return array(
      'create' => TRUE,
      'read' => TRUE,
      'update' => TRUE,
      'delete' => TRUE,
      'publish' => TRUE,
      'admin' => FALSE,
  );
}

/**
 * helper fonction
 * creer un element par defaut de la matrice des droits (tout a vrai sauf admin)
 *    'create' => create content
 *    'view' => view content
 *    'update' => modify content
 *    'delete' => remove content
 *    'publish' => publish
 *    'admin' => administration permissions
 * @return type
 */
function ctm_installation_get_create_perms() {
  return array(
      'create' => TRUE,
      'read' => TRUE,
      'update' => TRUE,
      'delete' => FALSE,
      'publish' => FALSE,
      'admin' => FALSE,
  );
}

/**
 * helper fonction
 * creer un element par defaut de la matrice des droits (tout a vrai sauf admin)
 *    'create' => create content
 *    'view' => view content
 *    'update' => modify content
 *    'delete' => remove content
 *    'publish' => publish
 *    'admin' => administration permissions
 * @return type
 */
function ctm_installation_get_read_perms() {
  return array(
      'create' => FALSE,
      'read' => TRUE,
      'update' => FALSE,
      'delete' => FALSE,
      'publish' => FALSE,
      'admin' => FALSE,
  );
}

function ctm_installation_read_perm($role, $ct = NULL) {
  $list = array(
    'access content' => 'access content'
  );
  return $list;
}

/**
 * 
 * @param string $role
 * @param string $ct
 * @return array
 */
function ctm_installation_create_perm($role, $ct = 'default') {
  $list = array();
  if($ct != 'default') {
    $list = array(
          'create ' . $ct . ' content' => 'create ' . $ct . ' content'
    );
  }
  return $list;
}

function ctm_installation_update_perm($ct) {
  $list = array_merge(
      ctm_installation_read_perm($ct), 
      array(
        'display EU Cookie Compliance popup' =>'display EU Cookie Compliance popup',
        'update any ' . $ct . ' content' => 'update any ' . $ct . ' content',  // og
        'edit any ' . $ct . ' content' => 'edit any ' . $ct . ' content',
        'edit own ' . $ct . ' content' => 'edit own ' . $ct . ' content',
        'view any unpublished ' . $ct . ' content' => 'view any unpublished ' . $ct . ' content',
        'enter ' . $ct . ' revision log entry' => 'enter ' . $ct . ' revision log entry',
        'override ' . $ct . ' revision option' => 'override ' . $ct . ' revision option',
        'override ' . $ct . ' promote to front page option' => 'override ' . $ct . ' promote to front page option',
        'override ' . $ct . ' sticky option' => 'override ' . $ct . ' sticky option',
        'view revisions' =>'view revisions',
        'access administration pages' =>'access administration pages',
        'view the administration theme' =>'view the administration theme',
        'access site in maintenance mode' =>'access site in maintenance mode',
        'access content overview' =>'access content overview',
        'execute node_make_sticky_action' =>'execute node_make_sticky_action',
        'execute node_make_unsticky_action' =>'execute node_make_unsticky_action',
        'execute node_promote_action' =>'execute node_promote_action',
        'execute node_unpromote_action' =>'execute node_unpromote_action',
        'revert revisions' =>'revert revisions',
        'define menu item for node' =>'define menu item for node',
        'access administration menu' =>'access administration menu', 
        'flush caches' =>'flush caches',
        'boost flush pages' =>'boost flush pages',
        'view own unpublished content' =>'view own unpublished content',
        'show format selection for node' =>'show format selection for node',
        'show format selection for taxonomy_term' =>'show format selection for taxonomy_term',
      )
  );
  return $list;
}

/**
 * 
 * @param string $role
 * @param string $ct
 * @return array
 */
function ctm_installation_delete_perm($role, $ct = 'default') {
  $list = array();
  if($ct != 'default') {
    $list = array(
          'delete any ' . $ct . ' content' => 'delete any ' . $ct . ' content',
          'delete own ' . $ct . ' content' => 'delete own ' . $ct . ' content',
    );
  } else {
    $list = array(
          'delete revisions' => 'delete revisions',
          'execute views_bulk_operations_delete_item' => 'execute views_bulk_operations_delete_item',
          'execute views_bulk_operations_delete_revision' => 'execute views_bulk_operations_delete_revision',
    );
  }
  return $list;
}

/**
 * 
 * @param string $role
 * @param string $ct
 * @return array
 */
function ctm_installation_publish_perm($role, $ct = 'default') {
  $list = array();
  if($ct != 'default') {
    $list = array(
          'publish any ' . $ct . ' content' => 'publish any ' . $ct . ' content',
          'unpublish any ' . $ct . ' content' => 'unpublish any ' . $ct . ' content',
          'publish own ' . $ct . ' content' => 'publish own ' . $ct . ' content',
          'unpublish own ' . $ct . ' content' => 'unpublish own ' . $ct . ' content',
          'publish editable ' . $ct . ' content' => 'publish editable ' . $ct . ' content',
          'unpublish editable ' . $ct . ' content' => 'unpublish editable ' . $ct . ' content',
          'override ' . $ct . ' published option' => 'override ' . $ct . ' published option', // sera masquer en css c est juste pour avoir l info
    );
  } else {
    $list = array(
          'schedule publishing of nodes' => 'schedule publishing of nodes',
          'execute node_publish_action' => 'execute node_publish_action',
          'execute node_unpublish_action' => 'execute node_unpublish_action',
          'execute node_unpublish_by_keyword_action' => 'execute node_unpublish_by_keyword_action',
          'diff view changes' => 'diff view changes',
    );
  }
  return $list;
}


function ctm_installation_assign_role($role) {
  $roles = ctm_installation_user_roles();
  return array(
    'assign '.$role.' role',
    'edit users with role '.$roles[$role],
    'cancel users with role '.$roles[$role],
  );
}

/**
 * 
 * @param string $role
 * @param string $ct
 * @return array
 */
function ctm_installation_admin_perm($role, $ct = 'default') {
  $list = array();
  if($ct == 'default') {
    $list = array_merge(
        ctm_installation_assign_role(ADMIN_SITE), 
        array(
          'maintain nodeblock' => 'maintain nodeblock',
          'execute node_assign_owner_action' => 'execute node_assign_owner_action',
          'execute views_bulk_operations_change_owner_action' => 'execute views_bulk_operations_change_owner_action',
          'execute views_bulk_operations_user_cancel_action' => 'execute views_bulk_operations_user_cancel_action', 
          'execute system_message_action' => 'execute system_message_action',
          'execute system_send_email_action' => 'execute system_send_email_action',
          'execute system_block_ip_action' => 'execute system_block_ip_action',
          'execute user_block_user_action' => 'execute user_block_user_action',
          'administer main-menu menu items' => 'administer main-menu menu items',
          'view any unpublished content' => 'view any unpublished content',
          'create url aliases' => 'create url aliases',
          'view scheduled content' => 'view scheduled content',
          'access user profiles' => 'access user profiles',
          'create users' => 'create users',
          'access users overview' => 'access users overview',
          'administer redirects' => 'administer redirects',
          'edit users with role '.DRUPAL_AUTHENTICATED_RID => 'edit users with role '.DRUPAL_AUTHENTICATED_RID,
          'cancel users with role '.DRUPAL_AUTHENTICATED_RID => 'cancel users with role '.DRUPAL_AUTHENTICATED_RID,
        )
    );
  }
  return $list;
}

/**
 * Liste des roles et des droits par content type
 * matrice des droits par defauts par role. C'est de la que l'on defini tous
 * les droits.
 * Le niveau defini les diferente matrice (standard ou og)
 * le cas 'default' dans le sous niveau est le cas de la gestion des droits 
 * dans les vues quand on ne sait pas quel ct on va obtenir
 *    'create' => create content
 *    'view' => view content
 *    'update' => modify content
 *    'delete' => remove content
 *    'publish' => publish
 *    'admin' => administration permissions
 * 
 */
function ctm_installation_get_permission_matrix() {
  static $matrice; // Use the advanced drupal_static() pattern, since this is called very often.
  if (!isset($matrice)) {
    $matrice = &drupal_static(__FUNCTION__, array());
  }
  $matrice = array('user' => array());
  $matrice['user']['default'] = array(
      ADMINISTRATEUR => ctm_installation_get_true_perms(),
      ADMIN_SITE => ctm_installation_get_true_perms(),
      AUTHENTICATED => ctm_installation_get_default_perms(),
      ANONYMOUS => ctm_installation_get_default_perms(),
  );
  $matrice['user']['default'][ADMINISTRATEUR]['admin'] = TRUE;
  $matrice['user']['default'][ADMIN_SITE]['admin'] = TRUE;
  $matrice['user']['default'][AUTHENTICATED]['read'] = TRUE;
  if (drupal_get_profile() == CTM_INTERNET) {
    $matrice['user']['default'][ANONYMOUS]['read'] = TRUE;
  }  
  $types = array_keys(node_type_get_names());
  foreach ($types as $type) {
    $matrice['user'][$type] = $matrice['user']['default'];
  }
  drupal_alter('installation_permission_matrix', $matrice);
  return $matrice;
}

/**
 * grant or revoke a group of permissions for a content type, for a role
 * @param string $ct : content type
 * @param array $group : a group name of permission define by hook_installation_permission
 * @param array $role : role name
 * @param string : "grant" or "revoke"
 * @param string : "user" or "og"
 */
function ctm_installation_set_permissions($ct, $group, $role, $grant = 'grant', $module = 'user') {
  
  if ($module == 'user') {
    $tabRoles = ctm_installation_user_roles();
  } else {
    // add og function
  }
  
  $perm = array();
  $perm_groups = module_invoke_all('installation_permissions');
  $func_list = $perm_groups[$group];
  foreach($func_list as $callback) {
    $perm = array_merge($perm,$callback($ct));
  }
  
  if ($module == 'user') {
    $permissions = array_keys(module_invoke_all('permission'));
    $perm = array_intersect($perm, $permissions);
    if($grant == 'grant') {
      $var = variable_get('content_type_extras_default_settings', array());
      $new_perm = '';
      switch($group) {
        case 'create':
          $new_perm = 'create_roles';
          break;
        case 'update':
          $new_perm = 'edit_roles';
          break;
        case 'delete':
          $new_perm = 'delete_roles';
          break;
        default:
          continue;
      }
      if ($new_perm != '') {
        $var['user_permissions'][$new_perm][$tabRoles[$role]] = $tabRoles[$role];
      }
      variable_set('content_type_extras_default_settings', $var);
    }
  } else {
    $permissions = array_keys(module_invoke_all($module.'_permission'));
    $perm = array_intersect($perm, $permissions);
  }
  $callback = $module . '_role_' . $grant . '_permissions';
  $callback($tabRoles[$role], $perm);
}
