<h1><?php print t('Administer consent'); ?></h1>

<div class="consent">
  <label class="container"><strong><?php print variable_get('consent_analytics_headline'); ?>:</strong> <?php print variable_get('consent_analytics_text'); ?>
    <input type="checkbox" checked="checked" id="consent_ga_tracking" >
    <span class="checkmark"></span>
  </label>
</div>

<div class="consent">
  <label class="container"><strong><?php print variable_get('consent_cookies_headline'); ?>:</strong> <?php print variable_get('consent_cookies_text'); ?>
    <input type="checkbox" checked="checked" id="consent_cookies" >
    <span class="checkmark"></span>
  </label>
</div>
