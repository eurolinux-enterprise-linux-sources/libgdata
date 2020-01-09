/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * GData Client
 * Copyright (C) Philip Withnall 2008-2009 <philip@tecnocode.co.uk>
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

#ifndef GDATA_YOUTUBE_SERVICE_H
#define GDATA_YOUTUBE_SERVICE_H

#include <glib.h>
#include <glib-object.h>
#include <libsoup/soup.h>

#include <gdata/gdata-service.h>
#include <gdata/services/youtube/gdata-youtube-video.h>

G_BEGIN_DECLS

/**
 * GDataYouTubeStandardFeedType:
 * @GDATA_YOUTUBE_TOP_RATED_FEED: This feed contains the most highly rated YouTube videos.
 * @GDATA_YOUTUBE_TOP_FAVORITES_FEED: This feed contains videos most frequently flagged as favorite videos.
 * @GDATA_YOUTUBE_MOST_VIEWED_FEED: This feed contains the most frequently watched YouTube videos.
 * @GDATA_YOUTUBE_MOST_POPULAR_FEED: This feed contains the most popular YouTube videos, selected using an algorithm that combines many
 * different signals to determine overall popularity.
 * @GDATA_YOUTUBE_MOST_RECENT_FEED: This feed contains the videos most recently submitted to YouTube.
 * @GDATA_YOUTUBE_MOST_DISCUSSED_FEED: This feed contains the YouTube videos that have received the most comments.
 * @GDATA_YOUTUBE_MOST_LINKED_FEED: This feed contains the YouTube videos that receive the most links from other websites.
 * @GDATA_YOUTUBE_MOST_RESPONDED_FEED: This feed contains YouTube videos that receive the most video responses.
 * @GDATA_YOUTUBE_RECENTLY_FEATURED_FEED: This feed contains videos recently featured on the YouTube home page or featured videos tab.
 * @GDATA_YOUTUBE_WATCH_ON_MOBILE_FEED: This feed contains videos suitable for playback on mobile devices.
 *
 * Standard feed types for standard feed queries with gata_youtube_service_query_standard_feed(). For more information, see
 * the <ulink type="http" url="http://code.google.com/apis/youtube/2.0/developers_guide_protocol.html#Standard_feeds">online documentation</ulink>.
 **/
typedef enum {
	GDATA_YOUTUBE_TOP_RATED_FEED,
	GDATA_YOUTUBE_TOP_FAVORITES_FEED,
	GDATA_YOUTUBE_MOST_VIEWED_FEED,
	GDATA_YOUTUBE_MOST_POPULAR_FEED,
	GDATA_YOUTUBE_MOST_RECENT_FEED,
	GDATA_YOUTUBE_MOST_DISCUSSED_FEED,
	GDATA_YOUTUBE_MOST_LINKED_FEED,
	GDATA_YOUTUBE_MOST_RESPONDED_FEED,
	GDATA_YOUTUBE_RECENTLY_FEATURED_FEED,
	GDATA_YOUTUBE_WATCH_ON_MOBILE_FEED
} GDataYouTubeStandardFeedType;

/**
 * GDataYouTubeServiceError:
 * @GDATA_YOUTUBE_SERVICE_ERROR_API_QUOTA_EXCEEDED: the API request quota for this developer account has been exceeded
 * @GDATA_YOUTUBE_SERVICE_ERROR_ENTRY_QUOTA_EXCEEDED: the entry (e.g. video) quota for this user account has been exceeded
 *
 * Error codes for #GDataYouTubeService operations.
 **/
typedef enum {
	GDATA_YOUTUBE_SERVICE_ERROR_API_QUOTA_EXCEEDED,
	GDATA_YOUTUBE_SERVICE_ERROR_ENTRY_QUOTA_EXCEEDED
} GDataYouTubeServiceError;

#define GDATA_TYPE_YOUTUBE_SERVICE		(gdata_youtube_service_get_type ())
#define GDATA_YOUTUBE_SERVICE(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), GDATA_TYPE_YOUTUBE_SERVICE, GDataYouTubeService))
#define GDATA_YOUTUBE_SERVICE_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), GDATA_TYPE_YOUTUBE_SERVICE, GDataYouTubeServiceClass))
#define GDATA_IS_YOUTUBE_SERVICE(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), GDATA_TYPE_YOUTUBE_SERVICE))
#define GDATA_IS_YOUTUBE_SERVICE_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), GDATA_TYPE_YOUTUBE_SERVICE))
#define GDATA_YOUTUBE_SERVICE_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), GDATA_TYPE_YOUTUBE_SERVICE, GDataYouTubeServiceClass))

