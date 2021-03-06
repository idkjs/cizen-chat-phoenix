// Generated by ReScript, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Caml_js_exceptions = require("bs-platform/lib/js/caml_js_exceptions.js");
var MessageMap$CizenChat = require("./MessageMap.bs.js");

function addMsg(avatar_id, room, body, map) {
  var msg = {
    body: body,
    avatar_id: avatar_id
  };
  if (!Curry._2(MessageMap$CizenChat.MessageMap.mem, room, map)) {
    return Curry._3(MessageMap$CizenChat.MessageMap.add, room, [msg], map);
  }
  var messages = Curry._2(MessageMap$CizenChat.MessageMap.find, room, map);
  return Curry._3(MessageMap$CizenChat.MessageMap.add, room, $$Array.concat({
                  hd: messages,
                  tl: {
                    hd: [msg],
                    tl: /* [] */0
                  }
                }), map);
}

function getMsg(room, map) {
  try {
    return Curry._2(MessageMap$CizenChat.MessageMap.find, room, map);
  }
  catch (raw_exn){
    var exn = Caml_js_exceptions.internalToOCamlException(raw_exn);
    if (exn.RE_EXN_ID === "Not_found") {
      return [];
    }
    throw exn;
  }
}

var MessageMap = MessageMap$CizenChat.MessageMap;

exports.MessageMap = MessageMap;
exports.addMsg = addMsg;
exports.getMsg = getMsg;
/* MessageMap-CizenChat Not a pure module */
