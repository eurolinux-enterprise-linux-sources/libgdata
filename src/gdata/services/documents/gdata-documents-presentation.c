/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * GData Client
 * Copyright (C) Thibault Saunier 2009 <saunierthibault@gmail.com>
 * Copyright (C) Philip Withnall 2010 <philip@tecnocode.co.uk>
 *
 * GData Client is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * GData Client is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GData Client.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * SECTION:gdata-documents-presentation
 * @short_description: GData documents presentation object
 * @stability: Unstable
 * @include: gdata/services/documents/gdata-documents-presentation.h
 *
 * #GDataDocumentsPresentation is a subclass of #GDataDocumentsEntry to represent a Google Documents presentation.
 *
 * For more details of Google Documents' GData API, see the
 * <ulink type="http://code.google.com/apis/document/docs/2.0/developers_guide_protocol.html">online documentation</ulink>.
 *
 * Since: 0.4.0
 **/

#include <config.h>
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <libxml/parser.h>
#include <string.h>

#include "gdata-documents-presentation.h"
#include "gdata-documents-service.h"
#include "gdata-parser.h"
#include "gdata-types.h"
#include "gdata-private.h"

static void get_xml (GDataParsable *parsable, GString *xml_string);

static const gchar *export_formats[] = {
	"pdf", /* GDATA_DOCUMENTS_PRESENTATION_PDF */
	"png", /* GDATA_DOCUMENTS_PRESENTATION_PNG */
	"ppt", /* GDATA_DOCUMENTS_PRESENTATION_PPT */
	"swf", /* GDATA_DOCUMENTS_PRESENTATION_SWF */
	"txt" /* GDATA_DOCUMENTS_PRESENTATION_TXT */
};

G_DEFINE_TYPE (GDataDocumentsPresentation, gdata_documents_presentation, GDATA_TYPE_DOCUMENTS_ENTRY)
#define GDATA_DOCUMENTS_PRESENTATION_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GDATA_TYPE_DOCUMENTS_PRESENTATION, GDataDocumentsPresentationPrivate))

static void
gdata_documents_presentation_class_init (GDataDocumentsPresentationClass *klass)
{
	GDataParsableClass *parsable_class = GDATA_PARSABLE_CLASS (klass);
	parsable_class->get_xml = get_xml;
}

static void
gdata_documents_presentation_init (GDataDocumentsPresentation *self)
{
	/* Why am I writing it? */
}

static void
get_xml (GDataParsable *parsable, GString *xml_string)
{
	const gchar *document_id;

	/* Chain up to the parent class */
	GDATA_PARSABLE_CLASS (gdata_documents_presentation_parent_class)->get_xml (parsable, xml_string);

	document_id = gdata_documents_entry_get_document_id (GDATA_DOCUMENTS_ENTRY (parsable));
	if (document_id != NULL)
		g_string_append_printf (xml_string, "<gd:resourceId>presentation:%s</gd:resourceId>", document_id);
}

/**
 * gdata_documents_presentation_new:
 * @id: the entry's ID (not the document ID of the presentation), or %NULL
 *
 * Creates a new #GDataDocumentsPresentation with the given entry ID (#GDataEntry:id).
 *
 * Return value: a new #GDataDocumentsPresentation, or %NULL; unref with g_object_unref()
 *
 * Since: 0.4.0
 **/
GDataDocumentsPresentation *
gdata_documents_presentation_new (const gchar *id)
{
	return g_object_new (GDATA_TYPE_DOCUMENTS_PRESENTATION, "id", id, NULL);
}

/**
 * gdata_documents_presentation_download_document:
 * @self: a #GDataDocumentsPresentation
 * @service: a #GDataDocumentsService
 * @content_type: return location for the document's content type, or %NULL; free with g_free()
 * @export_format: the format in which the presentation should be exported
 * @destination_file: the #GFile into which the presentation file should be saved
 * @replace_file_if_exists: %TRUE if the file should be replaced if it already exists, %FALSE otherwise
 * @cancellable: optional #GCancellable object, or %NULL
 * @error: a #GError, or %NULL
 *
 * Downloads and returns the presentation file represented by the #GDataDocumentsPresentation. If the document doesn't exist,
 * %NULL is returned, but no error is set in @error. TODO: What?
 *
 * If @cancellable is not %NULL, then the operation can be cancelled by triggering the @cancellable object from another thread.
 * If the operation was cancelled, the error %G_IO_ERROR_CANCELLED will be returned.
 *
 * If there is an error getting the document, a %GDATA_SERVICE_ERROR_PROTOCOL_ERROR error will be returned.
 *
 * If @destination_file is a directory, then the file will be downloaded in this directory with the #GDataEntry:title with 
 * the apropriate extension as name.
 *
 * Return value: the document's data, or %NULL; unref with g_object_unref()
 *
 * Since: 0.4.0
 **/
GFile *
gdata_documents_presentation_download_document (GDataDocumentsPresentation *self, GDataDocumentsService *service, gchar **content_type,
						GDataDocumentsPresentationFormat export_format, GFile *destination_file,
						gboolean replace_file_if_exists, GCancellable *cancellable, GError **error)
{
	gchar *link_href;

	g_return_val_if_fail (GDATA_IS_DOCUMENTS_PRESENTATION (self), NULL);
	g_return_val_if_fail (GDATA_IS_DOCUMENTS_SERVICE (service), NULL);
	g_return_val_if_fail (G_IS_FILE (destination_file), NULL);
	g_return_val_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable), NULL);
	g_return_val_if_fail (export_format < G_N_ELEMENTS (export_formats), NULL);

	/* Call the common download method on the parent class */
	link_href = gdata_documents_presentation_get_download_uri (self, export_format);
	destination_file = _gdata_documents_entry_download_document (GDATA_DOCUMENTS_ENTRY (self), GDATA_SERVICE (service), content_type, link_href,
								     destination_file, export_formats[export_format], replace_file_if_exists, cancellable, error);
	g_free (link_href);

	return destination_file;
}

/**
 * gdata_documents_presentation_get_download_uri:
 * @self: a #GDataDocumentsPresentation
 * @export_format: the format in which the presentation should be exported when downloaded
 *
 * Builds and returns the download URI for the given #GDataDocumentsPresentation in the desired format. Note that directly downloading
 * the document using this URI isn't possible, as authentication is required. You should instead use gdata_download_stream_new() with
 * the URI, and use the resulting #GInputStream.
 *
 * Return value: the download URI; free with g_free()
 *
 * Since: 0.5.0
 **/
gchar *
gdata_documents_presentation_get_download_uri (GDataDocumentsPresentation *self, GDataDocumentsPresentationFormat export_format)
{
	const gchar *document_id;

	g_return_val_if_fail (export_format < G_N_ELEMENTS (export_formats), NULL);

	document_id = gdata_documents_entry_get_document_id (GDATA_DOCUMENTS_ENTRY (self));
	g_assert (document_id != NULL);

	return g_strdup_printf ("%s://docs.google.com/feeds/download/presentations/Export?exportFormat=%s&docID=%s",
	                        _gdata_service_get_scheme (), export_formats[export_format], document_id);
}
