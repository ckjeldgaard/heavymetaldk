<?php

require_once __DIR__ . '/../firebase/google-api-php-client/vendor/autoload.php';

/**
 * Implements hook_cron().
 */
function statistics_cron() {
  $last_sync = variable_get('analytics_last_sync_run', 0);
  if ((time() - $last_sync) > 86400) {
    try {
      $analytics = _initialize_analytics();
      $profile = _get_first_profile_id($analytics);
      $results = _get_results($analytics, $profile);
      _save_results($results->getRows());

      watchdog('statistics', "Successfully updated :total reviews with Google Analytics data", array(':total' => $results->totalResults),WATCHDOG_INFO, NULL);
      variable_set('analytics_last_sync_run', time());
    } catch (Exception $e) {
      watchdog('statistics', "Couldn't initialize Google Analytics. Check service account credentials JSON file. Error message: @error", array('@error' => $e->getMessage()),WATCHDOG_ERROR, NULL);
    }
  }
  _send_pageview_emails();
}

function _get_results($analytics, $profileId) {
  return $analytics->data_ga->get(
    'ga:' . $profileId,
    '2014-10-01',
    'today',
    'ga:pageviews',
    array(
      'dimensions' => 'ga:pagePath',
      'sort' => '-ga:pageviews',
      'filters' => 'ga:pagePath=~^/anmeldelse/',
      'max-results' => '8000'
    )
  );
}

function _save_results($rows) {
  foreach ($rows as $row) {
    $nid = _get_nid_by_path($row[0]);
    if ($nid) {
      try {
        db_merge('analytics_pageviews')
          ->key(array('nid' => $nid))
          ->fields(array(
            'nid' => $nid,
            'pageviews' => $row[1],
          ))
          ->execute();
      } catch (Exception $e) {
        watchdog('statistics', "Error merging analytics_pageviews table: " . $e->getMessage(), NULL, WATCHDOG_ERROR);
      }
    }
  }
}

function _get_nid_by_path($path) {
  $sql = "SELECT u.source FROM url_alias u WHERE u.alias = :path LIMIT 1";
  $rs = db_query($sql, array(':path' => substr($path, 1)))->fetchField();
  $parts = explode("/", $rs);
  if ($rs) {
    return $parts[1];
  }
  return FALSE;
}

function _initialize_analytics() {
  $KEY_FILE_NAME = 'service-account-credentials.json';
  $KEY_FILE_LOCATION = DRUPAL_ROOT . '/' . variable_get('file_private_path') . '/' . $KEY_FILE_NAME;

  $client = new Google_Client();
  $client->setApplicationName("Heavymetal.dk");
  $client->setAuthConfig($KEY_FILE_LOCATION);
  $client->setScopes(['https://www.googleapis.com/auth/analytics.readonly']);
  $analytics = new Google_Service_Analytics($client);

  return $analytics;
}

function _get_first_profile_id($analytics) {
  $accounts = $analytics->management_accounts->listManagementAccounts();

  if (count($accounts->getItems()) > 0) {
    $items = $accounts->getItems();
    $firstAccountId = $items[0]->getId();

    $properties = $analytics->management_webproperties
      ->listManagementWebproperties($firstAccountId);

    if (count($properties->getItems()) > 0) {
      $items = $properties->getItems();
      $firstPropertyId = $items[0]->getId();
      $profiles = $analytics->management_profiles
        ->listManagementProfiles($firstAccountId, $firstPropertyId);

      if (count($profiles->getItems()) > 0) {
        $items = $profiles->getItems();
        // Return the first view (profile) ID.
        return $items[0]->getId();
      } else {
        throw new Exception('No views (profiles) found for this user.');
      }
    } else {
      throw new Exception('No properties found for this user.');
    }
  } else {
    throw new Exception('No accounts found for this user.');
  }
}
