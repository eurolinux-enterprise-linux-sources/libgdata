/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * GData Client
 * Copyright (C) Richard Schwarting 2009 <aquarichy@gmail.com>
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

/**
 * SECTION:gdata-picasaweb-service
 * @short_description: GData PicasaWeb service object
 * @stability: Unstable
 * @include: gdata/services/picasaweb/gdata-picasaweb-service.h
 *
 * #GDataPicasaWebService is a subclass of #GDataService for communicating with the GData API of Google PicasaWeb. It supports querying for files
 * and albums, and uploading files.
 *
 * For more details of PicasaWeb's GData API, see the <ulink type="http" url="http://code.google.com/apis/picasaweb/developers_guide_protocol.html">
 * online documentation</ulink>.
 **/

#include <config.h>
#include <glib.h>
#include <glib/gi18n-lib.h>

#include "gdata-service.h"
#include "gdata-picasaweb-service.h"
#include "gdata-private.h"
#include "gdata-parser.h"
#include "atom/gdata-link.h"
#include "gdata-upload-stream.h"

G_DEFINE_TYPE (GDataPicasaWebService, gdata_picasaweb_service, GDATA_TYPE_SERVICE)

static void
gdata_picasaweb_service_class_init (GDataPicasaWebServiceClass *klass)
{
	GDataServiceClass *service_class = GDATA_SERVICE_CLASS (klass);
	service_class->service_name = "lh2";
}

static void
gdata_picasaweb_service_init (GDataPicasaWebService *self)
{
	/* Nothing to see here */
}

/*
 * This constructs the URI we want to access for querying albums.
 *
 * Remember to free the URI in the caller.
*/
static gchar *
create_uri (GDataPicasaWebService *self, const gchar *username)
{
	if (username == NULL) {
		/* Ensure we're authenticated first */
		if (gdata_service_is_authenticated (GDATA_SERVICE (self)) == FALSE)
			return NULL;

		/* Querying Picasa albums for the "default" user when logged in returns the albums for the authenticated user */
		username = "default";
	}

	return g_strdup_printf ("http://picasaweb.google.com/data/feed/api/user/%s", username);
}

/**
 * gdata_picasaweb_service_new:
 * @client_id: your application's client ID
 *
 * Creates a new #GDataPicasaWebService. The @client_id must be unique for your application, and as registered with Google.
 *
 * Return value: a new #GDataPicasaWebService, or %NULL
 *
 * Since: 0.4.0
 **/
GDataPicasaWebService *
gdata_picasaweb_service_new (const gchar *client_id)
{
	g_return_val_if_fail (client_id != NULL, NULL);

	return g_object_new (GDATA_TYPE_PICASAWEB_SERVICE,
			     "client-id", client_id,
			     NULL);
}

/**
 * gdata_picasaweb_service_query_all_albums:
 * @self: a #GDataPicasaWebService
 * @query: a #GDataQuery with the query parameters, or %NULL
 * @username: the username of the user whose albums you wish to retrieve, or %NULL
 * @cancellable: optional #GCancellable object, or %NULL
 * @progress_callback: a #GDataQueryProgressCallback to call when an entry is loaded, or %NULL
 * @progress_user_data: data to pass to the @progress_callback function
 * @error: a #GError, or %NULL
 *
 * Queries the service to return a list of all albums belonging to the specified @username which match the given
 * @query. If a user is authenticated with the service, @username can be set as %NULL to return a list of albums belonging
 * to the currently-authenticated user.
 *
 * Note that the #GDataQuery:q query parameter cannot be set on @query for album queries.
 *
 * For more details, see gdata_service_query().
 *
 * Return value: a #GDataFeed of query results; unref with g_object_unref()
 *
 * Since: 0.4.0
 **/
GDataFeed *
gdata_picasaweb_service_query_all_albums (GDataPicasaWebService *self, GDataQuery *query, const gchar *username, GCancellable *cancellable,
					  GDataQueryProgressCallback progress_callback, gpointer progress_user_data, GError **error)
{
	gchar *uri;
	GDataFeed *album_feed;

	g_return_val_if_fail (GDATA_IS_PICASAWEB_SERVICE (self), NULL);
	g_return_val_if_fail (query == NULL || GDATA_IS_QUERY (query), NULL);

	if (query != NULL && gdata_query_get_q (query) != NULL) {
		/* Bug #593336 — Query parameter "q=..." isn't valid for album kinds */
		g_set_error_literal (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_BAD_QUERY_PARAMETER,
				     _("Query parameter not allowed for albums."));
		return NULL;
	}

	uri = create_uri (self, username);
	if (uri == NULL) {
		g_set_error_literal (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_AUTHENTICATION_REQUIRED,
				     _("You must specify a username or be authenticated to query all albums."));
		return NULL;
	}

	/* Execute the query */
	album_feed = gdata_service_query (GDATA_SERVICE (self), uri, query, GDATA_TYPE_PICASAWEB_ALBUM,
					  cancellable, progress_callback, progress_user_data, error);
	g_free (uri);

	return album_feed;
}

