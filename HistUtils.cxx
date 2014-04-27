#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// for POSIX regex
#include <sys/types.h>
#include <regex.h>

#include <boost/regex.hpp>

#include <TKey.h>
#include <TCollection.h>

#include "HistUtils.hxx"


namespace Hist {

  void rescale(TH1 *hist, Double_t factor)
  {
    // change X scale
    Double_t xmin = hist->GetXaxis()->GetXmin();
    Double_t xmax = hist->GetXaxis()->GetXmax();
    hist->GetXaxis()->SetLimits(xmin*factor,xmax*factor);
    return;
  }


  ///////////////////////////////
  // HistReader implementation //
  ///////////////////////////////

  void HistReader::_getHistVec(std::string hregex,
			       std::vector<TObject*> &histos,
			       TDirectory *dir, TClass *fclass)
  {
    try {
      TIter keylist(dir->GetListOfKeys());
      TKey *key;

      while ((key = dynamic_cast<TKey*> (keylist()))) {
	TClass *objClass = TClass::GetClass(key->GetClassName());

	// recurse through subdirectories
	if (objClass->InheritsFrom(TDirectory::Class())) {
	  TDirectory *subdir = dynamic_cast<TDirectory*>(key->ReadObj());
	  _getHistVec(hregex, histos, subdir, fclass);
	}

	// get histogram if there is a match
	if (objClass->InheritsFrom(fclass)) {
	  std::string keyname(key->GetName());
	  if (boost::regex_match(keyname, boost::regex(hregex))) {
	    histos.push_back(key->ReadObj());
	  }
	}
      }
    } catch (boost::regex_error &exc) {
      std::cerr << "Bad regex: " << hregex << std::endl
		<< exc.what() << std::endl;
    }
    return;
  }


  void HistReader::_getHistVec_posix(std::string hregex,
				     std::vector<TObject*> &histos,
				     TDirectory *dir, TClass *fclass)
  {
    TIter keylist(dir->GetListOfKeys());
    TKey *key;

    while ((key = dynamic_cast<TKey*> (keylist()))) {
      TClass *objClass = TClass::GetClass(key->GetClassName());

      // recurse through subdirectories
      if (objClass->InheritsFrom(TDirectory::Class())) {
	TDirectory *subdir = dynamic_cast<TDirectory*>(key->ReadObj());
	_getHistVec(hregex, histos, subdir, fclass);
      }

      // get histogram if there is a match
      if (objClass->InheritsFrom(fclass)) {
	std::string keyname(key->GetName());

	int status(0);
	regex_t regex;
	if (0 != (status = regcomp(&regex, hregex.c_str(), REG_EXTENDED))) {
	  int length = regerror(status, &regex, NULL, size_t(0));
	  char * errmsg = new char[length];
	  regerror(status, &regex, errmsg, length);
	  std::cout << "Bad regex: " << errmsg << std::endl;
	  delete errmsg;
	} else {
	  status = regexec(&regex, keyname.c_str(), size_t(0), NULL, 0);
	  regfree(&regex);

	  if (not status) {
	    histos.push_back(key->ReadObj());
	  }
	}
      }
    }

    return;
  }
}
