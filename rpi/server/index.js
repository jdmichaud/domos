var express = require('express');
var bodyParser = require('body-parser');
var lodash = require('lodash');
var db = require('./db-memory');

const constants = Object.freeze({
  REST_URL_PREFIX: '/api',
});

const watcher = {};

function list(resource, query, res) {
  if (query.hasOwnProperty('watch')) {
    watcher[resource] = watcher[resource] || [];
    watcher[resource].push(res);
  } else {
    res.send(db.get(resource));
  }
}

function create(resource, data, query, res) {
  res.send(db.create(resource, data));
}

function retrieve(resource, id, query, res) {
  res.send(db.get(resource, parseInt(id, 10)));
}

function update(resource, id, data, query, res) {
  const result = db.update(resource, id, data);
  if (lodash.isEmpty(result)) {
    res.status(404).send('Not found');
  }
  res.send(result);
}

function del(resource, id, res) {
  res.send(db.del(resource, parseInt(id, 10)));
}

function publish(resource) {
  if (watcher[resource]) {
    watcher[resource].forEach((res) => {
      // Trigger a list response on the waiting clients
      list(resource, {}, res);
    });
    // Once watchers are called back, remove them from the waiting list
    watcher[resource].length = 0;
  }
}

var app = express();
app.disable('x-powered-by');
app.use(bodyParser.json());

app.get('/', (req, res) => res.send('Hello World!'));

app.get(`${constants.REST_URL_PREFIX}/[^/]+/`, (req, res) => {
  const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
  list(resource, req.query, res);
});

app.get(`${constants.REST_URL_PREFIX}/*/:id`, (req, res) => {
  const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
  retrieve(resource, req.params.id, req.query, res);
});

app.post(`${constants.REST_URL_PREFIX}/[^/]+/`, (req, res) => {
  const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
  create(resource, req.body, req.query, res);
  publish(resource);
});

app.post(`${constants.REST_URL_PREFIX}/*/:id`, (req, res) => {
  const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
  update(resource, req.params.id, req.body, req.query, res);
  publish(resource);
});

app.delete(`${constants.REST_URL_PREFIX}/*/:id`, (req, res) => {
  const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
  del(resource, req.params.id, res);
  publish(resource);
});

app.listen(3000, () => console.log('domos server listening on port 3000!'));
