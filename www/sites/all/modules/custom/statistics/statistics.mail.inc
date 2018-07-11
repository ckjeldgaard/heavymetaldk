<?php

define("THRESHOLD_LOW", 500);
define("THRESHOLD_HIGH", 1000);
define("EPOCH_DATE", mktime(0, 0, 0, 1, 10, 2018));

define("MAIL_TYPE_LOW", 0);
define("MAIL_TYPE_HIGH", 1);

function _send_pageview_emails() {
  $sql = "SELECT p.*, d.published_at FROM analytics_pageviews p ";
  $sql.= "INNER JOIN publication_date d ON d.nid = p.nid ";
  $sql.= "WHERE p.pageviews >= :threshold ";
  $sql.= "AND d.published_at >= :published_at";
  $rs = db_query($sql, array('threshold' => THRESHOLD_LOW, 'published_at' => EPOCH_DATE));

  foreach ($rs as $row) {
    if (!_has_mail_been_sent($row->nid, MAIL_TYPE_LOW) && $row->pageviews >= THRESHOLD_LOW) {
      _send_mail($row->nid, MAIL_TYPE_LOW, $row->pageviews);
    }
    if (!_has_mail_been_sent($row->nid, MAIL_TYPE_HIGH) && $row->pageviews >= THRESHOLD_HIGH) {
      _send_mail($row->nid, MAIL_TYPE_HIGH, $row->pageviews);
    }
  }
}

function _send_mail($nid, $mail_type, $pageviews) {
  global $base_url;
  $url = $base_url . "/" . drupal_get_path_alias('node/' . $nid);
  $site_email = variable_get('site_mail', '');

  $sql = "SELECT n.title, u.name, u.mail ";
  $sql.= "FROM node n ";
  $sql.= "INNER JOIN users u ON n.uid = u.uid ";
  $sql.= "WHERE n.nid = :nid ";
  $rs = db_query($sql, array(':nid' => $nid))->fetchAssoc();

  $to = $rs['mail'];

  $subject = ($mail_type == MAIL_TYPE_LOW) ? variable_get('email_low_subject') : variable_get('email_high_subject');
  $subject = str_replace(":REVIEW_NAME", $rs['title'], $subject);

  $message = ($mail_type == MAIL_TYPE_LOW) ? variable_get('email_low_contents') : variable_get('email_high_contents');
  $message = str_replace(":AUTHOR", $rs['name'], $message);
  $message = str_replace(":REVIEW_NAME", $rs['title'], $message);
  $message = str_replace(":REVIEW_URL", $url, $message);
  $message = str_replace(":PAGEVIEWS", $pageviews, $message);

  // Send the email.
  $params = array(
    'subject' => $subject,
    'body' => check_markup(
      t($message),
      'plain_text'
    ),
    'headers' => array(
      'Bcc' => _editor_emails($to)
    )
  );

  $watchdog_vars = array(
    "%type" => $mail_type,
    "%to" => $to,
    "%review" => $rs['title'],
  );
  watchdog('statistics', "Sending statistics mail of type %type to %to about the review %review.", $watchdog_vars, WATCHDOG_INFO);
  drupal_mail('statistics', 'pageviews_mail', $to, language_default(), $params, $site_email);
  _log_mail_sent($nid, $mail_type);
}

function _editor_emails($exclude) {
  $admin_rid = 3;
  $editor_rid = 5;
  $sql = "SELECT u.mail ";
  $sql.= "FROM users u ";
  $sql.= "INNER JOIN users_roles ur ON u.uid = ur.uid ";
  $sql.= "WHERE ur.rid = :editor OR ur.rid = :admin  ";
  $rs = db_query($sql, array(':editor' => $editor_rid, ':admin' => $admin_rid));

  $mails = array();
  foreach ($rs as $row) {
    if ($row->mail != $exclude) {
      $mails[] = $row->mail;
    }
  }

  return implode(", ", $mails);
}

/**
 * Implements hook_mail.
 */
function statistics_mail($key, &$message, $params) {
  switch ($key) {
    case 'pageviews_mail':
      $message['subject'] = $params['subject'];
      $message['body'][] = $params['body'];
      break;
  }
}


function _log_mail_sent($nid, $mail_type) {
  try {
    db_insert('analytics_mail_log')
      ->fields(array(
        'nid' => $nid,
        'mail_sent' => $mail_type,
        'time_created' => time(),
      ))
      ->execute();
  } catch (Exception $e) {
  }
}

function _has_mail_been_sent($nid, $mail_type) {
  $sql = "SELECT COUNT(*) AS count FROM analytics_mail_log l WHERE l.nid = :nid AND l.mail_sent = :mail_sent LIMIT 1";
  return (db_query($sql, array('nid' => $nid, 'mail_sent' => $mail_type))->fetchAssoc()['count'] > 0) ? true : false;
}

function _statistics_mail_form() {
  $form = array();

  $form['low'] = array(
    '#type' => 'fieldset',
    '#title' => t('Low threshold'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['low']['email_low_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email subject'),
    '#default_value' => variable_get('email_low_subject'),
    '#maxlength' => 128,
    '#required' => TRUE,
    '#description' => t('Placeholders: :REVIEW_NAME'),
  );
  $form['low']['email_low_contents'] = array(
    '#type' => 'textarea',
    '#title' => t('Email contents'),
    '#default_value' => variable_get('email_low_contents'),
    '#required' => TRUE,
    '#description' => t('Placeholders: :AUTHOR, :REVIEW_NAME, :REVIEW_URL, :PAGEVIEWS'),
  );

  $form['high'] = array(
    '#type' => 'fieldset',
    '#title' => t('High threshold'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['high']['email_high_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email subject'),
    '#default_value' => variable_get('email_high_subject'),
    '#maxlength' => 128,
    '#required' => TRUE,
    '#description' => t('Placeholders: :REVIEW_NAME'),
  );
  $form['high']['email_high_contents'] = array(
    '#type' => 'textarea',
    '#title' => t('Email contents'),
    '#default_value' => variable_get('email_high_contents'),
    '#required' => TRUE,
    '#description' => t('Placeholders: :AUTHOR, :REVIEW_NAME, :REVIEW_URL :PAGEVIEWS'),
  );

  return system_settings_form($form);
}
