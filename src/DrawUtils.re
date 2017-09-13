let scale (x, y) offset scale off => ((x +. offset) *. scale +. off, (y +. offset) *. scale +. off);

let showGrid fn ctx w h by => {
  let hw = w /. 2.;
  let hh = h /. 2.;
  let byf = (float_of_int by) /. 2.;
  MyDom.Canvas.setStrokeStyle ctx "rgba(255, 100, 50, 0.5)";
  for xs in 0 to by {
    let pts = ref [];
    for ys in 0 to by {
      let x = (float_of_int xs) /. byf -. 1.;
      let y = (float_of_int ys) /. byf -. 1.;
      let (a, b) = fn (x, y);
      let a = (a +. 1.) *. hw +. hw;
      let b = (b +. 1.) *. hh +. hh;
      pts := [(a, b), ...!pts];
    };
    MyDom.Canvas.polyline ctx !pts;
  };
  for ys in 0 to by {
    let pts = ref [];
    for xs in 0 to by {
      let x = (float_of_int xs) /. byf -. 1.;
      let y = (float_of_int ys) /. byf -. 1.;
      let (a, b) = fn (x, y);
      /* let a = (a +. 1.) *. hw;
      let b = (b +. 1.) *. hh; */
      let a = (a +. 1.) *. hw +. hw;
      let b = (b +. 1.) *. hh +. hh;
      pts := [(a, b), ...!pts];
    };
    MyDom.Canvas.polyline ctx !pts;
  };
};

let showMovement fn ctx w h by => {
  let hw = w /. 2.;
  let hh = h /. 2.;
  let byf = (float_of_int by) /. 2.;
  MyDom.Canvas.setStrokeStyle ctx "rgba(0, 0, 0, 0.3)";
  for xs in 0 to by {
    for ys in 0 to by {
      let x = (float_of_int xs) /. byf -. 1.;
      let y = (float_of_int ys) /. byf -. 1.;
      let (a, b) = fn (x, y);
      MyDom.Canvas.line ctx (scale (x, y) 1. hw hw) (scale (a, b) 1. hh hh);
    }
  };
};

let slider: MyDom.element => float => float => float => float => (float => unit) => unit = [%bs.raw {|
  function(container, min, max, step, initial, done) {
    const num = document.createElement('div')
    const slider = document.createElement('input')
    slider.style.width = '400px';
    slider.type = 'range'
    slider.min = min
    slider.max = max
    slider.step = step
    slider.value = initial
    num.textContent = initial
    num.onclick = () => {
      slider.value = 0
      num.textContent = 0
      done(0)
    }
    slider.oninput = function(e) {
      num.textContent = slider.value
      done(slider.value)
    }
    container.appendChild(slider)
    container.appendChild(num)
    document.body.appendChild(container)
    done(initial)
  }
|}];

let slide2 a b c d node => slider node a b c d;
