 Mise en place d'une ferme Drupal industrialisée
=================================================
Ce document décrit comment installer une ferme et un site Drupal
pour développement ou mise en recette.

Une ferme (distribution / core drupal) peut contenir plusieurs site. Pour
installer un site il faut donc d'abord installer la ferme.Si la ferme existe
déja vous pouvez passer à l'étape d'installation du site.

Chaque site peut gérer une (mono-domain) ou plusieurs URL (multi-domain).
Chaque site correspond à une base de données. Il faut donc une base de données
par site même si il n'y a qu'une seule ferme.

1. Pré-requis
--------------
  - Vous devez disposer d'un serveur apache avec PHP5.4 with opcache ou supérieur d'installé
  - Vous devez disposer d'un serveur mysql 5.5 ou supérieur avec une base de
    données vide et d'un utilisateur capable d'accéder à cette base
  - pour windows vous devez avoir installer cygwin

2. Configuration apache
------------------------
  - configuration standard minimum : "mod_dir", "mod_env", "mod_negociation",
    "mod_setif"
  - "mod_rewrite" doit également être activé
  - <directory> doit accepter les commande du fichier ".htaccess"
    (AllowOverride all)
  - "DirectoryIndex" doit contenit "index.php"
  - L'option "followsymlink" doit etre active
  - "Document_root" de votre virtual host doit pointer sur le répertoire "web"
  - Si vous utilisez des url du type "http://<hostname>/<alias>" au
    lieu de "http://<hostname>" il vous faudra modifier votre
    configuration apache. Il vous faut créer un alias (pour la suite nous
    l'appellerons <alias>) qui pointe vers le répertoire "web" de la ferme

3. Configuration PHP
---------------------
  - Si la directive "open_basedir"est utilise, elle doit pointer sur le
    répertoire qui contient les 2 répertoires : "web", et "config"
  - Il est recommandé d'installer le module APCU pour PHP et d'activer opcache
    avec les parametres :

```
    zend_extension = "*path/to*/php_opcache.dll"
    extension=php_apcu.dll
    [APCU]
    apc.enabled = 1
    apc.rfc1867 = On
    apc.shm_size = 512M
    apc.max_file_size = 100M
    [opcache]
    opcache.enable=1
    opcache.enable_cli=1
    opcache.memory_consumption=128
    opcache.interned_strings_buffer=8
    opcache.max_accelerated_files=4000
    opcache.save_comments=0 ;incompatible drupal 8 but optimal for drupal 7
    opcache.load_comments=0 ;incompatible drupal 8 but optimal for drupal 7
    opcache.fast_shutdown=1
    opcache.enable_file_override=1
```

  - les variables PHP :

```
    memory_limit = 256Mo
    max_execution_time = 90
```

  - En prod ajoutez la configuration :

```
    expose_php = Off
    display_errors = Off
```

4. Configuration Mysql
-----------------------
  Vous devez disposer
  - d'une base de données vide
  - d'un utilisateur capable d'accéder à cette base
  - voici notre configuration recommandé :

```
  key_buffer              = 384M
  key_buffer_size         = 32M
  max_allowed_packet      = 128M
  sort_buffer_size        = 2M
  read_buffer_size        = 2M
  read_rnd_buffer_size    = 64M
  myisam_sort_buffer_size = 64M
  table_open_cache        = 4096
  thread_cache_size       = 8M
  query_cache_type        = ON
  query_cache_size        = 32M
  query_cache_limit       = 256K

  innodb_buffer_pool_size = 384M
  innodb_log_file_size    = 256M
  innodb_log_buffer_size  = 4M
  innodb_flush_log_at_trx_commit = 0
  innodb_lock_wait_timeout = 180


  skip-name-resolve
  skip-external-locking

  tmp_table_size      = 64M
  max_heap_table_size = 64M
```

5. Récupération des fichiers
-----------------------------
  - cas de la création d'une nouvelle ferme par un developpeur :
    Récupérer la distribution vierge depuis le scm de la distribution
    et la placer dans votre environnement ou il y a le scm de votre projet
  - cas d'installation d'une ferme existante en dev ou recette :
    Récupérer les fichiers dans le scm du projet existant
  - cas d'une installation en prod :
    Récupérer le zip fourni

 Installation d'un site Drupal industrialisé
=============================================

1. Installation
----------------
  (Sous windows utilisez cygwin)
  - créez les 2 fichier de configuration my_project.config.local.ini et
    my_project.config.local.ini dans le répertoire config en prenant exemple
    sur les fichiers d'exemple.
  - puis lancez la commande `./project.sh site deploy my_project` dans le
    répertoire script,.

  ATTENTION : durant l'installation les mots de passes pour les premiers
              utilisateurs (developer, administrator, et contributor) seront
              affichés pensez à les noter si vous n'avez pas activé
              l'auto-login.

2. opérations specifiques à l'environnement
--------------------------------------------
  Votre site est maintenant installé, cependant vous devez, peut être,
  compléter les informations incorrectes dans le fichier
  "config/settings-site_<site_name>.php". entre les lignes :

```
//MANUAL SETTINGS =====================================================
```

et

```
//END MANUAL SETTINGS =====================================================
```

3. mise en place du cron
-------------------------
  Le cron drupal doit impérativement être lancé toutes les 10 minutes pour un
  fonctionnement optimal. Il faut donc utiliser un crontab ou autre distant ou
  local capable désécuter la commande suivante toutes les 10 minutes :

```
scripts/cron.sh
```
