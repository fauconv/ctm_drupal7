<?php

/**
 * @file
 * Contains page callbacks for publishcontent
 */

/**
 * Administration settings form.
 */
function publishcontent_config_form($form, &$form_state) {

  $form['publishcontent_method'] = array(
    '#type' => 'radios',
    '#title' => t('Quick publish method'),
    '#default_value' => variable_get('publishcontent_method', PUBLISHCONTENT_METHOD_TABS),
    '#description' => t('Choose the quick links method. With no quick links, the published checkbox will still appear on the node edit form. Note that a Drupal cache clear is required after changing this.'),
    '#options' => array(
      PUBLISHCONTENT_METHOD_NONE => t('None.'),
      PUBLISHCONTENT_METHOD_ACTION_LINKS => t('Action links on node view.'),
      PUBLISHCONTENT_METHOD_BUTTON => t('Button.'),
      PUBLISHCONTENT_METHOD_TABS => t('Tabs.'),
    ),
  );
  $form['publishcontent_publish_on_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add publish button on node create page.'),
    '#description' => t('If checked, add the publish button on node create page. If not checked node must be saved before publishing like with "Tabs" method'),
    '#default_value' => variable_get('publishcontent_publish_on_create', FALSE),
    '#states' => array(
      // Only show this field when the value of publishcontent_method is PUBLISHCONTENT_METHOD_BUTTON
      'invisible' => array(
        array(
          array(':input[name="publishcontent_method"]' => array('value' => PUBLISHCONTENT_METHOD_NONE)),
          array(':input[name="publishcontent_method"]' => array('value' => PUBLISHCONTENT_METHOD_ACTION_LINKS)),
          array(':input[name="publishcontent_method"]' => array('value' => PUBLISHCONTENT_METHOD_TABS)),
        )
      ),
    ),
  );
  return system_settings_form($form);
}
