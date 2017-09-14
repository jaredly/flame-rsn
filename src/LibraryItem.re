
type libraryShowMode = Grid | Lines | Both;
type item = {
  attractor: Library.attractor,
  showMode: libraryShowMode,
};

let module RetinaCanvas = {
  let component = ReasonReact.statelessComponent "RetinaCanvas";
  let make ::width ::height ::onContext _ => {
    let rendered = ref false;
    {
    ...component,
    render: fun _ => {
      <canvas
        width=(string_of_int (width * 2))
        height=(string_of_int (height * 2))
        ref=(fun canvas => {
          if (not !rendered) {
            switch (Js.Null.to_opt canvas) {
            | Some canvas => {
              rendered := true;
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
  }
};

let str = ReasonReact.stringToElement;

let size = 200;
let fsize = (float_of_int size) /. 2.;

let component = ReasonReact.reducerComponent "LibraryItem";
let make ::item _children => {
  ...component,
  initialState: fun () => ref None,
  reducer: fun () _ => ReasonReact.NoUpdate,
  didMount: fun {state} => {
    switch (!state) {
    | Some ctx => {
      MyDom.Canvas.setStrokeStyle ctx "rgba(100, 100, 100, 0.3)";
      MyDom.Canvas.strokeRect ctx 50. 50. 100. 100.;
      DrawUtils.showGrid (Library.run item.attractor) ctx fsize fsize 40;
    }
    | None => ()
    };
    ReasonReact.NoUpdate
  },
  didUpdate: fun {newSelf: {state}} => {
    switch (!state) {
    | Some ctx => {
      MyDom.Canvas.setStrokeStyle ctx "rgba(100, 100, 100, 0.3)";
      MyDom.Canvas.strokeRect ctx 50. 50. 100. 100.;
      DrawUtils.showGrid (Library.run item.attractor) ctx fsize fsize 40;
    }
    | None => ()
    };
  },
  render: fun {handle} => {
    <div>
    <RetinaCanvas
      width=size
      height=size
      onContext=(handle (fun context {state} => state := Some context))
    />
    <div>
    (str (Library.name item.attractor))
    </div>
    </div>
  }
};
