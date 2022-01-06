#!/usr/bin/env python3
# encoding: utf-8

# Credit: Thanks to @venzen for the initial design and implementation of this test framework.


""" This script runs a simulation test of a biosim4 configuration.
    """

from sys import version_info, platform, argv, exit

try:
    assert version_info >= (3, 4)
except:
    print("Python version >= 3.4 required")
    exit(1)

import argparse
from pathlib import Path
import pydoc

# this script must be executed from the ./tests directory
try:
    assert str(Path(__file__).resolve().parent).endswith("tests")
except:
    print("this script must be executed from the ./tests directory")
    exit(1)

# Path(__file__) yields the filename only in Python <= 3.8
# so, we force .resolve() of the full pathname:
_scriptpath = Path(__file__).resolve()    # full path
_scriptname = _scriptpath.name  # filename incl .py
_appname = _scriptpath.stem     # filename excl .py
_projname = "biosim4"
_projconfig = "%s.ini" % _projname
_modname = "%s-mod.py" % _appname
_uiname = "%s-ui" % _appname
_libname = "%s-lib" % _appname
_configfile = "%s.ini" % _appname
# temporary file used by this app to create test configurations:
_temp_ini = "tmp.ini"
_results_log = "epoch-log.txt"
_version = "21.12.12"


usagemsg = """\nScript for testing a biosim4 simulation.
\n%s initializes a test environment and simulation parameters for a %s simulation. Always execute %s inside the '<project_root>/tests/' working directory. To see a list of configured tests, use the --show flag:

    python3 %s --show

Run the pre-defined simulation test:

    python3 %s --test quicktest

To see all available options and flags, run:

    python3 %s --help


Simulation parameters:

This script uses its own configuration file '%s' for storing and retrieving two sets of parameters:

1) simulation parameters, and
2) expected simulation outcome results

Inspect './configs/%s' to see the predefined example 'quicktest'. This test's simulation and result parameters can also be output to the console:

    python3 %s --test quicktest --params


Configure a new test:

1. Copy the default config file '%s' to the 'configs/' directory and rename the copy with the unique name of your new test:

    cp ../%s ./configs/my_new_test.ini

2. Edit the new test's parameters in './configs/my_new_test.ini'

3. Import the test parameters into the test environment:

    python3 %s --import my_new_test.ini

Simulation parameters (from .configs/my_new_test.ini) are converted to test-style format and stored in a new section 'my_new_test' in the configuration file './configs/%s'

After being imported, test parameters can be edited directly in the '%s' file.

Result parameters:

Result parameters can be manually added to '%s', or they can be initialed automatically by simply running a test simulation:

    python3 %s --test my_new_test

After completing the simulation, the script will propose to add result parameters (using actual test result values) to the config file. These can then be manually edited to match variations in test results.
""" % ( _scriptname,
        _projname,
        _scriptname,
        _scriptname,
        _scriptname,
        _scriptname,
        _configfile,
        _configfile,
        _scriptname,
        _projconfig,
        _projconfig,
        _scriptname,
        _configfile,
        _configfile,
        _configfile,
        _scriptname
)

# check args passed from the command line,
argp = argparse.ArgumentParser(
    description = "Script for running %s" % _projname
    + " simulation tests"
)
argp.add_argument(
    "-a",
    "--adjust",
    action = "store_true",
    default = False,
    help = "adjust result params according to test results"
)
argp.add_argument(
    "-c",
    "--check",
    action = "store_true",
    default = None,
    help = "check the test environment"
    # TODO show color [Yes] or [No] env checks
    # offer to repair, else --repair
)
argp.add_argument(
    # use this test section
    "-l",
    "--load",
    type = str,
    metavar = "FILE",
    default = None,
    help = "import parameters for a new test configuration\n"
            + "provide the name of the source .ini file in ./configs/"
)
argp.add_argument(
    # this shows both test amd result params
    "-p",
    "--params",
    action = "store_true",
    default = None,
    help = "use with --test to see the specified test's config params and result params"
)
argp.add_argument(
    # show result params only
    "-r",
    "--resultparams",
    action = "store_true",
    default = None,
    help = "use with --test to see the specified test's result parameters only"
)
argp.add_argument(
    # show all test sections found in the config file
    "-s",
    "--show",
    action = "store_true",
    default = None,
    help = "see a list of configured simulation tests"
)
argp.add_argument(
    # use this test section
    "-t",
    "--test",
    type = str,
    metavar = "NAME",
    default = None,
    help = "use this simulation test configuration"
)
argp.add_argument(
    "-u",
    "--usage", 
    action = "store_true", 
    help = "see more detailed usage instructions"
)
argp.add_argument(
    "-v",
    "--verbose", 
    action = "store_true",
    default = False,
    help = "see more verbose output"
)

