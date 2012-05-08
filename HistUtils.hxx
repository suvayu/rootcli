#ifndef __HISTUTILS_HXX
#define __HISTUTILS_HXX

#include <string>
#include <vector>

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <THnSparse.h>
#include <TKey.h>
#include <TClass.h>
#include <TCollection.h>
#include <TDirectory.h>
#include <TString.h>
#include <TRegexp.h>

namespace Hist {

  /**
   * Rescale X-axis of the passed histogram.
   *
   * @param h Pointer to histogram
   * @param factor Scale factor. (default = 0.001, corresponds to GeV/MeV -> TeV/GeV)
   */
  void rescale(TH1 *hist, Double_t factor);


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
      _getHistVec(TRegexp(hregex), histograms, _dir);
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
      _getHistVec(TRegexp(hregex), histograms, _dir);
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
    void _getHistVec(TRegexp hregex, std::vector<T*> &histograms,
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
	  TString keyname(key->GetName());
	  if (keyname.Contains(hregex)) {
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
