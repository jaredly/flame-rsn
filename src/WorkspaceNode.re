
open Library.T;

let evtValue event => (ReactDOMRe.domElementToObj (ReactEventRe.Form.target event))##value;
let str = ReasonReact.stringToElement;

let render ::onContext ::size ::toggleEnabled ::enabled ::item ::setWeight => {

    <div className=Glamor.(css [
      border (enabled ? "5px solid #f5a" : "5px solid #fff"),
      cursor "pointer",
      margin "5px",
      /* transition "box-shadow .3s ease", */
      Selector ":hover" [
        border (enabled ? "5px solid #fad" : "5px solid #fde")
      ]
    ])
    onClick=(fun _ => toggleEnabled ())
    >
      <RetinaCanvas
        width=(size * 2)
        height=size
        onContext
      />
      <div className=Glamor.(css[
        flexDirection "row",
        fontSize "12px",
        alignItems "center",
        padding "4px"
      ])>
        (str (item.name))
        <div className=Glamor.(css[flex "1"])/>
        (str "Weight:")
        <input
          value=(string_of_int item.weight)
          className=Glamor.(css[width "30px", marginLeft "8px"])
          onClick=(fun evt => ReactEventRe.Mouse.stopPropagation evt)
          _type="number"
          onChange=(fun evt => setWeight (int_of_string (evtValue evt)))
        />
      </div>
    </div>

}