const path = require('path');
const webpack = require('webpack');
const CopyWebpackPlugin = require('copy-webpack-plugin');

module.exports = {
  entry: path.join('app', 'js', 'main.ts'),
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'app.js'
  },
  devtool: "source-map",
  resolve: {
    // Add '.ts' and '.tsx' as resolvable extensions.
    extensions: [".ts", ".js", ],
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
        options: {
          configFile: 'node_modules/tslint-microsoft-contrib/tslint.json',
          typeCheck: true,
        }
      },
      { 
        test: /\.tsx?$/, 
        use: 'awesome-typescript-loader?configFileName="tscondig.json"',
        exclude: /(node_modules)/,
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
  },  
  plugins: [
    new webpack.ProvidePlugin({ jQuery: "jquery" }),
    new CopyWebpackPlugin([{ from: 'app/index.html' }]),
  ],
};