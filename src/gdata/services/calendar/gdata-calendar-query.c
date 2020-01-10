/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * GData Client
 * Copyright (C) Philip Withnall 2009–2010 <philip@tecnocode.co.uk>
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
 * SECTION:gdata-calendar-query
 * @short_description: GData Calendar query object
 * @stability: Unstable
 * @include: gdata/services/calendar/gdata-calendar-query.h
 *
 * #GDataCalendarQuery represents a collection of query parameters specific to the Google Calendar service, which go above and beyond
 * those catered for by #GDataQuery.
 *
 * For more information on the custom GData query parameters supported by #GDataCalendarQuery, see the <ulink type="http"
 * url="http://code.google.com/apis/calendar/docs/2.0/reference.html#Parameters">online documentation</ulink>.
 *
 * <example>
 * 	<title>Querying for Events</title>
 * 	<programlisting>
 *	GDataCalendarService *service;
 *	GDataCalendarCalendar *calendar;
 *	GDataCalendarQuery *query;
 *	GDataFeed *feed;
 *	GTimeVal current_time;
 *	GList *i;
 *	GError *error = NULL;
 *
 *	/<!-- -->* Create a service and find a calendar to query in *<!-- -->/
 *	service = create_calendar_service ();
 *	calendar = get_calendar (service);
 *
 *	/<!-- -->* Create the query to use. We're going to query for events within the next week which match the search term "party",
 *	 * ordered by last modification time (descending). *<!-- -->/
 *	g_get_current_time (&current_time);
 *	query = gdata_calendar_query_new_with_limits ("party", current_time.tv_sec, current_time.tv_sec + 7 * 24 * 60 * 60);
 *	gdata_calendar_query_set_order_by (query, "lastmodified");
 *	gdata_calendar_query_set_sort_order (query, "descending");
 *
 *	/<!-- -->* Execute the query *<!-- -->/
 *	feed = gdata_calendar_service_query_events (service, calendar, query, NULL, NULL, NULL, &error);
 *
 *	g_object_unref (query);
 *	g_object_unref (calendar);
 *	g_object_unref (service);
 *
 *	if (error != NULL) {
 *		g_error ("Error querying for events: %s", error->message);
 *		g_error_free (error);
 *		return;
 *	}
 *
 *	/<!-- -->* Iterate through the returned events and do something with them *<!-- -->/
 *	for (i = gdata_feed_get_entries (feed); i != NULL; i = i->next) {
 *		GDataCalendarEvent *event = GDATA_CALENDAR_EVENT (i->data);
 *
 *		/<!-- -->* Do something with the event here, such as insert it into a UI *<!-- -->/
 *	}
 *
 *	g_object_unref (feed);
 * 	</programlisting>
 * </example>
 **/

#include <config.h>
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <string.h>

#include "gdata-calendar-query.h"
#include "gdata-query.h"
#include "gdata-parser.h"

static void gdata_calendar_query_finalize (GObject *object);
static void gdata_calendar_query_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);
static void gdata_calendar_query_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void get_query_uri (GDataQuery *self, const gchar *feed_uri, GString *query_uri, gboolean *params_started);

struct _GDataCalendarQueryPrivate {
	gboolean future_events;
	gchar *order_by; /* TODO: #defined values */
	gint64 recurrence_expansion_start;
	gint64 recurrence_expansion_end;
	gboolean single_events;
	gchar *sort_order; /* TODO: #defined values */
	gint64 start_min;
	gint64 start_max;
	gchar *timezone;
	guint max_attendees;
	gboolean show_deleted;
};

enum {
	PROP_FUTURE_EVENTS = 1,
	PROP_ORDER_BY,
	PROP_RECURRENCE_EXPANSION_START,
	PROP_RECURRENCE_EXPANSION_END,
	PROP_SINGLE_EVENTS,
	PROP_SORT_ORDER,
	PROP_START_MIN,
	PROP_START_MAX,
	PROP_TIMEZONE,
	PROP_MAX_ATTENDEES,
	PROP_SHOW_DELETED,
};

G_DEFINE_TYPE (GDataCalendarQuery, gdata_calendar_query, GDATA_TYPE_QUERY)

