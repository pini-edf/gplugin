# vi:et:ts=4 sw=4 sts=4

from gi.repository import GPlugin

def gplugin_query():
    return GPlugin.PluginInfo(
        id='gplugin-python/basic-plugin',
        abi_version=0x01020304,
        name='basic plugin',
        authors=['author1'],
        category='test',
        version='version',
        license_id='license',
        summary='summary',
        website='website',
        description='description',
    )

def gplugin_load(plugin):
    return True


def gplugin_unload(plugin):
    return True

