var express = require('express')
var bodyParser = require('body-parser');

const REST_URL_PREFIX = '/api';

function list(resource, query) {
}

function create(resource, data, query) {
}

function retrieve(resource, id, query) {
}

function update(resource, id, data, query) {
}

function del(resource, id) {
}

var app = express()
app.disable('x-powered-by');
app.use(bodyParser.json())

app.get('/', function (req, res) {
  res.send('Hello World!')
});

app.get(REST_URL_PREFIX + '/[^/]+/', function (req, res) {
  const resource = req.slice(5);
  res.send(list(resource, req.query));
});

app.get(REST_URL_PREFIX + '/*/:id', function (req, res) {
  const resource = req.url.slice(5, req.url.indexOf('/'));
  res.send(retrieve(resource, req.params.id, req.query));
});

app.post(REST_URL_PREFIX + '/[^/]+/', function (req, res) {
  const resource = req.url.slice(5);
  res.send(create(resource, req.body, req.query));
});

app.post(REST_URL_PREFIX + '/*/:id', function (req, res) {
  const resource = req.url.slice(5, req.url.indexOf('/'));
  res.send(update(resource, req.params.id, req.body, req.query));
});

app.delete(REST_URL_PREFIX + '/*/:id', function (req, res) {
  const resource = req.url.slice(5, req.url.indexOf('/'));
  res.send(del(resource, res.params.id));
});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!')
})
