open Types;

let str = ReasonReact.stringToElement;

let size = 300;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let sendFlame id items item => {
  let items = List.map (fun i => i == item ? {...item, enabled: not item.enabled} : i) items;
  let attractors = List.filter (fun i => i.enabled) items
  |> List.map (fun i => (i.weight, i.attractor));
  WorkerClient.postMessage (WorkerClient.Render id attractors size 40_000);
};

let uid: unit => string = [%bs.raw "function(){return Math.random().toString(16)}"];

let component = ReasonReact.reducerComponentWithRetainedProps "WorkspaceItem";
let make ::setWeight ::toggleEnabled ::item ::items _children => {
  ...component,
  initialState: fun () => (ref None, uid()),
  retainedProps: (item, items),
  reducer: fun () _ => ReasonReact.NoUpdate,
  didMount: fun {state: (ctx, id)} => {
    !ctx |> consume (DrawUtils.preview item size);
    sendFlame id items item;
    WorkerClient.listen id (fun data => {
      ()
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
      ::size ::item ::toggleEnabled ::setWeight;
  }
};
