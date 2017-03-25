@ECHO OFF
setlocal DISABLEDELAYEDEXPANSION
SET BIN_TARGET=%~dp0/../vendor/drush/drush/drush.php
SET ABS_DOCUMENT_ROOT=%~dp0/../web
SET ABS_DRUSH_ALIAS=%~dp0/../drush/site-aliases
SET ABS_DRUSH_CONFIG=%~dp0/../drush
php "%BIN_TARGET%" %* -y --root=%ABS_DOCUMENT_ROOT% --alias-path=%ABS_DRUSH_ALIAS% --config=%ABS_DRUSH_CONFIG% --include=%ABS_DRUSH_CONFIG%
