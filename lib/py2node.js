/*
 * Copyright (C) Daniel Francis, 2014
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

var Functional = require('functional-node').load();
var py2node = require('../build/Release/py2node.node');
var operator = py2node.import('operator');
module.exports = py2node;

function binop (name, a, b) {
  return operator.getAttr(name).call(a, b);
}

var binops = ['lt', 'le', 'eq', 'ne', 'ge', 'gt', 'is_', 'is_not', 'add',
  'and_', 'div', 'floordiv', 'lshift', 'mod', 'mul', 'or_', 'pow', 'rshift',
  'sub', 'truediv', 'xor', 'concat', 'contains', 'countOf', 'delitem',
  'getitem', 'indexOf'];

binops.forEach(function (name) {
  module.exports[name] = binop.partial(name);
});
