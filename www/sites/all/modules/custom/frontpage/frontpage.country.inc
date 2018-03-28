<?php

function _is_danish($nid, $type) {
  switch ($type) {
    case 'review':
      return _is_danish_review($nid);
      break;
    case 'concert_review':
      return _is_danish_concert_review($nid);
      break;
    case 'reportage':
      return _is_danish_reportage($nid);
      break;
    case 'interview':
      return _is_danish_interview($nid);
      break;
    case 'blog':
      return _is_danish_blog($nid);
      break;
  }
  return false;
}

function _is_danish_review($nid) {
  $sql = "SELECT c.field_country_iso2 ";
  $sql.= "FROM field_data_field_country c ";
  $sql.= "INNER JOIN field_data_field_artist a ON a.field_artist_target_id = c.entity_id ";
  $sql.= "INNER JOIN field_data_field_release r ON r.field_release_target_id = a.entity_id ";
  $sql.= "WHERE r.entity_id = :nid ";
  $country = db_query($sql, array(':nid' => $nid))->fetchField();
  return ($country == 'DK');
}

/**
 * @param $nid
 * @return bool Returns TRUE if at least one of the headliners are Danish
 */
function _is_danish_concert_review($nid) {
  $sql = "SELECT c.field_country_iso2 AS country ";
  $sql.= "FROM field_data_field_country c ";
  $sql.= "INNER JOIN field_data_field_artists a ON a.field_artists_target_id = c.entity_id ";
  $sql.= "INNER JOIN field_data_field_concert n ON n.field_concert_target_id = a.entity_id ";
  $sql.= "WHERE n.entity_id = :nid ";

  $rs = db_query($sql, array(':nid' => $nid));
  foreach ($rs as $row) {
    if ($row->country == "DK") {
      return true;
    }
  }
  return false;
}

/**
 * Reportages are only considered Danish if ALL bands are Danish.
 *
 * @param $nid
 * @return bool
 */
function _is_danish_reportage($nid) {
  $sql = "SELECT c.field_country_iso2 AS country ";
  $sql.= "FROM field_data_field_country c ";
  $sql.= "INNER JOIN field_data_field_artists a ON a.field_artists_target_id = c.entity_id ";
  $sql.= "INNER JOIN field_data_field_concert_reference n ON n.field_concert_reference_target_id = a.entity_id ";
  $sql.= "WHERE n.entity_id = :nid ";

  $rs = db_query($sql, array(':nid' => $nid));
  foreach ($rs as $row) {
    if ($row->country != "DK") {
      return false;
    }
  }
  return true;
}

function _is_danish_interview($nid) {
  $sql = "SELECT c.field_country_iso2 ";
  $sql.= "FROM field_data_field_country c ";
  $sql.= "INNER JOIN field_data_field_artist a ON a.field_artist_target_id = c.entity_id ";
  $sql.= "WHERE a.entity_id = :nid AND a.bundle = :type";
  $country = db_query($sql, array(':nid' => $nid, ':type' => 'interview'))->fetchField();
  return ($country == 'DK');
}

/**
 * Blogs are only considered Danish if ALL bands are Danish.
 */
function _is_danish_blog($nid) {
  $sql = "SELECT c.field_country_iso2 AS country ";
  $sql.= "FROM field_data_field_artists a ";
  $sql.= "LEFT JOIN field_data_field_country c ON a.field_artists_target_id = c.entity_id ";
  $sql.= "WHERE a.entity_id = :nid ";

  $rs = db_query($sql, array(':nid' => $nid));
  foreach ($rs as $row) {
    if ($row->country != "DK") {
      return false;
    }
  }
  return true;
}
