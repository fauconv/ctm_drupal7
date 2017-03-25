<?php

/**
 * @file
 * Librairie de fonction helper utilisable par les autres modules/features CTM
 */


/**
 * Enable/Disable tools (in queue)
 * @param string $tools_type :
 *  'devel' : enable/disable development modules
 * 'seo'    : enable/disable SEO modules
 * 'review' : enable/disable review and quality modules
 * @param bool $status :
 *  TRUE  : enable modules
 *  FALSE : disable modules
 * @return false if parameters are wrong
 */
function ctm_installation_tools($tools_type, $status) {
  switch($tools_type) {
    case 'devel':
      if($status) {
        $list = variable_get('ctm_installation_devel_modules_enabled', array());
        $list = array_merge($list, ctm_installation_get_default_devel_modules());
        variable_del('ctm_installation_devel_modules_enabled');
      } else {
        $list = ctm_installation_get_devel_modules();
        $list = array_intersect(module_list(FALSE, FALSE), $list);
        if (!empty($list)) {
          $devel2 = array();
          foreach ($list as $value) {
            $devel2[$value] = $value;
          }
          variable_set('ctm_installation_devel_modules_enabled', $devel2);
        }
      }
      break;
    case 'seo':
      $list = array('contentoptimizer', 'kwresearch', 'readability', 'readability', 'seo_cheker');
      break;
    case 'review':
      $list = array('qa_checklist', 'security_review');
      if(drupal_get_profile() == CTM_INTERNET) {
        $list[] = 'seo_cheklist';
      }
      break;
    default :
      return FALSE;
  }
  if ($status) {
    ctm_installation_queue_module_enable($list);
  }
  else {
    ctm_installation_queue_module_disable($list);
  }
  return TRUE;
}

/**
 * module_invoke_all by module
 */
function ctm_installation_module_invoke_all($hook) {
  $args = func_get_args();
  unset($args[0]);
  $return = array();
  foreach (module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    if (function_exists($function)) {
      $return[$module] = call_user_func_array($function, $args);
    }
  }
  return $return;
}

/**
 * Supprime un repertoire/fichier (vide ou non) recursivement
 * */
function ctm_installation_deleteDirectory($dir) {
  if (!file_exists($dir))
    return TRUE;
  if (!is_dir($dir))
    return unlink($dir);
  foreach (scandir($dir) as $item) {
    if ($item == '.' || $item == '..')
      continue;
    if (!ctm_installation_deleteDirectory($dir . DIRECTORY_SEPARATOR . $item))
      return FALSE;
  }
  return rmdir($dir);
}

/**
 * retourne la version ferme
 */
function ctm_installation_get_version_info() {
  return array(
      'app' => ctm_installation_get_properties_value('releases/release.properties', 'VERSION_APP', '0.0.0'),
      'dist' => ctm_installation_get_properties_value('releases/release.properties', 'dist', '0.0.0'),
      'build' => ctm_installation_get_properties_value('releases/release.properties', 'build', '0.0.0'),
  );
}

/**
 * recupere la valeur d'une variable ant dans un fichier properties
 * @param string $filename : nom du fichier + chemin depuis www
 * @param string $variable : nom de la variable ant
 * @param string $default_value : valeur par defaut retourne si la variable n est pas trouve
 * @return string
 */
function ctm_installation_get_properties_value($filename, $variable, $default_value = NULL) {
  $file = @file($filename);
  if ($file) {
    foreach ($file as $line) {
      if (strpos($line, $variable) === 0) {
        $pos = strpos($line, '=');
        if ($pos > 0) {
          $default_value = str_replace('"','',trim(substr($line, $pos + 1))); //@ignore upgrade47_7
          break;
        }
      }
    }
  }
  return $default_value;
}

/**
 * fonction identique a dd() et drupal_debug() de devel.
 * a utiliser quand devel n'est pas dispo.
 * @param type $data
 * @param type $label
 * @return boolean
 */