static void
gdata_calendar_query_class_init (GDataCalendarQueryClass *klass)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	GDataQueryClass *query_class = GDATA_QUERY_CLASS (klass);

	g_type_class_add_private (klass, sizeof (GDataCalendarQueryPrivate));

	gobject_class->set_property = gdata_calendar_query_set_property;
	gobject_class->get_property = gdata_calendar_query_get_property;
	gobject_class->finalize = gdata_calendar_query_finalize;

	query_class->get_query_uri = get_query_uri;

	/**
	 * GDataCalendarQuery:future-events:
	 *
	 * A shortcut to request all events scheduled for the future. Overrides the
	 * #GDataCalendarQuery:recurrence-expansion-start, #GDataCalendarQuery:recurrence-expansion-end,
	 * #GDataCalendarQuery:start-min and #GDataCalendarQuery:start-max properties.
	 **/
	g_object_class_install_property (gobject_class, PROP_FUTURE_EVENTS,
	                                 g_param_spec_boolean ("future-events",
	                                                       "Future events?", "A shortcut to request all events scheduled for the future.",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:order-by:
	 *
	 * Specifies order of entries in a feed. Supported values are <literal>lastmodified</literal> and
	 * <literal>starttime</literal>.
	 **/
	g_object_class_install_property (gobject_class, PROP_ORDER_BY,
	                                 g_param_spec_string ("order-by",
	                                                      "Order by", "Specifies order of entries in a feed.",
	                                                      NULL,
	                                                      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:recurrence-expansion-start:
	 *
	 * Specifies the beginning of the time period to expand recurring events for, inclusive.
	 **/
	g_object_class_install_property (gobject_class, PROP_RECURRENCE_EXPANSION_START,
	                                 g_param_spec_int64 ("recurrence-expansion-start",
	                                                     "Recurrence expansion start", "Specifies start of period to expand recurrences for.",
	                                                     -1, G_MAXINT64, -1,
	                                                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:recurrence-expansion-end:
	 *
	 * Specifies the end of the time period to expand recurring events for, exclusive.
	 **/
	g_object_class_install_property (gobject_class, PROP_RECURRENCE_EXPANSION_END,
	                                 g_param_spec_int64 ("recurrence-expansion-end",
	                                                     "Recurrence expansion end", "Specifies end of period to expand recurrences for.",
	                                                     -1, G_MAXINT64, -1,
	                                                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:single-events:
	 *
	 * Indicates whether recurring events should be expanded or represented as a single event.
	 **/
	g_object_class_install_property (gobject_class, PROP_SINGLE_EVENTS,
	                                 g_param_spec_boolean ("single-events",
	                                                       "Single events?", "Indicates whether recurring events should be expanded.",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:sort-order:
	 *
	 * Specifies direction of sorting. Supported values are <literal>ascending</literal> and
	 * <literal>descending</literal>.
	 **/
	g_object_class_install_property (gobject_class, PROP_SORT_ORDER,
	                                 g_param_spec_string ("sort-order",
	                                                      "Sort order", "Specifies direction of sorting.",
	                                                      NULL,
	                                                      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:start-min:
	 *
	 * Together with #GDataCalendarQuery:start-max, creates a timespan such that only events within the timespan are returned.
	 *
	 * #GDataCalendarQuery:start-min is inclusive, while #GDataCalendarQuery:start-max is exclusive. Events that overlap the range are
	 * included.
	 *
	 * If not specified, the default #GDataCalendarQuery:start-min is <literal>1970-01-01</literal>.
	 **/
	g_object_class_install_property (gobject_class, PROP_START_MIN,
	                                 g_param_spec_int64 ("start-min",
	                                                     "Start min", "A timespan such that only events within the timespan are returned.",
	                                                     -1, G_MAXINT64, -1,
	                                                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:start-max:
	 *
	 * Together with #GDataCalendarQuery:start-min, creates a timespan such that only events within the timespan are returned
	 *
	 * #GDataCalendarQuery:start-min is inclusive, while #GDataCalendarQuery:start-max is exclusive. Events that overlap the range are
	 * included.
	 *
	 * If not specified, the default #GDataCalendarQuery:start-max is <literal>2031-01-01</literal>.
	 **/
	g_object_class_install_property (gobject_class, PROP_START_MAX,
	                                 g_param_spec_int64 ("start-max",
	                                                     "Start max", "A timespan such that only events within the timespan are returned.",
	                                                     -1, G_MAXINT64, -1,
	                                                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:timezone:
	 *
	 * The current timezone. If this is not specified, all times are returned in UTC.
	 *
	 * Since: 0.2.0
	 **/
	g_object_class_install_property (gobject_class, PROP_TIMEZONE,
	                                 g_param_spec_string ("timezone",
	                                                      "Timezone", "The current timezone.",
	                                                      NULL,
	                                                      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:max-attendees:
	 *
	 * Specifies the maximum number of attendees to list for an event. If the actual number of attendees for an event is greater than this value,
	 * only the current user and the event organiser are listed.
	 *
	 * Since: 0.9.1
	 */
	g_object_class_install_property (gobject_class, PROP_MAX_ATTENDEES,
	                                 g_param_spec_uint ("max-attendees",
	                                                    "Max attendee count", "Specifies the maximum number of attendees to list for an event.",
	                                                    0, G_MAXUINT, 0,
	                                                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

	/**
	 * GDataCalendarQuery:show-deleted:
	 *
	 * Whether to include deleted/cancelled events in the query feed. Deleted events have their #GDataCalendarEvent:status property set to
	 * %GDATA_GD_EVENT_STATUS_CANCELED. They do not normally appear in query results.
	 *
	 * Since: 0.9.1
	 */
	g_object_class_install_property (gobject_class, PROP_SHOW_DELETED,
	                                 g_param_spec_boolean ("show-deleted",
	                                                       "Show deleted?", "Whether to include deleted/cancelled events in the query feed.",
	                                                       FALSE,
	                                                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
}

static void
gdata_calendar_query_init (GDataCalendarQuery *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, GDATA_TYPE_CALENDAR_QUERY, GDataCalendarQueryPrivate);
	self->priv->recurrence_expansion_start = -1;
	self->priv->recurrence_expansion_end = -1;
	self->priv->start_min = -1;
	self->priv->start_max = -1;
}

static void
gdata_calendar_query_finalize (GObject *object)
{
	GDataCalendarQueryPrivate *priv = GDATA_CALENDAR_QUERY (object)->priv;

	g_free (priv->order_by);
	g_free (priv->sort_order);
	g_free (priv->timezone);

	/* Chain up to the parent class */
	G_OBJECT_CLASS (gdata_calendar_query_parent_class)->finalize (object);
}

static void
gdata_calendar_query_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
	GDataCalendarQueryPrivate *priv = GDATA_CALENDAR_QUERY (object)->priv;

	switch (property_id) {
		case PROP_FUTURE_EVENTS:
			g_value_set_boolean (value, priv->future_events);
			break;
		case PROP_ORDER_BY:
			g_value_set_string (value, priv->order_by);
			break;
		case PROP_RECURRENCE_EXPANSION_START:
			g_value_set_int64 (value, priv->recurrence_expansion_start);
			break;
		case PROP_RECURRENCE_EXPANSION_END:
			g_value_set_int64 (value, priv->recurrence_expansion_end);
			break;
		case PROP_SINGLE_EVENTS:
			g_value_set_boolean (value, priv->single_events);
			break;
		case PROP_SORT_ORDER:
			g_value_set_string (value, priv->sort_order);
			break;
		case PROP_START_MIN:
			g_value_set_int64 (value, priv->start_min);
			break;
		case PROP_START_MAX:
			g_value_set_int64 (value, priv->start_max);
			break;
		case PROP_TIMEZONE:
			g_value_set_string (value, priv->timezone);
			break;
		case PROP_MAX_ATTENDEES:
			g_value_set_uint (value, priv->max_attendees);
			break;
		case PROP_SHOW_DELETED:
			g_value_set_boolean (value, priv->show_deleted);
			break;
		default:
			/* We don't have any other property... */
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}

static void
gdata_calendar_query_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
	GDataCalendarQuery *self = GDATA_CALENDAR_QUERY (object);

	switch (property_id) {
		case PROP_FUTURE_EVENTS:
			gdata_calendar_query_set_future_events (self, g_value_get_boolean (value));
			break;
		case PROP_ORDER_BY:
			gdata_calendar_query_set_order_by (self, g_value_get_string (value));
			break;
		case PROP_RECURRENCE_EXPANSION_START:
			gdata_calendar_query_set_recurrence_expansion_start (self, g_value_get_int64 (value));
			break;
		case PROP_RECURRENCE_EXPANSION_END:
			gdata_calendar_query_set_recurrence_expansion_end (self, g_value_get_int64 (value));
			break;
		case PROP_SINGLE_EVENTS:
			gdata_calendar_query_set_single_events (self, g_value_get_boolean (value));
			break;
		case PROP_SORT_ORDER:
			gdata_calendar_query_set_sort_order (self, g_value_get_string (value));
			break;
		case PROP_START_MIN:
			gdata_calendar_query_set_start_min (self, g_value_get_int64 (value));
			break;
		case PROP_START_MAX:
			gdata_calendar_query_set_start_max (self, g_value_get_int64 (value));
			break;
		case PROP_TIMEZONE:
			gdata_calendar_query_set_timezone (self, g_value_get_string (value));
			break;
		case PROP_MAX_ATTENDEES:
			gdata_calendar_query_set_max_attendees (self, g_value_get_uint (value));
			break;
		case PROP_SHOW_DELETED:
			gdata_calendar_query_set_show_deleted (self, g_value_get_boolean (value));
			break;
		default:
			/* We don't have any other property... */
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
			break;
	}
}

static void
get_query_uri (GDataQuery *self, const gchar *feed_uri, GString *query_uri, gboolean *params_started)
{
	GDataCalendarQueryPrivate *priv = GDATA_CALENDAR_QUERY (self)->priv;

	#define APPEND_SEP g_string_append_c (query_uri, (*params_started == FALSE) ? '?' : '&'); *params_started = TRUE;

	/* Chain up to the parent class */
	GDATA_QUERY_CLASS (gdata_calendar_query_parent_class)->get_query_uri (self, feed_uri, query_uri, params_started);

	APPEND_SEP
	if (priv->future_events == TRUE)
		g_string_append (query_uri, "futureevents=true");
	else
		g_string_append (query_uri, "futureevents=false");

	if (priv->order_by != NULL) {
		APPEND_SEP
		g_string_append (query_uri, "orderby=");
		g_string_append_uri_escaped (query_uri, priv->order_by, NULL, FALSE);
	}

	if (priv->recurrence_expansion_start != -1) {
		gchar *recurrence_expansion_start;

		APPEND_SEP
		g_string_append (query_uri, "recurrence-expansion-start=");
		recurrence_expansion_start = gdata_parser_int64_to_iso8601 (priv->recurrence_expansion_start);
		g_string_append (query_uri, recurrence_expansion_start);
		g_free (recurrence_expansion_start);
	}

	if (priv->recurrence_expansion_end != -1) {
		gchar *recurrence_expansion_end;

		APPEND_SEP
		g_string_append (query_uri, "recurrence-expansion-end=");
		recurrence_expansion_end = gdata_parser_int64_to_iso8601 (priv->recurrence_expansion_end);
		g_string_append (query_uri, recurrence_expansion_end);
		g_free (recurrence_expansion_end);
	}

	APPEND_SEP
	if (priv->single_events == TRUE)
		g_string_append (query_uri, "singleevents=true");
	else
		g_string_append (query_uri, "singleevents=false");

	if (priv->sort_order != NULL) {
		APPEND_SEP
		g_string_append (query_uri, "sortorder=");
		g_string_append_uri_escaped (query_uri, priv->sort_order, NULL, FALSE);
	}

	if (priv->start_min != -1) {
		gchar *start_min;

		APPEND_SEP
		g_string_append (query_uri, "start-min=");
		start_min = gdata_parser_int64_to_iso8601 (priv->start_min);
		g_string_append (query_uri, start_min);
		g_free (start_min);
	}

	if (priv->start_max != -1) {
		gchar *start_max;

		APPEND_SEP
		g_string_append (query_uri, "start-max=");
		start_max = gdata_parser_int64_to_iso8601 (priv->start_max);
		g_string_append (query_uri, start_max);
		g_free (start_max);
	}

	if (priv->timezone != NULL) {
		APPEND_SEP
		g_string_append (query_uri, "ctz=");
		g_string_append_uri_escaped (query_uri, priv->timezone, NULL, FALSE);
	}

	if (priv->max_attendees > 0) {
		APPEND_SEP
		g_string_append_printf (query_uri, "max-attendees=%u", priv->max_attendees);
	}

	APPEND_SEP
	if (priv->show_deleted == TRUE) {
		g_string_append (query_uri, "showdeleted=true");
	} else {
		g_string_append (query_uri, "showdeleted=false");
	}
}

/**
 * gdata_calendar_query_new:
 * @q: (allow-none): a query string, or %NULL
 *
 * Creates a new #GDataCalendarQuery with its #GDataQuery:q property set to @q.
 *
 * Return value: a new #GDataCalendarQuery
 **/
GDataCalendarQuery *
gdata_calendar_query_new (const gchar *q)
{
	return g_object_new (GDATA_TYPE_CALENDAR_QUERY, "q", q, NULL);
}

/**
 * gdata_calendar_query_new_with_limits:
 * @q: (allow-none): a query string, or %NULL
 * @start_min: (allow-none): a starting time for the event period, or %NULL
 * @start_max: (allow-none): an ending time for the event period, or %NULL
 *
 * Creates a new #GDataCalendarQuery with its #GDataQuery:q property set to @q, and the time limits @start_min and @start_max
 * applied.
 *
 * Return value: a new #GDataCalendarQuery
 **/
GDataCalendarQuery *
gdata_calendar_query_new_with_limits (const gchar *q, gint64 start_min, gint64 start_max)
{
	return g_object_new (GDATA_TYPE_CALENDAR_QUERY,
	                     "q", q,
	                     "start-min", start_min,
	                     "start-max", start_max,
	                     NULL);
}

/**
 * gdata_calendar_query_get_future_events:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:future-events property.
 *
 * Return value: the future events property
 **/
gboolean
gdata_calendar_query_get_future_events (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), FALSE);
	return self->priv->future_events;
}

/**
 * gdata_calendar_query_set_future_events:
 * @self: a #GDataCalendarQuery
 * @future_events: %TRUE to unconditionally show future events, %FALSE otherwise
 *
 * Sets the #GDataCalendarQuery:future-events property of the #GDataCalendarQuery to @future_events.
 **/
void
gdata_calendar_query_set_future_events (GDataCalendarQuery *self, gboolean future_events)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));
	self->priv->future_events = future_events;
	g_object_notify (G_OBJECT (self), "future-events");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_order_by:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:order-by property.
 *
 * Return value: the order by property, or %NULL if it is unset
 **/
const gchar *
gdata_calendar_query_get_order_by (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), NULL);
	return self->priv->order_by;
}

/**
 * gdata_calendar_query_set_order_by:
 * @self: a #GDataCalendarQuery
 * @order_by: (allow-none): a new order by string, or %NULL
 *
 * Sets the #GDataCalendarQuery:order-by property of the #GDataCalendarQuery to the new order by string, @order_by.
 *
 * Set @order_by to %NULL to unset the property in the query URI.
 **/
void
gdata_calendar_query_set_order_by (GDataCalendarQuery *self, const gchar *order_by)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));

	g_free (self->priv->order_by);
	self->priv->order_by = g_strdup (order_by);
	g_object_notify (G_OBJECT (self), "order-by");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_recurrence_expansion_start:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:recurrence-expansion-start property. If the property is unset, <code class="literal">-1</code> will be returned.
 *
 * Return value: the UNIX timestamp for the recurrence-expansion-start property, or <code class="literal">-1</code>
 **/
gint64
gdata_calendar_query_get_recurrence_expansion_start (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), -1);
	return self->priv->recurrence_expansion_start;
}

/**
 * gdata_calendar_query_set_recurrence_expansion_start:
 * @self: a #GDataCalendarQuery
 * @start: a new start time, or <code class="literal">-1</code>
 *
 * Sets the #GDataCalendarQuery:recurrence-expansion-start property of the #GDataCalendarQuery
 * to the new time/date, @start.
 *
 * Set @start to <code class="literal">-1</code> to unset the property in the query URI.
 **/
void
gdata_calendar_query_set_recurrence_expansion_start (GDataCalendarQuery *self, gint64 start)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));
	g_return_if_fail (start >= -1);

	self->priv->recurrence_expansion_start = start;
	g_object_notify (G_OBJECT (self), "recurrence-expansion-start");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_recurrence_expansion_end:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:recurrence-expansion-end property. If the property is unset, <code class="literal">-1</code> will be returned.
 *
 * Return value: the UNIX timestamp for the recurrence-expansion-end property, or <code class="literal">-1</code>
 **/
gint64
gdata_calendar_query_get_recurrence_expansion_end (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), -1	);
	return self->priv->recurrence_expansion_end;
}

/**
 * gdata_calendar_query_set_recurrence_expansion_end:
 * @self: a #GDataCalendarQuery
 * @end: a new end time, or <code class="literal">-1</code>
 *
 * Sets the #GDataCalendarQuery:recurrence-expansion-end property of the #GDataCalendarQuery
 * to the new time/date, @end.
 *
 * Set @end to <code class="literal">-1</code> to unset the property in the query URI.
 **/
void
gdata_calendar_query_set_recurrence_expansion_end (GDataCalendarQuery *self, gint64 end)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));
	g_return_if_fail (end >= -1);

	self->priv->recurrence_expansion_end = end;
	g_object_notify (G_OBJECT (self), "recurrence-expansion-end");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_single_events:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:single-events property.
 *
 * Return value: the single events property
 **/
gboolean
gdata_calendar_query_get_single_events (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), FALSE);
	return self->priv->single_events;
}

/**
 * gdata_calendar_query_set_single_events:
 * @self: a #GDataCalendarQuery
 * @single_events: %TRUE to show recurring events as single events, %FALSE otherwise
 *
 * Sets the #GDataCalendarQuery:single-events property of the #GDataCalendarQuery to @single_events.
 **/
void
gdata_calendar_query_set_single_events (GDataCalendarQuery *self, gboolean single_events)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));
	self->priv->single_events = single_events;
	g_object_notify (G_OBJECT (self), "single-events");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_sort_order:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:sort-order property.
 *
 * Return value: the sort order property, or %NULL if it is unset
 **/
