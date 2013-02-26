SimpleOSC 0.3.2

ixi audio - 13/12/2012

contact us on www.ixi-audio.net
info@ixi-audio.net


Install :
in the command line / terminal type 'python setup.py install'. In Unix systems you might need to have root permissions. Windows users can also just copy the simpleOSC.py file into C:/python/2.x/Lib/site-packages


Description:
The main aim is to provide with a simple way to deal with the OSC implementation that makes life easier to newcomers or students. Therefore SimpleOSC is a very simple (and *limited*) API for the Open Sound Control for Python (by V2 --> https://trac.v2.nl/wiki/pyOSC). You must have pyOSC installed for this module to run. We provide with functions to perform the most basic OSC send and receive.

Previous versions of simpleOSC were based on OSC.py by Daniel Holth, Clinton McChesney.


How to use:
First you need to download and install pyOSC from https://trac.v2.nl/wiki/pyOSC
Once you have that installed you can start checking the examples provided in SimpleOSC. You can install the module with python setup.py install or you can just place the simpleOSC.py file next to your files
or you copy it manually into the python site-packages folder (if you know where this lives in your system. Note that firewalls and routers tend to block nerwork ports for security. Make sure you use ports that are open for the OSC comunication.

For more details on simpleOSC check its documentation.


Download page:
www.ixi-software.net/download/simpleosc.html
or go to www.ixi-software.net and get into backyard/code section


License : 
 This library is free software; you can redistribute it and/or modify it under the terms of the Lesser GNU, Lesser General Public License as published by the Free Software Foundation.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

SimpleOSC contains parts by others such as OSC.py by V2. Licence and credits are included on those parts from others.


System requirements:
OS X, GNU/Linux, Windows .... with Python and pyOSC installed


About OSC:
http://cnmat.cnmat.berkeley.edu/OSC/



Changes

0.3.2
- added startOSCServer to solve a weird error that just pop up, example updated as well

0.3.1
- updating example 

0.3
- moved from old OSC.py by Daniel Holth and Clinton McChesney to latest OSC.py by V2
So everything changes ....


----------- old OSC.py below ----------

0.2.7
- patch for OSCArgument() in OSC.py by ondeosfrangosnaotemvez. Checks if the type
of variable is a unicode string, and treats them as a normal string


0.2.6
- thread listener improved by Antony Templier (thanks!)


0.2.5.1
- removed self.socket.settimeout(1.0) from OSCServer class,
 this apparently caused an error on mac

0.2.5
- added threads to listener area thanks to Christopher Frauenberger.

0.2.4
- added double values thanks to Christopher Frauenberger

0.2.3.2
- ChucK example added.

0.2.3.1
- array in sendMsg takes default value to [] to allow for this type of messages
osc.sendMsg('/quit')

0.2.3
- some optimisation. got rid of some variables and functions

0.2.2
- added setup.py

0.2.1
- it sends by default to local host "127.0.0.1" ip and port 9000
- it receives by default from local host "127.0.0.1" ip and port 9001

0.2
- in order to make it simpler to use simpleOSC the callBackManager and ouSockets are now
global variables in the oscAPI module so that the user does not have to deal with them. This
makes the API cleaner and more compact. 

0.1.3
- switched licence to LGPL.

0.1.2
- some tiding up

0.1.1
- removed oscController.py to make it more general
- osc.py and oscAPI.py are now a package to make it more compact
- included latest version of OSC.py 





