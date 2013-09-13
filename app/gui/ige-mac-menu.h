/* GTK+ Integration for the Mac OS X Menubar.
 *
 * Copyright (C) 2007 Pioneer Research Center USA, Inc.
 * Copyright (C) 2007 Imendio AB
 *
 * For further information, see:
 * http://developer.imendio.com/projects/gtk-macosx/menubar
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; version 2.1
 * of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef __IGE_MAC_MENU_H__
#define __IGE_MAC_MENU_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _IgeMacMenuGroup IgeMacMenuGroup;

void              ige_mac_menu_set_menu_bar       (GtkMenuShell    *menu_shell);
void              ige_mac_menu_set_quit_menu_item (GtkMenuItem     *menu_item);

IgeMacMenuGroup * ige_mac_menu_add_app_menu_group (void);
void              ige_mac_menu_add_app_menu_item  (IgeMacMenuGroup *group,
						   GtkMenuItem     *menu_item,
						   const gchar     *label);

G_END_DECLS

#endif /* __IGE_MAC_MENU_H__ */
