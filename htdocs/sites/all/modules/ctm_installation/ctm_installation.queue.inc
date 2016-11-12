<?php

/*
 * @file
 * API pour la gestion des queues de ctm_installation
 */
define('CTM_MENU_POSITION_PAGE','pages');
define('CTM_MENU_POSITION_CT','content_type');

/**
 * essai de vider les element en attente dans la queue
 * voir ctm_installation_check_urls
 * ne devrait etre appeler que par ctm_installation_init()
 * @param bool $delayed
 *    if true they function process delayed task (internal use only)
 * @return
 *    true si quelquechose a ete fait
 *    false si il n y a plus rien dans la queue
 */
function ctm_installation_process_queue($delayed = FALSE) {

  //
  // si ce n est pas une page normal on s en va
  //
  $arg1 = arg(0);
  $arg2 = arg(1);
  $arg3 = arg(2);
  $arg4 = arg(3);
  if (ctm_installation_drupal_is_ajax() || variable_get('install_task') != 'done' ||
      $arg1 === 'admin' &&
      !array_key_exists($arg2, array('structure' => TRUE, 'content' => TRUE)) &&
      $arg3 !== 'ctm' && $arg4 !== 'ctm' ||
      $arg1 === 'devel') {
    return FALSE;
  }

  //
  // si on est pas admin on s'en va
  //
  $drush = function_exists('drush_log');
  $admin_rid = variable_get('user_admin_role');
  if (!$drush && !user_access('Bypass content access control') && !user_has_role($admin_rid)) {
    return FALSE;
  }

  //traitement
  $queue = ctm_installation_get_queue();
  $batchs = array();
  $old_file = '';
  $batch = array(
      'title' => t('Update website...'),
      'operations' => array(),
  );
  $opPresent = FALSE;
  $toRelease = array();
  $toDelay = array();
  while ($item = $queue->claimItem(40)) { // pour chaque element dans la queue. le release se fait par le cron ou queue_ui si le batch plante
    $checker = $item->data['#checker'];
    $operation = $checker($item->data['#callback'], $item, $item->data['#datas']);
    if (!$delayed && $operation === 'delayed') { // c est une operation a faire en fin de queue
      $toDelay[] = $item;
      $toRelease[] = $item;
    } elseif ($operation) { // si on peut faire l operation maintenant
      if($operation === 'delayed') {
        $operation = array($item->data['#callback'], array($item, $item->data['#datas']));
      }
      if (!empty($item->data['#file']) && $old_file != $item->data['#file']) { // si il y a un fichier je cree un nouveau batch
        if ($old_file != '') {
          $batchs[] = $batch;
          $batch = array(
              'title' => 'Update website... (continu)',
              'operations' => array(),
          );
        }
        $batch['file'] = $item->data['#file'];
        $old_file = $item->data['#file'];
      }
      $batch['operations'][] = $operation;
      $opPresent = TRUE;
    }
    else { //je ne rempli pas les condition pour faire la tache maintenant
     $toRelease[] = $item;
    }
  }
  foreach ($toRelease as $item) {
    $queue->releaseItem($item);
  }
  if ($opPresent) { // j'ai rempli ma queue
    $batch['operations'][] = array('_ctm_installation_end_queue_batch', array(0, $toDelay)); // tache de fin de queue courante
    $batchs[] = $batch;
    foreach ($batchs as $batch) {
      batch_set($batch);
    }
    if ($drush) {
      $batch = &batch_get();
      $batch['progressive'] = FALSE;
      batch_process();
      return ctm_installation_process_queue();
    }
    batch_process(request_uri());
    return TRUE;
  }elseif(!empty($toDelay)) {
    return ctm_installation_process_queue(TRUE);
  }
  return FALSE;
}

/**
 * renvoi la queue sous forme de tableau
 * @return array
 */
function ctm_installation_get_queue() {
  return DrupalQueue::get('ctm_installation');
}

/**
 * release tous les element de la queue pour une reexecution imediate
 */
function ctm_installation_queue_release() {
  db_update('queue')
      ->fields(array(
          'expire' => 0,
      ))
      ->condition('name', 'ctm_installation', '=')
      ->execute();
}

/**
 * supprime un element de la queue
 * @param string $item : l'item a supprimer
 */
function ctm_installation_del_elem_queue($item) {
  if(!empty($item)) {
    $queue = ctm_installation_get_queue();
    $queue->deleteItem($item);
  }
}

/**
 * supprime un element de la queue
 * @param string $item : l'item a supprimer
 */
function ctm_installation_release_elem_queue($item) {
  if(!empty($item)) {
    $queue = ctm_installation_get_queue();
    $queue->releaseItem($item);
  }
}

/**
 * Ajoute un element a la queue
 * Permet de mettre en attente des traitements qui n ecessite l'existance
 * d'une / des url qui n existe pas encore
 * @param callback $callback
 *   la fonction a appeler pour faire le traitement une fois que le test est valide
 *   la signature du callback depend du checker. C'est le callback qui doit ou non enlever
 *   l element de la queue a l aide de la fonction ctm_installation_del_elem_queue($id)
 * @param mixed $datas
 *   donnees utile pour le checker et le callback. (voir les checker pour plus d'info)
 * @param string $file
 *   un fichier a charger pour avoir acces au callback
 * @param string $checker
 *   la function de test a utiliser
 * @param bool unique
 *   la fonction ajoute l element en queue mais s assure avant de supprimer
 * les elements identique dans la queue si il y en a.
 */
function ctm_installation_add_to_queue($callback, $datas = array(), $file = '', $checker = 'ctm_installation_check_no', $unique = FALSE) {
  if($unique) {
    ctm_installation_remove_from_queue($callback, $datas, $file, $checker);
  }
  $queue = ctm_installation_get_queue();
  $queue->createItem(array(
      '#callback' => $callback,
      '#checker' => $checker,
      '#datas' => $datas,
      '#file' => $file,
  ));
}