const gchar *
gdata_calendar_query_get_sort_order (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), NULL);
	return self->priv->sort_order;
}

/**
 * gdata_calendar_query_set_sort_order:
 * @self: a #GDataCalendarQuery
 * @sort_order: (allow-none): a new sort order string, or %NULL
 *
 * Sets the #GDataCalendarQuery:sort-order property of the #GDataCalendarQuery to the new sort order string, @sort_order.
 *
 * Set @sort_order to %NULL to unset the property in the query URI.
 **/
void
gdata_calendar_query_set_sort_order (GDataCalendarQuery *self, const gchar *sort_order)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));

	g_free (self->priv->sort_order);
	self->priv->sort_order = g_strdup (sort_order);
	g_object_notify (G_OBJECT (self), "sort-order");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_start_min:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:start-min property. If the property is unset, <code class="literal">-1</code> will be returned.
 *
 * Return value: the UNIX timestamp for the start-min property, or <code class="literal">-1</code>
 **/
gint64
gdata_calendar_query_get_start_min (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), -1);
	return self->priv->start_min;
}

/**
 * gdata_calendar_query_set_start_min:
 * @self: a #GDataCalendarQuery
 * @start_min: a new minimum start time, or <code class="literal">-1</code>
 *
 * Sets the #GDataCalendarQuery:start-min property of the #GDataCalendarQuery
 * to the new time/date, @start_min.
 *
 * Set @start_min to <code class="literal">-1</code> to unset the property in the query URI.
 **/
