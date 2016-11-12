<?php

/**
 * @file
 * Page d'administration du mock permettant de changer le mode
 */

/**
 * Create the admin configuration form.
 */
function ctm_mock_config_form($form, &$form_state) {
  $form['ctm_mock_activation'] = array(
      '#title' => t('Authentication System'),
      '#type' => 'select',
      '#options' => ctm_mock_get_modes(),
      '#default_value' => variable_get('ctm_mock_activation', CTM_MOCK_NO),
  );
  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Enregistrer'),
  );
  return $form;
}

/**
 * form submit
 */
function ctm_mock_config_form_submit($form, &$form_state) {
  ctm_mock_activate($form_state['values']['ctm_mock_activation']);
  drupal_set_message(t('informations have been updated'));
}
