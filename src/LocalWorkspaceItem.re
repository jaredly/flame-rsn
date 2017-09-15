open Types;

let str = ReasonReact.stringToElement;

let size = 300;
let fsize = (float_of_int size) /. 2.;

let consume fn item => {
  switch item {
  | Some x => fn x
  | None => ()
  }
};

let flame items item ctx => {
  let items = List.map (fun i => i == item ? {...item, enabled: not item.enabled} : i) items;
  let attractors = List.filter (fun i => i.enabled) items
  |> List.map (fun i => (i.weight, i.attractor));
  Flame.draw ctx attractors size 40_000;
  ()
};

let evtValue event => (ReactDOMRe.domElementToObj (ReactEventRe.Form.target event))##value;

let component = ReasonReact.reducerComponentWithRetainedProps "WorkspaceItem";
let make ::setWeight ::toggleEnabled ::item ::items _children => {
  ...component,
  initialState: fun () => ref None,
  retainedProps: (item, items),
  reducer: fun () _ => ReasonReact.NoUpdate,
  didMount: fun {state: ctx} => {
    !ctx |> consume (DrawUtils.preview item size);
    !ctx |> consume (flame items item);
    ReasonReact.NoUpdate
  },
  didUpdate: fun {oldSelf: {retainedProps: (oldItem, oldItems)}, newSelf: {state: ctx}} => {
    if (oldItem != item) {
      !ctx |> consume (DrawUtils.preview item size);
    };
    if (oldItems != items) {
      !ctx |> consume (flame items item);      
    }
  },
  render: fun {handle} => {
    WorkspaceNode.render
      onContext::(handle (fun context {state: ctx} => ctx := Some context))
      ::size ::item ::toggleEnabled ::setWeight;
  }
};

