
type either 'a 'b = Left 'a | Right 'b;

  type t 'a = ('a => unit) => unit;
  let return x fin => fin x;
  let map = fun work f::use fin => work (fun result => fin (use result));
  let bind = fun work f::use fin => work (fun result => (use result) fin);
  let consume = fun work f::use => work use;

  type side 'a 'b = One 'a | Two 'b | Neither | Done;
  let join2 = fun one two fin => {
    let side = ref Neither;
    one (fun one => switch (!side) {
      | Neither => side := One one
      | Two two => {
        side := Done;
        fin (one, two);
      }
      /* not allowed to call multiple times */
      | One _ | Done => ()
    });
    two (fun two => switch (!side) {
      | Neither => side := Two two
      | One one => {
        side := Done;
        fin (one, two);
      }
      /* not allowed to call multiple times */
      | Two _ | Done => ()
    });
  };
  let first = fun one two fin => {
    let finished = ref false;
    one (fun one => if (not !finished) {
      finished := true;
      fin (Left one);
    });
    two (fun two => if (not !finished) {
      finished := true;
      fin (Right two);
    });
  };
