# Tests

The _tests_ directory contains tools to test _biosim4_. Some tools are scripts, typically written in shell or Python. Other tools (like _rng/random_) need to be compiled.

## simtest.sh

This shell script starts a short simulation and evaluates _biosim4_ console output for both expected output and errors. The scripts uses _configs/biosim4-test-1.ini_ to configure simulation parameters. From the _tests_ working directory execute the following from a shell:

`./simtest.sh`

If this script reports any errors, search for this error in Issues.

## testapp.py

A Python script for testing a biosim4 simulation. It also includes some utility functions to manipulate .ini files and parameters.

testapp.py initializes a test environment and simulation parameters for a biosim4 simulation. Always execute testapp.py inside the '<project_root>/tests/' working directory. To see a list of configured tests, use the --show flag:
```python
python3 testapp.py --show
```

Run a sample simulation test:

    `python3 testapp.py --test microtest`

To see all available options and flags, run:
```python
    python3 testapp.py --help
```

### Simulation parameters

This script uses its own configuration file 'testapp.ini' for storing and retrieving two sets of parameters:

- simulation parameters, and
- expected simulation outcome results

Inspect './configs/testapp.ini' to see the predefined example 'quicktest'. This test's simulation and result parameters can also be output to the console:

    python3 testapp.py --test quicktest --params

> _Note: If no config file is specified, biosim4 uses the default parameters listed in biosim4.ini. Additionally, in the absence of the biosim4.ini config file, biosim4 will revert to hardcoded parameters that correspond to the values in defined in biosim4.ini. 

> When using testapp.py you can specify only the few parameters that differ from the default parameters defined in biosim4.ini. See the sample 'microtest' in ./configs/testapp.ini for an example.

> However, the caveat of this approach is that, should one or more default parameter values change in future, your test's results may deviate from expectation. Unless defined explicitly in your test configuration, biosim4 will substitute default parameter values for any test parameter that is missing. Consider this when creating and storing a test configuration.

### Configure a new test

1. Copy the default config file 'biosim4.ini' to the 'configs/' directory and rename the copy with the unique name of your new test:

    cp ../biosim4.ini ./configs/my_new_test.ini

2. Edit the new test's parameters in './configs/my_new_test.ini'

3. Import the test parameters into the test environment:

    python3 testapp.py --import my_new_test.ini

Simulation parameters (from .configs/my_new_test.ini) are converted to test-style format and stored in a new section 'my_new_test' in the configuration file './configs/testapp.ini'

After being imported, test parameters can be edited directly in the 'testapp.ini' file.

### Result parameters

Result parameters can be manually added to 'testapp.ini', or they can be initialed automatically by simply running a test simulation:

    python3 testapp.py --test my_new_test

After completing the simulation, the script will propose to add result parameters (using actual test result values) to the config file. These can then be manually edited to match variations in test results.

#### Finally

To see this documentation in the console run

```python
python3 testapp.py --usage 
```