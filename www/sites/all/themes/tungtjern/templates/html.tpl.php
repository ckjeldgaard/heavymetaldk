<!doctype html>
<html lang="<?php print $language->language; ?>">
<head>
  <title><?php print $head_title; ?></title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel="stylesheet" type="text/css" href="//fonts.googleapis.com/css?family=PT+Sans">
  <?php print $styles; ?>
  <?php print $scripts; ?>
  <?php print $head; ?>
</head>
<body>
  <div id="fb-root"></div>
  <script>(function(d, s, id) {
    var js, fjs = d.getElementsByTagName(s)[0];
    if (d.getElementById(id)) return;
    js = d.createElement(s); js.id = id;
    js.src = "//connect.facebook.net/en_US/sdk.js#xfbml=1&version=v2.0";
    fjs.parentNode.insertBefore(js, fjs);
  }(document, 'script', 'facebook-jssdk'));</script>
  <?php print $page; ?>
  
  <?php print $page_bottom; ?>
</body>
</html>
