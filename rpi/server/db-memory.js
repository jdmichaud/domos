var lodash = require('lodash');

function db() {
  const _db = {};
  return {
    get: function (resource, id) {
      _db[resource] = _db[resource] || [];
      if (id === undefined) {
        return _db[resource];
      }
      const result = _db[resource].filter(instance => instance.id === id);
      if (result.length) {
        return result[0];
      }
      return {};
    },
    create: function (resource, data) {
      _db[resource] = _db[resource] || [];
      let maxid = Math.max.apply({}, _db[resource].map(instance => instance.id));
      maxid = maxid > 0 ? maxid : 0;
      data.id = maxid + 1;
      _db[resource].push(data);
      return data;
    },
    update: function (resource, id, data) {
      _db[resource] = _db[resource] || [];
      let result = _db[resource].filter(instance => instance.id === id);
      if (result.length) {
        result = result[0];
        _db[resource] = _db[resource].filter(instance => instance.id !== id);
        lodash.merge(result, data);
        _db[resource].push(result);
        return result;
      }
      return {};
    },
    del: function (resource, id) {
      _db[resource] = _db[resource] || [];
      _db[resource] = _db[resource].filter(instance => instance.id !== id);
      return _db[resource];
    },
  };
}

module.exports = db();
