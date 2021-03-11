open Phx;
open MessageMap;

let subtract = (a1, a2) => {
  let l2 = Array.to_list(a2);
  Array.to_list(a1) |> List.filter(e => !List.mem(e, l2)) |> Array.of_list;
};

type ready_state = {
  id: string,
  name: string,
  socket: Socket.t,
  channel: Channel.t,
  rooms: Room.by_room_id,
  available: array(string), /* redundant? */
  entered: array(string),
  messages: Messages.by_room_id_t,
  text: string,
  selected: option(string),
};

type state =
  | Connecting
  | Ready(ready_state)
  | Error;

type action =
  | Connected(string, string, Socket.t, Channel.t)
  | RoomCreated(string, string, string)
  | RoomEnter(string)
  | RoomSelect(string)
  | ReceiveRoomSetting(string, string, string)
  | ReceiveAvatarProfile(string, string)
  | Receive(string, string, string)
  | UpdateText(string);
  // let (state, dispatch) = React.useReducer(
  //   (state, action) =>
  //     switch (action) {
  //     | Tick => {count: state.count + 1}
  //     },
  //   {count: 0}
  // );
let reducer = (state, action) =>
  switch (action) {
  | Connected(id, name, socket, channel) =>
    let state =
      Ready({
        id,
        name,
        socket,
        channel,
        rooms: Room.RoomMap.empty,
        available: [||],
        entered: [||],
        messages: MessageMap.empty,
        text: "",
        selected: None,
      });
    state;

  | Receive(source, room_id, body) =>
    switch (state) {
    | Ready({messages} as state) =>
      let state =
        Ready({
          ...state,
          messages: Message.addMsg(source, room_id, body, messages),
        });
      state;

    | _ => state
    }

  | RoomCreated(room_id, name, color) =>
    switch (state) {
    | Ready({rooms, available, entered} as state) =>
      let state =
        Ready({
          ...state,
          rooms: Room.upsertRoom(room_id, name, color, rooms),
          available: Room.uniqRooms(room_id, available),
          entered: Room.uniqRooms(room_id, entered),
          selected: Some(room_id),
        });
      state;
    | _ => state
    }
  | RoomEnter(room_id) =>
    switch (state) {
    | Ready({id, channel, entered} as state) =>
      push("room:enter", {"source": id, "room_id": room_id}, channel)
      |> ignore;

      let state =
        Ready({
          ...state,
          entered: Room.uniqRooms(room_id, entered),
          selected: Some(room_id),
        });
      state;
    | _ => state
    }
  | RoomSelect(room_id) =>
    switch (state) {
    | Ready({selected} as state) =>
      let state = Ready({...state, selected: Some(room_id)});
      state;
    | _ => state
    }
  | ReceiveRoomSetting(room_id, name, color) =>
    switch (state) {
    | Ready({rooms, available} as state) =>
      let state =
        Ready({
          ...state,
          rooms: Room.upsertRoom(room_id, name, color, rooms),
          available: Room.uniqRooms(room_id, available),
        });
      state;

    | _ => state
    }

  | ReceiveAvatarProfile(avatar_id, name) =>
    switch (state) {
    | Ready(state) =>
      let state = Ready({...state, name});
      state;
    | _ => state
    }

  | UpdateText(input) =>
    Js.log2("UpdateText", input);
    switch (state) {
    | Ready({text} as state) =>
      Js.log2("UpdateText", text);
      Js.log2("UpdateText_state", state);
      let state = Ready({...state, text: input});
      state;
    | _ => state
    };
};

