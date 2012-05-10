/**
 * @file   HistUtils.hxx
 * @author Suvayu Ali <Suvayu.Ali@cern.ch>
 * @date   Tue May  8 17:25:50 2012
 *
 * @brief  This file implements ROOT histogram manipulation tools
 *
 *         1. Hist::rescale()
 *         2. Hist::HistReader - implements a class to read multiple
 *            ROOT objects matching a regular expression.
 *
 */

#ifndef __HISTUTILS_HXX
#define __HISTUTILS_HXX

#include <string>
#include <vector>

#include <boost/regex.hpp>

#include <TH1.h>
#include <TKey.h>
#include <TClass.h>
#include <TCollection.h>
#include <TDirectory.h>
#include <TString.h>


namespace Hist {

  /**
   * Rescale X-axis of the passed histogram.
   *
   * @param h Pointer to histogram
   * @param factor Scale factor. (default = 0.001, corresponds to GeV/MeV -> TeV/GeV)
   */
  void rescale(TH1 *hist, Double_t factor);


  /**
   * This class provides an interface to read histograms from ROOT
   * files by matching regular expressions to histogram names. It
   * recursively looks through subdirectories to find matches. All the
   * getter methods are templated, so essentially one can also get
   * other ROOT objects.
   *
   * TODO: Port the regex bits to Boost.Regex or Boost.Xpressive
   *
   */
  class HistReader {

  public:

    /**
     * Instantiate HistReader with "dir" as the top level directory
     *
     * @param dir root directory
     */
    HistReader(TDirectory *dir) : _dir(dir) {}

    /**
     * Instantiate HistReader with "dir/subdir" as the top level directory
     *
     * @param dir root directory
     * @param subdir Subdirectory
     */
    HistReader(TDirectory *dir, std::string subdir)
    {
      _dir = dynamic_cast<TDirectory*>(dir->GetDirectory(subdir.c_str()));
    }

    ~HistReader() {}

    /**
     * Return the first histogram matching the regex
     *
     * @param hregex Regular expression to match with histogram names
     * @param hist First matching histogram
     */
    template <class T>
    void getHist(std::string hregex, T* &hist)
    {
      std::vector<T*> histograms;
      _getHistVec(boost::regex(hregex), histograms, _dir);
      hist = histograms[0];
      return;
    }

    /**
     * Get a vector of histograms matching the regular expression
     *
     * @param hregex Regular expression to match with histogram names
     * @param histograms Vector of matching histograms
     */
    template <class T>
    void getHistVec(std::string hregex, std::vector<T*> &histograms)
    {
      _getHistVec(boost::regex(hregex), histograms, _dir);
      return;
    }

  private:

    /**
     * Get a vector of histograms matching the regular expression
     *
     * @param hregex Regular expression to match with histogram names
     * @param histograms Vector of matching histograms
     * @param dir Start search in this directory
     */
    template <class T>
    void _getHistVec(boost::regex hregex, std::vector<T*> &histograms,
		     TDirectory *dir)
    {
      TIter keylist(dir->GetListOfKeys());
      TKey *key;

      while ((key = dynamic_cast<TKey*> (keylist()))) {
	TClass *objClass = TClass::GetClass(key->GetClassName());

	// recurse through subdirectories
	if (objClass->InheritsFrom(TDirectory::Class())) {
	  TDirectory *subdir = static_cast<TDirectory*>
	    (key->ReadObjectAny(TDirectory::Class()));
	  _getHistVec(hregex, histograms, subdir);
	}

	// get histogram if there is a match
	if (objClass->InheritsFrom(T::Class())) {
	  std::string keyname(key->GetName());
	  if (boost::regex_match(keyname, hregex)) {
	    histograms.push_back(static_cast<T*>
				 (key->ReadObjectAny(T::Class())));
	  }
	}
      }
    }

    TDirectory *_dir;		/**< Top level directory to search for histograms */
  };
}

#endif	// __HISTUTILS_HXX
