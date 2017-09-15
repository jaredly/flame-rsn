
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

let max_iterations = 10_000_000;

let blit data size ctx => {
  MyDom.Canvas.putImageData ctx data 0. 0.;
};

let component = ReasonReact.reducerComponentWithRetainedProps "Display";
let make ::attractors _children => {
  ...component,
  initialState: fun () => (ref None, uid(), 0),
  reducer: fun num (ctx, id, iters) => ReasonReact.Update (ctx, id, num),
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
  didMount: fun {state: (ctx, id, _), reduce} => {
    sendFlame id attractors max_iterations;
    WorkerClient.listen id (fun (data, iters) => {
      !ctx |> consume (blit data size);
      reduce (fun () => iters) ();
      /* !ctx |> consume (fun ctx => Flame.render ctx mx max size) */
    });
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps}, newSelf: {state: (ctx, id, _)}} => {
    if (retainedProps != attractors) {
      sendFlame id attractors max_iterations;
    }
  },
  willUnmount: fun {state: (_, id, _)} => {
    WorkerClient.unlisten id;
  },
  render: fun {handle, reduce, state: (_, _, iterations)} => {
    <div className=Glamor.(css [
      border "1px solid #aaa",
      cursor "pointer",
    ])>
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state: (ctx, _, _)} => ctx := Some context))
      />
      <div>
      <progress
        value=(string_of_int iterations)
        max=(string_of_int max_iterations)
        className=Glamor.(css[width "100%"])
      />
        (str (string_of_int iterations))
      </div>
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

