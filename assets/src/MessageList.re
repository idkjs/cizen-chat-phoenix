open Message;
[@react.component]
let make = (~messages) => {
  <div className="c-messages">
    {messages
     |> Array.mapi((i: int, msg: Message.t) =>
          <div className="c-message" key={string_of_int(i)}>
            <b> {React.string(msg.body)} </b>
            <i> {React.string(" by " ++ msg.avatar_id)} </i>
          </div>
        )
     |> React.array}
  </div>;
};
