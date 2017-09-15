/* open DrawUtils; */

let attractors = Library.([
  Affine ((0.5, 0., 0.5), (0., 0.5, 0.)),
  Affine ((0.5, 0., -0.5), (0., 0.5, 0.)),
  Affine ((0.5, 0., 0.), (0., 0.5, 0.5)),
  Affine ((0.5, 0., 0.), (0., 0.5, -0.5)),
  Affine ((0.5, 0., 0.), (0., 0.5, 0.)),
  Affine ((-1., 0., 0.), (0., 1., 0.)),
  Affine ((1., 0., 0.), (0., -1., 0.)),
  Column,
  Tangent (1., 1., 1.),
  Popcorn (
    0.04,
    5.69,
    0.04,
    2.28,
  ),
  Fisheye 1.,
  Fisheye2 1.,
  Waves (
    0.04,
    0.21,
    0.17,
    0.29,
  ),
  CosWaves (
    0.04,
    0.21,
    0.17,
    0.29,
  ),
  Ex (
    (-1.58),
    (-2.87),
  ), /** TODO want a way to include multiple "presets" */
  Diamond (2., 2., 1., 1.),
  Hyperbolic (1.87, 2.82),
  Handkercheif ((-2.54), 2.45),
  Disc (3.4, 4.94),
  Disc2 (2.74, 2.74),
  Horseshoe (1., 1., 1., 1.),
  Sinusoidal (2., 2.),
  Swirl (1., 1., 1., 1.),
  Swirl (0.1, 0.1, 0.1, 0.1),
  Swirl (-1., -1., -1., -1.),
  Swirl (-1., 1., -1., 1.),
  Swirl (-1., 0.5, 0.5, -1.),
  Swirl (-1., 2.0, 2.0, -1.),
  Swirl (-1., 1.0, 2.0, -1.),
  Spherical,
  FoldDown,
  FoldUp,
  FoldLeft,
  FoldRight,
]);

let initialState = {
  App.workspace: List.mapi (fun i attractor => ({
    Types.attractor,
    enabled: i == 0 || i == 22,
    weight: 1
  })) attractors
};



ReactDOMRe.renderToElementWithId <App initialState /> "index";
/**
System state:

paramedFunction =
| Waves (float, float, float, float)
| ..;

{
workspace: list (boolean, paramedFunction)
library: list (paramedFunction, showGrid, showLines)
libraryModal: option int /* index */
}
-> hmm in the library, I want to be able to modify them...

 */

