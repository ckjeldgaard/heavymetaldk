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

  const gitChanges = await gitFilesChangedSinceLastBuild(args.commitrange);
  console.log('Files changed since last build:\n', gitChanges);

  const modifiedFiles = await changes(gitChanges, wwwPath, ['M', 'A', 'R', 'C']);
  const deletedFiles = await changes(gitChanges, wwwPath, ['D']);

  if (modifiedFiles.length > 0) {
    gulp.src(modifiedFiles, {base: wwwPath})
      .pipe(conn.dest(remotePath));
  }

  if (deletedFiles.length > 0) {
    for(let i = 0; i < deletedFiles.length; i++) {
      const delFilename = remotePath + deletedFiles[i].substring(wwwPath.length, deletedFiles[i].length);
      try {
        gulp.src(deletedFiles, {base: wwwPath, allowEmpty: true})
          .pipe(conn.delete(delFilename, () => {
              console.log('Deleting ' + delFilename);
            }
          ));
      } catch (error) {
        console.error('An error occurred when deleting ' + delFilename, error);
      }
    }
  }
});

async function changes(files, basePath, statuses) {
  const lines = files.split('\n');
  const filesList = [];
  for(let i = 0;i < lines.length;i++){
    if (lines[i].length > 0) {
      const lineParts = lines[i].split('\t');
      if (statuses.includes(lineParts[0]) && lineParts[1].startsWith(basePath)) {
        filesList.push(lineParts[1]);
      }
    }
  }
  return filesList;
}

async function gitFilesChangedSinceLastBuild(commitRange) {
  const diff = 'diff --name-status ' + commitRange;
  console.log('diff =', diff);
  return new Promise(resolve => {
    if (commitRange.length > 2) {
      git.exec({args: diff}, (err, stdout) => {
        resolve(stdout);
      });
    } else {
      resolve('');
    }
  });
}