function ctm_installation_remove_from_queue($callback, $datas = array(), $file = '', $checker = 'ctm_installation_check_no') {
  while($item = _ctm_installation_is_in_queue($callback, $datas, $file, $checker)) {
    ctm_installation_del_elem_queue($item);
  }
}

function ctm_installation_is_in_queue($callback, $datas = array(), $file = '', $checker = 'ctm_installation_check_no') {
  if($item = _ctm_installation_is_in_queue($callback, $datas, $file, $checker)) {
    ctm_installation_release_elem_queue($item);
    return TRUE;
  }
  return FALSE;
}

function _ctm_installation_is_in_queue($callback, $datas = array(), $file = '', $checker = 'ctm_installation_check_no') {
  $queue = ctm_installation_get_queue();
  $stack = array();
  $ret = FALSE;
  while ($item = $queue->claimItem(40)) { // pour chaque element dans la queue. le release se fait par le cron ou queue_ui
    if($checker  != $item->data['#checker'] ||
       $callback != $item->data['#callback'] ||
       $datas !== $item->data['#datas'] ||
       $file != $item->data['#file']
        ) {
      $stack[] = $item;
      continue;
    }
    $ret = $item;
    break;
  }
  foreach($stack as $item) {
    $queue->releaseItem($item);
  }
  return $ret;
}

/**
 * affiche et log le batch en cours
 * @param array $context
 * @param string $message
 */
function ctm_installation_queue_message(&$context, $message) {
  $context['message'] = t($message);
  watchdog('Installation and configuration', $message, array(), WATCHDOG_DEBUG);
  ctm_installation_drush_log($message,'ok');
}

/**
 * fonction helper a placer a la fin d'un batch pour gerer automatiquement le bouclage du batch et la queue
 * cela implique que les variables $context['sandbox']['max'] et $context['sandbox']['progress'] aient ete initialise
 * @param array $context
 *  le tableau context fournis par le systeme de batch
 * @param string $id
 *  l id fournis par le systeme de queue
 */
function ctm_installation_queue_batch_end(&$context, $id) {
  if (empty($context['sandbox']['max'])) {
    ctm_installation_del_elem_queue($id);
  }
  $context['sandbox']['progress'] ++;
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
  else {
    ctm_installation_del_elem_queue($id);
  }
}

/**
 * init the batch must be called like this at start of a batch:
 * if (!isset($context['sandbox']['progress'])) {
 *   xxxxxxxx;
 *   if(ctm_installation_queue_batch_start($context, yyyyy)) {
 *     return;
 *   }
 * }
 * @param array $context
 * @param array $max
 *  an array to count to know the total number of progress to do
 * @return boolean
 *  true if there is nothing to do
 */
function ctm_installation_queue_batch_start($id, $context, $max) {
  $context['sandbox']['progress'] = 0;
  $context['sandbox']['max'] = count($max);
  if (!$context['sandbox']['max']) {
    ctm_installation_del_elem_queue($id);
    return TRUE;
  }
  return FALSE;
}
// ------------------------------------ CHECKERS -------------------------------
/**
 * Fonction appeler par le systeme de queue. Elle verifie l existante des items
 * de menu et appel le callback si elles existe en donnant en deuxieme
 * parametre la list des items de menu trouve
 * @param string $callback
 *   la fonction doit etre de la forme
 *   $callback($id, $existing_link, $datas)
 * @param string $id
 *   voir ctm_installation_add_to_queue
 * @param array $datas
 *   le tableau est constitue des cles suivantes :
 *   - menu_paths (array) : le comportement de la fonction depand des cle de ce
 *      tableau (voir plus bas)
 *   - menu_names (array): contient le nom du menu dans lequel on cherche le ou
 *      les items. Doit avoir le meme nombre de parametre et les memes cles que
 *      menu_paths. dans le cas "features" la valeur n'a pas d'importance
 *   - mixed datas (mixed) : donnees pour le call back
 * 1) si menu_path contient une cle "router_path" alors on cherche un item qui a
 * pour router cette valeur (on verifie juste que le router est present)
 * 2) si menu_path contient une cle "node_uuid" alors on cherche un node qui
 * a cette uuid et on renvoi son path drupal
 * 3) sinon on cherche un item de menu qui a pour url la valeur de menu_path
 * pour cette cle
 */
function ctm_installation_check_urls($callback, $id, $datas) {
  $menu_link = array();
  $menu_paths = $datas['menu_paths'];
  $menu_names = $datas['menu_names'];
  foreach ($menu_paths as $key => $menu_path) {
    switch((string)$key) {
      case 'router_path' :
        if ($menu_names[$key] != _menu_find_router_path($menu_path)) {
          return FALSE;
        }
        break;
      case 'node_uuid' :
        $nids = entity_get_id_by_uuid('node', array($menu_path));
        if(empty($nids)) {
          return false;
        }
        $menu_link[$key] = 'node/'.reset($nids);
        break;
      default:
        $elem = ctm_installation_menu_link_get($menu_path, $menu_names[$key]); // on utile ctm_installation car on peut passer par drush
        if (empty($elem)) {
          return FALSE;
        }
        $menu_link[$key] = $elem;
    }
  }
  return array($callback, array($id, $menu_link, $datas['datas']));
}

/**
 * check if the list of user permission passed in $datas['perms'] exist
 * @param string $callback
 * @param string $id
 * @param array $datas
 *  must content at least a key 'perms' which the value is an array of string
 */
function ctm_installation_check_user_permissions($callback, $id, $datas) {
  $modules = user_permission_get_modules();
  foreach($datas['perms'] as $perm) {
    if(empty($modules[$perm])) {
      return FALSE;
    }
  }
  return array($callback, array($id, $datas));
}

/**
 * Check to execute batch only if we are not in phpcli (drush, cron...)
 * @param string $callback
 * @param string $id
 * @param mixed $datas
 * @return boolean
 */
