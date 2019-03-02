console.log("[debug.js] BEGIN.")
/********************* Begin **********************/
const Addons=require('./addons')
const addon = Addons.class;
/********************* Loaded *********************/
console.log(addon.Empty)
var marker = addon.marker();
console.log(`1. marker = ${marker}`)
let e = new addon.Empty();
marker = addon.marker();
console.log(`2. marker = ${marker}`)
e.delete();
marker = addon.marker();
console.log(`3. marker = ${marker}`)

e = undefined;

setTimeout(function () {
    console.log('end');
  }, 1000);
  
/********************* END **********************/
console.log("[debug.js] End.")