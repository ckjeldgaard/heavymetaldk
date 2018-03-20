'use strict';

import gulp from 'gulp';
import babel from 'gulp-babel';
import git from 'gulp-git';

gulp.task('deploy', async () => {

  const files = await filesChanged();
  console.log('filesChanged', files);

  return gulp.src('src/**/*.js')
    .pipe(babel())
    .pipe(gulp.dest('dist'));
});

function lastCommit() {
  return new Promise(resolve => {
    git.exec({args : 'log --format="%H" -n 1'}, (err, stdout) => {
      resolve(stdout);
    });
  });
}

async function filesChanged() {
  const commitId = await lastCommit();
  console.log('commitId', commitId);
  const diff = 'diff-tree --no-commit-id --name-only -r ' + commitId;
  return new Promise(resolve => {
    git.exec({args : diff}, (err, stdout) => {
      resolve(stdout);
    });
  });
}

/* var gulp = require('gulp');
var git = require('gulp-git');
var ftp = require('vinyl-ftp');
var gutil = require('gulp-util');
var minimist = require('minimist');
var args = minimist(process.argv.slice(2));

gulp.task('deploy', function() {
  console.log('user', args.user);
  console.log('password', args.password);

  var l = log();
  console.log('log = ', l);

  var remotePath = '/web/html/heavymetal.dk/';
  var conn = ftp.create({
    host: 'heavymetal.dk',
    user: args.user,
    password: args.password,
    log: gutil.log
  });
  gulp.src(['./www/'])
    .pipe(conn.dest(remotePath));

});

var log = function() {
  git.exec({args : 'log --format="%H" -n 1'}, function (err, stdout) {
    console.log('sdfsdf', stdout);
    return stdout;
  });
};
*/
