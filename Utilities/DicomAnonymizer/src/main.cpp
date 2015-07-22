/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <fwDicomIOExt/gdcm/helper/DicomAnonymizer.hpp>

/** \file main.cpp
 *********************
 * Software : DicomAnonymizer
 *********************
 * Allows to anonymize a Dicom folder
 * HELP  : DicomAnonymizer.exe --help
 * USE :   DicomAnonymizer.exe <options>
 * Allowed options:
 *   -h [ --help ]           produce help message
 *   -i [ --input ] arg      set the input folder
 *   -o [ --output ] arg     set the output folder
 */
int main(int argc, char** argv)
{
    // Declare the supported options.
    ::boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("input,i", ::boost::program_options::value< std::string >(), "set input folder")
        ("output,o", ::boost::program_options::value< std::string >(), "set output folder")
    ;

    // Manage the options
    ::boost::program_options::variables_map vm;
    ::boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    ::boost::program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return EXIT_SUCCESS;
    }
    else if(!vm.count("input"))
    {
        std::cout << "You must specify an input file." << "\n";
        return EXIT_FAILURE;
    }
    else if(!vm.count("output"))
    {
        std::cout << "You must specify an output file." << "\n";
        return EXIT_FAILURE;
    }
    else if(vm["input"].as< std::string >() == vm["output"].as< std::string >())
    {
        std::cout << "The output folder can not be the input folder." << "\n";
        return EXIT_FAILURE;
    }

    // Get paths
    ::boost::filesystem::path input(vm["input"].as< std::string >());
    ::boost::filesystem::path output(vm["output"].as< std::string >());

    if(!::boost::filesystem::exists(input) || !::boost::filesystem::is_directory(input))
    {
        std::cout << "The specified input folder " << input << " is not a directory." << "\n";
        return EXIT_FAILURE;
    }
    else if(::boost::filesystem::exists(output))
    {
        std::cout << "The specified output folder " << output << " already exists." << "\n";
        return EXIT_FAILURE;
    }

    // Copy and anonymize
    ::fwDicomIOExt::gdcm::helper::DicomAnonymizer::copyDirectory(input, output);
    ::fwDicomIOExt::gdcm::helper::DicomAnonymizer anonymizer;
    anonymizer.anonymize(output);

    return EXIT_SUCCESS;
}

