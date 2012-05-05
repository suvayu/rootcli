#include <iostream>
#include <vector>
#include <string>

#include <boost/foreach.hpp>

#include <TFile.h>

int main(int argc, char * argv[])
{
  if (argc == 1) {
    std::cout << "Insufficient/incorrect number of arguments."
	      << std::endl << std::endl;
    std::cout << "Usage: " << argv[0] << " <ROOT file(s)>"
	      << std::endl << std::endl;
    std::cout << "   List contents of the ROOT file."
	      << std::endl;
    return 1;
  }

  std::vector<std::string> files;
  for (int i = 1; i < argc; ++i) {
    files.push_back(argv[i]);
  }

  BOOST_FOREACH(std::string filename, files) {
    TFile rfile(filename.c_str(), "read");
    rfile.ls();
    std::cout << std::endl;
  }
  return 0;
}
