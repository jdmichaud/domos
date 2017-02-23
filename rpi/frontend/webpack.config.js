const path = require('path');

module.exports = {
  entry: path.join('app', 'js', 'main.ts'),
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'app.js'
  },
  devtool: "source-map",
  resolve: {
    // Add '.ts' and '.tsx' as resolvable extensions.
    extensions: [".ts", ".js"],
    modules: [
      "node_modules",
      path.resolve(__dirname, ".")
    ],
  },
  module: {
    rules: [
      {
        enforce: 'pre',
        test: /\.tsx?$/,
        loader: 'tslint-loader',
        exclude: /(node_modules)/,
      },
      { 
        test: /\.tsx?$/, 
        use: 'awesome-typescript-loader?configFileName="tscondig.json"',
        exclude: /(node_modules)/,
      }
    ],
  },  
};