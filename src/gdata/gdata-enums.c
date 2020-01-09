
/* Generated data (by glib-mkenums) */

#include "gdata.h"
#include "gdata-enums.h"

/* enumerations from "gdata-service.h" */
GType
gdata_service_error_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { GDATA_SERVICE_ERROR_UNAVAILABLE, "GDATA_SERVICE_ERROR_UNAVAILABLE", "unavailable" },
      { GDATA_SERVICE_ERROR_PROTOCOL_ERROR, "GDATA_SERVICE_ERROR_PROTOCOL_ERROR", "protocol-error" },
      { GDATA_SERVICE_ERROR_WITH_QUERY, "GDATA_SERVICE_ERROR_WITH_QUERY", "with-query" },
      { GDATA_SERVICE_ERROR_ENTRY_ALREADY_INSERTED, "GDATA_SERVICE_ERROR_ENTRY_ALREADY_INSERTED", "entry-already-inserted" },
      { GDATA_SERVICE_ERROR_WITH_INSERTION, "GDATA_SERVICE_ERROR_WITH_INSERTION", "with-insertion" },
      { GDATA_SERVICE_ERROR_AUTHENTICATION_REQUIRED, "GDATA_SERVICE_ERROR_AUTHENTICATION_REQUIRED", "authentication-required" },
      { GDATA_SERVICE_ERROR_WITH_UPDATE, "GDATA_SERVICE_ERROR_WITH_UPDATE", "with-update" },
      { GDATA_SERVICE_ERROR_WITH_DELETION, "GDATA_SERVICE_ERROR_WITH_DELETION", "with-deletion" },
      { GDATA_SERVICE_ERROR_NOT_FOUND, "GDATA_SERVICE_ERROR_NOT_FOUND", "not-found" },
      { GDATA_SERVICE_ERROR_CONFLICT, "GDATA_SERVICE_ERROR_CONFLICT", "conflict" },
      { GDATA_SERVICE_ERROR_FORBIDDEN, "GDATA_SERVICE_ERROR_FORBIDDEN", "forbidden" },
      { GDATA_SERVICE_ERROR_WITH_DOWNLOAD, "GDATA_SERVICE_ERROR_WITH_DOWNLOAD", "with-download" },
      { GDATA_SERVICE_ERROR_WITH_UPLOAD, "GDATA_SERVICE_ERROR_WITH_UPLOAD", "with-upload" },
      { GDATA_SERVICE_ERROR_BAD_QUERY_PARAMETER, "GDATA_SERVICE_ERROR_BAD_QUERY_PARAMETER", "bad-query-parameter" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("GDataServiceError", values);
  }
  return etype;
}
GType
gdata_authentication_error_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { GDATA_AUTHENTICATION_ERROR_BAD_AUTHENTICATION, "GDATA_AUTHENTICATION_ERROR_BAD_AUTHENTICATION", "bad-authentication" },
      { GDATA_AUTHENTICATION_ERROR_NOT_VERIFIED, "GDATA_AUTHENTICATION_ERROR_NOT_VERIFIED", "not-verified" },
      { GDATA_AUTHENTICATION_ERROR_TERMS_NOT_AGREED, "GDATA_AUTHENTICATION_ERROR_TERMS_NOT_AGREED", "terms-not-agreed" },
      { GDATA_AUTHENTICATION_ERROR_CAPTCHA_REQUIRED, "GDATA_AUTHENTICATION_ERROR_CAPTCHA_REQUIRED", "captcha-required" },
      { GDATA_AUTHENTICATION_ERROR_ACCOUNT_DELETED, "GDATA_AUTHENTICATION_ERROR_ACCOUNT_DELETED", "account-deleted" },
      { GDATA_AUTHENTICATION_ERROR_ACCOUNT_DISABLED, "GDATA_AUTHENTICATION_ERROR_ACCOUNT_DISABLED", "account-disabled" },
      { GDATA_AUTHENTICATION_ERROR_SERVICE_DISABLED, "GDATA_AUTHENTICATION_ERROR_SERVICE_DISABLED", "service-disabled" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("GDataAuthenticationError", values);
  }
  return etype;
}

/* enumerations from "gdata-parsable.h" */
GType
gdata_parser_error_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { GDATA_PARSER_ERROR_PARSING_STRING, "GDATA_PARSER_ERROR_PARSING_STRING", "parsing-string" },
      { GDATA_PARSER_ERROR_EMPTY_DOCUMENT, "GDATA_PARSER_ERROR_EMPTY_DOCUMENT", "empty-document" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("GDataParserError", values);
  }
  return etype;
}

/* Generated data ends here */