args = argp.parse_args()

if args.usage:
    pydoc.pager(usagemsg)
    exit(0)


#
# We made it this far without incident or exiting
# Load additional modules for environment set-up.
import locale
import time
from datetime import timedelta
from pylib import config
from pylib import include_tests
from pylib import testlib


# the following is for initial config file creation.
# Don't edit here, it will have no effect.
# Instead, add/edit parameters in _configfile
#
# NOTE the values below are written to the config file's
# 'default' section (compulsory) and remain hidden unless
# specifically queried. Whenever any param listed here is
# removed from the .ini file it will be reinserted at the 
# new instantiation (next execution of this script). 
# The value will 'magically' revert to the default set below 
TEST_DEFAULTS = [
    ["internal", "name", _appname],
    ["internal", "version", _version],
    ["internal", "github_url", "https://github.com/davidrmiller/biosim4/"],
    ["microtest", "description", "Very quick test that only runs for a few seconds"],
    ['microtest', 'param-genomeinitiallengthmax', '20'],
    ['microtest', 'param-genomeinitiallengthmin', '20'],
    ['microtest', 'param-genomemaxlength', '30'],
    ['microtest', 'param-maxgenerations', '11'],
    ['microtest', 'param-maxnumberneurons', '2'],
    ['microtest', 'param-numthreads', '1'],
    ['microtest', 'param-population', '100'],
    ['microtest', 'param-sizex', '64'],
    ['microtest', 'param-sizey', '64'],
    ['microtest', 'param-stepspergeneration', '30'],
    ['microtest', 'result-diversity-max', '0.75'],
    ['microtest', 'result-diversity-min', '0.125'],
    ['microtest', 'result-generations', '10'],
    ['microtest', 'result-genomesize', '20'],
    ['microtest', 'result-kills', '0'],
    ['microtest', 'result-survivors-max', '41'],
    ['microtest', 'result-survivors-min', '12'],
    ["slowtest", "description", "Slow test that runs for hours/days"],
    ['slowtest', 'param-numthreads', '1'],
    ["slowtest", "param-stepsPerGeneration", "10000"],
    ['slowtest', 'param-maxgenerations', '100001']
]
# concatenate additional tests (with more params) stored in pylib/include_tests.py
TEST_DEFAULTS += include_tests.incl_list


# set US locale to ensure well-defined behavior for number formatting.
for loc in ["en_US.UTF8", "en_EN", "C"]:
    try:
        locale.setlocale(locale.LC_NUMERIC, loc)
        break
    except locale.Error:
        continue

# Check paths and files
try:
    #_scriptpath = Path(__file__) # defined at top of script
    assert _scriptpath.parent == Path.cwd()
    testspath = _scriptpath.parent
    configspath = testspath.joinpath("configs")
    configfile = configspath.joinpath(_configfile)
    biosimpath = testspath.parent
    #_projconfig set to 'biosim4.ini' at top of script
    defaultconfigfile = biosimpath.joinpath(_projconfig)
    # May not exist at this time:
    #logfile = ".logs/epoch-log.txt"
    
    # biosim4 does not like an abspath to configfile:
    #tempini = configspath.joinpath(_temp_ini)
    # So, map temporary ini file relative to project root:
    #tempini = "./tests/configs/%s" % _temp_ini
    # afterthought: just use the filename _temp_ini (defined at top of this file)
except AssertionError:
    print("testapp.py must be executed in the 'tests' directory")
    exit(1)
except Exception as e:
    print("Environment exception:\n%s" % e)
    exit(1)

#
# instantiate configparser 
#
thisconfig = config.TestConfig(str(configfile))
thisconfig.init_defaults(TEST_DEFAULTS)

if args.load:
    param_src = None
    try:
        param_src = Path.cwd().joinpath("configs", args.load)
        assert param_src.exists()
    except:
        print("\n%s not found in the ./configs/ directory\n" % param_src)
        exit(1)
    thisname = param_src.stem
    print("stem: ", thisname)
    #thisname = stem.name
    # add section with thisname, exit if thisname already exists
    try:
        thisconfig.add_section(thisname)
    except:
        print("add_section exception: section named %s already exists" % thisname)
    try:
        testlib.loadStdParamsFromFile(thisconfig, thisname, param_src)
    except Exception as e:
        print("load exception: ", e)
        exit(1)
    try:
        testlib.writeTestParamsToConfig(thisconfig, False)
    except Exception as e:
        print("save exception: ", e)
        exit(1)
    exit(0)

