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
 * Set le format date dans le module scheduler "systeme" pour etre compatible
 * avec toutes les langues
 * @param type $profile (Pas utilise)
 * @param type $roles   (Pas utilise)
 */
function ctm_installation_config_core_scheduler($profile, $roles, $op = 'update') {
  variable_set('scheduler_date_format', 'Y/m/d H:i');
}


