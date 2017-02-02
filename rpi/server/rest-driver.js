
function RestDriver(database) {
  return {
    list: function list(resource, query, res) {
      if (query.hasOwnProperty('watch')) {
        watcher[resource] = watcher[resource] || [];
        watcher[resource].push(res);
      } else {
        res.send(db.get(resource));
      }
    },
    
    create: function create(resource, data, query, res) {
      res.send(db.create(resource, data));
    },
    
    retrieve: function retrieve(resource, id, query, res) {
      res.send(db.get(resource, parseInt(id, 10)));
    },
    
    update: function update(resource, id, data, query, res) {
      const result = db.update(resource, id, data);
      if (lodash.isEmpty(result)) {
        res.status(404).send('Not found');
      }
      res.send(result);
    },
    
    del: function del(resource, id, res) {
      res.send(db.del(resource, parseInt(id, 10)));
    },
  };
}

module.exports = RestDriver;
