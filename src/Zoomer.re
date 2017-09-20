
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

type state = {
  ctx: ref (option MyDom.ctx),
  id: string,
  exportLink: option string,
  iterations: int,
  /** TODO maybe this shouldn't be optional, might be faster that way */
  transform: option matrix,
} [@@noserialize];

type action = 
  | SetIterations int
  | SetDownloadLink string
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

let roundish f => (float_of_int (int_of_float (f *. 100.))) /. 100.;

let str = ReasonReact.stringToElement;
let component = ReasonReact.reducerComponentWithRetainedProps "Zoomer";
let make ::onClose ::attractors _children => {
  ...component,
  initialState: fun () => {
    ctx: ref None,
    id: DrawUtils.uid(),
    iterations: 0,
    exportLink: None,
    transform: None,
  },
  reducer: fun action state => switch action {
  | SetIterations iterations => ReasonReact.Update {...state, iterations}
  | SetDownloadLink link => ReasonReact.Update {...state, exportLink: Some link}
  | ZoomOut => ReasonReact.Update {...state, transform: None}
  | ZoomTo ((x0, y0), sz) => {
    let ((scale, _, ox), (_, _, oy)) = switch state.transform {
    | Some t => t
    | None => Library.idMatrix
    };
    let (x0, y0, sz) = (x0 *. 2., y0 *. 2., sz *. 2.);
    let newScale = fsize /. sz *. scale;
    let ox' = -. (x0 -. ox) /. scale *. newScale;
    let oy' = -. (y0 -. oy) /. scale *. newScale;
    ReasonReact.Update {...state, transform: Some (
      (newScale, 0., ox'),
      (0., newScale, oy')
    )}
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
  render: fun {handle, reduce, state: {iterations, ctx, transform, exportLink, id}} => {
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
      <div className=Glamor.(css[flexDirection "row", padding "8px"])>
        (switch exportLink {
        | None => <button onClick=(fun _ => {
          let canvas = MyDom.Canvas.canvas (force !ctx);
          MyDom.toBlob canvas (reduce (fun blob => {
            SetDownloadLink (MyDom.createObjectURL blob)
          }));
        })>(str "Export")</button>
        | Some link => <a href=link download="flame.png">(str "Download")</a>
        })
        <div className=Glamor.(css[flexBasis "8px"]) />
        <button onClick=(fun _ => WorkerClient.stop id)>(str "Stop")</button>
        <div className=Glamor.(css[flexBasis "8px"]) />
        <progress
          value=(string_of_int iterations)
          max=(string_of_int max_iterations)
          className=Glamor.(css[flex "1"])
        />
        <div className=Glamor.(css[flexBasis "8px"]) />
        (switch transform {
        | None => ReasonReact.nullElement
        | Some ((zoom, _, _), _) => <div
            onClick=(reduce (fun _ => ZoomOut))
          >
            (str ("Zoom: " ^ (string_of_float (roundish zoom))))
          </div>
        })
      </div>
      </div>
    </div>
  }
};
