var gulp = require('gulp');
var ftp = require('vinyl-ftp');
var gutil = require('gulp-util');
var minimist = require('minimist');
var args = minimist(process.argv.slice(2));

gulp.task('deploy', function() {
  var remotePath = '/web/html/dev.heavymetal.dk/';
  var conn = ftp.create({
    host: 'heavymetal.dk',
    user: args.user,
    password: args.password,
    log: gutil.log
  });
  gulp.src(['www/index.php'])
    .pipe(conn.newer(remotePath))
    .pipe(conn.dest(remotePath));
});
