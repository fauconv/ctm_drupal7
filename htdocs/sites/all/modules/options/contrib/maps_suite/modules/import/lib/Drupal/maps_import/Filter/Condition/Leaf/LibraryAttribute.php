<?php

namespace Drupal\maps_import\Filter\Condition\Leaf;

/**
 * Condition on library attribute class.
 */
class LibraryAttribute extends Leaf {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return 'Library attribute';
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'library_attribute';
  }

  /**
   * @inheritdoc
   */
  public function form($form, &$form_state) {
    $libraries = array();

    // Retrieve the existing attributes.
    $profile = $this->getConverter()->getProfile();
    $attributes = $profile->getConfigurationTypes('attribute', $profile->getLanguage());

    $values = array();

    $criteria = $this->getCriteria();

    // Filter to only keep library attributes.
    foreach ($attributes as $id_attribute => $attribute) {
      $attribute['data'] = unserialize($attribute['data']);
      if ($attribute['data']['attribute_type_code'] === 'library') {
        $libraries[$id_attribute] = $attribute;

        if (!isset($values[$id_attribute])) {
          $values[$id_attribute] = array();
        }
      }
    }

    // Display an error message if there is no library attribute.
    if (empty($libraries)) {
      drupal_set_message(t('The current publication does not contain any library attribute'), 'error');
      return $form;
    }

    // Retrieve the libraries values.
    $result = db_select('maps_import_libraries', 'l')
      ->fields('l')
      ->condition('id_attribute', array_keys($libraries))
      ->condition('pid', $profile->getPid())
      ->execute();

    while ($row = $result->fetchAssoc()) {
      $values[$row['id_attribute']][$row['code']][$row['id_language']] = $row;
    }

    // Construct the library select.
    $form['library'] = array(
      '#type' => 'select',
      '#title' => t('Library attribute'),
      '#description' => t('Select the library'),
      '#options' => array('' => '- None -'),
      '#required' => TRUE,
      '#default_value' => isset($criteria['library']) ? $criteria['library'] : '',
    );

    foreach ($libraries as $id_library => $library) {
      $form['library']['#options'][$id_library] = $library['title'];
    }

    // Construct the library value select.
    $form['library_value'] = array('#type' => 'value', '#value' => isset($criteria['value']) ? $criteria['value'] : '');

    $form['values']['#tree'] = TRUE;
    foreach ($values as $id_attribute => $value) {
      $form['values'][$id_attribute] = array(
        '#type' => 'select',
        '#title' => t('Library value'),
        '#description' => t('Select the library value to use'),
        '#options' => array('' => '- None -'),
        '#states' => array(
          'visible' => array(
            ':input[name="library"]' => array('value' => $id_attribute),
          ),
          'required' => array(
            ':input[name="library"]' => array('value' => $id_attribute),
          ),
        ),
        '#default_value' => isset($criteria['value']) && $id_attribute == $criteria['library'] ? $criteria['value'] : '',
      );

      foreach ($values[$id_attribute] as $_values) {
        if (isset($_values[$profile->getLanguage()])) {
          $form['values'][$id_attribute]['#options'][ $_values[$profile->getLanguage()]['id']] = $_values[$profile->getLanguage()]['value'];
        }
      }
    }

    $form['negate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Negate'),
      '#default_value' => $this->negate,
    );

    return $form;
  }

  /**
   * @inheritdoc
   */
  public function formValidate($form, &$form_state) {
    $values = array_merge($form_state['values'], $form_state['input']);

    if ($id_attribute = $values['library']) {
      if (empty($values['values'][$id_attribute])) {
        form_set_error('values][' . $id_attribute, t('!name field is required.', array('!name' => $values[$id_attribute]['#title'])));
      }
      else {
        $form_state['input']['library_value'] = $values['values'][$id_attribute];
      }
    }
  }

  /**
   * @inheritdoc
   */
  public function formSave($form, &$form_state) {
    // Necessary for multistep forms.
    $values = array_merge($form_state['values'], $form_state['input']);

    $classes = $this->getConverter()->getFilter()->getAvailableConditions();
    $condition = new $classes[$values['type']]['class']($this->getConverter(), $values);

    $criteria = array(
      'library' => $values['library'],
      'value' => $values['library_value'],
    );

    $condition->setCriteria($criteria);
    $condition->negate = $values['negate'];
    $condition->save();
  }

  /**
   * @inheritdoc
   */
  public function getLabel() {
    $criteria = $this->getCriteria();
    $library = $criteria['library'];
    $library_value = $criteria['value'];

    // Retrieve the name of the library.
    $profile = $this->getConverter()->getProfile();
    $attributes = $profile->getConfigurationTypes('attribute', $profile->getLanguage());
    $attribute = $attributes[$library];

    // Retrieve the textual value.
    $result = db_select('maps_import_libraries', 'l')
      ->fields('l')
      ->condition('id', $library_value)
      ->condition('id_language', $profile->getLanguage())
      ->condition('pid', $profile->getPid())
      ->execute()
      ->fetchAssoc();

    $t_args = array(
      '%library' => $attribute['title'],
      '%value' => $result['value'],
    );

    return $this->negate ?
      t('Library %library value is not %value.', $t_args) :
      t('Library %library value is %value.', $t_args);
  }

  /**
   * @inheritdoc
   */
  public function match(array $entity) {
    $criteria = $this->getCriteria();
    if (empty($criteria)) {
      return !$this->negate;
    }

    $attributes = unserialize($entity['attributes']);
    if (!isset($attributes[$criteria['library']])) {
      return $this->negate;
    }

    return $this->checkNegate(in_array($criteria['value'], array_keys($attributes[$criteria['library']][0])));
  }

}
