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

#include <config.h>
#include <glib.h>
#include <glib/gi18n-lib.h>
#include <sys/time.h>
#include <time.h>
#include <libxml/parser.h>

#include "gdata-parser.h"
#include "gdata-service.h"

static gchar *
print_element (xmlNode *node)
{
	gboolean node_has_ns = (node->ns == NULL || node->ns->prefix == NULL ||
				xmlStrcmp (node->ns->href, (xmlChar*) "http://www.w3.org/2005/Atom") == 0) ? FALSE : TRUE;

	if (node->parent == NULL) {
		/* No parent node */
		if (node_has_ns == TRUE)
			return g_strdup_printf ("<%s:%s>", node->ns->prefix, node->name);
		else
			return g_strdup_printf ("<%s>", node->name);
	} else {
		/* We have a parent node, which makes things a lot more complex */
		gboolean parent_has_ns = (node->parent->type == XML_DOCUMENT_NODE || node->parent->ns == NULL || node->parent->ns->prefix == NULL ||
					  xmlStrcmp (node->parent->ns->href, (xmlChar*) "http://www.w3.org/2005/Atom") == 0) ? FALSE : TRUE;

		if (parent_has_ns == TRUE && node_has_ns == TRUE)
			return g_strdup_printf ("<%s:%s/%s:%s>", node->parent->ns->prefix, node->parent->name, node->ns->prefix, node->name);
		else if (parent_has_ns == FALSE && node_has_ns == TRUE)
			return g_strdup_printf ("<%s/%s:%s>", node->parent->name, node->ns->prefix, node->name);
		else
			return g_strdup_printf ("<%s/%s>", node->parent->name, node->name);
	}
}

gboolean
gdata_parser_error_required_content_missing (xmlNode *element, GError **error)
{
	gchar *element_string = print_element (element);

	/* Translators: the parameter is the name of an XML element, including the angle brackets ("<" and ">").
	 *
	 * For example:
	 *  A <title> element was missing required content. */
	g_set_error (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_PROTOCOL_ERROR, _("A %s element was missing required content."), element_string);
	g_free (element_string);

	return FALSE;
}

gboolean
gdata_parser_error_not_iso8601_format (xmlNode *element, const gchar *actual_value, GError **error)
{
	gchar *element_string = print_element (element);
	g_set_error (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_PROTOCOL_ERROR,
		     /* Translators: the first parameter is the name of an XML element (including the angle brackets ("<" and ">")), and the second parameter is
		      * the erroneous value (which was not in ISO 8601 format).
		      *
		      * For example:
		      *  The content of a <media:group/media:uploaded> element ("2009-05-06 26:30Z") was not in ISO 8601 format. */
		     _("The content of a %s element (\"%s\") was not in ISO 8601 format."), element_string, actual_value);
	g_free (element_string);

	return FALSE;
}

gboolean
gdata_parser_error_unknown_property_value (xmlNode *element, const gchar *property_name, const gchar *actual_value, GError **error)
{
	gchar *property_string, *element_string;

	property_string = g_strdup_printf ("@%s", property_name);
	element_string = print_element (element);

	g_set_error (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_PROTOCOL_ERROR,
		     /* Translators: the first parameter is the name of an XML property, the second is the name of an XML element
		      * (including the angle brackets ("<" and ">")) to which the property belongs, and the third is the unknown value.
		      *
		      * For example:
		      *  The value of the @time property of a <media:group/media:thumbnail> element ("00:01:42.500") was unknown. */
		     _("The value of the %s property of a %s element (\"%s\") was unknown."), property_string, element_string, actual_value);
	g_free (property_string);
	g_free (element_string);

	return FALSE;
}

gboolean
gdata_parser_error_unknown_content (xmlNode *element, const gchar *actual_content, GError **error)
{
	gchar *element_string = print_element (element);

	g_set_error (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_PROTOCOL_ERROR,
		     /* Translators: the first parameter is the name of an XML element (including the angle brackets ("<" and ">")),
		      * and the second parameter is the unknown content of that element.
		      *
		      * For example:
		      *  The content of a <gphoto:access> element ("protected") was unknown. */
		     _("The content of a %s element (\"%s\") was unknown."), element_string, actual_content);
	g_free (element_string);

	return FALSE;
}

gboolean
gdata_parser_error_required_property_missing (xmlNode *element, const gchar *property_name, GError **error)
{
	gchar *property_string, *element_string;

	property_string = g_strdup_printf ("@%s", property_name);
	element_string = print_element (element);

	g_set_error (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_PROTOCOL_ERROR,
		     /* Translators: the first parameter is the name of an XML element (including the angle brackets ("<" and ">")),
		      * and the second is the name of an XML property which it should have contained.
		      *
		      * For example:
		      *  A required property of a <entry/gAcl:role> element (@value) was not present. */
		     _("A required property of a %s element (%s) was not present."), element_string, property_string);
	g_free (property_string);
	g_free (element_string);

	return FALSE;
}

gboolean
gdata_parser_error_required_element_missing (const gchar *element_name, const gchar *parent_element_name, GError **error)
{
	/* NOTE: This can't take an xmlNode, since such a node wouldn't exist. */
	gchar *element_string = g_strdup_printf ("<%s/%s>", parent_element_name, element_name);

	/* Translators: the parameter is the name of an XML element, including the angle brackets ("<" and ">").
	 *
	 * For example:
	 *  A required element (<entry/title>) was not present. */
	g_set_error (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_PROTOCOL_ERROR, _("A required element (%s) was not present."), element_string);
	g_free (element_string);

	return FALSE;
}

