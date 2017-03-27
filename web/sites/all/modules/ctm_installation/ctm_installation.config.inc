<?php

/**
 * @file
 * librairie de fonction pour l'installation des profils etat et du socle du master etat
 * commune internet intranet
 */

/**
 * dans le cas de l install :cree les roles par default
 */
function ctm_installation_config_roles($profile, &$roles, $op = 'update') {

  $admin = ctm_installation_insert_or_update_role($roles, ADMINISTRATEUR, 3);
  $admin_site = ctm_installation_insert_or_update_role($roles, ADMIN_SITE, 4);

  //Initialise le role administrateur du site.
  variable_set('user_admin_role', $admin->rid);

  //Seul l admin peut creer un compte
  variable_set('user_register', USER_REGISTER_ADMINISTRATORS_ONLY);
  variable_set('user_mail_status_activated_notify', 0);

  drupal_static_reset('ctm_installation_user_roles');
}

/**
 * config de rules
 */
function ctm_installation_config_rules($profile, $roles, $op = 'update') {
  variable_set('rules_path_cleaning_callback', "rules_path_clean_pathauto");
}

/**
 * configuration content_type_extras
 */
function ctm_installation_config_content_type_extras($profile, $roles, $op = 'update') {
  variable_set('content_type_extras_cancel_block', '1');
  variable_set('content_type_extras_default_settings', array(
      'title_label' => 'Title',
      'node_preview' => '1',
      'content_type_extras_preview_button' => 'Preview',
      'content_type_extras_save_and_new' => '1',
      'content_type_extras_save_and_new_button' => 'Save and New',
      'content_type_extras_save_and_edit' => '1',
      'content_type_extras_save_and_edit_button' => 'Save and Edit',
      'content_type_extras_cancel' => '1',
      'node_options' => array(
          'status' => 0,
          'promote' => 0,
          'sticky' => 0,
          'revision' => 0,
      ),
      'node_submitted' => 0,
      'user_permissions' => array(
          'create_roles' => array(
              $roles[ADMIN_SITE] => $roles[ADMIN_SITE],
              $roles[ADMINISTRATEUR] => $roles[ADMINISTRATEUR],
          ),
          'edit_roles' => array(
              $roles[ADMIN_SITE] => $roles[ADMIN_SITE],
              $roles[ADMINISTRATEUR] => $roles[ADMINISTRATEUR],
          ),
          'edit_own_roles' => array(),
          'delete_roles' => array(
              $roles[ADMIN_SITE] => $roles[ADMIN_SITE],
              $roles[ADMINISTRATEUR] => $roles[ADMINISTRATEUR],
          ),
          'delete_own_roles' => array(),
      ),
      'content_type_extras_descriptions_required' => 0,
      'content_type_extras_remove_body' => 1,
      'content_type_extras_title_hide' => 0,
      'content_type_extras_title_hide_css' => 0,
      'content_type_extras_title_hide_front' => 0,
      'content_type_extras_top_buttons' => array(
          'manage_fields' => 'manage_fields',
          'node_edit' => 'node_edit',
      ),
      'content_type_extras_excluded_node_forms' => array(
          '0' => '',
      ),
      'xmlsitemap_settings' => array(
          'status' => 0,
          'priority' => 0.5,
      ),
      'scheduler_settings' => array(
          'publish_enable' => 1,
          'publish_touch' => 0,
          'publish_require' => 0,
          'publish_revision' => 0,
          'unpublish_enable' => 1,
          'unpublish_require' => 0,
          'unpublish_revision' => 0,
      ),
      'content_type_extras__active_tab' => 'edit-configuration',
  ));
}

/**
 * config de contentpublish
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_publishcontent($profile, $roles, $op = 'update') {
  variable_set('publishcontent_method', PUBLISHCONTENT_METHOD_BUTTON);
  variable_set('publishcontent_publish_on_create', TRUE);
}

/**
 * Creation des dates standard francaise
 */
