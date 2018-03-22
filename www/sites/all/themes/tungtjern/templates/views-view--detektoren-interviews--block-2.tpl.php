<h2 id="interviews"><?php print t('Latest Detektoren interviews'); ?></h2>
  <?php if ($rows): ?>
    <div class="view-content focus-list">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
  <?php endif; ?>
<?php print l(t('More Detektoren interviews'), 'detektoren-interviews', array('attributes' => array('class' => array('pure-button', 'pure-button-primary')))); ?>
