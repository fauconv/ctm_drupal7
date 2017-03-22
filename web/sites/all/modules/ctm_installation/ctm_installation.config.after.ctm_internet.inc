<?php

/**
 * @file
 * fonctions specifique a l'internet execute apres le formulaire d'installation
 * format des fonction ici :
 * ctm_installation_config_XXXXXX_after_ctm_internet($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 */

/**
 * config xmlsitemap
 */
function ctm_installation_config_xmlsitemap_after_ctm_internet($profile, $roles, $op = 'update') {
  global $base_path;
  $base_root_list = variable_get('base_root_list');
  $base_root_public = reset($base_root_list);
  $base_url_public = $base_root_public . substr($base_path, 0, -1);
  variable_set('xmlsitemap_settings_menu_link_main-menu', array('status' => 1, 'priority' => 1));
  variable_set('xmlsitemap_rebuild_needed', TRUE);
  variable_set('xmlsitemap_regenerate_needed', TRUE);
  variable_set('xmlsitemap_base_url', $base_url_public);
  variable_set('xmlsitemap_prefetch_aliases', FALSE);
}
