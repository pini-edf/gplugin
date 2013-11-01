# vi:et:ts=4 sw=4 sts=4

from gi.repository import GPlugin

def gplugin_query():
    return GPlugin.PluginInfo(
        id="gplugin-python/unload-failed",
    )


def gplugin_load(plugin):
    return True


def gplugin_unload(plugin):
    return False


