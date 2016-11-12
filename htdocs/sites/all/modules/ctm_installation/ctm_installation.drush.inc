<?php
/**
 * @file
 * ctm_installation module drush integration.
 */

/**
 * Implements hook_drush_command().
 *
 * @return
 *   An associative array describing your command(s).
 *
 * @see drush_parse_command()
 */
function ctm_installation_drush_command() {
  $items = array();
  $items['ctm-installation-revert'] = array(
      'description' => "Equivalent to a feature revert but for ctm_installation",
      'aliases' => array('ctm_revert'),
  );
  $items['ctm-installation-queue-process'] = array(
      'description' => "launch the queue process",
      'aliases' => array('ctm_queue'),
  );
  $items['ctm-installation-mise-a-jour'] = array(
      'description' => "Update the site : combine drush updb + drush ctm_revert + drush fra + drush ctm_queue and more",
      'aliases' => array('ctm_update'),
  );
  $items['ctm-installation-config'] = array(
      'description' => "Launch a configuration function from ctm_installation individualy",
      'aliases' => array('ctm_config'),
      'arguments' => array(
          'function' => 'the name of the function without "ctm_installation_config_"',
      ),
  );
  $items['ctm-installation-tools'] = array(
      'description' => "Enable [Disable] ctm tools",
      'aliases' => array('ctm_tools'),
      'options' => array(
          'dis' => 'Disable ctm tools',
      ),
      'arguments' => array(
          'tools' => 'the type of tools you want to enable or disable. can be : "seo", "devel", "review"',
      ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function ctm_installation_drush_help($section) {
  switch($section) {
    case 'drush:ctm-installation-revert':
      return "Equivalent a un revert de feature mais pour ctm_installation. permet de rejouer l installation du site sans perte. Peut etre fait dans l admin via configuration->developpement->etat de geneve->installation->revert";
    case 'drush:ctm-installation-queue-process':
      return "Execute le contenu de la queue et evite l'apparition d'une barre de progression a la fin d'une installation ou d'une mise a jour.";
    case 'drush:ctm-installation-mise-a-jour':
      return "Effectue une mise a jour du master vers la derniere version";
    case 'drush:ctm-installation-dis-dev':
      return "Disable all developer modules";
    case 'drush:ctm-installation-en-dev':
      return "Enable all default developer modules";
    case 'drush:entitycache-load':
      return dt('Used without parameters, this command loads all the various content in the entities into cache using entity_load()');
  }
}

/**
 * Enable/Disable seo tools
 */
function drush_ctm_installation_tools($tool) {
  global $base_url;
  if (drush_get_option('dis', FALSE)) {
    echo "disabling '.$tool.' tools....$base_url\n";
    ctm_installation_tools($tool, FALSE);
    ctm_installation_process_queue();
    drush_log($tool.' tools disabled....', 'success');
  }
  else {
    echo "enabling '.$tool.' tools....$base_url\n";
    ctm_installation_tools($tool, TRUE);
    ctm_installation_process_queue();
    drush_log($tool.' tools enabled....', 'success');
  }
}

/**
 * launch an individual ctm_installation_config_* function
 * @global string $base_url
 * @param string $function
 */
function drush_ctm_installation_config($function) {
  global $base_url;
  $profile = drupal_get_profile();
  $roles = ctm_installation_user_roles();
  ctm_installation_load_config_files(array('before','normal','after','queue'), $profile);
  switch ($function) {
    case 'revert' :
      echo "Processing ctm_installation revert....$base_url\n";
      ctm_installation_revert();
      drush_log('ctm_installation reverted....', 'success');
      break;
    case 'all' :
    case 'all_after' :
      echo "Processing ctm_installation all....$base_url\n";
      $function = 'ctm_installation_config_' . $function;
      $function($profile);
      drush_log('Process finished....', 'success');
    default :
      $function = 'ctm_installation_config_' . $function;
      if (function_exists($function)) {
        echo "Processing $function....$base_url\n";
        $function($profile, $roles);
        drush_log('Process finished....', 'success');
      }
      else {
        drush_log('function: ' . $function . ' not exist', 'error');
      }
  }
}

/**
 * fait un "revert de ctm_installation comme si c'etait une feature
 */
function drush_ctm_installation_revert() {
  global $base_url;
  echo "Processing ctm_installation revert....$base_url\n";
  ctm_installation_revert();
  ctm_installation_process_queue();
  drush_log('ctm_installation reverted....', 'success');
}

/**
 * execute le contenu de la queue
 */
function drush_ctm_installation_queue_process() {
  global $base_url;
  echo "Processing queue....$base_url\n";
  ctm_installation_process_queue();
  drush_log('Queue processed', 'success');

}

/**
 * execute l ensemble des operation de mise a jour
 */
function drush_ctm_installation_mise_a_jour() {
  global $base_url;
  echo "Updating site.... $base_url\n";
  drush_invoke('updb'); // update.php
  drush_invoke('ctm_revert'); // revert de ctm_installation : replay all config function
  drush_invoke('fra'); // revert complet de toute les feature
  drush_invoke('ctm_queue'); // execution de la queue
  drush_invoke('cr'); // execution du cron
  drush_log('site updated', 'success');
}
