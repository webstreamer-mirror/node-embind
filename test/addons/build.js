const chalk = require('chalk');

const {GYP} = require('../../src/GYP')

const log = console.log;
const _B = chalk.blue;
const _G = chalk.green;
const _R = chalk.red;
var _BuildingProject = null;

//console.log(process.argv)
async function Main(argv) {
    var debug = false;
    var prj = [];
    var command = undefined;
    argv.forEach(element => {
        if(element.slice(0,2) == '--') {
            var opt = element.slice(2);
            if(opt == 'debug'){
                debug = true;
            } else {
                GYP.COMMANDS.forEach(name =>{
                    if(opt ==name){
                        command =[name];
                    }
                });
            }
        } else {
            prj.push(element);
        }
    });

    if (!command) {
        command = ['configure', 'rebuild'];
    }

    if(prj.length <1){
      prj =[
	    'hello',
        'class_','fn','value_object','em_asm','em_js','val','vector','map_',
        'enum_', 'constants', 'unique_ptr'
      ];
    }

    for (var i in prj) {
        var name = prj[i];
        _BuildingProject = name;
        var path = `${__dirname}/${name}`
        var gyp = new GYP(path, debug);

        if (process.env.PYTHON2_HOME){
            gyp.options.python2 = process.env.PYTHON2_HOME
        }
        
        for (var j in command) {
            var cmd = command[j];
            await gyp.run_(cmd);
        }

    }
}

/**
 *     Main
 */
var argv = process.argv.slice(2);
Main(argv).then(() => {
    log(_G(`[Build ${_BuildingProject}]  OK !`))
    process.exitCode = 0;
}).catch((err) => {
    log(err);

    log(_R(`[Build ${_BuildingProject}]  Failed !`))
    process.exitCode = 99;
});