function ctm_installation_config_date($profile, $roles, $op = 'update') {

  //'throw_exception' => FALSE car db_insert ne gere pas INSERT IGNORE donc si le record exist deja (cas update) il envoi une erreur
  // si on veut faire un vrai update (changement d'un existant) il faudra revoir le code
  db_insert('date_format_type', array('throw_exception' => FALSE))
      ->fields(array("type,title,locked"))
      ->values(array('tech', 'Technique', '1'))
      ->values(array('day_long', 'Jour - Long', '1'))
      ->values(array('day_medium', 'Jour - Moyen', '1'))
      ->values(array('day_short', 'Jour - Court', '1'))
      ->values(array('day_tech', 'Technique - Jour', '1'))
      ->values(array('month_short', 'Mois - Court', '1'))
      ->values(array('month_tech', 'Technique - Mois', '1'))
      ->values(array('day', 'Jour', '1'))
      ->values(array('day_hour', 'Jour et Heure', '1'))
      ->values(array('month', 'Mois', '1'))
      ->values(array('month_year', 'Mois et Annee', '1'))
      ->values(array('month_hour', 'Mois et Heure', '1'))
      ->values(array('year', 'Annee', '1'))
      ->values(array('year_hour', 'Annee et Heure', '1'))
      ->execute();
  db_insert('date_formats', array('throw_exception' => FALSE))
      ->fields(array("format,type,locked"))
      ->values(array("j.n.Y", 'day_short', '1'))
      ->values(array("n.Y", 'month_short', '1'))
      ->values(array("Y-m-d", 'day_tech', '1'))
      ->values(array("Y-m-d-H:i", 'tech', '1'))
      ->values(array("Y-m", 'month_tech', '1'))
      ->values(array("Y", 'year', '1'))
      ->values(array("l j", 'day', '1'))
      ->values(array("l j \à  G\hi", 'day_hour', '1'))
      ->values(array("l j F \à  G\hi", 'month_hour', '1'))
      ->values(array('l j M Y \à  G\hi', 'year_hour', '1'))
      ->values(array("F Y", 'month_year', '1'))
      ->values(array("F", 'month', '1'))
      ->values(array("M Y", 'month_year', '1'))
      ->values(array("M", 'month', '1'))
      ->values(array("l j F Y", 'day_long', '1'))
      ->values(array("j F Y", 'day_long', '1'))
      ->values(array("D j M Y", 'day_medium', '1'))
      ->values(array("j M Y", 'day_medium', '1'))
      ->values(array("l j F Y - G:i", 'long', '1'))
      ->values(array("j F Y - G:i", 'long', '1'))
      ->values(array("D j M Y - G:i", 'medium', '1'))
      ->values(array("j M Y - G:i", 'medium', '1'))
      ->values(array("j.n.Y - G:i", 'short', '1'))
      ->execute();

  variable_set('date_format_long', 'l j F Y - G:i');
  variable_set('date_format_medium', 'D j M Y - G:i');
  variable_set('date_format_short', 'j.n.Y - G:i');
  variable_set('date_format_tech', 'Y-m-d-H:i');
  variable_set('date_format_day_long', 'l j F Y');
  variable_set('date_format_day_medium', 'D j M Y');
  variable_set('date_format_day_short', 'j.n.Y');
  variable_set('date_format_day_tech', 'Y-m-d');
  variable_set('date_format_month_short', 'n.Y');
  variable_set('date_format_month_tech', 'Y-m');
  variable_set('date_format_month', 'F');
  variable_set('date_format_month_year', 'F Y');
  variable_set('date_format_year', 'Y');
  variable_set('date_format_day_hour', 'l j \à G\hi');
  variable_set('date_format_month_hour', 'l j F \à G\hi');
  variable_set('date_format_year_hour', 'l j M Y \à G\hi');
}

/**
 * configuration de localisation update
 */
function ctm_installation_config_l10($profile, $roles, $op = 'update') {
  $path = drupal_get_path('module', 'ctm_installation');
  variable_set('l10n_update_download_store', $path . '/translations');
  variable_set('l10n_update_import_mode', 2);
  variable_set('l10n_update_check_mode', 2);
}

