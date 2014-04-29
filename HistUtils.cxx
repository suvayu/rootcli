#include <iostream>

// for POSIX regex
#include <sys/types.h>
#include <regex.h>

#include <boost/regex.hpp>

#include "FileUtils.hxx"
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
    auto regex_match_obj_p =
      [hregex](TKey *key, std::vector<TObject*> &histos) {
      std::string keyname(key->GetName());

      try {
	if (boost::regex_match(keyname, boost::regex(hregex))) {
	  // get histogram if there is a match
	  histos.push_back(key->ReadObj());
	}
      } catch (boost::regex_error &exc) {
	std::cerr << "Bad regex: " << hregex << std::endl
	<< exc.what() << std::endl;
      }
    };

    File::recurse_thru_dir(dir, fclass, regex_match_obj_p, histos);
    return;
  }


  void HistReader::_getHistVec_posix(std::string hregex,
				     std::vector<TObject*> &histos,
				     TDirectory *dir, TClass *fclass)
  {
    auto regex_match_obj_p =
      [hregex](TKey *key, std::vector<TObject*> &histos) {
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
	    // get histogram if there is a match
	    histos.push_back(key->ReadObj());
	  }
	}
      };

    File::recurse_thru_dir(dir, fclass, regex_match_obj_p, histos);
    return;
  }
}
