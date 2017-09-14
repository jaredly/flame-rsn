
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
  render: fun {handle, reduce} => {
    <div className=Glamor.(css [
      border "1px solid #aaa",
      cursor "pointer",
    ])>
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state: (ctx, _)} => ctx := Some context))
      />
      <button onClick=(reduce (fun _ => 100000))>(str "100k")</button>
      <button onClick=(reduce (fun _ => 500000))>(str "500k")</button>
      <button onClick=(reduce (fun _ => 1000000))>(str "1m")</button>
      <button onClick=(reduce (fun _ => 10000000))>(str "10m")</button>
    </div>
  }
};


