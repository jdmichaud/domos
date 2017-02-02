const constants = require('./constants');
const RestConstructor = require('./rest-driver');
const DbConstructor = require('./db-filesystem');
const DomosServer = require('./server');

db = DbConstructor(constants.DATA_FOLDER);
rest = RestConstructor(db);
console.log('rest2:', rest);

console.log('DomosServer:', DomosServer);
server = DomosServer(db. rest);
server.listen('127.0.0.1', 12000);
