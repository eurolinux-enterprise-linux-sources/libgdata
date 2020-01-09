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

#ifndef GDATA_DOCUMENTS_ENTRY_H
#define GDATA_DOCUMENTS_ENTRY_H

#include <glib.h>
#include <glib-object.h>

#include <gdata/gdata-entry.h>
#include <gdata/gdata-service.h>
#include <gdata/atom/gdata-author.h>

G_BEGIN_DECLS

#define GDATA_TYPE_DOCUMENTS_ENTRY		(gdata_documents_entry_get_type ())
#define GDATA_DOCUMENTS_ENTRY(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), GDATA_TYPE_DOCUMENTS_ENTRY, GDataDocumentsEntry))
#define GDATA_DOCUMENTS_ENTRY_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GDATA_TYPE_DOCUMENTS_ENTRY, GDataDocumentsEntryClass))
#define GDATA_IS_DOCUMENTS_ENTRY(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GDATA_TYPE_DOCUMENTS_ENTRY))
#define GDATA_IS_DOCUMENTS_ENTRY_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), GDATA_TYPE_DOCUMENTS_ENTRY))
#define GDATA_DOCUMENTS_ENTRY_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GDATA_TYPE_DOCUMENTS_ENTRY, GDataDocumentsEntryClass))

typedef struct _GDataDocumentsEntryPrivate	GDataDocumentsEntryPrivate;

/**
 * GDataDocumentsEntry:
 *
 * All the fields in the #GDataDocumentsEntry structure are private and should never be accessed directly.
 *
 * Since: 0.4.0
 **/
typedef struct {
	GDataEntry parent;
	GDataDocumentsEntryPrivate *priv;
} GDataDocumentsEntry;

/**
 * GDataDocumentsEntryClass:
 *
 * All the fields in the #GDataDocumentsEntryClass structure are private and should never be accessed directly.
 *
 * Since: 0.4.0
 **/
typedef struct {
	/*< private >*/
	GDataEntryClass parent;
} GDataDocumentsEntryClass;

GType gdata_documents_entry_get_type (void) G_GNUC_CONST;

gchar *gdata_documents_entry_get_path (GDataDocumentsEntry *self);

const gchar *gdata_documents_entry_get_document_id (GDataDocumentsEntry *self);

void gdata_documents_entry_get_edited (GDataDocumentsEntry *self, GTimeVal *edited);
void gdata_documents_entry_get_last_viewed (GDataDocumentsEntry *self, GTimeVal *last_viewed);

void gdata_documents_entry_set_writers_can_invite (GDataDocumentsEntry *self, gboolean writers_can_invite);
gboolean gdata_documents_entry_writers_can_invite (GDataDocumentsEntry *self);

GDataAuthor *gdata_documents_entry_get_last_modified_by (GDataDocumentsEntry *self);

gboolean gdata_documents_entry_is_deleted (GDataDocumentsEntry *self);

G_END_DECLS

#endif /* !GDATA_DOCUMENTS_ENTRY_H */
