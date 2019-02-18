module.exports = {
	scripts: {
		/*
            Windows scripts, run on a windows environment
        */
		win: {

			build: 'cmd /c test\\addons\\build.cmd',

		},
		/*
            Unix scripts, run on a unix environment
        */
		nix: {
			build: 'bash test/addons/build',

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