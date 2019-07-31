<?php

/**
 * Implements hook_permission().
 */
function api_permission() {
  return array(
    'use rest api' => array(
      'title' => t('Use REST API'),
    ),
    'administer rest api' => array(
      'title' => t('Administer REST API'),
    ),
    'view swagger ui' => array(
      'title' => t('View Swagger UI'),
    ),
  );
}

function _api_keys_list() {
  $header = array(
    t('API name'),
    t('API key'),
    t('Active'),
    t('Created'),
    t('Actions')
  );
  $results = db_query("SELECT * FROM {api_keys} ORDER BY name");
  $rows = array();
  foreach($results as $row) {
    $rows[] = array(
      $row->name,
      $row->api_key,
      ($row->active) ? t('Yes') : t('No'),
      format_date($row->created, 'fulldate'),
      l(t('edit'), "admin/config/api/edit_api_key/{$row->id}") . '&emsp;' .
      l(t('delete'), "admin/config/api/delete_api_key/{$row->id}")
    );
  }

  $form = drupal_get_form('_new_api_key_form');

  return theme('table', array('header' => $header, 'rows' => $rows)) . render($form);
}

/**
 * Defines a button form
 */
function _new_api_key_form($form, &$form_state) {
  $form = array();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add new API key'),
  );
  return $form;
}

function _new_api_key_form_submit($form, &$form_state) {
  drupal_goto('admin/config/api/add_api_key');
}

function _add_api_key_form() {
  $form = array();
  $form['#id'] = 'add-api-key-form';

  $form['api_name'] = array(
    '#type' => 'textfield',
    '#title' => t('API name'),
    '#description' => t('A name to identify the user of the API key. E.g. Android app'),
    '#maxlength' => 32,
    '#required' => TRUE,
  );
  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active'),
    '#default_value' => 1,
  );
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save'),
  ];

  return $form;
}

function _add_api_key_form_submit($form, &$form_state) {
  try {
    db_insert('api_keys')
      ->fields(array(
        'name' => $form_state['values']['api_name'],
        'api_key' => $form_state['values']['api_key'],
        'active' => $form_state['values']['active'],
        'created' => time(),
      ))->execute();
  } catch (Exception $e) {
  }
  drupal_set_message(t('Successfully added new API key.'));
  drupal_goto('admin/config/api');
}

function _edit_api_key_form($form ,&$form_state, $id) {
  $api_key = _get_api_key($id);

  $form = _add_api_key_form();
  $form['#id'] = 'edit-api-key-form';
  $form['api_name']['#default_value'] = $api_key['name'];
  $form['api_key']['#default_value'] = $api_key['api_key'];
  $form['active']['#default_value'] = $api_key['active'];

  return $form;
}

function _edit_api_key_form_submit($form, &$form_state) {
  db_update('api_keys')
    ->fields(array(
      'name' => $form_state['values']['api_name'],
      'api_key' => $form_state['values']['api_key'],
      'active' => $form_state['values']['active'],
    ))
    ->condition('id', arg(4))
    ->execute();
  drupal_set_message(t('Successfully updated API key.'));
  drupal_goto('admin/config/api');
}

function _delete_api_key_form($form ,&$form_state, $id) {
  $api_key = _get_api_key($id);
  $form['_api_key'] = array('#type' => 'value', '#value' => $id);

  return confirm_form(
    $form,
    t('Are you sure you want to delete %title?', array('%title' => $api_key['name'])),
    'admin/config/api',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function _delete_api_key_form_submit($form ,&$form_state) {
  $num_deleted = db_delete('api_keys')
    ->condition('id', $form_state['values']['_api_key'])
    ->execute();
  drupal_set_message(t('Successfully deleted API key.'));
  drupal_goto('admin/config/api');
}

function _get_api_key($id) {
  return db_query("SELECT * FROM api_keys WHERE id = :id LIMIT 1", array(':id' => $id))->fetchAssoc();
}
