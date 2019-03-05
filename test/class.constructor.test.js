const addons = require('./addons')

var chai = require('chai'),
    assert = chai.assert

describe('class.constructor', function () {
    const {
        Constructor,
        Constructor_create,
        Constructor_id,
        Constructor_mark,
        Constructor_set_id,
        Constructor_set_mark,
        Constructor_val_return_val,
        Constructor_ref_return_ref,
        Constructor_pointer_return_pointer,
        Constructor_const_ref_return_const_ref,
        Constructor_const_pointer_return_const_pointer
    } = addons.class;

    it('#new Construct', function () {
            var c0 = new Constructor();
            assert.strictEqual(Constructor_id(c0), 0);
            assert.strictEqual(Constructor_mark(c0), 0);

            Constructor_set_id(c0,100), 
            Constructor_set_mark(c0,100)

            assert.strictEqual(Constructor_id(c0), 100);
            assert.strictEqual(Constructor_mark(c0), 100);
            c0.delete();
    });

    it('#Constructor_create', function () {
        var c = Constructor_create(1,1);
        assert.strictEqual(Constructor_id(c), 1);
        assert.strictEqual(Constructor_mark(c), 1);

        Constructor_set_id(c,100), 
        Constructor_set_mark(c,100)

        assert.strictEqual(Constructor_id(c), 100);
        assert.strictEqual(Constructor_mark(c), 100);
        c.delete();
    });

    it('#Constructor_val_return_val', function () {
        var c = Constructor_create(1,1);
        var c1 = Constructor_val_return_val(c);
        assert.strictEqual(Constructor_id(c1), 1);
        assert.strictEqual(Constructor_mark(c1), 1);

        Constructor_set_id(c1,100), 
        Constructor_set_mark(c1,100)

        assert.strictEqual(Constructor_id(c1), 100);
        assert.strictEqual(Constructor_mark(c1), 100);

        assert.strictEqual(Constructor_id(c), 1);
        assert.strictEqual(Constructor_mark(c), 1);

        c.delete();
        c1.delete();
    });

    it('#Constructor_ref_return_ref', function () {
        var c = Constructor_create(1,1);
        var c1 = Constructor_ref_return_ref(c);
        assert.strictEqual(Constructor_id(c1), 1);
        assert.strictEqual(Constructor_mark(c1), 1);

        Constructor_set_id(c1,100), 
        Constructor_set_mark(c1,100)

        assert.strictEqual(Constructor_id(c1), 100);
        assert.strictEqual(Constructor_mark(c1), 100);

        assert.strictEqual(Constructor_id(c), 100);
        assert.strictEqual(Constructor_mark(c), 100);

        //How to delete ?

        //c.delete();
        //c1.delete();
    });

    it('#Constructor_pointer_return_pointer', function () {
        var c = Constructor_create(1,1);
        var c1 = Constructor_pointer_return_pointer(c);
        assert.strictEqual(Constructor_id(c1), 1);
        assert.strictEqual(Constructor_mark(c1), 1);

        Constructor_set_id(c1,100), 
        Constructor_set_mark(c1,100)

        assert.strictEqual(Constructor_id(c1), 100);
        assert.strictEqual(Constructor_mark(c1), 100);

        assert.strictEqual(Constructor_id(c), 100);
        assert.strictEqual(Constructor_mark(c), 100);

        //How to delete ?

        //c.delete();
        //c1.delete();
    });

    it('#Constructor_const_ref_return_const_ref', function () {
        var c = Constructor_create(1,1);
        var c1 = Constructor_const_ref_return_const_ref(c);
        assert.strictEqual(Constructor_id(c1), 1);
        assert.strictEqual(Constructor_mark(c1), 1);

        Constructor_set_id(c1,100), 
        Constructor_set_mark(c1,100)

        assert.strictEqual(Constructor_id(c1), 100);
        assert.strictEqual(Constructor_mark(c1), 100);

        assert.strictEqual(Constructor_id(c), 100);
        assert.strictEqual(Constructor_mark(c), 100);

        //How to delete ?

        //c.delete();
        //c1.delete();
    });

    
    it('#Constructor_const_pointer_return_const_pointer', function () {
        var c = Constructor_create(1,1);
        var c1 = Constructor_const_pointer_return_const_pointer(c);
        assert.strictEqual(Constructor_id(c1), 1);
        assert.strictEqual(Constructor_mark(c1), 1);

        Constructor_set_id(c1,100), 
        Constructor_set_mark(c1,100)

        assert.strictEqual(Constructor_id(c1), 100);
        assert.strictEqual(Constructor_mark(c1), 100);

        assert.strictEqual(Constructor_id(c), 100);
        assert.strictEqual(Constructor_mark(c), 100);

        //How to delete ?

        //c.delete();
        //c1.delete();
    });
});