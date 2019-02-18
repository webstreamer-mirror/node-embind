
const fs = require('fs');
const log = console.log;


function findAddons(path){
    var pa = fs.readdirSync(path);
    var dirs=[];

	pa.forEach(function(name,index){
		var info = fs.statSync(path+"/"+name)	
		if(info.isDirectory()){
            dirs.push(name)
		}	
    })
    return dirs;
}

var CONFIG_ = 'Release'
if(process.env.NODE_ENV){
    var c = process.env.NODE_ENV.replace(/(^\s*)|(\s*$)/g, "").toUpperCase()
    if( c == 'DEBUG'){
        CONFIG_ = 'Debug'
    }
}


const MODULE_LIST = findAddons(__dirname)
console.log(MODULE_LIST)
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
            
            exports[c][name] = require(mpath);
            if(CONFIG_ == c ){
                exports[name] = exports[c][name];
            }
        }
    })
})
module.exports = exports;
//