function ctm_installation_debug($data, $label = NULL) {
  $out = ($label ? $label . ': ' : '') . print_r($data, TRUE) . "\n";
  // The temp directory does vary across multiple simpletest instances.
  $file = file_directory_temp() . '/drupal_debug.txt';
  if (file_put_contents($file, $out, FILE_APPEND) === FALSE) {
    drupal_set_message(t('ctm_installation was unable to write to %file.', array('%file' => $file)), 'error');
    return FALSE;
  }
}

/**
 * fonction manquante en drupal
 * renvoie true si c'est un ajax ou un batch ou tout appel qui ne correspond pas vraiment a une page
 * @return boolean
 */
function ctm_installation_drupal_is_ajax() {
  if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && !empty($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest')
    return true;
  $path = current_path();
  if (empty($path) || strpos($path, 'admin_menu') !== FALSE || strpos($path, 'batch') !== FALSE || strpos($path, 'ajax') !== FALSE)
    return true;
  return false;
}

/**
 * Verifie qu'une rule menu_position n'existe pas
 */
function ctm_installation_menu_position_load_by_name($name) {
  return db_select('menu_position_rules', 'mpr')
          ->fields('mpr', array('rid', 'admin_title'))
          ->condition('mpr.admin_title', $name)
          ->execute()
          ->fetchAllAssoc('rid');
}

/**
 * Verifie si un noeud existe avant de le creer. par son NOM
 * @param object $node
 * @param bool $update si TRUE le noeud existant sera mis a jour. si FALSE le noeud existant ne sera pas modifier et le nouveau noeud ne sera pas cree.
 */
function ctm_installation_node_save($node, $update = FALSE) {
  $olds = node_load_multiple(array(), array('title' => $node->title, 'type' => $node->type));
  if (empty($olds)) {
    node_save($node);
  }
  elseif ($update) {
    $old = reset($olds);
    $node->nid = $old->nid;
    $node->vid = $old->vid;
    $node->uid = $old->uid;
    node_save($node);
  }
}

/**
 * permet de creer un field
 * @param $field array(
  'field_name' => 'blabla',
  'entity_types' => array('node'),
  'type' => 'text_long',
  )
 */
function ctm_installation_insert_or_update_field($field) {
  $info = field_info_field($field['field_name']);
  if (empty($info)) {
    field_create_field($field);
  }
  else {
    field_update_field($field);
  }
}


/**
 * user role creation wrapper
 * @param $roles list of roles
 * @param $name role name
 * @param $weight sort roles list
 */
function ctm_installation_insert_or_update_role(&$roles, $name, $weight) {
  if (empty($roles[$name])) {
    //Cree un role
    $role = new stdClass();
    $role->name = $name;
    $role->weight = $weight;
    user_role_save($role);
  }
  else {
    $role = user_role_load_by_name($name);
  }
  $roles[$role->name] = $role->rid;
  return $role;
}

/**
 * permet de creer une instance de field lier a un bundle (node ou taxo)
 * @param $instance array(
  'field_name' => 'texte_page_base',
  'entity_type' => 'node',
  'bundle' => 'page_de_base',
  'label' => 'Texte',
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
  'ckeditor_images_liens_etroit' => 'ckeditor_images_liens_etroit',
  'ckeditor_images_liens_large' => 'ckeditor_images_liens_large',
  ),
  'default_order_toggle' => 0,
  ),
  'user_register_form' => FALSE,
  ),
  'widget' => array('type' => 'text_textarea'),
  );
 */
function ctm_installation_insert_or_update_field_instance($instance) {
  $exist = field_info_instance($instance['entity_type'], $instance['field_name'], $instance['bundle']);
  if ($exist) {
    field_update_instance($instance);
  }
  else {
    field_create_instance($instance);
  }
}

/**
 * cree ou mes a jour des enregistrements dans une table
 * @param $table string
 *    le nom de la table
 * @param $values array(array(field_name string => field_value string))
 *  les valeurs a stocker
 * @param $conditions array(field_name string)
 *  liste des champs qui constitue la cle primaire
 */
function ctm_installation_insert_or_update($table, $values, $conditions) {
  foreach ($values as $record) {
    $query = db_select($table, 't')
        ->fields('t');
    foreach ($conditions as $field) {
      $query->condition($field, $record[$field]);
    }
    $result = $query->execute();
    if ($result->rowCount() > 0) {
      $query = db_update($table)
          ->fields($record);
      foreach ($conditions as $field) {
        $query->condition($field, $record[$field]);
      }
      $query->execute();
    }
    else {
      $query = db_insert($table)
          ->fields($record)
          ->execute();
    }
  }
}

/**
 * insert or update de blocks
 * @param array $values array(array(field_name string => field_value string))
 *  les valeurs a stocker
 */
function ctm_installation_insert_or_update_blocks($values) {

  ctm_installation_insert_or_update('block', $values, array('module', 'delta', 'theme'));
}

/**
 * renvoi un menu link comme menu_link_get_preferred meme si le menu n'a pas (encore) de router
 * ou si l'on passe par drush ou si il s'agit du menu accueil (qui n a pas de router)
 * @param type $path : The path; for example, 'node/5'.
 * @param type $selected_menu : the name of the menu
 */
function ctm_installation_menu_link_get($path, $selected_menu) {
  $path = str_replace('_', '-', $path);
  $nolink = '<nolink>?id=';
  if (strpos($path, $nolink) === 0) {
    $id = substr($path, strlen($nolink));
    $path = '<nolink>';
  }
  $query = db_select('menu_links', 'ml');
  $query->leftJoin('menu_router', 'm', 'm.path = ml.router_path');
  $query->fields('ml');
  $query->addField('ml', 'weight', 'link_weight');
  $query->fields('m');
  $query->condition('ml.link_path', $path);
  $query->condition('ml.menu_name', $selected_menu);
  $links = $query->execute();
  $link = $links->fetchAssoc();
  if (empty($link)) {
    return FALSE;
  }
  if (!isset($id)) {
    if (empty($link['router_path'])) {
      return $link;
    }
  }
  else {
    while ($link) {
      $options = unserialize($link['options']);
      if (isset($options['query']['id']) && $options['query']['id'] == $id) {
        if (empty($link['router_path'])) {
          return $link;
        }
        break;
      }
      $link = $links->fetchAssoc();
    }
    if (empty($link)) {
      return FALSE;
    }
  }
  $map = explode('/', $path);
  _menu_translate($link, $map);
  return $link;
}

/**
 * fonction manquante dans drupal. le "set" qui va avec theme_get_setting
 * @param string $setting comme theme_get_setting
 * @param string $value la valeur a definir
 * @param string $theme comme theme_get_setting
 */
function theme_set_setting($setting, $value, $theme = NULL) {
  theme_get_setting($setting, $theme); // on initialise le cache
  $cache = &drupal_static('theme_get_setting', NULL); // oui on prend le cache d'une autre fonction
  // If no key is given, use the current theme if we can determine it.
  if (!isset($theme)) {
    $theme = !empty($GLOBALS['theme_key']) ? $GLOBALS['theme_key'] : '';
  }
  $cache[$theme][$setting] = $value; // on met en cache pour l avoir dessuite
  $theme_array = variable_get('theme_' . $theme . '_settings', array()); // et on enregistre
  $theme_array[$setting] = $value;
  variable_set('theme_' . $theme . '_settings', $theme_array);
}

/**
 * copy de la fonction user_roles de drupal mais sans la traduction pour pouvoir
 * etre utilise comme fonction systeme. De plus le tableau rendu est dans
 * le sens inverse : l'index est le nom, la valeur est le rid
 *
 * @return
 *   An associative array with the role name as the key and the role id as
 *   value.
 */
function ctm_installation_user_roles($clear = FALSE) {
  // Use the advanced drupal_static() pattern, since this is called very often.
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast['roles'] = &drupal_static(__FUNCTION__);
  }
  $roles = &$drupal_static_fast['roles'];
  if ($clear)
    $roles = array();
  if (empty($roles)) {
    $query = db_select('role', 'r');
    $query->fields('r', array('rid', 'name'));
    $result = $query->execute();
    $roles = array();
    foreach ($result as $role) {
      $roles[$role->name] = $role->rid;
    }
  }
  return $roles;
}

