<?php

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
    "tagline" => $node->field_tagline[LANGUAGE_NONE][0]['safe_value'],
    "summary" => $node->body[LANGUAGE_NONE][0]['safe_summary'],
    "bodyHtml" => $node->body[LANGUAGE_NONE][0]['safe_value'],
    "youtubeEmbedCode" => $node->field_embed_code[LANGUAGE_NONE][0]['safe_value'],
    "dates" => array(
      "created" => (int) $node->created,
      "changed" => (int) $node->changed,
      "publishedAt" => (int) $published_at
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
    "releaseDate" => (int) $release->field_release_date_value,
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
