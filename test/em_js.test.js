const addons = require('./addons')
var chai = require('chai'),
  expect = chai.expect ,
  assert = chai.assert

global.out = function out(val){
    out.result = val;
}
describe('em_js', function() {
    const { EMJS} = addons.em_js;

    it('noarg', function () {
        var val = EMJS.noarg();
        assert.isUndefined(val);
        assert.strictEqual(global.out.result, "no args works");
    });

    it('noarg_int', function () {
        var val = EMJS.noarg_int();
        assert.strictEqual(val, 12);
        assert.strictEqual(global.out.result, "no args returning int");
    });

    it('noarg_double', function () {
         var val = EMJS.noarg_double();
         assert.strictEqual(val, 12.25);
         assert.strictEqual(global.out.result, "no args returning double");
     });

    it('intarg', function () {
        var x = 123;
        var val = EMJS.intarg(123);
        assert.isUndefined(val);
        assert.strictEqual(global.out.result, "  takes ints: " + x);
    });

    it('doublearg', function () {
        var d = 654.321;
        var val = EMJS.doublearg(d);
        assert.isUndefined(val);
        assert.strictEqual(global.out.result, "  takes doubles: " + d);
    });
    
    it('multi_intarg', function() {
        var x = 123000 ,y = 456;
        var val = EMJS.multi_intarg(x,y);
        assert.strictEqual(val, 6);
        assert.strictEqual(global.out.result, "  takes multiple ints: " + x + ", " + y);
    });

    it('unused_args', function() {
        var val = EMJS.unused_args(1000);
        assert.strictEqual(val, 5);
        assert.strictEqual(global.out.result, "  ignores unused args");        
    });

    it('skip_args', function() {
        var x = 123000 ,y = 456;
        var val = EMJS.skip_args(x,y);
        assert.strictEqual(val, 6);
        assert.strictEqual(global.out.result, "  skips unused args: " + y);        
    });

    it('add_outer', function() {
        var x = 120000.12 ,y = 3400.0034,z=12.000012;
        var val = EMJS.add_outer(x,y,z);
        assert.strictEqual(val, x+z);
        assert.strictEqual(global.out.result, "  " + x + " + " + z);        
    });

    it('user_separator', function() {
        var val = EMJS.user_separator();
        assert.strictEqual(val, 15);
        assert.strictEqual(global.out.result,"  can use <::> separator in user code");      
    });



});