/**
 * ajoute un content type a la liste des content type interogeable par linkit
 * @param $ct_name (string) nom machine du content type
 */
function ctm_installation_add_ct_to_linkit($ct_name) {
  $linkit_profile_editeur = linkit_profile_load('editeur');
  $linkit_profile_champs = linkit_profile_load('champs');
  if (empty($linkit_profile_editeur->name) || empty($linkit_profile_champs->name)) {
    drupal_set_message(t('Linkit Profil cannot be found and set'), 'warning');
    return;
  }
  $linkit_profile_editeur->data['entity:node']['bundles'][$ct_name] = $ct_name;
  $linkit_profile_champs->data['entity:node']['bundles'][$ct_name] = $ct_name;
  ctools_export_crud_save('linkit_profiles', $linkit_profile_editeur);
  ctools_export_crud_save('linkit_profiles', $linkit_profile_champs);
}

/**
 * desactive une liste de vue
 * @param type $view_list
 */
function ctm_installation_deactivate_views($view_list) {

  $status = variable_get('views_defaults', array());
  foreach ($view_list as $name) {
    $status[$name] = TRUE;
  }
  variable_set('views_defaults', $status);
}

/**
 * active une liste de vue
 * @param type $view_list
 */
function ctm_installation_activate_views($view_list) {
  $status = variable_get('views_defaults', array());
  foreach ($view_list as $name) {
    if (isset($status[$name])) {
      unset($status[$name]);
    }
  }
  variable_set('views_defaults', $status);
}

