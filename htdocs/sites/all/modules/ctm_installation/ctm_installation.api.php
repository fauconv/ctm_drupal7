<?php

/**
 * @file
 * Hooks provided by the ctm_installation module.
 */

/**
 * alter list of module to deactivate automaticaly each night
 * place all ui modules and devel modules that not already exist in ctm_installation
 * @param array $list_modules
 */
function hook_installation_modules_alter(&$list_modules) {
  $list_modules['mon_module_de_dev'] = 'mon_module_de_dev';
}

/**
 * alter list of module to activate automaticaly by default on click "activate developer modules"
 * place all ui modules that not already exist in ctm_installation
 * @param array $list_modules
 */
function hook_installation_default_modules_alter(&$list_modules) {
  $list_modules['mon_module_de_dev'] = 'mon_module_de_dev';
}

/**
 * Permet de modifier les droits par type de contenu dans l'indus
 * le cas 'default' est le cas de la gestion des droits dans les vues quand
 * on ne sait pas quel ct on va obtenir
 * @param type $matrice
 */
function hook_installation_permission_matrice_alter(&$matrice) {

}

/**
 * implements hook_installation_config_file
 * Permet de declarer un module comme un module de config
 * qui pourra donc contenir un ou des fichiers parmis :
 * - mon_module.config.before.inc
 * - mon_module.config.before.ctm_internet.inc
 * - mon_module.config.before.ctm_intranet.inc
 * - mon_module.config.inc
 * - mon_module.config.ctm_internet.inc
 * - mon_module.config.ctm_intranet.inc
 * - mon_module.config.after.inc
 * - mon_module.config.after.ctm_internet.inc
 * - mon_module.config.after.ctm_intranet.inc
 * Chaque fichier contient des fonctions ctm_installation_config_mon_module_XXXX();
 * Qui seront executer a  l'installation
 * Si les fonction du module defini doivent etre executer avant d'autre
 * modules il suffit de specifier ceux ci dans le tableau retourne
 */
function hook_installation_config() {
 return array();
}

/**
 * permet d'alterer la liste des fonctions qui va etre executer before
 */
function hook_installation_config_all_before_alter(&$list_functions) {

}

/**
 * permet d'alterer la liste des fonctions qui va etre executer before
 */
function hook_installation_config_all_alter(&$list_functions) {

}

/**
 * permet d'alterer la liste des fonctions qui va etre executer after
 */
function hook_installation_config_all_after_alter(&$list_functions) {

}

/**
 * Allows alias creation/update
 */
function hook_installation_alias() {
  return array(
    'path_system' => array(
      'en' => 'download',
      'fr' => 'telechargement',
    )
  );
}
