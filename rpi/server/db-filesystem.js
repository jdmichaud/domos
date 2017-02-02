const fs = require('fs');
const path = require('path');
const lodash = require('lodash');

const constants = Object.freeze({
  DELETE_MARK: '_deleted_',
});

const idRegex = new RegExp('^[0-9]*$');

function mkdirSync(dirpath) {
  try {
    console.log('mkdir:', dirpath);
    fs.mkdirSync(dirpath);
  } catch (e) {
    if (e.code !== 'EEXIST') throw e;
  }
}

function mkdirpSync(dirpath) {
  console.log('dirpath:', dirpath);
  const parts = dirpath.split(path.sep);
  const format = path.parse(dirpath);
  for (var i = 1; i <= parts.length; i += 1) {
    mkdirSync(path.join.apply(null, [format.root].concat(parts.slice(0, i))));
  }
}

function timestamp() {
  return new Date().toISOString();
}

function getLastEntryInDir(pathWithId) {
  console.log('getLastEntryInDir:', pathWithId);
  return fs.readdirSync(pathWithId)
    .concat()
    .sort((lhs, rhs) =>
      Date.parse(lhs.slice(lhs.indexOf('_') + 1)) > Date.parse(rhs.slice(rhs.indexOf('_') + 1)))
    .pop();
}

function getItemFromPath(dirpath) {
  try {
    const filename = getLastEntryInDir(dirpath);
    return JSON.parse(fs.readFileSync(path.join(dirpath, filename)));
  } catch (e) {
    if (e.code !== 'ENOENT') throw e;
  }
  return {};
}

function db(rootPath) {
  if (lodash.isUndefined(rootPath)) {
    rootPath = path.join(process.cwd(), 'db');
  }
  return {
    get: function (resource, id) {
      if (lodash.isUndefined(id)) {
        const filepath = path.join(rootPath, resource);
        console.log('filepath:', filepath);
        const filenames = fs.readdirSync(filepath)
          // Filter out the deleted items
          .filter(id => idRegex.test(id))
          // Get the last updated file in the folder
          .map(id => path.join(id, getLastEntryInDir(path.join(filepath, id))));
        console.log('filenames:', filenames);
        return filenames
          .map(filename => JSON.parse(fs.readFileSync(path.join(filepath, filename))));
      }
      const filepath = path.join(rootPath, resource, String(id));
      return getItemFromPath(filepath);
    },
    create: function (resource, data) {
      const filepath = path.join(rootPath, resource);
      mkdirpSync(filepath);
      const pathWithIds = fs.readdirSync(filepath).map(pathWithId => parseInt(pathWithId, 10));
      let maxid = Math.max.apply({}, pathWithIds);
      maxid = maxid > 0 ? maxid : 0;
      data.id = maxid + 1;
      mkdirpSync(path.join(filepath, String(maxid)));
      fs.writeFileSync(path.join(filepath, String(maxid), `${resource}_${timestamp()}`), data);
      return data;
    },
    update: function (resource, id, data) {
    },
    del: function (resource, id) {
      const filepath = path.join(rootPath, resource, String(id));
      const item = getItemFromPath(filepath);
      if (lodash.isEmpty(resource)) {
        try {
          fs.renameSync(filepath, `filepath${constants.DELETE_MARK}${timestamp}`);
        } catch (e) {}
      }
      return(item);
    },
  };
}

module.exports = db;