#define GDATA_YOUTUBE_SERVICE_ERROR		gdata_youtube_service_error_quark ()

typedef struct _GDataYouTubeServicePrivate	GDataYouTubeServicePrivate;

/**
 * GDataYouTubeService:
 *
 * All the fields in the #GDataYouTubeService structure are private and should never be accessed directly.
 **/
typedef struct {
	GDataService parent;
	GDataYouTubeServicePrivate *priv;
} GDataYouTubeService;

/**
 * GDataYouTubeServiceClass:
 *
 * All the fields in the #GDataYouTubeServiceClass structure are private and should never be accessed directly.
 **/
typedef struct {
	/*< private >*/
	GDataServiceClass parent;
} GDataYouTubeServiceClass;

GType gdata_youtube_service_get_type (void) G_GNUC_CONST;
GQuark gdata_youtube_service_error_quark (void) G_GNUC_CONST;

GDataYouTubeService *gdata_youtube_service_new (const gchar *developer_key, const gchar *client_id) G_GNUC_WARN_UNUSED_RESULT;

GDataFeed *gdata_youtube_service_query_standard_feed (GDataYouTubeService *self, GDataYouTubeStandardFeedType feed_type, GDataQuery *query,
						      GCancellable *cancellable,
						      GDataQueryProgressCallback progress_callback, gpointer progress_user_data,
						      GError **error) G_GNUC_WARN_UNUSED_RESULT;
void gdata_youtube_service_query_standard_feed_async (GDataYouTubeService *self, GDataYouTubeStandardFeedType feed_type, GDataQuery *query,
						      GCancellable *cancellable,
						      GDataQueryProgressCallback progress_callback, gpointer progress_user_data,
						      GAsyncReadyCallback callback, gpointer user_data);

GDataFeed *gdata_youtube_service_query_videos (GDataYouTubeService *self, GDataQuery *query,
					       GCancellable *cancellable, GDataQueryProgressCallback progress_callback, gpointer progress_user_data,
					       GError **error) G_GNUC_WARN_UNUSED_RESULT;
void gdata_youtube_service_query_videos_async (GDataYouTubeService *self, GDataQuery *query,
					       GCancellable *cancellable, GDataQueryProgressCallback progress_callback, gpointer progress_user_data,
					       GAsyncReadyCallback callback, gpointer user_data);

GDataYouTubeVideo *gdata_youtube_service_query_single_video (GDataYouTubeService *self, GDataQuery *query, const gchar *video_id,
							     GCancellable *cancellable, GError **error) G_GNUC_WARN_UNUSED_RESULT;
void gdata_youtube_service_query_single_video_async (GDataYouTubeService *self, GDataQuery *query, const gchar *video_id,
						     GCancellable *cancellable, GAsyncReadyCallback callback, gpointer user_data);
GDataYouTubeVideo *gdata_youtube_service_query_single_video_finish (GDataYouTubeService *self, GAsyncResult *async_result,
								    GError **error);

GDataFeed *gdata_youtube_service_query_related (GDataYouTubeService *self, GDataYouTubeVideo *video, GDataQuery *query,
						GCancellable *cancellable, GDataQueryProgressCallback progress_callback, gpointer progress_user_data,
						GError **error) G_GNUC_WARN_UNUSED_RESULT;
void gdata_youtube_service_query_related_async (GDataYouTubeService *self, GDataYouTubeVideo *video, GDataQuery *query,
						GCancellable *cancellable, GDataQueryProgressCallback progress_callback, gpointer progress_user_data,
						GAsyncReadyCallback callback, gpointer user_data);

GDataYouTubeVideo *gdata_youtube_service_upload_video (GDataYouTubeService *self, GDataYouTubeVideo *video, GFile *video_file,
						       GCancellable *cancellable, GError **error) G_GNUC_WARN_UNUSED_RESULT;

const gchar *gdata_youtube_service_get_developer_key (GDataYouTubeService *self);
const gchar *gdata_youtube_service_get_youtube_user (GDataYouTubeService *self);

G_END_DECLS

#endif /* !GDATA_YOUTUBE_SERVICE_H */