function ctm_installation_check_cli($callback, $id, $datas) {
  if (drupal_is_cli() || function_exists('drush_log')) {
    return FALSE;
  }
  return array($callback, array($id, $datas));
}

/**
 * indicate to the queue processor to execute this operation to end of the queue
 * @param type $callback
 * @param type $id
 * @param type $datas
 * @return "delayed"
 */
function ctm_installation_check_empty_queue($callback, $id, $datas) {
  return "delayed";
}

/**
 * Fonction appeler par le systeme de queue. Elle execute le callback
 * sans aucun check
 * @param string $callback
 *   la fonction doit etre de la forme
 *   $callback($id, $datas)
 * @param string $id
 *   voir ctm_installation_add_to_queue
 * @param array $datas
 *   les donnees pour le callback
 */
function ctm_installation_check_no($callback, $id, $datas) {
  return array($callback, array($id, $datas));
}

//--------------------CALLBACKS (batch operation) ------------------------------------------


/**
 * import all .po file from : conf_path/translation
 * @param string $id
 * @param void $empty
 * @param array $context
 */
function _ctm_installation_import_po($id, $empty, &$context) {
  $dir = DRUPAL_ROOT.'/'.conf_path().'/translations';
  if(!file_exists($dir)) {
    return;
  }
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['files'] = scandir($dir);
    if(ctm_installation_queue_batch_start($id, $context, $context['sandbox']['files'])) {
      return;
    }
  }
  $file = $context['sandbox']['files'][$context['sandbox']['progress']];
  if(is_file($dir.'/'.$file)) {
    ctm_installation_queue_message($context, 'Add translations: '.$file);
    ctm_installation_insert_po_file($dir.'/'.$file);
  }
  ctm_installation_queue_batch_end($context, $id);
}
/**
 * call back de la queuepour ctm_installation_queue_inset_or_update_menu_block
 * @param string $id
 *   l id fournis par le systeme de queue
 * @param array $existing_links
 *   le menu link fournis par le checker (le parent du menu block)
 * @param array $item
 *   les info sur le menu bloc a cree : (voir ctm_installation_insert_or_update_menu_block)
 *   block => info du bloc
 *   menu  => info du menu
 * @param array $context
 *   le context fournis par le systeme de batch
 */
function _ctm_installation_menu_block_batch($id, $existing_links, $item, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    if(ctm_installation_queue_batch_start($id, $context, $existing_links)) {
      return;
    }
  }
  ctm_installation_queue_message($context, 'Add or update menu blocks');
  $index = $context['sandbox']['progress'];
  $existing_link = $existing_links[$index];
  $menu = $item[$index]['menu'];
  $block = $item[$index]['block'];
  $menu['parent'] = $existing_link['menu_name'] . ':' . $existing_link['mlid'];
  ctm_installation_insert_or_update_menu_block($block, $menu);
  ctm_installation_queue_batch_end($context, $id);
}

/**
 * callback de la queue pour ctm_installation_menu_link_save
 * @param string $id
 *  l id fournis par le systeme de queue
 * @param array $existing_links
 *  le menu link fournis par le checker
 * @param array $item
 *  l item de menu fournis par l appel a la fonction de l api
 * @param array $context
 *  le context fournis par le systeme de batch
 */
function _ctm_installation_menu_link_save_batch($id, $existing_links, $item, &$context) {
  if (empty($item['link_path'])) {
    ctm_installation_queue_message($context, 'Updating menu ' . $existing_links['link_path']['link_path']);
  }
  else {
    ctm_installation_queue_message($context, 'Adding menu ' . $item['link_path']);
  }

  $existing_item = array();
  $parent_candidates = array();
  $item['customized'] = 1;
  if (isset($existing_links['parent_path'])) { // on doit lier le menu a un parent
    $parent_candidates = $existing_links['parent_path'];
    $item['plid'] = $parent_candidates['mlid'];
  }
  if (isset($existing_links['link_path'])) { // c est une mise a jour de noeud
    $existing_item = $existing_links['link_path'];
    $item = array_merge($existing_item, $item);
    $existing_item['options'] = '';
  }
  if(isset($existing_links['node_uuid'])) { // c est un lien vers un node
    $item['link_path'] = $existing_links['node_uuid'];
  }
  if (menu_link_save($item, $existing_item, $parent_candidates)) {
    ctm_installation_del_elem_queue($id);
  }
}

/**
 * callback de la queue pour supprimer une rules menu position
 * * @param string $id
 *  l id fournis par le systeme de queue
 * @param string $title
 *  l item de menu fournis par l appel a la fonction de l api
 * @param array $context
 *  le context fournis par le systeme de batch
 */
function ctm_installation_remove_menu_position_rule_batch($id, $title, &$context) {
  module_load_include('inc', 'menu_position', 'menu_position.admin');
  $rid = db_select('menu_position_rules', 'm')
          ->fields('m', array('rid'))
          ->condition('admin_title', $title)
          ->execute()->fetchField();
  menu_position_delete_rule($rid);
  ctm_installation_queue_message($context, 'Delete a menu position rule');
  ctm_installation_del_elem_queue($id);
}

/**
 * callback de la queue pour ctm_installation_menu_position_add_rule
 * peut etre utiliser comme fonction normale, avec un tableau de context vide
 * @param string $id
 *  l id fournis par le systeme de queue
 * @param array $menu
 *  le menu link fournis par le checker
 * @param array $item
 *  l item de menu fournis par l appel a la fonction de l api
 * @param array $context
 *  le context fournis par le systeme de batch
 */
