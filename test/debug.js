console.log("[debug.js] Begin.")
/********************* Begin **********************/
const Addons=require('./addons')
const addon = Addons.class;
/********************* Loaded *********************/
const {
    Constructor,
    Constructor_create,
    Constructor_id,
    Constructor_mark,
    Constructor_set_id,
    Constructor_set_mark,

}=addon

var c0 = new Constructor()
var c1 = Constructor_create(1,1);

console.log(`c1.id : ${Constructor_id(c1)}`)
console.log(`c1.mark : ${Constructor_mark(c1)}`)
console.log(`c0.id : ${Constructor_id(c0)}`)
console.log(`c0.mark : ${Constructor_mark(c0)}`)

/********************* End *********************/
console.log("[debug.js] End.")