void
gdata_calendar_query_set_start_min (GDataCalendarQuery *self, gint64 start_min)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));
	g_return_if_fail (start_min >= -1);

	self->priv->start_min = start_min;
	g_object_notify (G_OBJECT (self), "start-min");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_start_max:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:start-max property. If the property is unset, <code class="literal">-1</code> will be returned.
 *
 * Return value: the UNIX timestamp for the start-max property, or <code class="literal">-1</code>
 **/
gint64
gdata_calendar_query_get_start_max (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), -1);
	return self->priv->start_max;
}

/**
 * gdata_calendar_query_set_start_max:
 * @self: a #GDataCalendarQuery
 * @start_max: a new maximum start time, or <code class="literal">-1</code>
 *
 * Sets the #GDataCalendarQuery:start-max property of the #GDataCalendarQuery
 * to the new time/date, @start_max.
 *
 * Set @start_max to <code class="literal">-1</code> to unset the property in the query URI.
 **/
void
gdata_calendar_query_set_start_max (GDataCalendarQuery *self, gint64 start_max)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));
	g_return_if_fail (start_max >= -1);

	self->priv->start_max = start_max;
	g_object_notify (G_OBJECT (self), "start-max");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_timezone:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:timezone property.
 *
 * Return value: the timezone property, or %NULL if it is unset
 *
 * Since: 0.2.0
 **/
