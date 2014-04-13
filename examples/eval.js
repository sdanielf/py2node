var py2node = require('../lib/py2node.js'),
    builtin = py2node.import('__builtin__');

var eval_ = builtin.getAttr('eval');
var result = eval_.call(py2node.String('1 + 1'),
                        builtin.getAttr('dict').call(),
                        builtin.getAttr('dict').call());
console.log('1 + 1 = ' + result.toNumber());
