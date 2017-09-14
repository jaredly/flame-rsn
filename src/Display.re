
let str = ReasonReact.stringToElement;

let size = 400;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let draw attractors iterations ctx => {
  MyDom.Canvas.clearRect ctx 0. 0. (float_of_int size) (float_of_int size);
  MyDom.Canvas.setStrokeStyle ctx "rgba(100, 100, 100, 0.3)";
  /* MyDom.Canvas.strokeRect ctx qsize qsize fsize fsize; */
  switch attractors {
  | [] => ()
  | _ => Flame.draw ctx attractors size iterations;
  }
};

let component = ReasonReact.reducerComponentWithRetainedProps "Display";
let make ::attractors _children => {
  ...component,
  initialState: fun () => (ref None, 100000),
  reducer: fun num (ctx, _) => ReasonReact.Update (ctx, num),
  retainedProps: attractors,
  didMount: fun {state: (ctx, iterations)} => {
    !ctx |> consume (draw attractors iterations);
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps, state: (_, oldIterations)}, newSelf: {state: (ctx, iterations)}} => {
    if (retainedProps != attractors || oldIterations !== iterations) {
      !ctx |> consume (draw attractors iterations);
    }
  },
  render: fun {handle, reduce, state: (_, iterations)} => {
    let nums = [|(100_000, "100k"), (500_000, "500k"), (1_000_000, "1m"), (10_000_000, "10m")|];
    <div className=Glamor.(css [
      border "1px solid #aaa",
      cursor "pointer",
    ])>
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state: (ctx, _)} => ctx := Some context))
      />
      <div className=Glamor.(css[flexDirection "row"])>
        (Array.map
        (fun (num, title) => (
          <button
            className=Glamor.(css[
              backgroundColor (num == iterations ? "#aaa": "white"),
              flex "1",
              padding "10px",
              cursor "pointer",
              border "none",
              outline "none",
            ])
            onClick=(reduce (fun _ => num))
          >
            (str title)
          </button>
        ))
        nums
        |> ReasonReact.arrayToElement)
      </div>
    </div>
  }
};


