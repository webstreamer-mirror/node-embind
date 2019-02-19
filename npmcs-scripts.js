module.exports = {
	scripts: {
		/*
            Windows scripts, run on a windows environment
        */
		win: {

			build: 'test\\addons\\build.cmd >build.log 2>&1'
		},
		/*
            Unix scripts, run on a unix environment
        */
		nix: {
			build: 'bash test/addons/build >build.log 2>&1',
			build_docker: 'sudo -E docker run --rm -v $PWD:/home/ci/project webstreamer/node-gyp-gcc54 bash -c "source /opt/nvm/nvm.sh && cd project"'  
		},
		/* 
			Additionally add different commands for osx: (note that if running npmcs
			on osx and this is not specified it will then look to nix for the commands
			to run.
		*/
		osx:{

		}
	}
}