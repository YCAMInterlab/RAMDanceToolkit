require 'fileutils'
require 'find'

# !!!: tasks of this script
# 1. replacing all of resource dir path in source code
# 2. copying ./resources to apps/RAMDanceToolkit/bin
# 3. removing trash files
# 
# build new release binary after running this script,
# and check it works fine.
# 
# note that the source codes which is replaced by this script should not be commited to our repos!

ram_root_dir = "../../"
FileUtils.cd(ram_root_dir, {:verbose => false})


resource_path_before = Regexp.new("..\/..\/..\/..\/resources")
resource_path_after = "../resources"
release_bin_path = "apps/RAMDanceToolkit/bin/"

ignore_files = [
  ".DS_Store",
  "preset.init.xml"
  ]


Find.find('.') {|path|
  
  next if File.directory?(path)
  next unless (File::extname(path) == '.h' || File::extname(path) == '.cpp')
  
  open(path){ |f|
    
    buffer = f.read()
    f.rewind
    replace_flag = false
    
    # logging
		while line = f.gets
		  if resource_path_before =~ line
        puts "["+path+"]"
        puts line + "\n"
        replace_flag = true
      end
		end
    
    next if (replace_flag == false)
    
    # replacing texts if needed
    buffer.gsub!(resource_path_before, resource_path_after);
    new_file = File.open(path, "w")
    new_file.write(buffer)
    new_file.close()
  }
}


# copy resources dir into ramdancetoolkit
FileUtils.cp_r("resources", release_bin_path+"resources", {:remove_destination=>true})
p "copied shared resources to RAMDanceToolkit/bin"


# kill .DS_Store etc...
Find.find('.') {|f|
	ignore_files.each{ |ignore|
		if File::basename(f) == ignore
			FileUtils.rm(f, {:force=>true})
			p "removed: " + f
			next
		end
	}
	if File::basename(f) =~ /^\./
		FileUtils.rm(f, {:force=>true})
		p "removed: " + f
	end
}
