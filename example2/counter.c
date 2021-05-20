#include "m_pd.h"

// define a new "class"
static t_class *counter_class;

// this is the dataspace of our new object; the first (mandatory) "t_object"
typedef struct _counter {
  t_object x_obj;
  t_float  i_count;
} t_counter;

void counter_bang(t_counter* x)
{
  outlet_float(x->x_obj.ob_outlet, x->i_count++);
}

// "constructor" of the class
// we have one argument of type floating-point (as specified below in the counter_setup() routine)
void* counter_new(t_floatarg f)
{
  post("Constructor: %f", f);
  t_counter* x = (t_counter *)pd_new(counter_class);
  x->i_count=f;
  // create a new outlet for floating-point values
  outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}

// function-space of the class
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
void counter_setup(void) {
  counter_class = class_new(gensym("counter"),
                            counter_new,         // this is the culprit; casting does not help either
                            0,
                            sizeof(t_counter),
                            CLASS_DEFAULT,
                            A_DEFFLOAT,          // the object takes one argument which is a floating-point and defaults to 0
                            0);

  // call a function when object gets banged
  class_addbang(counter_class, counter_bang);
}
#pragma GCC diagnostic pop
