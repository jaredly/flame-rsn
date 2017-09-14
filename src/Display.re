
let str = ReasonReact.stringToElement;

let size = 400;
let fsize = (float_of_int size) /. 2.;
let ffsize = (float_of_int size);
let qsize = fsize /. 2.;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let rand: unit => float = [%bs.raw "function() {return Math.random()}"];
let runit () => rand () *. 2. -. 1.;

let choose list => List.nth list (int_of_float (rand () *. (float_of_int (List.length list))));

let scale x by off => (x *. by) +. off;

let draw attractors ctx => {
  MyDom.Canvas.clearRect ctx 0. 0. ffsize ffsize;
  MyDom.Canvas.setStrokeStyle ctx "rgba(100, 100, 100, 0.3)";
  MyDom.Canvas.strokeRect ctx qsize qsize fsize fsize;
  let pos = ref (runit (), runit ());
  /* Js.log (!pos); */
  MyDom.Canvas.setFillStyle ctx "#000";
  if (attractors != []) {
    for i in 0 to 100000 {
      let (x, y) = !pos;
      /* Js.log (x, y); */
      MyDom.Canvas.fillRect ctx (scale x qsize fsize) (scale y qsize fsize) 1. 1.;
      let attractor = choose attractors;
      pos := (Library.run attractor) !pos;
    };
  };
  /* switch attractors {
  | [] => ()
  | [attractor, ..._] => {
    DrawUtils.showGrid (Library.run attractor) ctx fsize fsize 40;
  }
  } */
};

let component = ReasonReact.reducerComponentWithRetainedProps "Display";
let make ::attractors _children => {
  ...component,
  initialState: fun () => ref None,
  reducer: fun () _ => ReasonReact.NoUpdate,
  retainedProps: attractors,
  didMount: fun {state} => {
    !state |> consume (draw attractors);
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps}, newSelf: {state}} => {
    if (retainedProps != attractors) {
      !state |> consume (draw attractors);
    }
  },
  render: fun {handle} => {
    <div className=Glamor.(css [
      border "1px solid #aaa",
      cursor "pointer",
    ])>
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state} => state := Some context))
      />
    </div>
  }
};


