<?php
/**
 * +-----------------------------------------------------------+
 * |                                                           |
 * | all CTM path and constants                                |
 * | composer and drush dont like bin dir out of vendor dir    |
 * |                                                           |
 * +-----------------------------------------------------------+
 * | version : VERSION_APP                                     |
 * +-----------------------------------------------------------+
 **/
define('CONFIG_PATH', 'config');
define('DOCUMENT_ROOT', 'web');
define('SITES_PATH', DOCUMENT_ROOT.'/sites'); #if change update sites.php
define('DOC_PATH', 'docs');
define('SCRIPTS_PATH', 'scripts'); #if change update project.sh file
define('VENDOR_PATH', 'vendor');
define('VENDOR_BIN_PATH', VENDOR_PATH.'/bin');
define('FILES_PATH', 'files');
define('TRANSLATIONS_PATH', FILES_PATH.'/translations');
define('MEDIAS_PATH', DOCUMENT_ROOT.'/media');
define('RELEASES_PATH', DOCUMENT_ROOT.'/releases');
define('DUMP_PATH','dump');
define('DRUSH_CONFIG', 'drush');
define('DRUSH_ALIAS', DRUSH_CONFIG.'/site-aliases');
define('DRUSH_PATH', VENDOR_PATH.'/drush/drush');

define('PATH_SCRIPT_PATH', ABS_SCRIPT_PATH);

define('ABS_ROOT_PATH', dirname(ABS_SCRIPT_PATH));
define('PATH_ROOT_PATH', ABS_ROOT_PATH);

define('ABS_CONFIG_PATH', ABS_ROOT_PATH.'/'.CONFIG_PATH);
define('ABS_DOCUMENT_ROOT', ABS_ROOT_PATH.'/'.DOCUMENT_ROOT);
define('ABS_SITES_PATH', ABS_ROOT_PATH.'/'.SITES_PATH);
define('ABS_DOC_PATH', ABS_ROOT_PATH.'/'.DOC_PATH);
define('ABS_DRUSH_PATH', ABS_ROOT_PATH.'/'.DRUSH_PATH);
define('ABS_VENDOR_PATH', ABS_ROOT_PATH.'/'.VENDOR_PATH);
define('ABS_VENDOR_BIN_PATH', ABS_ROOT_PATH.'/'.VENDOR_BIN_PATH);
define('PATH_VENDOR_BIN_PATH', PATH_ROOT_PATH.'/'.VENDOR_BIN_PATH);
define('ABS_SCRIPTS_PATH', ABS_ROOT_PATH.'/'.SCRIPTS_PATH);
define('ABS_DRUSH_CONFIG', ABS_ROOT_PATH.'/'.DRUSH_CONFIG);
define('ABS_DRUSH_ALIAS', ABS_ROOT_PATH.'/'.DRUSH_ALIAS);
define('ABS_MEDIAS_PATH', ABS_ROOT_PATH.'/'.MEDIAS_PATH);
define('ABS_DUMP_PATH', ABS_ROOT_PATH.'/'.DUMP_PATH);
define('ABS_SOURCE_PATH', ABS_SCRIPTS_PATH.'/'.SOURCE_PATH);
define('ABS_RELEASES_PATH', ABS_ROOT_PATH.'/'.RELEASES_PATH);

#
# const
#
define('ADMIN_NAME', 'developer');
define('LOCAL_CONF', '.config.local.ini');
define('GLOBAL_CONF', '.config.global.ini');
define('EXAMPLE', 'example');

#
# version
#
include_once ABS_RELEASES_PATH.'/property_read.php';
define('VERSION_APP', 
    ctm_installation_get_properties_value(ABS_RELEASES_PATH.'/release.properties', 'VERSION_APP', '0.0.0'));
define('BUILD', 
    ctm_installation_get_properties_value(ABS_RELEASES_PATH.'/release.properties', 'BUILD', '0.0.0'));
define('DIST', 
    ctm_installation_get_properties_value(ABS_RELEASES_PATH.'/release.properties', 'DIST', '0.0.0'));