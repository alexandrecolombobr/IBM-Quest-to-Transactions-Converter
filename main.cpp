/*
** IBM Quest to transactions converter
** Copyright (C) 2019 Alexandre Colombo
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/

#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

int main(int argc, char *argv[])
{
    // Check the number of parameters
    if (argc < 2 || argc > 2) {
        // Telling the user how to run the program
        std::cerr << "IBM Quest to transactions converter" << std::endl
                  << "Usage: " << argv[0] << " INPUT-FILE-NAME" << std::endl;
        return 1;
    }

    std::string line; // Current file line
    std::ifstream inputFile(argv[1]); // Input file
    std::ofstream outputFile(std::string(argv[1]) + ".out"); // Output file

    // Trying to open output file
    if ( !outputFile.is_open() ) {
        std::cout << "Unable to open output file" << std::endl;
        return 5; // I/O error
    }

    // Trying to open input file
    if ( inputFile.is_open() )
    {
        std::vector<std::string> currentOutputLine; // Vector to serialize current transaction
        std::string stringLineIndex; // String that represents the current output line index

        // Read input file
        while ( getline (inputFile,line) )
        {
            // Split current line to separate output line index and content
            std::istringstream lineSplit(line);
            std::vector<std::string> results((std::istream_iterator<std::string>(lineSplit)), std::istream_iterator<std::string>());

            // Verify if current output line has changed
            if ( results.front().compare(stringLineIndex) != 0) {

                // If so, and we were iterating over a previous valid line, let's write it to the output file
                if ( !currentOutputLine.empty() ) {

                    // Using auxiliar string to avoid comparisons inside the loop
                    std::string auxString;

                    // Loop to prepare the output file current line
                    for (auto i = currentOutputLine.begin(); i!=currentOutputLine.end(); ++i) {
                        auxString.append(*i);
                        auxString.push_back(' ');
                    }
                    // At the end of the line we wish having a newline instead of an extra space
                    auxString.pop_back();
                    auxString.push_back('\n');

                    // Write the current line to output file
                    outputFile << auxString;

                    // Clear current line vector
                    currentOutputLine.clear();
                }

                // After saving content to file, update the current output line index
                stringLineIndex = results.front();
            }

            // Add content to vector
            currentOutputLine.push_back(results.back());
        }

        // Write the last line to output file
        std::string auxString;
        for (auto i = currentOutputLine.begin(); i!=currentOutputLine.end(); ++i) {
            auxString.append(*i);
            auxString.push_back(' ');
        }
        auxString.pop_back();
        auxString.push_back('\n');
        outputFile << auxString;
        currentOutputLine.clear();

        // Close files
        inputFile.close();
        outputFile.close();
    }
    // Error trying to open input file
    else {
        // Close output file
        outputFile.close();

        // Print error
        std::cout << "Unable to open input file" << std::endl;
        return 5; // I/O error
    }

    return 0;
}
