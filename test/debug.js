console.log("[debug.js] BEGIN.")
/********************* Begin **********************/
const Addons=require('./addons')
const addon = Addons.class;
/********************* Loaded *********************/
var C=addon.Function
console.log(C)
var c = new C()
c.Get();
//console.log(marker())
//console.log(c,"@@",c.set(0))  
/********************* END **********************/
console.log("[debug.js] End.")