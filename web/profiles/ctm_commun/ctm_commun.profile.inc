<?php
/**
 * @file
 * Fichier regroupant toutes les parties commune entre le profil ctm_internet et ctm_intranet pour le fichier .profil
 */

/**
 * Implements hook_form_FORM_ID_alter().
 * Allows the profile to alter the site configuration form.
 */
function ctm_commun_form_install_configure_form_alter(&$form, $form_state) {
  global $base_url, $base_root;
  //Préremplie le nom du site avec le nom de l alias
  $form['site_information']['site_name']['#default_value'] = basename($base_url);
  $form['site_information']['site_mail']['#default_value'] = 'no-reply@'.basename($base_root);

  //Definit les valeurs par defaut pour le pays et le fuseau horaire
  $form['server_settings']['site_default_country']['#default_value'] = 'CH';
  $form['server_settings']['date_default_timezone']['#default_value'] = 'Europe/Zurich';
  unset($form['server_settings']['date_default_timezone']['#attributes']); // pour que les 2 lignes ci-dessus soit prise en compte

  //Desactive l'envoi des notification de mise a jour par mail
  $form['update_notifications']['update_status_module']['#default_value'] = array(0);

  $form['admin_account']['account']['name']['#default_value'] = 'developer';
  $form['admin_account']['account']['mail']['#default_value'] = 'webmaster@'.basename($base_root);
}

/**
 * Implements hook_install_tasks_alter().
 */
function ctm_commun_install_tasks_alter(&$tasks, $install_state) {

  $tasksTmp = array();

  foreach ($tasks as $key => $valueTask) {

    $tasksTmp[$key] = $valueTask;

    if ($key == 'install_verify_requirements') {
      $tasksTmp['ctm_commun_requirements'] =  array('display_name' => st('Verify Infrastructure'));
    }
    elseif ($key == 'install_configure_form') {
      $tasksTmp['ctm_commun_config_after_form'] =  array('display_name' => st('Custom configuration'));
    }
  }
  $tasks = $tasksTmp;
}

/**
 * lance les configuration qui doivent etre faites apres le formulaire de configuration du site
 * et active le bon systemùe d authentification
 */
function ctm_commun_config_after_form($install_state) {
  ctm_installation_config_all_after($install_state['parameters']['profile'],'install');
}

/**
 * Implementation of hook_requirements
 */
function ctm_commun_requirements(&$install_state) {
  $failures = array();

  $filehtdocs = explode(DIRECTORY_SEPARATOR, (dirname(__FILE__)));
  $longueur = count($filehtdocs);

  unset($filehtdocs[$longueur -1]);
  unset($filehtdocs[$longueur -2]);
  $drupal_root = implode('/', $filehtdocs);
  unset($filehtdocs[$longueur -3]);
  $basePath = implode('/', $filehtdocs);

  if (!is_dir($basePath . DIRECTORY_SEPARATOR . 'config')) {
    $failures[] = '<b>' . st('directory not found') . ' :</b> ' . st("config\n\n");
  }
  if (!is_dir($drupal_root . DIRECTORY_SEPARATOR . 'media')) {
    $failures[] = '<b>' . st('directory not found') . ' :</b> ' . st("media\n\n");
  }
  if (!empty($failures)) {
    throw new Exception(implode("\n\n", $failures));
  }

}