/**
 * configuration de features et ses dependance
 */
function ctm_installation_config_features($profile, $roles, $op = 'update') {
  variable_set('features_default_export_path', conf_path() . '/modules/features/custom');
  variable_set('features_rebuild_on_flush', 0);
}

/**
 * configuration du module admin_menu
 */
function ctm_installation_config_admin_menu($profile, $roles, $op = 'update') {
  //Active Collapse module groups on the permission page
  variable_set('admin_menu_tweak_permissions', 1);
  //Desactive des composant du menu admin
  variable_set('admin_menu_components', array('admin_menu.icon' => 1, 'admin_menu.menu' => 1, 'admin_menu.account' => 1));
}

/**
 * configuration du module admin_menu
 */
function ctm_installation_config_menu_attributes($profile, $roles, $op = 'update') {
  //Desactive les case a cocher suivantes
  variable_set('menu_attributes_id_enable', 0);
  variable_set('menu_attributes_name_enable', 0);
  variable_set('menu_attributes_rel_enable', 0);
  variable_set('menu_attributes_style_enable', 0);
  variable_set('menu_attributes_class_enable', 0);
  variable_set('menu_attributes_accesskey_enable', 0);
  variable_set('menu_attributes_title_enable', 0);
}

/**
 * configuration du module views
 */
function ctm_installation_config_views($profile, $roles, $op = 'update') {
  //Active ou desactive des case a cocher
  variable_set('views_ui_show_listing_filters', 1);
  variable_set('views_ui_show_master_display', 1);
  variable_set('views_ui_show_advanced_column', 1);
  variable_set('views_ui_show_advanced_help_warning', 0);
  variable_set('views_ui_display_embed', 1);
  variable_set('views_ui_show_sql_query', 1);
  variable_set('views_ui_show_performance_statistics', 1);
  variable_set('views_ui_always_live_preview', 0);
  variable_set('views_devel_output', 1);
  variable_set('views_devel_region', 'message');
}

/**
 * config global pathologic
 */
function ctm_installation_config_pathologic($profile, $roles, $op = 'update') {
  variable_set('pathologic_local_paths',ctm_installation_get_base_url_str());
  variable_set('pathologic_protocol_style','path');
}

/**
 * Ajout des configuration de imce selon population
 */
function ctm_installation_config_imce($profile, $roles, $op = 'update') {
  variable_set('imce_profiles', array(
      '1' => array(
          'name' => 'Admin',
          'usertab' => 0,
          'filesize' => 0,
          'quota' => 0,
          'tuquota' => 0,
          'extensions' => '*',
          'dimensions' => '0',
          'filenum' => 0,
          'directories' => array(0 => array(
                  'name' => 'imce',
                  'subnav' => 1,
                  'browse' => 1,
                  'upload' => 1,
                  'thumb' => 1,
                  'delete' => 1,
                  'resize' => 1,
                  'crop' => 1,
                  'mkdir' => 1,
                  'rmdir' => 1,
              )),
          'thumbnails' => array(),
          'mkdirnum' => 0,
      ),
      '2' => array(
          'name' => 'User',
          'usertab' => 0,
          'filesize' => 0,
          'quota' => 0,
          'tuquota' => 0,
          'extensions' => '*',
          'dimensions' => '0',
          'filenum' => 0,
          'directories' => array(0 => array(
                  'name' => 'imce',
                  'subnav' => 1,
                  'browse' => 1,
                  'upload' => 1,
                  'thumb' => 1,
                  'delete' => 1,
                  'resize' => 1,
                  'crop' => 1,
                  'mkdir' => 1,
                  'rmdir' => 1,
              )),
          'thumbnails' => array(),
          'mkdirnum' => 0,
      ),
  ));
  variable_set('imce_roles_profiles', array(DRUPAL_AUTHENTICATED_RID => array(
          'weight' => 11,
          'public_pid' => 2,
          'private_pid' => 2,
  )));
}

/**
 * Ajout des configuration du profil linkit
 */
