/*
 * Copyright (C) 2011-2012 Gary Kramlich <grim@reaperworld.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gplugin/gplugin-plugin-info.h>
#include <gplugin/gplugin-core.h>

#include <gplugin/gplugin-enums.h>
#include <gplugin/gplugin-private.h>

#define GPLUGIN_PLUGIN_INFO_GET_PRIVATE(obj) \
	(G_TYPE_INSTANCE_GET_PRIVATE((obj), GPLUGIN_TYPE_PLUGIN_INFO, GPluginPluginInfoPrivate))

/******************************************************************************
 * Structs
 *****************************************************************************/
typedef struct {
	gchar *id;

	guint32 abi_version;
	GPluginPluginInfoFlags flags;

	gchar *name;
	gchar *version;

	gchar *license;
	gchar *license_text;
	gchar *license_url;

	gchar *icon;

	gchar *summary;
	gchar *description;
	gchar *category;
	gchar *author;
	gchar *website;

	gchar *dependencies;
} GPluginPluginInfoPrivate;

/******************************************************************************
 * Enums
 *****************************************************************************/
enum {
	PROP_ZERO = 0,
	PROP_ID,
	PROP_ABI_VERSION,
	PROP_FLAGS,
	PROP_NAME,
	PROP_VERSION,
	PROP_LICENSE,
	PROP_LICENSE_TEXT,
	PROP_LICENSE_URL,
	PROP_ICON,
	PROP_SUMMARY,
	PROP_DESCRIPTION,
	PROP_CATEGORY,
	PROP_AUTHOR,
	PROP_WEBSITE,
	PROP_DEPENDENCIES,
	N_PROPERTIES,
};

/******************************************************************************
 * Globals
 *****************************************************************************/
static GObjectClass *parent_class = NULL;

/******************************************************************************
 * Private API
 *****************************************************************************/
static void
gplugin_plugin_info_set_id(GPluginPluginInfo *info, const gchar *id) {
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->id);
	priv->id = (id) ? g_strdup(id) : NULL;
}

static void
gplugin_plugin_info_set_abi_version(GPluginPluginInfo *info,
                                    guint32 abi_version)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	priv->abi_version = abi_version;
}

static void
gplugin_plugin_info_set_flags(GPluginPluginInfo *info,
                              GPluginPluginInfoFlags flags)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	priv->flags = flags;
}

static void
gplugin_plugin_info_set_name(GPluginPluginInfo *info, const gchar *name) {
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->name);
	priv->name = (name) ? g_strdup(name) : NULL;
}

static void
gplugin_plugin_info_set_version(GPluginPluginInfo *info,
                                const gchar *version)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->version);
	priv->version = (version) ? g_strdup(version) : NULL;
}

static void
gplugin_plugin_info_set_license(GPluginPluginInfo *info,
                                const gchar *license)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->license);
	priv->license = (license) ? g_strdup(license) : NULL;
}


static void
gplugin_plugin_info_set_license_text(GPluginPluginInfo *info,
                                     const gchar *license_text)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->license_text);
	priv->license_text = (license_text) ? g_strdup(license_text) : NULL;
}


static void
gplugin_plugin_info_set_license_url(GPluginPluginInfo *info,
                                    const gchar *license_url)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->license_url);
	priv->license_url = (license_url) ? g_strdup(license_url) : NULL;
}

static void
gplugin_plugin_info_set_icon(GPluginPluginInfo *info, const gchar *icon) {
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->icon);
	priv->icon = (icon) ? g_strdup(icon) : NULL;
}

static void
gplugin_plugin_info_set_summary(GPluginPluginInfo *info,
                                const gchar *summary)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->summary);
	priv->summary = (summary) ? g_strdup(summary) : NULL;
}

static void
gplugin_plugin_info_set_description(GPluginPluginInfo *info,
                                    const gchar *description)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->description);
	priv->description = (description) ? g_strdup(description) : NULL;
}

static void
gplugin_plugin_info_set_category(GPluginPluginInfo *info,
                                 const gchar *category)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->category);
	priv->category = (category) ? g_strdup(category) : NULL;
}

static void
gplugin_plugin_info_set_author(GPluginPluginInfo *info, const gchar *author) {
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->author);
	priv->author = (author) ? g_strdup(author) : NULL;
}

static void
gplugin_plugin_info_set_website(GPluginPluginInfo *info,
                                const gchar *website)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->website);
	priv->website = (website) ? g_strdup(website) : NULL;
}

static void
gplugin_plugin_info_set_dependencies(GPluginPluginInfo *info,
                                     const gchar *dependencies)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	g_free(priv->dependencies);
	priv->dependencies = (dependencies) ? g_strdup(dependencies) : NULL;
}

