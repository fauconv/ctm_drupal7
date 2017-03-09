<?php

/**
 * @file
 *
 * This is the task handler plugin to handle page redirecting based on context.
 */

// Plugin definition
$plugin = array(
  // is a 'context' handler type, meaning it supports the API of the
  // context handlers provided by ctools context plugins.
  'handler type' => 'context',
  'visible' => TRUE, // may be added up front.

  // Administrative fields.
  'title' => t('Redirect to View'),
  'admin summary' => 'page_manager_views_redirect_context_admin_summary',
  'admin title' => 'page_manager_views_redirect_context_title',
  'operations' => array(
    'settings' => array(
      'title' => t('Settings'),
      'description' => t('Change general settings about this variant.'),
      'form' => 'page_manager_views_redirect_context_edit_settings',
    ),
    'criteria' => array(
      'title' => t('Selection rules'),
      'description' => t('Control the criteria used to decide whether or not this variant is used.'),
      'ajax' => FALSE,
      'form' => array(
        'order' => array(
          'form' => t('Selection rules'),
        ),
        'forms' => array(
          'form' => array(
            'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
            'form id' => 'ctools_context_handler_edit_criteria',
          ),
        ),
      ),
    ),
    'context' => array(
      'title' => t('Contexts'),
      'ajax' => FALSE,
      'description' => t('Add additional context objects to this variant that can be used by the content.'),
      'form' => array(
        'order' => array(
          'form' => t('Context'),
        ),
        'forms' => array(
          'form' => array(
            'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
            'form id' => 'ctools_context_handler_edit_context',
          ),
        ),
      ),
    ),
  ),

  // Callback to render the data.
  'render' => 'page_manager_views_redirect_context_render',

  // Various callbacks for operations performed on the handler to ensure
  // related data is updated properly. We don't have any additional operations
  // to do (like panels does, with it's displays), except for removing legacy
  // displays on exporting.
  //'save' => 'page_manager_views_redirect_context_save',
  //'delete' => 'page_manager_views_redirect_context_delete',
  'export' => 'page_manager_views_redirect_context_export',
  //'clone' => 'page_manager_views_redirect_context_clone',

  'add features' => array(
    'criteria' => t('Selection rules'),
    'context' => t('Contexts'),
  ),
  // Where to go when finished.
  'add finish' => 'settings',

  'required forms' => array(
    'settings' => t('Configure settings'),
  ),

  'edit forms' => array(
    'criteria' => t('Selection rules'),
    'settings' => t('Settings'),
    'context' => t('Contexts'),
  ),
  'forms' => array(
    'settings' => array(
      'form id' => 'page_manager_views_redirect_context_edit_settings',
    ),
    'context' => array(
      'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
      'form id' => 'ctools_context_handler_edit_context',
    ),
    'criteria' => array(
      'include' => drupal_get_path('module', 'ctools') . '/includes/context-task-handler.inc',
      'form id' => 'ctools_context_handler_edit_criteria',
    ),
  ),
  'default conf' => array(
    'title' => t('Redirect'),
    'contexts' => array(),
    'relationships' => array(),
    'path' => '',
    'query' => '',
    'fragment' => '',
  ),
);

/**
 * Check selection rules and, if passed, render the contexts.
 *
 * We must first check to ensure the node is of a type we're allowed
 * to render. If not, decline to render it by returning NULL.
 */
function page_manager_views_redirect_context_render($handler, $base_contexts, $args, $test = TRUE) {
  // Go through arguments and see if they match.
  ctools_include('context');
  ctools_include('context-task-handler');
  // Add my contexts
  $contexts = ctools_context_handler_get_handler_contexts($base_contexts, $handler);

  // Test.
  if ($test && !ctools_context_handler_select($handler, $contexts)) {
    return;
  }

  if (isset($handler->handler)) {
    ctools_context_handler_pre_render($handler, $contexts, $args);
  }

  $info = array(
    'content' => page_manager_views_redirect_context_render_actual($handler, $contexts),
  );

  return $info;
}

/**
 * Special handling for exporting a panel task handler.
 *
 * When a panel is exported, we need to export the display separately
 * rather than just letting its object be unpacked, which does not work
 * very well.
 */
function page_manager_views_redirect_context_export(&$handler, $indent) {
  // Left to remove legacy code when exporting.
  // TODO: Take this out, and do it to any old displays in upgrade path instead.
  foreach (array('display', 'did') as $item) {
    if (isset($handler->conf[$item])) {
      unset($handler->conf[$item]);
    }
  }
}

