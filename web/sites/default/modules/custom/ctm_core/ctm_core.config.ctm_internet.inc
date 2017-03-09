<?php

/**
 * @file
 * fonctions execute avant le formulaire d'installation et apres les before
 * format des fonction ici :
 * ctm_installation_config_XXXXXX($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 * ATTENTION il s'agit bien de fonction CTM_INSTALLATION
 */

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

