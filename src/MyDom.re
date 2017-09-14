type imageElement;

type canvasRenderingContext2D;

type ctx = canvasRenderingContext2D;

type canvasElement;

type document;

type element;

type window;

type event_like 'a;

type keyboardEvent;

external document : document = "" [@@bs.val];

external window : window = "" [@@bs.val];

external getBody : document => element = "body" [@@bs.get];

external appendCanvas : element => canvasElement => unit = "appendChild" [@@bs.send];

external createCanvasElement : document => _ [@bs.as "canvas"] => canvasElement =
  "createElement" [@@bs.send];

external getContext : canvasElement => _ [@bs.as "2d"] => canvasRenderingContext2D =
  "getContext" [@@bs.send];

external createImg : document => _ [@bs.as "img"] => imageElement = "createElement" [@@bs.send];

let addCanvasToBody doc canvas => appendCanvas (getBody doc) canvas;

external setTimeout : (unit => unit) => int => int = "" [@@bs.val];

external clearTimeout : int => unit = "" [@@bs.val];

external requestAnimationFrame : (float => unit) => int = "" [@@bs.val];

external cancelAnimationFrame : int => unit = "" [@@bs.val];

external getElementById : document => string => option element =
  "" [@@bs.return null_to_opt] [@@bs.send];

external addEventListener :
  document => string => (event_like 'a => Js.boolean) => Js.boolean => unit =
  "" [@@bs.send];

external addEventListenerImg :
  imageElement => string => (event_like 'a => Js.boolean) => Js.boolean => unit =
  "addEventListener" [@@bs.send]; /* unsafe casts */

/* canvas api methods */
module Canvas = {
  external setFillStyle : ctx => string => unit = "fillStyle" [@@bs.set];
  external setStrokeStyle : ctx => string => unit = "strokeStyle" [@@bs.set];
  external setStrokeWidth : ctx => float => unit = "lineWidth" [@@bs.set];
  external fillRect : ctx => float => float => float => float => unit = "" [@@bs.send];
  external strokeRect : ctx => float => float => float => float => unit = "" [@@bs.send];
  external clearRect : ctx => float => float => float => float => unit = "" [@@bs.send];
  external ellipse : ctx => float => float => float => float => float => float => float => unit =
    "" [@@bs.send];
  external moveTo : ctx => float => float => unit = "" [@@bs.send];
  external scale : ctx => float => float => unit = "" [@@bs.send];
  external lineTo : ctx => float => float => unit = "" [@@bs.send];
  external fill : ctx => unit = "" [@@bs.send];
  external beginPath : ctx => unit = "" [@@bs.send];
  external stroke : ctx => unit = "" [@@bs.send];
  let line ctx (x, y) (a, b) => {
    beginPath ctx;
    moveTo ctx x y;
    lineTo ctx a b;
    stroke ctx;
  };
  let polyline ctx pts => {
    beginPath ctx;
    switch pts {
    | [(a, b), ...rest] => {
      moveTo ctx a b;
      List.iter (fun (a, b) => lineTo ctx a b) rest;
      stroke ctx;
    }
    | _ => assert false
    }
  }
};

/* unsafe casts */
external imageElementToJsObj : imageElement => Js.t {..} = "%identity";

external canvasRenderingContext2DToJsObj : canvasRenderingContext2D => Js.t {..} = "%identity";

external canvasElementToJsObj : canvasElement => Js.t {..} = "%identity";

external domElToCanvas : Dom.element => canvasElement = "%identity";

external keyboardEventToJsObj : keyboardEvent => Js.t {..} = "%identity";

external elementToCanvasElement : element => canvasElement = "%identity";

external windowToJsObj : window => Js.t {..} = "%identity";

let createBodyDiv: unit => element = [%bs.raw{|
  function() {
    const div = document.createElement('div')
    document.body.appendChild(div)
    return div
  }
|}];

let createCanvas (width: float) (height: float) => {
  let elem = createCanvasElement document;
  let jscanv = canvasElementToJsObj elem;
  jscanv##width#=width;
  jscanv##height#=height;
  elem
};

let createCtx container width height => {
  let canv = createCanvas width height;
  appendCanvas container canv;
  getContext canv
};