<?php

/**
 * languages must be already set
 * @param type $profile
 * @param type $roles
 * @param type $op
 */
function ctm_installation_config_multi_lang_xmlsitemap_ctm_internet($profile, $roles, $op = 'update') {
  module_enable(array('xmlsitemap_i18n'));
}
