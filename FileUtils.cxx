#include <TCollection.h>

#include "FileUtils.hxx"


namespace File {
  void recurse_thru_dir(TDirectory *dir, TClass *fclass, obj_p_t obj_p,
			std::vector<TObject*> &obj_vec)
  {
    TIter keylist(dir->GetListOfKeys());
    TKey *key;

    while ((key = dynamic_cast<TKey*> (keylist()))) {
      TClass *objClass = TClass::GetClass(key->GetClassName());

      // recurse through subdirectories
      if (objClass->InheritsFrom(TDirectory::Class())) {
	TDirectory *subdir = dynamic_cast<TDirectory*>(key->ReadObj());
	recurse_thru_dir(subdir, fclass, obj_p, obj_vec);
      }

      // call predicate if not a directory (iow, regular object)
      if (objClass->InheritsFrom(fclass)) {
	obj_p(key, obj_vec);
      }
    }
    return;
  }
}
