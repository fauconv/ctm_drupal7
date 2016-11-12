<?php

/**
 * @file
 * pages d'administration de ctm_installation :
 * -release notes
 * -installation
 */

/**
 * Menu callback; Enable/disable developer modules.
 */
function ctm_installation_toggle_modules() {
  if (!user_access('administer modules')) {
    return MENU_ACCESS_DENIED;
  }
  $saved_state = variable_get('ctm_installation_devel_modules_enabled', NULL);
  ctm_installation_tools('devel', !empty($saved_state));
  if (isset($saved_state)) {
    drupal_set_message(t('Developer modules enabled'));
  }
  else {
    drupal_set_message(t('All developer modules disabled'));
  }
  drupal_goto();
}

/**
 * formulaire de reinstall des fonctions
 */
function ctm_installation_installation_form($form, &$form_state) {
  global $ctm_installation_dir;

  $fct_list = array();

  $profile = drupal_get_profile();
  ctm_installation_load_config_files(array('before','normal','after','queue'), $profile);

  $var = get_defined_functions();
  foreach ($var['user'] as $func) {
    if (substr($func, 0, 24) == 'ctm_installation_config_') {
      $fct_list[$func] = substr($func, 24);
    }
  }
  unset($var);
  ksort($fct_list);
  $fct_list = array('revert' => 'revert') + $fct_list;
  $form['fonction'] = array(
      '#type' => 'select',
      '#title' => 'Function to call',
      '#options' => $fct_list,
      '#default_value' => '',
  );
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Executer',
  );
  return $form;
}

/**
 * formulaire d'admin installation
 */
function ctm_installation_installation_form_submit($form, &$form_state) {
  global $ctm_installation_dir;
  $profile = drupal_get_profile();
  if (!empty($form_state['values']['fonction'])) {
    ctm_installation_load_config_files(array('before','normal','after','queue'), $profile);
    $roles = ctm_installation_user_roles();
    $function = $form_state['values']['fonction'];
    if ($function == 'revert') {
      ctm_installation_revert();
    }
    elseif (substr($function, 0, 27) != 'ctm_installation_config_all') {
      $function($profile, $roles);
    } else {
      $function($profile);
    }
    drupal_set_message(t('function call done'));
  }
}
