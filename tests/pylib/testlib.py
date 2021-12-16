""" Test application functions for manipulating parameters,
    running a simulation test, and analysing results. 
    """

from pathlib import Path
import subprocess
#from pylib import config

global TEMPinifile
TEMPinifile = None


#TODO initialize object (perhaps instantiate a class singleton)?
# attributes: sections, active class, etc 
# update sections if added, deleted, modified

def updateSections(thisconfig):
    """ Re-read the test config INI file. Load into the
        test environment config object.
        """

    raise NotImplementedError


def showTests(thisconfig):
    """ display all test sections in app config file
        """
    sections = thisconfig.sections()
    tests = [s for s in sections if s != "internal"]
    
    return tests


def getTestSection(thisconfig, testname):
    """ Select the test section 'testname' from the
        test config object. 
        """

    s = None
    try:
        s = thisconfig[testname]
    except KeyError:
        print("section '%s' does not exist" % testname)
        # create it here or prefer user to deliberately add?
    except Exception as e:
        print("getTestSection() exception:\n", e)

    return s


def checkFileExists(filename, create=False):
    """ Check if file 'filename' exists. If not, create it. 
        """

    try:
        assert Path(filename).exists()
    except:
        #TODO ensure it is one of the expected patterns/locations
        #       else warn user and exit with an error
        if create:
            # create file
            Path(filename).touch(exist_ok=True)
            print("checkFileExists() created file ", filename)
        else:
            return False
    
    return True

def readTestParamsFromConfig(thisconfig):
    """ (Re)read the test config INI file. 
        """

    try:
        # the config module's load() method loads section
        # keys and values from '.tests/configs/testapp.ini'
        # this filename is an attribute of the config class
        thisconfig.load()
        print("Configuration reloaded.\n")
    except Exception as e:
        print("readTestParams() exception:\n%s" % e)


def writeTestParamsToConfig(thisconfig, sort=False):
    """ Save the test config object to test config INI file.
        """
    try:
        # the config module's save() method writes section
        # keys and values to '.tests/configs/testapp.ini'
        # this filename is already an attribute of the 
        # config class, so no need to specify it here
        thisconfig.save(sort)
        print("Success.\n")
    except Exception as e:
        print("writeParams() exception:\n%s" % e)


def readStdConfigFile(param_src):
    """ Read a biosim4 style INI file and cast as a
        dictionary.
        """

    sectiondict = dict()

    checkFileExists(param_src, True)
    try:
        print("reading", param_src)
        with open(param_src, 'r') as src:
            print("src: ", src)
            for line in src:
                if line.startswith("#") or line.startswith(";") or line.startswith(" ") or line.startswith("\n"):
                    continue
                keyval = str.split(line, "=")
                #TODO more succinct way of doing this?
                k = str.strip(keyval[0])
                v = str.strip(keyval[1]) 
                sectiondict[k] = v
        print("completed reading INI source file")
        
        return (sectiondict, True)

    except Exception as e:
        print("readStdConfigFile() exception: %s" % e)

        return (sectiondict, False)


def loadStdParamsFromFile(thisconfig, testname, param_src):
    """ Load biosim4 parameters from an INI file and
        load them into the testing environment in 
        test config format.
        e.g. MaxGenerations -> param-maxgenerations 
        """

    sectiondict, res = readStdConfigFile(param_src)
    numparams = len(sectiondict)
    if not res:
        return False
    elif numparams == 0:
        print("file contains no parameters")
    print("got %s parameters" % numparams)

    section = getTestSection(thisconfig, testname)
    try:
        for k, v in sectiondict.items():
            #TODO check k against default ref params for validity
            k = "param-" + k
            section[k] = v

    except Exception as e:
        print("loadStdParams() exception: %s" % e)


def writeStdTestFile(thisconfig, testname):
    """ Write a biosim4 style INI file prior to running
        a sim test. Test-style params are converted to
        biosim4-style params before being written to file.
        """
    # see https://realpython.com/python-pathlib/
    # can also use methods to Path
    # e.g. .write_text(): open the path, write string data
    global TEMPinifile  # 'tmp.ini'

    s = getTestSection(thisconfig, testname)

    try:
        # get the absolute path to 'tmp.ini'
        pathstr = "./configs/%s" % TEMPinifile
        relpath = Path(pathstr)
        abspath = relpath.resolve()
        if checkFileExists(abspath, create=True):
            print("using temp file ", abspath)
            with open(abspath, 'w') as ini:
                for k, v in s.items():
                    if 'param' in k:
                        p = str.split(k, '-')
                        ini.write(p[1] + "= " + v + "\n")

    except Exception as e:
        print("writeTestFile() exception: %s" % e)

    
