<?php

function _output_concert_review($node, $published_at) {

  $concert_review = array(
    "nid" => (int) $node->nid,
    "title" => $node->title,
    "url" => url('node/' . $node->nid, array('absolute' => TRUE)),
    "concert" => _output_concert($node->field_concert[LANGUAGE_NONE][0]['target_id']),
    "rating" => (isset($node->field_rating[LANGUAGE_NONE][0]['value'])) ? (int) $node->field_rating[LANGUAGE_NONE][0]['value'] : NULL,
    "tagline" => (isset($node->field_tagline[LANGUAGE_NONE][0]['safe_value'])) ? $node->field_tagline[LANGUAGE_NONE][0]['safe_value'] : $node->field_tagline[LANGUAGE_NONE][0]['value'],
    "summary" => (isset($node->body[LANGUAGE_NONE][0]['safe_summary'])) ? $node->body[LANGUAGE_NONE][0]['safe_summary'] : $node->body[LANGUAGE_NONE][0]['summary'],
    "bodyHtml" => (isset($node->body[LANGUAGE_NONE][0]['safe_value'])) ? $node->body[LANGUAGE_NONE][0]['safe_value'] : $node->body[LANGUAGE_NONE][0]['value'],
    "reviews" => NULL,
    "conclusionHtml" => (isset($node->field_conclusion[LANGUAGE_NONE][0]['safe_value'])) ? $node->field_conclusion[LANGUAGE_NONE][0]['safe_value'] : $node->field_conclusion[LANGUAGE_NONE][0]['value'],
    "primaryImage" => image_cache('story', $node->field_image[LANGUAGE_NONE][0]),
    "dates" => array(
      "created" => date('c', $node->created),
      "changed" => date('c', $node->changed),
      "publishedAt" => date('c', $published_at)
    ),
    "author" => array(
      "uid" => (int) $node->uid,
      "name" => $node->name,
      "url" => url('user/' . $node->uid, array('absolute' => TRUE)),
    ),
  );

  foreach ($node->field_concert_reviews[LANGUAGE_NONE] as $c) {
    $item = field_collection_item_load($c['value']);
    $concert_review['reviews'][] = array(
      "headline" => (isset($item->field_headline[LANGUAGE_NONE][0]['safe_value'])) ? $item->field_headline[LANGUAGE_NONE][0]['safe_value'] : $item->field_headline[LANGUAGE_NONE][0]['value'],
      "reviewTextHtml" => (isset($item->field_review_text[LANGUAGE_NONE][0]['safe_value'])) ? $item->field_review_text[LANGUAGE_NONE][0]['safe_value'] : $item->field_review_text[LANGUAGE_NONE][0]['value'],
      "rating" => (isset($item->field_rating[LANGUAGE_NONE][0]['value'])) ? (int) $item->field_rating[LANGUAGE_NONE][0]['value'] : NULL,
      "image" => (isset($item->field_image[LANGUAGE_NONE][0])) ? image_cache('story', $item->field_image[LANGUAGE_NONE][0]) : null,
    );
  }

  return $concert_review;
}

function _output_review($node, $published_at) {

  $title_parts = explode(" - ", $node->title);

  $review = array(
    "nid" => (int) $node->nid,
    "title" => $node->title,
    "artistName" => $title_parts[0],
    "releaseTitle" => $title_parts[1],
    "url" => url('node/' . $node->nid, array('absolute' => TRUE)),
    "apiHref" => url('api/' . API_VERSION . '/reviews/' . $node->nid, array('absolute' => TRUE, 'query' => array('api_key' => check_plain($_GET['api_key'])))),
    "release" => _output_release($node->field_release[LANGUAGE_NONE][0]['target_id']),
    "rating" => (int) $node->field_rating[LANGUAGE_NONE][0]['value'],
    "tagline" => (isset($node->field_tagline[LANGUAGE_NONE][0]['safe_value'])) ? $node->field_tagline[LANGUAGE_NONE][0]['safe_value'] : $node->field_tagline[LANGUAGE_NONE][0]['value'],
    "summary" => (isset($node->body[LANGUAGE_NONE][0]['safe_summary'])) ? $node->body[LANGUAGE_NONE][0]['safe_summary'] : $node->body[LANGUAGE_NONE][0]['summary'],
    "bodyHtml" => (isset($node->body[LANGUAGE_NONE][0]['safe_value'])) ? $node->body[LANGUAGE_NONE][0]['safe_value'] : $node->body[LANGUAGE_NONE][0]['value'],
    "youtubeEmbedCode" => $node->field_embed_code[LANGUAGE_NONE][0]['safe_value'],
    "dates" => array(
      "created" => date('c', $node->created),
      "changed" => date('c', $node->changed),
      "publishedAt" => date('c', $published_at)
    ),
    "author" => array(
      "uid" => (int) $node->uid,
      "name" => $node->name,
      "url" => url('user/' . $node->uid, array('absolute' => TRUE)),
    ),
    "meta" => array(
      "classic" => ($node->field_classic[LANGUAGE_NONE][0]['value'] == 1) ? TRUE : FALSE,
      "detektoren" => ($node->field_detektoren[LANGUAGE_NONE][0]['value'] == 1) ? TRUE : FALSE,
    )
  );

  $genres = array();
  foreach ($node->field_genre[LANGUAGE_NONE] as $g) {
    $genres[] = _output_genre($g['target_id']);
  }
  $review['genres'] = $genres;

  return $review;
}

