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