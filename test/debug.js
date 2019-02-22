console.log("[debug.js] BEGIN.")
/********************* Begin **********************/
const Addons=require('./addons')
const addon = Addons.string;
/********************* Loaded *********************/
console.log('EMBIND_STD_STRING_IS_UTF8 :', addon.EMBIND_STD_STRING_IS_UTF8);
addon.EMBIND_STD_STRING_IS_UTF8 = true;

var expected = '';
//ASCII
expected = 'aei';
//Latin-1 Supplement
expected += '\u00E1\u00E9\u00ED';
//Greek
expected += '\u03B1\u03B5\u03B9';
//Cyrillic
expected += '\u0416\u041B\u0424';
//CJK
expected += '\u5F9E\u7345\u5B50';
//Euro sign
expected += '\u20AC';

var x = addon.nop(expected);
console.log("expected:",expected)
console.log("nop     :", x)
console.log(x == expected)
//console.log(addon.chinese().toString("utf8"));




/********************* END **********************/
console.log("[debug.js] End.")