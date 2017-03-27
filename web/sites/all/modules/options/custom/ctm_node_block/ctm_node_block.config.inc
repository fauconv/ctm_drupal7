<?php

/**
 * @file
 * librairie de fonction pour l'installation des profils etat et du socle du master etat
 * commune internet intranet
 */

/**
 * Ajoute le content type bloc
 */
function ctm_installation_config_content_type_block($profile, $roles, $op = 'update') {
  $type = 'block';
  $field_name = 'texte_block';
  $content_type_bundle = array(
      'name' => 'Block',
      'description' => 'to create block',
      'title_label' => 'Title',
      'type' => $type,
      'module' => 'ctm_installation',
      'base' => 'node_content',
      'locked' => 1,
      'custom' => 0,
  );
  ctm_installation_insert_or_update_content_type($content_type_bundle);
  $field_texte_page_base = array(
      'field_name' => $field_name,
      'entity_types' => array('node'),
      'type' => 'text_long',
  );
  ctm_installation_insert_or_update_field($field_texte_page_base);
  $largeurs = array_keys(variable_get('ckeditor_sizes', array('100' => 'p;h2;h3;h4;h5;h6;address;div')));
  reset($largeurs);
  $format_name = 'ckeditor_images_liens_' . key($largeurs);
  $instances_texte_page_base = array(
      'field_name' => $field_name,
      'entity_type' => 'node',
      'bundle' => $type,
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
                  $format_name => $format_name,
              ),
              'default_order_toggle' => 0,
          ),
          'user_register_form' => FALSE,
      ),
      'widget' => array('type' => 'text_textarea'),
  );
  ctm_installation_insert_or_update_field_instance($instances_texte_page_base);

  //Config du contenu
  variable_set('nodeblock_block', '1');
  variable_set('nodeblock_node_overrides_block', array('0' => 'block_title', 'translation_fallback'));

  ctm_installation_set_default_block_datas($type, $roles, $profile, array($field_name));
}

/**
 * Ajoute la rule page manager pour empecher la visualisation d un contenu
 * de type bloc_header et/ou bloc
 */
function ctm_installation_config_rules_page_manager($profile, $roles, $op = 'update') {
  variable_set('page_manager_node_view_disabled', FALSE);
  //Donnees
  $handler = new stdClass();
  $handler->disabled = FALSE;
  $handler->api_version = 1;
  $handler->name = 'node_view_http_response';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'http_response';
  $handler->weight = 0;
  $handler->conf = array(
      'title' => 'hide content of block type',
      'contexts' => array(),
      'relationships' => array(),
      'code' => '301',
      'destination' => '<front>',
      'access' => array(
          'plugins' => array(
              0 => array(
                  'name' => 'node_type',
                  'settings' => array(
                      'type' => array(
                          'block' => 'block',
                      ),
                  ),
                  'context' => 'argument_entity_id:node_1',
                  'not' => FALSE,
              ),
          ),
          'logic' => 'and',
      ),
  );
  //Ajoute les configurations dans la base de donnee et cree les configuration
  $old = page_manager_load_task_handler(
      page_manager_get_task($handler->task), $handler->subtask, $handler->name);
  if (!empty($old)) {
    $handler->did = $old->did;
  }
  page_manager_save_task_handler($handler);
}
