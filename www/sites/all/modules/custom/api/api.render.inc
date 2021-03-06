<?php

function _output_concert_review($node, $published_at) {

  $concert_review = array(
    "nid" => (int) $node->nid,
    "title" => $node->title,
    "url" => url('node/' . $node->nid, array('absolute' => TRUE)),
    "apiHref" => url('api/' . API_VERSION . '/concert_reviews/' . $node->nid, array('absolute' => TRUE)),
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
    "apiHref" => url('api/' . API_VERSION . '/reviews/' . $node->nid, array('absolute' => TRUE)),
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

function _output_concert_node($node, $published_at) {
  $concert = _output_concert($node->nid);

  $concert["dates"] = array(
    "created" => date('c', $node->created),
    "changed" => date('c', $node->changed),
    "publishedAt" => date('c', $published_at)
  );
  $concert["author"] = array(
    "uid" => (int) $node->uid,
    "name" => $node->name,
    "url" => url('user/' . $node->uid, array('absolute' => TRUE)),
  );
  return $concert;
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

  $ticket_price = (int) $rs->field_ticket_price_value;
  $concert = array(
    "nid" => (int) $concert_nid,
    "title" => $rs->title,
    "url" => url('node/' . $concert_nid, array('absolute' => TRUE)),
    "apiHref" => url('api/' . API_VERSION . '/concerts/' . $concert_nid, array('absolute' => TRUE)),
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
      "ticketPrice" => ($ticket_price > 0) ? $ticket_price : NULL,
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
  $concert["headlinerArtists"] = $headliners;

  // Support artists:
  $sql_support = "SELECT a.field_support_artists_target_id FROM field_data_field_support_artists a WHERE a.entity_id = :concert_nid AND a.bundle = :bundle";
  $rs = db_query($sql_support, array(':concert_nid' => $concert_nid, ':bundle' => 'concert'));
  $support = array();
  foreach ($rs as $row) {
    $support[] = _output_artist($row->field_support_artists_target_id);
  }
  $concert["supportArtists"] = $support;

  return $concert;
}

function _output_interview($node, $published_at) {

  $interview = array(
    "nid" => (int) $node->nid,
    "title" => $node->title,
    "url" => url('node/' . $node->nid, array('absolute' => TRUE)),
    "apiHref" => url('api/' . API_VERSION . '/interviews/' . $node->nid, array('absolute' => TRUE)),
    "primaryImage" => image_cache('top_image', $node->field_image[LANGUAGE_NONE][0]),
    "summary" => (isset($node->body[LANGUAGE_NONE][0]['safe_summary'])) ? $node->body[LANGUAGE_NONE][0]['safe_summary'] : $node->body[LANGUAGE_NONE][0]['summary'],
    "bodyHtml" => (isset($node->body[LANGUAGE_NONE][0]['safe_value'])) ? $node->body[LANGUAGE_NONE][0]['safe_value'] : $node->body[LANGUAGE_NONE][0]['value'],
    "artist" => (isset($node->field_artist[LANGUAGE_NONE][0]['target_id'])) ? _output_artist($node->field_artist[LANGUAGE_NONE][0]['target_id']) : NULL,
    "youtubeEmbedCode" => (isset($node->field_embed_code[LANGUAGE_NONE][0]['value'])) ? $node->field_embed_code[LANGUAGE_NONE][0]['value'] : NULL,
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
      "detektoren" => ($node->field_detektoren[LANGUAGE_NONE][0]['value'] == 1) ? TRUE : FALSE,
    )
  );

  return $interview;
}

function _output_news($node, $published_at) {

  $artists = array();
  foreach ($node->field_artists[LANGUAGE_NONE] as $a) {
    $artists[] = _output_artist($a['target_id']);
  }

  $news = array(
    "nid" => (int) $node->nid,
    "title" => $node->title,
    "url" => url('node/' . $node->nid, array('absolute' => TRUE)),
    "apiHref" => url('api/' . API_VERSION . '/news/' . $node->nid, array('absolute' => TRUE)),
    "primaryImage" => (isset($node->field_image[LANGUAGE_NONE][0])) ? image_cache('top_image', $node->field_image[LANGUAGE_NONE][0]) : NULL,
    "summary" => (isset($node->body[LANGUAGE_NONE][0]['safe_summary'])) ? $node->body[LANGUAGE_NONE][0]['safe_summary'] : $node->body[LANGUAGE_NONE][0]['summary'],
    "bodyHtml" => (isset($node->body[LANGUAGE_NONE][0]['safe_value'])) ? $node->body[LANGUAGE_NONE][0]['safe_value'] : $node->body[LANGUAGE_NONE][0]['value'],
    "artists" => $artists,
    "youtubeEmbedCode" => (isset($node->field_embed_code[LANGUAGE_NONE][0]['value'])) ? $node->field_embed_code[LANGUAGE_NONE][0]['value'] : NULL,
    "source" => array(
      "title" => $node->field_source[LANGUAGE_NONE][0]['title'],
      "url" => $node->field_source[LANGUAGE_NONE][0]['url']
    ),
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

  return $news;
}

function _output_reportage($node, $published_at) {

  $tabs = array();
  if (isset($node->field_tabs[LANGUAGE_NONE])) {
    for ($i = 0; $i < count($node->field_tabs[LANGUAGE_NONE]); $i++) {
      $tab_item = field_collection_item_load($node->field_tabs[LANGUAGE_NONE][$i]['value']);
      if (isset($tab_item->field_tab_headline[LANGUAGE_NONE][0]['machine']) && count($tab_item->field_reportage_review[LANGUAGE_NONE]) > 0) {

        $reviews = array();
        foreach ($tab_item->field_reportage_review[LANGUAGE_NONE] as $r) {
          $review = field_collection_item_load($r['value']);

          $author = NULL;
          if (isset($review->field_author[LANGUAGE_NONE][0])) {
            $a = user_load($review->field_author[LANGUAGE_NONE][0]['target_id']);
            $author = array(
              "uid" => (int) $a->uid,
              "name" => $a->name,
              "url" => url('user/' . $a->uid, array('absolute' => TRUE)),
            );
          }

          $reviews[] = array(
            "artist" => (isset($review->field_artist[LANGUAGE_NONE][0]['target_id'])) ? _output_artist($review->field_artist[LANGUAGE_NONE][0]['target_id']) : NULL,
            "reviewTextHtml" => $review->field_review_text[LANGUAGE_NONE][0]['value'],
            "rating" => (int) $review->field_rating[LANGUAGE_NONE][0]['value'],
            "image" => (isset($review->field_image[LANGUAGE_NONE][0])) ? image_cache('story', $review->field_image[LANGUAGE_NONE][0]) : NULL,
            "author" => $author,
            "stage" => (isset($review->field_stage[LANGUAGE_NONE][0])) ? $review->field_stage[LANGUAGE_NONE][0]['value'] : NULL,
            "time" => (isset($review->field_time[LANGUAGE_NONE][0])) ? date('c', $review->field_time[LANGUAGE_NONE][0]['value']) : NULL,
          );
        }

        $tabs[] = array(
          "title" => $tab_item->field_tab_headline[LANGUAGE_NONE][0]['human'],
          "machineName" => $tab_item->field_tab_headline[LANGUAGE_NONE][0]['machine'],
          "reviews" => $reviews
        );
      }


    }
  }

  $concert_ref = $node->field_concert_reference[LANGUAGE_NONE][0]['target_id'];

  $reportage = array(
    "nid" => (int) $node->nid,
    "title" => $node->title,
    "url" => url('node/' . $node->nid, array('absolute' => TRUE)),
    "apiHref" => url('api/' . API_VERSION . '/reportages/' . $node->nid, array('absolute' => TRUE)),
    "primaryImage" => (isset($node->field_image[LANGUAGE_NONE][0])) ? image_cache('top_image', $node->field_image[LANGUAGE_NONE][0]) : NULL,
    "tagline" => (isset($node->field_tagline[LANGUAGE_NONE][0]['safe_value'])) ? $node->field_tagline[LANGUAGE_NONE][0]['safe_value'] : $node->field_tagline[LANGUAGE_NONE][0]['value'],
    "summary" => (isset($node->body[LANGUAGE_NONE][0]['safe_summary'])) ? $node->body[LANGUAGE_NONE][0]['safe_summary'] : $node->body[LANGUAGE_NONE][0]['summary'],
    "bodyHtml" => (isset($node->body[LANGUAGE_NONE][0]['safe_value'])) ? $node->body[LANGUAGE_NONE][0]['safe_value'] : $node->body[LANGUAGE_NONE][0]['value'],
    "tabs" => $tabs,
    "concert" => ($concert_ref != NULL) ? _output_concert($concert_ref) : NULL,
    "dates" => array(
      "created" => date('c', $node->created),
      "changed" => date('c', $node->changed),
      "publishedAt" => date('c', $published_at)
    ),
    "author" => array(
      "uid" => (int) $node->uid,
      "name" => $node->name,
      "url" => url('user/' . $node->uid, array('absolute' => TRUE)),
      "display" => (bool) $node->field_display_author[LANGUAGE_NONE][0]['value']
    ),
  );

  return $reportage;
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

  $track_list = array();
  foreach(preg_split("/((\r?\n)|(\r\n?))/", $release->field_tracklist_value) as $track){
    $track = preg_replace("/\s+/", " ", $track);
    if (strlen($track) > 0) {
      $track_list[] = rtrim(preg_replace("/\s+/", " ", $track));
    }
  }

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
    "tracklist" => $track_list
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
    "apiHref" => url('api/' . API_VERSION . '/artists/' . $nid, array('absolute' => TRUE)),
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
    'apiHref' => url('api/' . API_VERSION . '/genres/' . $tid, array('absolute' => TRUE))
  );
  return $genre;
}

function _output_genre_detailed($term) {

  $sql = "SELECT n.nid, n.title, n.type FROM node n INNER JOIN taxonomy_index t ON t.nid = n.nid WHERE t.tid = :tid ORDER BY n.title";
  $refs = db_query($sql, array(":tid" => $term->tid));

  $artists = array();
  $reviews = array(); 
  foreach ($refs as $r) {
    if ($r->type == 'artist') {
      $artists[] = (object) [
        'nid' => (int) $r->nid,
        'title' => $r->title,
        'url' => url('node/' . $r->nid, array('absolute' => TRUE)),
        'apiHref' => url('api/' . API_VERSION . '/artists/' . $r->nid, array('absolute' => TRUE))
      ];
    } else if ($r->type == 'review') {
      $reviews[] = (object) [
        'nid' => (int) $r->nid,
        'title' => $r->title,
        'url' => url('node/' . $r->nid, array('absolute' => TRUE)),
        'apiHref' => url('api/' . API_VERSION . '/reviews/' . $r->nid, array('absolute' => TRUE))
      ];
    }
  }

  $genre = array(
    "tid" => (int) $term->tid,
    "title" => $term->name,
    "url" => url('taxonomy/term/' . $term->tid, array('absolute' => TRUE)),
    "descriptionHtml" => (strlen($term->description) > 0) ? $term->description : NULL,
    'artists' => $artists,
    'reviews' => $reviews,
  );

  return $genre;
}

function _output_venue_detailed($term) {
  $venue = _output_venue($term->tid);

  $concerts = array();
  $sql = "SELECT n.nid, n.title FROM node n INNER JOIN taxonomy_index t ON t.nid = n.nid INNER JOIN field_data_field_event_date d ON d.entity_id = n.nid WHERE t.tid = :tid AND n.type = :type ORDER BY d.field_event_date_value DESC";
  $refs = db_query($sql, array(":tid" => $term->tid, ":type" => "concert"));
  foreach ($refs as $r) {
    $concerts[] = (object) [
      'nid' => (int) $r->nid,
      'title' => $r->title,
      'url' => url('node/' . $r->nid, array('absolute' => TRUE)),
      'apiHref' => url('api/' . API_VERSION . '/concerts/' . $r->nid, array('absolute' => TRUE))
    ];
  }
  $venue['concerts'] = $concerts;

  return $venue;
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
    "apiHref" => url('api/' . API_VERSION . '/venues/' . $tid, array('absolute' => TRUE)),
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

function _output_artist_detailed($node, $published_at) {

  $artist = array(
    "nid" => (int) $node->nid,
    "title" => $node->title,
    "url" => url('node/' . $node->nid, array('absolute' => TRUE)),
    "apiHref" => url('api/' . API_VERSION . '/artists/' . $node->nid, array('absolute' => TRUE)),
    "descriptionHtml" => $node->field_description[LANGUAGE_NONE][0]['safe_value'],
    "logoImage" => (isset($node->field_logo[LANGUAGE_NONE][0])) ? image_cache('bandlogo', $node->field_logo[LANGUAGE_NONE][0]) : NULL,
    "countryIso" => (isset($node->field_country[LANGUAGE_NONE][0])) ? $node->field_country[LANGUAGE_NONE][0]['iso2'] : NULL,
    "websiteLink" => (isset($node->field_website_link[LANGUAGE_NONE][0])) ? $node->field_website_link[LANGUAGE_NONE][0]['url'] : NULL,
    "genres" => array(),
    "relatedArtists" => array(),
    "reviews" => array(),
    "interviews" => array(),
  );

  foreach ($node->field_artist_genres[LANGUAGE_NONE] as $g) {
    $artist['genres'][] = _output_genre($g['tid']);
  }

  foreach ($node->field_related_bands[LANGUAGE_NONE] as $a) {
    $artist['relatedArtists'][] = _output_artist($a['target_id']);
  }

  // Get reviews
  $sql = <<<EOT
  SELECT a.`entity_id` AS release_id, r.entity_id AS review_id, n.title
  FROM `field_data_field_artist` a
  INNER JOIN field_data_field_release r ON r.field_release_target_id = a.entity_id
  INNER JOIN field_data_field_release_date d ON d.entity_id = a.entity_id
  INNER JOIN node n ON n.nid = r.entity_id
  WHERE a.`field_artist_target_id` = :artist_id
  AND a.bundle = :bundle
  ORDER BY d.field_release_date_value DESC
EOT;
  $rs = db_query($sql, array(':artist_id' => $node->nid, ':bundle' => 'release'));
  foreach ($rs as $row) {
    $artist['reviews'][] = array(
      "nid" => (int) $row->review_id,
      "title" => $row->title,
      "url" => url('node/' . $row->review_id, array('absolute' => TRUE)),
      "apiHref" => url('api/' . API_VERSION . '/reviews/' . $row->review_id, array('absolute' => TRUE)),
    );
  }

  // Interviews
  $interviews = _get_artist_interview($node->nid);
  foreach ($interviews as $i) {
    $artist['interviews'][] = array(
      "nid" => (int) $i->nid,
      "title" => $i->title,
      "url" => url('node/' . $i->nid, array('absolute' => TRUE)),
      "apiHref" => url('api/' . API_VERSION . '/interviews/' . $i->nid, array('absolute' => TRUE)),
    );
  }

  return $artist;
}
