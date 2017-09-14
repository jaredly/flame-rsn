
type item = {
  enabled: bool,
  weight: int,
  attractor: Library.attractor,
};

let str = ReasonReact.stringToElement;

let size = 150;
let fsize = (float_of_int size) /. 2.;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let draw item ctx => {
  MyDom.Canvas.setStrokeStyle ctx "rgba(100, 100, 100, 0.3)";
  MyDom.Canvas.translate ctx (float_of_int size) 0.;
  let qsize = fsize /. 2.;
  MyDom.Canvas.strokeRect ctx qsize qsize fsize fsize;
  DrawUtils.showMovement (Library.run item.attractor) ctx fsize fsize (size / 10);
  DrawUtils.showGrid (Library.run item.attractor) ctx fsize fsize (size / 5);
  MyDom.Canvas.translate ctx (float_of_int (-size)) 0.;
};

let flame items item ctx => {
  let items = List.map (fun i => i == item ? {...item, enabled: not item.enabled} : i) items;
  let attractors = List.filter (fun i => i.enabled) items
  |> List.map (fun i => (i.weight, i.attractor));
  Flame.draw ctx attractors size 10_000;
  ()
};

let evtValue event => (ReactDOMRe.domElementToObj (ReactEventRe.Form.target event))##value;

let component = ReasonReact.reducerComponentWithRetainedProps "WorkspaceItem";
let make ::setWeight ::toggleEnabled ::item ::items _children => {
  ...component,
  initialState: fun () => ref None,
  retainedProps: (item, items),
  reducer: fun () _ => ReasonReact.NoUpdate,
  didMount: fun {state} => {
    !state |> consume (draw item);
    !state |> consume (flame items item);
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps: (oldItem, oldItems)}, newSelf: {state}} => {
    if (oldItem != item) {
      !state |> consume (draw item);
    };
    if (oldItems != items) {
      !state |> consume (flame items item);      
    }
  },
  render: fun {handle} => {
    <div className=Glamor.(css [
      border (item.enabled ? "5px solid #f5a" : "5px solid #fff"),
      cursor "pointer",
      margin "5px",
    ])>
      <RetinaCanvas
        onClick=(fun _ => toggleEnabled ())
        width=(size * 2)
        height=size
        onContext=(handle (fun context {state} => state := Some context))
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
};

