const express = require('express');
const bodyParser = require('body-parser');
const lodash = require('lodash');
const constants = require('./constants');

// Used to callback the client which requested a long polling
// on the affected resource.
function publish(resource) {
  if (watcher[resource]) {
    watcher[resource].forEach((res) => {
      // Trigger a list response on the waiting clients
      rest.list(resource, {}, res);
    });
    // Once watchers are called back, remove them from the waiting list
    watcher[resource].length = 0;
  }
}

function DomosServer(db, rest) {
  console.log('db:', db);
  console.log('rest:', rest);
  const watcher = {};

  // Create the Express application
  const app = express();
  app.disable('x-powered-by');
  app.use(bodyParser.json());

  // Define the application routes
  app.get(`${constants.REST_URL_PREFIX}/[^/]+/`, (req, res) => {
    const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
    rest.list(resource, req.query, res);
  });
  
  app.get(`${constants.REST_URL_PREFIX}/*/:id`, (req, res) => {
    const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
    rest.retrieve(resource, req.params.id, req.query, res);
  });
  
  app.post(`${constants.REST_URL_PREFIX}/[^/]+/`, (req, res) => {
    const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
    rest.create(resource, req.body, req.query, res);
    publish(resource);
  });
  
  app.post(`${constants.REST_URL_PREFIX}/*/:id`, (req, res) => {
    const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
    rest.update(resource, req.params.id, req.body, req.query, res);
    publish(resource);
  });
  
  app.delete(`${constants.REST_URL_PREFIX}/*/:id`, (req, res) => {
    const resource = req.url.match(/\/api\/([^/]+)\/.*/)[1];
    rest.del(resource, req.params.id, res);
    publish(resource);
  }); 
  
  return { 
    listen: function listen(host, port) {
      app.listen(port, host, () => 
      console.log(`domos server listening on port ${host}:${port}`));
    },
  };
}

module.exports = DomosServer;
