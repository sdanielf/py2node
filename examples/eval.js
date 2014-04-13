var py2node = require('../lib/py2node.js'),
    builtin = py2node.import('__builtin__');

var eval_ = builtin.getAttr('eval');
var result = eval_.call(py2node.String('1 + 1'),
                        py2node.Dict(), py2node.Dict());
console.log('1 + 1 = ' + result.toNumber());
