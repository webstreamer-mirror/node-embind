const addons = require('./addons')

var chai = require('chai'),
    assert = chai.assert

describe('class.Deleter', function () {
    const {
        Deleter,
        Deleter_id,
        Deleter_mark,
        Deleter_set_mark,
        Deleter_val_return_val,
        Deleter_ref_return_ref,
        Deleter_pointer_return_pointer,
        Deleter_const_ref_return_const_ref,
        Deleter_const_pointer_return_const_pointer,
        get_ctor_count,
        get_dector_count,
    } = addons.class;

    it('#new', function () {
            var c0 = new Deleter();
            var id0=Deleter_id(c0);
            
            assert.strictEqual(get_ctor_count(id0), 1);
            assert.strictEqual(get_dector_count(id0), 0);

            c0.delete();
            assert.strictEqual(get_ctor_count(id0), 1);
            assert.strictEqual(get_dector_count(id0), 1);
    });



    it('#Deleter_val_return_val', function () {
        var c0 = new Deleter();
        var id0=Deleter_id(c0);
        Deleter_set_mark(c0,100);
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 0);

        var c1 = Deleter_val_return_val(c0);
        assert.strictEqual(get_ctor_count(id0), 1);

        var id1=Deleter_id(c1);
        assert.notEqual(id0, id1); // by value is new object 

        c0.delete();        

        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 1);
        assert.strictEqual(get_ctor_count(id1), 1);
        assert.strictEqual(get_dector_count(id1), 0);
        assert.strictEqual(Deleter_mark(c1), 100);
        c1.delete();
        assert.strictEqual(get_ctor_count(id1), 1);
        assert.strictEqual(get_dector_count(id1), 1);
    });

    
    it('#Deleter_ref_return_ref', function () {
        var c0 = new Deleter();
        var id0=Deleter_id(c0);
        Deleter_set_mark(c0,100);
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 0);

        var c1 = Deleter_ref_return_ref(c0);
        assert.strictEqual(get_ctor_count(id0), 1);

        var id1=Deleter_id(c1);
        assert.strictEqual(id0, id1); // by ref is same object 

        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0),0 );

        assert.strictEqual(Deleter_mark(c1), 100);
        Deleter_set_mark(c1,12345);
        assert.strictEqual(Deleter_mark(c1), 12345);
        assert.strictEqual(Deleter_mark(c0), 12345);
        c0.delete();
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 1);
    });

    it('#Deleter_ref_return_ref', function () {
        var c0 = new Deleter();
        var id0=Deleter_id(c0);
        Deleter_set_mark(c0,100);
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 0);

        var c1 = Deleter_ref_return_ref(c0);
        assert.strictEqual(get_ctor_count(id0), 1);

        var id1=Deleter_id(c1);
        assert.strictEqual(id0, id1); // by ref is same object 

        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0),0 );

        assert.strictEqual(Deleter_mark(c1), 100);
        Deleter_set_mark(c1,12345);
        assert.strictEqual(Deleter_mark(c1), 12345);
        assert.strictEqual(Deleter_mark(c0), 12345);
        c0.delete();
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 1);
    });

    it('#Deleter_pointer_return_pointer', function () {
        var c0 = new Deleter();
        var id0=Deleter_id(c0);
        Deleter_set_mark(c0,100);
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 0);

        var c1 = Deleter_pointer_return_pointer(c0);
        assert.strictEqual(get_ctor_count(id0), 1);

        var id1=Deleter_id(c1);
        assert.strictEqual(id0, id1); // by ref is same object 

        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0),0 );

        assert.strictEqual(Deleter_mark(c1), 100);
        Deleter_set_mark(c1,12345);
        assert.strictEqual(Deleter_mark(c1), 12345);
        assert.strictEqual(Deleter_mark(c0), 12345);
        c0.delete();
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 1);
    });

    it('#Deleter_const_ref_return_const_ref', function () {
        var c0 = new Deleter();
        var id0=Deleter_id(c0);
        Deleter_set_mark(c0,100);
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 0);

        var c1 = Deleter_const_ref_return_const_ref(c0);
        assert.strictEqual(get_ctor_count(id0), 1);

        var id1=Deleter_id(c1);
        assert.strictEqual(id0, id1); // by ref is same object 

        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0),0 );

        assert.strictEqual(Deleter_mark(c1), 100);
        Deleter_set_mark(c1,12345);
        assert.strictEqual(Deleter_mark(c1), 12345);
        assert.strictEqual(Deleter_mark(c0), 12345);
        c0.delete();
        assert.strictEqual(get_ctor_count(id0), 1);
        assert.strictEqual(get_dector_count(id0), 1);
    });

});