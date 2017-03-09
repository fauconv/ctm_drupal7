<?php

/**
 * @file
 * fonctions execute avant les autres et avant le formulaire d'installation
 * format des fonction ici :
 * ctm_installation_config_XXXXXX_before($profile, $roles, $op = 'update')
 * Les fonctions de ce fichier son execute dans l'ordre du fichier
 * ATTENTION il s'agit bien de fonction CTM_INSTALLATION
 */
function ctm_installation_config_core_site_name($profile, $roles, $op = 'update') {
  variable_set('site_name', SITE_NAME);
}

/**
 * positionnement des largeur possible pour le ckeditor
 */
function ctm_installation_config_core_ckeditor_before($profile, $roles, $op = 'update') {
  variable_set('ckeditor_sizes', array('960' => 'p;h2;h3;h4;h5;h6;address;div')); // largeur du bloc ckeditor en px et tag possible
  variable_set('ckeditor_color', '#64A0B4'); // couleur de la toolbar de ckeditor
}
