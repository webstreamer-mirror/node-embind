const addons = require('./addons')

var chai = require('chai'),
    assert = chai.assert

describe('value_array', function () {
    const {
        ValueArray,
        take_and_return_ValueArray,
        return_ValueArray
    } = addons.value_array;

    it('#value_array', function () {

        var c = new ValueArray();
        c[0] =0;
        c[1] =1;
        c[2] =2;
        c[3] =3;

        assert.strictEqual(c[0],0);
        assert.strictEqual(c[1],1);
        assert.strictEqual(c[2],2);
        assert.strictEqual(c[3],3);

        c.delete();

    });

    it('#JS Array', function () {

        var c = take_and_return_ValueArray([10,20,30,40]);
        assert.strictEqual(c[0],10);
        assert.strictEqual(c[1],20);
        assert.strictEqual(c[2],30);
        assert.strictEqual(c[3],40)

        var c1 = return_ValueArray();
        assert.strictEqual(c1[0],1);
        assert.strictEqual(c1[1],2);
        assert.strictEqual(c1[2],3);
        assert.strictEqual(c1[3],4)        

    });


});