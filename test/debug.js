console.log("[debug.js] BEGIN.")
/********************* Begin **********************/
const Addons=require('./addons')
const addon = Addons.class;
/********************* Loaded *********************/
var C=addon.GlobalFunction_Tester
var marker = addon.marker
marker(100);
console.log(C)
console.log(marker())
var c = new C()
console.log(marker())
console.log(c,"@@",c.member)  
/********************* END **********************/
console.log("[debug.js] End.")