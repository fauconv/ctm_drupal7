<?php


 /*
 * Formulaire de configuration du theme custom
 *
 * Configuration des bandeaux, de la page de contact ainsi que la liste des domaines internes au site
 */
function ctm_back_form_system_theme_settings_alter(&$form, &$form_state) {

  // Determine les chemins personalises pour les differents icones en haut a droite
  $form['settings_ctm_back'] = array(
    '#type' => 'fieldset',
    '#title' => t('Administration avancee du site'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['settings_ctm_back']['bloc_node_width_limit'] = array(
    '#type' => 'textfield',
    '#title' => t("bloc_node_width_limit"),
    '#default_value' => theme_get_setting('bloc_node_width_limit')
  );
}
