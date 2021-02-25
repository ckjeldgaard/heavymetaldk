<div class="detektoren-page">
  <div class="pure-g detektoren-overview">
    <div class="pure-u-1 pure-u-sm-2-3 pure-u-md-2-3 pure-u-lg-2-3">
      <h1>Detektoren</h1>
      <p><?php $settings = variable_get('detektoren_overview_text'); print $settings['value']; ?></p>
    </div>
    <div class="pure-u-1 pure-u-sm-1-3 pure-u-md-1-3 pure-u-lg-1-3">
      <img src="/sites/all/themes/tungtjern/img/detektoren-logo.png" class="pure-img" alt="Detektoren" />
    </div>
  </div>

  <?php print views_embed_view('detektoren_anmeldelser', 'block_1'); ?>
</div>
