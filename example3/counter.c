// creates an object that increments and outputs a counter
// whenever it gets banged.
// the counter value can be "set" to a special value, or "reset" to a default
// an upper and lower boundary can be specified: whenever the counter crosses
// such boundary a "bang" is emitted at the 2nd outlet and the counter value wraps

#include "m_pd.h"

static t_class* counter_class;

typedef struct _counter {
  t_object    x_obj;          // mandatory t_object 
  t_int       i_count;        // the current counter value 
  t_float     step;           // step size; this is "float" because of the passive inlet we are using
  t_int       i_down, i_up;   // lower and upper boundary
  t_outlet    *f_out, *b_out; // outlets
} t_counter;

void counter_bang(t_counter* x)
{
  t_float f   = x->i_count;
  t_int step  = x->step;
  x->i_count += step;

  if (x->i_down - x->i_up) {
    if ((step > 0) && (x->i_count > x->i_up)) {
      x->i_count = x->i_down;
      outlet_bang(x->b_out);
    } else if (x->i_count < x->i_down) {
      x->i_count = x->i_up;
      outlet_bang(x->b_out);
    }
  }
  outlet_float(x->f_out, f);
}

void counter_reset(t_counter *x)
{
  x->i_count = x->i_down;
}

void counter_set(t_counter *x, t_floatarg f)
{
  x->i_count = f;
}

// this is called whenever a "bound" message is sent to the inlet of the object
// note that in counter_new(), we rewrite a list to the 2nd inlet
// to a "bound" message to the 1st inlet
void counter_bound(t_counter *x, t_floatarg f1, t_floatarg f2)
{
  x->i_down = (f1 < f2) ? f1 : f2;
  x->i_up   = (f1 > f2) ? f1 : f2;
}

// we expect a variable number of arguments to this object
// symbol "s" is the name of the object itself
// the arguments are given as a t_atom array of argc elements.
void* counter_new(t_symbol* s, int argc, t_atom *argv)
{
  (void)s; // suppress unused warning
  t_counter* x = (t_counter *)pd_new(counter_class);
  t_float f1 = 0, f2 = 0;

  // depending on the number of arguments we interprete them differently
  x->step = 1;
  if(argc >= 3)
    x->step = atom_getfloat(argv+2);
  if(argc >= 2)
      f2 = atom_getfloat(argv+1);
  if(argc >= 1)
      f1 = atom_getfloat(argv);
  /* switch(argc){ */
  /*   default: */
  /*   case 3: */
  /*     x->step = atom_getfloat(argv+2); */
  /*   case 2: */
  /*     f2 = atom_getfloat(argv+1); */
  /*   case 1: */
  /*     f1 = atom_getfloat(argv); */
  /*     break; */
  /*   case 0: */
  /*     break; */
  /* } */
  if (argc < 2)
    f2 = f1;

  x->i_down  = (f1 < f2) ? f1 : f2;
  x->i_up    = (f1 > f2) ? f1 : f2;
  x->i_count = x->i_down;

  // create a new active inlet for this object
  // a message with the selector "list" that is sent
  // to this inlet (it is the 2nd inlet from left),
  // will be appear to be the same message but with the selector "bound"
  // at the 1st inlet.
  // the method for "bound" messages is given in counter_setup()
  inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("list"), gensym("bound"));

  // create a passive inlet (it will be the 2nd inlet from left)
  // whenever a floating point number is sent to this inlet,
  // its value will be immediately stored in "x->step"
  // no function will be called
  floatinlet_new(&x->x_obj, &x->step);

  // create a new outlet which will output floats
  // we store a reference to this outlet in x->f_out
  // so we are able to send data to this very outlet
  x->f_out = outlet_new(&x->x_obj, &s_float);
  // create a new outlet which will output bangs
  x->b_out = outlet_new(&x->x_obj, &s_bang);

  return (void *)x;
}

// define the function-space of the class
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
void counter_setup(void) {
  counter_class = class_new(gensym("counter"),
                            counter_new,
                            0,
                            sizeof(t_counter),
                            CLASS_DEFAULT,
                            A_GIMME,             // an arbitrary number of arguments which are of arbitrary type
                            0);

  // call a function when a "bang" message appears on the first inlet
  class_addbang  (counter_class, counter_bang);

  // call a function when a "reset" message (without arguments) appears on the first inlet
  class_addmethod(counter_class, (t_method)counter_reset, gensym("reset"), 0);

  // call a function when a "set" message with one float-argument (defaults to 0) appears on the first inlet
  class_addmethod(counter_class, (t_method)counter_set, gensym("set"), A_DEFFLOAT, 0);

  // call a function when a "bound" message with 2 float-argument (both default to 0)
  // appears on the first inlet
  // this is used for "list" messages which appear on the 2nd inlet
  // the magic is done in counter_new()
  class_addmethod(counter_class, (t_method)counter_bound, gensym("bound"), A_DEFFLOAT, A_DEFFLOAT, 0);

  // set the name of the help-patch to "help-counter"(.pd)
  class_sethelpsymbol(counter_class, gensym("help-counter"));
}
#pragma GCC diagnostic pop
