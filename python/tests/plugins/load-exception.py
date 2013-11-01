# vi:et:ts=4 sw=4 sts=4

from gi.repository import GPlugin

def gplugin_query():
    return GPlugin.PluginInfo(
        id="gplugin-python/load-exception",
    )


def gplugin_load(plugin):
    raise ValueError('explosion!')


def gplugin_unload(plugin):
    return True