function ctm_installation_config_linkit($profile, $roles, $op = 'update') {
  ctools_include('export');
  $linkit_profile = _ctm_installation_get_linkit_profil();
  $old = ctools_export_crud_load('linkit_profiles', $linkit_profile->name);
  if (!empty($old)) {
    $linkit_profile->pid = $old->pid;
    $linkit_profile->export_type = $old->export_type;
  }
  ctools_export_crud_save('linkit_profiles', $linkit_profile);
  $linkit_profile2 = _ctm_installation_get_linkit_profil();
  $linkit_profile2->name = 'champs';
  $linkit_profile2->admin_title = 'Champs';
  $linkit_profile2->profile_type = '2';
  unset($linkit_profile2->data['text_formats']);
  $linkit_profile2->data['insert_plugin']['plugin'] = 'raw_url';
  $linkit_profile2->data['imce'] = 0;
  $old2 = ctools_export_crud_load('linkit_profiles', $linkit_profile2->name);
  if (!empty($old2)) {
    $linkit_profile2->pid = $old2->pid;
    $linkit_profile2->export_type = $old2->export_type;
  }
  ctools_export_crud_save('linkit_profiles', $linkit_profile2);
}


/**
 * recupere un profil linkit de base editeur
 */
function _ctm_installation_get_linkit_profil() {
  $largeurs = array_keys(variable_get('ckeditor_sizes', array('100' => 'p;h2;h3;h4;h5;h6;address;div')));
  $format_name = array(
      'html' => 0,
      'mail' => 0,
      'php_code' => 0,
      'plain_text' => 0,
  );
  foreach ($largeurs as $largeur => $format_texte) {
    $format_name['ckeditor_' . $largeur] = 'ckeditor_' . $largeur;
    $format_name['ckeditor_liens_' . $largeur] = 'ckeditor_liens_' . $largeur;
    $format_name['ckeditor_images_' . $largeur] = 'ckeditor_images_' . $largeur;
    $format_name['ckeditor_images_liens_' . $largeur] = 'ckeditor_images_liens_' . $largeur;
  }
  $linkit_profile = new LinkitProfile();
  $linkit_profile->disabled = FALSE; /* Edit this to true to make a default linkit_profile disabled initially */
  $linkit_profile->api_version = 1;
  $linkit_profile->name = 'editeur';
  $linkit_profile->admin_title = 'Editeur';
  $linkit_profile->admin_description = '';
  $linkit_profile->profile_type = '1';
  $linkit_profile->data = array(
      'text_formats' => $format_name,
      'search_plugins' => array(
          'entity:user' => array(
              'enabled' => 0,
              'weight' => '-10',
          ),
          'entity:file' => array(
              'enabled' => 1,
              'weight' => '-10',
          ),
          'entity:node' => array(
              'enabled' => 1,
              'weight' => '-10',
          ),
          'entity:taxonomy_term' => array(
              'enabled' => 1,
              'weight' => '-10',
          ),
          'entity:menu_link' => array(
              'enabled' => 1,
              'weight' => '-10',
          ),
      ),
      'entity:node' => array(
          'result_description' => '',
          'bundles' => array(),
          'group_by_bundle' => 1,
          'include_unpublished' => 1,
      ),
      'entity:file' => array(
          'result_description' => '',
          'show_scheme' => 0,
          'group_by_scheme' => 0,
          'image_extra_info' => array(
              'thumbnail' => 'thumbnail',
              'dimensions' => 'dimensions',
          ),
      ),
      'entity:taxonomy_term' => array(
          'result_description' => '',
      ),
      'entity:user' => array(
          'result_description' => '',
      ),
      'entity:menu_link' => array(
          'result_description' => '',
          'bundles' => array(
              'main-menu' => 'main-menu',
              'management' => 0,
              'navigation' => 0,
              'user-menu' => 0,
          ),
          'group_by_bundle' => 1,
      ),
      'insert_plugin' => array(
          'url_method' => '1',
      ),
      'attribute_plugins' => array(
          'class' => array(
              'enabled' => 0,
              'weight' => '-10',
          ),
          'target' => array(
              'enabled' => 1,
              'weight' => '-10',
          ),
          'accesskey' => array(
              'enabled' => 0,
              'weight' => '-10',
          ),
          'id' => array(
              'enabled' => 0,
              'weight' => '-10',
          ),
          'rel' => array(
              'enabled' => 0,
              'weight' => '-10',
          ),
          'title' => array(
              'enabled' => 0,
              'weight' => '-10',
          ),
      ),
      'imce' => 1,
      'autocomplete' => array(
          'charLimit' => '3',
          'wait' => '350',
          'remoteTimeout' => '10000',
      ),
  );
  return $linkit_profile;
}

