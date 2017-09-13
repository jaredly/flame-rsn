
type t 'a = ('a => unit) => unit;
let return x fin => fin x;
let map = fun work f::use fin => work (fun result => fin (use result));
let bind = fun work f::use fin => work (fun result => (use result) fin);
let consume = fun work f::use => work use;

type side 'a 'b = One 'a | Two 'b | Both ('a, 'b) | Neither;
let join2 = fun one two fin => {
  let side = ref Neither;
  one (fun one => switch (!side) {
    | One _
    | Neither => side := One one
    | Both (_, two)
    | Two two => {
      side := Both (one, two);
      fin (one, two);
    }
  });
  two (fun two => switch (!side) {
    | Two _
    | Neither => side := Two two
    | Both (one, _)
    | One one => {
      side := Both (one, two);
      fin (one, two);
    }
  });
};

/* let first = fun one two fin => {
  let finished = ref false;
  one (fun one => if (not !finished) {
    finished := true;
    fin (Left one);
  });
  two (fun two => if (not !finished) {
    finished := true;
    fin (Right two);
  });
}; */