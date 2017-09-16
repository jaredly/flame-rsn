
let str = ReasonReact.stringToElement;
let component = ReasonReact.reducerComponent "Snapshot";
let make ::id ::attractors ::getImage _children => {
  ...component,
  initialState: fun () => None,
  reducer: fun url _ => ReasonReact.Update url,
  didMount: fun {reduce} => {
    getImage id (reduce (fun url => url));
    ReasonReact.NoUpdate
  },
  render: fun {state} => {
    switch state {
    | None => <div
    >
      (str ("Loading : " ^ id))
    </div>
    | Some src => <img width=(string_of_int (Display.size / 4)) src/>
    }
  }
};