console.log("[debug.js] Begin.")
/********************* Begin **********************/
const addons=require('./addons')
const addon = addons.value_object;
/********************* Loaded *********************/
const {
    ValueObject
} = addon;

var c = new ValueObject();
c.v1 =1;
c.v2 =2;
c.v3 =3;
c.v4 =4;
console.log(c.v1);
console.log(c.v2);
console.log(c.v3);
console.log(c.v4);
//global.gc()
/********************* End *********************/
console.log("[debug.js] End.")