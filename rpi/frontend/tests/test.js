// This is just so webpack convert test.ts to javascript
// require('./test.ts');

// load all specs in ./src
const context = require.context('../app/', true, /\.spec\.ts$/);
console.log('context:', context.keys);
context.keys().map((entry) => console.log(entry));
context.keys().map(context);

