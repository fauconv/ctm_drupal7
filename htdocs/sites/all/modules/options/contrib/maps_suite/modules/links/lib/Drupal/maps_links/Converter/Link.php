<?php

namespace Drupal\maps_links\Converter;

use Drupal\maps_import\Converter\Converter;
use Drupal\maps_links\Mapping\Link as LinkMapping;

class Link extends Converter implements LinkInterface{

  /**
   * The link type.
   *
   * @var string
   */
  private $link_type;

  /**
   * @inheritdoc
   */
  private $entity_type = 'relation';

  /**
   * @inheritdoc
   */
  protected $baseTable = 'maps_links_converter';

  /**
   * @inheritdoc
   */
  public function getMapping() {
    return new LinkMapping($this);
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'link';
  }

  /**
   * @inheritdoc
   */
  public function isMappingAllowed() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  public function entityInfo() {
    $entityInfo = parent::entityInfo();
    return $entityInfo['relation']['bundles'];
  }

  /**
   * @inheritdoc
   */
  public function getLinkType() {
  	return $this->link_type;
  }

  /**
   * @inheritdoc
   */
  public function setLinkType($link_type) {
  	$this->link_type = $link_type;
  }

  /**
   * @inheritdoc
   */
  public function getEntityType() {
  	return 'relation';
  }

  /**
   * @inheritdoc
   */
  public function delete(array $options = array()) {
    // Delete from the link converter table.
    db_delete('maps_links_converter')
      ->condition('cid', $this->getCid())
      ->execute();

    // @todo Delete created relations.
  }

  /**
   * @inheritdoc
   */
  public function hasAdditionalOptions() {
    return FALSE;
  }

}
