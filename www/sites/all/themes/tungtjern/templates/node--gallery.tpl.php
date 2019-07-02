<article>

<div class="pure-g byline">
    <div class="pure-u-2-3 pure-u-md-4-5">
        <p class="post-meta">
        <?php print _get_type($node->type); ?>
        <span class="author">
            <i class="fa fa-user"></i> Af <?php print ($node->uid > 0) ? l($node->name, 'user/' . $node->uid) : t('Anonymous'); ?>, <?php print formatted_date($node->published_at); ?>
        </span>
        <?php if ($node->comment == COMMENT_NODE_OPEN) : ?>
        <span class="comments">
            <i class="fa fa-comments"></i> <a href="#comments"><?php print format_plural($node->comment_count, '1 comment', '@count comments'); ?></a>
        </span>
        <?php endif; ?>
        </p>
    </div>
    <div class="pure-u-1-3 pure-u-md-1-5 fb-actions">
        <div class="fb-like" data-href="<?php print url('node/' . $node->nid, array('absolute' => TRUE)); ?>" data-layout="button_count" data-action="like" data-show-faces="false" data-share="true"></div>
    </div>
</div>

<h1><?php print $node->title; ?></h1>

<?php if (isset($node->concert)) : ?>
  <p><strong>
  <?php if($node->concert->field_is_festival[LANGUAGE_NONE][0]['value']): ?>
    <?php print t('Festival'); ?>:
<?php else: ?>
    <?php print t('Concert'); ?>:
<?php endif; ?></strong>
  <?php print l($node->concert->title, 'node/' . $node->concert->nid); ?>
  </p>
<?php endif; ?>

<?php if (isset($node->body[LANGUAGE_NONE][0])) : ?>
  <?php if (strlen($node->body[LANGUAGE_NONE][0]['summary']) > 0) : ?>
    <p class="summary"><?php print $node->body[LANGUAGE_NONE][0]['summary']; ?></p>
  <?php endif; ?>
  <?php print $node->body[LANGUAGE_NONE][0]['safe_value']; ?>
<?php endif; ?>

<div class="gallery">
<?php foreach ($node->field_photos[LANGUAGE_NONE] as $img) : ?>
  <a href="<?php print image_cache('gallery_large', $img); ?>" data-lightbox="concertgallery" <?php if (strlen($img['title']) > 0): ?>data-title="<?php print check_plain($img['title']); ?>"<?php endif; ?>><img src="<?php print image_cache('gallery_thumbnail', $img); ?>" alt="<?php print check_plain($img['alt']); ?>" /></a>
<?php endforeach; ?>
</div>

</article>

<?php print render($content['comments']); ?>