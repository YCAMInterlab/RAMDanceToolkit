require 'fileutils'
require 'find'

# !!!: task of this script
# 1. to replace all of resource dir path in source code
# 2. to copy ./resources to apps/RAMDanceToolkit/bin
# 3. to remove trash files
# 
# build new release binary after running this script,
# and check it works fine.
# 
# note that the source codes which is replaced by this script should not commit to develop/master branch!

ram_root_dir = "../../apps/RAMDanceToolkit/bin/"
FileUtils.cd(ram_root_dir, {:verbose => false})


# kill .DS_Store etc...
Find.find(".") {|f|
	  if File::basename(f) =~ /^\./
      FileUtils.rm(f, {:force=>true})
			p "removed: " + f
		end
}
