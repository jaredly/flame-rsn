
let size = 2000;
let max_iterations = 50_000_000;

let blit data _ ctx => {
  MyDom.Canvas.putImageData ctx data 0. 0.;
};

let sendFlame id attractors iterations => {
  if (attractors != []) {
    let attractors = List.map (fun {Library.T.weight, transform} => (weight, transform)) attractors;
    /* WorkerClient.postMessage (WorkerClient.Render id attractors size iterations); */
    WorkerClient.postMessage (WorkerTypes.Render {id, attractors, size, iterations, transform: None});
  }
};

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

type state = {
  ctx: ref (option MyDom.ctx),
  id: string,
  iterations: int,
} [@@noserialize];

let str = ReasonReact.stringToElement;
let component = ReasonReact.reducerComponentWithRetainedProps "Zoomer";
let make ::onClose ::attractors _children => {
  ...component,
  initialState: fun () => {ctx: ref None, id: DrawUtils.uid(), iterations: 0},
  reducer: fun iterations state => ReasonReact.Update {...state, iterations},
  retainedProps: attractors,
  didMount: fun {state: {ctx, id}, reduce} => {
    sendFlame id attractors max_iterations;
    WorkerClient.listen id (fun (data, iters) => {
      !ctx |> consume (blit data size);
      reduce (fun () => iters) ();
    });
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps}, newSelf: {state: {id}}} => {
    if (retainedProps != attractors) {
      sendFlame id attractors max_iterations;
    }
  },
  willUnmount: fun {state: {id}} => {
    WorkerClient.unlisten id;
  },
  render: fun {handle, state: {iterations}} => {
    <div
      className=Glamor.(css [
        position "absolute",
        zIndex "10000",
        top    "20px",
        left   "20px",
        bottom "20px",
        right  "20px",
        alignItems "center",
      ])
      onClick=(fun _ => onClose ())
    >
    <div className=Glamor.(css[
      width "1000px",
      backgroundColor "white",
      boxShadow "0 2px 5px #aaa",
    ])
      onClick=(fun evt => ReactEventRe.Mouse.stopPropagation evt)
    >
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state: {ctx}} => ctx := Some context))
      />
      <progress
        value=(string_of_int iterations)
        max=(string_of_int max_iterations)
        className=Glamor.(css[width "100%"])
      />
      </div>
    </div>
  }
};
