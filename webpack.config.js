const path = require("path");
const MiniCssExtractPlugin = require("mini-css-extract-plugin");
const HtmlWebpackPlugin = require("html-webpack-plugin");

const isProduction = process.env.NODE_ENV === "production";

module.exports = {
  mode: isProduction ? "production" : "development",
  entry: path.resolve("src/Index.bs"),
  output: {
    path: path.resolve("build"),
    filename: isProduction
      ? "static/js/[name].[contenthash:8].js"
      : "static/js/bundle.js",
    chunkFilename: isProduction
      ? "static/js/[name].[contenthash:8].chunk.js"
      : "static/js/[name].chunk.js"
  },
  module: {
    rules: [
      {
        test: /\.css$/,
        use: [
          isProduction
            ? {
              loader: MiniCssExtractPlugin.loader
            }
            : "style-loader",
          {
            loader: "css-loader",
            options: {
              importLoaders: 1,
              sourceMap: isProduction
            }
          }
        ]
      },
      {
        test: /\.(png|jpg|gif|svg)$/,
        use: [
          {
            loader: require.resolve("url-loader"),
            options: {
              limit: 8192,
              fallback: require.resolve("file-loader")
            }
          }
        ]
      }
    ]
  },
  plugins: [
    new HtmlWebpackPlugin({
      inject: true,
      template: "public/index.html"
    }),
    isProduction && new MiniCssExtractPlugin({
      filename: "static/css/[name].[contenthash:8].css",
      chunkFilename: "static/css/[name].[contenthash:8].chunk.css"
    })
  ].filter(Boolean),
  devServer: {
    hot: true,
    compress: true,
    contentBase: "public",
    watchContentBase: true
  }
};
