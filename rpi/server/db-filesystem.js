const fs = require('fs');
const path = require('path');
const lodash = require('lodash');

function mkdirSync(dirpath) {
  try {
    fs.mkdirSync(dirpath);
  } catch (e) {
    if (e.code !== 'EEXIST') throw e;
  }
}

function mkdirpSync(dirpath) {
  var parts = dirpath.split(path.sep);
  for (var i = 1; i <= parts.length; i += 1) {
    mkdirSync(path.join.apply(null, parts.slice(0, i)));
  }
}

function timestamp() {
  return new Date().toISOString();
}

function getLastEntryInDir(pathWithId) {
  return fs.readdirSync(pathWithId)
    .concat()
    .sort((lhs, rhs) =>
      Date.parse(lhs.slice(lhs.indexOf('_') + 1)) < Date.parse(rhs.slice(rhs.indexOf('_') + 1)))
    .pop();
}

function db(rootPath) {
  if (lodash.isUndefined(rootPath)) {
    rootPath = path.join([process.cwd(), 'db']);
  }
  return {
    get: function (resource, id) {
      if (lodash.isUndefined(id)) {
        const filepath = path.join([rootPath, resource]);
        const filenames = fs.readdirSync(filepath).map(pathWithId => getLastEntryInDir(pathWithId));
        return filenames
          .filter(filename => filename.startsWith(resource))
          .map(filename => JSON.parse(fs.readFileSync(path.join([filepath, filepath]))));
      }
      const filepath = path.join([rootPath, resource, String(id)]);
      try {
        const filename = getLastEntryInDir(filepath);
        const filenames = fs.readdirSync(path.join([filepath, filename]));
        return JSON.parse(fs.readFileSync(filepath));
      } catch (e) {
        if (e.code !== 'ENOENT') throw e;
        return {};
      }
    },
    create: function (resource, data) {
      const filepath = path.join([rootPath, resource]);
      const pathWithIds = fs.readdirSync(filepath).map(pathWithId => parseInt(pathWithId, 10));
      let maxid = Math.max.apply({}, pathWithIds);
      maxid = maxid > 0 ? maxid : 0;
      data.id = maxid + 1;
      mkdirpSync(path.join([filepath, maxid]));
      fs.writeFileSync(path.join([filepath, maxid, `${resource}_${timestamp()}`]), data);
    },
    update: function (resource, id, data) {
    },
    del: function (resource, id) {
    },
  };
}

module.exports = db();
