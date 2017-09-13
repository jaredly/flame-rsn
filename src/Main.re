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
  /* showMovement fn ctx w h 20; */
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
  let r = sqrt((x ** 2.) +. y ** 2.);
  let s = 0.5 +. x ** 0.5 /. 2.;
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
};

/* let spiral (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  ((cos t +. sin r) /. r, (sin t -. cos r) /. r)
}; */

let hyperbolic a b (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  (sin (a *. t) /. r, r *. cos (b *. t))
};

let diamond a b c d (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  (sin (a *. t) *. cos (b *. r), cos (c *. t) *. sin (d *. r))
};

let ex a b (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  let p0 = sin(a *. t +. r);
  let p1 = cos(b *. t -. r);
  (p0 ** 3. +. p1 ** 3., p0 ** 3. -. p1 ** 3.)
};

let waves b c e f (x, y) => {
  (
    x +. b *. sin (y /. c ** 2.),
    y +. e *. sin (x /. f ** 2.)
  )
};

let fisheye a (x, y) => {
  let r = sqrt((x ** 2.) +. y ** 2.);
  let s = 1. /. (a *. r +. 1.);
  (x *. s, y *. s)
};

let fisheye2 a (x, y) => {
  let r = ((x ** 2.) +. y ** 2.);
  let s = 1. /. (a *. r +. 1.);
  (x *. s, y *. s)
};

let popcorn a b c d (x, y) => {
  (
    x +. a *. sin (tan (b *. y)),
    y +. c *. sin (tan (d *. x))
  )
};

let tangent a b c (x, y) => {
  (sin (a *. x) /. cos (b *. y), tan (c *. y))
};

let column (x, y) => {
  (sin x, y)
};

let affine a b c d e f (x, y) => {
  (
    /* a *. x +. b *. y +. c, */
    /* d *. x +. e *. y +. f */
    a *. x +. b *. y +. 0.1 *. c,
    d *. x +. e *. y +. 0.1 *. f,
  )
};

/* =  sin x cos y ,tan y */

let slide default fn node => {
  [%map let a = slider node (-8.) 8. 0.01 default
  and fn = fn node];
  fn a
};
let wrap fn _ => Observish.return fn;
let slide2 a b fn => slide a (slide b (wrap fn));
let slide3 a b c fn => slide a (slide b (slide c (wrap fn)));
let slide4 a b c d fn => slide a (slide b (slide c (slide d (wrap fn))));
let slide6 a b c d e f fn => 
fn
|> wrap
|> slide a
|> slide b
|> slide c
|> slide d
|> slide e
|> slide f
  ;

run (slide6 1. 1. 1. 1. 1. 1. affine);
run (wrap column);
run (slide3 1. 1. 1. tangent);
run (slide4 2.28 0.04 5.69 0.04 popcorn);
run (slide 1. (wrap fisheye));
run (slide 1. (wrap fisheye2));
run (slide4 0.29 0.17 0.21 0.04 waves);
/* 0.45 0.5 0.41 0.54 */
run (slide2 (-2.87) (-1.58) ex);
/* 1.95 0.83 */
run (slide4 1. 1. 2. 2. diamond);
run (slide2 2.82 1.87 hyperbolic);
run (slide2 2.45 (-2.54) handkercheif);
run (slide2 3.4 4.94 disc);
run (slide2 2.74 2.74 disc2);
run (slide4 1. 1. 1. 1. horseshoe);
run (slide2 2. 2. sinusoidal);
run (slide4 1. 1. 1. 1. swirl);
run (wrap spherical);
