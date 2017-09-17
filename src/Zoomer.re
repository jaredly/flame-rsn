
let size = 2000;
let fsize = float_of_int size;
let max_iterations = 100_000_000;

let blit data _ ctx => {
  MyDom.Canvas.putImageData ctx data 0. 0.;
};

let sendFlame id attractors iterations transform => {
  if (attractors != []) {
    let attractors = List.map (fun {Library.T.weight, transform} => (weight, transform)) attractors;
    /* WorkerClient.postMessage (WorkerClient.Render id attractors size iterations); */
    WorkerClient.postMessage (WorkerTypes.Render {id, attractors, size, iterations, transform});
  }
};

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

type matrix = ((float, float, float), (float, float, float));

type moving =
  | NotMoving
  | Moving MyDom.imageBitmap (int, int);

type state = {
  ctx: ref (option MyDom.ctx),
  id: string,
  iterations: int,
  /** TODO maybe this shouldn't be optional, might be faster that way */
  transform: option matrix,
  moving: moving,
} [@@noserialize];

type action = 
  | SetIterations int
  | ZoomTo ((float, float), float)
  | ZoomOut
  /* | StartMoving (int, int) MyDom.imageBitmap
  | StopMoving matrix */
  ;

let force v => switch v {
| Some v => v
| None => assert false
};

let combineMatricies ((a, b, c), (d, e, f)) ((a', b', c'), (d', e', f')) => {
  (
    (a *. a', 0., c +. c' *. a),
    (0., e *. e', f +. f' *. e)
  )
};

let spy x => {Js.log x; x};

let str = ReasonReact.stringToElement;
let component = ReasonReact.reducerComponentWithRetainedProps "Zoomer";
let make ::onClose ::attractors _children => {
  ...component,
  initialState: fun () => {
    ctx: ref None,
    id: DrawUtils.uid(),
    iterations: 0,
    transform: None,
    moving: NotMoving,
  },
  reducer: fun action state => switch action {
  | SetIterations iterations => ReasonReact.Update {...state, iterations}
  | ZoomOut => ReasonReact.Update {...state, transform: None}
  | ZoomTo ((x0, y0), sz) => {
    let ((a, b, c), (d, e, f)) = switch state.transform {
    | Some t => t
    | None => Library.idMatrix
    };
    /* let newSize = a *. sz; */
    let x0 = x0 *. 2.;
    let y0 = y0 *. 2.;
    let sz = sz *. 2.;
    let newScale = fsize /. sz *. a;
    let ox = c;
    let oy = f;
    let ox' = -. (x0 -. ox) /. a *. newScale;
    let oy' = -. (y0 -. oy) /. a *. newScale;

    let t = ((
      newScale,
      0.,
      ox',
    ), (
      0.,
      newScale,
      oy',
    ));
    Js.log4 t x0 y0 sz;
    Js.log fsize;
    Js.log2 (a, b, c) (d, e, f);

    ReasonReact.Update {...state, transform: Some t}
  }
  },
  retainedProps: attractors,
  didMount: fun {state: {ctx, id, transform}, reduce} => {
    sendFlame id attractors max_iterations transform;
    WorkerClient.listen id (fun (data, iters) => {
      !ctx |> consume (blit data size);
      reduce (fun () => SetIterations iters) ();
    });
    ReasonReact.NoUpdate
  },
  didUpdate: fun {
    oldSelf: {retainedProps, state: {transform: oldTransform}},
    newSelf: {state: {id, transform}}
  } => {
    if (retainedProps != attractors || transform !== oldTransform) {
      sendFlame id attractors max_iterations transform;
    }
  },
  willUnmount: fun {state: {id}} => {
    WorkerClient.unlisten id;
  },
  render: fun {handle, reduce, state: {iterations, ctx, transform, moving}} => {
    <div
      className=Glamor.(css [
        position "absolute",
        zIndex "10000",
        top    "0",
        left   "0",
        bottom "0",
        right  "0",
        justifyContent "center",
        alignItems "center",
      ])
      onClick=(fun _ => onClose ())
    >
    <div className=Glamor.(css[
      width "1000px",
      backgroundColor "white",
      boxShadow "0 2px 5px #aaa",
      position "relative",
    ])
      onClick=(fun evt => ReactEventRe.Mouse.stopPropagation evt)
    >
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state: {ctx}} => ctx := Some context))
      />
      <ZoomCanvas
        size=(size / 2)
        className=Glamor.(css[
          position "absolute",
          left "0",
          top "0",
        ])
        onZoom=(reduce (fun zoom => ZoomTo zoom))
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
