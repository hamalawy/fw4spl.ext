/* ***** BEGIN LICENSE BLOCK *****
* FW4SPL - Copyright (C) IRCAD, 2009-2013.
* Distributed under the terms of the GNU Lesser General Public License (LGPL) as
* published by the Free Software Foundation.
* ****** END LICENSE BLOCK ****** */
 

#ifndef _CTRLCOMPUTECPR_CONFIG_HPP_
#define _CTRLCOMPUTECPR_CONFIG_HPP_

    #ifdef _WIN32

        #ifdef CTRLCOMPUTECPR_EXPORTS
            #define CTRLCOMPUTECPR_API __declspec(dllexport)
        #else
            #define CTRLCOMPUTECPR_API __declspec(dllimport)
        #endif

        #define CTRLCOMPUTECPR_CLASS_API

        #pragma warning(disable: 4290)

    #elif defined(__GNUC__) && (__GNUC__>=4) && defined(__USE_DYLIB_VISIBILITY__)

        #ifdef CTRLCOMPUTECPR_EXPORTS
            #define CTRLCOMPUTECPR_API __attribute__ ((visibility("default")))
            #define CTRLCOMPUTECPR_CLASS_API __attribute__ ((visibility("default")))
        #else
            #define CTRLCOMPUTECPR_API __attribute__ ((visibility("hidden")))
            #define CTRLCOMPUTECPR_CLASS_API __attribute__ ((visibility("hidden")))
        #endif

    #else

        #define CTRLCOMPUTECPR_API
        #define CTRLCOMPUTECPR_CLASS_API

    #endif

#endif //CTRLCOMPUTECPR_API