function ctm_installation_menu_position_add_rule_batch($id, $menu, $item, &$context) {
  ctm_installation_queue_message($context, 'Add menu position rule for ' . $item[$item['type']]);
  module_load_include('inc', 'menu_position', 'menu_position.admin');
  $rule = array(
      'admin_title' => $item['title'],
      'conditions' => array($item['type'] => array($item['type'] => array())),
      'menu_name' => $item['menu_name'],
      'plid' => $menu[0]['mlid'],
  );
  switch($item['type']) {
    case 'content_type':
      $rule['conditions'][$item['type']][$item['type']] = array(
          $item[$item['type']] => $item[$item['type']],
      );
      break;
    default:
      $rule['conditions'][$item['type']][$item['type']] = $item[$item['type']];
      break;
  }
  menu_position_add_rule($rule);
  ctm_installation_del_elem_queue($id);
}

/**
 * callback de la queue pour ctm_installation_set_menu_parent
 * @param string $id
 *  l id fournis par le systeme de queue
 * @param array $menu
 *  le menu link fournis par le checker
 * @param array $item
 *  l item de menu fournis par l appel a la fonction de l api
 * @param array $context
 *  le context fournis par le systeme de batch
 */
function _ctm_installation_set_menu_parent_batch($id, $menu, $item, &$context) {
  ctm_installation_queue_message($context, 'Add menu parent for ' . $item['ct_name']);
  $mlid = $menu[0]['mlid'];
  variable_set('menu_parent_' . $item['ct'], $item['menu_name'] . ':' . $mlid);
  ctm_installation_del_elem_queue($id);
}

/**
 * supprime une rules
 * @param string $id
 *  l id fournis par le systeme de queue
 * @param string $rules_code
 *  l item de menu fournis par l appel a la fonction de l appis
 * @param array $context
 *  le context fournis par le systeme de batch
 */
function ctm_installation_remove_rules_batch($id, $rules_code, &$context) {
  ctm_installation_queue_message($context, 'remove rule ' . $rules_code);
  $rule = rules_config_load($rules_code);
  if (!empty($rule)) {
    rules_config_delete(array($rule->id));
  }
  ctm_installation_del_elem_queue($id);
}

/**
 * callback de la queue pour ctm_installation_add_clean_view_rules
 * peut etre utiliser comme fonction normale, avec un tableau de context vide
 * @param string $id
 *  l id fournis par le systeme de queue
 * @param array $menu
 *  le menu link fournis par le checker
 * @param array $content
 *  l item de menu fournis par l appel a la fonction de l api
 * @param array $context
 *  le context fournis par le systeme de batch
 */
function ctm_installation_add_clean_view_rules_batch($id, $menu, $content, &$context) {
  ctm_installation_queue_message($context, 'Add clean view  rule for ' . $content['rules_name']);
  $rule = rules_config_load($content['view_name']);
  if (!empty($rule)) {
    rules_config_delete(array($rule->id)); // on supprime l'ancienne avant de la remettre
  }
  $str_rule = ' { "' . $content['view_name'] . '" : {
                    "LABEL" : "' . $content['rules_name'] . '",
                    "PLUGIN" : "reaction rule",
                    "OWNER" : "rules",
                    "TAGS" : [ "ctm_installation" ],
                    "REQUIRES" : [ "cache_actions", "rules" ],
                    "ON" : {';
  $first = TRUE;
  foreach ($content['cts'] as $type) {
    if(!$first) {
      $str_rule .= ',';
    } else {
      $first = FALSE;
    }
    $str_rule .= '"node_insert--' . $type . '" : { "bundle" : "' . $type . '" },
                      "node_update--' . $type . '" : { "bundle" : "' . $type . '" },
                      "node_delete--' . $type . '" : { "bundle" : "' . $type . '" }';
  }
  $str_rule .= '},
                    "DO" : [
                      { "cache_actions_action_clear_views_cache" : { "view" : { "value" : { "' . $content['view_name'] . '" : "' . $content['view_name'] . '" } } } }
                    ]
                  }
                }';
  $error_msg = '';
  $rule_config = rules_import($str_rule, $error_msg);
  if(!empty($error_msg)) {
    drupal_set_message($error_msg,'error');
  }
  $rule_config->save($content['view_name'], "ctm_installation");
  ctm_installation_del_elem_queue($id);
}

/**
 * callback de la queue pour disable (module et feature)
 * @param string $id
 * @param array $datas liste de coutent types
 * @param array $context
 */
function _ctm_installation_disable_batch($id, $datas, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    if(ctm_installation_queue_batch_start($id, $context, $datas['modules'])) {
      return;
    }
    $context['sandbox']['profile'] = drupal_get_profile();
    $context['sandbox']['datas'] = $datas['modules'];
  }
  // on verifie si le module n est pas utilise par un autre et que ce n est pas une feature
  $parent = array_shift($context['sandbox']['datas']);
  if ($datas['feature']) {
    ctm_installation_queue_message($context, "Deactivating de $parent");
  }
  else {
    ctm_installation_queue_message($context, "try to deactivate $parent");
  }

  $module_data = system_rebuild_module_data();
  $active_modules = module_list();
  $desactivable = FALSE;
  if (isset($active_modules[$parent]) && (!isset($module_data[$parent]->info['features']) || $datas['feature'])) {
    $desactivable = TRUE;
    foreach ($module_data[$parent]->required_by as $dependent => $dependent_data) {
      if (isset($active_modules[$dependent]) || $dependent == $context['sandbox']['profile']) {
        $desactivable = FALSE;
        break;
      }
    }
  }
  if ($desactivable) {
    module_disable(array($parent));
  }
  ctm_installation_queue_batch_end($context, $id);
}

/**
 * callback de la queue pour enable (module et feature)
 * @param string $id
 * @param array $disable liste de coutent types
 * @param array $context
 */
function _ctm_installation_enable_batch($id, $disable, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    if(ctm_installation_queue_batch_start($id, $context, $disable)) {
      return;
    }
    $context['sandbox']['datas'] = $disable;
  }
  $module = array_shift($context['sandbox']['datas']);
  ctm_installation_queue_message($context, "Activating $module");
  module_enable(array($module));
  ctm_installation_queue_batch_end($context, $id);
}

