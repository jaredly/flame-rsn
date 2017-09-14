
let component = ReasonReact.reducerComponent "RetinaCanvas";
let make ::width ::height ::onClick=? ::onContext _ => {
  {
  ...component,
  initialState: fun () => ref false,
  reducer: fun () _ => ReasonReact.NoUpdate,
  render: fun {state} => {
    <canvas
      width=(string_of_int (width * 2))
      onClick=?onClick
      height=(string_of_int (height * 2))
      ref=(fun canvas => {
        if (not !state) {
          switch (Js.Null.to_opt canvas) {
          | Some canvas => {
            state := true;
            let element = MyDom.domElToCanvas canvas;
            let obj = MyDom.canvasElementToJsObj element;
            obj##style##width #= ((string_of_int width) ^ "px");
            obj##style##height #= ((string_of_int height) ^ "px");
            let ctx = MyDom.getContext element;
            MyDom.Canvas.scale ctx 2. 2.;
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