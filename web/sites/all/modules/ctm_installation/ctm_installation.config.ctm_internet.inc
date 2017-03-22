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
