<?php

/**
 * @file
 * fonctions execute avant le formulaire d'installation et apres les before
 * format des fonction ici :
 * ctm_installation_config_XXXXXX($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 * ATTENTION il s'agit bien de fonction CTM_INSTALLATION
 */


/**
 * dans le cas de l install :cree les roles par default et initialise les permission
 * dans le cas de la reeinit : reactive les permissions par defaut mais n'enleve pas les permission existante
 */
function ctm_installation_config_core_roles($profile, &$roles, $op = 'update') {
  $contrib = ctm_installation_insert_or_update_role($roles, CONTRIBUTEUR, 5);  
  drupal_static_reset('ctm_installation_user_roles');
}

/**
 * Set le format date dans le module scheduler "systeme" pour etre compatible
 * avec toutes les langues
 * @param type $profile (Pas utilise)
 * @param type $roles   (Pas utilise)
 */
function ctm_installation_config_core_scheduler($profile, $roles, $op = 'update') {
  variable_set('scheduler_date_format', 'Y/m/d H:i');
}


/**
 * configuration des profils de ckeditor
 */
function ctm_installation_config_core_ckeditor($profile, $roles, $op = 'update') {
  $path = drupal_get_path('module', 'ctm_installation');
  $toolbar = "
    ['Cut','Copy','Paste','PasteText','PasteFromWord'],
    ['BulletedList','NumberedList','-','Table','HorizontalRule','SpecialChar','Abbr','Templates'],
    ['Undo','Redo','Find','Replace','SpellChecker','Scayt'],
    ['ShowBlocks','SelectAll','RemoveFormat','Source']
    ";
  $toolbar_plus = "
    [['Bold','Italic','Underline','Subscript','Superscript','Strike','-','JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock','-','Outdent','Indent'],
    ['Format','Styles'],
    " . $toolbar;
  $toolbar = "[".$toolbar."]";
  $toolbar_images = $toolbar_plus . ",['EnhancedImage','Flash','MediaEmbed','ImageMaps']]";
  $toolbar_liens = $toolbar_plus . ",['Link','linkit','Unlink','Anchor']]";
  $toolbar_images_liens = $toolbar_plus . ",['Link','linkit','Unlink','Anchor'],['EnhancedImage','Flash','MediaEmbed','ImageMaps']]";
  $toolbar_plus .= "]";
  $linkit_config = array(
      'name' => 'linkit',
      'desc' => 'Prise en charge du module Linkit',
      'path' => '%base_path%' . drupal_get_path('module', 'linkit') . '/editors/ckeditor/',
      'buttons' => array(
          'linkit' => array(
              'label' => 'Linkit',
              'icon' => 'linkit.png',
          ),
      ),
  );
  $mediaembed_config = array(
      'name' => 'mediaembed',
      'desc' => 'Plugin for inserting Drupal embeded media',
      'path' => '%plugin_dir%mediaembed/',
      'buttons' => array(
          'MediaEmbed' => array(
              'label' => 'MediaEmbed',
              'icon' => 'images/icon.png',
          ),
      ),
  );
  
  $largeurs = array('100' => 'p;h2;h3;h4;h5;h6;address;div'); // largeur du bloc ckeditor
  variable_set('ckeditor_sizes', $largeurs);
  $color = '#64A0B4'; // couleur de la toolbar de ckeditor
  //Supprime les profil advanced et full et la config global
  $condi = db_or()->condition('name', 'Advanced')
      ->condition('name', 'Full')
      ->condition('name', 'CKEditor Global Profile');

  db_delete('ckeditor_settings')->condition($condi)
      ->execute();
  db_delete('ckeditor_input_format')->condition($condi)
      ->execute();

  //-------------------------------------------------------------- config general -----------------------------------------------
  $data = array(
      'name' => 'CKEditor Global Profile',
      'settings' => array(
          'skin' => 'moonocolor',
          'ckeditor_path' => '%l/ckeditor',
          'ckeditor_local_path' => '',
          'ckeditor_plugins_path' => '%m/plugins',
          'ckeditor_plugins_local_path' => '',
          'ckfinder_path' => '%m/ckfinder',
          'ckfinder_local_path' => '',
          'ckeditor_aggregate' => 'f',
          'toolbar_wizard' => 't',
          'loadPlugins' => array(),
      ),
      'input_formats' => array(),
  );
  ctm_installation_create_ckeditor_profil($data);

  //----------------------------------------------------------------largeur normal------------------------------------------------------
  //ckeditor_bloc

  $data = array(
      'settings' => array(
          'ss' => 2,
          'toolbar' => $toolbar,
          'expand' => 't',
          'default' => 't',
          'show_toggle' => 'f',
          'uicolor' => 'custom',
          'uicolor_user' => $color,
          'lang' => 'fr',
          'auto_lang' => 'f',
          'language_direction' => 'ltr',
          'allowed_content' => 't',
          'extraAllowedContent' => '',
          'enter_mode' => 'p',
          'shift_enter_mode' => 'br',
          'custom_formatting' => 't',
          'formatting' => array(
              'custom_formatting_options' => array(
                  'indent' => 'indent',
                  'breakBeforeOpen' => 'breakBeforeOpen',
                  'breakAfterOpen' => 'breakAfterOpen',
                  'breakBeforeClose' => 'breakBeforeClose',
                  'breakAfterClose' => 'breakAfterClose',
                  'pre_indent' => 0,
              ),
          ),
          'css_mode' => 'none',
          'css_path' => '',
          'css_style' => 'default',
          'styles_path' => '',
          'filebrowser' => 'imce',
          'filebrowser_image' => 'imce',
          'filebrowser_flash' => 'imce',
          'UserFilesPath' => '%b%f/',
          'UserFilesAbsolutePath' => '%d%b%f/',
          'forcePasteAsPlainText' => 'f',
          'html_entities' => 'f',
          'scayt_autoStartup' => 't',
          'theme_config_js' => 'f',
          'js_conf' => '',
          'loadPlugins' => array_merge(ctm_installation_ckeditor_plugin(TRUE), array(
              'counter' => array(
                  'name' => 'counter',
                  'desc' => 'Plugin to count symbols, symbols without blanks and words',
                  'path' => '%plugin_dir%counter/',
                  'buttons' => FALSE,
                  'default' => 'f',
              ),
              )
          ),
      ),
      'input_formats' => array(
      ),
  );

  // minimal
  foreach ($largeurs as $largeur => $format_texte) {
    $data['name'] = 'ckeditor_minimal_' . $largeur;
    $data['settings']['width'] = $largeur;
    $data['settings']['font_format'] = $format_texte;
    $data['input_formats'] = array($data['name'] => 'Editeur - Minimal - ' . $largeur);
    ctm_installation_create_ckeditor_profil($data);
  }

  //normal
  foreach ($largeurs as $largeur => $format_texte) {
    $data['settings']['toolbar'] = $toolbar_plus;
    $data['name'] = 'ckeditor_' . $largeur;
    $data['settings']['width'] = $largeur;
    $data['settings']['font_format'] = $format_texte;
    $data['input_formats'] = array($data['name'] => 'Editeur - ' . $largeur);
    ctm_installation_create_ckeditor_profil($data);
  }

  //ckeditor_images
  $data['settings']['toolbar'] = $toolbar_images;
  $data['settings']['loadPlugins']['mediaembed'] = $mediaembed_config;
  $data['input_formats'] = array($data['name'] => 'Ckeditor - images - block');
  foreach ($largeurs as $largeur => $format_texte) {
    $data['name'] = 'ckeditor_images_' . $largeur;
    $data['settings']['width'] = $largeur;
    $data['settings']['font_format'] = $format_texte;
    $data['input_formats'] = array($data['name'] => 'Editeur - images - ' . $largeur);
    ctm_installation_create_ckeditor_profil($data);
  }

  //ckeditor_liens_et_images
  $data['settings']['toolbar'] = $toolbar_images_liens;
  $data['settings']['loadPlugins']['linkit'] = $linkit_config;
  foreach ($largeurs as $largeur => $format_texte) {
    $data['name'] = 'ckeditor_images_liens_' . $largeur;
    $data['settings']['width'] = $largeur;
    $data['settings']['font_format'] = $format_texte;
    $data['input_formats'] = array($data['name'] => 'Editeur - images - liens - ' . $largeur);
    ctm_installation_create_ckeditor_profil($data);
  }

  //ckeditor_liens
  $data['settings']['toolbar'] = $toolbar_liens;
  unset($data['settings']['loadPlugins']['mediaembed']);
  foreach ($largeurs as $largeur => $format_texte) {
    $data['name'] = 'ckeditor_liens_' . $largeur;
    $data['settings']['width'] = $largeur;
    $data['settings']['font_format'] = $format_texte;
    $data['input_formats'] = array($data['name'] => 'Editeur - liens - ' . $largeur);
    ctm_installation_create_ckeditor_profil($data);
  }
}

/**
 * configuration des formats de texte et des profils ctm_internet
 */
function ctm_installation_config_core_format_texte($profile, $roles, $op = 'update') {
  $largeurs = variable_get('ckeditor_sizes', array('100' => 'p;h2;h3;h4;h5;h6;address;div'));

  $pathologic = array(
      'weight' => 50,
      'status' => 1,
      'settings' => array(
          'settings_source' => 'global',
      ),
  );

  $filter_htmlcorrector = array(
      'weight' => 10,
      'status' => 1,
      'settings' => array(),
  );

  $contenteditable = array(
      'weight' => 9,
      'status' => 1,
      'settings' => array(),
  );

  $image_resize_filter = array(
      'weight' => 0,
      'status' => 1,
      'settings' => array(
          'link' => 1,
          'link_class' => 'fancybox',
          'link_rel' => '',
          'image_locations' => array(
              'local' => 'local',
              'remote' => 0,
          ),
      ),
  );

  //Formats
  $format = new stdClass();
  $format->cache = 1;
  $format->status = 1;

  // Exported format: Mail.
  $format->format = 'mail';
  $format->name = 'Mail';
  $format->weight = 3;
  $format->filters = array('filter_htmlcorrector' => $filter_htmlcorrector);
  filter_format_save($format);

  /////////////////////////////////////////////3 ///////////////////////////////////////////////////
  //
  $format->filters = array(
      'ctm_contenteditable' => $contenteditable,
      'filter_htmlcorrector' => $filter_htmlcorrector,
      'pathologic' => $pathologic,
      'image_resize_filter' => $image_resize_filter,
  );

  $format_name = array();
  $name = array();
  foreach ($largeurs as $largeur => $format_texte) {
    $format_name[] = 'ckeditor_images_' . $largeur;
    $format_name[] = 'ckeditor_images_liens_' . $largeur;
    $name[] = 'Editeur - images - ' . $largeur;
    $name[] = 'Editeur - images - liens - ' . $largeur;
  }

  foreach ($format_name as $key => $fn) {
    $format->format = $fn;
    $format->name = $name[$key];
    $format->weight = $key - 10;
    filter_format_save($format);
  }


  /////////////////////////////////////////// 2 /////////////////////////////////////////////////////

  $format->filters = array(
      'ctm_contenteditable' => $contenteditable,
      'filter_htmlcorrector' => $filter_htmlcorrector,
      'pathologic' => $pathologic,
  );

  $format_name_deux = array(
      'html',
  );

  $name_deux = array(
      'HTML',
  );
  foreach ($largeurs as $largeur => $format_texte) {
    $format_name_deux[] = 'ckeditor_minimal_' . $largeur;
    $format_name_deux[] = 'ckeditor_' . $largeur;
    $format_name_deux[] = 'ckeditor_liens_' . $largeur;
    $name_deux[] = 'Editeur - Minimal - ' . $largeur;
    $name_deux[] = 'Editeur - ' . $largeur;
    $name_deux[] = 'Editeur - liens - ' . $largeur;
  }

  foreach ($format_name_deux as $key => $fn) {
    $format->format = $fn;
    $format->name = $name_deux[$key];
    $format->weight = $key - 4;
    filter_format_save($format);
  }

  //permissions
  user_role_grant_permissions(DRUPAL_AUTHENTICATED_RID, array('use text format mail'));
  foreach ($format_name as $sysname) {
    user_role_grant_permissions(DRUPAL_AUTHENTICATED_RID, array('use text format ' . $sysname));
  }
  foreach ($format_name_deux as $sysname) {
    user_role_grant_permissions(DRUPAL_AUTHENTICATED_RID, array('use text format ' . $sysname));
  }

  // php filter
  $format->filters = array(
      'php_code' => array(
          'weight' => 10,
          'status' => 1,
          'settings' => array(),
      ),
  );
  $format->format = 'php_code';
  $format->name = 'PHP code';
  $format->weight = 4;
  filter_format_save($format);
}


/**
 * Active le theme admin et le theme par default
 */
function ctm_installation_config_core_themes($profile, $roles, $op = 'update') {

  if ($profile != CTM_INTERNET && $profile != CTM_INTRANET) {
    return;
  }
  //Initialise le theme admin
  variable_set('admin_theme', 'ctm_back');
  variable_set('node_admin_theme', '1');

  //Active le theme par default
  db_update('system')
      ->fields(array('status' => 1))
      ->condition('type', 'theme')
      ->condition('name', 'ctm')
      ->execute();
  variable_set('theme_default', 'ctm');

  theme_disable(array('bartik'));
}