[@react.component]
let make = () => {
  //   let (state, dispatch) = React.useReducer(
  //   (state, action) =>
  //     switch (action) {
  //     | Tick => {count: state.count + 1}
  //     },
  //   {count: 0}
  // );
  let (state, dispatch) = React.useReducer(reducer, Connecting);

  let sendAvatarProfile = name => {
    switch (state) {
    | Ready({id, channel}) =>
      push("avatar:profile", {"source": id, "name": name}, channel) |> ignore;
      /* Loopback. Update self state */
      dispatch(ReceiveAvatarProfile(id, name));
    | _ => ()
    };
  };
  let sendRoomSetting = (name_opt, color_opt) => {
    switch (state) {
    | Ready({id, channel, rooms, selected}) =>
      switch (selected) {
      | Some(room) =>
        let name =
          switch (name_opt) {
          | Some(name) => name
          | None => Room.getRoomName(room, rooms)
          };
        let color =
          switch (color_opt) {
          | Some(color) => color
          | None => Room.getRoomColor(room, rooms)
          };
        push(
          "room:setting",
          {"source": id, "room_id": room, "name": name, "color": color},
          channel,
        )
        |> ignore;
        /* Loopback. Update self state */
        dispatch(ReceiveRoomSetting(room, name, color));
      | None => ()
      }
    | _ => ()
    };
  };
  let connect = () => {
    Js.log("CONNECTING")
    let socket = initSocket("/socket") |> connectSocket(_);
    let channel = socket |> initChannel("lounge:hello", _);
    let _ =
      channel
      |> putOn("room:message", (res: Abstract.any) => {
           let {source, room_id, body}: Decode.message = Decode.receive(res);
           dispatch(Receive(source, room_id, body));
         })
      |> putOn("room:setting", (res: Abstract.any) => {
           let {room_id, name, color}: Decode.setting = Decode.setting(res);
           dispatch(ReceiveRoomSetting(room_id, name, color));
         })
      |> putOn("avatar:profile", (res: Abstract.any) => {
           let {avatar_id, name}: Decode.profile = Decode.profile(res);
           dispatch(ReceiveAvatarProfile(avatar_id, name));
         })
      |> joinChannel(_)
      |> putReceive("ok", (res: Abstract.any) => {
           let welcome: Decode.welcome = Decode.welcome(res);
           dispatch(Connected(welcome.id, welcome.name, socket, channel));
         });
    ();
  };

  React.useEffect0(() => {
    connect() |> ignore;
    None;
  });
  let roomCreate = () => {
    switch (state) {
    | Ready({id, channel}) =>
      push("room:create", {"source": id}, channel)
      |> putReceive("ok", (res: Abstract.any) => {
           let {room_id, name, color}: Decode.setting = Decode.setting(res);
           dispatch(RoomCreated(room_id, name, color));
         })
      |> ignore;
      ();
    | _ => ()
    };
  };
  let handleSend = () => {
    switch (state) {
    | Ready({id, channel, text, selected}) =>
      switch (selected) {
      | Some(room) =>
        push(
          "room:message",
          {"source": id, "room_id": room, "body": text},
          channel,
        )
        |> ignore;
        /* Loopback. Update self state */
        dispatch(Receive(id, room, text)) |> ignore;
        dispatch(UpdateText(""));
      | None => ()
      }
    | _ => ()
    };
  };

  <div className="p-container">
    {switch (state) {
     | Ready({name, rooms, available, entered, messages, text, selected}) =>
       <>
         <div className={Room.roomClassName(selected, rooms)}>
           <header className="c-header">
             {React.string("CizenChat")}
           </header>
           <div className="p-side-content">
             <InPlaceEdit
               name="user"
               text=name
               handleChange={value => sendAvatarProfile(value) |> ignore}
             />
             <button className="c-button" onClick={_event => roomCreate()}>
               {React.string("Create Room")}
             </button>
             <RoomList
               title="Available Rooms"
               rooms={Room.byIds(subtract(available, entered), rooms)}
               handleSelect={room => RoomEnter(room) |> dispatch}
             />
             <RoomList
               title="Joined Rooms"
               rooms={Room.byIds(entered, rooms)}
               handleSelect={room => RoomSelect(room) |> dispatch}
             />
           </div>
         </div>
         <div className="p-chat">
           <div className="c-chat">
             {switch (selected) {
              | Some(room) =>
                <>
                  <div className="c-chat-header">
                    <InPlaceEdit
                      name="room-title"
                      text={Room.getRoomName(room, rooms)}
                      handleChange={value =>
                        sendRoomSetting(Some(value), None) |> ignore
                      }
                    />
                    <ThemeChanger
                      handleChange={color =>
                        sendRoomSetting(None, Some(color)) |> ignore
                      }
                    />
                  </div>
                  <MessageList messages={Message.getMsg(room, messages)} />
                </>
              | None =>
                <p> {React.string("Select or create a room")} </p>
              }}
           </div>
           <div className="c-text-area-wrapper">
             <div className="c-text-area">
               <textarea
                 rows=1
                 placeholder="What's up?"
                 value=text
                 onKeyDown={event =>
                   if (ReactEvent.Keyboard.keyCode(event) === 13) {
                     ReactEvent.Keyboard.preventDefault(event);
                     handleSend() |> ignore;
                   }
                 }
                 onChange={event =>
                   dispatch(
                     UpdateText(ReactEvent.Form.target(event)##value),
                   )
                 }
               />
             </div>
           </div>
         </div>
         <div className="p-avatars" />
       </>
     | _ => <div> {React.string("Connecting...")} </div>
     }}
  </div>;
};
