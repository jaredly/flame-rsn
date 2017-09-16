
type request = {
  id: string,
  attractors: list (int, Library.transform),
  size: int,
  iterations: int,
  transform: option ((float, float, float), (float, float, float)),
};

type message =
  | Render request
  | Stop string
  ;

type workerMessage =
  /* | Rendered string (array int, int) */
  | Blit string MyDom.imagedata int [@@noserialize];
