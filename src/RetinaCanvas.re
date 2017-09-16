
let component = ReasonReact.reducerComponent "RetinaCanvas";
let make ::width ::height ::onClick=? ::onMouseDown=? ::onMouseMove=? ::onMouseUp=? ::onContext _ => {
  {
  ...component,
  initialState: fun () => ref false,
  reducer: fun () _ => ReasonReact.NoUpdate,
  render: fun {state} => {
    <canvas
      width=(string_of_int width)
      onClick=?onClick
      onMouseDown=?onMouseDown
      onMouseMove=?onMouseMove
      onMouseUp=?onMouseUp
      height=(string_of_int height)
      style=ReactDOMRe.Style.(make
        width::(string_of_int (width / 2) ^ "px")
        height::(string_of_int (height / 2) ^ "px")
        ()
      )
      ref=(fun canvas => {
        if (not !state) {
          switch (Js.Null.to_opt canvas) {
          | Some canvas => {
            state := true;
            let element = MyDom.domElToCanvas canvas;
            let ctx = MyDom.getContext element;
            /* MyDom.Canvas.scale ctx 2. 2.; */
            onContext ctx;
          }
          | None => ()
          }
        }
      })
    />
    }
  }
};