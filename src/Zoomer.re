
let size = 2000;
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
  | StartMoving (int, int) MyDom.imageBitmap
  | StopMoving matrix
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
  | StartMoving pos imageBitmap => {
      WorkerClient.stop state.id;
      ReasonReact.Update {...state, moving: Moving imageBitmap pos}
    }
  | StopMoving matrix => ReasonReact.Update {
    ...state, transform: Some (switch state.transform {
    | Some mx => {
      Js.log mx; Js.log matrix;
      spy (combineMatricies mx matrix)
    }
    | None => spy (matrix)
    }), moving: NotMoving}
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
    ])
      onClick=(fun evt => ReactEventRe.Mouse.stopPropagation evt)
    >
      <RetinaCanvas
        width=size
        height=size
        onContext=(handle (fun context {state: {ctx}} => ctx := Some context))
        onMouseDown=(fun evt => {
          [%guard let 0 = ReactEventRe.Mouse.button evt][@else ()];
          let x = ReactEventRe.Mouse.clientX evt;
          let y = ReactEventRe.Mouse.clientY evt;
          let canvas = MyDom.Canvas.canvas (force !ctx);
          let p = MyDom.createImageBitmap canvas;
          Js.Promise.then_ (fun image => {
            (reduce (fun image => StartMoving (x, y) image)) image;
            Js.Promise.resolve ()
          }) p |> ignore;
        })
        onMouseMove=?(switch moving {
        | Moving image (cx, cy) => {
          Some (fun evt => {
            let x = ReactEventRe.Mouse.clientX evt;
            let y = ReactEventRe.Mouse.clientY evt;
            let (dx, dy) = (x - cx |> float_of_int, y - cy |> float_of_int);
            let percent = dy /. (float_of_int size /. 4.);
            let ctx = force !ctx;
            let (tx, ty) = MyDom.getOffset (MyDom.Canvas.canvas ctx);
            let (cfx, cfy) = (float_of_int (cx - tx) *. 2., float_of_int (cy - ty) *. 2.);
            let zoom = 1. +. 4. *. percent;
            let nx = (cfx *. (1. -. zoom));
            let ny = (cfy *. (1. -. zoom));
            let fsize = float_of_int size;
            let ns = (fsize *. zoom);
            /* Js.logMany [|cfx, cfy, nx, ny, zoom|]; */
            MyDom.Canvas.clearRect ctx 0. 0. fsize fsize;
            MyDom.Canvas.drawImage ctx image nx ny ns ns;
          })
        }
        | _ => None
        })
        onMouseUp=?(switch moving {
        | Moving image (cx, cy) => {
          Some (reduce (fun evt => {
            let x = ReactEventRe.Mouse.clientX evt;
            let y = ReactEventRe.Mouse.clientY evt;
            let (dx, dy) = (x - cx |> float_of_int, y - cy |> float_of_int);
            let percent = dy /. (float_of_int size /. 4.);
            let ctx = force !ctx;
            let (tx, ty) = MyDom.getOffset (MyDom.Canvas.canvas ctx);
            let (cfx, cfy) = (float_of_int (cx - tx) *. 2., float_of_int (cy - ty) *. 2.);
            let zoom = 1. +. 4. *. percent;
            let nx = (cfx *. (1. -. zoom));
            let ny = (cfy *. (1. -. zoom));
            StopMoving ((zoom, 0., nx), (0., zoom, ny))
          }))
        }
        | _ => None
        })
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
