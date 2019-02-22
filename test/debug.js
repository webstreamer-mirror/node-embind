console.log("[debug.js] BEGIN.")
/********************* Begin **********************/
const Addons=require('./addons')
const addon = Addons.string;
/********************* Loaded *********************/
console.log('EMBIND_STD_STRING_IS_UTF8 :', addon.EMBIND_STD_STRING_IS_UTF8);
addon.EMBIND_STD_STRING_IS_UTF8 = false;

var n = addon.int_to_string('2018');




/********************* END **********************/
console.log("[debug.js] End.")