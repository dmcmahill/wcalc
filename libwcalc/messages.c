/*
 * Copyright (c) 2021 Dan McMahill
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
 * This contains some convenience routines for some common alert messages.
 */

#include "alert.h"
#include "config.h"
#include "messages.h"

#define WC_BUG_MSG "If the input values seem reasonable then please submit a report at " WCALC_BUG_URL "\n"

void alert_bracket()
{
   alert("Could not bracket the solution.  Synthesis failed.\n"
         "This is typically caused by requesting a value that is not possible.\n"
         WC_BUG_MSG);
}

void alert_bug()
{
    alert( WC_BUG_MSG );
}