/**
 * operation de fin de queue courante :
 * si il ne reste aucune operation valide a faire on clear cache sinon on ne fait
 * rien
 * @param type $id
 * @param type $toDelay
 * @param type $context
 */
function _ctm_installation_end_queue_batch($id, $toDelay, &$context) {
  if(!empty($toDelay)) {
    ctm_installation_del_elem_queue($id);
    return; // il reste les element delayed a executer
  }
  $toRelease = array();
  $executable = FALSE;
  $queue = ctm_installation_get_queue();
  while ($executable === FALSE && $item = $queue->claimItem(40)) { // pour chaque element dans la queue. le release se fait par le cron ou queue_ui si le batch plante
    $checker = $item->data['#checker'];
    $executable = $checker($item->data['#callback'], $item, $item->data['#datas']);
    $toRelease[] = $item;
  }
  foreach ($toRelease as $item) {
    $queue->releaseItem($item);
  }
  if(!$executable) {
    return _ctm_installation_cleanup_batch($id, array(), $context);
  }
  ctm_installation_del_elem_queue($id);
}

/**
 * callback de la queue pour vider le cache
 * @param void $empty
 * @param array $context
 */
function _ctm_installation_cleanup_batch($id, $empty, &$context) {
  ctm_installation_queue_message($context, 'Flushing cache');
  ctm_installation_drupal_flush_cache();
  ctm_installation_del_elem_queue($id);
}

/**
 * call all config_xxx.queue functions
 * @param type $id
 * @param type $data
 * @param type $context
 * @return type
 */
function _ctm_installation_call_config_queue_batch($id, $data, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['list'] = ctm_installation_call_config('queue', $data['profile'], $data['op'], -1); // -1 to dont execute the first function
    if(ctm_installation_queue_batch_start($id, $context, $context['sandbox']['list'])) {
      ctm_installation_queue_message($context, "no config.queue function");
      return;
    }
  }
  $message = 'Configuration of ' . str_replace('ctm_installation_config_', '', $context['sandbox']['list'][$context['sandbox']['progress']]);
  ctm_installation_queue_message($context, $message);
  ctm_installation_call_config('queue', $data['profile'], $data['op'], $context['sandbox']['progress']);
  ctm_installation_queue_batch_end($context, $id);
}

/**
 * Configure xautoload
 * @param type $id
 * @param type $empty
 * @param type $context
 */
function _ctm_installation_xautoload_batch($id, $empty, &$context) {
  ctm_installation_queue_message($context, "Configuring APC");
  $cache_status = array(
      'apcu_q' => ($apcu_status = (extension_loaded('apcu') && function_exists('apcu_store'))) ? 'apcu_q' : 0,
      'apc' => (extension_loaded('apc') && function_exists('apc_store')) ? 'apc' : 0,
      'apcu' => ($apcu_status) ? 'apcu' : 0,
      'wincache' => (extension_loaded('WinCache') && function_exists('wincache_ucache_get')) ? 'wincache' : 0,
      'xcache' => (extension_loaded('Xcache') && function_exists('xcache_get')) ? 'xcache' : 0,
      'dbcache' => 'dbcache',
  );
  variable_set(XAUTOLOAD_VARNAME_CACHE_TYPES, $cache_status);
  variable_set(XAUTOLOAD_VARNAME_CACHE_LAZY, 1);
  variable_set(XAUTOLOAD_VARNAME_REPLACE_CORE, 1);
  ctm_installation_del_elem_queue($id);
}

/**
 * Add role to users
 * @param string $id
 *    see queue checker
 * @param array $data
 *    ['uids'] => array (uid1,uid2...) : list of uid of users
 *    ['rid'] => rid of role to add
 * @param array $context
 *    see queue checker
 */
function _ctm_installation_add_role_batch($id, $data, &$context) {
  ctm_installation_queue_message($context, 'Add roles to user');
  user_multiple_role_edit($data['uids'], 'add_role', $data['rid']);
  ctm_installation_del_elem_queue($id);
}

/**
 * batch de suppression de nodes
 * @param string $id
 * @param array $cts liste de coutent types
 * @param array $context
 */
function _ctm_installation_delete_nodes_batch($id, $cts, &$context) {
  ctm_installation_queue_message($context, 'Cleaning nodes');
  $pas = 50; //Nombre de nodes a supprimer par passe
  if (!isset($context['sandbox']['progress'])) {
    $nids = db_select('node', 'n')
        ->fields('n', array('nid'))
        ->condition('n.type', $cts, 'IN')
        ->execute()
        ->fetchCol();
    $context['sandbox']['datas'] = array_chunk($nids, $pas);
    if(ctm_installation_queue_batch_start($id, $context, $context['sandbox']['datas'])) {
      return;
    }
  }
  $nids_index = $context['sandbox']['progress'];
  node_delete_multiple($context['sandbox']['datas'][$nids_index]);
  ctm_installation_queue_batch_end($context, $id);
}

/**
 * batch de suppression de terms
 * @param string $id
 * @param array $vids liste de coutent types
 * @param array $context
 */
function _ctm_installation_delete_terms_batch($id, $vids, &$context) {
  ctm_installation_queue_message($context, 'Cleaning terms');
  $pas = 50; //Nombre de nodes a supprimer par passe

  if (!isset($context['sandbox']['progress'])) {
    $nids = db_select('taxonomy_term_data', 'n')
        ->fields('n', array('tid'))
        ->condition('n.vid', $vids, 'IN')
        ->execute()
        ->fetchCol();
    $context['sandbox']['datas'] = $nids;
    if(ctm_installation_queue_batch_start($id, $context, $context['sandbox']['datas'])) {
      return;
    }
  }
  for ($i = 0; $i < $pas && $context['sandbox']['progress'] < $context['sandbox']['max']; $i++) {
    $tid = $context['sandbox']['datas'][$context['sandbox']['progress']];
    taxonomy_term_delete($tid);
    $context['sandbox']['progress'] ++;
  }
  $context['sandbox']['progress'] --;
  ctm_installation_queue_batch_end($context, $id);
}

