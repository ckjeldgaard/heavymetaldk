<?php
// $Id: statistics.admin.inc Exp $

/**
 * Implements hook_menu().
 */
function statistics_menu() {
  $items['admin/config/statistics'] = [
    'title' => t('Statistics'),
    'description' => t('Google Analytics statistics for reviews'),
    'page callback' => 'drupal_get_form',
    'page arguments' => ['_statistics_admin_form'],
    'access arguments' => ['read statistics'],
    'type' => MENU_NORMAL_ITEM,
  ];
  $items['admin/config/statistics/emails'] = array(
    'title' => 'Statistics mails',
    'description' => 'Configure statistics emails',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('_statistics_mail_form'),
    'access arguments' => array('read statistics'),
    'type' => MENU_NORMAL_ITEM,
  );
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

function _available_years() {
  $start_year = 2001;
  $years = array();
  for ($i = $start_year; $i <= date('Y'); $i++) {
    $years[$i . ''] = $i;
  }
  return $years;
}

/**
 * Gets the statistics admin page contents.
 */
function _statistics_admin_form($form, &$form_state) {
  global $pager_total_items;

  $year = (isset($_GET['filter_year']) && in_array($_GET['filter_year'], _available_years())) ? $_GET['filter_year'] : date('Y');
  $show_all = (isset($_GET['filter_show_all'])) ? TRUE : FALSE;
  $element = 1; // Integer to distinguish between multiple pagers on same page.
  $num_per_page = ($show_all) ? 1000 : 50;

  $form['#method'] = 'get';

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Filter option'),
  );
  $form['filter']['filter_year'] = array(
    '#title' => t('Year'),
    '#type' => 'select',
    '#options' => _available_years(),
    '#default_value' => $year . '',
  );
  $form['filter']['filter_show_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show all?'),
    '#default_value' => $show_all,
  );
  $form['filter']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $header = array(
    array('data' => t('Review'), 'field' => 'title'),
    array('data' => t('Rating'), 'field' => 'rating'),
    array('data' => t('Artist country'), 'field' => 'country'),
    array('data' => t('Type'), 'field' => 'type'),
    array('data' => t('Review publish date'), 'field' => 'published_at', 'sort' => 'asc'),
    array('data' => t('Pageviews'), 'field' => 'pageviews')
  );

  $start_time = mktime(0, 0, 0, 1, 1, $year);
  $end_time = mktime(0, 0, 0, 12, 31, $year);

  $query = db_select('node', 'n')
    ->extend('PagerDefault')->element($element)
    ->limit($num_per_page)
    ->extend('TableSort')
    ->orderByHeader($header);
  $query->fields('n', array('nid', 'title'));
  $query->addField('k', 'field_rating_value', 'rating');
  $query->addField('co', 'name', 'country');
  $query->addField('d', 'published_at');
  $query->addField('p', 'pageviews');
  $query->addField('t', 'field_release_type_value', 'type');
  $query->innerJoin('field_data_field_release', 'r', 'n.nid = r.entity_id');
  $query->innerJoin('field_data_field_artist', 'a', 'r.field_release_target_id = a.entity_id');
  $query->join('publication_date', 'd', 'n.nid = d.nid');
  $query->join('field_data_field_rating', 'k', 'n.nid = k.entity_id');
  $query->leftJoin('field_data_field_country', 'c', 'a.field_artist_target_id = c.entity_id');
  $query->leftJoin('countries_country', 'co', 'co.iso2 = c.field_country_iso2');
  $query->leftJoin('analytics_pageviews', 'p', 'p.nid = n.nid');
  $query->join('field_data_field_release_type', 't', 't.entity_id = r.field_release_target_id');
  $query->condition('n.type', 'review');
  $query->condition('n.status', 1);
  $query->condition('d.published_at', $start_time, '>');
  $query->condition('d.published_at', $end_time, '<');
  $query->orderBy('d.published_at');
  $query->groupBy('n.nid');

  $results = $query->execute();

  $rows = array();
  foreach ($results as $row) {
    $rows[] = array(
      l($row->title, 'node/' . $row->nid),
      $row->rating,
      $row->country,
      $row->type,
      format_date($row->published_at, 'displaydate_short'),
      $row->pageviews
    );
  }

  # add the pager
  $pager = array('#theme' => 'pager', '#element' => $element);

  $form['stats'] = array(
    '#markup' => "<p>" . t('Total reviews in @year:', array('@year' => $year)) . " <strong>" . $pager_total_items[1] . "</strong><br />
    " . t('Last synchronized with Google Analytics:') . " <strong>" . format_date(variable_get('analytics_last_sync_run', 0), "long") . "</strong></p>"
  );

  $form['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('Table has no rows')
  );
  $form['pager'] = array('#markup' => render($pager));
  return $form;
}
