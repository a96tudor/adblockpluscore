/*
 * This file is part of Adblock Plus <http://adblockplus.org/>,
 * Copyright (C) 2006-2014 Eyeo GmbH
 *
 * Adblock Plus is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * Adblock Plus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Adblock Plus.  If not, see <http://www.gnu.org/licenses/>.
 */

"use strict";

/**
 * @fileOverview
 * This is an implementation of typed objects similar to the ECMAScript Harmony
 * proposal (http://wiki.ecmascript.org/doku.php?id=harmony:typed_objects).
 * The main difference is that it allows creating actual objects rather than
 * merely structured data.
 *
 * Defining a type
 * ---------------
 *
 *    const Point2D = new ObjectType({
 *                      x: uint32,
 *                      y: uint32,
 *                      rotate: function() { ... }
 *                    }, {
 *                      constructor: function(x, y) { ... },
 *                      bufferSize: 16
 *                    });
 *
 * The first parameter to ObjectType defines object properties and methods. A
 * name can either be associted with a type (property) or function (method).
 * Numeric value types from the ECMAScript Harmony proposal are predefined as
 * well as "boolean" which is an alias for uint8. In addition to that, already
 * defined object types can be used.
 *
 * The optional second parameter sets type metadata:
 *
 *    constructor: function that will be called whenever an object of the type
 *        is created.
 *    bufferSize: number of objects that should be placed into a single typed
 *        buffer (by default 128).
 *
 * Creating an object instance
 * ---------------------------
 *
 *    var point = Point2D(5, 10);
 *    point.rotate(10);
 *    Console.log(point.x + ", " + point.y);
 *
 * The parameters 5 and 10 will be passed to the constructor function defined
 * for this type.
 */

function forwardExports(module)
{
  let moduleExports = require(module);
  for (let key in moduleExports)
    exports[key] = moduleExports[key];
}

forwardExports("typedObjects/primitiveTypes");
forwardExports("typedObjects/objectTypes");