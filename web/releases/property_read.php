<?php

/* 
 * Copyright (C) 2017 cv
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * recupere la valeur d'une variable ant dans un fichier properties
 * @param string $filename : nom du fichier + chemin depuis www
 * @param string $variable : nom de la variable ant
 * @param string $default_value : valeur par defaut retourne si la variable n est pas trouve
 * @return string
 */
function ctm_installation_get_properties_value($filename, $variable, $default_value = NULL) {
  $file = @file($filename);
  if ($file) {
    foreach ($file as $line) {
      if (strpos($line, $variable) === 0) {
        $pos = strpos($line, '=');
        if ($pos > 0) {
          $default_value = str_replace('"','',trim(substr($line, $pos + 1))); //@ignore upgrade47_7
          break;
        }
      }
    }
  }
  return $default_value;
}

