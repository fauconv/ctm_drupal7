<?php

function ctm_installation_read_perm($ct = null) {
  $list = array(
    'access content' => 'access content'
  );
  return $list;
}

function ctm_installation_create_perm($ct) {
  $list = array_merge(
      ctm_installation_update_perm($ct),
      array(
        'create ' . $ct . ' content' => 'create ' . $ct . ' content'
      )
  );
  return $list;
}

function ctm_installation_update_perm($ct) {
  $list = array_merge(
      ctm_installation_read_perm($ct), 
      array(
        'display EU Cookie Compliance popup' =>'display EU Cookie Compliance popup',
        'update any ' . $ct . ' content' => 'update any ' . $ct . ' content',  // og
        'edit any ' . $ct . ' content' => 'edit any ' . $ct . ' content',
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
      )
  );
  return $list;
}

function ctm_installation_delete_perm($ct) {
  $list = array_merge(
      ctm_installation_update_perm($ct),
      array(
        'delete any ' . $ct . ' content' => 'delete any ' . $ct . ' content',
        'delete revisions' => 'delete revisions',
        'execute views_bulk_operations_delete_item' => 'execute views_bulk_operations_delete_item',
      )
  );
  return $list;
}
function ctm_installation_publish_perm($ct) {
  $list = array_merge(
      ctm_installation_read_perm($ct), 
      array(
        'publish any ' . $ct . ' content' => 'publish any ' . $ct . ' content',
        'unpublish any ' . $ct . ' content' => 'unpublish any ' . $ct . ' content',
        'override ' . $ct . ' published option' => 'override ' . $ct . ' published option', // sera masquer en css c est juste pour avoir l info
        'schedule publishing of nodes' => 'schedule publishing of nodes',
        'execute node_publish_action' => 'execute node_publish_action',
        'execute node_unpublish_action' => 'execute node_unpublish_action',
      )
  );
  return $list;
}

function ctm_installation_admin_perm($ct) {
  $list = array_merge(
      ctm_installation_create_perm($ct), 
      ctm_installation_delete_perm($ct),
      ctm_installation_publish_perm($ct),
      array(
        'maintain nodeblock' => 'maintain nodeblock',
        'administer ctm configuration' => 'administer ctm configuration',
        'administer main-menu menu items' => 'administer main-menu menu items',
        'view any unpublished content' => 'view any unpublished content',
        'create url aliases' => 'create url aliases',
        'view scheduled content' => 'view scheduled content',
        'access user profiles' => 'access user profiles',
        'assign '.ADMIN_SITE.' role' => 'assign '.ADMIN_SITE.' role',
        'create users' => 'create users',
        'access users overview' => 'access users overview',
        'administer redirects' => 'administer redirects',
        'edit users with role '.DRUPAL_AUTHENTICATED_RID => 'edit users with role '.DRUPAL_AUTHENTICATED_RID,
        'cancel users with role '.DRUPAL_AUTHENTICATED_RID => 'cancel users with role '.DRUPAL_AUTHENTICATED_RID,
      )
  );
  return $list;
}