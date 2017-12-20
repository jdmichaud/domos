// https://developers.google.com/web/updates/2017/06/headless-karma-mocha-chai
const webpackConf = require('../webpack.config');

// We need a different tsconfig.json so we override the webpack config file.
webpackConf.module.rules.filter(rule =>
  rule.loader === 'ts-loader')[0].loader = 'ts-loader?configFile=tsconfig.tests.json';

// Check CHROME_BIN
if (process.env.CHROME_BIN === undefined) {
  console.error('error: CHROME_BIN environement not set. Set it to chrome binary path');
  console.error('example: export CHROME_BIN=$(which chromium-browser)');
  process.exit(1);
}

module.exports = function(config) {
  config.set({
    basePath: './',
    frameworks: ['jasmine'],
    files: [{
      pattern: 'test.js', watched: false
    }],
    preprocessors: {
      // add webpack as preprocessor
      'test.js': ['webpack']
    },
    webpack: webpackConf,
    reporters: ['progress' ],
    port: 9876,  // karma web server port
    colors: true,
    logLevel: config.LOG_DEBUG,
    browsers: ['ChromeHeadless'],
    autoWatch: false,
    singleRun: false, // Karma captures browsers, runs the tests and exits
    concurrency: Infinity,
  });
}
