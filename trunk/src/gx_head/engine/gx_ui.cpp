/*
 * Copyright (C) 2009, 2010 Hermann Meyer, James Warden, Andreas Degert
 * Copyright (C) 2011 Pete Shorthose
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
 *
 *
 *    GRAPHIC USER INTERFACE (v2)
 *    This is the Guitarix Engine GUI builder
 *
 *
 * --------------------------------------------------------------------------
 */

#include "engine.h"

namespace gx_ui {
list<GxUI*> GxUI::fGuiList;
bool GxUI::in_updateAll = false;

// constructor
GxUI::GxUI()
    : in_update(false), fZoneMap() {
    fGuiList.push_back(this);
}

GxUI::~GxUI() {
    fGuiList.remove(this);
    for (zmap::iterator i = fZoneMap.begin(); i != fZoneMap.end(); ++i) {
	delete i->second;
    }
}

// -- registerZone(z,c) : zone management
void GxUI::registerZone(void* z, GxUiItem* c) {
    if (fZoneMap.find(z) == fZoneMap.end()) {
	fZoneMap[z] = new clist();
    }
    fZoneMap[z]->push_back(c);
}

void GxUI::unregisterZone(void* z, GxUiItem* c) {
    zmap::iterator i = fZoneMap.find(z);
    if (i == fZoneMap.end()) {
	return;
    }
    i->second->remove(c);
}

void GxUI::updateAllGuis(bool force) {
    if (in_updateAll) {
	return;
    }
    in_updateAll = true;
    list<GxUI*>::iterator g;
    for (g = fGuiList.begin(); g != fGuiList.end(); ++g) {
        (*g)->updateAllZones(force);
    }
    in_updateAll = false;
}

// Update all user items not up to date
void GxUI::updateAllZones(bool force) {
    if (in_update) {
	return;
    }
    in_update = true;
    for (zmap::iterator m = fZoneMap.begin(); m != fZoneMap.end(); ++m) {
        clist*	l = m->second;
        for (clist::iterator c = l->begin(); c != l->end(); ++c) {
	    if (force || (*c)->hasChanged()) {
		(*c)->reflectZone();
	    }
        }
    }
    in_update = false;
}

/* ---------------- GxUiItem stuff --------------- */
GxUiItem::~GxUiItem() {
}

/* ----------------- GxUiCallbackItem stuff ---------------- */
GxUiCallbackItemFloat::GxUiCallbackItemFloat(GxUI* ui, float* zone, GxUiCallback foo, void* data)
    : GxUiItemFloat(ui, zone), fCallback(foo), fData(data) {}

// reflect zone
void GxUiCallbackItemFloat::reflectZone() {
    float v = *fZone;
    fCache  = v;
    fCallback(v, fData);
}

} /* end of guitarix namespace */
