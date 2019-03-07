console.log("[debug.js] Begin.")
/********************* Begin **********************/
const addons=require('./addons')
const addon = addons.value_array;
/********************* Loaded *********************/
const {
    ValueArray,
    take_and_return_ValueArray,
    return_ValueArray,
    xValueArray
} = addon;


//const assert = require('assert')
var c0 = new ValueArray();
//var c1 = take_and_return_ValueObject(c);
//c={v1: 1, v2: 2, v3: 3, v4: 4}
var c1 = take_and_return_ValueArray(c0);

var c =take_and_return_ValueArray([1,2,3,4]);
// c.v1 =1;
// c.v2 =2;
// c.v3 =3;
// c.v4 =4;
console.log(c)
console.log(c[0]);
console.log(c[1]);
console.log(c[2]);
console.log(c[3]);
console.log(c[4]);

console.log('----------------------')
console.log(c1[0]);
console.log(c1[1]);
console.log(c1[2]);
console.log(c1[3]);
console.log(c1[4]);

//global.gc()
/********************* End *********************/
console.log("[debug.js] End.")