/*
 * Copyright (C) 2011-2014 Gary Kramlich <grim@reaperworld.com>
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
var GPlugin = imports.gi.GPlugin;

function gplugin_query() {
	return new GPlugin.PluginInfo({
		id: "gplugin/seed-basic-plugin",
		abi_version: 0x01020304,
		name: "basic plugin",
		authors: ['author1'],
		category: 'test',
		version: 'version',
		license_id: 'license',
		summary: 'summary',
		website: 'website',
		description: 'description'
	});
};

function gplugin_load(plugin) {
	return true;
};

function gplugin_unload(plugin) {
	return true;
};


