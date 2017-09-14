module.exports = {
  entry: {
    main: './lib/js/src/main.js',
    worker: './lib/js/src/worker.js',
  },
  output: {
    path: __dirname + '/docs',
    filename: '[name].js',
  }
}