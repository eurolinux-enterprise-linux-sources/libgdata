


#include "gdata-freebase-service.h"
#include "gdata-freebase-search-query.h"
#include "gdata-freebase-result.h"
#include "gdata-freebase-enums.h"

/* enumerations from "/opt/gnome/source/libgdata/gdata/services/freebase/gdata-freebase-search-query.h" */
GType
gdata_freebase_search_filter_type_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { GDATA_FREEBASE_SEARCH_FILTER_ALL, "GDATA_FREEBASE_SEARCH_FILTER_ALL", "all" },
      { GDATA_FREEBASE_SEARCH_FILTER_ANY, "GDATA_FREEBASE_SEARCH_FILTER_ANY", "any" },
      { GDATA_FREEBASE_SEARCH_FILTER_NOT, "GDATA_FREEBASE_SEARCH_FILTER_NOT", "not" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("GDataFreebaseSearchFilterType", values);
  }
  return etype;
}



