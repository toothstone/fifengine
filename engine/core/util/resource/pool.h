/***************************************************************************
 *   Copyright (C) 2005-2008 by the FIFE team                              *
 *   http://www.fifengine.de                                               *
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

#ifndef FIFE_POOL_H
#define FIFE_POOL_H

// Standard C++ library includes
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <cassert>

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "resource.h"
#include "resource_location.h"

namespace FIFE {

	class IResource;

	/**  Clients of pool get notifications about pool events through this interface
	 */
	class IPoolListener {
	public:
		virtual void poolCleared() = 0;
		virtual ~IPoolListener() {};
	};

	enum { RES_LOADED = 0x01, RES_NON_LOADED  = 0x02};

	/**  Pool is used to optimize memory usage for resources
	 *
	 * Pool guarantees that there is minimal amount of resources
	 *   used in cases when it is would possible that multiple 
	 *   instances of the same data would be loaded into the memory.
	 *   Pool is the owner for resources taking care of their deletion.
	 */
	class Pool {
	public:
		/** Indicates invalid index for pool
		 */
		static const int INVALID_ID = -1;

		/** Default constructor.
		 */
		Pool();

		/** Destructor.
		 */
		virtual ~Pool();

		/** Adds new resource provider. Transfers provider ownership to the pool
		 */
		virtual void addResourceLoader(IResourceLoader* loader);

		/** Adds new resource into the pool using the given location.
		 * @return The index of the resource in the pool.
		 */
		virtual int addResourceFromLocation(const ResourceLocation& loc);
		
		/** This is a convenience version of addResourceFromLocation().
		 * It converts the filename into a ResourceLocation and then
		 * calls addResourceFromLocation.
		 *
		 * @param filename The file to be loaded.
		 * @return The index of the resource in the pool.
		 */
		virtual int addResourceFromFile(const std::string& filename);

		/** Gets resource from pool with given index
		 * 
		 * @param inc Specifies weither this call will increase the ref counter
		 */
		virtual IResource& get(unsigned int index, bool inc = false);
		
		/** Gets resource index from pool with given filename
		 * The resource will be created if it is not in the pool
		 */
		virtual unsigned int getIndex(const std::string& filename);

		/** Removes the resource from pool if reference counter is null
		 * 
		 * @param dec Specifies weither the ref counter will be decreased
		 * before checking
		 */
		virtual void release(unsigned int index, bool dec = false);

		/** Gets amount of resources in the pool with given status
		 */
		virtual int getResourceCount(int status);

		/** Clears pool from resources. Frees associated memory 
		 */
		virtual void clear();

		/** Adds pool listener.
		 * Pool listeners get indications e.g. when ownerships of pooled
		 * resources change.
		 */
		virtual void addPoolListener(IPoolListener* listener);

		/** Removes pool listener
		 */
		virtual void removePoolListener(IPoolListener* listener);

		/** Prints the cache statistics to the standard output
		 */
		virtual void printStatistics();

	protected:
	private:
		class PoolEntry {
		public:
			PoolEntry(): resource(0), location(0), loader(0) {}
			~PoolEntry() {
				delete location;
				delete resource;
			}

			// Pointer to the resource that is loaded.
			IResource* resource;
			// Location of the resource.
			ResourceLocation* location;
			// Resource loader.
			IResourceLoader* loader;
		};

		void findAndSetProvider(PoolEntry& entry);
		std::vector<PoolEntry*> m_entries;
		std::vector<IPoolListener*> m_listeners;
		std::vector<IResourceLoader*> m_loaders;
		int m_curind;
	};

} // FIFE

#endif
