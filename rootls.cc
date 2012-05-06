#include <iostream>
#include <vector>
#include <string>

#include <boost/foreach.hpp>

#include <TFile.h>
#include <TCollection.h>
#include <TKey.h>
#include <TClass.h>


/**
 * Browse and list directories. If recurse is true, list recursively.
 *
 * @param dir Directory to list
 * @param recurse Whether to recursively list directories
 */
void browse_dir(TDirectory *dir, bool recurse);


/** 
 * Take ROOT files and input
 *
 * @param argc Argument count
 * @param argv Options and ROOT filenames
 *
 * @return Exit status
 */
int main(int argc, char * argv[])
{
  if (argc == 1) {
    std::cout << "Insufficient/incorrect number of arguments."
	      << std::endl << std::endl;
    std::cout << "Usage: " << argv[0] << " <ROOT file(s)>"
	      << std::endl << std::endl;
    std::cout << "   List contents of the ROOT file(s) recursively."
	      << std::endl;
    return 1;
  }

  // get args
  std::vector<std::string> arguments;
  for (int i = 1; i < argc; ++i) {
    arguments.push_back(argv[i]);
  }

  // get opts
  bool recurse(false);
  for (std::vector<std::string>::iterator argItr = arguments.begin();
       argItr != arguments.end(); ++argItr) {
    if (*argItr == "-r") {
      recurse = true;
      arguments.erase(argItr);
      break;
    }
  }

  // get filenames
  std::vector<std::string> files;
  BOOST_FOREACH(std::string argv, arguments) {
    files.push_back(argv);
  }

  if (files.empty()) {
    std::cout << "ERROR: No ROOT files provided! " << std::endl;
    return 2;
  }

  // list files
  BOOST_FOREACH(std::string filename, files) {
    TFile *rfile = new TFile(filename.c_str(), "read");
    browse_dir(rfile, recurse);
    delete rfile;
  }
  return 0;
}


void browse_dir(TDirectory * dir, bool recurse)
{
  // list directory contents
  dir->ls();
  std::cout << std::endl;

  // check for subdirectories
  if (recurse) {
    TIter keylist(dir->GetListOfKeys());
    TKey *key;
    while ((key = dynamic_cast<TKey*>(keylist()))) {
      TClass *objClass = TClass::GetClass(key->GetClassName());
      if (objClass->InheritsFrom(TDirectory::Class())) {
	browse_dir(static_cast<TDirectory*>
		   (key->ReadObjectAny(TDirectory::Class())), recurse);
      }
    }
  }
}
