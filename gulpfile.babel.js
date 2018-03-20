'use strict';

import gulp from 'gulp';
import git from 'gulp-git';
import ftp from 'vinyl-ftp';
import gutil from 'gulp-util';
import minimist from 'minimist';

const args = minimist(process.argv.slice(2));

gulp.task('deploy', async () => {
  const wwwPath = 'www/';
  const remotePath = '/web/html/heavymetal.dk/';
  const conn = ftp.create({
    host: 'heavymetal.dk',
    user: args.user,
    password: args.password,
    log: gutil.log
  });

  const files = await changes(wwwPath, args.commitrange);

  gulp.src(files, {base: wwwPath})
    .pipe(conn.dest(remotePath));
});

async function changes(basePath, commitRange) {
  const files = await gitFilesChangedSinceLastBuild(commitRange);
  console.log('Files changed since last build:\n', files);
  const lines = files.split('\n');
  const filesList = [];
  for(let i = 0;i < lines.length;i++){
    if (lines[i].length > 0 && lines[i].startsWith(basePath)) {
      filesList.push(lines[i]);
    }
  }
  return filesList;
}

async function gitFilesChangedSinceLastBuild(commitRange) {
  const diff = 'diff --name-only ' + commitRange;
  console.log('diff =', diff);
  return new Promise(resolve => {
    git.exec({args : diff}, (err, stdout) => {
      resolve(stdout);
    });
  });
}
