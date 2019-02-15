const { spawn } = require('child_process');
const chalk = require('chalk');

const log = console.log;
const _B = chalk.blue;
const _G = chalk.green;
const _R = chalk.red;


class GYP {

    constructor(dir,debug) {
  
      this.options ={}
  
      if( typeof(dir) == 'object'){
        var opt = dir;
        this.options.dir = opt.dir || '.';
        this.options.debug = debug || opt.debug || false;
      } else {
        this.options.dir = dir || '.';
        this.options.debug = debug  || false;
      }
  
      this.options.dir = this.options.dir.replace(/\\/g, '/')
  
      var methods = ['build','rebuild','configure'];
      methods.forEach((name)=>{
        this[name]=function(){
          return this.run_(name);
        }
      })
    }
  
    run(command, options) {
      let gyp = null;
      if (process.platform == 'win32'){    
        
        options.splice(0,0,'/C','npx','node-gyp', command);
        gyp = spawn('cmd',options)
  
      }else {
  
        options.splice(0,0, 'node-gyp',command);
        console.log(options)
        gyp = spawn('npx',options)
  
      }
      
      // log(`@${command}`,options)
      gyp.stdout.on('data', (data) => {
        log(data.toString());
      });
    
      gyp.stderr.on('data', (data) => {
      log(data.toString());
      });
    
      return new Promise((resolve, reject) =>{
        gyp.on('close', (code) => {
          if(code ==0) {
            resolve();
          }else{
            reject();
          }
        });  
      });
    }
  
    run_(command) {
      var options =['-C',this.options.dir];
  
      if(process.platform == 'win32'){
        options.splice(0,0,'--msvs_version=2017','--python','c:/python27');
      }
  
      if(this.options.debug){
        options.splice(0,0,'--debug');
      }
  
      return this.run(command, options);
    }

 
  }

  GYP.COMMANDS=['clear','configure','build','rebuild'];
  module.exports ={
      GYP
  }