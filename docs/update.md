Update of a CTM farm
https://github.com/fauconv/ctm_drupal7
====================

This document present how to update an existing farm in development, staging, or production.
A farm is a mono or multi-site with only 1 drupal core.
In a multi-site configuration it is not possible to update only one site.

1. In development
-----------------

  - take files from the project SCM (`git pull` ou `svn update`)
  - Launch `project.sh update` from script directory


2. In production
----------------
  - you have previously created a package with the command `project.sh package x.y`
  - place the .tgz file in the root directory of the project (where there are www, config, and scripts directory)
  - Launch `project.sh unpack <nom_du_fichier_zip>` from script directory