function _output_concert($concert_nid) {
  $sql = <<<EOT
  SELECT n.title,
  v.field_venue_tid,
  c.field_cancelled_value,
  isf.field_is_festival_value,
  fn.field_festival_name_value,
  e.field_event_date_value,
  e.field_event_date_value2,
  st.field_start_time_value,
  tp.field_ticket_price_value,
  bl.field_buy_link_url,
  fb.field_facebook_event_url
  FROM node n
  INNER JOIN field_data_field_venue v ON v.entity_id = n.nid AND v.bundle = 'concert'
  INNER JOIN field_data_field_cancelled c ON c.entity_id = n.nid
  INNER JOIN field_data_field_is_festival isf ON isf.entity_id = n.nid
  LEFT JOIN field_data_field_festival_name fn ON fn.entity_id = n.nid
  INNER JOIN field_data_field_event_date e ON e.entity_id = n.nid AND e.bundle = 'concert'
  LEFT JOIN field_data_field_start_time st ON st.entity_id = n.nid AND st.bundle = 'concert'
  LEFT JOIN field_data_field_ticket_price tp ON tp.entity_id = n.nid AND tp.bundle = 'concert'
  LEFT JOIN field_data_field_buy_link bl ON bl.entity_id = n.nid AND bl.bundle = 'concert'
  LEFT JOIN field_data_field_facebook_event fb ON fb.entity_id = n.nid AND fb.bundle = 'concert'
  WHERE n.nid = :concert_nid AND n.status = :status
EOT;

  $params = array(
    ":concert_nid" => $concert_nid, ":status" => 1
  );
  $rs = db_query($sql, $params)->fetchObject();

  $concert_review = array(
    "nid" => (int) $concert_nid,
    "title" => $rs->title,
    "venue" => _output_venue($rs->field_venue_tid),
    "cancelled" => (bool) $rs->field_cancelled_value,
    "eventDate" => array(
      "startDate" => date('c', $rs->field_event_date_value),
      "endDate" => date('c', $rs->field_event_date_value2),
    ),
    "festival" => array(
      "isFestival" => (bool) $rs->field_is_festival_value,
      "festivalName" => $rs->field_festival_name_value,
    ),
    "meta" => array(
      "startTime" => $rs->field_start_time_value,
      "ticketPrice" => (int) $rs->field_ticket_price_value,
      "buyLink" => $rs->field_buy_link_url,
      "facebookEventUrl" => $rs->field_facebook_event_url,
    )
  );

  // Headliners:
  $sql_headliners = "SELECT a.field_artists_target_id FROM field_data_field_artists a WHERE a.entity_id = :concert_nid AND a.bundle = :bundle";
  $rs = db_query($sql_headliners, array(':concert_nid' => $concert_nid, ':bundle' => 'concert'));
  $headliners = array();
  foreach ($rs as $row) {
    $headliners[] = _output_artist($row->field_artists_target_id);
  }
  $concert_review["headlinerArtists"] = $headliners;

  // Support artists:
  $sql_support = "SELECT a.field_support_artists_target_id FROM field_data_field_support_artists a WHERE a.entity_id = :concert_nid AND a.bundle = :bundle";
  $rs = db_query($sql_support, array(':concert_nid' => $concert_nid, ':bundle' => 'concert'));
  $support = array();
  foreach ($rs as $row) {
    $support[] = _output_artist($row->field_support_artists_target_id);
  }
  $concert_review["supportArtists"] = $support;

  return $concert_review;
}

