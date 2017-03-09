<?php

/**
 * @file
 * Override of the default maintenance page.
 *
 * This is an override of the default maintenance page. Used for Garland and
 * Minnelli, this file should not be moved or modified since the installation
 * and update pages depend on this file.
 *
 * This mirrors closely page.tpl.php for Garland in order to share the same
 * styles.
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
  </head>
  <body class="<?php print $classes ?>">

<?php print render($page['header']); ?>
<div id="wrapper">
  <div id="container" class="clearfix">

    <div id="header">
      <div id="logo-floater">
        <h1 id="branding"><a href="<?php print $front_page ?>">
          <?php if ($logo): ?>
            <img src="<?php print $logo ?>" id="logo" />
          <?php endif; ?>
          <?php print $site_name ?>
        </a></h1>
      </div>
    </div>

    <div id="center"><div id="squeeze"><div class="right-corner"><div class="left-corner">
      <a id="main-content"></a>
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1<?php print $tabs ? ' class="with-tabs"' : '' ?>><?php print $title ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php print $messages; ?>
      <div class="clearfix">
        <?php print render($page['content']); ?>
      </div>
      <?php print $feed_icons ?>
      <?php print render($page['footer']); ?>
    </div></div></div></div>

  </div> <!-- /#container -->
</div> <!-- /#wrapper -->


  </body>
</html>
