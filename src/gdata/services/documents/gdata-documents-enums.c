
/* Generated data (by glib-mkenums) */

#include "gdata-documents-service.h"
#include "gdata-documents-text.h"
#include "gdata-documents-spreadsheet.h"
#include "gdata-documents-presentation.h"
#include "gdata-documents-enums.h"

/* enumerations from "gdata-documents-service.h" */
GType
gdata_documents_service_error_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { GDATA_DOCUMENTS_SERVICE_ERROR_INVALID_CONTENT_TYPE, "GDATA_DOCUMENTS_SERVICE_ERROR_INVALID_CONTENT_TYPE", "type" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("GDataDocumentsServiceError", values);
  }
  return etype;
}

/* enumerations from "gdata-documents-text.h" */
GType
gdata_documents_text_format_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { GDATA_DOCUMENTS_TEXT_DOC, "GDATA_DOCUMENTS_TEXT_DOC", "doc" },
      { GDATA_DOCUMENTS_TEXT_HTML, "GDATA_DOCUMENTS_TEXT_HTML", "html" },
      { GDATA_DOCUMENTS_TEXT_ODT, "GDATA_DOCUMENTS_TEXT_ODT", "odt" },
      { GDATA_DOCUMENTS_TEXT_PDF, "GDATA_DOCUMENTS_TEXT_PDF", "pdf" },
      { GDATA_DOCUMENTS_TEXT_PNG, "GDATA_DOCUMENTS_TEXT_PNG", "png" },
      { GDATA_DOCUMENTS_TEXT_RTF, "GDATA_DOCUMENTS_TEXT_RTF", "rtf" },
      { GDATA_DOCUMENTS_TEXT_TXT, "GDATA_DOCUMENTS_TEXT_TXT", "txt" },
      { GDATA_DOCUMENTS_TEXT_ZIP, "GDATA_DOCUMENTS_TEXT_ZIP", "zip" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("GDataDocumentsTextFormat", values);
  }
  return etype;
}

/* enumerations from "gdata-documents-presentation.h" */
GType
gdata_documents_presentation_format_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { GDATA_DOCUMENTS_PRESENTATION_PDF, "GDATA_DOCUMENTS_PRESENTATION_PDF", "pdf" },
      { GDATA_DOCUMENTS_PRESENTATION_PNG, "GDATA_DOCUMENTS_PRESENTATION_PNG", "png" },
      { GDATA_DOCUMENTS_PRESENTATION_PPT, "GDATA_DOCUMENTS_PRESENTATION_PPT", "ppt" },
      { GDATA_DOCUMENTS_PRESENTATION_SWF, "GDATA_DOCUMENTS_PRESENTATION_SWF", "swf" },
      { GDATA_DOCUMENTS_PRESENTATION_TXT, "GDATA_DOCUMENTS_PRESENTATION_TXT", "txt" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("GDataDocumentsPresentationFormat", values);
  }
  return etype;
}

/* enumerations from "gdata-documents-spreadsheet.h" */
GType
gdata_documents_spreadsheet_format_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { GDATA_DOCUMENTS_SPREADSHEET_XLS, "GDATA_DOCUMENTS_SPREADSHEET_XLS", "xls" },
      { GDATA_DOCUMENTS_SPREADSHEET_CSV, "GDATA_DOCUMENTS_SPREADSHEET_CSV", "csv" },
      { GDATA_DOCUMENTS_SPREADSHEET_PDF, "GDATA_DOCUMENTS_SPREADSHEET_PDF", "pdf" },
      { GDATA_DOCUMENTS_SPREADSHEET_ODS, "GDATA_DOCUMENTS_SPREADSHEET_ODS", "ods" },
      { GDATA_DOCUMENTS_SPREADSHEET_TSV, "GDATA_DOCUMENTS_SPREADSHEET_TSV", "tsv" },
      { GDATA_DOCUMENTS_SPREADSHEET_HTML, "GDATA_DOCUMENTS_SPREADSHEET_HTML", "html" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("GDataDocumentsSpreadsheetFormat", values);
  }
  return etype;
}

/* Generated data ends here */