def runTest():
    """ Execute the biosim4 binary using the 'subprocess' module
        """

    global TEMPinifile

    relpath = "./tests/configs/%s" % TEMPinifile
    shellcmd = "./bin/Release/biosim4 %s" % relpath
    print("Running the simulation...\n")
    # launch biosim4
    process = subprocess.run(shellcmd, cwd='../', shell=True, capture_output=False, text=True, check=True)
    
    return process


def getResultParams(thisconfig, testname):
    """ Read the test app config file and filter for
        result parameters. Return a dictionary.
        """

    section = getTestSection(thisconfig, testname)
    resultdict = dict()
    for k, v in section.items():
        if 'result' in k:
            kk = str.split(k, "-", 1)  #only split at first "-"
            try:
                v = int(v)
            except:
                v = float(v)
            resultdict[kk[1]] = v
    if len(resultdict) == 7:
        return (resultdict, True)
    else:
        return (resultdict, False)

def updateResultParams(thisconfig, testname, addrpdict):

    section = getTestSection(thisconfig, testname)
    for k, v in addrpdict.items():
        section[k] = str(v)
    #thisconfig.write_config_setting(testname, k, v)


def showResultParams(resultdict):
    """ can be called after getResultParams() to see the result params in resultdict returned by getResultParams() 
    returns True after displaying dict items
    """
    # display params in dict 
    if len(resultdict) > 0:
        count = 0
        print("\nDefined result params:\n")
        for k, v in resultdict.items():
            print("%s= %s" % (k, v))
            count += 1
        print("\n%i result param(s) missing" % (7 - count))
    else:
        print("\nthis test has no result params defined")


def readLog(results_log):
    """ Read the biosim4 log file and return the last line.
        return a dictionary of results
        """
    logfile = '../logs/%s' % results_log
    with open(logfile, 'r') as log:
        for line in log:
            pass
        last_line = line
    try:
        reslist = str.split(last_line)
        resdict = {
            'generation' : int(reslist[0]),
            'survivors' : int(reslist[1]),
            'diversity' : float(reslist[2]),
            'genomeSize' : int(reslist[3]),
            'kills' : int(reslist[4])
        }
        return resdict
    except Exception as e:
        print("readLog() exception:\n%s" % e)
        return False


