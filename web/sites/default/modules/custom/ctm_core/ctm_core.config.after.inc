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
 * Ajout des configuration pour le type de contenu page de base
 */
function ctm_installation_config_core_content_type_page_base_after($profile, $roles, $op = 'update') {
  $type = 'page_de_base';
  $field_name = 'texte_page_base';
  $content_type_bundle = array(
      'name' => 'Page',
      'description' => 'To create a basic page',
      'title_label' => 'Title',
      'type' => $type,
      'module' => 'ctm_installation',
      'base' => 'node_content',
      'locked' => 1,
      'custom' => 0,
  );
  $content_type_bundle = node_type_set_defaults($content_type_bundle);
  node_type_save($content_type_bundle);
  $bundle_settings = field_bundle_settings('node', $content_type_bundle->type);
  $bundle_settings['view_modes']['teaser']['custom_settings'] = FALSE;
  field_bundle_settings('node', $content_type_bundle->type, $bundle_settings);

  // champs texte
  $field_texte_page_base = array(
      'field_name' => $field_name,
      'entity_types' => array('node'),
      'type' => 'text_long',
  );
  ctm_installation_insert_or_update_field($field_texte_page_base);

  $largeurs = array_keys(variable_get('ckeditor_sizes', array('100' => 'p;h2;h3;h4;h5;h6;address;div')));
  end($largeurs);
  $format_name = 'ckeditor_images_liens_' . key($largeurs);
  $instances_texte_page_base = array(
      'field_name' => $field_name,
      'entity_type' => 'node',
      'bundle' => $type,
      'label' => 'Text',
      'display' => array(
          'default' => array(
              'label' => 'hidden',
              'type' => 'text_default',
              'weight' => 0,
          ),
      ),
      'settings' => array(
          'text_processing' => '1',
          'better_formats' => array(
              'allowed_formats_toggle' => 1,
              'allowed_formats' => array(
                  $format_name => $format_name,
              ),
              'default_order_toggle' => 0,
          ),
          'user_register_form' => FALSE,
      ),
      'widget' => array('type' => 'text_textarea'),
  );
  ctm_installation_insert_or_update_field_instance($instances_texte_page_base);

}


/**
 * gestion des droits contributeur
 */
function ctm_installation_config_core_permissions_after($profile, $roles, $op = 'update') {
  
  //create users
  if ($op != 'update') {
    $base_root_list = variable_get('base_root_list');
    $url = str_replace('http://', '', str_replace('https://', '', $base_root_list[0]));

    //create contributor user
    $new_user = new stdClass();
    $new_user->name = 'contributor';
    $new_user->pass = user_password();
    $new_user->mail = $new_user->name . '@' . $url;
    $new_user->status = 1;
    $new_user->roles = array($roles[CONTRIBUTEUR] => CONTRIBUTEUR);
    user_save($new_user);
    $message = '  User name: ' . $new_user->name . '  User password: ' . $new_user->pass;
    drupal_set_message($message);
  }
}

/**
 * active les features uuid specifique du site
 */
function ctm_installation_config_core_features_after($profile, $roles, $op = 'update') {
  variable_set('uuid_features_entity_node', array(
    'page_de_base' => 'page_de_base',
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
