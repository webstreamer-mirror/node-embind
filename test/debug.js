console.log("[debug.js] BEGIN.")
const Addons=require('./addons')

const addon=Addons.string;
console.log('EMBIND_STD_STRING_IS_UTF8 :', addon.EMBIND_STD_STRING_IS_UTF8);
addon.EMBIND_STD_STRING_IS_UTF8=true;

console.log(addon.chinese().toString("utf8"));
console.log("[debug.js] End.")