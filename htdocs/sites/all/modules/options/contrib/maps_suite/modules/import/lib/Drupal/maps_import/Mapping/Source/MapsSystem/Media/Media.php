<?php

/**
 * @file
 * Class that defines operation on MaPS Object's object-media relation.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Media;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapper;
use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface as DrupalEntityInterface;
use Drupal\maps_import\Exception\MappingException;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Converter\Media as MediaConverter;

abstract class Media extends PropertyWrapper {

  /**
   * Return the media key.
   *
   * @return string
   */
  public function getKey() {
    return 'media:' . $this->id;
  }

  /**
   * @inheritdoc
   */
  public function __construct(array $definition = array()) {
    parent::__construct($definition);
  }

    /**
   * Whether the property is translatable.
   *
   * @var boolean
   */
  protected $translatable = FALSE;

  /**
   * Whether the property may have multiple values.
   *
   * @var boolean
   */
  protected $multiple = TRUE;

  /**
   * @inheritdoc
   */
  public function extractValues(EntityInterface $entity, $options = array(), ConverterInterface $currentConverter) {
    //$medias = $entity->getMedias($this->media_type);
    $_medias = $entity->getMedias(NULL);

    // Now, we have to filer the medias, to keep only the ones imported by
    // the given media converter.
    if (!$converter = maps_import_converter_load_by_name($this->id, $currentConverter->getProfile()->getPid())) {
      return array();
    }

    if (empty($_medias)) {
      return array();
    }

    $medias_by_id = array();
    foreach ($_medias as $media) {
      $medias_by_id[$media['media_id']] = $media;
    }

    // Look for the medias imported by the given converter.
    $query = db_select('maps_import_medias', 'mim');
    $query->join('maps_import_media_ids', 'mimi', 'mim.id = mimi.maps_id');
    $query->join('maps_import_entities', 'mie', 'mimi.correspondence_id = mie.id');
    $query->condition('mie.cid', $converter->getCid());
    $query->condition('mim.id', array_keys($medias_by_id));
    $query->fields('mim', array('id'));

    $results = $query
      ->execute()
      ->fetchAll(\PDO::FETCH_ASSOC);

    $medias = array();
    foreach ($results as $result) {
      if (isset($medias_by_id[$result['id']])) {
        $medias[] = $medias_by_id[$result['id']];
      }
    }

    if (!isset($options['media_start_range']) || !isset($options['media_limit_range'])) {
      throw new MappingException('Media ranges are not defined.', 0, array());
    }

    $values = array();
    $values[0] = array();

    for ($i = ((int) $options['media_start_range'] - 1); $i < (int) $options['media_limit_range']; $i++) {
      if (isset($medias[$i])) {
        $values[DrupalEntityInterface::LANGUAGE_NONE][] = $medias[$i]['entity_id'];
      }
    }

    return $values;
  }

  /**
   * @inheritdoc
   */
  public function getGroupLabel() {
    return t('Object media relation');
  }

  /**
   * @inheritdoc
   */
  public function getTranslatedTitle() {
    return 'Media';
  }

  /**
   * Return the available Media classes.
   */
  static public function getMediaClassFromMediaType($mediaType) {
  	$mediaTypes = array(
  	  1 => 'Image',
  	  2 => 'Document',
  	  3 => 'Video',
  	  4 => 'Sound',
  	);

  	return 'Drupal\\maps_import\\Mapping\\Source\\MapsSystem\\Media\\' . $mediaTypes[$mediaType];
  }

  /**
   * Load a MaPS System® media entity from the MaPS media type id.
   *
   * @param $mediaType
   *   The MaPS System® media type id.
   * @param $definition
   *   The MaPS System® property definition.
   */
  static public function createMediaFromMediaType($mediaType, array $definition = array()) {
    $className = self::getMediaClassFromMediaType($mediaType);
    return new $className($definition);
  }

  /**
   * @inheritdoc
   */
  public function exists(Profile $profile) {
    $exploded = explode(':', $this->id);
    if(isset($exploded[1])) {
      $cid = $exploded[1];
    } else {
      $cid = $exploded[0];
    }

    return (bool) maps_import_converter_load_by_name($cid, $profile->getPid());
  }

}
