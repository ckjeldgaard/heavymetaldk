<h1>Frontpage template</h1>
<pre><?php
  print_r($content);
  ?></pre>

<section class="regular slider">
  <?php for($i = 0; $i < 6; $i++): ?>
  <div>
    <?php $node = node_load(32321); ?>
    <?php $node_view = node_view($node, 'topfront'); ?>
    <?php print render($node_view); ?>
  </div>
  <?php endfor; ?>
  <!-- <div>
    <img src="http://placehold.it/350x300?text=2">
  </div>
  <div>
    <img src="http://placehold.it/350x300?text=3">
  </div>
  <div>
    <img src="http://placehold.it/350x300?text=4">
  </div>
  <div>
    <img src="http://placehold.it/350x300?text=5">
  </div>
  <div>
    <img src="http://placehold.it/350x300?text=6">
  </div> -->
</section>
