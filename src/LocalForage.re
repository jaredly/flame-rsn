
type t;
external localForage: t = "localforage" [@@bs.val][@@bs.module];

type err = Js.Null.t Js.Exn.t [@@noserialize];

external setJson: t => string => Js.Json.t => (err => Js.Json.t => unit) => unit = "setItem" [@@bs.send];
external setBlob: t => string => MyDom.blob => (err => MyDom.blob => unit) => unit = "setItem" [@@bs.send];
external getJson: t => string => (err => Js.Null.t Js.Json.t => unit) => unit = "getItem" [@@bs.send];
external getBlob: t => string => (err => Js.Null.t MyDom.blob => unit) => unit = "getItem" [@@bs.send];

let setJson = setJson localForage;
let setBlob = setBlob localForage;
let getJson = getJson localForage;
let getBlob = getBlob localForage;
