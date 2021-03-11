[@react.component]
let make = (~title, ~rooms, ~handleSelect) => {
  <div className="c-list">
    <div className="c-list-header"> {React.string(title)} </div>
    <div className="c-list-body">
      {rooms
       |> Array.map((room: Room.t) =>
            <div
              className="c-list-item"
              key={room.id}
              onClick={_event => handleSelect(room.id)}>
              {React.string(room.name)}
            </div>
          )
       |> React.array}
    </div>
  </div>;
};
