#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "png_toolkit.h"
#include "filter.h"
#include "parseconfig.h"

int main( int argc, char *argv[] )
{
    // toolkit filter_name base_pic_name sudent_tool student_pic_name limitPix limitMSE
    // toolkit near test images!
    try
    {
        if (argc != 4)
            throw "Not enough arguments";

        png_toolkit studTool;
		ParseConfig parse;
		vector<configData> data;
		data = parse.readConfig(argv[1]);
        studTool.load(argv[2]);
		image_data imgData = studTool.getPixelData();
		Filters filters;
		for (auto elem : data)
			filters.doFilter(elem, imgData);

		
        studTool.save(argv[3]);

    }
    catch (const char *str)
    {
        std::cout << "Error: " << str << std::endl;
        return 1;
    }

    return 0;
}
