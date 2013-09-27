# vi:et:ts=4 sw=4 sts=4

use strict;

use Glib::Object::Introspection;

Glib::Object::Introspection->setup(basename => "GPlugin", version => "0.0", package=> "GPlugin");

sub gplugin_plugin_query() {
	return GPlugin::PluginInfo->new(
		id => "gplugin-perl/basic-plugin",
		abi_version => 0x01000001,
		name => "basic plugin",
		authors => ("Gary Kramlich <grim\@reaperworld.com>"),
		category => "test",
		version => "version",
		license_id => "license id",
		summary => "summary",
		website => "website",
		description => "description",
	);
}

sub gplugin_plugin_load() {
	my $plugin = shift;

	return 0;
}

sub gplugin_plugin_unload() {
	my $plugin = shift;

	return 0;
}

