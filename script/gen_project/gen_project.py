import os
import subprocess

for i in os.listdir('.'):
	if not os.path.isdir(i): continue
	cmd = 'premake4 --project-name=%s vs2010' % i
	subprocess.check_call(cmd, shell=True)
