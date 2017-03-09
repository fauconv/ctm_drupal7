<?php

/**
 * generate alias
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_alias_queue($profile, &$roles, $op = 'update') {
  ctm_installation_queue_xmlsitemap_rebuild();
}