gboolean
gdata_parser_error_duplicate_element (xmlNode *element, GError **error)
{
	gchar *element_string = print_element (element);

	/* Translators: the parameter is the name of an XML element, including the angle brackets ("<" and ">").
	 *
	 * For example:
	 *  A singleton element (<feed/title>) was duplicated. */
	g_set_error (error, GDATA_SERVICE_ERROR, GDATA_SERVICE_ERROR_PROTOCOL_ERROR, _("A singleton element (%s) was duplicated."), element_string);
	g_free (element_string);

	return FALSE;
}

gboolean
gdata_parser_time_val_from_date (const gchar *date, GTimeVal *_time)
{
	gchar *iso8601_date;
	gboolean success;

	if (strlen (date) != 10 && strlen (date) != 8)
		return FALSE;

	/* Note: This doesn't need translating, as it's outputting an ISO 8601 time string */
	iso8601_date = g_strdup_printf ("%sT00:00:00Z", date);
	success = g_time_val_from_iso8601 (iso8601_date, _time);
	g_free (iso8601_date);

	return success;
}

gchar *
gdata_parser_date_from_time_val (GTimeVal *_time)
{
	time_t secs;
	struct tm *tm;

	secs = _time->tv_sec;
	tm = gmtime (&secs);

	/* Note: This doesn't need translating, as it's outputting an ISO 8601 date string */
	return g_strdup_printf ("%04d-%02d-%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
}

/*
 * gdata_parser_is_namespace:
 * @element: the element to check
 * @namespace_uri: the URI of the desired namespace
 *
 * Checks whether @element is in the namespace identified by @namespace_uri. If @element isn't in a namespace,
 * %FALSE is returned.
 *
 * Return value: %TRUE if @element is in @namespace_uri; %FALSE otherwise
 *
 * Since: 0.6.4
 */
gboolean
gdata_parser_is_namespace (xmlNode *element, const gchar *namespace_uri)
{
	if ((element->ns != NULL && xmlStrcmp (element->ns->href, (const xmlChar*) namespace_uri) == 0) ||
	    (element->ns == NULL && strcmp (namespace_uri, "http://www.w3.org/2005/Atom") == 0))
		return TRUE;
	return FALSE;
}

void
gdata_parser_string_append_escaped (GString *xml_string, const gchar *pre, const gchar *element_content, const gchar *post)
{
	/* Allocate 10 extra bytes when reallocating the GString, to try and avoid having to reallocate again, by assuming
	 * there will be an increase in the length of element_content when escaped of less than 10 characters. */
/*	#define SIZE_FUZZINESS 10*/

/*	guint new_size;*/
	const gchar *p;

	/* Expand xml_string as necessary */
	/* TODO: There is no way to expand the allocation of a GString if you know in advance how much room
	 * lots of append operations are going to require. */
/*	new_size = xml_string->len + strlen (pre) + strlen (element_content) + strlen (post) + SIZE_FUZZINESS;
	if (new_size > xml_string->allocated_len)
		g_string_set_size (xml_string, new_size);*/

	/* Append the pre content */
	if (pre != NULL)
		g_string_append (xml_string, pre);

	/* Loop through the string to be escaped. Code adapted from GLib's g_markup_escape_text() function.
	 *  Copyright 2000, 2003 Red Hat, Inc.
	 *  Copyright 2007, 2008 Ryan Lortie <desrt@desrt.ca>
	 */
	p = element_content;
	while (p != NULL && *p != '\0') {
		const gchar *next = g_utf8_next_char (p);

		switch (*p) {
			case '&':
				g_string_append (xml_string, "&amp;");
				break;
			case '<':
				g_string_append (xml_string, "&lt;");
				break;
			case '>':
				g_string_append (xml_string, "&gt;");
				break;
			case '\'':
				g_string_append (xml_string, "&apos;");
				break;
			case '"':
				g_string_append (xml_string, "&quot;");
				break;
			default: {
				gunichar c = g_utf8_get_char (p);

				if ((0x1 <= c && c <= 0x8) ||
				    (0xb <= c && c  <= 0xc) ||
				    (0xe <= c && c <= 0x1f) ||
				    (0x7f <= c && c <= 0x84) ||
				    (0x86 <= c && c <= 0x9f)) {
					g_string_append_printf (xml_string, "&#x%x;", c);
				} else {
					g_string_append_len (xml_string, p, next - p);
					break;
				}
			}
		}
		p = next;
	}

	/* Append the post content */
	if (post != NULL)
		g_string_append (xml_string, post);
}

gchar *
gdata_parser_utf8_trim_whitespace (const gchar *s)
{
	glong len;
	const gchar *_s;

	/* Skip the leading whitespace */
	while (*s != '\0' && g_unichar_isspace (g_utf8_get_char (s)))
		s = g_utf8_next_char (s);

	/* Find the end of the string and backtrack until we've passed all the whitespace */
	len = g_utf8_strlen (s, -1);
	_s = g_utf8_offset_to_pointer (s, len - 1);
	while (len > 0 && g_unichar_isspace (g_utf8_get_char (_s))) {
		_s = g_utf8_prev_char (_s);
		len--;
	}
	_s = g_utf8_next_char (_s);

	return g_strndup (s, _s - s);
}
