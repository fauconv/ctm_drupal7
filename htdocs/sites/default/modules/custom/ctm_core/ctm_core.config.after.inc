<?php

/**
 * @file
 * fonctions execute avant les autres et avant le formulaire d'installation
 * format des fonction ici :
 * ctm_installation_config_XXXXXX_after($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 * ATTENTION il s'agit bien de fonction CTM_INSTALLATION
 */

/**
 * set website name
 **/
function ctm_installation_config_core_site_name_after($profile, $roles, $op = 'update') {
  variable_set('site_name', SITE_NAME);
}


/**
 * add static field for page content type
 * modify format of texte field (ckeditor)
 */
/*
function ctm_installation_config_core_static_field_after($profile, $roles, $op = 'update') {
  // field
  $field_name = 'field_static_content';
  $type = 'page_de_base';
  $field_texte_page_base = array(
      'field_name' => $field_name,
      'entity_types' => array('node'),
      'type' => 'blob',
  );
  ctm_installation_insert_or_update_field($field_texte_page_base);
  // instance
  $instances_texte_page_base = array(
      'field_name' => $field_name,
      'entity_type' => 'node',
      'bundle' => $type,
      'label' => 'Static content',
      'display' => array(
          'default' => array(
              'label' => 'hidden',
              'module' => 'field_from_theme',
              'settings' => array(),
              'type' => 'field_from_theme',
              'weight' => 0,
          ),
      ),
      'settings' => array(
      'entity_translation_sync' => FALSE,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'field_from_theme',
      'settings' => array(),
      'type' => 'widget_from_theme',
      'weight' => 41,
    ),
  );
  ctm_installation_insert_or_update_field_instance($instances_texte_page_base);
  //display
  ctm_installation_set_default_display_cache_field($type, array($field_name));
  ctm_installation_set_multi_language_field($field_name);
  
  $permission_list = array('allow users to pick page_de_base templates');
  user_role_grant_permissions($roles[CONTRIBUTEUR], $permission_list);
  user_role_grant_permissions($roles[ADMIN_SITE], $permission_list);
  user_role_grant_permissions($roles[ADMINISTRATEUR], $permission_list);
  
}
*/

/**
 * desactivation de l anglais si on en veut pas
 * et activer d'autres langue
 */
/*
function ctm_installation_config_core_local_after($profile, $roles, $op = 'update') {
  if($op != 'update') {
    locale_add_language('ja');
    locale_add_language('de');
    locale_add_language('es');
    locale_add_language('it');
    locale_add_language('ko');
    locale_add_language('pt');
    locale_add_language('ru');
    locale_add_language('th');
    locale_add_language('zh-hans');
    locale_add_language('zh-hant');
  }
    variable_set('language_count', variable_get('language_count', 2) - 1);
    db_update('languages')
      ->fields(array('enabled' => 0))
      ->condition('language', 'en')
      ->execute();
}
*/

/**
 * active les features specifique du site
 */
function ctm_installation_config_core_features_after($profile, $roles, $op = 'update') {
  variable_set('uuid_features_entity_node', array(
    'page_de_base' => 'page_de_base',
    'block' => 'block',
  ));
  //ctm_installation_queue_module_enable(array('fte_menu'));
}

/**
 * rebuild all permission
 **/
function ctm_installation_config_core_rebuild_perm_after($profile, $roles, $op = 'update') {
  ctm_installation_queue_access_rebuild();
}

/**
 * Import all .po files
 **/
/*
function ctm_installation_config_core_translate_after($profile, $roles, $op = 'update') {
  ctm_installation_queue_import_po();
}
*/
