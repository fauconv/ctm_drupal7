<?php

require_once dirname(__FILE__) . DIRECTORY_SEPARATOR . '..' . DIRECTORY_SEPARATOR . 'ctm_commun' . DIRECTORY_SEPARATOR . 'ctm_commun.profile.inc';

/**
 * Implements hook_form_FORM_ID_alter().
 * Allows the profile to alter the site configuration form.
 */
function ctm_internet_form_install_configure_form_alter(&$form, $form_state) {
  ctm_commun_form_install_configure_form_alter($form, $form_state);
}

/**
 * Implements hook_install_tasks_alter().
 */
function ctm_internet_install_tasks_alter(&$tasks, $install_state) {
  ctm_commun_install_tasks_alter($tasks, $install_state);
}
