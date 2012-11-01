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

#include <TH1.h>
#include <TClass.h>
#include <TDirectory.h>


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
   * TODO: Ported regex bits to Boost.Regex. Check if Boost.Xpressive
   * would be better.
   *
   */
  class HistReader {

  public:

    /// Regular expression backend
    enum backend_t {
      kposix,			/**< Use POSIX regular expressions from regex.h */
      kBoost			/**< Use regular expressions from Boost.Regex */
    };

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
     * Note that the only safe use of this method is when the regex is
     * an exact match to the object name.
     *
     * @param hregex Regular expression to match with histogram names
     * @param hist First matching histogram
     */
    template <class T>
    void getHist(std::string hregex, T* &hist,
		 backend_t backend=HistReader::kBoost)
    {
      std::vector<TObject*> histos;
      _getHistVec(hregex, histos, _dir, T::Class(), backend);
      hist = dynamic_cast<T*>(histos[0]);
      return;
    }

    /**
     * Get a vector of histograms matching the regular expression
     *
     * @param hregex Regular expression to match with histogram names
     * @param histograms Vector of matching histograms
     */
    template <class T>
    void getHistVec(std::string hregex, std::vector<T*> &histograms,
		    backend_t backend=HistReader::kBoost)
    {
      std::vector<TObject*> histos;
      if (kBoost == backend)
	_getHistVec(hregex, histos, _dir, T::Class());
      else if (kposix == backend)
	_getHistVec_posix(hregex, histos, _dir, T::Class());
      else
	// FIXME: Error message goes here
	return;
      for (unsigned i = 0; i < histos.size(); ++i) {
	histograms.push_back(dynamic_cast<T*>(histos[i]));
      }
      return;
    }

  private:

    /**
     * Get a vector of histograms matching the regular expression
     *
     * This method uses Boost.Regex for the regular expression
     * matching. This can be troublesome to compile with sucky/obscure
     * build systems like CMT. In that case use _getHistVec_posix(..).
     *
     * @param hregex Regular expression to match with histogram names
     * @param histos Vector of matching histograms
     * @param dir Start search in this directory
     * @param fclass TClass object for type checking
     */
    void _getHistVec(std::string hregex, std::vector<TObject*> &histos,
		     TDirectory *dir, TClass *fclass);

    /**
     * Get a vector of histograms matching the regular expression
     *
     * This method uses POSIX regex.h regular expressions. Note that
     * for this backend a match includes a partial match. Hence it is
     * recommended to anchor your regular expression.
     *
     * + Anchored regex - ^hperBeetle.*_35_(0|1|2|3)$
     * + Normal regex - hperBeetle.*_35_(0|1|2|3)
     *
     * @param hregex Regular expression to match with histogram names
     * @param histos Vector of matching histograms
     * @param dir Start search in this directory
     * @param fclass TClass object for type checking
     */
    void _getHistVec_posix(std::string hregex, std::vector<TObject*> &histos,
			   TDirectory *dir, TClass *fclass);

    TDirectory *_dir;		/**< Top level directory to search for histograms */
  };
}

#endif	// __HISTUTILS_HXX
