const path = require('path');
const webpack = require('webpack');
const CopyWebpackPlugin = require('copy-webpack-plugin');

module.exports = {
  context: path.join(__dirname, 'app'),
  // WARNING !!
  // 1. If you don't specify './' webpack is lost.
  // 2. You can't specify './' if you use path.join
  // 3. Without path.join, it will fail on Windows.
  // https://github.com/webpack/webpack/issues/223
  entry: './main',
  output: {
    path: path.join(__dirname, 'dist'),
    filename: 'app.js'
  },
  devtool: "source-map",
  resolve: {
    // Add '.ts' and '.tsx' as resolvable extensions.
    extensions: [".ts", ".js"],
    modules: [
      "node_modules",
      path.join(__dirname, 'app'),
    ],
  },
  module: {
    rules: [
      {
        enforce: 'pre',
        test: /\.tsx?$/,
        loader: 'tslint-loader',
        exclude: /(node_modules)/,
        options: {
          configFile: 'tslint.json',
          typeCheck: true,
        }
      },
      // Faster alternative to ts-loader
      { 
        test: /\.tsx?$/, 
        loaders: [{
          loader: 'awesome-typescript-loader',
          options: {
            configFileName: 'tsconfig.json',
          },
        // angular2-template-loader will replace templateUrl by template: require
        // to embed the HTML into app.js
        // For this, @types/requirejs is necessary
        },'angular2-template-loader'],
        exclude: /(node_modules)/,
      },
      {
        test: /\.html$/,
        loader: 'raw-loader',
      },
      {
        test: /\.css$/,
        use: [
          { loader: "style-loader" },
          { loader: "css-loader" },
        ],
      },
      {
        test: /\.(png|jpg|jpeg|gif|svg)([\?]?.*)$/,
        use: 'file-loader?name=resources/img/[name].[ext]?[hash]',
      },
      {
        test: /\.(woff|woff2|ttf|eot)([\?]?.*)$/,
        use: 'file-loader?name=resources/fonts/[name].[ext]?[hash]',
      },
    ],
    // https://github.com/angular/angular/issues/11580
    exprContextCritical: false,
  },  
  plugins: [
    new webpack.ProvidePlugin({ jQuery: 'jquery' }),
    new CopyWebpackPlugin([{ from: 'index.html' }]),
  ],
};