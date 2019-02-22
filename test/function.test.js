const addons = require('./addons')

var chai = require('chai'),
  assert = chai.assert

  describe('function', function() {
  const {
    Inc,
    intSum,
    doubleSum,
    Set,
    Get
  } = addons.function;

    it('function without param', function() {
      assert.strictEqual(Inc() ,Inc()-1);
    });

    it('function overload', function() {
        assert.strictEqual(intSum(1 ,2 , 3),6)
        assert.strictEqual(intSum(1 ,2 ),3)
        assert.approximately(doubleSum(1.1 ,2.2 , 3.3),6.6,0.01)
        assert.approximately(doubleSum(1.1 ,2.2 ),3.3, 0.01)

    });

    it('function no return', function() {
        Set(2018);
        assert.strictEqual(2018,Get());
        Set(2019);
        assert.strictEqual(2019,Get());

    });

});