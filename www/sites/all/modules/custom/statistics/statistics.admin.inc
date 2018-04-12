<?php
// $Id: contest.admin.inc Exp $

/**
 * Implements hook_menu().
 */
function statistics_menu() {
  $items['admin/config/statistics'] = [
    'title' => t('Statistics'),
    'description' => t('Google Analytics statistics for reviews'),
    'page callback' => 'drupal_get_form',
    'page arguments' => ['_statistics_admin_page'],
    'access arguments' => ['read statistics'],
    'type' => MENU_NORMAL_ITEM,
  ];
  return $items;
}

/**
 * Implements hook_permission().
 */
function statistics_permission() {
  return [
    'read statistics' => [
      'title' => t('Read statistics'),
    ],
  ];
}

/**
 * Gets the statistics admin page contents.
 */
function _statistics_admin_page() {

}
