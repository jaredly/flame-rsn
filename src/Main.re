
let attractors = Library.([
  affine enabled::true "Right" ((0.5, 0., 0.5), (0., 0.5, 0.)),
  affine enabled::true "Left" ((0.5, 0., -0.5), (0., 0.5, 0.)),
  affine enabled::true "Down" ((0.5, 0., 0.), (0., 0.5, 0.5)),
  affine "Up" ((0.5, 0., 0.), (0., 0.5, -0.5)),
  affine "Center" ((0.5, 0., 0.), (0., 0.5, 0.)),
  affine "Reflect X" ((-1., 0., 0.), (0., 1., 0.)),
  affine "Reflect Y" ((1., 0., 0.), (0., -1., 0.)),
  item "Column" Column,
  item "Vase" @@ Tangent (1., 1., 1.),
  item "Fuzzy" @@ Popcorn (
    0.04,
    5.69,
    0.04,
    2.28,
  ),
  item "Fisheye" @@ Fisheye 1.,
  item "Fisheye2" @@ Fisheye2 1.,
  item "Waves" @@ Waves (
    0.04,
    0.21,
    0.17,
    0.29,
  ),
  item "Waves2" @@ CosWaves (
    0.04,
    0.21,
    0.17,
    0.29,
  ),
  item "Ex" @@ Ex (
    (-1.58),
    (-2.87),
  ), /** TODO want a way to include multiple "presets" */
  item "Genie" @@ Diamond (2., 2., 1., 1.),
  item "Explosion" @@ Hyperbolic (1.87, 2.82),
  item "Folded" @@ Handkercheif ((-2.54), 2.45),
  /* Disc (3.4, 4.94), */
  item "Spin" @@ Disc2 (2.74, 2.74),
  item "Around" @@ Horseshoe (1., 1., 1., 1.),
  item "Box" @@ Sinusoidal (2., 2.),
  item "Swirl Clockwise" @@ Swirl (1., 1., 1., 1.),
  item enabled::true "Tilt" @@ Swirl (0.1, 0.1, 0.1, 0.1),
  item "Swirl Anticlockwise" @@ Swirl (-1., -1., -1., -1.),
  item "Rolling pin" @@ Swirl (-1., 1., -1., 1.),
  item "Tight swirl" @@ Swirl (-1., 0.5, 0.5, -1.),
  item "Compass" @@ Swirl (-1., 2.0, 2.0, -1.),
  item "Discus" @@ Swirl (-1., 1.0, 2.0, -1.),
  item "Inside out" Spherical,
  item "Fold up" FoldDown,
  item "Fold down" FoldUp,
  item "Fold left" FoldLeft,
  item "Fold right" FoldRight,
]);

let initialState = App.{
  workspace: attractors
};

ReactDOMRe.renderToElementWithId <App initialState /> "index";