/**
 * call a classic batch
 * @param type $id
 * @param type $info
 * @param type $context
 */
function _ctm_installation_process_batch($id, $info, &$context) {
  ctm_installation_queue_message($context, 'call '. $info['batch']);
  if(!is_array($info['param'])) {
    $info['param'] = array();
  }
  call_user_func_array($info['batch'], array_merge($info['param'], array(&$context)));
  if($context['finished'] >= 1) {
    ctm_installation_del_elem_queue($id);
  }
}

/**
 * call a user function
 * @param type $id
 * @param type $info
 * @param type $context
 */
function _ctm_installation_call_function_batch($id, $info, &$context) {
  ctm_installation_queue_message($context, 'call '. $info['function']);
  call_user_func_array($info['function'], $info['param']);
  ctm_installation_del_elem_queue($id);
}

/**
 *
 * @param type $id
 * @param type $datas
 * @param type $context
 * @return type
 */
function _ctm_installation_user_role_grant_permissions_batch($id, $datas, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    if(ctm_installation_queue_batch_start($id, $context, $datas['roles'])) {
      return;
    }
    ctm_installation_queue_message($context, "Set user permissions");
  }
  $rid = $datas['roles'][$context['sandbox']['progress']];
  user_role_grant_permissions($rid, $datas['perms']);
  ctm_installation_queue_batch_end($context, $id);
}

/**
 * batch de suppression de types de contenu
 * @param string $id
 * @param array $cts
 * @param array $context
 */
function _ctm_installation_delete_cts_batch($id, $cts, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['datas'] = $cts;
    if(ctm_installation_queue_batch_start($id, $context, $cts)) {
      return;
    }
  }

  $type = array_shift($context['sandbox']['datas']);
  ctm_installation_queue_message($context, "Delete conent type " . $type);
  node_type_delete($type);

  ctm_installation_queue_batch_end($context, $id);
}

/**
 * batch de suppression de vocabulaire
 * @param string $id
 * @param array $vids
 * @param array $context
 */
function _ctm_installation_delete_vocabulary_batch($id, $vids, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['datas'] = $vids;
    if(ctm_installation_queue_batch_start($id, $context, $vids)) {
      return;
    }
    ctm_installation_queue_message($context, 'Cleaning vocabularies');
  }

  $vid = array_shift($context['sandbox']['datas']);
  taxonomy_vocabulary_delete($vid);

  ctm_installation_queue_batch_end($context, $id);
}

/**
 * permet de positionner ou de modifier un bloc via la queue
 * @param string $id
 * @param array $values
 * @param array $context
 * @return void
 */
function _ctm_installation_insert_or_update_blocks_batch($id, $values, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    if(ctm_installation_queue_batch_start($id, $context, $values)) {
      return;
    }
    $context['sandbox']['datas'] = $values;
  }
  $bloc = array_shift($context['sandbox']['datas']);
  ctm_installation_queue_message($context, 'Create block ' . $bloc['delta']);
  ctm_installation_insert_or_update_blocks(array($bloc));
  ctm_installation_queue_batch_end($context, $id);
}

/**
 * permet de recontruire les droits d'acces via la queue
 * @param string $id
 * @param array $values
 * @param array $context
 */
function _ctm_installation_access_rebuild_batch($id, $values, &$context) {
  if (empty($context['sandbox'])) {
    ctm_installation_queue_message($context, 'Rebuild permissions');
    db_delete('node_access')->execute();
    if (count(module_implements('node_grants'))) {
      _node_access_rebuild_batch_operation($context);
    }
    else {
      // Not using any node_access modules. Add the default grant.
      db_insert('node_access')
          ->fields(array(
              'nid' => 0,
              'realm' => 'all',
              'gid' => 0,
              'grant_view' => 1,
              'grant_update' => 0,
              'grant_delete' => 0,
          ))
          ->execute();
    }
  }
  else {
    _node_access_rebuild_batch_operation($context);
  }
  if ($context['finished'] >= 1) {
    ctm_installation_del_elem_queue($id);
    node_access_needs_rebuild(FALSE);
  }
}

//------------------------------ API --------------------------------------

/**
 * cree ou modifie une rules pour cleaner une vue en fonction de la modification de content type
 * @param string $rules_name
 *      human readable name de la rules
 * @param array  $cts
 *      list des noms machine en cle des content type qui declanche la rules
 * @param string $view_name
 *      nom machine de la vue a clean
 */
function ctm_installation_queue_add_clean_view_rules($rules_name, $cts, $view_name) {
  $paths = array();
  $menu_names = array();
  foreach ($cts as $type) {
    $paths[] = 'node/add/' . $type;
    $menu_names[] = 'navigation';
  }
  $content = array(
      'rules_name' => $rules_name,
      'cts' => $cts,
      'view_name' => $view_name,
  );
  ctm_installation_add_to_queue('ctm_installation_add_clean_view_rules_batch', array('menu_paths' => $paths, 'menu_names' => $menu_names, 'datas' => $content), '', 'ctm_installation_check_urls');
}

/**
 * supprime une rules en queue
 * @param string $rules_code
 *        Nom machine de la vue correspondant a la rules a supprimer
 */
function ctm_installation_queue_remove_rules($rules_code) {
  ctm_installation_add_to_queue('ctm_installation_remove_rules_batch', $rules_code);
}

/**
 * desactive une serie de module a la condition qu il ne soit plus utile
 * desactive
 * @param array $modules
 *  liste des modules a desactive
 * @param bool $feature
 *  les modules a desactive sont ils des features ?
 */
