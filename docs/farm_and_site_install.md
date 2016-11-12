# Installation d'une ferme Drupal industrialisée
################################################

Une ferme (distribution / core drupal) peut contenir plusieurs site. Pour
installer un site il faut donc d'abord installer la ferme. Si la ferme existe
déja vous pouvez passer à l'étape d'installation du site.

01. *Pré-requis*
================
  - Vous devez disposer d'un serveur apache avec PHP5.4 ou supérieur d'installé
  - Vous devez disposer d'un serveur mysql 5.5 ou supérieur avec une base de 
    données vide et d'un utilisateur capable d'accéder à cette base
  - vous devez avoir installer une version de Drush 7.0 ou supérieur.
  - Pour un environnement de développement uniquement : 
    vous devez disposer de nodejs 0.12.7 et avoir installé gulp et bower
  - pour windows vous devez avoir installer cygwin

02. *Configuration apache*
==========================
  - configuration standard minimum : "mod_dir", "mod_env", "mod_negociation", 
    "mod_setif"
  - "mod_rewrite" doit également être activé
  - <directory> doit accepter les commande du fichier ".htaccess" 
    (AllowOverride all)
  - "DirectoryIndex" doit contenit "index.php"
  - L'option "followsymlink" doit etre active
  - Si vous utilisez des url du type "http://<hostname>/index.php", le 
    "Document_root" de votre virtual host doit pointer sur le répertoire "www"
  - Si vous utilisez des url du type "http://<hostname>/<alias>/index.php" au 
    lieu de "http://<hostname>/index.php" il vous faudra modifier votre 
    configuration apache. Il vous faut créer un alias(pour la suite nous 
    l'appellerons <alias>) qui pointe vers le répertoire "www" de la ferme

  
03. *Configuration PHP*
=======================
  - Si la directive "open_basedir"est utilise, elle doit pointer sur le 
    répertoire qui contient les 3 répertoires : "media", "www", et "config"
  - Il est recommandé d'installer APC
  - les variables PHP :
    - "memory_limit"=256Mo
    - "max_execution_time"=90
  - En prod ajouter la configuration :
    - expose_php = Off
    - display_errors = Off
    
04. *Configuration Mysql*
=========================


05. *Récupération des fichiers*
===============================
  - cas de la création d'une nouvelle ferme :
    Récupérer la distribution vierge depuis le scm de la distribution
    et la placer dans votre environnement.
  - cas d'une ferme existante :
    Récupérer les fichiers dans le scm du projet existant
  - cas d'une installation en prod :
    Récupérer le zip fourni

06. *Configuration fichiers spécifique pour windows*
====================================================
  Créez les liens Symboliques :
  - "mklink /d media ..\media" dans le répertoire "www"
  - "mklink /d sites.php ..\..\config\sites.php" dans le répertoire "www/sites"

07. *Configuration spécifique à l'utilisation d'alias Apache*
=============================================================
  Si il s'agit d'une création de nouvelle ferme (nouveau projet) :
  Si vous utilisez des url du type "http://<hostname>/<alias>/index.php" au lieu
  de "http://<hostname>/index.php" il vous faudra décommenter les 2 lignes
  suivante dans le fichier "www/.htaccess" :

~~~
  # uncomment the 2 lines if you use apache alias
  #RewriteCond %{REQUEST_URI} ^/([^/]*)/(.*)$
  #RewriteRule ^(.*)$ /%1/index.php?q=$1 [QSA,L]
~~~


# Installation d'un site Drupal industrialisé
#############################################

01. *Installation*
==================
  (Sous windows utilisez cygwin)
  Aller dans le répertoire scripts et lancer la commande "./deploy.sh" avec
  les bon arguments, pour installer/créer votre site.
  Avant de lancer la commande deploy il est possible de créer un fichier de
  configuration dans le répertoire "config" avec le nom :
  site_<site ref>.conf
  Lancer la commande ./deploy.sh sans paramètre pour plus d'information.

  ATTENTION : durant l'installation les mots de passes pour les premiers
              utilisateurs (admin, admin-site, et contributor) seront affichés
              pensez à les noter si vous n'avez pas activé l'auto-login.

02. *opérations specifiques à l'environnement*
==============================================
  Votre site est maintenant installé, cependant vous devez, peut être,
  compléter les informations incorrectes dans le fichier
"config/settings-site_<site_name>.php". entre les lignes :

~~~
//MANUAL SETTINGS =====================================================
~~~

et

~~~
//END MANUAL SETTINGS =====================================================
~~~

06. *mise en place du cron*
===========================
  Le cron drupal doit impérativement etre lancer toutes les 10 minutes pour un 
  fonctionnement optimal. Il faut donc utiliser un crontab ou autre distant ou 
  local capable désécuter la commande suivante toutes les 10 minutes :

~~~
scripts/cron.sh
~~~

07. *Installation des package front*
====================================
  Si il s'agit d'un serveur de dev :
  Dans le répertoire scripts lancer la commande "npm install". Sous windows
  cette commande doit être lancer dans un shell command.com et non sous
  cygwin

08. *Compilation du front*
==========================
  Si il s'agit d'un serveur de dev :
  Dans le répertoire scripts lancer la commande "gulp" ou "gulp prod".
  Sous windows cette commande doit être lancer avec cygwin
