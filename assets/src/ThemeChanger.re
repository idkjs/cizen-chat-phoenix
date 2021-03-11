[@react.component]
let make = (~handleChange) => {
  <div className="c-colors">
    <div
      onClick={_event => handleChange("red")}
      className="c-colors-item c-colors-item--red"
    />
    <div
      onClick={_event => handleChange("green")}
      className="c-colors-item c-colors-item--green"
    />
    <div
      onClick={_event => handleChange("blue")}
      className="c-colors-item c-colors-item--blue"
    />
    <div
      onClick={_event => handleChange("yellow")}
      className="c-colors-item c-colors-item--yellow"
    />
  </div>;
};
