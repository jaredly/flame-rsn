
let str = ReasonReact.stringToElement;
let style = ReactDOMRe.Style.make;

open Library.T;

type state = {
  workspace: list item,
};

type action =
  | UpdateWorkspace int item;

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
          attractors=(List.filter (fun {enabled} => enabled) state.workspace |>
          List.map (fun {transform, weight} => (weight, transform)))
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
        (fun i item => <LocalWorkspaceItem
          key=(string_of_int i)
          toggleEnabled=(reduce (fun () => UpdateWorkspace i {...item, enabled: not item.enabled}))
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

