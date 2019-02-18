module.exports = {
	scripts: {
		/*
            Windows scripts, run on a windows environment
        */
		win: {

			build: 'test\\addons\\build.cmd',
			build_debug: 'SET NODE_ENV=Debug&& test\\addons\\build.cmd',
			build_release: 'SET NODE_ENV=Release&& test\\addons\\build.cmd',

			


		},
		/*
            Unix scripts, run on a unix environment
        */
		nix: {
			build: 'bash test/addons/build',
			build_debug: 'export NODE_ENV=Debug && bash test/addons/build',
			build_release: 'export NODE_ENV=Release && bash test/addons/build',

			test_debug: 'export NODE_ENV=Debug  && npx mocha test/*.test.js',
			test_release: 'export NODE_ENV=Release && npx mocha test/*.test.js',

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