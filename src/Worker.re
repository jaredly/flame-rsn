
/** TODO make robust to absence of workers */

type self;
external self: self = "" [@@bs.val];
external onmessage: self => (Js.t {.data: WorkerClient.message} => unit) => unit = "" [@@bs.set];
external postMessage: self => WorkerClient.workerMessage => unit = "" [@@bs.send];

type workItem = {id: string, state: Flame.state, max: int};

let waiting = ref [];
let work = ref [];

onmessage self (fun evt => {
  switch evt##data {
  | WorkerClient.Render id attractors size max => {
    let state = Flame.init attractors size;
    let filtered = List.filter (fun item => item.id !== id) !waiting;
    waiting := [{id, state, max}, ...filtered];
    /* Js.log "gof message"; */
  }
  }
});

let nextIterations num => {
  if (num < 100_1000) {
    10_000
  } else if (num < 1_000_000) {
    100_000
  } else {
    500_000
  }
};

let process () => {
  switch !work {
  | [] => {
    work := !waiting;
    waiting := [];
  }
  | [{id, state, max}, ...rest] => {
    work := rest;
    let current = !state.Flame.iteration;
    if (current < max) {
      let next = (nextIterations current);
      let next = next + current > max ? max - current : next;
      Flame.flameStep state next;
      let mmax = Flame.findMax state.mx state.size;
      postMessage self (WorkerClient.Rendered id (state.mx, mmax));
      waiting := [{id, state, max}, ...!waiting];
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