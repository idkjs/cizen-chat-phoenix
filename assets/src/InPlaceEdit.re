type editing_state = {text: string};

type state =
  | Fixed
  | Editing(editing_state);

type action =
  | Toggle
  | Update(string);

let reducer = (state, action) =>
  switch (action) {
  | Toggle =>
    switch (state) {
    | Fixed => state
    | Editing(_state) => Fixed
    }
  | Update(str) => Editing({text: str})
  };
[@react.component]
let make = (~name, ~text, ~handleChange) => {
  let (state, dispatch) = React.useReducer(reducer, Fixed);

  <div className={"c-iedit-" ++ name}>
    {switch (state) {
     | Fixed =>
       <>
         <span className={"c-iedit-" ++ name ++ "-content"}>
           {ReasonReact.string(text)}
         </span>
         <span
           className={"c-iedit-" ++ name ++ "-action"}
           onClick={_event => Toggle |> dispatch}>
           {ReasonReact.string("Edit")}
         </span>
       </>
     | Editing({text}) =>
       <input
         value=text
         onKeyDown={event =>
           if (ReactEvent.Keyboard.keyCode(event) === 13) {
             ReactEvent.Keyboard.preventDefault(event);
             handleChange(text);
             Toggle |> dispatch;
           }
         }
         onChange={event =>
           Update(ReactEvent.Form.target(event)##value) |> dispatch
         }
       />
     }}
  </div>;
};
