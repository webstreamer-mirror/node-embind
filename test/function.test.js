const addons = require('./addons')
console.log(addons)
var chai = require('chai'),
  assert = chai.assert

  describe('function', function() {
  const {
    Inc,
    intSum,
    doubleSum
  } = addons.function;

  describe('#Inc()', function() {
    it('function without param', function() {
      assert.strictEqual(Inc() ,Inc()-1);
    });
  });



  describe('#Sum', function() {
    it('function overload', function() {
        assert.strictEqual(intSum(1 ,2 , 3),6)
        assert.strictEqual(intSum(1 ,2 ),3)
        assert.approximately(doubleSum(1.1 ,2.2 , 3.3),6.6,0.01)
        assert.approximately(doubleSum(1.1 ,2.2 ),3.3, 0.01)

    });
  })

});