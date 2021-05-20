#include "m_pd.h"

// define a new "class"
static t_class *helloworld_class;

// this is the dataspace of our new object
typedef struct _helloworld {
  t_object  x_obj;
} t_helloworld;

void helloworld_bang(t_helloworld* x)
{
  (void) x; // subpress not used warning
  post("Hello world !!");
}

// this is the "constructor" of the class
void *helloworld_new(void)
{
  // cast is necessary
  t_helloworld* x = (t_helloworld *)pd_new(helloworld_class);
  return (void *)x;
}

// function-space of the class
void helloworld_setup(void) {
  /* create a new class */
  helloworld_class = class_new(gensym("helloworld"),        // the object's name is "helloworld"
                               (t_newmethod)helloworld_new, // the object's constructor is "helloworld_new()"
                               0,                           // no special destructor
                               sizeof(t_helloworld),        // the size of the data-space
                               CLASS_DEFAULT,               // a normal pd object
                               0);                          // no creation arguments

  // attach function to bang message
  class_addbang(helloworld_class, helloworld_bang);
}
