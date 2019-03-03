const addons = require('./addons')

var chai = require('chai'),
    assert = chai.assert

describe('class', function () {
    const {
        Empty,
        marker,
        ConstMember,
        Member,
        MemberFunction_GetterOnly,
        GlobalFunction_GetterTester,
        MemberFunction,
        GlobalFunction,
        Function
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

    describe('#property', function () {
        it('const member@getter', function () {
            var count = marker();
            var c = new ConstMember();            
            assert.strictEqual(c.member, count);
            c.member = count + 10000;
            assert.strictEqual(c.member, count);
            c.delete();
        });

        it('non-const member@getter', function () {
            
            var c = new Member();
            c.member = 123;
            assert.strictEqual(c.member, 123);
            c.member = 456;
            assert.strictEqual(c.member, 456);
            c.delete();
        });

        it('const member function@getter', function () {
            
            var c = new MemberFunction_GetterOnly();
            marker(123);
            
            assert.strictEqual(c.member, 123);
            c.member = 321;
            assert.strictEqual(c.member, 123);

            marker(456);
            assert.strictEqual(c.member, 456);
            c.member = 654;
            assert.strictEqual(c.member, 456);

            c.delete();
        });

        it('global function@getter', function () {
            
            var c = new GlobalFunction_GetterTester();
            
            c.member_ = 123;
            assert.strictEqual(c.member_, 123);
            assert.strictEqual(c.member, 123);
            c.member = 321;
            assert.strictEqual(c.member_, 123);
            assert.strictEqual(c.member, 123);
            c.delete();
        });

        it('member function@getter/setter', function () {
            
            var c = new MemberFunction();
            c.member = 123;
            
            assert.strictEqual(c.member, 123);
            c.member = 321;
            assert.strictEqual(c.member, 321);

            c.delete();
        });        

        it('global function@getter/setter', function () {
            
            var c = new GlobalFunction();
            c.member = 123;
            assert.strictEqual(c.member_, 123);
            assert.strictEqual(c.member, 123);
            c.member = 321;
            assert.strictEqual(c.member_, 321);
            assert.strictEqual(c.member, 321);

            c.delete();
        });        
    });

    describe('.function', function () {
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