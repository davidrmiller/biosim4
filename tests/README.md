# Tests

The _tests_ directory contains tools to test _biosim4_.

## testapp.py

A Python script for testing a biosim4 simulation. It also includes some utility functions to manipulate .ini files and simulation parameters.

testapp.py initializes a test environment and simulation parameters for a biosim4 simulation. Always execute testapp.py inside the _<project_root>/tests/_ working directory. To see a list of configured tests, use the --show flag:

```python
python3 testapp.py --show
```

Run a sample simulation test:

```python
python3 testapp.py --test microtest
```

To see all available options and flags, run:
```python
python3 testapp.py --help
```

### Simulation parameters

This script uses its own configuration file _testapp.ini_ for storing and retrieving two sets of parameters:

- simulation parameters, and
- expected simulation outcome results

Inspect _./configs/testapp.ini_ to see the predefined example _microtest_. This test's simulation and result parameters can also be output to the console:

```python
python3 testapp.py --test microtest --params
```

### How biosim4 sources parameters

If no config file is specified, biosim4 uses the default parameters listed in biosim4.ini. Additionally, in the absence of the biosim4.ini config file, biosim4 will revert to hardcoded parameters that correspond to the values defined in biosim4.ini. 

When using testapp.py you can specify only the few parameters that differ from the default parameters defined in biosim4.ini. See the sample _microtest_ in ./configs/testapp.ini for an example.

However, the caveat of this approach is that, __should one or more default parameter values change in future__, your __test results may deviate from expectation__. Unless defined explicitly in your test configuration, _biosim4 will substitute default parameter values for any test parameter that is missing_.

Consider this when creating and storing a test configuration.

### Configure a new test

1. Copy the default config file _biosim4.ini_ to the _configs/_ directory and rename the copy with the unique name of your new test:

```bash
cp ../biosim4.ini ./configs/my_new_test.ini
```

2. Edit the new test's parameters in _./configs/my_new_test.ini_

3. Load (import) the test parameters into the test environment:

```python
python3 testapp.py --load my_new_test.ini
```

Simulation parameters (from _.configs/my_new_test.ini_) are converted to test-style format and stored in a new section _my_new_test_ in the configuration file _./configs/testapp.ini_

After being imported, test parameters can be edited directly in the _testapp.ini_ file.

### Result parameters

Result parameters can be manually added to _testapp.ini_, or they can be initialized automatically by simply running a test simulation:

```python
python3 testapp.py --test my_new_test
```

After completing the simulation, the script will propose to add result parameters (using actual test result values) to the config file. These can then be manually edited to match variations in test results. 

Alternatively, use the --adjust flag to have _testapp.py_ adjust parameters in response to simulation results. 

```python
python3 testapp.py -t my_new_test --adjust
```

Running the test several times with --adjust should establish crude ranges for -min/-max result parameter values.

#### Finally

To see this documentation in the console run

```python
python3 testapp.py --usage 
```
