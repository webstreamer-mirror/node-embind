const addons = require('./addons')
var chai = require('chai'),
  expect = chai.expect ,
  assert = chai.assert

//const { out } = require('./emutils')

global.out = function out(val){
    out.result = val;
}

describe('em_asm', function() {
    
    const em = addons.em_asm;
    it('wiht_comma_max', function () {
        var val = em.wiht_comma_max(100,200);
        assert.strictEqual(val, 200);

        val = em.wiht_comma_max(200,100);
        assert.strictEqual(val, 200);
    });

    it('unused_arguments', function () {
        var val = em.unused_arguments(1,2,3);
        assert.strictEqual(val, 4);

    });

    describe('EM_ASM',function(){
        const EM_ASM = em.EM_ASM;

        it('case_1', function(){
            EM_ASM.case_1();
            assert.isUndefined($0)
            var $0=5;
            assert.strictEqual(global.out.result, "  takes ints: " + $0);
        });

        it('case_2', function(){
            
            
            EM_ASM.case_2();
            assert.isUndefined($0)
            var $0=5.0675;
            assert.strictEqual(global.out.result, "  takes doubles: " + $0);
        });


        it('case_4', function(){
            EM_ASM.case_4();
            assert.isUndefined($0)
            assert.isUndefined($1)
            var $0=5, $1=7 ,res=6;
            assert.strictEqual(global.out.result, "  takes multiple ints: " + $0 + ", " + $1);
        });


        it('case_6', function(){
            EM_ASM.case_6();
            var res=5.5;
            assert.strictEqual(global.out.result, "  ignores unused args");
        });

        it('case_7', function(){
            EM_ASM.case_7();
            assert.isUndefined($0)
            assert.isUndefined($1)
            var $0=5, $1=7, res=6;
            assert.strictEqual(global.out.result, "  skips unused args: " + $1);
        });

        it('case_8', function(){
            EM_ASM.case_8();
            assert.isUndefined($0)
            assert.isUndefined($1)
            assert.isUndefined($2)
            var $0=5.5, $1=7.0, $2=14.375;
            var res =$0 + $2;
            assert.strictEqual(global.out.result, "  " + $0 + " + " + $2);
        });
    });



    
    describe('EM_ASM_INT',function(){
        const EM_ASM_INT = em.EM_ASM_INT;

        it('case_1', function(){
            var val = EM_ASM_INT.case_1();
            assert.isUndefined($0)
            var $0=5;
            assert.strictEqual(global.out.result, "  takes ints: " + $0);
			//assert.isUndefined(val);
        });

        it('case_2', function(){            
            var val = EM_ASM_INT.case_2();
            assert.isUndefined($0)
            var $0=5.0675;
            assert.strictEqual(global.out.result, "  takes doubles: " + $0);
			//assert.isUndefined(val);
        });


        it('case_4', function(){
            var val = EM_ASM_INT.case_4();
            assert.isUndefined($0)
            assert.isUndefined($1)
            var $0=5, $1=7 ,res=6;
            assert.strictEqual(global.out.result, "  takes multiple ints: " + $0 + ", " + $1);
			assert.strictEqual(val,res);
        });


        it('case_6', function(){
            var val = EM_ASM_INT.case_6();
            var res=5.5;
            assert.strictEqual(global.out.result, "  ignores unused args");
			assert.strictEqual(val,Math.floor(res));
        });

        it('case_7', function(){
            var val = EM_ASM_INT.case_7();
            assert.isUndefined($0)
            assert.isUndefined($1)
            var $0=5, $1=7, res=6;
            assert.strictEqual(global.out.result, "  skips unused args: " + $1);
			assert.strictEqual(val,Math.floor(res));
        });

        it('case_8', function(){
            var val = EM_ASM_INT.case_8();
            assert.isUndefined($0)
            assert.isUndefined($1)
            assert.isUndefined($2)
            var $0=5.5, $1=7.0, $2=14.375;
            var res =$0 + $2;
            assert.strictEqual(global.out.result, "  " + $0 + " + " + $2);
			assert.strictEqual(val,Math.floor(res));
        });
    });


    
    describe('EM_ASM_DOUBLE',function(){
        const EM_ASM_DOUBLE = em.EM_ASM_DOUBLE;

        it('case_1', function(){
            var val = EM_ASM_DOUBLE.case_1();
            assert.isUndefined($0)
            var $0=5;
            assert.strictEqual(global.out.result, "  takes ints: " + $0);
			//assert.isUndefined(val);
        });

        it('case_2', function(){            
            var val = EM_ASM_DOUBLE.case_2();
            assert.isUndefined($0)
            var $0=5.0675;
            assert.strictEqual(global.out.result, "  takes doubles: " + $0);
			//assert.isUndefined(val);
        });


        it('case_4', function(){
            var val = EM_ASM_DOUBLE.case_4();
            assert.isUndefined($0)
            assert.isUndefined($1)
            var $0=5, $1=7 ,res=6;
            assert.strictEqual(global.out.result, "  takes multiple ints: " + $0 + ", " + $1);
			assert.strictEqual(val,res);
        });


        it('case_6', function(){
            var val = EM_ASM_DOUBLE.case_6();
            var res=5.5;
            assert.strictEqual(global.out.result, "  ignores unused args");
			assert.strictEqual(val,res);
        });

        it('case_7', function(){
            var val = EM_ASM_DOUBLE.case_7();
            assert.isUndefined($0)
            assert.isUndefined($1)
            var $0=5, $1=7, res=6;
            assert.strictEqual(global.out.result, "  skips unused args: " + $1);
			assert.strictEqual(val,res);
        });

        it('case_8', function(){
            var val = EM_ASM_DOUBLE.case_8();
            assert.isUndefined($0)
            assert.isUndefined($1)
            assert.isUndefined($2)
            var $0=5.5, $1=7.0, $2=14.375;
            var res =$0 + $2;
            assert.strictEqual(global.out.result, "  " + $0 + " + " + $2);
			assert.strictEqual(val,res);
        });
    });


});