/**
 * gdata_picasaweb_service_query_all_albums_async:
 * @self: a #GDataPicasaWebService
 * @query: a #GDataQuery with the query parameters, or %NULL
 * @username: the username of the user whose albums you wish to retrieve, or %NULL
 * @cancellable: optional #GCancellable object, or %NULL
 * @progress_callback: a #GDataQueryProgressCallback to call when an entry is loaded, or %NULL
 * @progress_user_data: data to pass to the @progress_callback function
 * @callback: a #GAsyncReadyCallback to call when authentication is finished
 * @user_data: data to pass to the @callback function
 *
 * Queries the service to return a list of all albums belonging to the specified @username which match the given
 * @query. @self, @query and @username are all reffed/copied when this function is called, so can safely be unreffed/freed after
 * this function returns.
 *
 * For more details, see gdata_picasaweb_service_query_all_albums(), which is the synchronous version of
 * this function, and gdata_service_query_async(), which is the base asynchronous query function.
 *
 * Since: 0.4.0
 **/
void
gdata_picasaweb_service_query_all_albums_async (GDataPicasaWebService *self, GDataQuery *query, const gchar *username,
						GCancellable *cancellable, GDataQueryProgressCallback progress_callback, gpointer progress_user_data,
						GAsyncReadyCallback callback, gpointer user_data)
{
	gchar *uri;

	g_return_if_fail (GDATA_IS_PICASAWEB_SERVICE (self));
	g_return_if_fail (query == NULL || GDATA_IS_QUERY (query));
	g_return_if_fail (callback != NULL);

	if (query != NULL && gdata_query_get_q (query) != NULL) {
		/* Bug #593336 — Query parameter "q=..." isn't valid for album kinds */
		g_simple_async_report_error_in_idle (G_OBJECT (self), callback, user_data,
						     GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_BAD_QUERY_PARAMETER,
						     _("Query parameter not allowed for albums."));
		return;
	}

	uri = create_uri (self, username);
	if (uri == NULL) {
		g_simple_async_report_error_in_idle (G_OBJECT (self), callback, user_data,
						     GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_AUTHENTICATION_REQUIRED,
						     _("You must specify a username or be authenticated to query all albums."));
		return;
	}

	/* Schedule the async query */
	gdata_service_query_async (GDATA_SERVICE (self), uri, query, GDATA_TYPE_PICASAWEB_ALBUM, cancellable, progress_callback, progress_user_data,
				   callback, user_data);
	g_free (uri);
}

/**
 * gdata_picasaweb_service_query_files:
 * @self: a #GDataPicasaWebService
 * @album: a #GDataPicasaWebAlbum from which to retrieve the files, or %NULL
 * @query: a #GDataQuery with the query parameters, or %NULL
 * @cancellable: optional #GCancellable object, or %NULL
 * @progress_callback: a #GDataQueryProgressCallback to call when an entry is loaded, or %NULL
 * @progress_user_data: data to pass to the @progress_callback function
 * @error: a #GError, or %NULL
 *
 * Queries the specified @album for a list of the files which match the given @query. If @album is %NULL and a user is
 * authenticated with the service, the user's default album will be queried.
 *
 * For more details, see gdata_service_query().
 *
 * Return value: a #GDataFeed of query results; unref with g_object_unref()
 *
 * Since: 0.4.0
 **/
GDataFeed *
gdata_picasaweb_service_query_files (GDataPicasaWebService *self, GDataPicasaWebAlbum *album, GDataQuery *query, GCancellable *cancellable,
				     GDataQueryProgressCallback progress_callback, gpointer progress_user_data, GError **error)
{
	/* TODO: Async variant */
	const gchar *uri;

	if (album != NULL) {
		GDataLink *link = gdata_entry_look_up_link (GDATA_ENTRY (album), "http://schemas.google.com/g/2005#feed");
		if (link == NULL) {
			/* Error */
			g_set_error_literal (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_PROTOCOL_ERROR,
					     _("The album did not have a feed link."));
			return NULL;
		}
		uri = gdata_link_get_uri (link);
	} else {
		/* Default URI */
		uri = "http://picasaweb.google.com/data/feed/api/user/default/albumid/default";
	}

	/* Execute the query */
	return gdata_service_query (GDATA_SERVICE (self), uri, GDATA_QUERY (query), GDATA_TYPE_PICASAWEB_FILE, cancellable,
				    progress_callback, progress_user_data, error);
}

