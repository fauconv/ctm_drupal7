<?php

/**
 * Configuration du breadcrumb par defaut
 */
function ctm_dev_breadcrumb($variables) {

  $breadcrumb = $variables['breadcrumb'];
  $fil_d_ariane = theme_get_setting('fil_d_ariane');

  if(!empty($breadcrumb)) {
    // On enleve le menu "accueil" et ses doublons
    if(count($breadcrumb) > 1 && $breadcrumb[0] === $breadcrumb[1]) {
      array_shift($breadcrumb);
    }
    array_shift($breadcrumb);

    if(!empty($fil_d_ariane))
      $fil_d_ariane .= ' &gt; ';
    $fil_d_ariane .= l(t('Home'), "<front>");

    foreach($breadcrumb as $key => $value) {
      if(strpos($value, '#REMOVE#') != False) { // tag specifique a mettre dans les vue quand on veut supprimer le breadcrumb
        unset($breadcrumb[$key]);
      }
    }
    if(!empty($breadcrumb)) { // si il a d autre element a part l accueil ou qu il n y a pas de breadcrumb avant
      $fil_d_ariane .= ' &gt; ';
    }
  }

  return $fil_d_ariane . implode(' &gt; ', $breadcrumb);
}

/*
 * suppression de l affichage "journee entiere" pour les dates all day
 */
function ctm_dev_date_all_day($variables) {
  $field = $variables['field'];
  $instance = $variables['instance'];
  $which = $variables['which'];
  $date1 = $variables['date1'];
  $date2 = $variables['date2'];
  $format = $variables['format'];
  $entity = $variables['entity'];
  $view = !empty($variables['view'])?$variables['view']:NULL;

  if(empty($date1) || !is_object($date1) || $format == 'format_interval') {
    return;
  }
  if(empty($date2)) {
    $date2 = $date1;
  }

  if(!date_has_time($field['settings']['granularity'])) {
    $format = date_limit_format($format, array('year', 'month', 'day'));
  }
  else {
    $format_granularity = date_format_order($format);
    $format_has_time = FALSE;
    if(in_array('hour', $format_granularity)) {
      $format_has_time = TRUE;
    }
    $all_day = date_all_day_field($field, $instance, $date1, $date2);
    if($all_day && $format_has_time) {
      $format = date_limit_format($format, array('year', 'month', 'day'));
    }
  }
  $res = trim(date_format_date($$which, 'custom', $format));
  return str_replace('àh', '', $res);
}

/*
 * enleve le "àh" quand il n'y a pas d'heure
 */
function ctm_dev_date_display_range($variables) {
  $same_day = false;
  $date1 = $variables['date1'];
  $date2 = $variables['date2'];
  if(strpos($variables['date1'], 'àh') !== false) {
    $same_day = true; // si les jours sont differents mais qu'il n'y a pas d heure on traite comme si c'etait le meme jour mais avec des heures differentes
    $date1 = str_replace('àh', '', $date1);
    $date2 = str_replace('àh', '', $date2);
  }
  if($variables['dates']['value']['formatted_date'] == $variables['dates']['value2']['formatted_date']) { // si c est le meme jour et qu il n y a que les heures qui change
    $same_day = true;
    $date1 = str_replace('à', '', $date1);
    $date2 = str_replace('à', '', $date2);
  }
  $timezone = $variables['timezone'];
  $attributes_start = $variables['attributes_start'];
  $attributes_end = $variables['attributes_end'];

  $start_date = '<span class="date-display-start"' . drupal_attributes($attributes_start) . '>' . $date1 . '</span>';
  $end_date = '<span class="date-display-end"' . drupal_attributes($attributes_end) . '>' . $date2 . $timezone . '</span>';

// If microdata attributes for the start date property have been passed in,
// add the microdata in meta tags.
  if(!empty($variables['add_microdata'])) {
    $start_date .= '<meta' . drupal_attributes($variables['microdata']['value']['#attributes']) . '/>';
    $end_date .= '<meta' . drupal_attributes($variables['microdata']['value2']['#attributes']) . '/>';
  }
// Wrap the result with the attributes.
  if($same_day) {
    return t('!start-hour to !end-hour', array(
      '!start-hour' => $start_date,
      '!end-hour' => $end_date,
    ));
  }
  else {
    return t('From: !start-date <br />To: !end-date', array(
      '!start-date' => $start_date,
      '!end-date' => $end_date,
    ));
  }
}
