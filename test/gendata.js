'use strict';

function r() { return Math.floor(Math.random() * 256); }

function h(n) { return n.toString(16); }

function c() { return '#' + h(r()) + h(r()) + h(r()); }

function h(n) { let s = n.toString(16); return s.length == 1 ? '0' + s : s; }

function l(n) {
  let a = [];
  a.length = n;
  for (let i = 0; i < n; ++i) {
    a[i] = i;
  }
  return a;
}

let randnums = l(60 * 60).map(() => c());

let hexstring = randnums.join('') + '.';

function d(s) { return parseInt(s, 16); }

let rgbstring = randnums.map((n) => {
  return [d(n.slice(1, 3)), d(n.slice(3, 5)), d(n.slice(5, 7))].join(',');
}).join(' ') + ' ';

let fs = require('fs');

fs.writeFileSync(__dirname + '/testinput', hexstring);
fs.writeFileSync(__dirname + '/testoutput', rgbstring);