/**
 * gdata_picasaweb_service_upload_file:
 * @self: a #GDataPicasaWebService
 * @album: a #GDataPicasaWebAlbum into which to insert the file, or %NULL
 * @file_entry: a #GDataPicasaWebFile to insert
 * @file_data: the actual file to upload
 * @cancellable: optional #GCancellable object, or %NULL
 * @error: a #GError, or %NULL
 *
 * Uploads a file (photo or video) to the given PicasaWeb @album, using the @actual_file from disk and the metadata from @file. If @album is
 * %NULL, the file will be uploaded to the currently-authenticated user's "Drop Box" album. A user must be authenticated to use this function.
 *
 * If @file has already been inserted, a %GDATA_SERVICE_ERROR_ENTRY_ALREADY_INSERTED error will be returned. If no user is authenticated
 * with the service, %GDATA_SERVICE_ERROR_AUTHENTICATION_REQUIRED will be returned.
 *
 * If there is a problem reading @file_data, an error from g_output_stream_splice() or g_file_query_info() will be returned. Other errors from
 * #GDataServiceError can be returned for other exceptional conditions, as determined by the server.
 *
 * Return value: the inserted #GDataPicasaWebFile with updated properties from @file_entry; unref with g_object_unref()
 *
 * Since: 0.4.0
 **/
GDataPicasaWebFile *
gdata_picasaweb_service_upload_file (GDataPicasaWebService *self, GDataPicasaWebAlbum *album, GDataPicasaWebFile *file_entry, GFile *file_data,
				     GCancellable *cancellable, GError **error)
{
	GDataPicasaWebFile *new_entry;
	GDataCategory *category;
	GOutputStream *output_stream;
	GInputStream *input_stream;
	const gchar *slug = NULL, *content_type = NULL, *response_body = NULL, *user_id = NULL, *album_id = NULL;
	gchar *upload_uri;
	gssize response_length;
	GFileInfo *file_info = NULL;

	g_return_val_if_fail (GDATA_IS_PICASAWEB_SERVICE (self), NULL);
	g_return_val_if_fail (GDATA_IS_PICASAWEB_FILE (file_entry), NULL);
	g_return_val_if_fail (G_IS_FILE (file_data), NULL);

	if (gdata_entry_is_inserted (GDATA_ENTRY (file_entry)) == TRUE) {
		g_set_error_literal (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_ENTRY_ALREADY_INSERTED,
				     _("The entry has already been inserted."));
		return NULL;
	}

	if (gdata_service_is_authenticated (GDATA_SERVICE (self)) == FALSE) {
		g_set_error_literal (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_AUTHENTICATION_REQUIRED,
				     _("You must be authenticated to upload a file."));
		return NULL;
	}

	file_info = g_file_query_info (file_data, "standard::display-name,standard::content-type", G_FILE_QUERY_INFO_NONE, NULL, error);
	if (file_info == NULL)
		return NULL;

	slug = g_file_info_get_display_name (file_info);
	content_type = g_file_info_get_content_type (file_info);

	/* Add the "photo" kind if the entry is missing it. If it already has the kind category, no duplicate is added. */
	category = gdata_category_new ("http://schemas.google.com/photos/2007#photo", "http://schemas.google.com/g/2005#kind", NULL);
	gdata_entry_add_category (GDATA_ENTRY (file_entry), category);
	g_object_unref (category);

	/* PicasaWeb allows you to post to a default Dropbox */
	album_id = (album != NULL) ? gdata_entry_get_id (GDATA_ENTRY (album)) : "default";
	user_id = gdata_service_get_username (GDATA_SERVICE (self));

	/* Build the upload URI and upload stream */
	upload_uri = g_strdup_printf ("http://picasaweb.google.com/data/feed/api/user/%s/albumid/%s", user_id, album_id);
	output_stream = gdata_upload_stream_new (GDATA_SERVICE (self), SOUP_METHOD_POST, upload_uri, GDATA_ENTRY (file_entry), slug, content_type);
	g_free (upload_uri);
	g_object_unref (file_info);

	if (output_stream == NULL)
		return NULL;

	/* Pipe the input file to the upload stream */
	input_stream = G_INPUT_STREAM (g_file_read (file_data, cancellable, error));
	if (input_stream == NULL) {
		g_object_unref (output_stream);
		return NULL;
	}

	g_output_stream_splice (output_stream, input_stream, G_OUTPUT_STREAM_SPLICE_CLOSE_SOURCE | G_OUTPUT_STREAM_SPLICE_CLOSE_TARGET,
				cancellable, error);

	g_object_unref (input_stream);
	if (error != NULL && *error != NULL) {
		/* Error! */
		g_object_unref (output_stream);
		return NULL;
	}

	/* Get the response from the server */
	response_body = gdata_upload_stream_get_response (GDATA_UPLOAD_STREAM (output_stream), &response_length);
	g_assert (response_body != NULL && response_length > 0);

	/* Parse the response to produce a GDataPicasaWebFile */
	new_entry = GDATA_PICASAWEB_FILE (gdata_parsable_new_from_xml (GDATA_TYPE_PICASAWEB_FILE, response_body, (gint) response_length, error));
	g_object_unref (output_stream);

	return new_entry;
}
