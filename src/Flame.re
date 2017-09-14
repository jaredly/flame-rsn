
let rand: unit => float = [%bs.raw "function() {return Math.random()}"];
let runit () => rand () *. 2. -. 1.;

let choose list => List.nth list (int_of_float (rand () *. (float_of_int (List.length list))));

let scale x by off => (x *. by) +. off;

let flame attractors size iterations => {
  let ffsize = (float_of_int size);
  let fsize = (float_of_int size) /. 2.;
  let qsize = fsize /. 2.;
  let pos = ref (runit (), runit ());
  let mx = Array.make size (Array.make size 0);
  for x in 0 to (size - 1) {
    mx.(x) = Array.make size 0;
  };
  for i in 0 to iterations {
    let (x, y) = !pos;
    let attractor = choose attractors;
    pos := (Library.run attractor) !pos;
    if (i > 20) {
      let x = (scale x qsize fsize) |> int_of_float;
      let y = (scale y qsize fsize) |> int_of_float;
      if (x < 0 || x >= size || y < 0 || y >= size) {
        ()
      } else {
        mx.(x).(y) = mx.(x).(y) + 1;
      }
      /* Js.log (x, y); */
      /* MyDom.Canvas.fillRect ctx (scale x qsize fsize) (scale y qsize fsize) 1. 1.; */
    }
  };
  let max = ref 0;
  for x in 0 to (size - 1) {
    for y in 0 to (size - 1) {
      if (mx.(x).(y) > !max) {
        max := mx.(x).(y);
      }
    }
  };
  
  (mx, !max);
};
/* let flame  */


let draw ctx attractors size iterations => {
  let fsize = (float_of_int size) /. 2.;
  let ffsize = (float_of_int size);
  let qsize = fsize /. 2.;
  MyDom.Canvas.clearRect ctx 0. 0. ffsize ffsize;
  MyDom.Canvas.setStrokeStyle ctx "rgba(100, 100, 100, 0.3)";
  MyDom.Canvas.strokeRect ctx qsize qsize fsize fsize;

  MyDom.Canvas.setFillStyle ctx "rgba(100, 100, 100, 0.3)";

  let (mx, max) = flame attractors size iterations;

  MyDom.Canvas.setFillStyle ctx "#000";
  let fmax = float_of_int max;
  /* let fmax = log fmax /. fmax; */
  for x in 0 to (size - 1) {
    for y in 0 to (size - 1) {
      let n = (float_of_int mx.(x).(y));
      if (n > 0.) {

      /* let alpha = n /. fmax; */
      let alpha = log n /. log fmax;
      /* let alpha = fmax ** alpha; */
      MyDom.Canvas.setGlobalAlpha ctx alpha;
      MyDom.Canvas.fillRect ctx (float_of_int x) (float_of_int y) 1. 1.;
      }
    }
  }

  /* let pos = ref (runit (), runit ());
  if (attractors != []) {
    for i in 0 to iterations {
      if (i > 20) {
        let (x, y) = !pos;
        MyDom.Canvas.fillRect ctx (scale x qsize fsize) (scale y qsize fsize) 1. 1.;
        let attractor = choose attractors;
        pos := (Library.run attractor) !pos;
      }
    };
  }; */

};
