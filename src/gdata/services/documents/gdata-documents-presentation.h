/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * GData Client
 * Copyright (C) Thibault Saunier 2009 <saunierthibault@gmail.com>
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

#ifndef GDATA_DOCUMENTS_PRESENTATION_H
#define GDATA_DOCUMENTS_PRESENTATION_H

#include <glib.h>
#include <glib-object.h>

#include <gdata/services/documents/gdata-documents-entry.h>
#include <gdata/gdata-types.h>

G_BEGIN_DECLS

#define GDATA_TYPE_DOCUMENTS_PRESENTATION		(gdata_documents_presentation_get_type ())
#define GDATA_DOCUMENTS_PRESENTATION(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), GDATA_TYPE_DOCUMENTS_PRESENTATION, GDataDocumentsPresentation))
#define GDATA_DOCUMENTS_PRESENTATION_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GDATA_TYPE_DOCUMENTS_PRESENTATION, GDataDocumentsPresentationClass))
#define GDATA_IS_DOCUMENTS_PRESENTATION(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GDATA_TYPE_DOCUMENTS_PRESENTATION))
#define GDATA_IS_DOCUMENTS_PRESENTATION_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), GDATA_TYPE_DOCUMENTS_PRESENTATION))
#define GDATA_DOCUMENTS_PRESENTATION_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GDATA_TYPE_DOCUMENTS_PRESENTATION, GDataDocumentsPresentationClass))

typedef struct _GDataDocumentsPresentationPrivate	GDataDocumentsPresentationPrivate;

/**
 * GDataDocumentsPresentationFormat:
 * @GDATA_DOCUMENTS_PRESENTATION_PDF: the document in PDF format
 * @GDATA_DOCUMENTS_PRESENTATION_PNG: the document in PNG image format
 * @GDATA_DOCUMENTS_PRESENTATION_PPT: the document in Microsoft PowerPoint PPT format
 * @GDATA_DOCUMENTS_PRESENTATION_SWF: the document in Adobe Flash SWF format
 * @GDATA_DOCUMENTS_PRESENTATION_TXT: the document in text format
 *
 * The different available download formats for presentations.
 *
 * Since: 0.4.0
 **/
typedef enum {
	GDATA_DOCUMENTS_PRESENTATION_PDF = 0,
	GDATA_DOCUMENTS_PRESENTATION_PNG,
	GDATA_DOCUMENTS_PRESENTATION_PPT,
	GDATA_DOCUMENTS_PRESENTATION_SWF,
	GDATA_DOCUMENTS_PRESENTATION_TXT,
} GDataDocumentsPresentationFormat;

/**
 * GDataDocumentsPresentation:
 *
 * All the fields in the #GDataDocumentsPresentation structure are private and should never be accessed directly.
 *
 * Since: 0.4.0
 **/
typedef struct {
	GDataDocumentsEntry parent;
	GDataDocumentsPresentationPrivate *priv;
} GDataDocumentsPresentation;

/**
 * GDataDocumentsPresentationClass:
 *
 * All the fields in the #GDataDocumentsPresentationClass structure are private and should never be accessed directly.
 *
 * Since: 0.4.0
 **/
typedef struct {
	/*< private >*/
	GDataDocumentsEntryClass parent;
} GDataDocumentsPresentationClass;

GType gdata_documents_presentation_get_type (void) G_GNUC_CONST;

GDataDocumentsPresentation *gdata_documents_presentation_new (const gchar *id) G_GNUC_WARN_UNUSED_RESULT;

#include <gdata/services/documents/gdata-documents-service.h>
GFile *gdata_documents_presentation_download_document (GDataDocumentsPresentation *self, GDataDocumentsService *service, gchar **content_type,
						       GDataDocumentsPresentationFormat export_format, GFile *destination_file,
						       gboolean replace_file_if_exists, GCancellable *cancellable,
						       GError **error) G_GNUC_WARN_UNUSED_RESULT;
gchar *gdata_documents_presentation_get_download_uri (GDataDocumentsPresentation *self, GDataDocumentsPresentationFormat export_format) G_GNUC_WARN_UNUSED_RESULT;

G_END_DECLS

#endif /* !GDATA_DOCUMENTS_PRESENTATION_H */