/**
 * Set up a title for the panel based upon the selection rules.
 */
function page_manager_views_redirect_context_title($handler, $task, $subtask) {
  if (isset($handler->conf['title'])) {
    return check_plain($handler->conf['title']);
  }
  else {
    return t('View Redirect');
  }
}

/**
 * Provide a nice little summary of what's in a panel.
 *
 * The task handler manager provides a summary of a given handler in a
 * collapsible div. This callback provides that. For Redirects, we provide a
 * summary of the redirect on one side, and a summary of the contexts in use on
 * the other.
 */
function page_manager_views_redirect_context_admin_summary($handler, $task, $subtask, $page, $show_title = TRUE) {
  $task_name = page_manager_make_task_name($task['name'], $subtask['name']);
  $output = '';

  ctools_include('context');
  ctools_include('context-task-handler');

  // Get the operations
  $operations = page_manager_get_operations($page);

  // Get operations for just this handler.
  $operations = $operations['handlers']['children'][$handler->name]['children']['actions']['children'];
  $args = array('handlers', $handler->name, 'actions');
  $rendered_operations = page_manager_render_operations($page, $operations, array(), array('class' => array('actions')), 'actions', $args);

  $plugin = page_manager_get_task_handler($handler->handler);

  $object = ctools_context_handler_get_task_object($task, $subtask, $handler);
  $context = ctools_context_load_contexts($object, TRUE);

  $access = ctools_access_group_summary(!empty($handler->conf['access']) ? $handler->conf['access'] : array(), $context);
  if ($access) {
    $access = t('This panel will be selected if @conditions.', array('@conditions' => $access));
  }
  else {
    $access = t('This panel will always be selected.');
  }

  $rows = array();

  $type = $handler->type == t('Default') ? t('In code') : $handler->type;
  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Storage')),
    array('class' => t('page-summary-data'), 'data' => $type),
    array('class' => t('page-summary-operation'), 'data' => ''),
  );

  if (!empty($handler->disabled)) {
    $link = l(t('Enable'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'actions', 'enable')));
    $text = t('Disabled');
  }
  else {
    $link = l(t('Disable'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'actions', 'disable')));
    $text = t('Enabled');
  }

  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Status')),
    array('class' => t('page-summary-data'), 'data' => $text),
    array('class' => t('page-summary-operation'), 'data' => $link),
  );

  $link = l(t('Edit'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'criteria')));
  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Selection rule')),
    array('class' => t('page-summary-data'), 'data' => $access),
    array('class' => t('page-summary-operation'), 'data' => $link),
  );

  $link = l(t('Edit'), page_manager_edit_url($task_name, array('handlers', $handler->name, 'settings')));

  // Convert the query from a string to an array that can be used with url().

  $rows[] = array(
    array('class' => t('page-summary-label'), 'data' => t('Redirect view')),
    array('class' => t('page-summary-data'), 'data' => $handler->conf['view'].' => '. $handler->conf['display_view']),
    array('class' => t('page-summary-operation'), 'data' => $link),
  );

  $info = theme('table', array('rows' => $rows, 'attributes' => array('class' => 'page-manager-handler-summary')));

  $title = $handler->conf['title'];
  if ($title != t('Redirect')) {
    $title = t('Redirect: @title', array('@title' => $title));
  }

  $output .= '<div class="clear-block">';
  if ($show_title) {
    $output .= '<div class="handler-title clear-block">';
    $output .= '<div class="actions handler-actions">' . $rendered_operations['actions'] . '</div>';
    $output .= '<span class="title-label">' . $title . '</span>';
  }

  $output .= '</div>';
  $output .= $info;
  $output .= '</div>';

  return $output;
}


/**
 * 'Render' a redirect display by simply redirecting.
 *
 * if $incoming_content is NULL, default content will be applied. Use
 * an empty string to indicate no content.
 * @render
 * @ingroup hook_invocations
 */
function page_manager_views_redirect_context_render_actual(&$handler, $context) {
  ctools_include('context');
  module_load_include('module', 'views');

  if(!empty($handler->conf['args'])) {
    $args = str_replace(' ', '', $handler->conf['args']);
    $args = explode('/',$args);
    foreach($args as $key => $value) {
      $args[$key] = ctools_context_keyword_substitute($value, array(), $context);
    }
  } else {
    $args = array();
  }

  // Generate the view's output.
  $view = views_get_view($handler->conf['view']);
  $output = $view->execute_display($handler->conf['display_view'], $args);

  return $output;

}
