
/* let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let sendFlame id attractors iterations size => {
  if (attractors != []) {
    let attractors = List.map (fun {Library.T.weight, transform} => (weight, transform)) attractors;
    WorkerClient.postMessage (WorkerTypes.Render {id, attractors, size, iterations, transform: None});
  }
};

let max_iterations = 10_000_000;

let blit data _ ctx => {
  MyDom.Canvas.putImageData ctx data 0. 0.;
};

let component = ReasonReact.reducerComponentWithRetainedProps "Display";
let make ::disabled ::size ::attractors _children => {
  ...component,
  initialState: fun () => (ref None, DrawUtils.uid(), 0),
  reducer: fun num (ctx, id, _) => ReasonReact.Update (ctx, id, num),
  retainedProps: (attractors, disabled),
  didMount: fun {state: (ctx, id, _), reduce} => {
    sendFlame id attractors max_iterations size;
    WorkerClient.listen id (fun (data, iters) => {
      !ctx |> consume (blit data size);
      reduce (fun () => iters) ();
    });
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps: (oldAttractors, oldDisabled)}, newSelf: {state: (_, id, _)}} => {
    if (oldAttractors != attractors) {
      sendFlame id attractors max_iterations;
    };
    if (oldDisabled !== disabled && disabled) {
      WorkerClient.stop id;
    }
  },
  willUnmount: fun {state: (_, id, _)} => {
    WorkerClient.unlisten id;
  },
  render: fun {handle, state: (ctx, _, iterations)} => {
    <div className=Glamor.(css [
      border "1px solid #aaa",
      cursor "pointer",
    ])>
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state: (ctx, _, _)} => ctx := Some context))
        onClick
      />
      <div>
      <progress
        value=(string_of_int iterations)
        max=(string_of_int max_iterations)
        className=Glamor.(css[width "100%"])
      />
        (str (string_of_int iterations))
      </div>
      <button onClick=(fun _ => {
        let id = DrawUtils.uid();
        !ctx |> consume (fun ctx => {
          saveImage id ctx (fun () => {
            onSnapshot (id, attractors)
          })
        })
      })>(str "Save Snapshot")</button>
    </div>
  }
};
 */
