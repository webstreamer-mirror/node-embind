const addons = require('./addons')

var chai = require('chai'),
  assert = chai.assert

describe('string', function() {
	const {
		get_non_ascii_string
	} = addons.string;

  it('non-ascii strings (utf8)', function() {
	  addons.string.EMBIND_STD_STRING_IS_UTF8 = true;
		var expected = '';
			//ASCII
			expected = 'aei';
			//Latin-1 Supplement
			expected += '\u00E1\u00E9\u00ED';
			//Greek
			expected += '\u03B1\u03B5\u03B9';
			//Cyrillic
			expected += '\u0416\u041B\u0424';
			//CJK
			expected += '\u5F9E\u7345\u5B50';
			//Euro sign
			expected += '\u20AC';
			console.log('C++==>',get_non_ascii_string(true).toString('utf-16'));
			console.log('JS-->',expected);
//		assert.equal(expected, get_non_ascii_string(true));
  });

  it('non-ascii strings (not utf8)', function() {
	  addons.string.EMBIND_STD_STRING_IS_UTF8 = false;
			for (var i = 0; i < 128; ++i) {
				expected += String.fromCharCode(128 + i);
			}
		console.log()
		assert.equal(expected, get_non_ascii_string());
	});
	
});	
	