<?php

/**
 * appel de la deuxieme partie de settings.php dans le repertoire config
 */
require dirname(DRUPAL_ROOT).DIRECTORY_SEPARATOR.'config'.DIRECTORY_SEPARATOR.'settings-'.basename(conf_path()).'.php';


