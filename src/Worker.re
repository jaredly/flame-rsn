
/** TODO make robust to absence of workers */

open WorkerTypes;
type self;
external self: self = "" [@@bs.val];
external onmessage: self => (Js.t {.data: message} => unit) => unit = "" [@@bs.set];
external postMessage: self => workerMessage => unit = "" [@@bs.send];

type workItem = {request: request, state: Flame.state};

let waiting = ref [];
let work = ref [];

onmessage self (fun evt => {
  switch evt##data {
  | Render request => {
    if (request.attractors !== []) {
      let state = Flame.init request.attractors request.size request.transform;
      let filtered = List.filter (fun item => item.request.id !== request.id) !waiting;
      waiting := [{request, state}, ...filtered];
    }
    /* Js.log "gof message"; */
  }
  | Stop id => {
    work := List.filter (fun item => item.request.id !== id) !work;
    waiting := List.filter (fun item => item.request.id !== id) !waiting;
  }
  }
});

let nextIterations num => {
  if (num < 100_000) {
    10_000
  } else if (num < 1_000_000) {
    100_000
  } else if (num < 10_000_000) {
    500_000
  } else {
    5_000_000
  }
};

let process () => {
  switch !work {
  | [] => {
    work := !waiting;
    waiting := [];
  }
  | [{request, state}, ...rest] => {
    work := rest;
    let current = !state.Flame.iteration;
    if (current < request.iterations) {
      let next = (nextIterations current);
      let next = next + current > request.iterations ? request.iterations - current : next;
      Flame.flameStep state next;
      let mmax = Flame.findMax state.mx state.size;
      let imagedata = MyDom.make state.size;
      Flame.renderToData imagedata state.size state.mx mmax;
      postMessage self (Blit request.id imagedata !state.iteration);
      waiting := [{request, state}, ...!waiting];
    }
  }
  };
  !work == [] && !waiting == []
};

Js.Global.setInterval (fun () => {
  let start = Js.Date.now();
  while (Js.Date.now() -. start < 200. && not (process())) {
    ()
  }
}) 10;