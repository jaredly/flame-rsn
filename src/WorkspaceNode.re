
open Types;
open ReasonReact;

let evtValue event => (ReactDOMRe.domElementToObj (ReactEventRe.Form.target event))##value;
let str = ReasonReact.stringToElement;

let render ::onContext ::size ::toggleEnabled ::item ::setWeight => {

    <div className=Glamor.(css [
      border (item.enabled ? "5px solid #f5a" : "5px solid #fff"),
      cursor "pointer",
      margin "5px",
    ])>
      <RetinaCanvas
        onClick=(fun _ => toggleEnabled ())
        width=(size * 2)
        height=size
        onContext
      />
      <div>
        (str (Library.name item.attractor))
        <input
          value=(string_of_int item.weight)
          _type="number"
          onChange=(fun evt => setWeight (int_of_string (evtValue evt)))
        />
      </div>
    </div>

}