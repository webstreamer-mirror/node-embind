console.log("[debug.js] Begin.")
/********************* Begin **********************/
const addons=require('./addons')
const addon = addons.em_asm;
/********************* Loaded *********************/
const {
    wiht_comma_max,
    EM_ASM,
    EM_ASM_INT,
    EM_ASM_DOUBLE
} = addon;
global.out = function out(val){
    out.result = val;
}
var  n = EM_ASM_DOUBLE.case_6();

//var n = EM_ASM.case_1();
console.log(n)

//global.gc()
/********************* End *********************/
console.log("[debug.js] End.")