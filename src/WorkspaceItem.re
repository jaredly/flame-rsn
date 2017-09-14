
type item = {
  enabled: bool,
  weight: int,
  attractor: Library.attractor,
};

let str = ReasonReact.stringToElement;

let size = 200;
let fsize = (float_of_int size) /. 2.;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let draw item ctx => {
  MyDom.Canvas.setStrokeStyle ctx "rgba(100, 100, 100, 0.3)";
  MyDom.Canvas.strokeRect ctx 50. 50. 100. 100.;
  DrawUtils.showGrid (Library.run item.attractor) ctx fsize fsize 40;
};

let component = ReasonReact.reducerComponent "WorkspaceItem";
let make ::toggleEnabled ::item _children => {
  ...component,
  initialState: fun () => ref None,
  reducer: fun () _ => ReasonReact.NoUpdate,
  didMount: fun {state} => {
    !state |> consume (draw item);
    ReasonReact.NoUpdate
  },
  didUpdate: fun {newSelf: {state}} => {
    /* !state |> consume (draw item); */()
  },
  render: fun {handle} => {
    <div className=Glamor.(css [
      border (item.enabled ? "5px solid #aaa" : "5px solid #fff"),
      cursor "pointer",
      margin "10px",
    ]) onClick=(fun _ => toggleEnabled ())>
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state} => state := Some context))
      />
      <div>
        (str (Library.name item.attractor))
      </div>
    </div>
  }
};
