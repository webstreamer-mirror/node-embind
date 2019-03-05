const addons = require('./addons')

var chai = require('chai'),
    assert = chai.assert

describe('class.function', function () {
    const {
        Function,
    } = addons.class;


    it('#member function', function () {

        var c = new Function();
            
        assert.strictEqual(c.getV1(), 0);
        assert.strictEqual(c.getV2(), 0);

        c.setV1(123);
        c.setV2(456);

        assert.strictEqual(c.getV1(), 123);
        assert.strictEqual(c.getV2(), 456);

        c.set(78,90)
        assert.strictEqual(c.getV1(), 78);
        assert.strictEqual(c.getV2(), 90);

    });

    it('#external function', function () {

        var c = new Function();
            
        assert.strictEqual(c.GetV1(), 0);
        assert.strictEqual(c.GetV2(), 0);

        c.SetV1(123);
        c.SetV2(456);

        assert.strictEqual(c.getV1(), 123);
        assert.strictEqual(c.getV2(), 456);

        assert.strictEqual(c.GetV1(), 123);
        assert.strictEqual(c.GetV2(), 456);

        c.set(78,90)
        assert.strictEqual(c.getV1(), 78);
        assert.strictEqual(c.getV2(), 90);      

        assert.strictEqual(c.GetV1(), 78);
        assert.strictEqual(c.GetV2(), 90);      

    });

    it('#class_function', function () {

        var counter = Function.Counter();

        var c = new Function();
            
        assert.strictEqual(Function.Counter(), counter+1);

        Function.SetCounter(100);
        assert.strictEqual(Function.Counter(), 100);
    });

});