if args.check:

    cdict = dict()
    print("\nchecking the test environment:\n")
    print("OS platform: %s" % platform)
    # build a dictionary
    cdict["Python version"] = "%s.%s" % (version_info.major, version_info.minor)
    cdict["working dir"] = str(Path.cwd())
    cdict["app configpath"] = configspath
    cdict["app configfile"] = configfile
    cdict["biosim4 default config file"] = defaultconfigfile

    # get the longest string
    maxlen = 0
    for k, v in cdict.items():
        thislen = len(k) + len(str(v)) + 7
        if thislen > maxlen:
            maxlen = thislen 
    # now build output strings
    count = 0
    for k, v in cdict.items():
        thislen = len(k) + len(str(v)) + 7
        flag = "Pass"
        if 'Python' in k:
            if float(v) < 3.4:
                flag = "Fail"
        elif 'working' in k:
            if not v.endswith("tests"):
                flag = "Fail"
        elif 'configpath' in k or 'configfile' in k or 'default' in k:
            if not Path(v).exists():
                flag = "Fail"

        padding = maxlen - thislen
        s = "%s: %s   %s  %s" % (k, v, '.' * padding, flag)
        print(s)
        if flag != "Pass":
            count += 1
    if count != 0:
        print("\nFound %i error(s) in the test environment" % count)
        print("remedy missing files and run --check again")
    print("\nReport bugs to https://github.com/davidrmiller/biosim4/\n")
    exit(0)

if args.show:

    tests = testlib.showTests(thisconfig)
    print("\nConfigured tests:\n")
    for t in tests:
        print(t)
    print()

elif args.test:

    #TODO change writeStdTestFile() to take t (section) as arg
    t = testlib.getTestSection(thisconfig, args.test)
    if not t:
        print("to see available tests, run:\n\n    python3 %s --show\n" % _scriptname)
        exit(1)

    if args.params:
        # the output typically extends beyond one console screen
        # we use the pydoc.pager for convenient navigation
        # so, build a long text object aot print()
        paramspage = "\n%s parameters:\n\n" % args.test
        for k in sorted(t):
            paramspage += "%s= %s\n" % (k, t[k])
            # the line below will format output as 
            # TEST_DEFAULT list format instead:
            # e.g. ["quicktest", "param-stepsPerGeneration", "100"],
            #paramspage += "['%s', '%s', '%s'],\n" % (args.test, k, t[k])
        try:
            if len(paramspage) > 3:
                pydoc.pager(paramspage)
            else:
                print("no parameters to show")
        except Exception as e:
            print("Exception showing params:\n%s" % e)
        exit(0)

    if args.resultparams:
        try:
            resdict, comp = testlib.getResultParams(thisconfig, args.test)
            testlib.showResultParams(resdict)
        except Exception as e:
            print("Exception getting results params:\n%s" % e)
        exit(0)
    
    # here we run the sim test
    try:
        testlib.TEMPinifile = _temp_ini
        print("\nRunning %s sim" % t.name)
        #print(dir(t))
        testlib.writeStdTestFile(thisconfig, args.test)
        if args.verbose:
            print("\n# ", t['description'])
        # start objetive time
        start_time = time.monotonic()
        # start processor time
        cpu_start_time = time.perf_counter()
        #
        # run simulation
        proc = testlib.runTest()
        #
        cpu_end_time = time.perf_counter()
        end_time = time.monotonic()
        print("\n# simulation completed\n")
        if args.verbose:
            print(proc)
            print("clock time: %s seconds" % timedelta(seconds=end_time - start_time))
            print("CPU time: %s seconds" % (cpu_end_time - start_time))
        #
        # run test analysis
        res = testlib.resultsAnalysis(thisconfig, args.test, _results_log, args.adjust)
        # if, during the results analysis, the user opted
        # to add missing result params, we should save them
        # to the test config file
        if res == 'update':
            print("\nwriting test config to file...")
            testlib.writeTestParamsToConfig(thisconfig, False)
            print("\nresult parameters were added\nrun the test again")

    except Exception as e:
        print("test exception:\n%s" % e)
        exit(1)
else:
    print("\nYou must specify the name of a test configuration\n")
    print("Example:\tpython3 %s --test quicktest [other options]" % _appname)
    print("\nTo see the available test config names, run:\n")
    print("\tpython3 %s --show\n" % _appname)


print("%s done" % _appname)
exit(0)
