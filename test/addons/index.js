
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

const MODULE_LIST = findAddons(__dirname)
var exports={
    MODULE_LIST
}

MODULE_LIST.forEach(name =>{
    var config = process.env.NODE_ENV || 'Release'
    
    var mpath = `${__dirname}/${name}/build/${config}/${name}`;

    if(fs.existsSync(`${mpath}.node`)){
        exports[name] = require(mpath);
    }
})
module.exports = exports;
//