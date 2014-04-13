var py2node = require('../lib/py2node.js'),
    builtin = py2node.import('__builtin__');

var array = [1, 2, 3, 4];
var mapped = array.map(function (i) {return py2node.Int(i)});
var list = py2node.List(mapped);
console.log(list.toRepr());
