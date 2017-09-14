
let pi = 3.14159;

type fn = (float, float) => (float, float);

type p1 = float;
type p2 = (float, float);
type p3 = (float, float, float);
type p4 = (float, float, float, float);

let sinusoidal (a, b) (x,y) => (sin (x *. a), sin (y *. b));
let spherical (x, y) => {
  let r2 = x ** 2. +. y ** 2.;
  (x /. r2, y /. r2)
};
let swirl (a, b, c, d) (x, y) => {
  let r2 = (x *. x) +. (y *. y);
  (x *. cos(a *. r2) -. y *. sin(b *. r2), x *. sin(c *. r2) +. y *. cos(d *. r2))
};
let horseshoe (a, b, c, d) (x, y) => {
  let r = sqrt((x ** 2.) +. y ** 2.);
  ((a *. x -. b *. y) *. (c *. x +. d *. y) /. r, 2. *. x *. y /. r)
};
let disc (a, b) (x, y) => {
  let r = sqrt((x ** 2.) +. y ** 2.);
  let s = 0.5 +. x ** 0.5 /. 2.;
  (s *. sin (a *. pi *. r), s *. cos(b *. pi *. r))
};
let disc2 (a, b) (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  let s = t /. pi;
  (s *. sin (a *. pi *. r), s *. cos(b *. pi *. r))
};

let handkercheif (a, b) (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  (sin (a *. (t +. r)) *. r, cos (b *. (t -. r)) *. r)
};

let hyperbolic (a, b) (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  (sin (a *. t) /. r, r *. cos (b *. t))
};

let diamond (a, b, c, d) (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  (sin (a *. t) *. cos (b *. r), cos (c *. t) *. sin (d *. r))
};

let ex (a, b) (x, y) => {
  let t = atan2 y x;
  let r = sqrt((x ** 2.) +. y ** 2.);
  let p0 = sin(a *. t +. r);
  let p1 = cos(b *. t -. r);
  (p0 ** 3. +. p1 ** 3., p0 ** 3. -. p1 ** 3.)
};

let waves (b, c, e, f) (x, y) => {
  (
    x +. b *. sin (y /. c ** 2.),
    y +. e *. sin (x /. f ** 2.)
  )
};

let coswaves (b, c, e, f) (x, y) => {
  (
    x +. b *. cos (y /. c ** 2.),
    y +. e *. cos (x /. f ** 2.)
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

let popcorn (a, b, c, d) (x, y) => {
  (
    x +. a *. sin (tan (b *. y)),
    y +. c *. sin (tan (d *. x))
  )
};

let tangent (a, b, c) (x, y) => {
  (sin (a *. x) /. cos (b *. y), tan (c *. y))
};

let column (x, y) => {
  (sin x, y)
};

let row (x, y) => {
  (x, sin y)
};

let affine ((a, b, c), (d, e, f)) (x, y) => {
  (
    a *. x +. b *. y +. c,
    d *. x +. e *. y +. f,
  )
};

/** TODO this should be generatable via a macro. */
type attractor =
  | Sinusoidal p2
  | Spherical
  | Swirl p4
  | Horseshoe p4
  | Disc p2
  | Disc2 p2
  | Handkercheif p2
  | Hyperbolic p2
  | Diamond p4
  | Ex p2
  | Waves p4
  | CosWaves p4
  | Fisheye p1
  | Fisheye2 p1
  | Popcorn p4
  | Tangent p3
  | Column
  | Row
  | Affine (p3, p3)
;

let name attractor => switch attractor {
  | Sinusoidal p => "Sinusoidal"
  | Spherical => "Spherical"
  | Swirl p => "Swirl"
  | Horseshoe p => "Horseshow"
  | Disc p => "Disc"
  | Disc2 p => "Disc2"
  | Handkercheif p => "Handkerchief"
  | Hyperbolic p => "Hyperbolic"
  | Diamond p => "Diamond"
  | Ex p => "Ex"
  | Waves p => "Waves"
  | CosWaves p => "Waves"
  | Fisheye p => "Fisheye"
  | Fisheye2 p => "Fisheye2"
  | Popcorn p => "Popcorn"
  | Tangent p => "Tangent"
  | Column => "Column"
  | Row => "Row"
  | Affine (px, py) => "Affine"
};

let run attractor => switch (attractor) {
  | Sinusoidal p => sinusoidal p
  | Spherical => spherical
  | Swirl p => swirl p
  | Horseshoe p => horseshoe p
  | Disc p => disc p
  | Disc2 p => disc2 p
  | Handkercheif p => handkercheif p
  | Hyperbolic p => hyperbolic p
  | Diamond p => diamond p
  | Ex p => ex p
  | Waves p => waves p
  | CosWaves p => coswaves p
  | Fisheye p => fisheye p
  | Fisheye2 p => fisheye2 p
  | Popcorn p => popcorn p
  | Tangent p => tangent p
  | Column => column
  | Row => row
  | Affine (px, py) => affine (px, py)
};