/**
 * configuration du module pathauto
 */
function ctm_installation_config_pathauto($profile, $roles, $op = 'update') {

  //Creation des variables pour la configuration du module pathauto
  variable_set('pathauto_case', '1');
  variable_set('pathauto_node_block_pattern', '[node:content-type]/[node:title]');
  variable_set('pathauto_forum_pattern', '[term:vocabulary]/[term:name]');
  variable_set('pathauto_node_pattern', '[node:menu-link:parents:join-path]/[node:title]');
  variable_set('pathauto_ignore_words', 'a, an, the, and, or, this, that, by, before, after, for, in, with, d, ll, un, une, le, la, les, des, à, a, et, du, ses, ces, par, avant, après, pour, dans, que, avec, l, d, y, t,' . PATHAUTO_IGNORE_WORDS);
  variable_set('pathauto_max_component_length', '100');
  variable_set('pathauto_max_length', '100');
  variable_set('pathauto_punctuation_ampersand', '1');
  variable_set('pathauto_punctuation_asterisk', '0');
  variable_set('pathauto_punctuation_at', '0');
  variable_set('pathauto_punctuation_backtick', '0');
  variable_set('pathauto_punctuation_back_slash', '1');
  variable_set('pathauto_punctuation_caret', '0');
  variable_set('pathauto_punctuation_colon', '0');
  variable_set('pathauto_punctuation_comma', '0');
  variable_set('pathauto_punctuation_dollar', '0');
  variable_set('pathauto_punctuation_double_quotes', '0');
  variable_set('pathauto_punctuation_equal', '0');
  variable_set('pathauto_punctuation_exclamation', '0');
  variable_set('pathauto_punctuation_greater_than', '0');
  variable_set('pathauto_punctuation_hash', '0');
  variable_set('pathauto_punctuation_hyphen', '2');
  variable_set('pathauto_punctuation_left_curly', '0');
  variable_set('pathauto_punctuation_left_parenthesis', '1');
  variable_set('pathauto_punctuation_left_square', '0');
  variable_set('pathauto_punctuation_less_than', '0');
  variable_set('pathauto_punctuation_percent', '0');
  variable_set('pathauto_punctuation_period', '0');
  variable_set('pathauto_punctuation_pipe', '0');
  variable_set('pathauto_punctuation_plus', '0');
  variable_set('pathauto_punctuation_question_mark', '0');
  variable_set('pathauto_punctuation_quotes', '0');
  variable_set('pathauto_punctuation_right_curly', '0');
  variable_set('pathauto_punctuation_right_parenthesis', '1');
  variable_set('pathauto_punctuation_right_square', '0');
  variable_set('pathauto_punctuation_semicolon', '0');
  variable_set('pathauto_punctuation_slash', '1');
  variable_set('pathauto_punctuation_tilde', '0');
  variable_set('pathauto_punctuation_underscore', '2');
  variable_set('pathauto_reduce_ascii', 0);
  variable_set('pathauto_separator', '-');
  variable_set('pathauto_transliterate', 1);
  variable_set('pathauto_update_action', '1');
  variable_set('pathauto_user_pattern', 'users/[user:name]');
  variable_set('pathauto_verbose', 0);
}

/**
 * Configuration de menu position
 */
function ctm_installation_config_menu_position($profile, $roles, $op = 'update') {
  variable_set('menu_position_active_link_display', 'parent');
}