const gchar *
gdata_calendar_query_get_timezone (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), NULL);
	return self->priv->timezone;
}

/**
 * gdata_calendar_query_set_timezone:
 * @self: a #GDataCalendarQuery
 * @_timezone: (allow-none): a new timezone string, or %NULL
 *
 * Sets the #GDataCalendarQuery:timezone property of the #GDataCalendarQuery to the new timezone string, @timezone.
 *
 * Set @timezone to %NULL to unset the property in the query URI.
 *
 * Since: 0.2.0
 **/
void
gdata_calendar_query_set_timezone (GDataCalendarQuery *self, const gchar *_timezone)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));

	g_free (self->priv->timezone);

	/* Replace all spaces with underscores */
	if (_timezone != NULL) {
		gchar *zone, *i;

		zone = g_strdup (_timezone);
		for (i = zone; *i != '\0'; i++) {
			if (*i == ' ')
				*i = '_';
		}
		self->priv->timezone = zone;
	} else {
		self->priv->timezone = NULL;
	}

	g_object_notify (G_OBJECT (self), "timezone");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_get_max_attendees:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:max-attendees property. If the property is unset, <code class="literal">0</code> will be returned.
 *
 * Return value: the maximum number of attendees, or <code class="literal">0</code>
 *
 * Since: 0.9.1
 */