def resultsAnalysis(thisconfig, testname, results_log, adjust=False):
    """ result-generations = int
        result-survivors-min = int
        result-survivors-max = int
        result-diversity-min = float
        result-diversity-max = float
        result-genomesize = int
        result-kills = int
        """
    
    global addrpdict
    addrpdict = dict()

    def pad(thisobj, maxlen=16):
        """ calc diff between str(thisobj) and maxlen
            return diff number of space chars """
        sl = len(str(thisobj))
        return " " * (maxlen - sl)

    def add(key ,val, minmax=False):
        """ add key, val to global dictionary addrpdict
            if minmax == True then add 2 params """
        global addrpdict
        paramlist = list()
        if minmax:
            for suffix in ['-min', '-max']:
                newkey = 'result-' + key + suffix
                paramlist.append(newkey)
        else:
            paramlist.append('result-' + key)
        action = "adding"
        if adjust:
            action = "updating"
        for k in paramlist:
            print("%s param '%s = %s' to config" % (action, k, val))
            addrpdict[k] = val


    resdict = readLog(results_log)
    if not resdict:
        print("resultsAnalysis: Error in logfile %s" % results_log)
        print("unable to proceed with results analysis")
        exit(1)

    # expected results from result params in config file
    rp, complete = getResultParams(thisconfig, testname)
    addrp = False

    if not complete:
        # either:
        # 1) no results defined
        # 2) incomplete set of resultparams 
        print("incomplete set of results params")
        showResultParams(rp)
        # also, let's make our lives easy and avoid shooting at
        # moving targets - first establish a complete set of params
        # this means we can reuse the same global addrpdict (and 
        # its helper functions) for adding params during one pass
        # and then adjusting params during a subsequent pass
        if adjust:
            print("\n--adjust was requested but will be ignored while this test has an incomplete set of result params.\nthe script will now offer to add missing params to this test's configuration\n")
            adjust = False
        u = input("press any key...")
        print("\nYou can terminate script execution and manually update the test config file with the missing params, and then run this test again. Or proceed (recommended) and the missing result params will be added to this test's configuration\n")
        uadd = input("proceed and add result params? (Y/n) ")
        if uadd not in ["n", "N"]:
            addrp = True

    #TODO conditionsals below could be done with a loop that
    #   calls a fn() addOrUpdateResultParam()
    #TODO could use param 'maxgenerations' - 1 here, instead...
    #   diversity-min:\t expected\t actual\t %s
    #NOTE since Python 3.6 a dict() is insertion ordered, so
    #   resdict will consistently iterate actual results in
    #   expected order. nice
    try:
        count = 0
        # if --adjust was passed we will store params (& vals)
        # in this list to be confirmed for updating after
        # displaying all results
        adjlist = list()
        print("\n  parameter  \t  expected\t\t   actual\n")
        for k, v in resdict.items():
            # handle each actual result
            success = "Pass"
            rstr = ""   # result parameter string
            haverp = True
            minmax = False
            minstr = ""
            maxstr = ""
            # assign corresponding key name for rp dict()
            if k == 'generation':
                rstr = 'generations'
            elif k == 'genomeSize':
                rstr = 'genomesize'
            elif k == 'kills':
                rstr = 'kills'
            # now for some min/max cases
            elif k == 'survivors':
                rstr = 'survivors'
                minmax = True
            elif k == 'diversity':
                rstr = 'diversity'
                minmax = True

            if minmax:
                minstr = rstr + '-min'
                maxstr = rstr + '-max'
                if minstr in rp and maxstr in rp:
                    if v < rp[minstr] or v > rp[maxstr]:
                        success = "Fail"
                        count += 1
                        # --adjust
                        # store key and val as a tuple for
                        # processing after displaying to the user
                        if adjust and v < rp[minstr]:
                            adjlist.append((minstr, v))
                        elif adjust and v > rp[maxstr]:
                            adjlist.append((maxstr, v))
                            
                else:
                    haverp = False
                    count += 1
            # i.e. a regular equivalence case
            elif rstr in rp:
                if v != rp[rstr]:
                    success = "Fail"
                    count += 1
                    # --adjust
                    if adjust:
                        # store for processing after display output
                        adjlist.append((rstr, v))
            # fallthrough: rstr is not in dict rp
            else:
                haverp = False
                count += 1

            if addrp and not haverp:
                add(rstr, v, minmax)
                count += 1 if not minmax else 2
                continue  # don't display missing param

            # display:
            # if the user is not adding the missing result-param
            # to the dict rp, then we must handle displaying
            # its absence
            if not haverp:
                showrp = "NA"
                success = "Fail"
            else:
                if minmax:
                    showrp = "%s - %s" % (rp[minstr], rp[maxstr])
                else: 
                    showrp = rp[rstr]

            print("  %s%s %s%s %s%s %s" % (
                                            k, pad(k), 
                                            showrp, pad(showrp,24),
                                            v, pad(v),
                                            success)
            )
    except Exception as e:
        print("resultsAnalysis() exception:\n%s" % e)
        count -= 100

    # handle parameter val adjustments
    # whether adding or adjusting, process addrpdict
    # i.e. get param/val pairs from dict storage and set in config
    if addrp or adjust:
        if adjust and adjlist:
            # eventhough minmax might be True, pass 
            # minmax==False here since we would only be 
            # adjusting after building a complete set of
            # result params. Hence -min and -max params
            # arrive here singularly (k, v in adjlist)
            print()
            for t in adjlist:
                u = input("update %s = %s ? (Y/n)" % (t[0], t[1]))
                if u in ["n", "N"]:
                    continue
                add(t[0], t[1], False)

        if addrpdict:
            updateResultParams(thisconfig, testname, addrpdict)
            # return 'update' here so calling script can decide
            # whether to write to disk (config file) or another action 
            return 'update' 

    print("\ntest: %s\n" % ("Pass" if count==0 else "Fail"))

    if count > 0:
        print("\n%i errors\n" % count)
