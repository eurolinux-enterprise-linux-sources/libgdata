/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * GData Client
 * Copyright (C) Philip Withnall 2009 <philip@tecnocode.co.uk>
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

#ifndef GDATA_UPLOAD_STREAM_H
#define GDATA_UPLOAD_STREAM_H

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <gdata/gdata-service.h>
#include <gdata/gdata-entry.h>

G_BEGIN_DECLS

#define GDATA_TYPE_UPLOAD_STREAM		(gdata_upload_stream_get_type ())
#define GDATA_UPLOAD_STREAM(o)			(G_TYPE_CHECK_INSTANCE_CAST ((o), GDATA_TYPE_UPLOAD_STREAM, GDataUploadStream))
#define GDATA_UPLOAD_STREAM_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GDATA_TYPE_UPLOAD_STREAM, GDataUploadStreamClass))
#define GDATA_IS_UPLOAD_STREAM(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GDATA_TYPE_UPLOAD_STREAM))
#define GDATA_IS_UPLOAD_STREAM_CLASS(k)		(G_TYPE_CHECK_CLASS_TYPE ((k), GDATA_TYPE_UPLOAD_STREAM))
#define GDATA_UPLOAD_STREAM_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GDATA_TYPE_UPLOAD_STREAM, GDataUploadStreamClass))

typedef struct _GDataUploadStreamPrivate	GDataUploadStreamPrivate;

/**
 * GDataUploadStream:
 *
 * All the fields in the #GDataUploadStream structure are private and should never be accessed directly.
 *
 * Since: 0.5.0
 **/
typedef struct {
	GOutputStream parent;
	GDataUploadStreamPrivate *priv;
} GDataUploadStream;

/**
 * GDataUploadStreamClass:
 *
 * All the fields in the #GDataUploadStreamClass structure are private and should never be accessed directly.
 *
 * Since: 0.5.0
 **/
typedef struct {
	/*< private >*/
	GOutputStreamClass parent;
} GDataUploadStreamClass;

GType gdata_upload_stream_get_type (void) G_GNUC_CONST;

GOutputStream *gdata_upload_stream_new (GDataService *service, const gchar *method, const gchar *upload_uri, GDataEntry *entry,
					const gchar *slug, const gchar *content_type) G_GNUC_WARN_UNUSED_RESULT;

const gchar *gdata_upload_stream_get_response (GDataUploadStream *self, gssize *length);

GDataService *gdata_upload_stream_get_service (GDataUploadStream *self);
const gchar *gdata_upload_stream_get_upload_uri (GDataUploadStream *self);
GDataEntry *gdata_upload_stream_get_entry (GDataUploadStream *self);
const gchar *gdata_upload_stream_get_slug (GDataUploadStream *self);
const gchar *gdata_upload_stream_get_content_type (GDataUploadStream *self);

G_END_DECLS

#endif /* !GDATA_UPLOAD_STREAM_H */
