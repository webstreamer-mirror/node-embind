const addons = require('./addons')

var chai = require('chai'),
    assert = chai.assert

describe('value_object', function () {
    const {
        ValueObject,
    } = addons.value_object;

    it('#value_object', function () {

        var c = new ValueObject();
        c.v1 =1;
        c.v2 =2;
        c.v3 =3;
        c.v4 =4;
        assert.strictEqual(c.v1,1);
        assert.strictEqual(c.v2,2);
        assert.strictEqual(c.v3,3);
        assert.strictEqual(c.v4,4);

        c.delete();

    });


});