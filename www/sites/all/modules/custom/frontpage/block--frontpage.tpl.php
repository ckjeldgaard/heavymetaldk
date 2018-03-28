<article class="frontpage-mosaic">
  <!-- <pre><?php
    print_r($content);
    ?></pre> -->

  <section class="promoted slider">
    <?php for ($i = 0; $i < 4; $i++): ?>
      <div>
        <?php $node = node_load(32321); ?>
        <?php $node_view = node_view($node, 'topfront'); ?>
        <?php print render($node_view); ?>
      </div>
    <?php endfor; ?>
  </section>

  <h2><?php print t('Danish content'); ?></h2>
  <section class="danish pure-g focus slider">
    <?php for ($i = 0; $i < 6; $i++): ?>
      <div class="pure-u-1-1 pure-u-sm-1-3 l-box <?php if ($i % 3 == 0): ?>first<?php endif; ?> <?php if ($i % 3 == 2): ?>last<?php endif; ?>">
        <?php
        $n = 32521;
        if ($i % 2 == 1) {
          $n = 32596;
        }
        if ($i % 3 == 2) {
          $n = 32450;
        }
        ?>
        <?php $node = node_load($n); ?>
        <?php $node_view = node_view($node, 'focus'); ?>
        <?php print render($node_view); ?>
      </div>
    <?php endfor; ?>
  </section>

  <h2><?php print t('International content'); ?></h2>
  <section class="danish pure-g focus slider">
    <?php for ($i = 0; $i < 6; $i++): ?>
      <div class="pure-u-1-1 pure-u-sm-1-3 l-box <?php if ($i % 3 == 0): ?>first<?php endif; ?> <?php if ($i % 3 == 2): ?>last<?php endif; ?>">
        <?php
        $n = 32521;
        if ($i % 2 == 1) {
          $n = 32596;
        }
        if ($i % 3 == 2) {
          $n = 32450;
        }
        ?>
        <?php $node = node_load($n); ?>
        <?php $node_view = node_view($node, 'focus'); ?>
        <?php print render($node_view); ?>
      </div>
    <?php endfor; ?>
  </section>

</article>
