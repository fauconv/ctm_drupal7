<?php

/**
 * @file
 * fonctions execute avant le formulaire d'installation et apres les before
 * format des fonction ici :
 * ctm_installation_config_XXXXXX($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 * ATTENTION il s'agit bien de fonction CTM_INSTALLATION
 */


function ctm_installation_config_core_eu_cookie_ctm_internet($profile, $roles, $op = 'update') {
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

/**
 * Implements hook_installation_build_metatag_data()
 * Build datas to set metatags setting from installation
 * @return array
 */
function ctm_installation_config_core_build_metatag_data($profile, $roles, $op = 'update') {
  $cpt = 0;
  $datas = array(
    //config title in instance global
      array(++$cpt, 'global',
      array('title' => array('value' => '[current-page:title] | [site:name]'),
        'description' => array('value' => ''),
        'abstract' => array('value' => ''),
        'keywords' => array('value' => ''),
        'robots' => array('value' => array(
            'index' => 'index',
            'follow' => 'follow',
            'noindex' => 0,
            'nofollow' => 0,
            'noarchive' => 0,
            'nosnippet' => 0,
            'noodp' => 0,
            'noydir' => 0,
            'noimageindex' => 0,
            'notranslate' => 0,
          ),
        ),
        'news_keywords' => array('value' => ''),
        'standout' => array('value' => ''),
        'generator' => array('value' => ''),
        'copyright' => array('value' => '[site:name]'),
        'image_src' => array('value' => ''),
        'canonical' => array('value' => '[current-page:url:absolute]'),
        'shortlink' => array('value' => '[current-page:url:unaliased]'),
        'publisher' => array('value' => ''),
        'author' => array('value' => ''),
        'original-source' => array('value' => ''),
        'content-language' => array('value' => ''),
        'revisit-after' => array(
          'value' => 1,
          'period' => 'day',
        ),
      ),
    ),
    //config title in instance node
    array(++$cpt, 'node',
      $metatagvalues = array(
      'title' => array(
        'value' => '[node:title] | [site:name]',
      ),
      'content-language' => array('value' => '[node:language]'),
      ),
    ),
    //config title in instance global front page
    array(++$cpt, 'global:frontpage',
      $metatagvalues = array(
      'title' => array(
        'value' => '[site:name]',
      ),
      'canonical' => array(
        'value' => '[site:url]',
      ),
      'shortlink' => array(
        'value' => '[site:url]',
      ),
      ),
    ),
    //config title in instance taxonomy term
    array(++$cpt, 'taxonomy_term',
      $metatagvalues = array(
      'title' => array(
        'value' => '[term:name] | [site:name]',
      ),
      'description' => array(
        'value' => '[term:description]',
      ),
      ),
    ),
    //config title in instance user
    array(++$cpt, 'user',
      $metatagvalues = array(
      'title' => array(
        'value' => '[user:name] | [site:name]',
      ),
      ),
    ),
    //config title in instance view
    array(++$cpt, 'view',
      $metatagvalues = array(
      'title' => array(
        'value' => '[view:title] | [site:name]',
      ),
      'description' => array(
        'value' => '[view:description]',
      ),
      'canonical' => array(
        'value' => '[view:url]',
      ),
      ),
    ),
  );
  ctm_installation_metatag($datas);
}

