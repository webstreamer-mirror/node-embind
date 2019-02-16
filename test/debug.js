
const fs = require('fs');
const chalk = require('chalk');

const log = console.log;
const _B = chalk.blue;
const _G = chalk.green;
const _R = chalk.red;
const _Y = chalk.yellow;

var options = require('commander');
options
  .option('-r, --release', 'use release version to debuging')
  .option('-v, --verbose', 'verbose info or not')
  .parse(process.argv);


var Module = require('./addons');
CONFIG_ = 'Debug' 
if( options.release){
    CONFIG_ = 'Release' 
} 

var INFO = function(x){}
if(options.verbose){
    INFO = console.log;
}

INFO(chalk.bold(`========    node-embind debugging program  [${CONFIG_}]  ========`))

if(!Module[CONFIG_]){
    INFO(_R(`${CONFIG_} not exits in addon, please build it.`));
    process.exit(101);
}

var Addon=Module[CONFIG_];

INFO(`
    ----    modules infomation    ----
`)

Module.MODULE_LIST.forEach(name =>{
    var m = Addon[name];
    if(m){
        INFO(_B(`${name} :`))
        INFO(m)
    } else {
        INFO(_Y(`${name} : None`))
    }
    var mpath = `${__dirname}/addon/${name}/build/${CONFIG_}/${name}`
    if(!fs.existsSync(`${mpath}.node`)){
        INFO(_Y(`addon ${name} (${mpath}) not exists !`))
    }
})
const {
	hello,
    class_,fn,value_object,em_asm,em_js,val,vector,map_,
    enum_, unique_ptr, constants
} = Addon;
INFO(_B`    ----    program start    ----`)
/**********************************************/
/*      Debug Program                         */
/**********************************************/

console.log(hello.hello(),'<----------')

/**********************************************/
/*      Debug Program End                     */
/**********************************************/
log(_G`    ----    program done     ----`)
