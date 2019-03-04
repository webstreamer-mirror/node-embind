console.log("[debug.js] Begin.")
/********************* Begin **********************/
const Addons=require('./addons')
const addon = Addons.class;
/********************* Loaded *********************/
const {
    Constructor,
    Constructor_create
}=addon

var c1 = new Constructor();
console.log(`c1.v1 : ${c1.v1}`)
var c2 = Constructor_create(100)
console.log(`c1.v2 : ${c2.v1}`)
/********************* End *********************/
console.log("[debug.js] End.")