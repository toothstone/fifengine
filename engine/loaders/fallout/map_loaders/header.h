/***************************************************************************
 *   Copyright (C) 2005-2007 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/

#ifndef FIFE_MAP_LOADERS_FALLOUT_HEADER_H
#define FIFE_MAP_LOADERS_FALLOUT_HEADER_H

// Standard C++ library includes
#include <string>

// Platform specific includes
#include "util/fife_stdint.h"

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "vfs/vfs.h"

namespace FIFE { namespace map { namespace loaders { namespace fallout {

	class RawData;
	/** Helper class for Fallout Map loading
	 * Reads in a map file in the constructor and then provides
	 * usefull Information about a Fallout map (file).
	 */
	class Header {
		public:
			/** Constructor
			 * 
			 * @param data The map file
			 */
			Header(RawDataPtr data);

			/** Read in a map file and sets state accordingly
			 */
			void loadHeader(RawDataPtr data);

			/** Get the map name
			 */
			const std::string& getMapName() const;

			/** Get the fallout map format file version
			 */
			uint32_t getVersion() const;

			/** Get @b which elevations are contained.
			 * @return A bitfield: 0x02 = Elevation 0, 0x04 = Elevation 1, 
			 * 0x08 = Elevation 2
			 *
			 */
			uint32_t getElevations() const;		// Anchorite: For proper map loading

			/** Return the number of @b local scripting variables.
			 */
			uint32_t getNumLocalVars() const;

			/** Return the number of @b global scripting variables.
			 */
			uint32_t getNumGlobalVars() const;

			/** Check whether an elevation is contained.
			 */
			bool hasElevation(unsigned int num) const;

			/** Get total number of elevations
			 */
			unsigned int getNumElevations() const;

			/** Get default elevation
			 */
			int32_t getDefaultElevation() const;

			/** Get default player position
			 */
			int32_t getDefaultPlayerPosition() const;

		private:
			std::string m_mapname;

			uint32_t m_version;
			int32_t m_defaultPlayerPos;
			int32_t m_defaultElevation;
			int32_t m_defaultOrientation;
			uint32_t m_num_localvars;
			uint32_t m_scriptid;
			uint32_t m_elevations;
			uint32_t m_num_globalvars;
			uint32_t m_mapID;
			uint32_t m_timestamp;

			void checkSanity();
	};

} } } }

#endif