
type message =
  | Render string (list (int, Library.attractor)) int int
  ;

type workerMessage =
  | Rendered string (array int, int)
  | Blit string MyDom.imagedata int;

type worker;

external init: string => worker = "Worker" [@@bs.new];
external postMessage: worker => message => unit = "" [@@bs.send];
external addEventListener: worker => string => (Js.t {.data: workerMessage} => unit) => unit = "" [@@bs.send];

let worker = init "./worker.js";
let postMessage = postMessage worker;

let listeners = Hashtbl.create 20;

let listen id fn => Hashtbl.add listeners id fn;
let unlisten id => Hashtbl.remove listeners id;

addEventListener worker "message" (fun evt => {
  switch evt##data {
  | Rendered id (mx, max) => {
    ()
  }
  | Blit id data iters => {
    if (Hashtbl.mem listeners id) {
      let fn = Hashtbl.find listeners id;
      fn (data, iters);
      /* fn (mx, max); */
    } else {
      Js.log "No listener"
    }

  }
  };
});
