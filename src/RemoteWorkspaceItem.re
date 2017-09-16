open Library.T;

let str = ReasonReact.stringToElement;

let size = 300;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let blit data size ctx => {
  MyDom.Canvas.putImageData ctx data 0. 0.;
};

let sendFlame id items item => {
  let items = List.map (fun (e, i) => i == item ? (not e, i) : (e, i)) items;
  let attractors = List.filter (fun (e, i) => e) items
  |> List.map (fun (_, i) => (i.weight, i.transform));
  /* WorkerClient.postMessage (WorkerClient.Render id transforms size 40_000); */
  WorkerClient.postMessage (WorkerTypes.Render {id, attractors, size, iterations: 40_000, transform: None});
};

let component = ReasonReact.reducerComponentWithRetainedProps "WorkspaceItem";
let make ::setWeight ::toggleEnabled ::enabled ::item ::items _children => {
  ...component,
  initialState: fun () => (ref None, DrawUtils.uid()),
  retainedProps: (item, items),
  reducer: fun () _ => ReasonReact.NoUpdate,
  didMount: fun {state: (ctx, id)} => {
    !ctx |> consume (DrawUtils.preview item size);
    sendFlame id items item;
    WorkerClient.listen id (fun (data, _iters) => {
      !ctx |> consume (blit data size);
      /* !ctx |> consume (fun ctx => Flame.render ctx mx max size) */
    });
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps: (oldItem, oldItems)}, newSelf: {state: (ctx, id)}} => {
    if (oldItem != item) {
      !ctx |> consume (DrawUtils.preview item size);
    };
    if (oldItems != items) {
      sendFlame id items item;
    }
  },
  willUnmount: fun {state: (_, id)} => {
    WorkerClient.unlisten id;
  },
  render: fun {handle} => {
    WorkspaceNode.render
      onContext::(handle (fun context {state: (ctx, _)} => ctx := Some context))
      ::enabled
      ::size ::item ::toggleEnabled ::setWeight;
  }
};
