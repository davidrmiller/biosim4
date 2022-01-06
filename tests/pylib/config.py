#!/usr/bin/env python3
# encoding: utf-8

""" Configuration parser for reading, writing and manipulating 
    .ini files. This implementation features some customisations
    such as sorting of keys, and convenience functions that
    wrap load() and save() operations.
    """

from configparser import SafeConfigParser

# Signal is used in class methods write_config() at the bottom
# of this file. Signal lock forces exection to conclude before
# other Signal operations can execute. 
#
# from facil.threadutils import Signal
# with Signal._lock():
#     config.write_config(a,b,c)
from .threadutils import Signal

class TestConfig(SafeConfigParser):
    """ Return a config parser object with default values.
        """

    def __init__(self, filename, _DEFAULTS=[]):
        self.filename = filename
        self._DEFAULTS = _DEFAULTS
        SafeConfigParser.__init__(self)
        self.load()

        # Future use. Example of upgradimg deprecated key/val:
        # upgrade from deprecated "currency" to "quote_currency"
        if self.has_option("forex", "currency"):
            self.set("forex", "quote_currency", self.get_string("forex", "currency"))
            self.remove_option("forex", "currency")
            self.save()

    def init_defaults(self, defaults):
        """ add the missing default values, 
            default is a list of defaults
            """
        for (sect, opt, default) in defaults:
            self._default(sect, opt, default)

    def save(self, sort=False):
        """ save the config to the .ini file
            """
        with open(self.filename, 'w') as configfile:
            self.write(configfile, sort, space_around_delimiters=True)

    def load(self):
        """ (re)load the config from the .ini file
            """
        self.read(self.filename)

    def get_safe(self, sect, opt):
        """ get value without throwing exception.
            """
        try:
            return self.get(sect, opt)

        except:
            for (dsect, dopt, default) in self._DEFAULTS:
                if dsect == sect and dopt == opt:
                    self._default(sect, opt, default)
                    return default
            return ""

    def get_bool(self, sect, opt):
        """ get boolean value from config"""
        return self.get_safe(sect, opt) == "True"

    def get_string(self, sect, opt):
        """ get string value from config"""
        return self.get_safe(sect, opt)

    def get_int(self, sect, opt):
        """ get int value from config"""
        vstr = self.get_safe(sect, opt)
        try:
            return int(vstr)
        except ValueError:
            return 0

    def get_float(self, sect, opt):
        """ get int value from config"""
        vstr = self.get_safe(sect, opt)
        try:
            return float(vstr)
        except ValueError:
            return 0.0

    def _default(self, section, option, default, sort=False):
        """ create a default option if it does not yet exist
            """
        if not self.has_section(section):
            self.add_section(section)
        if not self.has_option(section, option):
            self.set(section, option, default)
            self.save(sort)
    
    def write(self, fp, sort=False, space_around_delimiters=True):
        """ Write an .ini-format representation of the configuration state.

        If `space_around_delimiters' is True (the default), delimiters between keys and values are surrounded by spaces.

        Please note that comments in the original configuration file are not preserved when writing the configuration back.
        """
        if space_around_delimiters:
            d = " {} ".format(self._delimiters[0])
        else:
            d = self._delimiters[0]
        if self._defaults:
            self._write_section(fp, self.default_section,
                                    self._defaults.items(), d)
        for section in self._sections:
            self._write_section(fp, section,
                                self._sections[section].items(), d, sort)

    def _write_section(self, fp, section_name, section_items, delimiter, sort):
        """ write a single section to the specified `fp'."""
        fp.write("[{}]\n".format(section_name))
        if sort:
            section_items = sorted(section_items)
        for key, value in section_items:
            value = self._interpolation.before_write(self, section_name, key, value)
            if value is not None or not self._allow_no_value:
                value = delimiter + str(value).replace('\n', '\n\t')
            else:
                value = ""
            fp.write("{}{}\n".format(key, value))
        fp.write("\n")


    ##
    # functions for micro-managing .ini file options
    #
    def write_config_setting(self, section, option, value):
        """ write a setting in the ini file
            """
        with Signal._lock:
            setting = self.get_string(section, option)
            self.set(section, option, str(value))
            self.save()
    
    def toggle_setting(self, alternatives, section, option, direction):
        """ toggle a setting in the ini file
            """
        with Signal._lock:
            setting = self.get_string(section, option)
            try:
                newindex = (alternatives.index(setting) + direction) % len(alternatives)
            except ValueError:
                newindex = 0
            self.set(section, option, alternatives[newindex])
            self.save()
        return alternatives[newindex]

