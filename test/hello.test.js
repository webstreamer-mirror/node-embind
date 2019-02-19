const { hello } = require('./addons')
var chai = require('chai'),
  expect = chai.expect ,
  assert = chai.assert
  
describe('hello', function() {

  it('hello-world', function() {
    assert.strictEqual(hello.hello() , 'world');
  });
});