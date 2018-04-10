<article class="frontpage-mosaic">

  <section class="promoted slider">
    <?php foreach($content['promoted'] as $n): ?>
      <div>
        <?php print $n['view']; ?>
      </div>
    <?php endforeach; ?>
  </section>

  <h2><?php print t('Danish content'); ?></h2>
  <section class="danish pure-g focus slider">
    <?php foreach($content['danish'] as $n): ?>
      <div class="pure-u-1-1 pure-u-sm-1-3 l-box <?php print $n['classes']; ?>">
        <?php print $n['view']; ?>
      </div>
    <?php endforeach; ?>
  </section>

  <h2><?php print t('International content'); ?></h2>
  <section class="international pure-g focus slider">
    <?php foreach($content['international'] as $n): ?>
      <div class="pure-u-1-1 pure-u-sm-1-3 l-box <?php print $n['classes']; ?>">
        <?php print $n['view']; ?>
      </div>
    <?php endforeach; ?>
  </section>

  <?php foreach($content['additional'] as $n): ?>
    <div class="additional">
      <?php print $n['view']; ?>
    </div>
  <?php endforeach; ?>

</article>
