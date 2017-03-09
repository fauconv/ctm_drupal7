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
 * we have to reset the prefix cause the value was updated by other install process
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_multi_language_prefix_after($profile, $roles, $op = 'update') {
  db_update('languages')
      ->fields(array('prefix' => 'en'))
      ->condition('language', 'en')
      ->execute();
}

/**
 * configuration de la vue node de admin_views
 * ajout du lien "translate"
 */
function ctm_installation_config_multi_lang_admin_views_after($profile, $roles, $op = 'update') {
  $view = views_get_view('admin_views_node', TRUE);
  $handler = $view->display['default'];
  $handler->display_options['style_options']['columns']['nid_2'] = 'nid';
  $handler->display_options['style_options']['info']['nid_2'] = array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
  );
  $handler->display_options['fields']['nid_2']['id'] = 'nid_2';
  $handler->display_options['fields']['nid_2']['table'] = 'node';
  $handler->display_options['fields']['nid_2']['field'] = 'nid';
  $handler->display_options['fields']['nid_2']['ui_name'] = 'Translation link';
  $handler->display_options['fields']['nid_2']['label'] = '';
  $handler->display_options['fields']['nid_2']['alter']['alter_text'] = TRUE;
  $handler->display_options['fields']['nid_2']['alter']['text'] = 'Translate';
  $handler->display_options['fields']['nid_2']['alter']['make_link'] = TRUE;
  $handler->display_options['fields']['nid_2']['alter']['path'] = 'node/[nid_2]/translate?destination=admin/content';
  $handler->display_options['fields']['nid_2']['element_label_colon'] = FALSE;

  $view->save();
}

/**
 * modification du main-menu and activation block langue backoffice
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_multi_language_menu_after($profile, $roles, $op = 'update') {
  $menu = menu_load('main-menu');
  $menu['i18n_mode'] = 5;
  $menu['language'] = LANGUAGE_NONE;
  menu_save($menu);
}

/**
 * active les features specifique du site
 */
function ctm_installation_config_multi_language_field_after($profile, $roles, $op = 'update') {

  variable_set('entity_translation_entity_types', array(
      'node' => 'node',
      'taxonomy_term' => 'taxonomy_term',
      'user' => 0,
  ));
  variable_set('entity_translation_revision_enabled', TRUE);
  variable_set('entity_translation_shared_labels', 1);
  variable_set('entity_translation_workflow_enabled', 0);
  variable_set('i18n_string_translate_langcode_en',TRUE);

  variable_set('i18n_language_list', '1');
  $list = filter_formats();
  foreach ($list as $key => $value) {
    $list[$key] = $key;
  }
  variable_set('i18n_string_allowed_formats', $list);
  variable_set('i18n_string_source_language', 'en');

  $negociation = array(
      'locale-url' => array(
          'callbacks' => array(
              'language' => 'locale_language_from_url',
              'switcher' => 'locale_language_switcher_url',
              'url_rewrite' => 'locale_language_url_rewrite_url',
          ),
          'file' => 'includes/locale.inc',
      ),
      'locale-browser' => array(
          'callbacks' => array(
              'language' => 'locale_language_from_browser',
          ),
          'file' => 'includes/locale.inc',
          'cache' => 0,
      ),
      'language-default' => array(
          'callbacks' => array(
              'language' => 'language_from_default',
          ),
      ),
  );
  variable_set('language_negotiation_language', $negociation);
  variable_set('language_negotiation_language_content', $negociation);
  variable_set('language_negotiation_language_url', array(
      'locale-url' => array(
          'callbacks' => array(
              'language' => 'locale_language_from_url',
              'switcher' => 'locale_language_switcher_url',
              'url_rewrite' => 'locale_language_url_rewrite_url',
          ),
          'file' => 'includes/locale.inc',
      ),
      'locale-url-fallback' => array(
          'callbacks' => array(
              'language' => 'locale_language_url_fallback',
          ),
          'file' => 'includes/locale.inc',
      )
  ));
  variable_set('language_types', array(
      'language' => 1,
      'language_content' => 1,
      'language_url' => '',
  ));
  variable_set('locale_field_language_fallback', 1);
  variable_set('locale_language_providers_weight_language', array(
      'locale-url' => '-8',
      'locale-session' => '-6',
      'locale-user' => '-4',
      'locale-browser' => '-2',
      'language-default' => '10',
  ));
  variable_set('locale_language_providers_weight_language_content', array(
      'locale-url' => '-8',
      'locale-session' => '-6',
      'locale-user' => '-4',
      'locale-browser' => '-2',
      'locale-interface' => '8',
      'language-default' => '10',
  ));
  variable_set('translation_language_type', 'language_content');
  variable_set('entity_translation_settings_taxonomy_term__taxonomy_term', array(
      'default_language' => 'xx-et-default',
      'hide_language_selector' => 0,
      'exclude_language_none' => 0,
      'lock_language' => 1,
      'shared_fields_original_only' => 0,
  ));
}

/**
 * configure title module
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_multi_language_field_title_after($profile, $roles, $op = 'update') {
  variable_set('title_node', array(
      'auto_attach' => array('title' => 0),
      'hide_label' => array(
          'entity' => 'entity',
          'page' => 0,
  )));
}

/**
 * mise en place du champs traduisible pour page_de_base
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_multi_language_field_page_base_after($profile, $roles, $op = 'update') {
  ctm_installation_set_multi_language_ct('page_de_base', true);
  ctm_installation_set_multi_language_field('texte_page_base');
}

function ctm_installation_config_multi_language_field_taxonomy_after($profile, $roles, $op = 'update') {
  $field = array(
    'field_name' => 'name_field',
    'type'  => 'text',
    'translatable' => 1,
  );
  ctm_installation_insert_or_update_field($field);
}

/**
 * mise en place du champs traduisible pour page_de_base
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_multi_language_field_block_after($profile, $roles, $op = 'update') {
  if(module_exists('ctm_node_block')) {
    ctm_installation_set_multi_language_ct('block');
    ctm_installation_set_multi_language_field('texte_block');
  }
}

/**
 * active les features specifique du site
 */
function ctm_installation_config_multi_language_perms_after($profile, $roles, $op = 'update') {
  $permission_list = array(
      'translate any entity',
      'translate node entities',
      'translate taxonomy_term entities',
  );
  user_role_grant_permissions($roles[CONTRIBUTEUR], $permission_list);
  $permission_list = array_merge($permission_list, array(
    'translate user-defined strings',
  ));
  user_role_grant_permissions($roles[ADMIN_SITE], $permission_list);
  user_role_grant_permissions($roles[ADMINISTRATEUR], $permission_list);
}