function _output_release($release_nid) {
  $sql = <<<EOT
  SELECT n.title,
  t.field_release_title_value,
  type.field_release_type_value,
  rdate.field_release_date_value,
  a.field_artist_target_id,
  l.field_label_tid,
  f.uri,
  rt.field_running_time_value,
  tr.field_tracks_value,
  tl.field_tracklist_value
  FROM node n
  INNER JOIN field_data_field_release_title t ON t.entity_id = n.nid AND t.bundle = 'release'
  INNER JOIN field_data_field_release_type type ON type.entity_id = n.nid AND type.bundle = 'release'
  INNER JOIN field_data_field_release_date rdate ON rdate.entity_id = n.nid AND rdate.bundle = 'release'
  INNER JOIN field_data_field_artist a ON a.entity_id = n.nid AND a.bundle = 'release'
  INNER JOIN field_data_field_label l ON l.entity_id = n.nid AND l.bundle = 'release'
  INNER JOIN field_data_field_image img ON img.entity_id = n.nid AND img.bundle = 'release'
  INNER JOIN file_managed f ON f.fid = img.field_image_fid
  LEFT JOIN field_data_field_running_time rt ON rt.entity_id = n.nid
  LEFT JOIN field_data_field_tracks tr ON tr.entity_id = n.nid
  LEFT JOIN field_data_field_tracklist tl ON tl.entity_id = n.nid
  WHERE n.nid = :review_nid
EOT;

  $params = array(
    ":review_nid" => $release_nid
  );
  $release = db_query($sql, $params)->fetchObject();

  $image = new stdClass();
  $image->uri = $release->uri;

  $release = array(
    "nid" => (int) $release_nid,
    "title" => $release->title,
    "releaseTitle" => $release->field_release_title_value,
    "releaseType" => $release->field_release_type_value,
    "releaseDate" => date('c', $release->field_release_date_value),
    "artist" => _output_artist($release->field_artist_target_id),
    "label" => _output_label($release->field_label_tid),
    "images" => array(
      "thumb" => image_cache('review_thumbnail', $image),
      "large" => image_cache('review', $image)
    ),
    "runningTime" => $release->field_running_time_value,
    "tracks" => (int) $release->field_tracks_value,
    "tracklist" => $release->field_tracklist_value,
  );
  return $release;
}

function _output_artist($nid) {
  $sql = <<<EOT
  SELECT n.title,
  d.field_description_value,
  c.field_country_iso2
  FROM node n
  LEFT JOIN field_data_field_description d ON d.entity_id = n.nid AND d.bundle = 'artist'
  LEFT JOIN field_data_field_country c ON c.entity_id = n.nid AND c.bundle = 'artist'
  WHERE n.nid = :artist_nid AND n.status = :status
EOT;

  $params = array(":artist_nid" => $nid, ":status" => 1);
  $artist = db_query($sql, $params)->fetchObject();

  $artist = array(
    "nid" => (int) $nid,
    "title" => $artist->title,
    "url" => url('node/' . $nid, array('absolute' => TRUE)),
    "description" => $artist->field_description_value,
    "countryIso" => $artist->field_country_iso2,
  );
  return $artist;
}

function _output_genre($tid) {
  $sql = <<<EOT
  SELECT t.name
  FROM taxonomy_term_data t
  WHERE t.tid = :genre_tid
EOT;

  $genre = db_query($sql, array(":genre_tid" => $tid))->fetchObject();

  $genre = array(
    "tid" => (int) $tid,
    "title" => $genre->name,
    "url" => url('taxonomy/term/' . $tid, array('absolute' => TRUE)),
  );
  return $genre;
}

function _output_venue($tid) {
  $sql = <<<EOT
  SELECT t.name,
  a.field_address_thoroughfare,
  a.field_address_postal_code,
  a.field_address_locality,
  a.field_address_country,
  p.field_province_value,
  w.field_website_link_url
  FROM taxonomy_term_data t
  INNER JOIN field_data_field_address a ON a.entity_id = t.tid AND a.bundle = 'venue'
  INNER JOIN field_data_field_province p ON p.entity_id = t.tid AND p.bundle = 'venue'
  LEFT JOIN field_data_field_website_link w ON w.entity_id = t.tid AND w.bundle = 'venue'
  WHERE t.tid = :venue_tid
EOT;

  $rs = db_query($sql, array(":venue_tid" => $tid))->fetchObject();

  $venue = array(
    "tid" => (int) $tid,
    "title" => $rs->name,
    "url" => url('taxonomy/term/' . $tid, array('absolute' => TRUE)),
    "address" => array(
      "streetAddress" => $rs->field_address_thoroughfare,
      "postalCode" => $rs->field_address_postal_code,
      "city" => $rs->field_address_locality,
      "countryIso" => $rs->field_address_country
    ),
    "province" => $rs->field_province_value,
    "website" => $rs->field_website_link_url
  );
  return $venue;
}

function _output_label($tid) {
  $sql = <<<EOT
  SELECT t.name,
  c.field_country_iso2,
  w.field_website_link_url
  FROM taxonomy_term_data t 
  LEFT JOIN field_data_field_country c ON c.entity_id = t.tid AND c.bundle = 'label'
  LEFT JOIN field_data_field_website_link w ON w.entity_id = t.tid AND w.bundle = 'label'
  WHERE t.tid = :label_tid
EOT;

  $params = array(":label_tid" => $tid);
  $label = db_query($sql, $params)->fetchObject();

  $label = array(
    "tid" => (int) $tid,
    "title" => $label->name,
    "url" => url('taxonomy/term/' . $tid, array('absolute' => TRUE)),
    "countryIso" => $label->field_country_iso2,
    "websiteUrl" => $label->field_website_link_url
  );
  return $label;
}
