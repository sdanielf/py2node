var py2node = require('../lib/py2node.js'),
    math = py2node.import('math');

var sin = math.getAttr('sin'),
    rad = math.getAttr('radians');

console.log('sin(90) = ' + sin.call(rad.call(py2node.Float(90))).toNumber())
