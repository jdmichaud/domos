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
  // devtool: "source-map",
  devtool: "eval",
  resolve: {
    // Add '.ts' and '.js' as resolvable extensions.
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
        exclude: /(tests|node_modules)/,
        options: {
          configFile: 'tslint.json',
          typeCheck: true,
        }
      },
      {
        test: /\.html$/,
        use: 'raw-loader',
      },
      {
        test: /\.tsx?$/,
        loader: 'ts-loader',
        exclude: /(tests|node_modules)/,
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