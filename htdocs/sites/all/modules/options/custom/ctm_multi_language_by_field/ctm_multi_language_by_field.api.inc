<?php

//============================== API ======================================
/**
 * set a content type as translatable by field
 * @param string $ct
 *   the content type machine name
 * @param bool $title
 *   true if you want to make the title translatable
 */
function ctm_installation_set_multi_language_ct($ct, $title = FALSE) {
  variable_set('entity_translation_hide_translation_links_' . $ct, 1);
  variable_set('entity_translation_node_metadata_' . $ct, 1);
  variable_set('entity_translation_settings_node__' . $ct, array(
      'default_language' => 'xx-et-default',
      'hide_language_selector' => 1,
      'exclude_language_none' => 1,
      'lock_language' => 1,
      'shared_fields_original_only' => 0,
  ));
  variable_set('language_content_type_' . $ct, I18N_LANGUAGE_EXTENDED);
  if ($title) {
    title_field_replacement_toggle('node', $ct, 'title');
    ctm_installation_set_multi_language_field('title_field');
  }
}

/**
 * set a field as translatable.
 * The content type must be translatrable by field
 * @param string $field_name
 *   the field machine name
 */
function ctm_installation_set_multi_language_field($field_name) {
  $info = field_info_field($field_name);
  $info['translatable'] = 1;
  field_update_field($info);
}

/**
 * provide internat url referer
 * @return string
 */
function ctm_installation_referer_path() {
  if (!empty($_SERVER['HTTP_REFERER'])) {
    $url = parse_url($_SERVER['HTTP_REFERER']);
    if ($url['path'][0] == '/') {
      $url['path'] = substr($url['path'], 1);
    }
    list(, $path) = language_url_split_prefix($url['path'], language_list());
    return $path;
  }
  return '<front>';
}
