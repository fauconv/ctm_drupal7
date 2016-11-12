<?php

/**
 * generate xmlsitemap
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_xmlsitemap_queue($profile, &$roles, $op = 'update') {
  ctm_installation_delete_alias_all();
  ctm_installation_generate_static_alias();
  ctm_installation_queue_generate_alias(array('node','term'));
}