/**
 * Configuration du block masquerade
 */
function ctm_installation_config_masquerade($profile, $roles, $op = 'update') {
  $default_theme = variable_get('theme_default', '');
  $values = array(
      array(
          'module' => 'masquerade',
          'delta' => 'masquerade',
          'theme' => $default_theme,
          'status' => 1,
          'weight' => 0,
          'region' => 'footer',
          'pages' => '',
          'cache' => DRUPAL_CACHE_PER_USER,
      ),
      array(
          'module' => 'ctm_masquerade',
          'delta' => 'ctm_masquerade',
          'theme' => $default_theme,
          'status' => 1,
          'weight' => 0,
          'region' => 'footer',
          'pages' => '',
          'cache' => DRUPAL_CACHE_PER_USER,
      ),
  );

  ctm_installation_insert_or_update_blocks($values);
}

/**
 * Active les blocs standard
 */
function ctm_installation_config_blocks($profile, $roles, $op = 'update') {

  $default_theme = variable_get('theme_default');
  $admin_theme = variable_get('admin_theme', '');

  $values = array(
      array(
          'module' => 'system',
          'delta' => 'main',
          'theme' => $default_theme,
          'status' => 1,
          'weight' => 0,
          'region' => 'content',
          'pages' => '',
          'cache' => DRUPAL_NO_CACHE,
      ),
      array(
          'module' => 'system',
          'delta' => 'main',
          'theme' => $admin_theme,
          'status' => 1,
          'weight' => 0,
          'region' => 'content',
          'pages' => '',
          'cache' => DRUPAL_NO_CACHE,
      ),
      array(
          'module' => 'system',
          'delta' => 'main-menu',
          'theme' => $default_theme,
          'status' => 1,
          'region' => 'menu',
          'weight' => -10,
          'pages' => '',
          'cache' => DRUPAL_CACHE_PER_PAGE,
      ),
      array(
          'module' => 'system',
          'delta' => 'help',
          'theme' => $default_theme,
          'status' => 1,
          'weight' => -10,
          'region' => 'help',
          'pages' => '',
          'cache' => DRUPAL_CACHE_GLOBAL,
      ),
      array(
          'module' => 'system',
          'delta' => 'help',
          'theme' => $admin_theme,
          'status' => 1,
          'weight' => -10,
          'region' => 'help',
          'pages' => 'admin/structure/menu', // on affiche pas l aide sur cette page car elle a un lien qu on ne veut pas
          'cache' => DRUPAL_CACHE_GLOBAL,
      ),
  );
  ctm_installation_insert_or_update_blocks($values);
}

/**
 * configuration de la vue node de admin_views
 */
