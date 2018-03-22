<?php
/**
 * @file
 * Administration pages and settings for the detektoren module.
 */

function _detektoren_settings_form() {

  $message = variable_get('detektoren_overview_text');
  $form = array();
  $form['detektoren_overview_text'] = array(
    '#type' => 'text_format',
    '#title' => t('Detektoren overview page text'),
    '#default_value' => $message['value'],
    '#required' => TRUE,
    '#description' => t('Main text on the Detektoren overview page.'),
  );

  return system_settings_form($form);
}
