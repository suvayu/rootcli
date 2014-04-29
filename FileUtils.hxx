/**
 * @file   FileUtils.hxx
 * @author Suvayu Ali <Suvayu.Ali@cern.ch>
 * @date   Mon Apr 28 18:47:50 2014
 *
 * @brief  This file implements ROOT File navigation & manipulation routines
 *
 */

#ifndef __FILEUTILS_HXX
#define __FILEUTILS_HXX

#include <vector>
#include <functional>

#include <TKey.h>
#include <TClass.h>
#include <TDirectory.h>


namespace File{
  // typedef void (* obj_p_t) (TKey *, std::vector<TObject*> &);
  typedef std::function<void (TKey *, std::vector<TObject*> &)> obj_p_t;

  void recurse_thru_dir(TDirectory *dir, TClass *fclass, obj_p_t obj_p,
			std::vector<TObject*> &obj_vec);
}

#endif	// __FILEUTILS_HXX
