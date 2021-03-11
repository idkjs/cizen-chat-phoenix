// Generated by BUCKLESCRIPT VERSION 4.0.7, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");

var component = ReasonReact.reducerComponent("InPlaceEdit");

function make(name, text, handleChange, _children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */component[/* didMount */4],
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (self) {
              var match = self[/* state */1];
              var tmp;
              if (match) {
                var str = match[0][/* text */0];
                tmp = React.createElement("input", {
                      value: str,
                      onKeyDown: (function ($$event) {
                          if ($$event.keyCode === 13) {
                            $$event.preventDefault();
                            Curry._1(handleChange, str);
                            return Curry._1(self[/* send */3], /* Toggle */0);
                          } else {
                            return 0;
                          }
                        }),
                      onChange: (function ($$event) {
                          return Curry._1(self[/* send */3], /* Update */[$$event.target.value]);
                        })
                    });
              } else {
                tmp = React.createElement(React.Fragment, undefined, React.createElement("span", {
                          className: "c-iedit-" + (name + "-content")
                        }, text), React.createElement("span", {
                          className: "c-iedit-" + (name + "-action"),
                          onClick: (function (_event) {
                              return Curry._1(self[/* send */3], /* Toggle */0);
                            })
                        }, "Edit"));
              }
              return React.createElement("div", {
                          className: "c-iedit-" + name
                        }, tmp);
            }),
          /* initialState */(function (param) {
              return /* Fixed */0;
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              if (action) {
                return /* Update */Block.__(0, [/* Editing */[/* record */[/* text */action[0]]]]);
              } else if (state) {
                return /* Update */Block.__(0, [/* Fixed */0]);
              } else {
                return /* Update */Block.__(0, [/* Editing */[/* record */[/* text */text]]]);
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.make = make;
/* component Not a pure module */
