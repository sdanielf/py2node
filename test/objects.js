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

var chai = require("chai"),
    expect = chai.expect,
    assert = chai.assert,
    py2node = require('../');


describe('PyObject', function () {
  describe('#Attributes', function () {
    it('should verify the existence of an attribute', function () {
      var pyobject = py2node.String('Hello World');
      assert.ok(pyobject.hasAttr('__str__'), 'attributes are not verified');
      assert.notOk(pyobject.hasAttr('false'),
                   'verification of attributes does not work')
    });
  });

  describe('#Booleans', function () {
    it('should create Python booleans with values True and False',
      function () {
        truebool = py2node.Bool(true);
        falsebool = py2node.Bool(false);
    });
    it('should convert Python values into JavaScript booleans',
      function () {
        var msg ='objects are not converted correctly' 
        assert.ok(truebool.toBool(), 'True ' + msg);
        assert.notOk(falsebool.toBool(), 'False ' + msg);
    });
  });

  describe('#Strings', function () {
    var string;
    it('should create Python strings from JS objects', function (){
      string = py2node.String('Hello world');
    });
    it('should convert Python objects to JS strings', function (){
      expect(string.toString()).to.be.equal('Hello world');
    });
    it('should get the representation of Python objects', function (){
      expect(string.toRepr()).to.be.equal("'Hello world'");
    });
  });
});
