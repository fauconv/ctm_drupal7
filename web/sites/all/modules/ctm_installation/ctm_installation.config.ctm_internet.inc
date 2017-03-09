<?php

/**
 * @file
 * fonctions specifique a l'internet
 * format des fonction ici :
 * ctm_installation_config_XXXXXX_ctm_internet($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 */
/**
 * Ajout des configuration pour le module boost
 */
/*
  function ctm_installation_config_boost_ctm_internet($profile, $roles, $op = 'update') {
  variable_set('boost_lifetime_max_text/html', 86400);
  variable_set('boost_enabled_text/xml', 1);
  variable_set('boost_lifetime_max_text/xml', 86400);
  variable_set('boost_enabled_application/xml', 1);
  variable_set('boost_lifetime_max_application/xml', 86400);
  variable_set('boost_enabled_application/rss', 1);
  variable_set('boost_lifetime_max_application/rss', 86400);
  variable_set('boost_enabled_application/rss+xml', 1);
  variable_set('boost_lifetime_max_application/rss+xml', 86400);
  variable_set('boost_root_cache_dir', '../media/cache');
  variable_set('boost_normal_dir', 'normal');

  user_role_grant_permissions($roles[ADMIN_SITE], array('boost flush pages'));
  user_role_grant_permissions($roles[MODERATEUR], array('boost flush pages'));
  }
 */

/**
 * Ajout des configuration pour le module expire
 */
/*
  function ctm_installation_config_expire_alias_ctm_internet($profile, $roles, $op = 'update') {
  variable_set('expire_alias_domainlist', ctm_installation_get_base_url_str());
  }
 */

function ctm_installation_config_eu_cookie_ctm_internet($profile, $roles, $op = 'update') {
  $largeurs = variable_get('ckeditor_sizes', array('100' => 'p;h2;h3;h4;h5;h6;address;div')); // largeur du bloc ckeditor
  $config = Array(
      "popup_enabled" => 1,
      "popup_clicking_confirmation" => 1,
      "popup_position" => 0,
      "popup_agree_button_message" => "OK, I agree",
      "popup_disagree_button_message" => "No, give me more info",
      "popup_info" => Array(
          "value" => "<h2>We use cookies on this site to enhance your user experience</h2><p>By clicking any link on this page you are giving your consent for us to set cookies.</p>",
          "format" => "ckeditor_" . key($largeurs),
      ),
      "popup_agreed_enabled" => 0,
      "popup_hide_agreed" => 0,
      "popup_find_more_button_message" => "More info",
      "popup_hide_button_message" => "Hide",
      "popup_agreed" => Array(
          "value" => "<h2>Thank you for accepting cookies</h2><p>You can now hide this message or find out more about cookies.</p>",
          "format" => "ckeditor_" . key($largeurs),
      ),
      "popup_link" => "http://www.google.fr",
      "popup_link_new_window" => 1,
      "popup_height" => "",
      "popup_width" => "100%",
      "popup_delay" => 1,
      "popup_bg_hex" => "0779BF",
      "popup_text_hex" => "ffffff",
      "domains_option" => 1,
      "domains_list" => "",
      "exclude_paths" => "",
  );
  variable_set('eu_cookie_compliance_en', $config);
  variable_set('eu_cookie_compliance_fr', $config);
  variable_set('eu_cookie_compliance_it', $config);
  variable_set('eu_cookie_compliance_de', $config);
  variable_set('eu_cookie_compliance_es', $config);
  variable_set('eu_cookie_compliance_ko', $config);
  variable_set('eu_cookie_compliance_pt', $config);
  variable_set('eu_cookie_compliance_ru', $config);
  variable_set('eu_cookie_compliance_th', $config);
  variable_set('eu_cookie_compliance_zh-hans', $config);
  variable_set('eu_cookie_compliance_zh-hant', $config);
}