/******************************************************************************
 * Object Stuff
 *****************************************************************************/
static void
gplugin_plugin_info_get_property(GObject *obj, guint param_id, GValue *value,
                                 GParamSpec *pspec)
{
	GPluginPluginInfo *info = GPLUGIN_PLUGIN_INFO(obj);

	switch(param_id) {
		case PROP_ID:
			g_value_set_string(value, gplugin_plugin_info_get_id(info));
			break;
		case PROP_ABI_VERSION:
			g_value_set_uint(value, gplugin_plugin_info_get_abi_version(info));
			break;
		case PROP_FLAGS:
			g_value_set_flags(value, gplugin_plugin_info_get_flags(info));
			break;
		case PROP_NAME:
			g_value_set_string(value, gplugin_plugin_info_get_name(info));
			break;
		case PROP_VERSION:
			g_value_set_string(value, gplugin_plugin_info_get_version(info));
			break;
		case PROP_LICENSE:
			g_value_set_string(value, gplugin_plugin_info_get_license(info));
			break;
		case PROP_LICENSE_TEXT:
			g_value_set_string(value,
			                   gplugin_plugin_info_get_license_text(info));
			break;
		case PROP_LICENSE_URL:
			g_value_set_string(value,
			                   gplugin_plugin_info_get_license_url(info));
			break;
		case PROP_ICON:
			g_value_set_string(value, gplugin_plugin_info_get_icon(info));
			break;
		case PROP_SUMMARY:
			g_value_set_string(value, gplugin_plugin_info_get_summary(info));
			break;
		case PROP_DESCRIPTION:
			g_value_set_string(value,
			                   gplugin_plugin_info_get_description(info));
			break;
		case PROP_CATEGORY:
			g_value_set_string(value, gplugin_plugin_info_get_category(info));
			break;
		case PROP_AUTHOR:
			g_value_set_string(value, gplugin_plugin_info_get_author(info));
			break;
		case PROP_WEBSITE:
			g_value_set_string(value, gplugin_plugin_info_get_website(info));
			break;
		case PROP_DEPENDENCIES:
			g_value_set_string(value,
			                   gplugin_plugin_info_get_dependencies(info));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_plugin_info_set_property(GObject *obj, guint param_id,
                                 const GValue *value, GParamSpec *pspec)
{
	GPluginPluginInfo *info = GPLUGIN_PLUGIN_INFO(obj);

	switch(param_id) {
		case PROP_ID:
			gplugin_plugin_info_set_id(info, g_value_get_string(value));
			break;
		case PROP_ABI_VERSION:
			gplugin_plugin_info_set_abi_version(info, g_value_get_uint(value));
			break;
		case PROP_FLAGS:
			gplugin_plugin_info_set_flags(info, g_value_get_flags(value));
			break;
		case PROP_NAME:
			gplugin_plugin_info_set_name(info, g_value_get_string(value));
			break;
		case PROP_VERSION:
			gplugin_plugin_info_set_version(info, g_value_get_string(value));
			break;
		case PROP_LICENSE:
			gplugin_plugin_info_set_license(info, g_value_get_string(value));
			break;
		case PROP_LICENSE_TEXT:
			gplugin_plugin_info_set_license_text(info,
			                                     g_value_get_string(value));
			break;
		case PROP_LICENSE_URL:
			gplugin_plugin_info_set_license_url(info,
			                                    g_value_get_string(value));
			break;
		case PROP_ICON:
			gplugin_plugin_info_set_icon(info, g_value_get_string(value));
			break;
		case PROP_SUMMARY:
			gplugin_plugin_info_set_summary(info, g_value_get_string(value));
			break;
		case PROP_DESCRIPTION:
			gplugin_plugin_info_set_description(info,
			                                    g_value_get_string(value));
			break;
		case PROP_CATEGORY:
			gplugin_plugin_info_set_category(info, g_value_get_string(value));
			break;
		case PROP_AUTHOR:
			gplugin_plugin_info_set_author(info, g_value_get_string(value));
			break;
		case PROP_WEBSITE:
			gplugin_plugin_info_set_website(info, g_value_get_string(value));
			break;
		case PROP_DEPENDENCIES:
			gplugin_plugin_info_set_dependencies(info,
			                                     g_value_get_string(value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, param_id, pspec);
			break;
	}
}

static void
gplugin_plugin_info_finalize(GObject *obj) {
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(obj);

    g_free(priv->id);
    g_free(priv->name);
    g_free(priv->version);
    g_free(priv->license);
    g_free(priv->license_text);
    g_free(priv->license_url);
    g_free(priv->icon);
    g_free(priv->summary);
    g_free(priv->description);
    g_free(priv->author);
    g_free(priv->website);
	g_free(priv->dependencies);

	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static void
gplugin_plugin_info_class_init(GPluginPluginInfoClass *klass) {
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);

	g_type_class_add_private(klass, sizeof(GPluginPluginInfoPrivate));

	obj_class->get_property = gplugin_plugin_info_get_property;
	obj_class->set_property = gplugin_plugin_info_set_property;
	obj_class->finalize = gplugin_plugin_info_finalize;

	/* properties */
	/**
	 * GPluginPluginInfo:id:
	 *
	 * The id of the plugin.
	 *
	 * While not required, the recommended convention is to use the following
	 * format: <application or library>/<name of the plugin>.
	 *
	 * For example, the python loader in GPlugin has an id of
	 * "gplugin/python-plugin-loader".
	 */
	g_object_class_install_property(obj_class, PROP_ID,
		g_param_spec_string("id", "id",
		                    "The ID of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:abi-version:
	 *
	 * The GPlugin ABI version that the plugin was compiled against.
	 *
	 * GPlugin only uses the first byte (0xff000000) of this value.  The
	 * remaining 3 bytes are available for the application to use.
	 *
	 * Take the following example from an application:
	 *
	 *  #define ABI_VERSION (GPLUGIN_NATIVE_ABI_VERSION |
	 *                       (APPLICATION_MAJOR_VERSION << 8) |
	 *                       (APPLICATION_MINOR_VERSION))
	 *
	 * The application here uses the thrid and fourth bytes, but could use
	 * the second as well.
	 */
	g_object_class_install_property(obj_class, PROP_ABI_VERSION,
		g_param_spec_uint("abi-version", "abi_version",
		                  "The ABI version of the plugin",
		                  0, G_MAXUINT32, 0,
		                  G_PARAM_READWRITE |
		                  G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:flags:
	 *
	 * The #GPluginPluginInfoFlags for the plugin.
	 */
	g_object_class_install_property(obj_class, PROP_FLAGS,
		g_param_spec_flags("flags", "flags",
		                   "The flags for the plugin",
		                   GPLUGIN_TYPE_PLUGIN_INFO_FLAGS, 0,
		                   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                   G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:name:
	 *
	 * The display name of the plugin.  This should be a translated string.
	 */
	g_object_class_install_property(obj_class, PROP_NAME,
		g_param_spec_string("name", "name",
		                    "The name of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:version:
	 *
	 * The version of the plugin.
	 */
	g_object_class_install_property(obj_class, PROP_VERSION,
		g_param_spec_string("version", "version",
		                    "The version of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:license:
	 *
	 * The short name of the license.
	 *
	 * It is recommended to use the identifier of the license from
	 * http://spdx.org/licenses/ and should be "Other" for custom licenses
	 * that are not on the SPDX list.
	 *
	 * If a plugin has multiple license, they should be separated by a comma.
	 */
	g_object_class_install_property(obj_class, PROP_LICENSE,
		g_param_spec_string("license", "license id",
		                    "The license id of the plugin according to SPDX",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:license-text:
	 *
	 * The text of the license for this plugin.  This should only be used when
	 * the plugin is licensed under a license that is not listed on SPDX.
	 */
	g_object_class_install_property(obj_class, PROP_LICENSE_TEXT,
		g_param_spec_string("license-text", "license text",
		                    "The text of the license for the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:license-url:
	 *
	 * The url to the text of the license.  This should primarily only be used
	 * for licenses not listed on SPDX.
	 */
	g_object_class_install_property(obj_class, PROP_LICENSE_URL,
		g_param_spec_string("license-url", "license url",
		                    "The url to the license of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:icon:
	 *
	 * A string representing an icon for the plugin.  The actual use of this
	 * is determined by the application/library using GPlugin.
	 */
	g_object_class_install_property(obj_class, PROP_ICON,
		g_param_spec_string("icon", "icon",
		                    "The file path of the icon for the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:summary:
	 *
	 * A short description of the plugin that can be listed with the name in a
	 * user interface.
	 */
	g_object_class_install_property(obj_class, PROP_SUMMARY,
		g_param_spec_string("summary", "summary",
		                    "The summary of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:description:
	 *
	 * The full description of the plugin that will be used in a "more
	 * information" section in a user interface.
	 */
	g_object_class_install_property(obj_class, PROP_DESCRIPTION,
		g_param_spec_string("description", "description",
		                    "The description of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:category:
	 *
	 * The category of this plugin.
	 *
	 * This property is used to organize plugins into categories in a user
	 * interface.  It is recommended that an application has a well defined
	 * set of categories that plugin authors should use, and put all plugins
	 * that don't match this category into an "Other" category.
	 */
	g_object_class_install_property(obj_class, PROP_CATEGORY,
		g_param_spec_string("category", "category",
		                    "The category of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:author:
	 *
	 * The name and email address of the author.
	 *
	 * It is recommended to use the RFC 822, 2822 format of:
	 * "First Last <user@domain.com>" with additional authors separated by a
	 * comma.
	 */
	g_object_class_install_property(obj_class, PROP_AUTHOR,
		g_param_spec_string("author", "author",
		                    "The author of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:website:
	 *
	 * The url of the plugin that can be represented in a user interface.
	 */
	g_object_class_install_property(obj_class, PROP_WEBSITE,
		g_param_spec_string("website", "website",
		                    "The website of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	/**
	 * GPluginPluginInfo:dependencies:
	 *
	 * A comma separated list of plugin id's that this plugin depends on.
	 */
	g_object_class_install_property(obj_class, PROP_DEPENDENCIES,
		g_param_spec_string("dependencies", "dependencies",
		                     "The dependencies of the plugin",
		                     NULL,
		                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                     G_PARAM_CONSTRUCT_ONLY));
}

/******************************************************************************
 * GPluginPlugin API
 *****************************************************************************/
GType
gplugin_plugin_info_get_type(void) {
	static volatile gsize type_volatile = 0;

	if(g_once_init_enter(&type_volatile)) {
		GType type = 0;

		static const GTypeInfo info = {
			.class_size = sizeof(GPluginPluginInfoClass),
			.class_init = (GClassInitFunc)gplugin_plugin_info_class_init,
			.instance_size = sizeof(GPluginPluginInfo),
		};

		type = g_type_register_static(G_TYPE_OBJECT,
		                              "GPluginPluginInfo",
		                              &info, 0);

		g_once_init_leave(&type_volatile, type);
	}

	return type_volatile;
}

/**
 * gplugin_plugin_info_new: (skip)
 * @id: The id of the plugin
 * @abi_version: The GPlugin ABI version that the plugin uses
 * @...: name/value pairs of properties to set, followed by %NULL.
 *
 * Creates a new GPluginPluginInfo instance.
 *
 * Returns: The new GPluginPluginInfo instance.
 */

/**
 * gplugin_plugin_info_get_id:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The id from @info.
 */
const gchar *
gplugin_plugin_info_get_id(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->id;
}

/**
 * gplugin_plugin_info_get_abi_version:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The abi_version from @info.
 */
guint32
gplugin_plugin_info_get_abi_version(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), 0);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->abi_version;
}

/**
 * gplugin_plugin_info_get_flags:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The flags from @info.
 */
GPluginPluginInfoFlags
gplugin_plugin_info_get_flags(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), 0);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->flags;
}

/**
 * gplugin_plugin_info_get_name:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The name from @info.
 */
const gchar *
gplugin_plugin_info_get_name(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->name;
}

/**
 * gplugin_plugin_info_get_version:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The version from @info.
 */
const gchar *
gplugin_plugin_info_get_version(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->version;
}

/**
 * gplugin_plugin_info_get_license:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The license from @info.
 */
const gchar *
gplugin_plugin_info_get_license(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->license;
}

/**
 * gplugin_plugin_info_get_license_text:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The text of the license from @info.
 */
const gchar *
gplugin_plugin_info_get_license_text(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->license_text;
}

/**
 * gplugin_plugin_info_get_license_url:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The url of the license from @info.
 */
const gchar *
gplugin_plugin_info_get_license_url(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->license_url;
}

/**
 * gplugin_plugin_info_get_icon:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The icon from @info.
 */
const gchar *
gplugin_plugin_info_get_icon(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->icon;
}

/**
 * gplugin_plugin_info_get_summary:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The summary from @info.
 */
const gchar *
gplugin_plugin_info_get_summary(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->summary;
}

/**
 * gplugin_plugin_info_get_description:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The description from @info.
 */
const gchar *
gplugin_plugin_info_get_description(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->description;
}

/**
 * gplugin_plugin_info_get_category:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The category from @info.
 */
const gchar *
gplugin_plugin_info_get_category(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->category;
}

/**
 * gplugin_plugin_info_get_author:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The author from @info.
 */
const gchar *
gplugin_plugin_info_get_author(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->author;
}

/**
 * gplugin_plugin_info_get_website:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The website from @info.
 */
const gchar *
gplugin_plugin_info_get_website(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->website;
}

/**
 * gplugin_plugin_info_get_dependencies:
 * @info: #GPluginPluginInfo instance
 *
 * Return value: The command separate list of dependencies from @info.
 */
const gchar *
gplugin_plugin_info_get_dependencies(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->dependencies;
}


