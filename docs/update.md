 Mise à jour d'une industrialisation Drupal
===========================================
Ce document décrit comment faire une mise a jour d'un site existant et déja
installé en prod sur une machine de dev.

Une ferme (distribution / core drupal) peut contenir plusieurs site. C'est
toujours une ferme complète qui est mise à jour. Il n est pas possible
de mettre à jour uniquement 1 site dans une ferme contenant plusieurs sites.

1. Récupération des fichiers
-----------------------------
  - Récupérer les fichiers dans le scm du projet existant
    (`git pull` ou `svn update`)

2. Compilation
---------------
  -  Dans le répertpore scripts lancez la commande :
    `./build.sh *site_ref*`

3. Mise à jour
---------------
  - Lancez la commande :
    `deploy.sh -u`
