  <?php if ($rows): ?>
    <div class="view-content focus-list">
    <section class="promoted slider">
      <?php print $rows; ?>
      </section>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>
