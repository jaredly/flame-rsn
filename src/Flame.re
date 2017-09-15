
let rand: unit => float = [%bs.raw "function() {return Math.random()}"];
let runit () => rand () *. 2. -. 1.;

let choose array => Array.get array (int_of_float (rand () *. (float_of_int (Array.length array))));

let scale x by off => (x *. by) +. off;

let makeWeights attractors => {
  let total = List.fold_left
  (fun total (weight, _) => total + weight)
  0
  attractors;
  let weights = Array.make total 0;
  let at = ref 0;
  List.iteri 
  (fun i (weight, _) => {
    for _ in 0 to (weight - 1) {
      weights.(!at) = i;
      at := !at + 1;
    }
  })
  attractors;
  weights;
};

type state = {
  size: int,
  indices: array int,
  attractors: array Library.attractor,

  pos: ref (float, float),
  iteration: ref int,
  mx: array int,
};

let init attractors size => {
  indices: makeWeights attractors,
  attractors: Array.of_list (List.map snd attractors),
  pos: ref (runit (), runit ()),
  iteration: ref 0,
  /** TODO try a UInt32Array for better speed? */
  mx: Array.make (size * size) 0,
  size,
};

let flameStep state iterations => {
  let fsize = (float_of_int state.size) /. 2.;
  let qsize = fsize /. 2.;
  /** TODO try a UInt32Array for better speed? */
  for i in 0 to iterations {
    let (x, y) = !state.pos;
    let index = choose state.indices;
    let attractor = state.attractors.(index);
    state.pos := (Library.run attractor) !state.pos;
    if (i > 20) {
      let x = (scale x qsize fsize) |> int_of_float;
      let y = (scale y qsize fsize) |> int_of_float;
      if (x < 0 || x >= state.size || y < 0 || y >= state.size) {
        ()
      } else {
        state.mx.(x * state.size + y) = state.mx.(x * state.size + y) + 1;
      }
    }
  };
  state.iteration := !state.iteration + iterations;
};

let flame attractors size iterations => {
  let indices = makeWeights attractors;
  let attractarray = Array.of_list (List.map snd attractors);

  let fsize = (float_of_int size) /. 2.;
  let qsize = fsize /. 2.;
  let pos = ref (runit (), runit ());
  /** TODO try a UInt32Array for better speed? */
  let mx = Array.make (size * size) 0;
  for i in 0 to iterations {
    let (x, y) = !pos;
    let index = choose indices;
    let attractor = attractarray.(index);
    pos := (Library.run attractor) !pos;
    if (i > 20) {
      let x = (scale x qsize fsize) |> int_of_float;
      let y = (scale y qsize fsize) |> int_of_float;
      if (x < 0 || x >= size || y < 0 || y >= size) {
        ()
      } else {
        mx.(x * size + y) = mx.(x * size + y) + 1;
      }
    }
  };

  mx
};

let now: unit => float = [%bs.raw "function(){return performance.now()}"];

let render ctx mx max size => {
  let ffsize = (float_of_int size);
  MyDom.Canvas.setGlobalAlpha ctx 1.;
  MyDom.Canvas.setFillStyle ctx "black";
  MyDom.Canvas.fillRect ctx 0. 0. ffsize ffsize;

  MyDom.Canvas.setFillStyle ctx "#f5a";
  let fmax = float_of_int max;
  for x in 0 to (size - 1) {
    for y in 0 to (size - 1) {
      let n = (float_of_int mx.(x * size + y));
      if (n > 0.) {
        let alpha = log n /. log fmax;
        MyDom.Canvas.setGlobalAlpha ctx alpha;
        MyDom.Canvas.fillRect ctx (float_of_int x) (float_of_int y) 1. 1.;
      }
    }
  };
};

let renderToData data size mx max => {
  let buff = MyDom.data data;
  let fmax = float_of_int max;
  for x in 0 to (size - 1) {
    for y in 0 to (size - 1) {
      let n = (float_of_int mx.(x * size + y));
      let o = (y * size + x) * 4;
      if (n > 0.) {
        let alpha = log n /. log fmax;
        buff.(o) = (int_of_float (alpha *. 255.));
        buff.(o + 1) = (int_of_float (alpha *. 40.));
        buff.(o + 2) = (int_of_float (alpha *. 160.));
        buff.(o + 3) = 255;
        /*
        This is for "without a black background"
        buff.(o) = 255;
        buff.(o + 1) = 40;
        buff.(o + 2) = 160;
        buff.(o + 3) = (int_of_float (alpha *. 255.));
         */
      } else {
        buff.(o + 3) = 255;
      }
    }
  }
};

let findMax mx size => {
  let max = ref 0;
  for x in 0 to (size - 1) {
    for y in 0 to (size - 1) {
      if (mx.(x * size + y) > !max) {
        max := mx.(x * size + y);
      }
    }
  };
  !max;  
};

let draw ctx attractors size iterations => {
  let start = now ();

  let mx = flame attractors size iterations;
  let max = findMax mx size;

  Js.log (now () -. start);

  render ctx mx max size;
};
