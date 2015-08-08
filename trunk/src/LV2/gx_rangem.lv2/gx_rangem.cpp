/*
 * Copyright (C) 2014 Guitarix project MOD project
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
#include "gx_rangem.h"        // define struct PortIndex
#include "gx_pluginlv2.h"   // define struct PluginLV2
#include "rangem.cc"    // dsp class generated by faust -> dsp2cc

////////////////////////////// PLUG-IN CLASS ///////////////////////////

namespace rangem {

class Gx_rangem_
{
private:
  // pointer to buffer
  float*      output;
  float*      input;
  // pointer to dsp class
  PluginLV2*  rangem;
  // private functions
  inline void run_dsp_(uint32_t n_samples);
  inline void connect_(uint32_t port,void* data);
  inline void init_dsp_(uint32_t rate);
  inline void connect_all__ports(uint32_t port, void* data);
  inline void activate_f();
  inline void clean_up();
  inline void deactivate_f();

public:
  // LV2 Descriptor
  static const LV2_Descriptor descriptor;
  // static wrapper to private functions
  static void deactivate(LV2_Handle instance);
  static void cleanup(LV2_Handle instance);
  static void run(LV2_Handle instance, uint32_t n_samples);
  static void activate(LV2_Handle instance);
  static void connect_port(LV2_Handle instance, uint32_t port, void* data);
  static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
                                double rate, const char* bundle_path,
                                const LV2_Feature* const* features);
  Gx_rangem_();
  ~Gx_rangem_();
};

// constructor
Gx_rangem_::Gx_rangem_() :
  output(NULL),
  input(NULL),
  rangem(rangem::plugin()) {};

// destructor
Gx_rangem_::~Gx_rangem_()
{
  // just to be sure the plug have given free the allocated mem
  // it didn't hurd if the mem is already given free by clean_up()
  if (rangem->activate_plugin !=0)
    rangem->activate_plugin(false, rangem);
  // delete DSP class
  rangem->delete_instance(rangem);
};

///////////////////////// PRIVATE CLASS  FUNCTIONS /////////////////////

void Gx_rangem_::init_dsp_(uint32_t rate)
{
  AVOIDDENORMALS(); // init the SSE denormal protection
  rangem->set_samplerate(rate, rangem); // init the DSP class
}

// connect the Ports used by the plug-in class
void Gx_rangem_::connect_(uint32_t port,void* data)
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

void Gx_rangem_::activate_f()
{
  // allocate the internal DSP mem
  if (rangem->activate_plugin !=0)
    rangem->activate_plugin(true, rangem);
}

void Gx_rangem_::clean_up()
{
  // delete the internal DSP mem
  if (rangem->activate_plugin !=0)
    rangem->activate_plugin(false, rangem);
}

void Gx_rangem_::deactivate_f()
{
  // delete the internal DSP mem
  if (rangem->activate_plugin !=0)
    rangem->activate_plugin(false, rangem);
}

void Gx_rangem_::run_dsp_(uint32_t n_samples)
{
  rangem->mono_audio(static_cast<int>(n_samples), input, output, rangem);
}

void Gx_rangem_::connect_all__ports(uint32_t port, void* data)
{
  // connect the Ports used by the plug-in class
  connect_(port,data); 
  // connect the Ports used by the DSP class
  rangem->connect_ports(port,  data, rangem);
}

////////////////////// STATIC CLASS  FUNCTIONS  ////////////////////////

LV2_Handle 
Gx_rangem_::instantiate(const LV2_Descriptor* descriptor,
                            double rate, const char* bundle_path,
                            const LV2_Feature* const* features)
{
  // init the plug-in class
  Gx_rangem_ *self = new Gx_rangem_();
  if (!self)
    {
      return NULL;
    }

  self->init_dsp_((uint32_t)rate);

  return (LV2_Handle)self;
}

void Gx_rangem_::connect_port(LV2_Handle instance, 
                                   uint32_t port, void* data)
{
  // connect all ports
  static_cast<Gx_rangem_*>(instance)->connect_all__ports(port, data);
}

void Gx_rangem_::activate(LV2_Handle instance)
{
  // allocate needed mem
  static_cast<Gx_rangem_*>(instance)->activate_f();
}

void Gx_rangem_::run(LV2_Handle instance, uint32_t n_samples)
{
  // run dsp
  static_cast<Gx_rangem_*>(instance)->run_dsp_(n_samples);
}

void Gx_rangem_::deactivate(LV2_Handle instance)
{
  // free allocated mem
  static_cast<Gx_rangem_*>(instance)->deactivate_f();
}

void Gx_rangem_::cleanup(LV2_Handle instance)
{
  // well, clean up after us
  Gx_rangem_* self = static_cast<Gx_rangem_*>(instance);
  self->clean_up();
  delete self;
}

const LV2_Descriptor Gx_rangem_::descriptor =
{
  GXPLUGIN_URI "#_rangem_",
  Gx_rangem_::instantiate,
  Gx_rangem_::connect_port,
  Gx_rangem_::activate,
  Gx_rangem_::run,
  Gx_rangem_::deactivate,
  Gx_rangem_::cleanup,
  NULL
};


} // end namespace rangem

////////////////////////// LV2 SYMBOL EXPORT ///////////////////////////

extern "C"
LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
  switch (index)
    {
    case 0:
      return &rangem::Gx_rangem_::descriptor;
    default:
      return NULL;
    }
}

///////////////////////////// FIN //////////////////////////////////////
