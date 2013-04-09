/*
 * Copyright (C) 2012 Hermann Meyer, Andreas Degert, Pete Shorthose
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * --------------------------------------------------------------------------
 */


////////////////////////////// LOCAL INCLUDES //////////////////////////

#include "gx_common.h"      // faust support and denormal protection (SSE)
#include "gx_expander.h"        // define struct PortIndex
#include "gx_pluginlv2.h"   // define struct PluginLV2
#include "expander.cc"    // dsp class generated by faust -> dsp2cc

////////////////////////////// PLUG-IN CLASS ///////////////////////////

class Gx_expander
{
private:
  // internal stuff
  float*                       output;
  float*                       input;
  PluginLV2*                   expander_st;
public:

  inline void run_dsp_mono(uint32_t n_samples);
  inline void connect_mono(uint32_t port,void* data);
  inline void init_dsp_mono(uint32_t rate);
  inline void connect_all_mono_ports(uint32_t port, void* data);
  inline void activate_f();
  inline void clean_up();
  Gx_expander();
  ~Gx_expander();
};

// constructor
Gx_expander::Gx_expander() :
  output(NULL),
  input(NULL),
  expander_st(expander::plugin()) {};

// destructor
Gx_expander::~Gx_expander()
{
  // just to be sure the plug have given free the allocated mem
  // check if the function is valid
  // it didn't hurd if the mem is already given free by clean_up()
  if ( expander_st->activate_plugin !=0)
    expander_st->activate_plugin(false, expander_st);
  // delete DSP class
  expander_st->delete_instance(expander_st);
};

////////////////////////////// PLUG-IN CLASS  FUNCTIONS ////////////////

void Gx_expander::init_dsp_mono(uint32_t rate)
{
  AVOIDDENORMALS(); // init the SSE denormal protection
  expander_st->set_samplerate(rate, expander_st); // init the DSP class
}

// connect the Ports used by the plug-in class
void Gx_expander::connect_mono(uint32_t port,void* data)
{
  switch ((PortIndex)port)
    {
    case EFFECTS_OUTPUT:
      output = static_cast<float*>(data);
      break;
    case EFFECTS_INPUT:
      input = static_cast<float*>(data);
      break;
    default:
      break;
    }
}

void Gx_expander::activate_f()
{
  // allocate the internal DSP mem
  // check if the function is valid
  if (expander_st->activate_plugin !=0)
    expander_st->activate_plugin(true, expander_st);
}

void Gx_expander::clean_up()
{
  // delete the internal DSP mem
  // check if the function is valid
  if (expander_st->activate_plugin !=0)
    expander_st->activate_plugin(false, expander_st);
}

void Gx_expander::run_dsp_mono(uint32_t n_samples)
{
  expander_st->mono_audio(static_cast<int>(n_samples), input,
                        output, expander_st);
}

void Gx_expander::connect_all_mono_ports(uint32_t port, void* data)
{
  // connect the Ports used by the plug-in class
  connect_mono(port,data); 
  // connect the Ports used by the DSP class
  expander_st->connect_ports(port,  data, expander_st);
}

///////////////////////////// LV2 defines //////////////////////////////

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
  // init the plug-in class
  Gx_expander *self = new Gx_expander();
  if (!self)
    {
      return NULL;
    }

  self->init_dsp_mono((uint32_t)rate);

  return (LV2_Handle)self;
}

static void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
  // connect all ports
  static_cast<Gx_expander*>(instance)->connect_all_mono_ports(port, data);
}

static void
activate(LV2_Handle instance)
{
  // allocate needed mem
  static_cast<Gx_expander*>(instance)->activate_f();
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
  // run dsp
  static_cast<Gx_expander*>(instance)->run_dsp_mono(n_samples);
}

static void
cleanup(LV2_Handle instance)
{
  // well, clean up after us
  Gx_expander* self = static_cast<Gx_expander*>(instance);
  self->clean_up();
  delete self;
}

///////////////////////////// LV2 DESCRIPTOR ///////////////////////////

static const LV2_Descriptor descriptor =
{
  GXPLUGIN_URI "#_expander",
  instantiate,
  connect_port,
  activate,
  run,
  NULL,
  cleanup,
  NULL
};

extern "C"
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
  switch (index)
    {
    case 0:
      return &descriptor;
    default:
      return NULL;
    }
}

///////////////////////////// FIN //////////////////////////////////////