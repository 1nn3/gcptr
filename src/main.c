/* GCPtr
 * Copyright (C) 2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>
 *
 * See http://github.com/1nn3/gcptr
 */

#include "main.h"

static gint monitor_num = -1;
static gint x = -1;
static gint y = -1;
static gboolean print_info_and_exit = FALSE;
static gboolean print_primary_monitor_and_exit = FALSE;

static GOptionEntry entries[] =
{
	{ "info", 'i', G_OPTION_FLAG_NONE, G_OPTION_ARG_NONE, &print_info_and_exit, "Print information and exit", '\0'},
	{ "monitor", 'm', G_OPTION_FLAG_NONE, G_OPTION_ARG_INT, &monitor_num, "Monitor number", "NUM"},
	{ "x", 'x', G_OPTION_FLAG_NONE, G_OPTION_ARG_INT, &x, "Alternative X coordinate", "X"},
	{ "y", 'y', G_OPTION_FLAG_NONE, G_OPTION_ARG_INT, &y, "Alternative Y coordinate", "Y"},
	{ NULL }
};

int main (argc, argv)
	int argc;
	char **argv;
{
	GdkDevice* pointer;
	GdkDisplay* display;
	GdkDisplayManager* display_manager;
	GdkMonitor* monitor;
	GdkRectangle rectangle;
	GdkScreen* screen;
	GdkSeat* seat;
	GError* error;
	gint n_monitors;
	gint primary_monitor_num;
	GOptionContext* option_context;

	/* Initialise */
	gdk_init (&argc, &argv);
	display_manager = gdk_display_manager_get ();
	display = gdk_display_manager_get_default_display (display_manager);
	seat = gdk_display_get_default_seat (display);
	n_monitors = gdk_display_get_n_monitors (display);
	screen = gdk_display_get_default_screen (display);
	pointer = gdk_seat_get_pointer (seat);

	/* Parse the arguments */
	error = NULL;
	option_context = g_option_context_new ('\0');
	g_option_context_add_main_entries (option_context, entries, '\0');
	g_option_context_add_group (option_context, gtk_get_option_group (TRUE));
	if (!g_option_context_parse (option_context, &argc, &argv, &error)) {
		g_critical ("Option parsing failed: %s", error->message);
		exit (EXIT_FAILURE);
	}
	g_clear_error (&error); /* g_error_free */
	g_option_context_free(option_context);

	/* Set primary monitor */
	/* monitor = gdk_display_get_primary_monitor (display); */
	primary_monitor_num = -1;
	for (int monitor_num = 0; monitor_num < n_monitors; monitor_num++) {
		if (gdk_monitor_is_primary (gdk_display_get_monitor (display, monitor_num))) {
			primary_monitor_num = monitor_num;
			break;
		}
	}
	monitor = gdk_display_get_monitor (display, primary_monitor_num);

	/* Set user defined monitor */
	if (monitor_num != -1) {
		/* Check for valid monitor */
		if (monitor_num < 0 || monitor_num >= n_monitors) {
			g_critical (
				"Invalid monitor number: %d\n"
				"Number of monitors: %d\n",
				monitor_num,
				n_monitors);
			exit (EXIT_FAILURE);
		}

		monitor = gdk_display_get_monitor (display, monitor_num);
	}

	/* Check pointer follows device motion */
	if (!gdk_device_get_has_cursor (pointer)) {
		g_warning (
			"Pointer don't follows device motion: %s\n",
			gdk_device_get_name (pointer));
	}

	/* Calculate new position
	 * Use centre if no alternative coordinates are given
	 */
	gdk_monitor_get_geometry (monitor, &rectangle);

	x = (x != -1)? x : rectangle.width / 2;
	y = (y != -1)? y : rectangle.height / 2;

	if (x < 0 || x > rectangle.width || y < 0 || y > rectangle.height) {
		g_critical ("Coordinates out of range: X=%d Y=%d\n", x, y);
		exit (EXIT_FAILURE);
	}

	/* Add offset to position */
	x += rectangle.x;
	y += rectangle.y;

	/* Print information and exit */
	if (print_info_and_exit) {
		g_print (
			"%s\n"
			"Width: %d\n"
			"Height: %d\n"
			"Name of monitor plug: %s\n"
			"Name of display: %s\n"
			"Name of device: %s\n"
			"Number of monitors: %d\n"
			"Primary monitor: %d\n",
			PACKAGE_STRING,
			rectangle.width,
			rectangle.height,
			gdk_monitor_get_model (monitor),
			gdk_display_get_name (display),
			gdk_device_get_name (pointer),
			n_monitors,
			primary_monitor_num);
		exit (EXIT_SUCCESS);
	}

	/* Move the pointer */
	gdk_device_warp (pointer, screen, x, y);
	gdk_display_sync (display);

	exit (EXIT_SUCCESS);
}

