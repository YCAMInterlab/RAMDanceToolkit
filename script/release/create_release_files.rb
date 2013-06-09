require 'fileutils'
require 'find'

ram_root_dir = "../../"
FileUtils.cd(ram_root_dir, {:verbose => false})

release_files_path =
  "RAM-release-v1_0_1"

sources = [
  "addons",
  "apps",
  "examples",
  "libs",
  "resources",
  "readme.md"
  ]

ignore_files = [
  ".DS_Store",
  "preset.init.xml"
  ]


# copy sources files to release_file_path
Dir::mkdir(release_files_path, 0700) 

sources.each { |path|
  
  case File::ftype(path)
  when "directory"
      FileUtils.cp_r(path, release_files_path)
  when "file"
      FileUtils.cp(path, release_files_path)
  else
      puts "unknown file:" + path 
  end
}


# remove files that should be ignored e.g. .DS_Store
Find.find('./'+release_files_path) {|f|
  
  if File::basename(f) =~ /^.+?\.app$/
    p File::basename(f) + " found"
    p ".app deleted"
    FileUtils.rm_r(f, {:force=>true})
    next
  end

  ignore_files.each{ |ignore|
    if File::basename(f) == ignore
      FileUtils.rm(f, {:force=>true})
      next
    end
  }
}