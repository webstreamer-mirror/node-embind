const addons = require('./addons')

var chai = require('chai'),
    assert = chai.assert

describe('class.property', function () {
    const {
        Property,
    } = addons.class;

    it('#const member', function () {

        var c = new Property(0,123);
            
        assert.strictEqual(c.id, 123);

        c.id = 100;
        assert.strictEqual(c.id, 123);

    });

    it('#member', function () {

        var c = new Property(123);
            
        assert.strictEqual(c.mark, 123);

        c.mark = 456;
        assert.strictEqual(c.mark, 456);

    });

    it('#member function', function () {

        var c = new Property(123);
            
        assert.strictEqual(c.Mark, 123);

        c.Mark = 456;
        assert.strictEqual(c.mark, 456);
        assert.strictEqual(c.Mark, 456);

    });

    it('#external function', function () {

        var c = new Property(123);
            
        assert.strictEqual(c.fMark, 123);

        c.fMark = 456;
        assert.strictEqual(c.mark, 456);
        assert.strictEqual(c.Mark, 456);
        assert.strictEqual(c.fMark, 456);

    });

    it('#class_property function', function () {

        
            
        assert.strictEqual(Property.COUNTER_BASE, 1000);
        Property.COUNTER_BASE=12345;
        assert.strictEqual(Property.COUNTER_BASE, 1000);

        Property.Counter=123;
        assert.strictEqual(Property.Counter, 123);

    });

});