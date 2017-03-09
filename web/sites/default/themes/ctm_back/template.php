<?php

/**
 * Implements hook_html_head_alter().
 */
function ctm_back_html_head_alter(&$head_elements) {
  //gestion couleur bandeau admin en fonction de l environnement
  switch (variable_get('environnement', 'prod')) {
    case 'dev':
      drupal_add_css('#admin-menu {background-image: none !important; background-color: #881010 !important;}', 'inline');
      break;
    case 'prod':
      break;
    default :
      drupal_add_css('#admin-menu {background-image: none !important; background-color: #108810 !important;}', 'inline');
      break;
  }
}

/**
 * alter fonction ckeditor_settings du module ckeditor.
 * pour configurer ckeditor en fonction du theme.
 * Ici on peut changer les fichier css et js de ckeditor dynamique en fonction
 * du type de contenu, de la taille du ckeditor... par exemple
 * on prefaire cette methode a la configuration en back
 */
function ctm_back_ckeditor_settings_alter(&$settings, $conf) {
  $back_path = base_path() . drupal_get_path('theme', 'ctm_back');
  $front_path = base_path() . drupal_get_path('theme', 'ctm');
  $settings['customConfig'] = $back_path . '/js/ckeditor.config.js?';
  $settings['stylesCombo_stylesSet'] = 'drupal:' . $back_path . '/js/ckeditor.styles.js';
  $settings['contentsCss'][1] = $back_path . '/css/ckeditor_back.css';
  $settings['contentsCss'][] = $front_path . '/css/ckeditor_styles.css';

  //
  // gestion des exception pour mytemplates.js
  //
  $settings['extraAllowedContent'] = "
          abbr[title];
          a[title,contenteditable];
          div[contenteditable,style];
          h3[contenteditable];
          h4[contenteditable];
          figcaption;figure(alignleft,alignright);
          dl;dt;dd;
          blockquote(alignright,alignleft);
          p[contenteditable];
          ul(rteindent1,rteindent2,rteindent3,rteindent4);
          ul[contenteditable];
          tbody[contenteditable];
          img[mapping,data-picture-mapping,contenteditable]";

  // ajout de la variable js avec le chemin du theme back
  $my_settings = array("base_ctm_back" => $back_path,
      "base_ctm" => $front_path);
  drupal_add_js(array('ckeditor' => $my_settings), "setting");
}
