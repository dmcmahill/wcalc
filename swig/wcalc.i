/*
 * Copyright (C) 2012, 2020 Dan McMahill
 * All rights reserved.
 *
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

/*
 * This file is the interface definition file for swig.  Swig is a wrapper generator
 * which allows the easy creation of modules for various script languages such
 * as perl, python, and ruby.  See www.swig.org for more details.
 */

%module wcalc
%{

#include "config.h"

#include "air_coil.h"
#include "bars.h"
#include "coax.h"
#include "coplanar.h"
#include "coupled_microstrip.h"
#include "coupled_stripline.h"
#include "ic_microstrip.h"
#include "microstrip.h"
#include "parallel_rc.h"
#include "parallel_rl.h"
#include "rods.h"
#include "stripline.h"

%}



%include "air_coil.h"
%include "bars.h"
%include "coax.h"
%include "coplanar.h"
%include "coupled_microstrip.h"
%include "coupled_stripline.h"
%include "ic_microstrip.h"
%include "microstrip.h"
%include "parallel_rc.h"
%include "parallel_rl.h"
%include "rods.h"
%include "stripline.h"

