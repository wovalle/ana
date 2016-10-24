'use strict';

import source from 'vinyl-source-stream';
import gulp from 'gulp';
import rimraf from 'rimraf';
import run from 'run-sequence';
import server from 'gulp-live-server';
import babel from 'gulp-babel';
import copy from 'gulp-copy';


let express;

const paths = {
    src: './src',
    dest: './build',
    bin: './bin',
    srvFile: './build/bin/www'
};

gulp.task('default', cb => {
    run('server', 'build','watch', cb);
});

gulp.task('build', cb => {
    run('clean', 'babel', 'bin', 'restart', cb);
});

//Clean the app destination, to prepare for new files
gulp.task('clean', cb => {
    rimraf(paths.dest, cb);
});

gulp.task('bin', cb => {
    return gulp.src(`${paths.bin}/**`)
        .pipe(copy(paths.dest));
});

//build when a file has changed
gulp.task('watch', cb => {
  gulp.watch([
    `${paths.src}/**/**`
  ], ['build']);
});

gulp.task('server', () => {
  express = server.new(paths.srvFile);
});

gulp.task('restart', () => {
  express.start.bind(express)();
});

gulp.task('babel', cb => {
  return gulp.src(`${paths.src}/**/*.js`)
  .pipe(babel())
  .pipe(gulp.dest(paths.dest));
});