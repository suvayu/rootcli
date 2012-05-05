#include <iostream>
#include <vector>
#include <string>

#include <boost/foreach.hpp>

#include <TFile.h>
#include <TCollection.h>
#include <TKey.h>
#include <TClass.h>


/** 
 * Recursively browse and list directories
 *
 * @param dir Directory to list
 */
void browse_dir(TDirectory *dir);


/** 
 * Take ROOT files and input
 *
 * @param argc Argument count
 * @param argv ROOT filenames
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

  std::vector<std::string> files;
  for (int i = 1; i < argc; ++i) {
    files.push_back(argv[i]);
  }

  BOOST_FOREACH(std::string filename, files) {
    TFile *rfile = new TFile(filename.c_str(), "read");
    browse_dir(rfile);
    delete rfile;
  }
  return 0;
}


void browse_dir(TDirectory * dir)
{
  // list directory contents
  dir->ls();
  std::cout << std::endl;

  // check for subdirectories
  TIter keylist(dir->GetListOfKeys());
  TKey *key;
  while ((key = dynamic_cast<TKey*>(keylist()))) {
    TClass *objClass = TClass::GetClass(key->GetClassName());
    if (objClass->InheritsFrom(TDirectory::Class())) {
      browse_dir(static_cast<TDirectory*>
		 (key->ReadObjectAny(TDirectory::Class())));
    }
  }
}
