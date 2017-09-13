open DrawUtils;

let module Let_syntax = Observish;

let run fnn => {
  let w = 600.;
  let h = 600.;
  let node = MyDom.createBodyDiv ();
  let ctx = MyDom.createCtx node w h;

  [%consume
    let by = slider node 5. 140. 1. 80.
    and fn = fnn node
  ];
  let by = int_of_float by;

  MyDom.Canvas.clearRect ctx 0. 0. w h;
  let w = w /. 2.;
  let h = h /. 2.;
  showMovement fn ctx w h 20;
  showGrid fn ctx w h by;
};

let pi = 3.14159;

let sinusoidal a b (x,y) => (sin (x *. a), sin (y *. b));
let spherical (x, y) => {
  let r2 = x ** 2. +. y ** 2.;
  (x /. r2, y /. r2)
};
let swirl a b c d (x, y) => {
  let r2 = (x *. x) +. (y *. y);
  (x *. cos(a *. r2) -. y *. sin(b *. r2), x *. sin(c *. r2) +. y *. cos(d *. r2))
};
let horseshoe a b c d (x, y) => {
  let r = sqrt((x ** 2.) +. y ** 2.);
  ((a *. x -. b *. y) *. (c *. x +. d *. y) /. r, 2. *. x *. y /. r)
};
let disc a b (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  let s = 0.5 +. x ** 0.5 /. 2.;
  /* t /. pi; */
  (s *. sin (a *. pi *. r), s *. cos(b *. pi *. r))
};
let disc2 a b (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  let s = t /. pi;
  (s *. sin (a *. pi *. r), s *. cos(b *. pi *. r))
};

let handkercheif a b (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  (sin (a *. (t +. r)) *. r, cos (b *. (t -. r)) *. r)
  /* = r · (sin(θ + r), cos(θ − r)) */
};

let slide fn node => {
  [%map let a = slider node (-8.) 8. 0.01 1.
  and fn = fn node];
  fn a
};
let wrap fn _ => Observish.return fn;
let slide2 fn => slide (slide (wrap fn));
let slide3 fn => slide (slide (slide (wrap fn)));
let slide4 fn => slide (slide (slide (slide (wrap fn))));

run (slide2 handkercheif);
run (slide2 disc);
run (slide2 disc2);
run (slide4 horseshoe);
run (slide2 sinusoidal);
run (slide4 swirl);
run (wrap spherical);