/**
 * une fois qu'un content type block est cree, permet de positionner les valeurs
 * par default des element autour du content type :
 * -variables
 */
function ctm_installation_set_default_block_datas($type) {
  //Configuration publish content
  variable_set('publishcontent_' . $type, 1);

  //Config du contenu
  variable_set('node_preview_' . $type, 0);
  variable_set('node_submitted_' . $type, 0);
  variable_set('node_options_' . $type, array('0' => ''));
  variable_set('menu_options_' . $type, array('0' => ''));
  variable_set('display_cache_node_' . $type . '_default', array('default' => Array(
          'use' => 1,
          'page_granularity' => 0,
          'user_granularity' => 0,
          'granularity' => 0,
  )));
  variable_set('display_cache_comment_publication_clears_host_entity_cache_node_' . $type, array('default' => 0));
}

/**
 * set the values for each field of the content type for display_cache
 * @param type $type
 * @param type $fields
 */
function ctm_installation_set_default_display_cache_field($type, $fields) {
  $var = variable_get('display_cache_node_' . $type . '_default', array());
  foreach ($fields as $field) {
    $var[$field] = array(
        'override' => 2,
        'page_granularity' => 0,
        'user_granularity' => 0,
        'granularity' => 0,
    );
    variable_set('display_cache_node_' . $type . '_default', $var);
  }
}

/**
 * une fois qu'un content type standard est cree, permet de positionner les valeurs
 * par default des element autour du content type :
 * -variables
 * -scheduler
 * -linkit
 */
function ctm_installation_set_default_content_type_datas($type) {

  ctm_installation_set_default_block_datas($type);

  //scheduler
  variable_set('scheduler_publish_enable_' . $type, 1);
  variable_set('scheduler_unpublish_enable_' . $type, 1);
  variable_set('scheduler_publish_touch_' . $type, 1);
  variable_set('scheduler_publish_past_date_' . $type, 'publish');
  variable_set('scheduler_use_vertical_tabs_' . $type, 1);
  variable_set('menu_options_' . $type, array('0' => 'main-menu'));
  variable_set('menu_parent_' . $type, 'main-menu:0');
  variable_set('node_preview_' . $type, '1');

  //profil linkit
  ctm_installation_add_ct_to_linkit($type);

  //Config du contenu
  variable_set('pathauto_node_' . $type . '_pattern', '[node:menu-link:parents:join-path]/[node:title]');
}

