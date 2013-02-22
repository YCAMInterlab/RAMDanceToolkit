import os
import subprocess

basedir = os.path.abspath('.')

def check(path):
	print path
	os.chdir(path)
	cmd = 'xcodebuild'
	subprocess.check_call(cmd, shell=True)

def check_dir(path):
	os.chdir(basedir)

	path = os.path.abspath(path)
	for i in os.listdir(path):
		p = os.path.join(path, i)
		if not os.path.isdir(p): continue
		check(p)

check_dir('../apps')
check_dir('../examples')