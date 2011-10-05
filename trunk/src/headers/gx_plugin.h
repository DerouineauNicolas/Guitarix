/*
 * Copyright (C) 2011 Hermann Meyer, James Warden, Andreas Degert, Pete Shorthose
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
 *
 *    This file is part of the guitarix GUI main class
 *    Note: this header file depends on gx_system.h 
 * 
 * ----------------------------------------------------------------------------
 */

#pragma once

#ifndef _GX_PLUGIN_H
#define _GX_PLUGIN_H

// forward declarations (need not be resolved for plugin definition)
namespace gx_gui { class GxMainInterface; }
class PluginDef;

/*
** helper class for PluginDef::load_ui
*/

class UiBuilder {
private:
    gx_gui::GxMainInterface *intf;
public:
    PluginDef *plugin;
public:
    UiBuilder(gx_gui::GxMainInterface *i, PluginDef* p): intf(i), plugin(p) {}
    void openVerticalBox(const char* label = "") const;
    void openHorizontalBox(const char* label = "") const;
    void create_small_rackknob(const char *id) const;
    void create_small_rackknob(const char *id, const char *label) const;
    void closeBox() const;
    //FIXME add missing functions
};

/*
** helper class for PluginDef::registerfunc
*/

struct value_pair {
    const char *value_id;
    const char *value_label;
};

class ParamReg {
public:
    PluginDef *plugin;
    float *registerVar(const char* id, const char* name, const char* tp,
		       const char* tooltip, float* var, float val = 0,
		       float low = 0, float up = 0, float step = 0,
		       bool exp = false) const;
    void registerEnumVar(const char *id, const char* name, const char* tp,
			 const char* tooltip, const value_pair* values, float *var, float val,
			 float low = 0, float up = 0, float step = 1,
			 bool exp = false) const;
    void registerUEnumVar(const char *id, const char* name, const char* tp,
			  const char* tooltip, const value_pair* values,
			  unsigned int *var, unsigned int std = 0,
			  bool exp = false) const;
public:
    ParamReg(PluginDef *p): plugin(p) {}
};

/*
** structure for plugin definition
*/

typedef void (*inifunc)(int samplingFreq, PluginDef *plugin);
typedef int (*activatefunc)(bool start, PluginDef *plugin);
typedef void (*clearstatefunc)(PluginDef *plugin);
// in-place would be a tad more efficient but some plugins have to be cleaned up before
//typedef void (*process_mono_audio) (int count, float *buffer);
//typedef void (*process_stereo_audio) (int count, float *buffer1, float *buffer2);
typedef void (*process_mono_audio) (int count, float *input, float *output);
typedef void (*process_stereo_audio) (int count, float *input1, float *input2,
				      float *output1, float *output2);
typedef int (*registerfunc)(const ParamReg& reg);
typedef int (*uiloader)(const UiBuilder& builder);

enum {
    PGN_STEREO      = 0x01, // stereo plugin (auto set when stereo_audio)
    PGN_PRE         = 0x02, // (mono) always "pre" position
    PGN_POST        = 0x04, // (mono) always "post" position
    PGN_GUI         = 0x08, // register variables for gui (auto set when load_ui)
    PGN_POST_PRE    = 0x10, // (mono) register post/pre variable (auto set when
				// gui and not always pre or post)
    PGN_ALTERNATIVE = 0x20, // plugin is part of a group of modules
				// managed by a ModuleSelector
    // for additional flags see struct Plugin
};

#define PLUGINDEF_VERMAJOR_MASK 0xff00
#define PLUGINDEF_VERSION       0x0100

struct PluginDef {
    int version;	 // = PLUGINDEF_VERSION
    int flags;		 // PGN_xx flags

    const char* id;	 // must be unique
    const char* name;	 // displayed name (not translated) (may be 0)
    const char** groups; // list of alternating group_id, group_name (not translated)
				// may be 0 (else end with 0 entry)

    // maximal one of mono_audio, stereo_audio must be set
    // all function pointers in PluginDef can be independently set to 0
    process_mono_audio mono_audio; // function for mono audio processing
    process_stereo_audio stereo_audio; //function for stereo audio processing

    inifunc set_samplerate; // called before audio processing and when rate changes
    activatefunc activate_plugin; // called when taking in / out of pressing chain
    registerfunc register_params; // called once after module loading (register parameter ids)
    uiloader load_ui; // called once after module loading (define user interface)
    clearstatefunc clear_state;	// clear internal audio state; may be called
				// before calling the process function
};

// shared libraries with plugin modules must define
// a function "get_gx_plugins" of type plugin_inifunc
// return != 0 for error
extern "C" typedef int (*plugin_inifunc)(int *count, PluginDef **p);

#endif /* !_GX_PLUGIN_H */