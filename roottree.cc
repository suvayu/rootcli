#include <iostream>
#include <vector>
#include <string>

#include <boost/foreach.hpp>

#include <TFile.h>
#include <TTree.h>

#include <TCollection.h>
#include <TKey.h>
#include <TClass.h>


int get_trees(TDirectory *dir, std::vector<TTree*>& trees);

/** 
 * Start tree viewer for the given tree
 *
 * @param ftree TTree to view
 */
void view_tree(TTree *ftree);

void scan_tree(TTree *ftree);

void draw_tree(TTree *ftree);


/** 
 * Take ROOT files and options as input
 *
 * @param argc Argument count
 * @param argv options and ROOT filename
 *
 * @return Exit status
 */
int main(int argc, char * argv[])
{
  if (argc == 2 or argc > 4) {
    std::cout << "Insufficient/incorrect number of arguments."
	      << std::endl << std::endl;
    std::cout << "Usage: " << argv[0] << " <ROOT file>"
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
    std::vector<TTree*> trees;
    get_trees(rfile, trees);
    
    delete rfile;
  }
  return 0;
}


void get_trees(TDirectory * dir, std::vector<TTree*>& trees)
{
  // check for subdirectories
  TIter keylist(dir->GetListOfKeys());
  TKey *key;
  while ((key = dynamic_cast<TKey*>(keylist()))) {
    TClass *objClass = TClass::GetClass(key->GetClassName());
    if (objClass->InheritsFrom(TDirectory::Class())) {
      get_trees(static_cast<TDirectory*>
		(key->ReadObjectAny(TDirectory::Class())), trees);
    } else if (objClass->InheritsFrom(TTree::Class())) {
      trees.push_back(static_cast<TTree*>
		      (key->ReadObjectAny(TDirectory::Class())));
    }
  }
}
