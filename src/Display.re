
let str = ReasonReact.stringToElement;

let size = 800;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let draw attractors iterations ctx => {
  switch attractors {
  | [] => ()
  | _ => Flame.draw ctx attractors size iterations;
  }
};


let sendFlame id attractors iterations => {
  WorkerClient.postMessage (WorkerClient.Render id attractors size iterations);
};


let uid: unit => string = [%bs.raw "function(){return Math.random().toString(16)}"];

let iterations = 10_000_000;

let component = ReasonReact.reducerComponentWithRetainedProps "Display";
let make ::attractors _children => {
  ...component,
  initialState: fun () => (ref None, uid()),
  reducer: fun () _ => ReasonReact.NoUpdate,
  retainedProps: attractors,
  /* didMount: fun {state: (ctx, id)} => {
    /* !ctx |> consume (draw attractors iterations); */
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps, state: (_, oldIterations)}, newSelf: {state: (ctx, iterations)}} => {
    if (retainedProps != attractors || oldIterations !== iterations) {
      !ctx |> consume (draw attractors iterations);
    }
  }, */
  didMount: fun {state: (ctx, id)} => {
    sendFlame id attractors iterations;
    WorkerClient.listen id (fun (mx, max) => {
      !ctx |> consume (fun ctx => Flame.render ctx mx max size)
    });
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps}, newSelf: {state: (ctx, id)}} => {
    if (retainedProps != attractors) {
      sendFlame id attractors iterations;
    }
  },
  willUnmount: fun {state: (_, id)} => {
    WorkerClient.unlisten id;
  },
  render: fun {handle, reduce, state: (_, iterations)} => {
    <div className=Glamor.(css [
      border "1px solid #aaa",
      cursor "pointer",
    ])>
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state: (ctx, _)} => ctx := Some context))
      />
    </div>
  }
};

    /* let nums = [|(100_000, "100k"), (500_000, "500k"), (1_000_000, "1m"), (10_000_000, "10m")|]; */
      /* <div className=Glamor.(css[flexDirection "row"])>
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
      </div> */

