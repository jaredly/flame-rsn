
let str = ReasonReact.stringToElement;
let style = ReactDOMRe.Style.make;

type state = {
  workspace: list Types.item,
};

type action =
  | UpdateWorkspace int Types.item;

let rec set list i item => switch list {
  | [] => []
  | [one, ...rest] => if (i === 0) {
      [item, ...rest]
    } else {
      [one, ...set rest (i - 1) item]
    }
};

let component = ReasonReact.reducerComponent "Page";
let make ::initialState _children => {
  ...component,
  initialState: fun () => initialState,
  reducer: fun action state => {
    let state = switch action {
    | UpdateWorkspace i item => {workspace: set state.workspace i item}
    };
    ReasonReact.Update state
  },
  render: fun {state, reduce} => {
    <div className=(Glamor.(css [
      position "absolute",
      top "0",
      left "0",
      right "0",
      bottom "0",
      flexDirection "row",
    ]))>
      <div>
        <Display
          attractors=(List.filter (fun {Types.enabled} => enabled) state.workspace |>
          List.map (fun {Types.attractor, weight} => (weight, attractor)))
        />
        <div className=(Glamor.(css [
          flex "1",
          overflow "auto",
          alignItems "center",
        ]))>
        </div>
      </div>
      <div className=(Glamor.(css [
        flex "1",
        flexDirection "row",
        flexWrap "wrap",
        overflow "auto",
      ]))>
        (List.mapi
        (fun i item => <RemoteWorkspaceItem
          key=(string_of_int i)
          toggleEnabled=(reduce (fun () => UpdateWorkspace i {...item, enabled: not item.Types.enabled}))
          setWeight=(reduce (fun weight => UpdateWorkspace i {...item, weight}))
          items=state.workspace
          item
        />)
        state.workspace
        |> Array.of_list |> ReasonReact.arrayToElement)
      </div>
    </div>
  }
};

