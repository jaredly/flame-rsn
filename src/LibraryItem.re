
type libraryShowMode = Grid | Lines | Both;
type item = {
  attractor: Library.attractor,
  showMode: libraryShowMode,
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


let component = ReasonReact.reducerComponentWithRetainedProps "LibraryItem";
let make ::item _children => {
  ...component,
  initialState: fun () => ref None,
  reducer: fun () _ => ReasonReact.NoUpdate,
  retainedProps: item,
  didMount: fun {state} => {
    !state |> consume (draw item);
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps}, newSelf: {state}} => {
    if (retainedProps !== item) {
      !state |> consume (draw item);
    }
  },
  render: fun {handle} => {
    <div className=Glamor.(css [
      border "1px solid #aaa",
      margin "10px",
    ])>
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
