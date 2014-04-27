#ifndef __PARSERS_HXX
#define __PARSERS_HXX

#include <string>

#include <TString.h>


namespace Parsers {

  /**
   * Parse print options and return output file format
   *
   * @param opt Print options
   * @param format output file format
   *
   * @return
   */
  bool PrintOpts(TString& opt, TString& format);

  /**
   * Parse the passed file to initialise variables.
   *
   * @param var Vector of TString for variable names.
   * @param val Vector of TString for variable values.
   * @param fname Configuration file to parse.
   */
  void readconf(std::vector<TString> &var, std::vector<TString> &val, std::string fname);

  /**
   * Parse file to read a list.
   *
   * @param var Vector of TString for file names.
   * @param fname Filename with list
   */
  void readlist(std::vector<TString> &var, std::string fname);

  /**
   * Search and replace string within provided string
   *
   * @param context Original string
   * @param from Search string
   * @param to Replacement string
   *
   * @return Return original string after replacement
   */
  std::string& replaceAll(std::string& context, const std::string& from, const std::string& to);
}

#endif	// __PARSERS_HXX