function ctm_installation_queue_module_disable($modules, $feature = FALSE) {
  foreach ($modules as $module) { // permet d avoir les bon message dans la queue
    ctm_installation_add_to_queue('_ctm_installation_disable_batch', array('modules' => array($module), 'feature' => $feature));
  }
}

/**
 * active une serie de module
 * @param array $modules
 *  liste des modules a desactive
 */
function ctm_installation_queue_module_enable($modules) {
  foreach ($modules as $module) { // permet d avoir les bon message dans la queue
    ctm_installation_add_to_queue('_ctm_installation_enable_batch', array($module));
  }
}

/**
 * ajoute une rule menu position limite par content type ou url
 * si le $menu_path n'existe pas encore (cas des vue) la demande est mise en
 * queue pour traitement ulterieur (voir ctm_installation_feature_post_restore)
 * si la demande peut etre traite elle est retire de la file d attente
 * @param string $title nom machine de la rules a creer
 * @param string $menu_path chemin du menu dans lequel mettre la rules
 * @param string $menu_name  nom machine du menu dans lequel mettre la rules
 * *param string $type "pages" ou "content_type"
 * @param string $value  nom machine du content type limitant la rules ou liste des pages separer par \n
 */
function ctm_installation_queue_menu_position_add_rule($title, $menu_path, $menu_name, $type, $value) {
  $item = array($type => $value, 'menu_name' => $menu_name, 'title' => $title, 'type' => $type);
  ctm_installation_add_to_queue('ctm_installation_menu_position_add_rule_batch', array('menu_paths' => array($menu_path), 'menu_names' => array($menu_name), 'datas' => $item), '', 'ctm_installation_check_urls');
}

/**
 * supprime une regle menu position
 * @param type $title
 */
function ctm_installation_queue_remove_menu_position_rule($title) {
  ctm_installation_add_to_queue('ctm_installation_remove_menu_position_rule_batch', $title);
}

function ctm_installation_queue_import_po() {
 ctm_installation_add_to_queue('_ctm_installation_import_po');
}

/**
 * Permet d'ajouter un lien ou de mettre a jour un lien lie a une feature
 * @param array $paths
 *    liste des chemins qui doivent etre present avant de creer ou de modifier le lien
 *     Si il s'agit d'un tableau il peut contenir :
 *       - une cle "parent_path" :
 *         Alors le systeme ajoutera ce menu comme parent du menu a creer quand le parent existera
 *       - une cle "link_path" :
 *         Alors il s'agit d'une mise a jour de lien existant et on attend que
 *         le lien existe avant de le modifier.
 *       - une cle "router_path" :
 *         dans ce cas le systeme crera un lien de menu ou "link_path" = "router_path"
 *         lorsque le router "router" existera et correspondra au router defini pour "router_path"
 *       - une cle "node_uuid" alors on cherche un node qui a cette uuid pour l'associer au menu
 * @param array $menus
 *    Contient le nom des menu correspondant a $paths. $menu doit
 *    contenir le meme nombres d'element et les meme cle
 * @param array $item
 *    un tableau contenant les element du menu (voir menu_item_save()). Si il
 *    s'agit d'une mise a jour, seul les elements a modifier sont necessaires.
 */
function ctm_installation_queue_menu_link_save($paths, $menus, $item) {
  ctm_installation_add_to_queue('_ctm_installation_menu_link_save_batch', array('menu_paths' => $paths, 'menu_names' => $menus, 'datas' => $item), '', 'ctm_installation_check_urls');
}

/**
 * Add a roles to a list of users
 * @param array $uids
 *     array of uid of users
 * @param int $rid
 *    rid od the role to add
 */
function ctm_installation_queue_add_role($uids, $rid) {
  ctm_installation_add_to_queue('_ctm_installation_add_role_batch', array('uids' => $uids, 'rid' => $rid));
}

/**
 * Permet d'ajouter un menu block qui est lie a un item de menu parent dont on connais le chemin
 * @param array $values
 *    une liste de array(blocks=>array, menu=>  array); (voir ctm_installation_insert_or_update_menu_block)
 * @param array $paths
 *    les chemins de l'item de menu parent
 * @param array $menus
 *    les noms des menus
 */
function ctm_installation_queue_insert_or_update_menu_block($values, $paths, $menus) {
  ctm_installation_add_to_queue('_ctm_installation_menu_block_batch', array('menu_paths' => $paths, 'menu_names' => $menus, 'datas' => $values), '', 'ctm_installation_check_urls');
}

/**
 * Ajoute la variable menu_parent_[content_type] si elle pointe vers une url cree par une feature
 * @param string $ct
 *              Nom machine du content type qui doit se placer dans ce menu parent
 * @param string $menu_path
 *              chemin du parent
 * @param string $menu_name
 *              menu du parent
 */
function ctm_installation_queue_set_menu_parent($ct, $menu_path, $menu_name) {
  ctm_installation_add_to_queue('_ctm_installation_set_menu_parent_batch', array('menu_paths' => array($menu_path), 'menu_names' => array($menu_name), 'datas' => array('menu_name' => $menu_name, 'ct' => $ct)), '', 'ctm_installation_check_urls');
}

/**
 * supprime une liste de content types et leurs nodes via la queue
 * @param array $cts liste de noms de content_type
 */
function ctm_installation_queue_cts_and_nodes_delete($cts) {
  ctm_installation_add_to_queue('_ctm_installation_delete_nodes_batch', $cts);
  ctm_installation_add_to_queue('_ctm_installation_delete_cts_batch', $cts);
}

/**
 * supprime une liste de vocabulaire via la queue
 * @param array $voc liste des nom machine des vocabulaires
 */
function ctm_installation_queue_vocabulary_delete($voc) {
  $vids = array();
  foreach ($voc as $name) {
    $vocab = taxonomy_vocabulary_machine_name_load($name);
    $vids[] = $vocab->vid;
  }
  ctm_installation_add_to_queue('_ctm_installation_delete_terms_batch', $vids);
  ctm_installation_add_to_queue('_ctm_installation_delete_vocabulary_batch', $vids);
}

