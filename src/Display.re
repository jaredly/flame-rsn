
let str = ReasonReact.stringToElement;

let size = 800;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let sendFlame id attractors iterations => {
  if (attractors != []) {
    WorkerClient.postMessage (WorkerClient.Render id attractors size iterations);
  }
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
  reducer: fun num (ctx, id, _) => ReasonReact.Update (ctx, id, num),
  retainedProps: attractors,
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