function ctm_installation_config_admin_views($profile, $roles, $op = 'update') {

  $view = views_get_view('admin_views_node', TRUE);

  // Display: Defaults : on insert la colonne modificateur (name_1) a la bonne place
  $handler = $view->display['default'];
  $handler->display_options['style_options']['columns'] = array(
      'views_bulk_operations' => 'views_bulk_operations',
      'title' => 'title',
      'timestamp' => 'title',
      'type' => 'type',
      'name' => 'name',
      'name_1' => 'name_1',
      'status' => 'status',
      'unpublish_on' => 'unpublish_on',
      'publish_on' => 'publish_on',
      'changed' => 'changed',
      'nid' => 'nid',
      'nid_1' => 'nid',
  );
  $old_values = $handler->display_options['style_options']['info'];
  $handler->display_options['style_options']['info'] = array();
  foreach ($old_values as $key => $value) {
    if ($key == 'edit_node' || $key == 'delete_node')
      continue; // on supprime les anciens liens d'actions
    $handler->display_options['style_options']['info'][$key] = $value;
    if ($key == 'name') { // ajout apres la colonne auteur
      $handler->display_options['style_options']['info']['name_1'] = array(
          'sortable' => 1,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 0,
      );
    }
    elseif ($key == 'status') { // ajout apres la colonne publie
      $handler->display_options['style_options']['info']['unpublish_on'] = array(
          'sortable' => 1,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 1,
      );
      $handler->display_options['style_options']['info']['publish_on'] = array(
          'sortable' => 1,
          'default_sort_order' => 'asc',
          'align' => '',
          'separator' => '',
          'empty_column' => 1,
      );
    }
  }
  // on ajoute les nouveaux lien d'action
  $handler->display_options['style_options']['info']['nid'] = array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '&emsp;',
      'empty_column' => 0,
  );
  $handler->display_options['style_options']['info']['nid_1'] = array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
  );

  // Relation: Revision de contenu : Utilisateur
  $handler->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display_options['relationships']['uid_1']['table'] = 'node_revision';
  $handler->display_options['relationships']['uid_1']['field'] = 'uid';

  // Champ: Operations en masse : Contenu
  $selected_skip = array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
  );
  $selected = array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
  );
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_assign_owner_action']['selected'] = 0;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_promote_action']['selected'] = 0;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_unpromote_action']['selected'] = 0;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::views_bulk_operations_modify_action'] = array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 1,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
          'show_all_tokens' => 1,
          'display_values' => array(
              '_all_' => '_all_',
          ),
      ),
  );
  //$handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::replicate_ui_replicate_item'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::views_bulk_operations_delete_item'] = $selected;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_unpublish_action'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_publish_action'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_make_unsticky_action'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_make_sticky_action'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_promote_action'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_unpromote_action'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['action::node_save_action'] = $selected;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['rules_component::scheduler_set_unpublish_date_component'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['rules_component::scheduler_set_publish_date_component'] = $selected_skip;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['rules_component::scheduler_remove_unpublish_date_component'] = $selected;
  $handler->display_options['fields']['views_bulk_operations']['vbo_operations']['rules_component::scheduler_remove_publish_date_component'] = $selected;

  // Champ: Utilisateur : Name_1 a mettre dans le bon ordre
  $old_values = $handler->display_options['fields'];
  $handler->display_options['fields'] = array();
  foreach ($old_values as $key => $value) {
    if ($key == 'edit_node' || $key == 'delete_node')
      continue; // on les supprime
    $handler->display_options['fields'][$key] = $value;
    if ($key == 'name') {
      $handler->display_options['fields']['name_1']['id'] = 'name_1';
      $handler->display_options['fields']['name_1']['table'] = 'users';
      $handler->display_options['fields']['name_1']['field'] = 'name';
      $handler->display_options['fields']['name_1']['relationship'] = 'uid_1';
      $handler->display_options['fields']['name_1']['label'] = 'Modifier';
    }
    elseif ($key == 'status') { // ajout apres la colonne publie
      /* Champ: Scheduler : Depublier le */
      $handler->display_options['fields']['unpublish_on']['id'] = 'unpublish_on';
      $handler->display_options['fields']['unpublish_on']['table'] = 'scheduler';
      $handler->display_options['fields']['unpublish_on']['field'] = 'unpublish_on';
      $handler->display_options['fields']['unpublish_on']['hide_empty'] = TRUE;
      $handler->display_options['fields']['unpublish_on']['date_format'] = 'short';
      $handler->display_options['fields']['unpublish_on']['second_date_format'] = 'long';
      /* Champ: Scheduler : Publier le */
      $handler->display_options['fields']['publish_on']['id'] = 'publish_on';
      $handler->display_options['fields']['publish_on']['table'] = 'scheduler';
      $handler->display_options['fields']['publish_on']['field'] = 'publish_on';
      $handler->display_options['fields']['publish_on']['hide_empty'] = TRUE;
      $handler->display_options['fields']['publish_on']['date_format'] = 'short';
      $handler->display_options['fields']['publish_on']['second_date_format'] = 'long';
    }
  }
  /* Champ: Lien de modification */
  $handler->display_options['fields']['nid']['id'] = 'nid';
  $handler->display_options['fields']['nid']['table'] = 'node';
  $handler->display_options['fields']['nid']['field'] = 'nid';
  $handler->display_options['fields']['nid']['ui_name'] = 'Lien de modification';
  $handler->display_options['fields']['nid']['label'] = 'Operations';
  $handler->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display_options['fields']['nid']['alter']['text'] = 'Modify';
  $handler->display_options['fields']['nid']['alter']['make_link'] = TRUE;
  $handler->display_options['fields']['nid']['alter']['path'] = 'node/[nid]/edit?destination=admin/content';
  $handler->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Champ: Lien de suppression */
  $handler->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display_options['fields']['nid_1']['ui_name'] = 'Lien de suppression';
  $handler->display_options['fields']['nid_1']['label'] = '';
  $handler->display_options['fields']['nid_1']['alter']['alter_text'] = TRUE;
  $handler->display_options['fields']['nid_1']['alter']['text'] = 'Delete';
  $handler->display_options['fields']['nid_1']['alter']['make_link'] = TRUE;
  $handler->display_options['fields']['nid_1']['alter']['path'] = 'node/[nid_1]/delete?destination=admin/content';
  $handler->display_options['fields']['nid_1']['element_label_colon'] = FALSE;

  // Suppression filtre vocabulaire
  unset($handler->display_options['filters']['vid']);

  // Critere de filtrage: Contenu : Promu en page d'accueil
  $handler->display_options['filters']['promote']['id'] = 'promote';
  $handler->display_options['filters']['promote']['table'] = 'node';
  $handler->display_options['filters']['promote']['field'] = 'promote';
  $handler->display_options['filters']['promote']['value'] = 'All';
  $handler->display_options['filters']['promote']['exposed'] = TRUE;
  $handler->display_options['filters']['promote']['expose']['operator_id'] = '';
  $handler->display_options['filters']['promote']['expose']['label'] = 'Promote in frontpage';
  $handler->display_options['filters']['promote']['expose']['operator'] = 'promote_op';
  $handler->display_options['filters']['promote']['expose']['identifier'] = 'promote';
  $handler->display_options['filters']['promote']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      8 => 0,
      6 => 0,
      5 => 0,
      4 => 0,
      3 => 0,
      7 => 0,
  );

  // Suppression de la relation sur le vocabulaire.
  unset($handler->display_options['relationships']['term_node_tid']);

  $view->save();
}