/**
 * insert or update de bloc en queue (pour eviter les times out)
 * @param array $values
 */
function ctm_installation_queue_insert_or_update_blocks($values) {
  ctm_installation_add_to_queue('_ctm_installation_insert_or_update_blocks_batch', $values);
}

/**
 * permet de reconstruire les droits d'acces via la queue
 */
function ctm_installation_queue_access_rebuild() {
  ctm_installation_add_to_queue('_ctm_installation_access_rebuild_batch');
}

/**
 * permet d'ajouter des liens en queue, suivant le format de fichier features.menu_links.inc
 * ce format peut etre modifier en ajoutant un champs "parent_path" a la place du champs "parent_identifier"
 * On peut egalement supprimer "menu_path" et le remplacer par "node_uuid".
 * La queue attendra que le noeud avec cet uuid exist et que le parent avec ce path existe.
 * @param array $menu_links
 *   Une liste d'items de menu
 */
function ctm_installation_queue_menu_link_features($menu_links) {
  foreach($menu_links as $item) {
    $path = array();
    $menu = array();
    if(isset($item['node_uuid'])) {
      $path['node_uuid'] = $item['node_uuid'];
      $menu['node_uuid'] = $item['menu_name'];
      unset($item['node_uuid']);
    }
    if(isset($item['parent_path'])) {
      $path['parent_path'] = $item['parent_path'];
      $menu['parent_path'] = $item['menu_name'];
      unset($item['parent_path']);
    }
    if(empty($path)) {
      $path['router_path'] = $item['router_path'];
      $menu['router_path'] = $item['menu_name'];
    }
    ctm_installation_queue_menu_link_save($path, $menu, $item);
  }
}

/**
 * permet d'ajouter un clear cache en milieu de queue
 */
function ctm_installation_queue_cache_clear() {
  ctm_installation_add_to_queue('_ctm_installation_cleanup_batch');
}

/**
 * permet d'ajouter des taches une fois que la queue est vide
 */
function ctm_installation_queue_end_config($profile, $op) {
  ctm_installation_add_to_queue('_ctm_installation_call_config_queue_batch', array('profile'=>$profile,'op'=>$op), '', 'ctm_installation_check_empty_queue');
}

/**
 * genere les alias en masse selon le type
 * @param array $types un ou plusieurs element de la liste suivante :
 *  - "node"
 *  - "term"
 * ou tout element de la liste : module_invoke_all('pathauto', 'settings')
 * si vide, tous les alias sont cree
 */
function ctm_installation_queue_generate_alias($types = array()) {
  $list = module_invoke_all('pathauto', 'settings');
  if(!empty($types)) {
    foreach($list as $key => $value) {
      if(!in_array($value->token_type, $types)) {
        unset($list[$key]);
      }
    }
  }
  foreach($list as $value) {
    $callback = $value->batch_update_callback;
    $file = $value->batch_file;
    ctm_installation_add_to_queue('_ctm_installation_process_batch',array('batch' => $callback), $file);
  }
}

/**
 * to embed a normal batch operation in the ctm installation queue
 * @param type $batch
 *  function to call (operation)
 * @param array $param
 *  list of param values of the function
 * @param string $file
 *  file to include to call the function
 */
function ctm_installation_queue_add_batch($batch, $param = array(), $file = '') {
  ctm_installation_add_to_queue('_ctm_installation_process_batch', array('batch' => $batch,'param' => $param),$file);
}

/**
 * call a normal function in queue
 * @param string $function
 *  function to call
 * @param array $param
 *  list of param values of the function
 * @param string $file
 *  file to include to call the function
 */
function ctm_installation_queue_call_function($function, $param = array(), $file = '') {
  ctm_installation_add_to_queue('_ctm_installation_call_function_batch', array('function' => $function,'param' => $param), $file);
}
/**
 * regenerate xmlsitemap
 * @param type $id
 * @param type $rebuild_types
 * @param type $context
 */
function ctm_installation_queue_xmlsitemap_rebuild() {
    variable_set('xmlsitemap_rebuild_needed', TRUE);
    variable_set('xmlsitemap_regenerate_needed', TRUE);
    $path = drupal_get_path('module', 'xmlsitemap').'/xmlsitemap.generate.inc';
    ctm_installation_queue_add_batch('xmlsitemap_rebuild_batch_clear', array(array('menu_link'), TRUE), $path);
    ctm_installation_queue_add_batch('xmlsitemap_rebuild_batch_fetch',array('menu_link'), $path);
    ctm_installation_queue_call_function('variable_set',array('xmlsitemap_rebuild_needed', FALSE));
    ctm_installation_queue_call_function('_xmlsitemap_regenerate_before', array(), $path);
    $smids = db_query("SELECT smid FROM {xmlsitemap_sitemap}")->fetchCol();
    foreach ($smids as $smid) {
      ctm_installation_queue_add_batch('xmlsitemap_regenerate_batch_generate',array($smid),$path);
      ctm_installation_queue_add_batch('xmlsitemap_regenerate_batch_generate_index',array($smid),$path);
    }
    ctm_installation_queue_call_function('variable_set',array('xmlsitemap_regenerate_needed', FALSE));
}

/**
 * equivalent to user_role_grant_permissions but in queue. And the first
 * parameter accept an list of rid (array) instead of anly one rid.
 * permission will be granted only when the permissions exists
 * @param array $roles
 *  list of rid
 * @param array $perms
 *  list of permissions
 */
function ctm_installation_queue_user_role_grant_permissions($roles, $perms) {
  ctm_installation_add_to_queue('_ctm_installation_user_role_grant_permissions_batch', array('roles' => $roles,'perms' => $perms), '', 'ctm_installation_check_user_permissions');
}