/**
 * Permet de mettre/supprimer en place un message recurant
 * @param type $id : id du message pour le retrouver a la suppresion
 * @param type $message : le message
 * @param type $path : un chemin ou le message ne doit pas s afficher
 */
function ctm_installation_set_message($id, $message = '', $path = '') {
  if (!empty($message)) {
    $_SESSION['ctm_reminder'][$id] = array(
        'message' => $message,
        'path' => $path,
    );
  }
  elseif (isset($_SESSION['ctm_reminder'][$id])) {
    unset($_SESSION['ctm_reminder'][$id]);
  }
}

/**
 * Fonction de soft reset de drupal avec hard clear
 * vide tous les caches et les variables static, ainsi que les batch en cours,
 * et reload tous les modules.
 * Attention ne surtout pas utiliser dans un batch !!!!!
 */
function ctm_installation_drupal_reset() {

  // reset du cache des batch si on est dans le cas ou on enchaine les commande drush dans une seule commande
  // car drush fait des batch pourri
  $batch = &batch_get();
  $batch = array();

  //cache clear
  ctm_installation_drupal_flush_cache();
}

/**
 * Fonction de hard clear du cache
 * vide tous les caches et les variables static,
 * et reload tous les modules.
 */
function ctm_installation_drupal_flush_cache() {

  //cache clear
  drupal_flush_all_caches();

  //static variable reset
  drupal_static_reset();

  // module list
  module_load_all();
  module_implements('', FALSE, TRUE);
  actions_synchronize(TRUE);
}

/**
 * donne en string la liste des base_path possible (1 par ligne)
 */
function ctm_installation_get_base_url_str() {
  global $base_path;
  $base_root_list = variable_get('base_root_list');
  $alias = substr($base_path, 0, -1);
  $str = '';
  foreach ($base_root_list as $base_root) {
    $str .= $base_root . $alias . "\n";
  }
  return $str;
}

/**
 * Ajout des configuration pour les metatag
 * If metatag doesn't exist, insertion of metatags line from $datas.
 * Else If metatag exist but title value doesn't equals, update title values from $datas
 * Else does nothing
 */
function ctm_installation_metatag($datas) {

  $cpt_updated = 0;
  $cpt_inserted = 0;
  $table_name = 'metatag_config';
  foreach ($datas as $field) {
    $record = db_select($table_name, 't')
        ->fields('t')
        ->condition('instance', $field[1])
        ->execute()
        ->fetchAssoc();
    if ($record != FALSE) {
      if (isset($record['config'])) {
        $unserialized_col = unserialize($record['config']);
        if ($unserialized_col['title']['value'] == $field[2]['title']['value']) {
          db_update($table_name)
              ->fields(array('instance' => $field[1], 'config' => serialize($field[2])))
              ->condition('cid', $record['cid'], '=')
              ->execute();
          $cpt_updated++;
        }
      }
    }
    else {
      db_insert($table_name)
          ->fields(array('instance', 'config'))
          ->values(array($field[1], serialize($field[2])))
          ->execute();
      $cpt_inserted++;
    }
  }
  if ($cpt_inserted == 0 && $cpt_updated > 0) {
    drupal_set_message('' . $cpt_updated . ' records updated!', 'status');
  }
}

/**
 * cree un menu_block et l'insert dans la bonne region
 * @param array $block
 *    like ctm_installation_insert_or_update_block but for aonly 1 bloc
 * @param array $menu
 *   depth
 *   expanded
 *   follow
 *   level
 *   parent
 *   sort
 *   title_link
 */
