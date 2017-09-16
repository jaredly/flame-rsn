

let key = "flame-rsn:state";
let imgPrefix = "flame-rsn:img:";

let imgKey id => imgPrefix ^ id;

let loadState () fin => {
  LocalForage.getJson key (fun err json => {
    Js.log2 err json;
    let state = switch (Js.Null.to_opt json) {
    | None => {Js.log "nope"; None}
    | Some json => App.state__from_json json
    };
    let state = switch state {
    | None => {Js.log "failed to parse"; DefaultState.state}
    | Some state => state;
    };
    fin state;
  })
};

let saveState state => {
  Js.log "saveing";
  LocalForage.setJson key (App.state__to_json state) (fun err _ => {
    Js.log err;
    Js.log "Done saving";
  })
};

let saveImage id ctx fin => {
  MyDom.toBlob (MyDom.Canvas.canvas ctx) (fun blob => {
    LocalForage.setBlob (imgKey id) blob (fun err _ => {
      Js.log err;
      Js.log "Saved image";
      fin ();
    });
  });
};

let getImage id fin => LocalForage.getBlob (imgKey id) (fun err blob => {
  switch (Js.Null.to_opt blob) {
  | None => fin None
  | Some blob => fin (Some (MyDom.createObjectURL blob))
  }
});

let module Let_syntax = Continuations;
let main () => {
  [%consume let initialState = loadState ()];
  ReactDOMRe.renderToElementWithId <App initialState saveImage saveState getImage /> "index";
};

main();