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

  const files = await changes(wwwPath);

  gulp.src(files, {base: wwwPath})
    .pipe(conn.dest(remotePath));
});

async function changes(basePath) {
  const files = await gitFilesChangedSinceLastBuild();
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

async function gitFilesChangedSinceLastBuild() {
  const diff = 'diff --name-only @ origin/master';
  return new Promise(resolve => {
    git.exec({args : diff}, (err, stdout) => {
      resolve(stdout);
    });
  });
}
