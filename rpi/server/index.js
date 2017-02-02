const constants = require('./constants');
const RestConstructor = require('./rest-driver');
const DbConstructor = require('./db-filesystem');
const DomosServer = require('./server');

const db = DbConstructor(constants.DATA_FOLDER);
const rest = RestConstructor(db);

const server = DomosServer(db, rest);
server.listen('127.0.0.1', 12000);
