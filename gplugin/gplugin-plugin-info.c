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

    gchar *icon;

    gchar *summary;
    gchar *description;
    gchar *author;
    gchar *website;

    GSList *dependencies;
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
	PROP_ICON,
	PROP_SUMMARY,
	PROP_DESCRIPTION,
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
                                     GSList *dependencies)
{
	GPluginPluginInfoPrivate *priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	if(priv->dependencies)
		g_slist_free(priv->dependencies);

	priv->dependencies = dependencies;
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
		case PROP_AUTHOR:
			g_value_set_string(value, gplugin_plugin_info_get_author(info));
			break;
		case PROP_WEBSITE:
			g_value_set_string(value, gplugin_plugin_info_get_website(info));
			break;
		case PROP_DEPENDENCIES:
			g_value_set_pointer(value,
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
		case PROP_AUTHOR:
			gplugin_plugin_info_set_author(info, g_value_get_string(value));
			break;
		case PROP_WEBSITE:
			gplugin_plugin_info_set_website(info, g_value_get_string(value));
			break;
		case PROP_DEPENDENCIES:
			gplugin_plugin_info_set_dependencies(info,
			                                     g_value_get_pointer(value));
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
    g_free(priv->icon);
    g_free(priv->summary);
    g_free(priv->description);
    g_free(priv->author);
    g_free(priv->website);

	if(priv->dependencies)
		g_slist_free(priv->dependencies);

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
	g_object_class_install_property(obj_class, PROP_ID,
		g_param_spec_string("id", "id",
		                    "The ID of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_ABI_VERSION,
		g_param_spec_uint("abi_version", "abi_version",
		                  "The ABI version of the plugin",
		                  0, G_MAXUINT32, 0,
		                  G_PARAM_READWRITE |
		                  G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_FLAGS,
		g_param_spec_flags("flags", "flags",
		                   "The flags for the plugin",
		                   GPLUGIN_TYPE_PLUGIN_INFO_FLAGS, 0,
		                   G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                   G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_NAME,
		g_param_spec_string("name", "name",
		                    "The name of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_VERSION,
		g_param_spec_string("version", "version",
		                    "The version of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_LICENSE,
		g_param_spec_string("license", "license",
		                    "The license of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_ICON,
		g_param_spec_string("icon", "icon",
		                    "The file path of the icon for the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_SUMMARY,
		g_param_spec_string("summary", "summary",
		                    "The summary of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_DESCRIPTION,
		g_param_spec_string("description", "description",
		                    "The description of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_AUTHOR,
		g_param_spec_string("author", "author",
		                    "The author of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_WEBSITE,
		g_param_spec_string("website", "website",
		                    "The website of the plugin",
		                    NULL,
		                    G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                    G_PARAM_CONSTRUCT_ONLY));

	g_object_class_install_property(obj_class, PROP_DEPENDENCIES,
		g_param_spec_pointer("dependencies", "dependencies",
		                     "The dependencies of the plugin",
		                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS |
		                     G_PARAM_CONSTRUCT_ONLY));
}

/******************************************************************************
 * GPluginPlugin API
 *****************************************************************************/
GType
gplugin_plugin_info_get_type(void) {
	static GType type = 0;

	if(G_UNLIKELY(type == 0)) {
		static const GTypeInfo info = {
			.class_size = sizeof(GPluginPluginInfoClass),
			.class_init = (GClassInitFunc)gplugin_plugin_info_class_init,
			.instance_size = sizeof(GPluginPluginInfo),
		};

		type = g_type_register_static(G_TYPE_OBJECT,
		                              "GPluginPluginInfo",
		                              &info, 0);
	}

	return type;
}

/**
 * gplugin_plugin_info_new:
 * @first: The first property name, or %NULL
 * @...: The value of the first property, followed optionally by more name/value
 *       pairs, followed by %NULL
 *
 * Creates a new #GPluginPluginInfo instance of the type provided by
 * gplugin_set_plugin_info_type().
 *
 * Return value: The new #GPluginPluginInfo instance.
 */
GPluginPluginInfo *
gplugin_plugin_info_new(const char *first, ...) {
	GObject *plugin_info;
	va_list var_args;

	if (!first) {
		plugin_info = g_object_newv(gplugin_get_plugin_info_type(), 0, NULL);
		return GPLUGIN_PLUGIN_INFO(plugin_info);
	}

	va_start (var_args, first);
	plugin_info = g_object_new_valist(gplugin_get_plugin_info_type(), first,
	                                  var_args);
	va_end (var_args);

	return GPLUGIN_PLUGIN_INFO(plugin_info);
}

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
 * Return value: (element-type utf8) (transfer container): The dependencies
 *               from @info.
 */
GSList *
gplugin_plugin_info_get_dependencies(const GPluginPluginInfo *info) {
	GPluginPluginInfoPrivate *priv = NULL;

	g_return_val_if_fail(GPLUGIN_IS_PLUGIN_INFO(info), NULL);

	priv = GPLUGIN_PLUGIN_INFO_GET_PRIVATE(info);

	return priv->dependencies;
}


