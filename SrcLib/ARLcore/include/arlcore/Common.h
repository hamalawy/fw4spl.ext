/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2012.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _ARLCORE_COMMMON_H
#define _ARLCORE_COMMMON_H

//#include <float.h>
#include <boost/cstdint.hpp>
#include <fwTools/Object.hpp>
#include <fwCore/macros.hpp>
#include <ARLcore/config.hpp>

//#include <arlcore/Type.h>

    /**
     * @date    2007
     * @brief   Directives de compilation
     * Definition de constantes
     */
    #ifndef _WIN32
        #ifndef CLK_TCK
            #define CLK_TCK 1000.0
        #endif
    #endif //_WIN32

    // COMPILATION OPTIONS
    #define ANN // Lib=ANN.lib
    //#undef ANN

//  #define ARL_VERBOSE ARL_LOG_LEVEL_NONE
    #define TMP_FILE "tmp.plt" // "c:/tmp/tmp.txt" // "/tmp/tmp.txt"
    #define TMP_DIR "./" //"c:/tmp/" // "/tmp/"
    #ifdef _WIN32
    //  #define GNUPLOT_EXE "d:/gnuplot/bin/pgnuplot <tmp.txt"
        //#define GNUPLOT_EXE "d:/gnuplot/bin/pgnuplot <tmp.txt"
        #define GNUPLOT_EXE "d:/gnuplot/bin/wgnuplot tmp.plt"
        //#define GNUPLOT_EXE "d:/gnuplot/bin/wgnuplot c:/tmp/tmp.txt"
    #else // _WIN32
        #define GNUPLOT_EXE "/usr/bin/gnuplot /tmp/tmp.plt"
    #endif // _WIN32
    #define GRAPHVIZ_EXE "\"c:/Program Files/ATT/Graphviz/bin/dotty.exe\""
    #define GRAPHVIZ_TMP "tmpGraph.dot"

typedef struct
{
    ::boost::uint32_t Low; //UINT32
    ::boost::uint32_t High; //UINT32
}uint32HL;



#endif // _ARLCORE_COMMMON_H
