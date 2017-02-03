const lodash = require('lodash');

module.exports = {
  testCreate: function (db) {
    const item = db.create('resource', { field: 5, field2: { data: 'toto' } });
    const item2 = db.get('resource', item.id);
    expect(item).toEqual(item2);
    expect(item2.field).toBe(5);
    expect(item2.field2.data).toBe('toto');
  },
  testFilter: function (db) {
    const item = db.create('resource', { field: 5, field2: { data: 'toto' } });
    const item2 = db.create('resource', { field: 9, field2: { data: 'toto' } });
    const item3 = db.create('resource2', { field: 12, foo: 'bar' });
    const item4 = db.create('resource2', { field: 73, foo: 'baz' });
    const item5 = db.create('resource2', { field: 10, foo: 'boo' });
    const items = db.filter('resource2');
    expect(items.length).toBe(3);
    expect(lodash.difference(items, [item3, item4, item5]).length).toBe(0);
  },
  testFilterQuery: function (db) {},
  testUpdate: function (db) {
    const item = db.create('resource', { field: 5, field2: { data: 'toto' } });
    const item2 = db.update('resource', item.id, {
      field: 4,
      field2: {
        data: 'titi',
        foo: 'bar',
      },
    });
    expect(item).toEqual(item2);
  },
  testDel: function (db) {
    const item = db.create('resource', { field: 5, field2: { data: 'toto' } });
    const item2 = db.create('resource2', { field: 5, field2: { data: 'toto' } });
    const item3 = db.create('resource', { field: 5, field2: { data: 'toto' } });
    db.del('resource', item3.id);
    const resources = db.filter('resource');
    expect(resources.length).toBe(1);
    expect(resources[0]).toEqual(item);
    const resource2s = db.filter('resource2');
    expect(resource2s.length).toBe(1);
    expect(resource2s[0]).toEqual(item2);  
  },
};
