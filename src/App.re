
let str = ReasonReact.stringToElement;
let style = ReactDOMRe.Style.make;

type workspaceItem = {
  enabled: bool,
  weight: int,
  attractor: Library.attractor,
};

type state = {
  workspace: list workspaceItem,
  library: list LibraryItem.item,
};

type action = None;

let component = ReasonReact.reducerComponent "Page";
let make ::initialState _children => {
  ...component,
  initialState: fun () => initialState,
  reducer: fun () _ => {
    ReasonReact.NoUpdate
  },
  render: fun {state} => {
    <div className=(Glamor.(css [
      alignSelf "center",
      margin "50px",
      width "300px"
    ]))>
    <div className=(Glamor.(css [
      margin "20px",
      textAlign "center",
      fontSize "1.3em",
    ]))>
      (List.mapi
      (fun i item => <LibraryItem key=(string_of_int i) item />)
      state.library |> Array.of_list |> ReasonReact.arrayToElement)
      </div>
    </div>
  }
};

