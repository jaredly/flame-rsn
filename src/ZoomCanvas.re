
let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

type state = {
  ctx: ref (option MyDom.ctx),
  mouseStart: ref (option (float, float)),
} [@@noserialize];

let force v => switch v {
| Some v => v
| None => assert false
};

let component = ReasonReact.reducerComponent "ZoomCanvas";
let make ::size ::onZoom ::className=? _children => {
  ...component,
  initialState: fun () => {
    ctx: ref None,
    mouseStart: ref None,
  },
  reducer: fun () _ => ReasonReact.NoUpdate,
  render: fun {state: {ctx, mouseStart}} => {
      let fsize = float_of_int size;
      <canvas
        width=(string_of_int size)
        height=(string_of_int size)
        ref=(fun canvas => switch (Js.Null.to_opt canvas) {
        | None => ()
        | Some canvas => ctx := Some (MyDom.getContext (MyDom.domElToCanvas canvas))
        })
        className=?className
        onMouseDown=(fun evt => {
          [%guard let 0 = ReactEventRe.Mouse.button evt][@else ()];
          let ctx = force !ctx;
          let (tx, ty) = MyDom.getOffset (MyDom.Canvas.canvas ctx);
          let x = ReactEventRe.Mouse.clientX evt - tx|> float_of_int;
          let y = ReactEventRe.Mouse.clientY evt - ty|> float_of_int;
          mouseStart := Some (x, y);
        })

        /** TODO allow pressing escape to cancel */
        /* tabIndex=0
        onKeyDown=(fun evt => {
          switch (ReactEventRe.Keyboard.key evt) {
          | "Escape" => mouseStart := None
          | x => Js.log x
          }
        }) */

        onMouseLeave=(fun evt => {
          let ctx = force !ctx;
          MyDom.Canvas.clearRect ctx 0. 0. fsize fsize;
          mouseStart := None;
        })

        onMouseUp=(fun evt => {
          [%guard let 0 = ReactEventRe.Mouse.button evt][@else ()];
          [%guard let Some(x0, y0) = !mouseStart][@else ()];
          let ctx = force !ctx;
          let (tx, ty) = MyDom.getOffset (MyDom.Canvas.canvas ctx);
          let x = ReactEventRe.Mouse.clientX evt - tx|> float_of_int;
          let y = ReactEventRe.Mouse.clientY evt - ty|> float_of_int;
          MyDom.Canvas.clearRect ctx 0. 0. fsize fsize;
          MyDom.Canvas.setStrokeStyle ctx "rgba(255, 0, 0, 0.5)";
          MyDom.Canvas.line ctx (0., y) (fsize, y);
          MyDom.Canvas.line ctx (x, 0.) (x, fsize);

          mouseStart := None;

          let dx = x -. x0;
          let dy = y -. y0;
          let d = abs_float dx < abs_float dy ? dx : dy;
          let ((x0, y0), d) = if (ReactEventRe.Mouse.metaKey evt) {
            ((x0 -. d, y0 -. d), d *. 2.)
          } else {
            ((x0, y0), d)
          };
          onZoom ((x0, y0), d)
        })

        onMouseMove=(fun evt => {
          let ctx = force !ctx;
          let (tx, ty) = MyDom.getOffset (MyDom.Canvas.canvas ctx);
          let x = ReactEventRe.Mouse.clientX evt - tx|> float_of_int;
          let y = ReactEventRe.Mouse.clientY evt - ty|> float_of_int;
          MyDom.Canvas.clearRect ctx 0. 0. fsize fsize;
          MyDom.Canvas.setStrokeStyle ctx "rgba(255, 0, 0, 0.5)";
          switch !mouseStart {
          | None => {
            MyDom.Canvas.line ctx (0., y) (fsize, y);
            MyDom.Canvas.line ctx (x, 0.) (x, fsize);
          }
          | Some(x0, y0) => {
            let dx = x -. x0;
            let dy = y -. y0;
            let d = abs_float dx < abs_float dy ? dx : dy;
            let ((x0, y0), (x1, y1)) = if (ReactEventRe.Mouse.metaKey evt) {
              let x1 = x0 +. d;
              let y1 = y0 +. d;
              ((x0 -. d, y0 -. d), (x1, y1))
            } else {
              let x1 = x0 +. d;
              let y1 = y0 +. d;
              ((x0, y0), (x1, y1))
            };
            MyDom.Canvas.line ctx (0., y0) (fsize, y0);
            MyDom.Canvas.line ctx (x0, 0.) (x0, fsize);
            MyDom.Canvas.line ctx (0., y1) (fsize, y1);
            MyDom.Canvas.line ctx (x1, 0.) (x1, fsize);
          }
          };
        })
      />
  }
};

