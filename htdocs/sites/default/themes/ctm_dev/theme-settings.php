<?php
/*
 * @file
 * Formulaire de configuration du theme
 */

function ctm_dev_form_system_theme_settings_alter(&$form, &$form_state) {

  $form['settings_ctm_admin'] = array(
      '#type' => 'fieldset',
      '#title' => 'Advenced configuration',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
  );

  $form['settings_ctm_admin']['fil_d_ariane'] = array(
      '#type' => 'textarea',
      '#title' => "BreadCrumb base",
      '#rows' => 2,
      '#default_value' => theme_get_setting('fil_d_ariane')
  );

}