function ctm_installation_insert_or_update_menu_block($block, $menu) {
  $id = $block['delta'];
  $menu_block_ids = variable_get('menu_block_ids', array());
  if (!in_array($id, $menu_block_ids)) {
    $menu_block_ids[] = $id;
  }
  variable_set('menu_block_' . $id . '_admin_title', $id);
  variable_set('menu_block_' . $id . '_depth', $menu['depth']);
  variable_set('menu_block_' . $id . '_depth_relative', $menu['depth_relative']);
  variable_set('menu_block_' . $id . '_expanded', $menu['expanded']);
  variable_set('menu_block_' . $id . '_follow', $menu['follow']);
  variable_set('menu_block_' . $id . '_level', $menu['level']);
  variable_set('menu_block_' . $id . '_parent', $menu['parent']);
  variable_set('menu_block_' . $id . '_sort', $menu['sort']);
  variable_set('menu_block_' . $id . '_title_link', $menu['title_link']);
  variable_set('menu_block_ids', $menu_block_ids);
  ctm_installation_insert_or_update_blocks(array($block));
}

/**
 * Insert a po file (translation file) with the format :
 * module.group.version.lang.po
 * @param string $poFile
 * @param cont $mode : LOCALE_IMPORT_OVERWRITE ou LOCALE_IMPORT_KEEP
 */
function ctm_installation_insert_po_file($poFile, $mode = LOCALE_IMPORT_OVERWRITE) {
  $file = new stdClass;
  $file->uri = $poFile;
  $file->filename = basename($poFile);
  $elems = explode('.',$file->filename);
  if(count($elems) != 5) {
    return FALSE;
  }
  $group = $elems[1];
  $langcode = $elems[3];
  $ext = $elems[4];
  if($group != 'menu' && $group != 'default' || $ext != 'po') {
    return FALSE;
  }
  return _locale_import_po($file, $langcode, $mode, $group);
}

/**
 * backportage de drupal 8
 * Load a language object
 * @param string $langcode
 *    a language code (fr, en ...)
 * @return a object language
 */
function language_load($langcode) {
  $languages = language_list();
  return $languages[$langcode];
}

/**
 * delete all url alias to rebuild it
 */
function ctm_installation_delete_alias_all() {
  db_delete('url_alias')->execute();
}

/**
 * generate all static alias with the hook_installation_alias and
 * not generated by pathauto
 */
function ctm_installation_generate_static_alias() {
  //Creates aliases when hook is invoked
  $aliases = module_invoke_all('installation_alias');
  foreach($aliases as $source => $paths) {
    //Removes rows matching source and creates clean ones
    $conditions = array(
      'source' => $source,
    );
    path_delete($conditions);
    foreach($paths as $language => $path) {
      $path = $conditions + array(
        'alias' => $path,
        'language' => $language,
      );
      path_save($path);
    }
  }
}

/**
 * test if drush is present then display a drush log
 * si drush_log for params
 */
function ctm_installation_drush_log($message) {
  if (function_exists('drush_log')) {
    drush_log($message,'status');
  }
}

/**
 * helper function to get value of a field from the form_state
 * @param array $form_state
 *   the form_state variable from a form
 * @param string $field_name
 *   the field_name
 */
function ctm_installation_get_form_state_value($form_state,$field_name) {
  return reset($form_state['values'][$field_name]);
}


/**
 * Cree un profil ckeditor et assigne leur format de texte
 * @param $data : array('name'=>string, 'settings'=>array, 'input_formats'=>array)
 */
function ctm_installation_create_ckeditor_profil($data) {
  $values = array(
      "name" => $data['name'],
      "settings" => serialize($data['settings']));
  ctm_installation_insert_or_update('ckeditor_settings', array($values), array('name'));

  if (!empty($data['input_formats'])) {
    $values = array();
    foreach (array_keys($data['input_formats']) as $format) {
      $values[] = array("name" => $data['name'], "format" => $format);
    }
    ctm_installation_insert_or_update('ckeditor_input_format', $values, array('name'));
  }
}

/**
 * 
 * @param type $content_type_bundle
 */
function ctm_installation_insert_or_update_content_type($content_type_bundle) {
  $content_type_bundle = node_type_set_defaults($content_type_bundle);
  node_type_save($content_type_bundle);
  $bundle_settings = field_bundle_settings('node', $content_type_bundle->type);
  $bundle_settings['view_modes']['teaser']['custom_settings'] = FALSE;
  field_bundle_settings('node', $content_type_bundle->type, $bundle_settings);
}