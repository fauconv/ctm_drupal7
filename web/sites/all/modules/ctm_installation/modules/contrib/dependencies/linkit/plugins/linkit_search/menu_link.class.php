<?php

/**
 * @file
 * Define Linkit user search plugin class.
 */

/**
 * Reprecents a Linkit user search plugin.
 */
class LinkitSearchPluginMenu_link extends LinkitSearchPluginEntity {

  /**
   * Overrides LinkitSearchPluginEntity::__construct().
   */
  function __construct($plugin, $profile) {
    /**
     * The user entity doesn't add any label in their entity keys as they define a
     * "label callback" instead. Therefore we have to tell which field the user
     * entity have as label.
     */
    $this->entity_field_label = 'link_title';
    parent::__construct($plugin, $profile);
  }
}
