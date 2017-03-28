<?php

/**
 * Add the group_group field to a content type. the content type become a group
 * @param type $ct
 */
function ctm_installation_insert_or_update_field_og_group_group($ct) {
  $og_field = og_fields_info(OG_GROUP_FIELD);
  ctm_installation_insert_or_update_field($og_field['field']);
  
  $instance = $og_field['instance'];
  $instance += array(
    'field_name' => OG_GROUP_FIELD,
    'bundle' => $ct,
    'entity_type' => 'node',
    'label' => 'group',
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
  );
  ctm_installation_insert_or_update_field_instance($instance);
  og_invalidate_cache();
  entity_property_info_cache_clear();
}
/**
 * Add the group_audience field to a content type. the content type become a group
 * @param string $ct
 * @param string $label
 * @param array $ct_ref
 * @param int $cardinality
 * @param array $view_mode
 * @param array $display
 * @param string $id
 */
function ctm_installation_insert_or_update_field_og_group_audience($ct, $label, 
    $ct_ref, $required = FALSE, $cardinality = -1, $view_mode = NULL, 
    $display = NULL, $id = OG_AUDIENCE_FIELD) {
  
  $og_field = og_fields_info(OG_AUDIENCE_FIELD);
  $og_field['field']['field_name'] = $id;
  $og_field['field']['cardinality'] = $cardinality;
  $og_field['field']['settings']['handler_settings']['target_bundles'] = $ct_ref;
  ctm_installation_insert_or_update_field($og_field['field']);
  
  if($display == NULL) {
    $display = array(
      'default' => array(
        'type' => 'og_list_default',
        'label' => 'above',
        'settings' => array(),
        'module' => 'og_ui',
        'weight' => 2,
      )
    );
  }
  if($view_mode == NULL) {
    $view_mode = array(
      'full' => array(
        'label' => 'Full',
        'type' => 'og_list_default',
        'custom settings' => FALSE,
      ),
      'teaser' => array(
        'label' => 'Teaser',
        'type' => 'og_list_default',
        'custom settings' => FALSE,
      ),
    );
  }
  $instance = $og_field['instance'];
  $instance += array(
    'field_name' => $id,
    'bundle' => $ct,
    'entity_type' => 'node',
    'description' => '',
    'default_value' => NULL,
    'label' => $label,
    'required' => $required,
    'view modes' => $view_mode,
    'display' => $display,
    'default_value' => array(
      0 => array(
        'value' => 1,
      ),
    ),
  );
  ctm_installation_insert_or_update_field_instance($instance);
  og_invalidate_cache();
  entity_property_info_cache_clear();
}