# vi:et:ts=4 sw=4 sts=4

from gi.repository import GPlugin

def gplugin_plugin_query():
    return GPlugin.PluginInfo(
        id="gplugin-python/load-failed",
    )


def gplugin_plugin_load(plugin):
    return False


def gplugin_plugin_unload(plugin):
    return True


