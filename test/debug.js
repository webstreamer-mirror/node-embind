console.log("[debug.js] Begin.")
/********************* Begin **********************/
const addons=require('./addons')
const addon = addons.class;
/********************* Loaded *********************/
const {
    Property,
    Property_IDCounter
} = addon;

var c = new Property(0,123);

/********************* End *********************/
console.log("[debug.js] End.")