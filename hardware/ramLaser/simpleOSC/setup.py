#!/usr/bin/env python

# please report errors to info@ixi-software.net
# to install :
# python setup.py install

# create a distribution :
# python setup.py sdist

#create win exe installer :
# python setup.py bdist_wininst




from distutils.core import setup





setup(name = 'SimpleOSC',
    version = '0.3',
    description = 'SimpleOSC is a very basic API that wraps pyOSC',
    license = 'LGPL',
    author = 'ixi audio',
    author_email = 'info@ixi-audio.net',
    url = 'http://www.ixi-audio.net/backyard',

    #packages = ['simpleOSC']
    py_modules = ['simpleOSC']
)







