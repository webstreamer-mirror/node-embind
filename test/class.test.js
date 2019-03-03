const addons = require('./addons')

var chai = require('chai'),
    assert = chai.assert

describe('class', function () {
    const {
        Empty,
        marker,
    } = addons.class;

    describe('#ctor/delete', function () {
        it('ctor()', function () {
            var count = marker();
            var c = new Empty();
            assert.strictEqual(marker(), count + 1);
            c.delete();
        });
        
        it('delete', function () {
            var count = marker();
            var c = new Empty();
            assert.strictEqual(marker(), count + 1);
            c.delete();
            assert.strictEqual(marker(), count);
        });
    });


});