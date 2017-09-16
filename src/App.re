
let str = ReasonReact.stringToElement;
let style = ReactDOMRe.Style.make;

open! Library.T;

type snapshot = {
  id: string,
  attractors: list item,
};

type state = {
  workspace: list (bool, item),
  snapshots: list snapshot,
};

type sstate = {
  state: state,
  zooming: bool,
};

type stateAction = 
  | AddSnapshot snapshot
  | ToggleEnabled int
  | UpdateWorkspace int item;

type action =
  | Change stateAction
  | Zoom
  | StopZoom;

let rec set list i item => switch list {
  | [] => []
  | [(enabled, one), ...rest] => if (i === 0) {
      [(enabled, item), ...rest]
    } else {
      [(enabled, one), ...set rest (i - 1) item]
    }
};

let component = ReasonReact.reducerComponent "Page";
let make ::getImage ::saveImage ::saveState ::initialState _children => {
  ...component,
  initialState: fun () => {state: initialState, zooming: false},
  reducer: fun action {state, zooming} => {
    switch action {
    | Change action => {
      let state = switch action {
      | UpdateWorkspace i item => {...state, workspace: set state.workspace i item}
      | ToggleEnabled index => {...state, workspace: List.mapi (fun i (e, t) => i === index ? (not e, t) : (e, t)) state.workspace}
      | AddSnapshot snapshot => {...state, snapshots: [snapshot, ...state.snapshots]}
      };
      saveState state;
      ReasonReact.Update {state, zooming}
    }
    | Zoom => ReasonReact.Update {state, zooming: true}
    | StopZoom => ReasonReact.Update {state, zooming: false}
    }
  },
  render: fun {state: {state, zooming}, reduce} => {
    let attractors = List.filter (fun (e, _) => e) state.workspace
            |> List.map (fun (_, i) => i);
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
          attractors
          saveImage
          onClick=(reduce (fun _ => Zoom))
          onSnapshot=(reduce (fun (id, attractors) => Change (AddSnapshot {id, attractors})))
          disabled=zooming
        />
        <div className=(Glamor.(css [
          flex "1",
          overflow "auto",
          /* flexWrap "wrap",
          width "300px", */
          alignItems "center",
        ]))>
          (List.map
          (fun {id, attractors} => {
            <Snapshot key=id id attractors getImage />
          })
          state.snapshots
          |> Array.of_list |> ReasonReact.arrayToElement)
        </div>
      </div>
      <div className=(Glamor.(css [
        flex "1",
        flexDirection "row",
        flexWrap "wrap",
        overflow "auto",
      ]))>
        (List.mapi
        (fun i (enabled, item) => <LocalWorkspaceItem
          key=(string_of_int i)
          enabled
          toggleEnabled=(reduce (fun () => Change (ToggleEnabled i)))
          setWeight=(reduce (fun weight => Change (UpdateWorkspace i {...item, weight})))
          items=state.workspace
          item
        />)
        state.workspace
        |> Array.of_list |> ReasonReact.arrayToElement)
      </div>
      (zooming ? <Zoomer
        attractors
        onClose=(reduce (fun _ => StopZoom))
      /> : ReasonReact.nullElement)
    </div>
  }
};