guint
gdata_calendar_query_get_max_attendees (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), 0);
	return self->priv->max_attendees;
}

/**
 * gdata_calendar_query_set_max_attendees:
 * @self: a #GDataCalendarQuery
 * @max_attendees: a new maximum attendee count, or <code class="literal">0</code>
 *
 * Sets the #GDataCalendarQuery:max-attendees property of the #GDataCalendarQuery to the new value, @max_attendees.
 *
 * Set @max_attendees to <code class="literal">0</code> to unset the property in the query URI.
 *
 * Since: 0.9.1
 */
void
gdata_calendar_query_set_max_attendees (GDataCalendarQuery *self, guint max_attendees)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));

	self->priv->max_attendees = max_attendees;
	g_object_notify (G_OBJECT (self), "max-attendees");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}

/**
 * gdata_calendar_query_show_deleted:
 * @self: a #GDataCalendarQuery
 *
 * Gets the #GDataCalendarQuery:show-deleted property.
 *
 * Return value: %TRUE if deleted/cancelled events should be shown, %FALSE otherwise
 *
 * Since: 0.9.1
 */
gboolean
gdata_calendar_query_show_deleted (GDataCalendarQuery *self)
{
	g_return_val_if_fail (GDATA_IS_CALENDAR_QUERY (self), FALSE);
	return self->priv->show_deleted;
}

/**
 * gdata_calendar_query_set_show_deleted:
 * @self: a #GDataCalendarQuery
 * @show_deleted: %TRUE to show deleted events, %FALSE otherwise
 *
 * Sets the #GDataCalendarQuery:show-deleted property of the #GDataCalendarQuery.
 *
 * Since: 0.9.1
 */
void
gdata_calendar_query_set_show_deleted (GDataCalendarQuery *self, gboolean show_deleted)
{
	g_return_if_fail (GDATA_IS_CALENDAR_QUERY (self));

	self->priv->show_deleted = show_deleted;
	g_object_notify (G_OBJECT (self), "show-deleted");

	/* Our current ETag will no longer be relevant */
	gdata_query_set_etag (GDATA_QUERY (self), NULL);
}
