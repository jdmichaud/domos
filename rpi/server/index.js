var express = require('express')
var bodyParser = require('body-parser');
var lodash = require('lodash');
var db = require('memory-db');

const constants = Object.freeze({
  REST_URL_PREFIX = '/api',
});

const db = {};
const watcher = {};

function list(resource, query, res) {
  if (query.hasOwnProperty('watch')) {
    watcher[resource] = watcher[resource] || [];
    watcher[resource].push(res);
  } else {
    if (db[resource]) {
      res.send(db[resource]);
    } else {
      res.send([]);
    }
  }
}

function create(resource, data, query, res) {
  db[resource] = db[resource] || [];
  let maxid = Math.max.apply({}, db[resource].map(instance => instance.id));
  maxid = maxid > 0 ? maxid : 0;
  data.id = maxid + 1;
  db[resource].push(data);
  res.send(data);
}

function retrieve(resource, id, query, res) {
  db[resource] = db[resource] || [];
  id = parseInt(id, 10);
  const result = db[resource].filter(instance => instance.id === id);
  if (result.length) {
    res.send(result[0]);
  } else {
    res.send({});
  }
}

function update(resource, id, data, query, res) {
  db[resource] = db[resource] || [];
  const result = db[resource].filter(instance => instance.id === id);
  if (result.length) {
    instance = result[0];
    db[resource] = db[resource].filter(instance => instance.id !== id);
    lodash.merge(instance, data);
    db[resource].push(instance);
    res.send(instance);
  } else {
    res.status(404).send('Not found');
  } 
}

function del(resource, id, res) {
  db[resource] = db[resource] || [];
  id = parseInt(id, 10);
  db[resource] = db[resource].filter(instance => instance.id !== id);
  res.send(db[resource]);
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

var app = express()
app.disable('x-powered-by');
app.use(bodyParser.json())

app.get('/', function (req, res) {
  res.send('Hello World!')
});

app.get(constants.REST_URL_PREFIX + '/[^/]+/', function (req, res) {
  const resource = req.url.match(/\/api\/([^\/]+)\/.*/)[1];
  list(resource, req.query, res);
});

app.get(constants.REST_URL_PREFIX + '/*/:id', function (req, res) {
  const resource = req.url.match(/\/api\/([^\/]+)\/.*/)[1];
  retrieve(resource, req.params.id, req.query, res);
});

app.post(constants.REST_URL_PREFIX + '/[^/]+/', function (req, res) {
  const resource = req.url.match(/\/api\/([^\/]+)\/.*/)[1];
  create(resource, req.body, req.query, res);
  publish(resource);
});

app.post(constants.REST_URL_PREFIX + '/*/:id', function (req, res) {
  const resource = req.url.match(/\/api\/([^\/]+)\/.*/)[1];
  update(resource, req.params.id, req.body, req.query, res);
  publish(resource);
});

app.delete(constants.REST_URL_PREFIX + '/*/:id', function (req, res) {
  const resource = req.url.match(/\/api\/([^\/]+)\/.*/)[1];
  del(resource, req.params.id, res);
  publish(resource);
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!')
})
