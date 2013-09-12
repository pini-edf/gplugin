# vi:et:ts=4 sw=4 sts=4

from gi.repository import GPlugin

def gplugin_plugin_query():
    return GPlugin.PluginInfo(
        id='gplugin-python/dependent-plugin',
        abi_version=0x01020304,
        name='dependent plugin',
        dependencies=['dep1', 'dep2'],
    )

def gplugin_plugin_load(plugin):
    return False


def gplugin_plugin_unload(plugin):
    return False

