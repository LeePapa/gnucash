/********************************************************************\
 * gnc-gsettings.h -- utility functions for storing/retrieving      *
 *              data in the GSettings database for GnuCash          *
 * Copyright (C) 2013 Geert Janssens <geert@kobaltwit.be>           *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 51 Franklin Street, Fifth Floor    Fax:    +1-617-542-2652       *
 * Boston, MA  02110-1301,  USA       gnu@gnu.org                   *
 *                                                                  *
\********************************************************************/

/** @addtogroup GLib
    @{ */
/** @addtogroup GSettings GSettings Utilities

    The API in this file is designed to make it easy to use the GSettings
    system from within Gnucash.  GSettings is a shared key/value storage
    system.

    The main benefits of these routines are that they
    -# maintain a list of GSettings objects (one per schema),
    -# convert gnucash internal schema names into full gsettings schema id's, and
    -# optionally take care of error checking on return values.

    @{ */
/** @file gnc-gsettings.h
 *  @brief GSettings helper routines.
 *  @author Copyright (C) 2013 Geert Janssens <geert@kobaltwit.be>
 */


#ifndef GNC_GSETIINGS_H
#define GNC_GSETTINGS_H

#include <gio/gio.h>

/* Schema ids used across multiple modules */
#define GSET_SCHEMA_PREFIX            "org.gnucash"
#define GSET_SCHEMA_GENERAL           "general"
#define GSET_SCHEMA_GENERAL_REGISTER  "general.register"
#define GSET_SCHEMA_GENERAL_REPORT    "general.report"
#define GSET_SCHEMA_WARNINGS          "general.warnings"
#define GSET_SCHEMA_WARNINGS_TEMP     "general.warnings.temporary"
#define GSET_SCHEMA_WARNINGS_PERM     "general.warnings.permanent"

/* Keys used across multiple modules */
/* Currently the first one conflicts with same definition in gnc-gconf-utils.h
 * Only load it if gnc-gconf-utils.h isn't loaded yet.
 */
#ifndef GNC_GCONF_UTILS_H
#define DESKTOP_GNOME_INTERFACE "/desktop/gnome/interface"
#endif /* GNC_GCONF_UTILS_H */
#define GSET_KEY_TOOLBAR_STYLE       "toolbar_style"
#define GSET_KEY_SAVE_GEOMETRY       "save_window_geometry"
#define GSET_KEY_LAST_PATH           "last_path"
#define GSET_KEY_USE_NEW             "use_new_window"
#define GSET_KEY_ACCOUNTING_LABELS   "use_accounting_labels"
#define GSET_KEY_ACCOUNT_SEPARATOR   "account_separator"
#define GSET_KEY_NEGATIVE_IN_RED     "negative_in_red"
#define GSET_KEY_NUM_SOURCE          "num_source"
#define GSET_KEY_ENABLE_EURO         "enable_euro"
#define GSET_KEY_DATE_FORMAT         "date_format"
#define GSET_KEY_DATE_COMPLETION     "date_completion"
#define GSET_KEY_DATE_BACKMONTHS     "date_backmonths"
#define GSET_KEY_SHOW_LEAF_ACCT_NAMES "show_leaf_account_names"

/** Convert a partial schema name into a complete gsettings schema name.
 *
 *  This function takes a partial gsettings schema name and converts
 *  it into a fully qualified gsettings schema name.  It does this
 *  by prepending the standard prefix for all gnucash schemas.
 *  If the schema is already fully qualified (i.e. begins with the
 *  default schema prefix, this routine will not change it.
 *
 *  @param name A partial schema name.  The default prefix is
 *  prepended to this name to produce a fully qualified schema
 *  name.
 *
 *  @return This function returns a string pointer to the fully
 *  qualified schema name.  It is the caller's responsibility to
 *  free this string.
 */
gchar *gnc_gsettings_normalize_schema_name (const gchar *name);


/** Set the default gsettings schema prefix. This is
 *  used to generate complete schema id's if only
 *  partial id's are passed.
 */
void gnc_gsettings_set_prefix (const gchar *prefix);

/** Get the default gsettings schema prefix.
 *  If none was set explicitly, this defaults to
 *  "org.gnucash"
 */
const gchar *gnc_gsettings_get_prefix (void);


/** @name Listening for changes
 @{
*/


/** Register a callback for when a specific key in the settings
 *  schema is changed.  Any time the key's value changes, the routine
 *  will be invoked and will be passed both the changed gsettings entry
 *  and the user data passed to this function.
 *
 *  @param schema This value contains the schema name of the key
 *  to watch.
 *
 *  @param key This value contains the name of the key to watch.
 *
 *  @param func This is a pointer to the function to call when the key
 *  changes.
 *
 *  @param user_data This pointer will be passed to the callback
 *  function.
 *
 *  @return This function returns the handler id for the registered
 *  callback.
 */
gulong gnc_gsettings_register_cb (const char *schema,
                                  const gchar *key,
                                  GCallback func,
                                  gpointer user_data);


/** Remove a function that was registered for a callback when a
 *  specific key in the settings schema changed.  Both the func and
 *  user_data arguments are used to match up the callback to remove.
 *  If no matching func and user_data are found to be registered
 *  for the given key, nothing will happen.
 *
 *  @param schema This value contains the schema name of the key
 *  that is being watched.
 *
 *  @param key This value contains the name of the key being watched.
 *
 *  @param func This is a pointer to the function that was registered
 *  earlier.
 *
 *  @param user_data This pointer was passed to the callback
 *  function when it was registered.
 */
void gnc_gsettings_remove_cb_by_func (const gchar *schema,
                                      const gchar *key,
                                      GCallback func,
                                      gpointer user_data);


/** Remove a function that was registered for a callback when a
 *  specific key in the settings schema changed.  The handler id
 *  that was generated when the callback was registered is
 *  use to find the callback to remove.
 *  If no handler id is found nothing will happen.
 *
 *  @param schema This value contains the schema name of the key
 *  that is being watched.
 *
 *  @param id The handler id of the callback to be removed.
 */
void gnc_gsettings_remove_cb_by_id (const gchar *schema,
                                    guint id);


/** Register a callback for when any key in the settings schema
 *  is changed.  Any time the value of a key in this schema changes,
 *  the routine will be invoked and will be passed the specified
 *  user data.
 *
 *  @param schema This value contains the name of the schema
 *  that is being watched.
 *
 *  @param func This is a pointer to the function to call when a key
 *  changes.
 *
 *  @param user_data This pointer will be passed to the callback
 *  function.
 */
guint gnc_gsettings_register_any_cb (const gchar *schema,
                                     GCallback func,
                                     gpointer user_data);


/** Remove a function that was registered for a callback when any key
 *  in the given settings schema changed.  Both the func and user_data
 *  arguments are used to match up the callback to remove.
 *  If no matching func and user_data are found to be registered
 *  for the given key, nothing will happen.
 *
 *  @param schema This value contains the name of the schema
 *  that is being watched.
 *
 *  @param func This is a pointer to the function that was registered
 *  earlier.
 *
 *  @param user_data This pointer was passed to the callback
 *  function when it was registered.
 *
 *  @note there is no gnc_settings_remove_any_cb_by_id. Use
 *  gnc_settings_remove_cb_by_id instead if you want to
 *  remove a callback set with gnc_settings_register_any_cb
 *  by its handler id.
 */
void gnc_gsettings_remove_any_cb_by_func (const gchar *schema,
                                          GCallback func,
                                          gpointer user_data);

/** @} */


#endif /* GNC_GSETTINGS_H */
/** @} */
/** @} */
