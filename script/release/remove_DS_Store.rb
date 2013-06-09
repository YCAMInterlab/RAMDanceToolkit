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

ram_root_dir = "../../"
FileUtils.cd(ram_root_dir, {:verbose => false})


resource_path_before = Regexp.new("..\/..\/..\/..\/resources")
resource_path_after = "../resources"
release_bin_path = "apps/RAMDanceToolkit/bin/"

ignore_files = [
  ".DS_Store",
  "preset.init.xml"
  ]


# kill .DS_Store etc...
Find.find('.') {|f|
  ignore_files.each{ |ignore|
    if File::basename(f) == ignore
      FileUtils.rm(f, {:force=>true})
      next
    end
  }
}