/**
 * configuration ultimate cron
 */
function ctm_installation_config_cron_jobs($profile, $roles, $op = 'update') {
  variable_set('ultimate_cron_plugin_settings_general_settings', array('nodejs' => 0));
  variable_set('ultimate_cron_plugin_scheduler_default', 'crontab');
  variable_set('ultimate_cron_plugin_settings_poorman_settings', array(
      'launcher' => '',
      'early_page_flush' => 1,
      'user_agent' => 'Ultimate Cron',
  ));
}

/**
 * config menu block
 */
function ctm_installation_config_menu_block($profile, $roles, $op = 'update') {
  $menu_order = array('main-menu' => '');
  variable_set('menu_block_menu_order', $menu_order);
}

/**
 * config db_maintenance
 */
function ctm_installation_config_db_maintenance($profile, $roles, $op = 'update') {
  variable_set('db_maintenance_all_tables', 1);
  variable_set('db_maintenance_cron_frequency', '86400');
}

/**
 * configuration du module redirect
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_redirect($profile, $roles, $op = 'update') {
  variable_set('redirect_auto_redirect', 1);
  variable_set('redirect_default_status_code', "301");
  variable_set('redirect_global_admin_paths', 0);
  variable_set('redirect_global_canonical', 1);
  variable_set('redirect_global_clean', 1);
  variable_set('redirect_global_deslash', 0);
  variable_set('redirect_global_home', 1);
  variable_set('redirect_page_cache', 1);
  variable_set('redirect_passthrough_querystring', 1);
  variable_set('redirect_purge_inactive', "0");
  variable_set('redirect_warning', FALSE);
}

/**
 * activate menuperformance
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_menuperformance($profile, $roles, $op = 'update') {
  variable_set('menu_override_parent_selector',TRUE);
}