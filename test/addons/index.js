
const fs = require('fs');
const log = console.log;


 
var CONFIG_ = 'Release'
if(process.env.NODE_ENV){
    var c = process.env.NODE_ENV.replace(/(^\s*)|(\s*$)/g, "").toUpperCase()
    if( c == 'DEBUG'){
        CONFIG_ = 'Debug'
    }
}


const MODULE_LIST =[
    'hello',
    'class_','fn','value_object','em_asm','em_js','val','vector','map_',
    'enum_', 'constants', 'unique_ptr'
    ];
var exports={
    IS_DEBUG: CONFIG_=='Debug' ? true : false,
    MODULE_LIST
}

MODULE_LIST.forEach(name =>{
    const configs=['Debug','Release'];
    configs.forEach( c =>{
        
        var mpath = `${__dirname}/${name}/build/${c}/${name}`;
        if(fs.existsSync(`${mpath}.node`)){
            if(exports[c] == undefined){
                exports[c] = {MODULE_LIST}
            }
			console.log(`${c} = ${name}`);

            exports[c][name] = require(mpath);
            if(CONFIG_ == c ){
                exports[name] = exports[c][name];
				console.log(`*** ${name}`,exports[name])
            }
        }
    })
})
module.exports = exports;
//